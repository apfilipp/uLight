/*
 * Resconfig.h
 *
 *  Created on: 30.01.2020
 *      Author: VasiliSk
 */
#pragma once

#include "stm32f10x.h"
#include "diag/Trace.h"
#include "Peripherial\gpio_user.h"
#include "Peripherial\gpio_hal.h"


#define STM32F1

//saves
#define Struct_number 3

#define FLASH_PAGESIZE	1*1024
//one rewritable page in flash
#define FLASH_PAGE_SIZE_DATA 1*1024
#define FLASH_ALIGN 4
//number of pages in bank
#define FLASH_PAGES_IN_BANK 3

//number of messages for queue TX
#define CAN_TX_BUFFER_SIZE 20
//number of messages for queue RX
#define CAN_RX_BUFFER_SIZE 20
//number of messages for queue Requests
#define CAN_REQ_BUFFER_SIZE 0
//connectivity line CPUs
//#define ConnectivityLine
//__attribute__ ((section(".sram")))
#define SRAM __attribute__ ((section(".sram")))
//#define SRAM2 __attribute__ ((section(".exram")))

#define PARAM_DIRECTORIES PD_Directories
#define PARAM_DIRECTORIES_SIZE PD_Directories_size
#define PARAM_NAME_LENGTH	24
#define STORAGE_CRC

#define FRONTEND
#define BACKEND
