#include "PRD.h"
#include "pro.h"
/*
1792:LBS(γ��),

087B:LBS(����),

1480:GPS(γ��),

00EA:GPS(���ȣ�,

1E9D:�����ٷֱ�,

023A:�ź�ǿ��,

2557:����ʱ��,

66AA:LED��,

22BC:������
*/
tlv AutomaticRDTlv[READONLYNUM]={{0x1792,0x0c,{0}},{0x087B,0x0c,{0}},{0x1480,0x0c,{0}},{0x00EA,0x0c,{0}},{0x1E9D,0x04,{0}},{0x023A,0x03,{0}},{0x2557,0x0a,{0}}};
tlv AutomaticWRTlv[READWRITE]={{0x66AA,0x01,{0}},{0x22BC,0x0a,{0}},{0xACE1,0x0F,{0}}};