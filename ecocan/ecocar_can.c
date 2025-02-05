#include "ecocar_can.h"
#include <stdint.h>

fdcanBytes_t mapDlcToBytes(uint32_t fdcanDlc) {
  fdcanBytes_t bytes;
  if (fdcanDlc <= FDCAN_DLC_BYTES_8) {
    return fdcanDlc;
  } else {
    switch (fdcanDlc) {
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
