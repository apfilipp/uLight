/*
 * gpio_hal.h
 *
 *  Created on: 30 jan 2020
 *      Author: VasiliSk
 */

#pragma once

enum {
	GPIO_MODE_AN = 0x00,
	GPIO_MODE_IN = 0x04,
	GPIO_MODE_IN_PULL_UP_DOWN = 0x08,
	GPIO_MODE_OUT2_PP = 0x02,
	GPIO_MODE_OUT2_OD = 0x06,
	GPIO_MODE_OUT2_ALT_PP = 0x0A,
	GPIO_MODE_OUT2_ALT_OD = 0x0E,
	GPIO_MODE_OUT10_PP = 0x01,
	GPIO_MODE_OUT10_OD = 0x05,
	GPIO_MODE_OUT10_ALT_PP = 0x09,
	GPIO_MODE_OUT10_ALT_OD = 0x0D,
	GPIO_MODE_OUT50_PP = 0x03,
	GPIO_MODE_OUT50_OD = 0x07,
	GPIO_MODE_OUT50_ALT_PP = 0x0B,
	GPIO_MODE_OUT50_ALT_OD = 0x0F,
	GPIO_MODE_RESERVED = 0x0C,

	GPIO_MODE_IN_PULL_DOWN = 0x80,
	GPIO_MODE_IN_PULL_UP = 0x81,
	GPIO_MODE_OUT2_PP_DOWN = 0x20,
	GPIO_MODE_OUT2_PP_UP = 0x21,
	GPIO_MODE_OUT2_OD_DOWN = 0x60,
	GPIO_MODE_OUT2_OD_UP = 0x61,
	GPIO_MODE_OUT10_PP_DOWN = 0x10,
	GPIO_MODE_OUT10_PP_UP = 0x11,
	GPIO_MODE_OUT10_OD_DOWN = 0x50,
	GPIO_MODE_OUT10_OD_UP = 0x51,
	GPIO_MODE_OUT50_PP_DOWN = 0x30,
	GPIO_MODE_OUT50_PP_UP = 0x31,
	GPIO_MODE_OUT50_OD_DOWN = 0x70,
	GPIO_MODE_OUT50_OD_UP = 0x71,
};
/*
//In - digital input, OUT- dig. out, AF - alternate funct. , AN - analog
enum {
	GPIO_Mode_AN, GPIO_Mode_IN, GPIO_Mode_OUT, GPIO_Mode_AF,
};
//PP - push-pull, OD - open drain
enum {
	GPIO_Type_PP, GPIO_Type_OD = 0x4
};
// Higher - moar speed
enum {
	GPIO_Speed_Reserved, GPIO_Speed_2, GPIO_Speed_1, GPIO_Speed_3,
};

enum {
	GPIO_Pull_NO, GPIO_Pull_UP, GPIO_Pull_DWN
};

enum {
	GPIO_AF_SYS,
	GPIO_AF_TIM1_2,
	GPIO_AF_TIM3_4_5,
	GPIO_AF_TIM8_9_10_11,
	GPIO_AF_I2C1_2_3,
	GPIO_AF_SPI1_2_I2S2_I2S2ext,
	GPIO_AF_SPI3_I2Sext_I2S3,
	GPIO_AF_USART1_2_3_I2S3ext,
	GPIO_AF_UART4_5_USART6,
	GPIO_AF_CAN1_2_TIM12_13_14,
	GPIO_AF_USB_OTG,
	GPIO_AF_ETH,
	GPIO_AF_FSMC_SDIO_OTG_FS,
	GPIO_AF_DCMI,
	GPIO_AF_NONE,
	GPIO_AF_EVENTOUT
};
*/


#define _BITSET(bits) ((uint16_t)((bits % 010) | (bits / 010 % 010) << 1 | (bits / 0100 % 010) << 2 | \
  (bits / 01000 % 010) << 3 | (bits / 010000 % 010) << 4 | (bits / 0100000 % 010) << 5 | (bits / 01000000 % 010) << 6 | \
  (bits / 010000000 % 010) << 7 | (bits / 0100000000 % 010) << 8 | (bits / 01000000000 % 010) << 9 | (bits / 010000000000 % 010) << 10 | \
  (bits / 0100000000000 % 010) << 11 | (bits / 01000000000000 % 010) << 12 | (bits / 010000000000000 % 010) << 13 | \
  (bits / 0100000000000000 % 010) << 14 | (bits / 01000000000000000 % 010) << 15))

#define BIN(bits) _BITSET(0##bits)

#define _BIT_IN_MASK(MASK,BIT)  ((MASK) & (1<<(BIT)))? (uint32_t)1<<((BIT)<<2) : 0
void GPIO_PIN_SETUP(int pin, GPIO_TypeDef *port, int mode);
void GPIO_SET_RESET(int pin, GPIO_TypeDef *port, int mode);
int GPIO_PIN_GET(int pin, GPIO_TypeDef *port);
/*
static inline void GPIO_PIN_SETUP(int pin, GPIO_TypeDef *port, int mode, int type, int speed, int pull) {
	if (pull == GPIO_Pull_UP)
		port->BSRR = (uint32_t) 0x01 << (pin);
	else if (pull == GPIO_Pull_DWN)
		port->BRR = (uint32_t) 0x01 << (pin);

	uint32_t pin_mode = 0;

	switch (mode) {
	case GPIO_Mode_AN:
		pin_mode = 0;
		break;

	case GPIO_Mode_IN:
		(pull == GPIO_Pull_NO) ? pin_mode = 0x4 : pin_mode = 0x8;
		break;

	case GPIO_Mode_OUT:
		pin_mode &= 0x7;
		break;

	case GPIO_Mode_AF:
		pin_mode |= 0x8;
		break;
	}

	if (pin < 8) {
		port->CRL = (port->CRL & (~((uint32_t) 0x0F << (((pin) & 0x07) << 2)))) | (((uint32_t)(pin_mode) & 0x0F) << (((pin_mode) & 0x07) << 2));
	} else {
		port->CRH = (port->CRH & (~((uint32_t) 0x0F << (((pin) & 0x07) << 2)))) | (((uint32_t)(pin_mode) & 0x0F) << (((pin_mode) & 0x07) << 2));
	}
}*/
//Example: GPIO_INIT_PIN(GPIOA,3,GPIO_MODE_INPUT_PULL_DOWN);
//Example2: GPIO_MULTI_INIT(GPIOA,BIN(1010101010101010),GPIO_MODE_OUTPUT50_OPEN_DRAIN_DOWN);
