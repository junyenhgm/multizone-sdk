/* Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved */

#ifndef HEXFIVE_PLATFORM_H
#define HEXFIVE_PLATFORM_H

#define CPU_FREQ	20000000
#define RTC_FREQ 	20000000

// -----------------------------------------------------------------------------
// RTC (MTIME)
// -----------------------------------------------------------------------------
#define RTC_BASE	0xE6000000
#define RTC_MTIME	    0x0000

// -----------------------------------------------------------------------------
// UART - Andes ATCUART100
// -----------------------------------------------------------------------------
#define UART_BASE 	  0xF0300000
#define UART_LCR 			0x2C	// Line Control Register (0x2C)
#define UART_LCR_WLS		0b11	// LCR.WLS 1:0 Word length setting, 0b11 = 8-bit
#define UART_LCR_STB		1<<2	// LCR.STB Number of STOP bits, 0 = 1 bits
#define UART_LCR_PEN		1<<3	// LCR.PEN Parity enable bit
#define UART_LCR_DLAB		1<<7	// LCR.DLAB Divisor latch access bit
#define UART_DLL			0x20	// Divisor Latch LSB (when DLAB = 1) (0x20)
#define UART_DLM			0x24	// Divisor Latch MSB (when DLAB = 1) (0x24)
#define UART_FIFOCR			0x28	// FIFO Control Register (0x28)
#define UART_FIFOCR_FIFOE	1<<0	// FIFO enable
#define UART_LSR			0x34	// Line Status Register (0x34)
#define UART_LSR_THRE		1<<5	// THRE Transmitter Holding Register empty
#define UART_LSR_DR			1<<0	// DR Data ready
#define UART_THR 			0x20	// Transmitter Holding Register (when DLAB = 0) (0x20)
#define UART_RBR 			0x20	// Receiver Buffer Register (when DLAB = 0) (0x20)

// -----------------------------------------------------------------------------
// PLIC - AndesStar V5 UM166-10
// -----------------------------------------------------------------------------
#define PLIC_BASE 					0xE4000000
#define PLIC_PRI_OFFSET 			0x0
#define PLIC_PRI_SHIFT_PER_SOURCE 	0x2
#define PLIC_IRQ_GPIO_SOURCE		0x7
#define PLIC_EN_OFFSET				0x2000
#define PLIC_ENE_SHIFT_PER_TARGET	0x7
#define PLIC_CLAIM_OFFSET			0x200004

// -----------------------------------------------------------------------------
// GPIO - Andes ATCGPIO100
// -----------------------------------------------------------------------------
#define GPIO_BASE 			0xF0700000
#define GPIO_OUTPUT_EN  	0x28	// 3.2.5. Channel Direction Register (Offset 0x28)
#define GPIO_INPUT_VAL  	0x20	// 3.2.3. Channel Data-In Register   (Offset 0x20)
#define GPIO_OUTPUT_VAL 	0x24	// 3.2.4. Channel Data-Out Register  (Offset 0x24)
#define GPIO_INT_EN			0x50	// 3.2.10. Interrupt Enable Register (Offset 0x50)
#define GPIO_INT_MODE		0x54	// 3.2.11. Interrupt Mode Register (Offset 0x54, 0x58, 0x5C, 0x60)
#define GPIO_INT_STATUS		0x64	// 3.2.12. Interrupt Status Register (Offset 0x64)
#define GPIO_DEBOUNCE_EN 	0x70	// 3.2.13. De-bounce Enable Register (Offset 0x70)
#define GPIO_DEBOUNCE_CTRL 	0x74	// 3.2.14. De-bounce Control Register (Offset 0x74)

// -----------------------------------------------------------------------------
// SMU - Andes AE250 System Management Unit
// -----------------------------------------------------------------------------
#define SMU_BASE 			0xF0100000
#define SMU_PINMUX_CTRL0	0x1000	// PIN MUX control register 0 Section 15.10.11
#define SMU_PINMUX_CTRL1	0x1004	// PIN MUX control register 1 Section 15.10.12

// -----------------------------------------------------------------------------
// C Helper functions
// -----------------------------------------------------------------------------

#define _REG64(base, offset) (*(volatile uint64_t *)((base) + (offset)))
#define _REG32(base, offset) (*(volatile uint32_t *)((base) + (offset)))
#define _REG16(base, offset) (*(volatile uint16_t *)((base) + (offset)))

#define SMU_REG(offset)  _REG32(SMU_BASE, offset)
#define RTC_REG(offset)  _REG64(RTC_BASE, offset)
#define GPIO_REG(offset) _REG32(GPIO_BASE, offset)
#define PWM_REG(offset)  _REG32(PWM_BASE, offset)
#define UART_REG(offset) _REG32(UART_BASE, offset)
#define PLIC_REG(offset) _REG32(PLIC_BASE, offset)

#endif /* HEXFIVE_PLATFORM_H */
