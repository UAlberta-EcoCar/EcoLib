#ifndef SX1281_REGISTERS_H
#define SX1281_REGISTERS_H

#include <stdint.h>

typedef enum {

    SX128X_RESERVED = 0x0,
    SX128X_TRANCIVER_COMMAND_PROCCESSED_SUCCESSFULLY = 0x1,
    SX128X_DATA_AVAILABLE_TO_HOST = 0x2,
    SX128X_COMMAND_TIMEOUT = 0x3,
    SX128X_COMMAND_PROCESSING_ERROR = 0x4,
    SX128X_COMMAND_EXECUTE_FAIL = 0x5,
    SX128X_COMMAND_TX_DONE = 0x6

} SX128X_Command_Status_t;

typedef enum {
 
    SX128X_RESERVED = 0x0,
    SX128X_RESERVED1 = 0x1,
    SX128X_STDBY_RC = 0x2,
    SX128X_STDBY_XOSC = 0x3,
    SX128X_FS = 0x4,
    SX128X_RX = 0x5,
    SX128X_TX = 0x6

} SX128X_Circuit_Mode_t;

/* OpCode Commands */


#define SX128X_GET_STATUS_COMMAND_OPCODE 0xC0 // returns status defined below
// OpCode Return Parameters
typedef struct {
    union {
        struct {
            uint8_t isBusy :1;
            uint8_t reserved :1;
            uint8_t command_status :3; // defined in command status enum
            uint8_t circuit_mode :3; // defined in circuit mode enum
        };
        uint8_t sx128x_status_raw;
    };
} SX128X_Get_Status_Command_t;

#define SX128X_WRITE_REGISTER_COMMAND_OPCODE 0x18 // returns status as defined in enum

#define SX128X_READ_REGISTER_COMMAND_OPCODE 0x19 // returns status as defined in enum

#define SX128X_WRITE_BUFFER_COMMAND_OPCODE 0x1A // returns status as defined in enum

#define SX128X_READ_BUFFER_COMMAND_OPCODE 0x1B // returns status as defined in enum

typedef struct {
    union {
        struct {
            uint8_t isRAMretained :1;
            uint8_t isDataBufferretained :1;
        };
        uint8_t sx128x_sleep_config_raw;
    };
} SX128X_Sleep_Mode_Config_t;


// 11.6.2 SetStandby

#define SX128X_SET_STANDBY_COMMAND_OPCODE 0x80 // returns status as defined in enum

typedef enum{
    STDBY_RC = 0,
    STDBY_XOSC = 1
} SX128X_Standby_t;

typedef struct {
    union{
        struct {
            SX128X_Standby_t standbyConfig :1;
        };
        uint8_t sx128x_standby_config_raw;
    };
} SX128X_Standby_Mode_Config_t;


// Used for SetTx, SetRx, SetRxDutyCycle, SetTxDutyCycle

typedef enum{
    SX128X_PERIODBASE_15_625μs = 0x00,
    SX128X_PERIODBASE_62_5μs = 0x01,
    SX128X_PERIODBASE_1ms = 0x02,
    SX128X_PERIODBASE_4ms = 0x03
} SX128X_PeriodBase_t;

// 11.6.4 SetTx - 11.6.5 SetRx

#define SX128X_SET_TX_COMMAND_OPCODE 0x83 // Table 11-22
#define SX128X_SET_RX_COMMAND_OPCODE 0x82 // Table 11-26

#define SX128X_PERIODBASECOUNT_NOTIMEOUT 0x00 // no time out TX, single mode RX Table 11-25
#define SX128X_TICKNUM_RX_CONTINUOUS 0xFF // Table 11-28

typedef struct {
    union{
        struct {
            SX128X_PeriodBase_t sx1281_periodBase;
            uint8_t sx1281_periodBaseCount_15_8:8;
            uint8_t sx1281_periodBaseCount_7_0:8;
        };
        uint32_t sx1281_periodBaseCount_config_raw;
    };
} SX128X_TX_RX_Mode_Config_t;


// 11.6.6 SetRxDutyCycle

#define SX128X_SET_RX_DUTY_CYCLE 0x94 // Table 11-29

#define SX128X_RX_PERIODBASECOUNT_NOTIMEOUT 0x00 // no time out TX Table 11-31

typedef struct {
    union{
        struct {
            uint8_t rxPeriodBaseCount_15_8:8;
            uint8_t rxPeriodBaseCount_15_8:8;
            uint8_t sleepPeriodBaseCount_15_8:8;
            uint8_t sleepPeriodBaseCount_7_0:8;
        };
        uint32_t sx1281_periodBaseCount_config_raw;
    };
} SX128X_TX_RX_Duty_Cycle_Config_t;


// 11.6.7 SetLongPreamble

#endif