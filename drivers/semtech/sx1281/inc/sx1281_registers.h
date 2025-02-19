#ifndef SX1281_REGISTERS_H
#define SX1281_REGISTERS_H

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
        }
        uint8_t sx128x_status_raw;
    }
} SX128X_Get_Status_Command_t;

#define SX128X_WRITE_REGISTER_COMMAND_OPCODE 0x18 // returns status as defined in enum

#define SX128X_READ_REGISTER_COMMAND_OPCODE 0x19 // returns status as defined in enum

#define SX128X_WRITE_BUFFER_COMMAND_OPCODE 0x1A // returns status as defined in enum

#define SX128X_READ_BUFFER_COMMAND_OPCODE 0x1B // returns status as defined in enum

typedef struct {

    struct {
        uint8_t isRAMretained :1;
        uint8_t isDataBufferretained :1;
    }
    uint8_t sx128x_sleep_config_raw;

} SX128X_Sleep_Mode_Config_t;

#endif