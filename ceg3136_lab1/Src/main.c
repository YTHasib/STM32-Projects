
#include <stdio.h>
#include "gpio.h"
#include "systick.h"

static const Pin_t BlueLED = {GPIOB, 7}; //Pin PB7 -> User LD2
static const Pin_t Button = {GPIOB, 13}; //Pin PC13 <- User B1

#define BLINK_PERIOD 1000 // 1 second
static Time_t lastToggle;

static unsigned int loopCount = 0;

int main (void){

	printf("Welcome to %c%c%c%d!\n", 67, 69, 71, 0xC40);

	Init_Alarm();

	StartSysTick();

	while (1){
		Task_Alarm();
		WaitForSysTick();
	}

	/* GPIO_Enable(BlueLED);
	GPIO_Mode(BlueLED, OUTPUT);
	GPIO_Output(BlueLED, LOW);

	GPIO_Enable(Button);
	GPIO_Mode(Button, INPUT);

	StartSysTick();
	lastToggle = TimeNow();

	printf("Hello, World! \n");

    while (1) {
    	if (TimePassed(lastToggle) >= BLINK_PERIOD / 2){
    		if (!GPIO_Input (Button))
    			GPIO_Toggle(BlueLED);
    		else
    			GPIO_Output(BlueLED, LOW);
    		lastToggle = TimeNow();
    		printf("Loop count %u\n", loopCount);
    	}

    loopCount++;
    WaitForSysTick();

	//msDelay(BLINK_PERIOD / 2);
	//if (!GPIO_Input (Button))
	//GPIO_Output(BlueLED, HIGH);

	//msDelay(BLINK_PERIOD / 2);
	//GPIO_Output(BlueLED, LOW);
	// loopCount++;
	//printf("Loop count %u\n", loopCount);

    }*/

}

