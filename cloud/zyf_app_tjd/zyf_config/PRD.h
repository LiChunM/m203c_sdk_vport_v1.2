#ifndef	 _PRD_H_  
#define 	_PRD_H_ 
#include "ql_type.h" 
#include "pro.h" 
/*The necessary information*/
#define USEPASSWORD  0
#define PASSWORD		 "123456789"
#define PRODUCTKEY		"727e412bb612"
/*The Dynamic TLV Info */
#define READONLYNUM	  8
extern tlv AutomaticRDTlv[READONLYNUM];
#define READWRITE	  3
extern tlv AutomaticWRTlv[READWRITE];
#endif