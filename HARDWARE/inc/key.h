#ifndef __KEY_H
#define __KEY_H
#include "sys.h"


#define KEY0	PCin(8)
#define KEY1	PCin(9)
#define KEY2	PDin(2)
#define WK_UP	PAin(0)


#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	  2	//KEY1按下
#define KEY2_PRES	  3	//KEY1按下
#define WKUP_PRES   4	//KEY_UP按下(即WK_UP/KEY_UP)

void KEY_Init(void);
u8 GetKeyValue(u8 mode);





#endif




