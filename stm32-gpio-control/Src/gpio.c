// General-purpose input/output driver

#include "gpio.h"

// --------------------------------------------------------
// Initialization
// --------------------------------------------------------
// Enable the GPIO port peripheral clock for the specified pin
void GPIO_Enable (Pin_t pin) {
 // ...

	RCC -> AHB2ENR |= (1 << GPIO_PORT_NUM(pin.port)); // register controlling clock enable gets its bit turned on by GPIO_PORT_NUM

}

// Set the operating mode of a GPIO pin:
// Input (IN), Output (OUT), Alternate Function (AF), or Analog (ANA)
void GPIO_Mode (Pin_t pin, PinMode_t mode) {
 // ...

	pin.port -> MODER &= ~(0x3 << (2 * pin.bit)); // 4 choices so we'll need 3 bits
	// in the pin, clear 2 bits for the mode
	// &= ~mask => Masking out those two mode bits


	pin.port -> MODER |= (mode << (2 * pin.bit));
	// set new mode with those two bits.

}

// --------------------------------------------------------
// Pin observation and control
// --------------------------------------------------------

// Observe the state of an input pin
PinState_t GPIO_Input(const Pin_t pin) {
 // ...

	// IDR = input data register
	uint32_t val= (pin.port -> IDR >> pin.bit) & 0x1;
	return val;
	// Send data from pin.port to IDR.
	// isolates main bit to index 0, and make sure it is either 1 or 0
	// returns either HIGH or LOW, AKA 0 and 1
}


// Control the state of an output pin
void GPIO_Output(Pin_t pin, const PinState_t state) {
 // ...

	if (state)
		pin.port ->ODR |= (1 << pin.bit); // Set bit to HIGH
	else
		pin.port ->ODR &= ~(1 << pin.bit); // Set bit to LOW, aka clear
	// &= ~mask is Masking out and |= value is setting new value
}

// Toggle the state of an output pin
void GPIO_Toggle (Pin_t pin) {
 // ...

	pin.port -> ODR ^= (1 << pin.bit); // recall ODR is output data register
	// allows to toggle without knowing current state
	// XOR => ^=, and 1 << pin.bit is a mask
	// pin.port = some specific pin
}

// --------------------------------------------------------
// Interrupt handling
// --------------------------------------------------------

// Array of callback function pointers
// Bits 0 to 15 (each can select one port GPIOA to GPIOH)
// Rising and falling edge triggers for each
static void (*callbacks[16][2]) (void);

// Register a function to be called when an interrupt occurs,
// enable interrupt generation, and enable interrupt vector
void GPIO_Callback(Pin_t pin, void (*func)(void), PinEdge_t edge)
{

	callbacks[pin.bit][edge] = func;
	// Enable interrupt generation
	if (edge == RISE)
	 EXTI->RTSR1 |= 1 << pin.bit;
	else // FALL
	 EXTI->FTSR1 |= 1 << pin.bit;
	EXTI->EXTICR[pin.bit / 4] |= GPIO_PORT_NUM(pin.port) << 8*(pin.bit % 4);
	EXTI->IMR1 |= 1 << pin.bit;
	// Enable interrupt vector
	NVIC->IPR[EXTI0_IRQn + pin.bit] = 0;
	__COMPILER_BARRIER();
	NVIC->ISER[(EXTI0_IRQn + pin.bit) / 32] = 1 << ((EXTI0_IRQn + pin.bit) % 32);
	__COMPILER_BARRIER();

}

// Interrupt handler for all GPIO pins
void GPIO_IRQHandler (int i) {
	// Clear pending IRQ
NVIC->ICPR[(EXTI0_IRQn + i) / 32] = 1 << ((EXTI0_IRQn + i) % 32);

// Detect rising edge
	if (EXTI->RPR1 & (1 << i)) {
		EXTI->RPR1 = (1 << i); // Service interrupt
		callbacks[i][RISE](); // Invoke callback function
	}

	// Detect falling edge
	if (EXTI->FPR1 & (1 << i)) {
		EXTI->FPR1 = (1 << i); // Service interrupt
		callbacks[i][FALL](); // Invoke callback function
	}
}

// Dispatch all GPIO IRQs to common handler function
void EXTI0_IRQHandler() { GPIO_IRQHandler( 0); }
void EXTI1_IRQHandler() { GPIO_IRQHandler( 1); }
void EXTI2_IRQHandler() { GPIO_IRQHandler( 2); }
void EXTI3_IRQHandler() { GPIO_IRQHandler( 3); }
void EXTI4_IRQHandler() { GPIO_IRQHandler( 4); }
void EXTI5_IRQHandler() { GPIO_IRQHandler( 5); }
void EXTI6_IRQHandler() { GPIO_IRQHandler( 6); }
void EXTI7_IRQHandler() { GPIO_IRQHandler( 7); }
void EXTI8_IRQHandler() { GPIO_IRQHandler( 8); }
void EXTI9_IRQHandler() { GPIO_IRQHandler( 9); }
void EXTI10_IRQHandler() { GPIO_IRQHandler(10); }
void EXTI11_IRQHandler() { GPIO_IRQHandler(11); }
void EXTI12_IRQHandler() { GPIO_IRQHandler(12); }
void EXTI13_IRQHandler() { GPIO_IRQHandler(13); }
void EXTI14_IRQHandler() { GPIO_IRQHandler(14); }
void EXTI15_IRQHandler() { GPIO_IRQHandler(15); }
