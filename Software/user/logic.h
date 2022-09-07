/*
 * logic.h
 *
 *  Created on: 16 feb 2020 ã.
 *      Author: VasiliSk
 */

#pragma once

void LogicTick(uint32_t dt);
void LogicProcessData(LC_NodeDescriptor_t *node, LC_Header_t header, void *data, int32_t size);
