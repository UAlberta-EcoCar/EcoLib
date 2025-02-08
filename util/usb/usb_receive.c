#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include <stdio.h>
#include "main.h"
#include "exported_typedef.h"

/* All users must provide a queue for this thread
 * to get the USB string data from
 * */
extern osMessageQueueId_t usbQueReceiveHandle;

/* Add variables as necessary for your application
 * by toggling the commented defines
 * Just note that whatever variables are relevant
 * to your application should be defined here with
 * the extern key word */

/*#define NEED_FET_STATE_COMMAND 1*/
#ifdef NEED_FET_STATE_COMMAND
extern fetState_t fet_state;
#endif

/* Header_StartUsbReceive */
/**
 * @brief Function implementing the usbReceiveTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartUsbReceive */
void StartUsbReceive(void *argument) {
  /* USER CODE BEGIN StartAdcConv */

  char ret[64] = {0}; // Assume received strings are never longer
  uint8_t iter = 0;

  UNUSED(argument);
  /* Infinite loop */
  for (;;) {
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
    osDelay(1);
  }
  /* USER CODE END StartAdcConv */
}
