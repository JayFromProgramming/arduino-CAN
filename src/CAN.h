// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef CAN_H
#define CAN_H

#ifdef USE_ESP32SJA1000
    #ifndef ARDUINO_ARCH_ESP32
    #error "Arduino-Jay-CAN: ESP32SJA1000 is only supported on the ESP32 platform"
    #endif
	#include "ESP32SJA1000.h"
#else
	#include "MCP2515.h"
#endif
#endif
