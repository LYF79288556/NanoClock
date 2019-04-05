/**
  ******************************************************************************
  * @file    timer.c 
  * @author  Kinvy
  * @version V_x.xx.x
  * @date    20xx-xx-xx
  * @brief   None         
  *          
  ******************************************************************************
  */
#include "timer.h"
#include "led.h"
#include "smg.h"
#include "set.h"

/**
 * @brief  TIM2_Int_Init
 * @param  arr: 重装载值   psc: 预分频值
					 定时器频率 Fck_psc/psc+1,  Fck_psc是定时器的时钟来源一般为72MHz 
 * @retval None
 */
void TIM3_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	NVIC_InitTypeDef 					NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	TIM_TimeBaseStructure.TIM_Period = arr;  												//设置重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;											//设置预分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;					//设置时中分频因子
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			//计数模式，向上计数
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx					 

}

void TIM2_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		switch(Display)
		{
			case DEFAULT	: Display_Time();	break;
			case DATE			: Display_Date(); break;
			case ALARM		: Display_Alarm();break;
			case SetTime	:	Display_Set_Time(); break;	
			case SetDate	: Display_Set_Date(); break;
		}
//		LED0 = !LED0;
	}
}



