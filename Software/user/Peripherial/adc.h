/*
 * adc.h
 *
 *  Created on: 16 feb. 2020 ã.
 *      Author: VasiliSk
 */

#pragma once

typedef struct {
	int32_t Amp;
	int32_t V12;
	int32_t Tint;
	int32_t T1;
	int32_t T2;
	int32_t VBrake;
	int32_t VThrottle;
} ADC1_Smp_t;

extern volatile ADC1_Smp_t ADC_Values;
extern volatile ADC1_Smp_t ADC_ValuesF;

void ADC_user_init(void);
int32_t Filter_LPi(int32_t new, int32_t previous, uint8_t beta);
int32_t Filter_LPibetter(int32_t new, int32_t previous, uint8_t beta, int32_t K ) ;
