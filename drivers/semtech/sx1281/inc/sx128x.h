#ifndef __SX128X_H__
#define __SX128X_H__

typedef void (*rf24_delay_func_t)(uint32_t);

typedef void (*rf24_post_init_func_t)();

typedef struct {
	SPI_HandleTypeDef *rf24_spi_handle;

	uint32_t rf24_spi_timeout;

	GPIO_TypeDef *rf24_nss_port;

	uint16_t rf24_nss_pin;

	GPIO_TypeDef *rf24_nreset_port;

	uint16_t rf24_nreset_pin;

	rf24_post_init_func_t rf24_post_init_clbk;

	rf24_delay_func_t rf24_delay_func;

	uint8_t rf24_module_identifier;

	uint32_t rf24_carrier_frequency;

} rf24_handle_t;

int rf24_initialize_radio() {

}

#endif //__SX128X_H__
