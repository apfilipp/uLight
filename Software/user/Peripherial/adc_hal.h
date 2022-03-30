/*
 * adc_hal.h
 *
 *  Created on: 16 feb. 2020 ã.
 *      Author: VasiliSk
 */
#include "Resconfig.h"

#pragma once

typedef enum
{
	ADC_CR1_MODE_INDEPENDENT = 0,
	ADC_CR1_MODE_REGSIMULT_INJECSIMULT = (1 << 16),
	ADC_CR1_MODE_REGSIMULT_ALTERTRIG = (2 << 16),
	ADC_CR1_MODE_INJECSIMULT_INTERLFAST = (3 << 16),
	ADC_CR1_MODE_INJECSIMULT_INTERLSLOW = (4 << 16),
	ADC_CR1_MODE_INJECSIMULT = (5 << 16),
	ADC_CR1_MODE_REGSIMULT = (6 << 16),
	ADC_CR1_MODE_INTERLFAST = (7 << 16),
	ADC_CR1_MODE_INTERLSLOW = (8 << 16),
	ADC_CR1_MODE_ALTERTRIG = (9 << 16)
} ADC_Modes;

extern uint32_t ADC_CLOCK;
#define ADC_CALIBRATION_TIMEOUT         ((uint32_t) 10)
//conversion sequences
#define ADC_SQR1_SetNumberOfConv(_NbrOfConversion_)	(((_NbrOfConversion_) - (uint8_t)1) << 20)

#define ADC_SQR3_RK(_CHANNELNB_, _RANKNB_)			((_CHANNELNB_) << (5 * ((_RANKNB_) - 1)))
#define ADC_SQR2_RK(_CHANNELNB_, _RANKNB_)			((_CHANNELNB_) << (5 * ((_RANKNB_) - 7)))
#define ADC_SQR1_RK(_CHANNELNB_, _RANKNB_)			((_CHANNELNB_) << (5 * ((_RANKNB_) - 13)))

//sampling times
#define ADC_SMPR1(_SAMPLETIME_, _CHANNELNB_)		((_SAMPLETIME_) << (3 * ((_CHANNELNB_) - 10)))
#define ADC_SMPR2(_SAMPLETIME_, _CHANNELNB_)		((_SAMPLETIME_) << (3 * (_CHANNELNB_)))

typedef enum
{
	SMPR_1CYCLE_5 = 0,
	SMPR_7CYCLES_5,
	SMPR_13CYCLES_5,
	SMPR_28CYCLES_5,
	SMPR_41CYCLES_5,
	SMPR_55CYCLES_5,
	SMPR_71CYCLES_5,
	SMPR_239CYCLES_5
} ADC_SampleTimes;

typedef enum
{
	CHNL_0 = 0,
	CHNL_1,
	CHNL_2,
	CHNL_3,
	CHNL_4,
	CHNL_5,
	CHNL_6,
	CHNL_7,
	CHNL_8,
	CHNL_9,
	CHNL_10,
	CHNL_11,
	CHNL_12,
	CHNL_13,
	CHNL_14,
	CHNL_15,
	CHNL_TEMPSENSOR,
	CHNL_VREFINT,
} ADC_Channels;

typedef enum
{
	ADC_PhaseV_Normal = 0, ADC_PhaseV_Extended
} ADC_PhaseV_Modes;

typedef enum
{
	ADC3_Injected_Update = 0, ADC3_Injected_Compare4 = ADC_CR2_JEXTSEL_0, //TIM1 CC4
	ADC3_Injected_Software = ADC_CR2_JEXTSEL,
} ADC3_Inj_Select;

typedef enum
{
	ADC12_Injected_Update = 0, //
	ADC12_Injected_Compare1 = ADC_CR2_JEXTSEL_0 | ADC_CR2_JEXTSEL_1, //TIM2 CC1
	ADC12_Injected_Compare4 = ADC_CR2_JEXTSEL_0, //TIM1 CC4
	ADC12_Injected_Software = ADC_CR2_JEXTSEL
} ADC12_Inj_Select;
