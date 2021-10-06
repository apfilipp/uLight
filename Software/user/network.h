#include "levcan.h"

#pragma once
extern LC_NodeDescriptor_t *LevcanNodePtr;
void Network_Update(uint32_t tick);
void Network_Init(uint8_t node_id);
