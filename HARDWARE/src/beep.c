#include "beep.h"

/**
  * @brief	蜂鸣器IO初始化
  *         
  * @param  None
  *         
  * @retval None
  */
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //定义GPIO初始初始化结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能GPIOC的时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  			//LED所在引脚是PC13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//GPIO的模式为推挽输出
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_50MHz;	//输出的速度为50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);				//初始化PC13
	
	BEEP = 0;
	
}



