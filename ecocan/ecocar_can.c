#include "ecocar_can.h"

fdcanBytes_t mapDlcToBytes(FDCAN_RxHeaderTypeDef *hfdcan) {
  fdcanBytes_t bytes;
  if (hfdcan->DataLength <= FDCAN_DLC_BYTES_8) {
    return hfdcan->DataLength;
  } else {
    switch (hfdcan->DataLength) {
    case FDCAN_DLC_BYTES_12:
      bytes = FDCAN_BYTES_12;
      break;
    case FDCAN_DLC_BYTES_16:
      bytes = FDCAN_BYTES_16;
      break;
    case FDCAN_DLC_BYTES_20:
      bytes = FDCAN_BYTES_20;
      break;
    case FDCAN_DLC_BYTES_24:
      bytes = FDCAN_BYTES_24;
      break;
    case FDCAN_DLC_BYTES_32:
      bytes = FDCAN_BYTES_32;
      break;
    case FDCAN_DLC_BYTES_48:
      bytes = FDCAN_BYTES_48;
      break;
    case FDCAN_DLC_BYTES_64:
      bytes = FDCAN_BYTES_64;
      break;
    }
  }
  return bytes;
}
