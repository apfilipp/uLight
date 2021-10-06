/*
 * gpio_hal.c
 *
 *  Created on: 30 џэт. 2020 у.
 *      Author: User
 */

#include "stm32f10x.h"

#define _INIT_PIN_CRL(PORT,PIN_NUM,PIN_MODE)    PORT->CRL = (PORT->CRL &(~((uint32_t)0x0F<<(((PIN_NUM) & 0x07)<<2)))) | (((uint32_t)(PIN_MODE)&0x0F)<<(((PIN_NUM) & 0x07)<<2))
#define _INIT_PIN_CRH(PORT,PIN_NUM,PIN_MODE)    PORT->CRH = (PORT->CRH &(~((uint32_t)0x0F<<(((PIN_NUM) & 0x07)<<2)))) | (((uint32_t)(PIN_MODE)&0x0F)<<(((PIN_NUM) & 0x07)<<2))
#define _INIT_PIN(PORT,PIN_NUM,PIN_MODE)        ((PIN_NUM)<8)? (_INIT_PIN_CRL(PORT,PIN_NUM,PIN_MODE)):(_INIT_PIN_CRH(PORT,PIN_NUM,PIN_MODE))
#define _GPIO_SET_RESET(PORT,PIN_NUM,PIN_MODE) ((PIN_MODE)==0x01)? (PORT->BSRR = (uint32_t)0x01<<(PIN_NUM)) : (PORT->BRR=(uint32_t)0x01<<(PIN_NUM))

#define _GPIO_INIT_PIN(PORT,PIN_NUM,PIN_MODE)           ((PIN_MODE)<0x10)? (_INIT_PIN(PORT,PIN_NUM,PIN_MODE))   :\
                (_GPIO_SET_RESET(PORT,PIN_NUM,(PIN_MODE)&0x01), _INIT_PIN(PORT,PIN_NUM,(PIN_MODE)>>4))

void GPIO_PIN_SETUP(int pin, GPIO_TypeDef *port, int mode) {
	_GPIO_INIT_PIN(port, pin, mode);
}

void GPIO_SET_RESET(int pin, GPIO_TypeDef *port, int mode) {
	_GPIO_SET_RESET(port, pin, mode);
}

int GPIO_PIN_GET(int pin, GPIO_TypeDef *port) {
	return (port->IDR & ((uint32_t)1 << pin)) ? 1 : 0;
}
