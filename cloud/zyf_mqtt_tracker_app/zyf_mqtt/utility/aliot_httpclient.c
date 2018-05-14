/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <string.h>
#include <stddef.h>
#include "aliot_platform.h"
#include "aliot_timer.h"
#include "aliot_error.h"
#include "aliot_log.h"
#include "aliot_httpclient.h"

#define HTTPCLIENT_MIN(x,y) (((x)<(y))?(x):(y))
#define HTTPCLIENT_MAX(x,y) (((x)>(y))?(x):(y))

#define HTTPCLIENT_AUTHB_SIZE     128

#define HTTPCLIENT_CHUNK_SIZE     500
#define HTTPCLIENT_SEND_BUF_SIZE  500

#define HTTPCLIENT_MAX_HOST_LEN   64
#define HTTPCLIENT_MAX_URL_LEN    500

#define HTTP_RETRIEVE_MORE_DATA   (1)            /**< More data needs to be retrieved. */

#if defined(MBEDTLS_DEBUG_C)
    #define DEBUG_LEVEL 2
#endif

static int httpclient_parse_host(const char *url, char *host, uint32_t maxhost_len);
static int httpclient_parse_url(const char *url, char *scheme, uint32_t max_scheme_len, char *host,
                                uint32_t maxhost_len, int *port, char *path, uint32_t max_path_len);
static int httpclient_conn(httpclient_t *client);
static int httpclient_recv(httpclient_t *client, char *buf, int min_len, int max_len, int *p_read_len, uint32_t timeout);
static int httpclient_retrieve_content(httpclient_t *client, char *data, int len, uint32_t timeout, httpclient_data_t *client_data);
static int httpclient_response_parse(httpclient_t *client, char *data, int len, uint32_t timeout, httpclient_data_t *client_data);

static void httpclient_base64enc(char *out, const char *in)
{
    const char code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    int i = 0, x = 0, l = 0;

    for (; *in; in++) {
        x = x << 8 | *in;
        for (l += 8; l >= 6; l -= 6) {
            out[i++] = code[(x >> (l - 6)) & 0x3f];
        }
    }
    if (l > 0) {
        x <<= 6 - l;
        out[i++] = code[x & 0x3f];
    }
    for (; i % 4;) {
        out[i++] = '=';
    }
    out[i] = '\0';
}

int httpclient_conn(httpclient_t *client)
{
    if (0 != client->net.connect(&client->net)) {
        ALIOT_LOG_ERROR("establish connection failed");
        return ERROR_HTTP_CONN;
    }

    return SUCCESS_RETURN;
}

int httpclient_parse_url(const char *url, char *scheme, uint32_t max_scheme_len, char *host, uint32_t maxhost_len,
                         int *port, char *path, uint32_t max_path_len)
{
    char *scheme_ptr = (char *) url;
    char *host_ptr = (char *) strstr(url, "://");
    uint32_t host_len = 0;
    uint32_t path_len;
    //char *port_ptr;
    char *path_ptr;
    char *fragment_ptr;

    if (host_ptr == NULL) {
        ALIOT_LOG_ERROR("Could not find host");
        return ERROR_HTTP_PARSE; /* URL is invalid */
    }

    if (max_scheme_len < host_ptr - scheme_ptr + 1) {
        /* including NULL-terminating char */
        ALIOT_LOG_ERROR("Scheme str is too small (%u >= %u)", max_scheme_len, (uint32_t)(host_ptr - scheme_ptr + 1));
        return ERROR_HTTP_PARSE;
    }
    memcpy(scheme, scheme_ptr, host_ptr - scheme_ptr);
    scheme[host_ptr - scheme_ptr] = '\0';

    host_ptr += 3;

    *port = 0;

    path_ptr = strchr(host_ptr, '/');
    if (NULL == path_ptr) {
        ALIOT_LOG_ERROR("invalid path");
        return -1;
    }
    
    if (host_len == 0) {
        host_len = path_ptr - host_ptr;
    }

    if (maxhost_len < host_len + 1) {
        /* including NULL-terminating char */
        ALIOT_LOG_ERROR("Host str is too small (%d >= %d)", maxhost_len, host_len + 1);
        return ERROR_HTTP_PARSE;
    }
    memcpy(host, host_ptr, host_len);
    host[host_len] = '\0';

    fragment_ptr = strchr(host_ptr, '#');
    if (fragment_ptr != NULL) {
        path_len = fragment_ptr - path_ptr;
    } else {
        path_len = STRLEN(path_ptr);
    }

    if (max_path_len < path_len + 1) {
        /* including NULL-terminating char */
        ALIOT_LOG_ERROR("Path str is too small (%d >= %d)", max_path_len, path_len + 1);
        return ERROR_HTTP_PARSE;
    }
    memcpy(path, path_ptr, path_len);
    path[path_len] = '\0';

    return SUCCESS_RETURN;
}

int httpclient_parse_host(const char *url, char *host, uint32_t maxhost_len)
{
    const char *host_ptr = (const char *) strstr(url, "://");
    uint32_t host_len = 0;
    char *path_ptr;

    if (host_ptr == NULL) {
        ALIOT_LOG_ERROR("Could not find host");
        return ERROR_HTTP_PARSE; /* URL is invalid */
    }
    host_ptr += 3;

    path_ptr = strchr(host_ptr, '/');
    if (host_len == 0) {
        host_len = path_ptr - host_ptr;
    }

    if (maxhost_len < host_len + 1) {
        /* including NULL-terminating char */
        ALIOT_LOG_ERROR("Host str is too small (%d >= %d)", maxhost_len, host_len + 1);
        return ERROR_HTTP_PARSE;
    }
    memcpy(host, host_ptr, host_len);
    host[host_len] = '\0';

    return SUCCESS_RETURN;
}

int httpclient_get_info(httpclient_t *client, char *send_buf, int *send_idx, char *buf,
                        uint32_t len) /* 0 on success, err code on failure */
{
    int ret;
    int cp_len;
    int idx = *send_idx;

    if (len == 0) {
        len = STRLEN(buf);
    }

    do {
        if ((HTTPCLIENT_SEND_BUF_SIZE - idx) >= len) {
            cp_len = len;
        } else {
            cp_len = HTTPCLIENT_SEND_BUF_SIZE - idx;
        }

        memcpy(send_buf + idx, buf, cp_len);
        idx += cp_len;
        len -= cp_len;

        if (idx == HTTPCLIENT_SEND_BUF_SIZE) {
            //            if (client->remote_port == HTTPS_PORT)
            //            {
            //                WRITE_IOT_ERROR_LOG("send buffer overflow");
            //                return ERROR_HTTP;
            //            }
            //ret = httpclient_tcp_send_all(client->handle, send_buf, HTTPCLIENT_SEND_BUF_SIZE);
            ret = client->net.write(&client->net, send_buf, HTTPCLIENT_SEND_BUF_SIZE, 5000);
            if (ret) {
                return (ret);
            }
        }
    } while (len);

    *send_idx = idx;
    return SUCCESS_RETURN;
}

void httpclient_set_custom_header(httpclient_t *client, char *header)
{
    client->header = header;
}

int httpclient_basic_auth(httpclient_t *client, char *user, char *password)
{
    if ((STRLEN(user) + STRLEN(password)) >= HTTPCLIENT_AUTHB_SIZE) {
        return ERROR_HTTP;
    }
    client->auth_user = user;
    client->auth_password = password;
    return SUCCESS_RETURN;
}

int httpclient_send_auth(httpclient_t *client, char *send_buf, int *send_idx)
{
    char b_auth[(int)((HTTPCLIENT_AUTHB_SIZE + 3) * 4 / 3 + 1)];
    char base64buff[HTTPCLIENT_AUTHB_SIZE + 3];

    httpclient_get_info(client, send_buf, send_idx, "Authorization: Basic ", 0);
    SPRINTF(base64buff, "%s:%s", client->auth_user, client->auth_password);
    ALIOT_LOG_DEBUG("bAuth: %s", base64buff) ;
    httpclient_base64enc(b_auth, base64buff);
    b_auth[STRLEN(b_auth) + 1] = '\0';
    b_auth[STRLEN(b_auth)] = '\n';
    ALIOT_LOG_DEBUG("b_auth:%s", b_auth) ;
    httpclient_get_info(client, send_buf, send_idx, b_auth, 0);
    return SUCCESS_RETURN;
}

int httpclient_send_header(httpclient_t *client, const char *url, int method, httpclient_data_t *client_data)
{
    char scheme[8] = { 0 };
    char host[HTTPCLIENT_MAX_HOST_LEN] = { 0 };
    char path[HTTPCLIENT_MAX_URL_LEN] = { 0 };
    int len;
    char send_buf[HTTPCLIENT_SEND_BUF_SIZE] = { 0 };
    char buf[HTTPCLIENT_SEND_BUF_SIZE] = { 0 };
    char *meth = (method == HTTPCLIENT_GET) ? "GET" : (method == HTTPCLIENT_POST) ? "POST" :
                 (method == HTTPCLIENT_PUT) ? "PUT" : (method == HTTPCLIENT_DELETE) ? "DELETE" :
                 (method == HTTPCLIENT_HEAD) ? "HEAD" : "";
    int ret;
    int port;

    /* First we need to parse the url (http[s]://host[:port][/[path]]) */
    //int res = httpclient_parse_url(url, scheme, sizeof(scheme), host, sizeof(host), &(client->remote_port), path, sizeof(path));
    int res = httpclient_parse_url(url, scheme, sizeof(scheme), host, sizeof(host), &port, path, sizeof(path));
    if (res != SUCCESS_RETURN) {
        ALIOT_LOG_ERROR("httpclient_parse_url returned %d", res);
        return res;
    }

    //if (client->remote_port == 0)
    //{
    if (STRCMP(scheme, "http") == 0) {
        //client->remote_port = HTTP_PORT;
    } else if (STRCMP(scheme, "https") == 0) {
        //client->remote_port = HTTPS_PORT;
    }
    //}

    /* Send request */
    MEMSET(send_buf, 0, HTTPCLIENT_SEND_BUF_SIZE);
    len = 0; /* Reset send buffer */

    SNPRINTF(buf, sizeof(buf), "%s %s HTTP/1.1\r\nHost: %s\r\n", meth, path, host); /* Write request */
    ret = httpclient_get_info(client, send_buf, &len, buf, STRLEN(buf));
    if (ret) {
        ALIOT_LOG_ERROR("Could not write request");
        return ERROR_HTTP_CONN;
    }

    /* Send all headers */
    if (client->auth_user) {
        httpclient_send_auth(client, send_buf, &len); /* send out Basic Auth header */
    }

    /* Add user header information */
    if (client->header) {
        httpclient_get_info(client, send_buf, &len, (char *) client->header, STRLEN(client->header));
    }

    if (client_data->post_buf != NULL) {
        SNPRINTF(buf, sizeof(buf), "Content-Length: %d\r\n", client_data->post_buf_len);
        httpclient_get_info(client, send_buf, &len, buf, STRLEN(buf));

        if (client_data->post_content_type != NULL) {
            SNPRINTF(buf, sizeof(buf), "Content-Type: %s\r\n", client_data->post_content_type);
            httpclient_get_info(client, send_buf, &len, buf, STRLEN(buf));
        }
    }

    /* Close headers */
    httpclient_get_info(client, send_buf, &len, "\r\n", 0);

    ALIOT_LOG_DEBUG("Trying to write %d bytes http header:%s", len, send_buf);

    //ret = httpclient_tcp_send_all(client->net.handle, send_buf, len);
    ret = client->net.write(&client->net, send_buf, len, 5000);
    if (ret > 0) {
        ALIOT_LOG_DEBUG("Written %d bytes", ret);
    } else if (ret == 0) {
        ALIOT_LOG_ERROR("ret == 0,Connection was closed by server");
        return ERROR_HTTP_CLOSED; /* Connection was closed by server */
    } else {
        ALIOT_LOG_ERROR("Connection error (send returned %d)", ret);
        return ERROR_HTTP_CONN;
    }

    return SUCCESS_RETURN;
}

int httpclient_send_userdata(httpclient_t *client, httpclient_data_t *client_data)
{
    int ret = 0;

    if (client_data->post_buf && client_data->post_buf_len) {
        ALIOT_LOG_DEBUG("client_data->post_buf:%s", client_data->post_buf);
        {
            //ret = httpclient_tcp_send_all(client->handle, (char *)client_data->post_buf, client_data->post_buf_len);
            ret = client->net.write(&client->net, (char *)client_data->post_buf, client_data->post_buf_len, 5000);
            if (ret > 0) {
                ALIOT_LOG_DEBUG("Written %d bytes", ret);
            } else if (ret == 0) {
                ALIOT_LOG_ERROR("ret == 0,Connection was closed by server");
                return ERROR_HTTP_CLOSED; /* Connection was closed by server */
            } else {
                ALIOT_LOG_ERROR("Connection error (send returned %d)", ret);
                return ERROR_HTTP_CONN;
            }
        }
    }

    return SUCCESS_RETURN;
}

 /* 0 on success, err code on failure */
int httpclient_recv(httpclient_t *client, char *buf, int min_len, int max_len, int *p_read_len, uint32_t timeout_ms)
{
    int ret = 0;
    aliot_time_t timer;

    aliot_time_init(&timer);
    aliot_time_cutdown(&timer, timeout_ms);

    *p_read_len = 0;

    ret = client->net.read(&client->net, buf, max_len, aliot_time_left(&timer));
    if (ret > 0) {
        *p_read_len = ret;
    } else if (ret == 0) {
        //timeout
        return 0;
    } else if (-1 == ret) {
        ALIOT_LOG_ERROR("Connection closed.");
        return ERROR_HTTP_CONN;
    } else {
        ALIOT_LOG_ERROR("Connection error (recv returned %d)", ret);
        return ERROR_HTTP_CONN;
    }
    ALIOT_LOG_INFO("%u bytes be read", *p_read_len);
    return 0;

//    while (readLen <= min_len) {
//        buf[readLen] = '\0';
//        if (readLen < min_len) {
//            //wait to read HTTP respond data
//            ret = client->net.read(&client->net, buf + readLen, min_len - readLen, aliot_timer_remain(&timer));
//        } else {
//            //read the rest data in TCP buffer (with little wait time)
//            ret = client->net.read(&client->net, buf + readLen, max_len - readLen, 100);
//        }
//
//        if (ret > 0) {
//            readLen += ret;
//        } else if (ret == 0) {
//            //timeout
//            break;
//        } else if (-1 == ret) {
//            ALIOT_LOG_INFO("Connection closed. %u bytes be read", readLen);
//            break;
//        } else {
//            ALIOT_LOG_ERROR("Connection error (recv returned %d)", ret);
//            return ERROR_HTTP_CONN;
//        }
//    }
//
//    ALIOT_LOG_INFO("%u bytes be read", readLen);
//    *p_read_len = readLen;
//    return 0;
}

int httpclient_retrieve_content(httpclient_t *client, char *data, int len, uint32_t timeout_ms, httpclient_data_t *client_data)
{
    int count = 0;
    int templen = 0;
    int crlf_pos;
    aliot_time_t timer;

    aliot_time_init(&timer);
    aliot_time_cutdown(&timer, timeout_ms);

    /* Receive data */
    ALIOT_LOG_DEBUG("Receiving data:%s", data);
    client_data->is_more = true;

    if (client_data->response_content_len == -1 && client_data->is_chunked == false) {
        while (true) {
            int ret, max_len;
            if (count + len < client_data->response_buf_len - 1) {
                memcpy(client_data->response_buf + count, data, len);
                count += len;
                client_data->response_buf[count] = '\0';
            } else {
                memcpy(client_data->response_buf + count, data, client_data->response_buf_len - 1 - count);
                client_data->response_buf[client_data->response_buf_len - 1] = '\0';
                return HTTP_RETRIEVE_MORE_DATA;
            }

            max_len = HTTPCLIENT_MIN(HTTPCLIENT_CHUNK_SIZE - 1, client_data->response_buf_len - 1 - count);
            ret = httpclient_recv(client, data, 1, max_len, &len, aliot_time_left(&timer));

            /* Receive data */
            ALIOT_LOG_DEBUG("data len: %d %d", len, count);

            if (ret == ERROR_HTTP_CONN) {
                ALIOT_LOG_ERROR("ret == ERROR_HTTP_CONN");
                return ret;
            }

            if (len == 0) {
                /* read no more data */
                ALIOT_LOG_ERROR("no more len == 0");
                client_data->is_more = false;
                return SUCCESS_RETURN;
            }
        }
    }

    while (true) {
        uint32_t readLen = 0;

        if (client_data->is_chunked && client_data->retrieve_len <= 0) {
            /* Read chunk header */
            bool foundCrlf;
            int n;
            do {
                foundCrlf = false;
                crlf_pos = 0;
                data[len] = 0;
                if (len >= 2) {
                    for (; crlf_pos < len - 2; crlf_pos++) {
                        if (data[crlf_pos] == '\r' && data[crlf_pos + 1] == '\n') {
                            foundCrlf = true;
                            break;
                        }
                    }
                }
                if (!foundCrlf) {
                    /* Try to read more */
                    if (len < HTTPCLIENT_CHUNK_SIZE) {
                        int new_trf_len, ret;
                        ret = httpclient_recv(client,
                                    data + len,
                                    0,
                                    HTTPCLIENT_CHUNK_SIZE - len - 1,
                                    &new_trf_len,
                                    aliot_time_left(&timer));
                        len += new_trf_len;
                        if (ret == ERROR_HTTP_CONN) {
                            return ret;
                        } else {
                            continue;
                        }
                    } else {
                        return ERROR_HTTP;
                    }
                }
            } while (!foundCrlf);
            data[crlf_pos] = '\0';
           n = SSCANF(data, "%x", &readLen);/* chunk length */
            client_data->retrieve_len = readLen;
            client_data->response_content_len += client_data->retrieve_len;
            if (n != 1) {
                ALIOT_LOG_ERROR("Could not read chunk length");
                return ERRO_HTTP_UNRESOLVED_DNS;
            }

            MEMMOVE(data, &data[crlf_pos + 2], len - (crlf_pos + 2)); /* Not need to move NULL-terminating char any more */
            len -= (crlf_pos + 2);

            if (readLen == 0) {
                /* Last chunk */
                client_data->is_more = false;
                ALIOT_LOG_DEBUG("no more (last chunk)");
                break;
            }
        } else {
            readLen = client_data->retrieve_len;
        }

        ALIOT_LOG_DEBUG("Retrieving %d bytes, len:%d", readLen, len);

        do {
            templen = HTTPCLIENT_MIN(len, readLen);
            if (count + templen < client_data->response_buf_len - 1) {
                memcpy(client_data->response_buf + count, data, templen);
                count += templen;
                client_data->response_buf[count] = '\0';
                client_data->retrieve_len -= templen;
            } else {
                memcpy(client_data->response_buf + count, data, client_data->response_buf_len - 1 - count);
                client_data->response_buf[client_data->response_buf_len - 1] = '\0';
                client_data->retrieve_len -= (client_data->response_buf_len - 1 - count);
                return HTTP_RETRIEVE_MORE_DATA;
            }

            if (len > readLen) {
                ALIOT_LOG_DEBUG("MEMMOVE %d %d %d\n", readLen, len, client_data->retrieve_len);
                MEMMOVE(data, &data[readLen], len - readLen); /* chunk case, read between two chunks */
                len -= readLen;
                readLen = 0;
                client_data->retrieve_len = 0;
            } else {
                readLen -= len;
            }

            if (readLen) {
                int ret;
                int max_len = HTTPCLIENT_MIN(HTTPCLIENT_CHUNK_SIZE - 1, client_data->response_buf_len - 1 - count);
                max_len = HTTPCLIENT_MIN(max_len, readLen);
                ret = httpclient_recv(client, data, 1, max_len, &len, aliot_time_left(&timer));
                if (ret == ERROR_HTTP_CONN) {
                    return ret;
                }
            }
        } while (readLen);

        if (client_data->is_chunked) {
            if (len < 2) {
                int new_trf_len, ret;
                /* Read missing chars to find end of chunk */
                ret = httpclient_recv(client, data + len, 2 - len, HTTPCLIENT_CHUNK_SIZE - len - 1, &new_trf_len, aliot_time_left(&timer));
                if (ret == ERROR_HTTP_CONN) {
                    return ret;
                }
                len += new_trf_len;
            }
            if ((data[0] != '\r') || (data[1] != '\n')) {
                ALIOT_LOG_ERROR("Format error, %s", data); /* after MEMMOVE, the beginning of next chunk */
                return ERRO_HTTP_UNRESOLVED_DNS;
            }
            MEMMOVE(data, &data[2], len - 2); /* remove the \r\n */
            len -= 2;
        } else {
            ALIOT_LOG_DEBUG("no more(content-length)\n");
            client_data->is_more = false;
            break;
        }

    }

    return SUCCESS_RETURN;
}

int httpclient_response_parse(httpclient_t *client, char *data, int len, uint32_t timeout_ms, httpclient_data_t *client_data)
{
    int crlf_pos;
    aliot_time_t timer;

    aliot_time_init(&timer);
    aliot_time_cutdown(&timer, timeout_ms);

    client_data->response_content_len = -1;

    char *crlf_ptr = strstr(data, "\r\n");
    if (crlf_ptr == NULL) {
        ALIOT_LOG_ERROR("\r\n not found");
        return ERRO_HTTP_UNRESOLVED_DNS;
    }

    crlf_pos = crlf_ptr - data;
    data[crlf_pos] = '\0';

    /* Parse HTTP response */
   if (SSCANF(data, "HTTP/%*d.%*d %d %*[^\r\n]", &(client->response_code)) != 1) {
        /* Cannot match string, error */
        ALIOT_LOG_ERROR("Not a correct HTTP answer : %s\n", data);
        return ERRO_HTTP_UNRESOLVED_DNS;
    }

    if ((client->response_code < 200) || (client->response_code >= 400)) {
        /* Did not return a 2xx code; TODO fetch headers/(&data?) anyway and implement a mean of writing/reading headers */
        ALIOT_LOG_WARN("Response code %d", client->response_code);
    }

    ALIOT_LOG_DEBUG("Reading headers%s", data);

    MEMMOVE(data, &data[crlf_pos + 2], len - (crlf_pos + 2) + 1); /* Be sure to move NULL-terminating char as well */
    len -= (crlf_pos + 2);

    client_data->is_chunked = false;

    /* Now get headers */
    while (true) {
        char key[32];
        char value[32];
        int n;

        key[31] = '\0';
        value[31] = '\0';

        crlf_ptr = strstr(data, "\r\n");
        if (crlf_ptr == NULL) {
            if (len < HTTPCLIENT_CHUNK_SIZE - 1) {
                int new_trf_len, ret;
                ret = httpclient_recv(client, data + len, 1, HTTPCLIENT_CHUNK_SIZE - len - 1, &new_trf_len, aliot_time_left(&timer));
                len += new_trf_len;
                data[len] = '\0';
                ALIOT_LOG_DEBUG("Read %d chars; In buf: [%s]", new_trf_len, data);
                if (ret == ERROR_HTTP_CONN) {
                    return ret;
                } else {
                    continue;
                }
            } else {
                ALIOT_LOG_DEBUG("header len > chunksize");
                return ERROR_HTTP;
            }
        }

        crlf_pos = crlf_ptr - data;
        if (crlf_pos == 0) {
            /* End of headers */
            MEMMOVE(data, &data[2], len - 2 + 1); /* Be sure to move NULL-terminating char as well */
            len -= 2;
            break;
        }

        data[crlf_pos] = '\0';

        n = SSCANF(data, "%31[^:]: %31[^\r\n]", key, value);
        if (n == 2) {
            ALIOT_LOG_DEBUG("Read header : %s: %s", key, value);
            if (!STRCMP(key, "Content-Length")) {
                SSCANF(value, "%d", &(client_data->response_content_len));
                client_data->retrieve_len = client_data->response_content_len;
            } else if (!STRCMP(key, "Transfer-Encoding")) {
                if (!STRCMP(value, "Chunked") || !STRCMP(value, "chunked")) {
                    client_data->is_chunked = true;
                    client_data->response_content_len = 0;
                    client_data->retrieve_len = 0;
                }
            }
            MEMMOVE(data, &data[crlf_pos + 2], len - (crlf_pos + 2) + 1); /* Be sure to move NULL-terminating char as well */
            len -= (crlf_pos + 2);

        } else {
            ALIOT_LOG_ERROR("Could not parse header");
            return ERROR_HTTP;
        }
    }

    return httpclient_retrieve_content(client, data, len, aliot_time_left(&timer), client_data);
}

aliot_err_t httpclient_connect(httpclient_t *client)
{
    int ret = ERROR_HTTP_CONN;

    client->net.handle = 0;

    ret = httpclient_conn(client);
    //    if (0 == ret)
    //    {
    //        client->remote_port = HTTP_PORT;
    //    }

    return ret;
}

aliot_err_t httpclient_send_request(httpclient_t *client, const char *url, int method, httpclient_data_t *client_data)
{
    int ret = ERROR_HTTP_CONN;

    if (0 == client->net.handle) {
        ALIOT_LOG_DEBUG("not connection have been established");
        return ret;
    }

    ret = httpclient_send_header(client, url, method, client_data);
    if (ret != 0) {
        ALIOT_LOG_ERROR("httpclient_send_header is error,ret = %d", ret);
        return ret;
    }

    if (method == HTTPCLIENT_POST || method == HTTPCLIENT_PUT) {
        ret = httpclient_send_userdata(client, client_data);
    }

    return ret;
}

aliot_err_t httpclient_recv_response(httpclient_t *client, uint32_t timeout_ms, httpclient_data_t *client_data)
{
    int reclen = 0, ret = ERROR_HTTP_CONN;
    char buf[HTTPCLIENT_CHUNK_SIZE] = { 0 };
    aliot_time_t timer;

    aliot_time_init(&timer);
    aliot_time_cutdown(&timer, timeout_ms);

    if (0 == client->net.handle) {
        ALIOT_LOG_DEBUG("not connection have been established");
        return ret;
    }

    if (client_data->is_more) {
        client_data->response_buf[0] = '\0';
        ret = httpclient_retrieve_content(client, buf, reclen, aliot_time_left(&timer), client_data);
    } else {
        ret = httpclient_recv(client, buf, 1, HTTPCLIENT_CHUNK_SIZE - 1, &reclen, aliot_time_left(&timer));
        if (ret != 0) {
            return ret;
        }

        buf[reclen] = '\0';

        if (reclen) {
            ALIOT_LOG_DEBUG("reclen:%d, buf: %s", reclen, buf);
            ret = httpclient_response_parse(client, buf, reclen, aliot_time_left(&timer), client_data);
        }
    }

    return ret;
}

void httpclient_close(httpclient_t *client)
{

    if (client->net.handle > 0) {
        client->net.disconnect(&client->net);
    }
    client->net.handle = 0;
}

int httpclient_common(httpclient_t *client, const char *url, int port, const char *ca_crt, int method, uint32_t timeout_ms,
                      httpclient_data_t *client_data)
{
    aliot_time_t timer;
    int ret = ERROR_HTTP_CONN;
    char host[HTTPCLIENT_MAX_HOST_LEN] = { 0 };

    aliot_time_init(&timer);
    aliot_time_cutdown(&timer, timeout_ms);

    httpclient_parse_host(url, host, sizeof(host));
    ALIOT_LOG_DEBUG("host:%s, port:%d", host, port);

    aliot_net_init(&client->net, host, port, ca_crt);

    ret = httpclient_connect(client);
    if (0 != ret) {
        ALIOT_LOG_ERROR("httpclient_connect is error,ret = %d", ret);
        httpclient_close(client);
        return ret;
    }

    ret = httpclient_send_request(client, url, method, client_data);
    if (0 != ret) {
        ALIOT_LOG_ERROR("httpclient_send_request is error,ret = %d", ret);
        httpclient_close(client);
        return ret;
    }

    ret = httpclient_recv_response(client, aliot_time_left(&timer), client_data);
    if (0 != ret) {
        ALIOT_LOG_ERROR("httpclient_recv_response is error,ret = %d", ret);
        httpclient_close(client);
        return ret;
    }

    httpclient_close(client);
    return ret;
}

int aliot_get_response_code(httpclient_t *client)
{
    return client->response_code;
}

aliot_err_t aliot_post(
            httpclient_t *client,
            const char *url,
            int port,
            const char *ca_crt,
            uint32_t timeout_ms,
            httpclient_data_t *client_data)
{
    return httpclient_common(client, url, port, ca_crt, HTTPCLIENT_POST, timeout_ms, client_data);
}

