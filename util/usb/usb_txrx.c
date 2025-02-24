#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "exported_typedef.h"
#include "main.h"
#include <stdio.h>
#include "usbd_cdc_if.h"

/* Users must provide these char queues */
extern osMessageQueueId_t usbQueReceiveHandle;
extern osMessageQueueId_t usbQueSendHandle;

/* Add variables as necessary for your application
 * by toggling the commented defines
 * Just note that whatever variables are relevant
 * to your application should be defined here with
 * the extern key word */

#define NEED_FET_STATE_COMMAND
#ifdef NEED_FET_STATE_COMMAND
extern fetState_t fet_state;
#endif

// Syscall implementation that printf will use
int _write(int file, char *ptr, int len) {
  UNUSED(file);
  for (uint32_t i = 0; i < (uint32_t)len; i++) {
    if (osMessageQueuePut(usbQueSendHandle, ptr + i, 0, 0) != osOK) {
      Error_Handler();
    }
  }
  return len;
}

// Remember to put data into usbQueReceiveHandle from
// CDC_Receive_FS function in usbd_cdc_if.c
void doReceiveUsbTask(void);

// User can implement their own usb send task
__weak void doSendUsbTask(void) {
  char ret[64];
  uint8_t iter = 0;
  if (osMessageQueueGetCount(usbQueSendHandle) > 0) {
    osDelay(10); // let queue fill up just in case characters are just beginning
                 // to enter the queue
    do {
      osMessageQueueGet(usbQueSendHandle, &ret[iter], 0, 0);
    } while (ret[iter++] != '\n');
    // TODO: Verify the iter length is proper for CDC
    CDC_Transmit_FS((uint8_t *)ret, iter);
  }
}

/* Header_StartUsbReceive */
/**
 * @brief Function implementing the usbReceiveTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartUsbReceive */
void StartUsb(void *argument) {
  /* USER CODE BEGIN StartAdcConv */
  UNUSED(argument);
  /* Infinite loop */
  for (;;) {
    doReceiveUsbTask();
    doSendUsbTask();
    osDelay(1);
  }
  /* USER CODE END StartAdcConv */
}

void doReceiveUsbTask(void) {
  char ret[64] = {0}; // Assume received strings are never longer
  uint8_t iter = 0;
  if (osMessageQueueGetCount(usbQueReceiveHandle) > 0) {
    do {
      osMessageQueueGet(usbQueReceiveHandle, &ret[iter], 0, 0);
    } while (ret[iter++] != '\n');
    ret[iter] = '\0';
    iter = 0;

    // If a prefix for a command is found
    if (ret[0] == ':') {
      switch (ret[1]) {
#ifdef NEED_FET_STATE_COMMAND
      case 'L':
        switch (ret[3]) {
        case '0':
          fet_state = FET_STBY;
          printf("VALID: Vehicle in STANDBY\r\n");
          break;
        case '1':
          fet_state = FET_CHRGE;
          printf("VALID: Vehicle in CHARGE\r\n");
          break;
        case '2':
          fet_state = FET_RUN;
          printf("VALID: Vehicle in RUN\r\n");
          break;
        default:
          printf("ERROR: Invalid command input: %s\r\n", ret);
          break;
        }
        break;
#endif
      default:
        printf("ERROR: Invalid command input: %s\r\n", ret);
      }
    } else {
      printf("ECHO: %s", ret);
    }
  };
}
