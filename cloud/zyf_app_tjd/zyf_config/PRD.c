#include "PRD.h"
#include "pro.h"
/*
1B7C:��ѹ,

1980:GPS(���ȣ�,

1EEE:GPS(γ�ȣ�,

247E:LBS(���ȣ�,

2018:LBS(γ�ȣ�,

1E9D:�����ٷֱ�,

023A:�ź�ǿ��,

2557:����ʱ��,

66AA:��￪��,

22BC:������ͨAPP
*/
tlv AutomaticRDTlv[READONLYNUM]={{0x1B7C,0x06,{0}},{0x1980,0x0c,{0}},{0x1EEE,0x0c,{0}},{0x247E,0x0c,{0}},{0x2018,0x0c,{0}},{0x1E9D,0x04,{0}},{0x023A,0x03,{0}},{0x2557,0x0a,{0}}};
tlv AutomaticWRTlv[READWRITE]={{0x66AA,0x01,{0}},{0x22BC,0x05,{0}},{0xACE1,0x0F,{0}}};