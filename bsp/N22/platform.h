/* Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved */

#ifndef HEXFIVE_PLATFORM_H
#define HEXFIVE_PLATFORM_H

#define CPU_FREQ	20000000
#define RTC_FREQ 	20000000

// -----------------------------------------------------------------------------
// RTC (MTIME)
// -----------------------------------------------------------------------------
#define RTC_MTIME	0xE6000000

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
// C Helper functions
// -----------------------------------------------------------------------------

#define _REG64(base, offset) (*(volatile uint64_t *)((base) + (offset)))
#define _REG32(base, offset) (*(volatile uint32_t *)((base) + (offset)))
#define _REG16(base, offset) (*(volatile uint16_t *)((base) + (offset)))

#define RTC_REG(offset)  _REG64(RTC_BASE, offset)
#define GPIO_REG(offset) _REG32(GPIO_BASE, offset)
#define PWM_REG(offset)  _REG32(PWM_BASE, offset)
#define UART_REG(offset) _REG32(UART_BASE, offset)


#endif /* HEXFIVE_PLATFORM_H */
