/*
 * EcoCar FDCAN Ids
 * created by: Nicholas Semmens
 */

#ifndef ECOCAR_CAN_H_
#define ECOCAR_CAN_H_

// Only guaranteed to work if project is generated by
// STM32CubeMX which is highly likely
#include "cmsis_os2.h"
#include "main.h"

// Multiply floats by this value to have precision when transfered
// on CANBUS
#define FDCAN_FOUR_FLT_PREC 10000

#define ECOCAN_3_FLT_PREC 1000

// Structs must be a certain size for FDCAN to transfer
// The following package sizes (in bytes) are 0, 1, 2, 3, 4, 5, 6,
// 7, 8, 12, 16, 20, 24, 32, 48, 64.

typedef enum {
	FDCAN_BYTES_0 = 0,
	FDCAN_BYTES_1 = 1,
	FDCAN_BYTES_2 = 2,
	FDCAN_BYTES_3 = 3,
	FDCAN_BYTES_4 = 4,
	FDCAN_BYTES_5 = 5,
	FDCAN_BYTES_6 = 6,
	FDCAN_BYTES_7 = 7,
	FDCAN_BYTES_8 = 8,
	FDCAN_BYTES_12 = 12,
	FDCAN_BYTES_16 = 16,
	FDCAN_BYTES_20 = 20,
	FDCAN_BYTES_24 = 24,
	FDCAN_BYTES_32 = 32,
	FDCAN_BYTES_48 = 48,
	FDCAN_BYTES_64 = 64
} fdcanBytes_t;

// Highest priority CAN messages
// ranging from 0x000 to 0x00F
// All boards must accept these
// messages
#define FDCAN_H2ALARM_ID 0x001 // 1 indicates tripped alarm
#define FDCAN_SYNCLED_ID 0x00F // 1 indicates led on

// HOW TO USE THESE ODD STRUCTS
// In your main program use any of the below typedefs
// and define a new struct such as:
// FDCAN_FetPack_t mypack = {0};
// This will initialize all the values to zero. You can
// then assign values like usual:
// mypack.input_volt = ...;
// Note that if you are assigning a float, multiply it
// by 10000 for four points of precision stored in the
// uint32_t. It should look something like this:
// float myVoltage = 3.123451239804f;
// mypack.input_volt = (uint32_t)(myVoltage * 10000);
// So if you printed this value you would see 31234
// stored as the unsigned integer. Once you have
// assigned all the values you can pass the pointer
// to FDCAN_RawFetPack in the FDCAN transmit function
// as the data argument like this:
// ...function_name(...arguments, &mypack.FDCAN_RawFetPack)

// Reserved IDs up to 0x01F
// 0x010 = 0b00000010000
// 0x01F = 0b00000011111
// To receive all can filter ids within
// this range you must set the mask to
// 0x7F0 = 0b11111110000
// because you care that the bits [10:4]
// of the can id are exactly the same as
// bits [10:4] in 0x010/0x01F but the last
// four bits [3:0] can be 0 or 1
// The same logic will be applied henceforth
#define FDCAN_FETPACK_ID 0x010
typedef struct {
	union {
		struct {
			uint32_t fet_config;
			uint32_t input_volt;
			uint32_t cap_volt;
			uint32_t cap_curr;
			uint32_t res_curr;
			uint32_t out_curr;
		};
		uint8_t FDCAN_RawFetPack[FDCAN_BYTES_24];
	};
} FDCAN_FetPack_t;

#define FDCAN_RELPACKMTR_ID 0x015
typedef struct {
	union {
		struct {
			uint32_t mtr_volt;
			uint32_t mtr_curr;
		};
	};
	uint8_t FDCAN_RawRelPackMtr[FDCAN_BYTES_8];
} FDCAN_RelPackMtr_t;
#define FDCAN_RELPACKCAP_ID 0x016
typedef struct {
	union {
		struct {
			uint32_t cap_volt;
			uint32_t cap_curr;
		};
	};
	uint8_t FDCAN_RawRelPackCap[FDCAN_BYTES_8];
} FDCAN_RelPackCap_t;
#define FDCAN_RELPACKFC_ID 0x017
typedef struct {
	union {
		struct {
			uint32_t fc_volt;
			uint32_t fc_curr;
		};
	};
	uint8_t FDCAN_RawRelPackFc[FDCAN_BYTES_8];
} FDCAN_RelPackFc_t;
#define FDCAN_RELSTATE_ID 0x018

// Reserved IDs up to 0x02F
// 0x020 = 0b00000100000
// 0x02F = 0b00000101111
// Mask: 0x7F0
#define FDCAN_FCCPACK1_ID 0x020
typedef struct {
	union {
		struct {
			int32_t fc_temp;
			uint32_t fc_press;
		};
		uint8_t FDCAN_RawFccPack[FDCAN_BYTES_8];
	};
} FDCAN_FccPack1_t;

#define FDCAN_FCCPACK2_ID 0x021
typedef struct {
	union {
		struct {
			uint32_t fan_rpm1;
			uint32_t fan_rpm2;
		};
		uint8_t FDCAN_RawFccPack[FDCAN_BYTES_8];
	};
} FDCAN_FccPack2_t;

#define FDCAN_FCCPACK3_ID 0x022
typedef struct {
	union {
		struct {
			uint32_t bme_temp;
			uint32_t bme_humid;
		};
		uint8_t FDCAN_RawFccPack[FDCAN_BYTES_8];
	};
} FDCAN_FccPack3_t;

// Reserved IDs up to 0x03F
// 0x030 = 0b00001000000
// 0x03F = 0b00001001111
// Mask: 0x7F0
#define ECOCAN_H2_PACK1_ID 0x030
typedef struct {
	union {
		struct {
			uint16_t h2_sense_1;
			uint16_t h2_sense_2;
			uint16_t h2_sense_3;
			uint16_t h2_sense_4;
		};
		uint8_t ECOCAN_raw_pack[FDCAN_BYTES_8];
	};
} ECOCAN_H2Pack1_t;

#define ECOCAN_H2_PACK2_ID 0x031
typedef struct {
	union {
		struct {
			uint16_t bme_temp;
			uint16_t bme_humid;
			uint16_t imon_7v;
			uint16_t imon_12v;
		};
		uint8_t ECOCAN_raw_pack[FDCAN_BYTES_8];
	};
} ECOCAN_H2Pack2_t;

#define FDCAN_BOOSTPACK_ID 0x040
typedef struct {
	union {
		struct {
			uint32_t in_curr;
			uint32_t in_volt;
		};
		uint8_t FDCAN_RawBOOSTPack[FDCAN_BYTES_8];
	};
} FDCAN_BOOSTPack_t;

#define FDCAN_BOOSTPACK2_ID 0x041
typedef struct {
	union {
		struct {
			uint32_t out_curr;
			uint32_t out_volt;
		};
		uint8_t FDCAN_RawBOOSTPack2[FDCAN_BYTES_8];
	};
} FDCAN_BOOSTPack2_t;

/* The stm32g4xx_fdcan_hal.h file provides enumerated
 * DLC defines but they don't correspond to the actual
 * number of bytes in the FDCAN frame so we need to map
 * the values 0..15 to 0,1,2...64 as shown by the
 * enumeration in ecocar_can.h
 * */

/// Map FDCAN enum used by peripheral to actual num of bytes in FDCAN frame
/// \param[in]    RxHeader.Datalength
/// \return value of FDCAN_BYTES enumeration in ecocar_can.h
fdcanBytes_t mapDlcToBytes(uint32_t fdcanDlc);

#endif
