#ifndef __SMG_H
#define __SMG_H

#include "sys.h"

////74HC138操作线
#define LED_A0 PCout(10) //A0地址线
#define LED_A1 PCout(11) //A1地址线
#define LED_A2 PCout(12) //A2地址线

////74HC595操作线
#define LED_DS		PBout(3) //数据线
#define LED_LCLK	PBout(4) //锁存时钟线
#define LED_SCK		PBout(5) //时钟线

//数码管是否显示小数点的宏
#define Dot		1
#define	NoDot	0

//u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,
//							0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x00};


void LED_SMG_Init(void);
void LED_Refresh(void);
void LED_Write_Data(uint8_t duan,uint8_t wei);
void LED_SMG_Display(uint8_t num,uint8_t wei,uint8_t dot);

#endif




