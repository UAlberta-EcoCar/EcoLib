
// Typedefines for FET board state
typedef enum {
  ALL_FET_OFF = 0x00,
  FUELCELL_FET = 0x01,
  CAP_FET = 0x02,
  RES_FET = 0x04,
  OUT_FET = 0x08,
} fetBit_t;

typedef enum {
  FET_STBY = ALL_FET_OFF,
  FET_CHRGE = FUELCELL_FET | CAP_FET | RES_FET,
  FET_RUN = FUELCELL_FET | CAP_FET | RES_FET | OUT_FET,
} fetState_t;
