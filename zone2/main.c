/* Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved */

#include <platform.h>
#include <libhexfive.h>

#define LED1 5
#define LED2 6
#define LED3 7

#define BTN1 1
#define BTN2 2
#define BTN3 3

static volatile int led = LED1;

void irq_11_handler(void)__attribute__((interrupt("user")));
void irq_11_handler(void){ // PLIC

	// PLIC: claim/read int (hart 0)
	const uint32_t plic_int = PLIC_REG(PLIC_CLAIM_OFFSET);

	// GPIO: read int
	const uint32_t gpio_int = GPIO_REG(GPIO_INT_STATUS);

	// Change LED
	GPIO_REG(GPIO_OUTPUT_VAL) &= ~((0x1 << LED1) | (0x1 << LED2) | (0x1 << LED3));
	switch (gpio_int) {

		case 1<<BTN1: led = LED1;
		ECALL_SEND(1, (int[4]){201,0,0,0});
		break;

		case 1<<BTN2: led = LED2;
		ECALL_SEND(1, (int[4]){211,0,0,0});
		break;

		case 1<<BTN3: led = LED3;
		ECALL_SEND(1, (int[4]){221,0,0,0});
		break;
	}

	// GPIO: clear int
	GPIO_REG(GPIO_INT_STATUS) = gpio_int;

	// PLIC: complete/write int (hart 0)
	PLIC_REG(PLIC_CLAIM_OFFSET) = plic_int;

}


/*configures Button BTN1, BTN2, BTN3 as a global gpio irq*/
void irq_11_init(){

	// GPIO: enable interrupts
	GPIO_REG(GPIO_INT_EN) |= ( (0x1 << BTN1) | (0x1 << BTN2) | (0x1 << BTN3) );

	// GPIO: clear all interrupts
	GPIO_REG(GPIO_INT_STATUS) = 0xFFFFFFFF;

	// GPIO: set trigger mode to 0x5: Negative-edge
	GPIO_REG(GPIO_INT_MODE) |= (0x5<<4 | 0x5<<8 | 0x5<<12);

	// GPIO: debounce
	GPIO_REG(GPIO_DEBOUNCE_EN) |= ( (0x1 << BTN1) | (0x1 << BTN2) | (0x1 << BTN3) );
	GPIO_REG(GPIO_DEBOUNCE_CTRL) = 0xFF, // 30us x (256+1) = 7.710ms

	//--------------------------------------------------------------------------------------

	// PLIC: set int priority >1 for source GPIO
	PLIC_REG(PLIC_PRI_OFFSET + (PLIC_IRQ_GPIO_SOURCE << PLIC_PRI_SHIFT_PER_SOURCE)) = 0x1;

	// PLIC: enable int source GPIO (hart 0)
	PLIC_REG(PLIC_EN_OFFSET) |= 1 << (PLIC_IRQ_GPIO_SOURCE & 0x1F);

	// ----------------------------------------------------------------------------------

	// MultiZone: register irq handler 11 (external)
    ECALL_IRQ_VECT(11, irq_11_handler);

}


int main (void){

	GPIO_REG(GPIO_OUTPUT_EN) |= ( (0x1 << LED1) | (0x1 << LED2) | (0x1 << LED3) );

	irq_11_init();

	while(1){

		GPIO_REG(GPIO_OUTPUT_VAL) ^= (0x1 << led);

		const uint64_t timeout = ECALL_CSRR_MTIME() + RTC_FREQ;
								//(GPIO_REG(GPIO_OUTPUT_VAL) & (0x1 << led) ? 50 : 950) * RTC_FREQ/1000 ;

		while (ECALL_CSRR_MTIME() < timeout){

			int msg[4]={0,0,0,0};

			if (ECALL_RECV(1, msg)) {
				switch (msg[0]) {
					case '1': ECALL_CSRS_MIE();	break;
					case '0': ECALL_CSRC_MIE();	break;
					case 'p': ECALL_SEND(1, (int[4] ) {'p','o','n','g'}); break;
				}
			}

			ECALL_YIELD();

		}

	}

}
