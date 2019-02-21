#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define STM32_LED_WS 			PFout(0)
#define STM32_LED_WD 			PFout(1)
#define STM32_LED_CO 			PFout(2)
#define STM32_LED_SO2 		PFout(3)
#define STM32_LED_NO2 		PFout(4)
#define STM32_LED_O3 			PFout(5)
#define STM32_LED_PM 			PFout(6)
#define STM32_LED_ADC1 		PFout(7)
#define STM32_LED_ADC2 		PFout(8)
#define STM32_LED_SDCARD 	PFout(9)
#define STM32_LED_NBIOT 	PFout(10)
#define STM32_LED_HTS			PEout(6)
#define NB_IOT_RESET			PBout(13)

void LED_Init(void);
void Reset_Init(void);
		 				    
#endif
