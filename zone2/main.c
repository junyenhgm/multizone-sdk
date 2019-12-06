/* Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved */

#include <platform.h>
#include <libhexfive.h>

#define LED1 5
#define LED2 6
#define LED3 7
#define LED4 8

#define BTN1 1
#define BTN2 2
#define BTN3 3
#define BTN4 4

static volatile int led = LED1;

void irq_25_handler(void)__attribute__((interrupt("user")));
void irq_25_handler(void){ // CLIC nt_src[7] GPIO

	// GPIO: read int
	const uint32_t gpio_int = GPIO_REG(GPIO_INT_STATUS);

	// Change LED
	GPIO_REG(GPIO_OUTPUT_VAL) &= ~((0x1 << LED1) | (0x1 << LED2) | (0x1 << LED3) | (0x1 << LED4));
	switch (gpio_int) {
		case 1<<BTN1: led = LED1; ECALL_SEND(1, (int[4]){201,0,0,0}); break;
		case 1<<BTN2: led = LED2; ECALL_SEND(1, (int[4]){202,0,0,0}); break;
		case 1<<BTN3: led = LED3; ECALL_SEND(1, (int[4]){203,0,0,0}); break;
		case 1<<BTN4: led = LED4; ECALL_SEND(1, (int[4]){204,0,0,0}); break;
	}

	// GPIO: clear int
	GPIO_REG(GPIO_INT_STATUS) = gpio_int;

}

void irq_25_init(){

	// GPIO: enable interrupts
	GPIO_REG(GPIO_INT_EN) |= ( (0x1 << BTN1) | (0x1 << BTN2) | (0x1 << BTN3) | (0x1 << BTN4));

	// GPIO: clear all interrupts
	GPIO_REG(GPIO_INT_STATUS) = 0xFFFFFFFF;

	// GPIO: set trigger mode to 0x5: Negative-edge
	GPIO_REG(GPIO_INT_MODE) |= (0x5<<4 | 0x5<<8 | 0x5<<12 | 0x5<<16);

	// GPIO: debounce
	GPIO_REG(GPIO_DEBOUNCE_EN) |= ( (0x1 << BTN1) | (0x1 << BTN2) | (0x1 << BTN3) );
	GPIO_REG(GPIO_DEBOUNCE_CTRL) = 0xFF, // 30us x (256+1) = 7.710ms

	// MultiZone: register irq handler (CLIC 25)
    ECALL_IRQ_VECT(25, irq_25_handler);

}

int main (void){

	GPIO_REG(GPIO_OUTPUT_EN) |= ( (0x1 << LED1) | (0x1 << LED2) | (0x1 << LED3) | (0x1 << LED4));

	irq_25_init();

	while(1){

		GPIO_REG(GPIO_OUTPUT_VAL) ^= (0x1 << led);

		const uint64_t timeout = ECALL_CSRR_MTIME() + RTC_FREQ*50/100;

		while (ECALL_CSRR_MTIME() < timeout){

			int msg[4]={0,0,0,0};

			if (ECALL_RECV(1, msg)) {
				switch (msg[0]) {
					// case '1': ECALL_CSRS_MIE();	break;
					// case '0': ECALL_CSRC_MIE();	break;
					case 'p': ECALL_SEND(1, (int[4] ) {'p','o','n','g'}); break;
				}
			}

			ECALL_YIELD();

		}

	}

}
