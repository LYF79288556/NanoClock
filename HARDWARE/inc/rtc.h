#ifndef __RTC_H
#define __RTC_H	    

#include "stm32f10x.h"
//#include "calendar.h"
//时间结构体
 typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_day;
	vu8  week;		 
} Time_InitTypeDef;	

extern Time_InitTypeDef Time;
extern Time_InitTypeDef Alarm;

uint8_t RTC_Init(void);        																					//初始化RTC,返回0,失败;1,成功;
uint8_t RTC_Get(void);         																					//更新时间 
uint32_t RTC_GetALarm(void);																						//获取闹钟寄存器的值
uint8_t RTC_Set(Time_InitTypeDef* Time_InitStruct);											//设置时间			 
uint8_t RTC_Alarm_Set(Time_InitTypeDef* Time_InitStruct,uint8_t mode);  //设置闹钟		 
//和硬件无关的时间转换函数
uint8_t CNT_to_TIME(uint32_t cnt,Time_InitTypeDef* Time_InitStruct);  	//时间戳(计数器计数值)转成时间
uint32_t TIME_to_CNT(Time_InitTypeDef* Time_InitStruct);							  //时间转成计数值


#endif



