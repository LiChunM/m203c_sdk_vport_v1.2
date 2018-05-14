#include "PRD.h"
#include "pro.h"
/*
1792:LBS(纬度),

087B:LBS(经度),

1480:GPS(纬度),

00EA:GPS(经度）,

1E9D:电量百分比,

023A:信号强度,

2557:运行时长,

66AA:LED灯,

22BC:传输间隔
*/
tlv AutomaticRDTlv[READONLYNUM]={{0x1792,0x0c,{0}},{0x087B,0x0c,{0}},{0x1480,0x0c,{0}},{0x00EA,0x0c,{0}},{0x1E9D,0x04,{0}},{0x023A,0x03,{0}},{0x2557,0x0a,{0}}};
tlv AutomaticWRTlv[READWRITE]={{0x66AA,0x01,{0}},{0x22BC,0x0a,{0}},{0xACE1,0x0F,{0}}};