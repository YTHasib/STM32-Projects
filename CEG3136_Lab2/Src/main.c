#include <stdio.h>

#include "systick.h"
#include "i2c.h"
#include "gpio.h"

#include "alarm.h"
#include "game.h"
#include "display.h"


int main (void){
	Init_Alarm();
	Init_Game();


	StartSysTick();

	while (1){
		Task_Alarm();
		Task_Game();


		UpdateIOExpanders();
		UpdateDisplay();
		ServiceI2CRequests();
		WaitForSysTick();

	}
}
