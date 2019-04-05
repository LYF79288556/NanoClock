/**
  ******************************************************************************
  * @file    main.c 
  * @author  Kinvy
  * @version V_0.0.0
  * @date    2019-01-20
  * @brief   None         
  * @email	 kinvy66@163.com	
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "delay.h"
#include "smg.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "usart.h"
#include "rtc.h"
#include "timer.h"
#include "set.h"


int main(void)
{

	Alarm.w_year 	= 2019;
	Alarm.w_month = 1;
	Alarm.w_day  	= 25;
	Alarm.hour 		= 11;
	Alarm.min 		= 57;
	Alarm.sec 		= 00;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //NVIC分组2
	delay_init();
	LED_Init();
	LED_SMG_Init();
	uart_init(115200);  //设置串口波特率115200
	EXTIX_Init();
	RTC_Init();
	RTC_Alarm_Set(&Alarm,1);  //这句没有多大用，闹钟功能还没有，只是测试
	TIM3_Init(19,7199);  //2ms进一次中断刷新数码管显示
	
	while(1)
	{
		Switch_Display();   //切换显示界面
		Set_Rtc();					//设置时间和日期
	}
	
}
