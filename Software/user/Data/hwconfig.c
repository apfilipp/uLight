/*
 * hwconfig.c
 *
 *  Created on: 12 февр. 2020 г.
 *      Author: VasiliSk
 */

#include "Resconfig.h"
#include "hwconfig.h"
#include "armutils.h"
#include "string.h"

extern uint32_t __boot_start__;
//hardware config pointer location in
#define HWLOCATION (*((uint32_t*) (&__boot_start__) + 8))
HWVersionStruct_t HWConfig;

void GoToBoot(uint8_t bootloader, int8_t node_id, int8_t server_id);

void HWInit(void) {
	if (HWLOCATION) {
		//new bootloader?
		HWVersionStruct_t* hwc = (void*) HWLOCATION;

		switch (hwc->StructVersion) {
		case 1:
			memcpy(&HWConfig, (void*) HWLOCATION, sizeof(HWConfig));
			break;

		default: {
			GoToBoot(1, -1, -1);  //unsupported boot version
			while (1)
				;
		}
			break;
		}

	} else {
#ifdef TRACE
		trace_printf("Unknown cpu configuration, no boot information found");
#endif
		GoToBoot(0, -1, -1);  //unsupported boot version
	}
}
