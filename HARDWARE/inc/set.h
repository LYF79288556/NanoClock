#ifndef __SET_H
#define __SET_H	 
#include "sys.h"

//定义显示界面编号
#define DEFAULT  1
#define DATE  	 2
#define ALARM  	 3
#define SetTime	 4
#define SetDate	 5

#define TEMPE  	 4
#define ADC      5

//设置item
#define SetHour		1
#define SetMin		2
#define SetSec		3
#define SetYear		4
#define SetMonth	5
#define SetDay		6



extern uint8_t Display;  //显示的界面


void Display_Time(void);
void Display_Date(void);
void Display_Alarm(void);
void Display_Set_Time(void);
void Display_Set_Date(void);

void Switch_Display(void);
void Set_Rtc(void);
void Set_Alarm(void);


#endif




