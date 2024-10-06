#pragma once

#define SERIAL_BASE 0x86001000
#define SERIAL_FLAG_REGISTER 0x18
#define SERIAL_BUFFER_FULL (1 << 5)

#define PIC_BASE 0x9000B200
#define HCD_DESIGNWARE_BASE ((void*)0x90980000)
#define BOARD_LOAD_BASE 0

