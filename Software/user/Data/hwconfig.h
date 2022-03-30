/*
 * hwconfig.h
 *
 *  Created on: 12 февр. 2020 г.
 *      Author: User
 */

#pragma once

typedef struct
{
	char Date[12];
	char Version[24];
} Version_t;

typedef struct
{
	uint16_t StructVersion;
	const char *Date;
	const char *Version;
	const char *Name;

	struct
	{
		int32_t Amp_mult;
		int32_t Amp_div;
		int32_t V12_mult;
		int32_t V12_div;
		int32_t VthVbrk_mult;
		int32_t VthVbrk_div;
		uint32_t NTCtype;
	} ADC;
} HWVersionStruct_t;

extern HWVersionStruct_t HWConfig;
void HWInit(void);
