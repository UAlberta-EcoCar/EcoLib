#ifndef ECOCAR_CAN_H_
#define ECOCAR_CAN_H_

// Only guaranteed to work if project is generated by 
// STM32CubeMX which is highly likely
#include "main.h"
#include "cmsis_os2.h"

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

typedef struct {
  union {
    struct {
      uint32_t fc_temp;
      uint32_t fc_press;
      uint32_t fan_rpm1;
      uint32_t fan_rpm2;
      uint32_t bme_temp;
      uint32_t bme_humid;
      uint32_t set_state;
      uint32_t FILLER;
    };
    uint8_t FDCAN_RawFccPack[FDCAN_BYTES_32];
  };
} FDCAN_FccPack_t;

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
