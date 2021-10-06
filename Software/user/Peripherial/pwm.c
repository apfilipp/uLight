/*
 * pwm.c
 *
 *  Created on: 18 feb 2020 ã.
 *      Author: VasiliSk
 */
#include "Resconfig.h"
#include "Data\parameters.h"
#include "pwm.h"

//local functions
void pwmInitOutput(TIM_TypeDef *timer, uint32_t timerclock, uint32_t pwm_clk, uint32_t arr, uint16_t out1, uint16_t out2, uint16_t out3, uint16_t out4);

void PWMoutInit(void) {
	pwmInitOutput(TIM1, SystemCoreClock, PWMIO_Freq[Config.PWMouts.PWMIOfreq], 100, TIM_OC1_PWM1, TIM_OC2_PWM1, TIM_OC3_PWM1, TIM_OC4_PWM1);
	pwmInitOutput(TIM2, SystemCoreClock, PWMIO_Freq[Config.PWMouts.PWM1_2freq], 100, TIM_OC1_PWM1, TIM_OC2_PWM1, 0, 0);
	pwmInitOutput(TIM3, SystemCoreClock, PWMIO_Freq[Config.PWMouts.PWM3_4freq], 100, TIM_OC1_PWM1, TIM_OC2_PWM1, 0, 0);
	pwmInitOutput(TIM4, SystemCoreClock, PWMIO_Freq[Config.PWMouts.PWM5_6freq], 100, TIM_OC1_PWM1, TIM_OC2_PWM1, 0, 0);
	TIM1->CR1 = TIM_CR1_CEN;
	TIM2->CR1 = TIM_CR1_CEN;
	TIM3->CR1 = TIM_CR1_CEN;
	TIM4->CR1 = TIM_CR1_CEN;
}

void pwmInitOutput(TIM_TypeDef *timer, uint32_t timerclock, uint32_t pwm_clk, uint32_t arr, uint16_t out1, uint16_t out2, uint16_t out3, uint16_t out4) {
	// disable all outputs,  polarity active - high
	timer->CR1 = 0;
	timer->CCR1 = 0;
	timer->CCR2 = 0;
	timer->CCR3 = 0;
	timer->CCR4 = 0;

	//timer->BDTR = 0;
	// timer clock max mhz
	timer->PSC = timerclock / (pwm_clk * arr);
	timer->ARR = arr;
	//disable all channel outputs  except adc trigger channel
	timer->CCER = (out1 ? TIM_OC1_ENABLE : 0) | (out2 ? TIM_OC2_ENABLE : 0) | (out3 ? TIM_OC3_ENABLE : 0) | (out4 ? TIM_OC4_ENABLE : 0);
	//apply mode of operation
	timer->CCMR1 = (out1 & TIM_CCMR1_OC1M) | (out2 & TIM_CCMR1_OC2M);
	timer->CCMR2 = (out3 & TIM_CCMR2_OC3M) | (out4 & TIM_CCMR2_OC4M);
	// Master enable
	timer->BDTR = TIM_BDTR_MOE;
}

void PWMsetOutput(uint8_t pwm, uint16_t value) {

	if (value > 100)
		value = 100;

	TIM_TypeDef *tim = 0;
	switch (pwm) {
	default: {
		tim = TIM2;
	}
		break;
	case PWM3:
	case PWM4: {
		tim = TIM3;
	}
		break;
	case PWM5:
	case PWM6: {
		tim = TIM4;
	}
		break;
	case IOPWM1:
	case IOPWM2: {
		tim = TIM1;
	}
		break;
	case IOPWM3: //extras
		TIM1->CCR3 = value;
		return;
	case IOPWM4:
		TIM1->CCR4 = value;
		return;
	}

	if (pwm & 0x1)
		tim->CCR2 = value;
	else
		tim->CCR1 = value;
}

void PWMDisableAll(void) {
	TIM1->CCER = 0;
	TIM2->CCER = 0;
	TIM3->CCER = 0;
	TIM4->CCER = 0;
}
