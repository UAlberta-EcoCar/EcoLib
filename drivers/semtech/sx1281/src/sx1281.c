#include "sx1281_registers.h"
#include "sx128x.h"

#include "ansi-codes.h"

#define LOG_ERROR(M, ...) printf("[Abi's RFlib SX128X]" SETFG_RED "[ERROR] (%s:%d) " M GR_RESET "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_WARN(M, ...) printf("[Abi's RFlib SX128X]" SETFG_YELLOW "[WARN] " M GR_RESET "\r\n", ##__VA_ARGS__)

#define LOG_INFO(M, ...) printf("[Abi's RFlib SX128X][INFO] " M "\r\n", ##__VA_ARGS__)

int rf24_initialize_radio(rf24_handle_t *_rf24_handle) {

	HAL_GPIO_WritePin(_rf24_handle->rf24_nss_port, _rf24_handle->rf24_nss_pin,
			GPIO_PIN_SET);
	HAL_GPIO_WritePin(_rf24_handle->rf24_reset_port,
			_rf24_handle->rf24_reset_pin, GPIO_PIN_SET); // Reset when low

	if (_rf24_handle->rf24_rxtx_enable) {
		//
	}

	return RF24_OK;
}

int rf24_reset(rf24_handle_t *_rf24_handle) {

	_rf24_handle->rf24_delay_func(20);
	HAL_GPIO_WritePin(_rf24_handle->rf24_reset_port,
			_rf24_handle->rf24_reset_pin, GPIO_PIN_RESET);
	_rf24_handle->rf24_delay_func(50);
	HAL_GPIO_WritePin(_rf24_handle->rf24_reset_port,
			_rf24_handle->rf24_reset_pin, GPIO_PIN_SET);
	_rf24_handle->rf24_delay_func(20);

	return RF24_OK;

}

int rf24_check_device(rf24_handle_t *_rf24_handle) {

//	 try to ping the device and verify.

	uint8_t Regdata1, Regdata2;
	rf24_spi_read_register(_rf24_handle, 0x0908, &Regdata1); //low byte of frequency setting
	rf24_spi_write_register(_rf24_handle, 0x0908, (Regdata1 + 1));
	rf24_spi_read_register(_rf24_handle, 0x0908, &Regdata2); //read changed value back
	rf24_spi_write_register(_rf24_handle, 0x0908, Regdata1); //restore register to original value

	if (Regdata2 == (Regdata1 + 1)) {
		return RF24_OK;
	} else {
		return RF24_ERROR;
	}
}

int rf24_check_busy(rf24_handle_t *rf24_handle) {

	uint32_t startmS = millis();

//	do {
//		if (((uint32_t) (millis() - startmS) > 9)) // wait 10mS for busy to complete
//		{
////			Serial.println(F("ERROR - Busy Timeout!"));
//			resetDevice();
//			setMode(MODE_STDBY_RC);
//			config();  // re-run saved config
//			break;
//		}
//	} while (digitalRead(_RFBUSY));

	return RF24_OK;
}

// 8MHz max speed MSBBFIRST and SPIMODE0

int rf24_spi_read_registers(rf24_handle_t *rf24_handle,
		uint16_t rf24_register_address, uint8_t *rf24_register_result,
		uint16_t rf24_read_size) {
	uint8_t addr_l, addr_h;

	addr_h = rf24_register_address >> 8;
	addr_l = rf24_register_address & 0x00FF;
	rf24_check_busy(rf24_handle);

	HAL_GPIO_WritePin(rf24_handle->rf24_nss_port, rf24_handle->rf24_nss_pin,
			GPIO_PIN_RESET);

	uint8_t prep_register[4] = { SX128X_READ_REGISTER_COMMAND_OPCODE, addr_h,
			addr_l, 0xFF };

	if (HAL_SPI_Transmit(rf24_handle->rf24_spi_handle, prep_register[0], 1,
			rf24_handle->rf24_spi_timeout) != HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", rf24_register_address);
		return RF24_ERROR;
	}

	if (HAL_SPI_Transmit(rf24_handle->rf24_spi_handle, prep_register[1], 1,
			rf24_handle->rf24_spi_timeout) != HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", rf24_register_address);
		return RF24_ERROR;
	}

	if (HAL_SPI_Transmit(rf24_handle->rf24_spi_handle, prep_register[2], 1,
			rf24_handle->rf24_spi_timeout) != HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", rf24_register_address);
		return RF24_ERROR;
	}

	if (HAL_SPI_Transmit(rf24_handle->rf24_spi_handle, prep_register[3], 1,
			rf24_handle->rf24_spi_timeout) != HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", rf24_register_address);
		return RF24_ERROR;
	}

	for (uint8_t i = 0; i < rf24_read_size; i++) {
		// send a 0xFF to read the response from the module?
		if (HAL_SPI_Transmit(rf24_handle->rf24_spi_handle, prep_register[3], 1,
				rf24_handle->rf24_spi_timeout) != HAL_OK) {
			LOG_ERROR("Failed to read register 0x%x", rf24_register_address);
			return RF24_ERROR;
		}

		// will return register contents.
		if (HAL_SPI_Receive(rf24_handle->rf24_spi_handle,
				rf24_register_result[i], 1, rf24_handle->rf24_spi_timeout)
				!= HAL_OK) {
			LOG_ERROR("Failed to read register 0x%x", rf24_register_address);
			return RF24_ERROR;
		}
	}

	HAL_GPIO_WritePin(rf24_handle->rf24_nss_port, rf24_handle->rf24_nss_pin,
			GPIO_PIN_SET);

	return RF24_OK;
}

int rf24_spi_read_register(rf24_handle_t *rf24_handle,
		uint16_t rf24_register_address, uint8_t *rf24_register_result) {

	return rf24_spi_read_registers(rf24_handle, rf24_register_address,
			rf24_register_result, 1);
}

int rf24_spi_write_registers(rf24_handle_t *rf24_handle,
		uint16_t rf24_register_address, uint8_t *rf24_register_to_write_buffer,
		uint16_t rf24_write_size) {
	uint8_t addr_l, addr_h;

	addr_l = rf24_register_address & 0xff;
	addr_h = rf24_register_address >> 8;
	checkBusy();

	HAL_GPIO_WritePin(rf24_handle->rf24_nss_port, rf24_handle->rf24_nss_pin,
			GPIO_PIN_RESET);

	uint8_t prep_register[3] = { SX128X_WRITE_REGISTER_COMMAND_OPCODE, addr_h,
			addr_l };

	if (HAL_SPI_Transmit(rf24_handle->rf24_spi_handle, prep_register, 3,
			rf24_handle->rf24_spi_timeout) != HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", rf24_register_address);
		return RF24_ERROR;
	}

	if (HAL_SPI_Transmit(rf24_handle->rf24_spi_handle,
			rf24_register_to_write_buffer, rf24_write_size,
			rf24_handle->rf24_spi_timeout) != HAL_OK) {
		LOG_ERROR("Failed to read register 0x%x", rf24_register_address);
		return RF24_ERROR;
	}

	HAL_GPIO_WritePin(rf24_handle->rf24_nss_port, rf24_handle->rf24_nss_pin,
			GPIO_PIN_SET);
	return 1;
}

int rf24_spi_write_register(rf24_handle_t *rf24_handle,
		uint16_t rf24_register_address, uint8_t rf24_register_to_write_value) {
	return rf24_spi_write_registers(rf24_handle, rf24_register_address,
			&rf24_register_to_write_value, 1);
}

int rf24_write_command(rf24_handle_t *rf24_handle, uint8_t rf24_command_address,
		uint8_t *rf24_command_buffer, uint16_t rf_command_size) {

	rf24_check_busy(rf24_handle);

	return RF24_OK;
}

int rf24_set_packet_parameters(rf24_handle_t *rf24_handle,
		uint8_t rf24_preamble_length_mantissa,
		uint8_t rf24_preamble_length_exponent, uint8_t rf24_header_mode,
		uint8_t rf24_payload_length, uint8_t rf24_lora_crc,
		uint8_t rf24_lora_iq) {

	uint8_t buffer[7];

	buffer[0] = rf24_preamble_length_mantissa * 2
			^ (rf24_preamble_length_exponent);
	buffer[1] = rf24_header_mode;
	buffer[2] = rf24_payload_length;
	buffer[3] = rf24_lora_crc;
	buffer[4] = rf24_lora_iq;

	rf24_write_command(rf24_handle, SX128X_SET_PACKET_PARAMETERS_COMMAND_OPCODE,
			buffer, 5);
}

//int rf24_set_dio_parameters(rf24_handle_t *rf24_handle,
//		SX128X_IRQ_Register_t rf24_mode) {
//
//}

int rf24_set_high_sensitivity_rx(rf24_handle_t *rf24_handle,
		uint8_t rf24_set_high_sensitivity) {

	uint8_t lna_regime = 0;

	if (RF24_ERROR
			== rf24_spi_read_register(rf24_handle, SX128X_LNA_REGIME_REGISTER,
					&lna_regime)) {
		LOG_ERROR("SPI ERROR");
	}

	if (1 == rf24_set_high_sensitivity) {
		lna_regime = lna_regime | SX128X_LNA_REGIME_LNA_ENABLE_OR; // enable the LNA GAIN
	} else {
		lna_regime = lna_regime & SX128X_LNA_REGIME_LNA_DISABLE_AND;
	}

	if (RF24_ERROR
			== rf24_spi_write_register(rf24_handle, SX128X_LNA_REGIME_REGISTER,
					lna_regime)) {
		LOG_ERROR("SPI ERROR");
	}

	return RF24_OK;

}
