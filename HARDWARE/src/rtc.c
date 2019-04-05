/**
  ******************************************************************************
  * @file    rtc.c 
  * @author  Kinvy
  * @version V_x.xx.x
  * @date    20xx-xx-xx
  * @brief   None         
  *          
  ******************************************************************************
  */

#include "rtc.h"
#include "delay.h"
#include "led.h"
#include "usart.h"

#define SEC_PER_DAY		86400UL
#define SEC_PER_HOU		3600UL
#define SEC_PER_MIN		60

Time_InitTypeDef Time;
Time_InitTypeDef Alarm;

const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};



/**
 * @brief  RTC NVIC
 * @param  None
 * @retval None
 */
static void RTC_NVIC_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


/**
 * @brief  Init RTC 
 * @param  None
 * @retval 0,Successful;  1,failed 
 */
uint8_t RTC_Init(void)   
{
	uint8_t temp=0;
  Time_InitTypeDef Time_InitStruct;	//日历结构体
	
	Time_InitStruct.w_year 	= 2019;
	Time_InitStruct.w_month 	= 1;
	Time_InitStruct.w_day  	= 27;
	Time_InitStruct.hour 		= 14;
	Time_InitStruct.min 			= 40;
	Time_InitStruct.sec 			= 00;
	

                 
	/* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{
		/* Reset Backup Domain */
		BKP_DeInit();

		/* Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);
		/* Wait till LSE is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&&temp<250)
		{
			temp++;
			delay_ms(10);
		}
			
		if(temp>=250)return 1;//初始化时钟失败,晶振有问题

		/* Select LSE as RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		/* Enable RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		
		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
			
		RTC_EnterConfigMode();/// 允许配置	

		/* Set RTC prescaler: set RTC period to 1sec */
		RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		
		RTC_Set(&Time_InitStruct);  //设置时间	
		RTC_ExitConfigMode(); //退出配置模式  
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	//向指定的后备寄存器中写入用户程序数据
	}
	else//系统继续计时
	{

		RTC_WaitForSynchro();	//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE);	//使能RTC秒中断和闹钟中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	}
	RTC_NVIC_Config();//RCT中断分组设置		    				     
	RTC_Get();//更新时间	
	return 0;
}

/**
 * @brief  None
 * @param  None
 * @retval None
 */
uint8_t RTC_Set(Time_InitTypeDef* Time_InitStruct)//设置时间			 
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	RTC_SetCounter(TIME_to_CNT(Time_InitStruct));	//设置RTC计数器的值

}

/**
 * @brief  设置闹钟
 * @param  Time_InitStruct: 时间结构体
					 mode  1:一次;0:每天
 * @retval None
 */
uint8_t RTC_Alarm_Set(Time_InitTypeDef* Time_InitStruct,uint8_t mode)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	RTC_SetAlarm(TIME_to_CNT(Time_InitStruct));
		
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	
	return 0;	 
}


/**
 * @brief  更新时间 
 * @param  None
 * @retval None
 */
uint8_t RTC_Get(void)        
{
	CNT_to_TIME(RTC_GetCounter(),&Time);
	return 0;
}

/**
 * @brief  获取当前设置的闹钟
 * @param  None
 * @retval None
 */
uint32_t RTC_GetALarm(void)
{
  uint16_t tmp = 0;
  tmp = RTC->ALRL;
  return (((uint32_t)RTC->ALRH << 16 ) | tmp) ;
}




//RTC中断服务函数
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
	{							
		RTC_Get();//更新时间   
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
	{
		LED0 = 0;
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	  	
		RTC_Get();//更新时间   
  } 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//清闹钟中断
	RTC_WaitForLastTask();	  	    						 	   	 
}





/**
 * @brief  判断是否为闰年
 * @param  year:年份
 * @retval 1:闰年		0:平年
 */
static uint8_t Is_Leap_Year(uint16_t year)
{
	uint8_t rev;
	(year%400==0||(year%4==0 &&year%100 !=0))? (rev=1) : (rev=0); 
	return rev;
}


/**
  * @brief  时间戳(计数器计数值)转成时间
  * @param  cnt: count value
	* @param  Time_InitTypeDef: Time structure
  * @retval 0:Successful;  other:failed 
  */
uint8_t CNT_to_TIME(uint32_t cnt,Time_InitTypeDef* Time_InitStruct)  
{
	uint16_t days;
	
	days = cnt / (24*3600);  //计算天数
	
	//计算时分秒
	Time_InitStruct->hour = cnt%(24*3600)/3600;  
	Time_InitStruct->min = cnt%(24*3600)%3600/60;
	Time_InitStruct->sec = cnt%(24*3600)%3600%60 ;
	
	//初始年为1970.1.1
	Time_InitStruct->w_year =1970;   	
	Time_InitStruct->w_month=1;
	Time_InitStruct->w_day	=1;
	Time_InitStruct->week 	= 4;
	if(days > 0) //超过一天
	{
		Time_InitStruct->week=(days+4)%7+1;	//计算星期，可通过其他算法优化
		
		while(days >= 365)  //计算年份
		{
			if(Is_Leap_Year(Time_InitStruct->w_year))
			{
				if(days>=366) days -= 366;
				else {Time_InitStruct->w_year++; break;}
			}
			else days -= 365;  //平年
			
			Time_InitStruct->w_year++;	
		}
//		printf("year-days:%d\n",days);
		while(days>=28)//计算月份
		{
//			printf("month:%d\n",Time_InitStruct->w_month);
			if(Is_Leap_Year(Time_InitStruct->w_year)&& (Time_InitStruct->w_month==2))
			{
				if(days>=29) days -= 29;
				else break;
			}
			else
			{
				if(days>=mon_table[Time_InitStruct->w_month-1])
					days -= mon_table[Time_InitStruct->w_month-1];
				else 
					{
						Time_InitStruct->w_month++;
						break;
					}
			}
			Time_InitStruct->w_month++;
		}
//		Time_InitStruct->w_month;
		if(days == 0)
			Time_InitStruct->w_day=mon_table[Time_InitStruct->w_month-1];
		else
			Time_InitStruct->w_day = days; //计算日期
	
	}
	
	return 0;
}

/**
 * @brief  时间转成计数值
 * @param  时间结构体
 * @retval 当前时间的时间戳
 */
uint32_t TIME_to_CNT(Time_InitTypeDef* Time_InitStruct)
{
	uint16_t days = 0;
	uint16_t i;
	
	//限定时间的设置范围
	if(Time_InitStruct->w_year<1970||Time_InitStruct->w_year>2070) return 1;	
	
	for(i = 1970;i < Time_InitStruct->w_year; i++)	//整年的天数
	{
		if(Is_Leap_Year(i))
			days += 366;
		else days += 365;
	}
	for(i = 1;i < Time_InitStruct->w_month; i++) //剩余月的天数
	{
		days += mon_table[i-1];
	}

	if(Is_Leap_Year(Time_InitStruct->w_year)&&(Time_InitStruct->w_month>2))
		days += 1;
	
	days += Time_InitStruct->w_day; //剩余日的天数
	

	return (days*SEC_PER_DAY)+(Time_InitStruct->hour*SEC_PER_HOU)
					+(Time_InitStruct->min*SEC_PER_MIN)+Time_InitStruct->sec;
}








