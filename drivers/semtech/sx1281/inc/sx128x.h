#ifndef __SX128X_H__
#define __SX128X_H__

#include "stm32g4xx_hal.h"

typedef void (*rf24_delay_func_t)(uint32_t);

typedef void (*rf24_post_init_func_t)();

typedef struct {
	SPI_HandleTypeDef *rf24_spi_handle;

	uint32_t rf24_spi_timeout;

	GPIO_TypeDef *rf24_txen_port;

	uint16_t rf24_txen_pin;

	GPIO_TypeDef *rf24_rxen_port;

	uint16_t rf24_rxen_pin;

	GPIO_TypeDef *rf24_nss_port;

	uint16_t rf24_nss_pin;

	GPIO_TypeDef *rf24_reset_port;

	uint16_t rf24_reset_pin;

	rf24_post_init_func_t rf24_post_init_clbk;

	rf24_delay_func_t rf24_delay_func;

	uint8_t rf24_module_identifier;

	uint32_t rf24_carrier_frequency;

} RF24_Handle_t;

typedef enum {
	RF24_OK = 0, RF24_ERROR = 1
} RF24_Error_t;

int rf24_initialize_radio(RF24_Handle_t *_rf_handle);

#endif //__SX128X_H__
