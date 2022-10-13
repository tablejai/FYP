#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stdio.h>

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define SPIBUS_READ (0x80)  /*!< addr | SPIBUS_READ  */
#define SPIBUS_WRITE (0x7F) /*!< addr & SPIBUS_WRITE */

esp_err_t spi_init(const int MOSI_PIN, const int MISO_PIN, const int SCLK_PIN, const int max_transfer_sz);
esp_err_t spi_deinit();

esp_err_t spi_add_device(uint8_t mode, uint32_t clock_speed_hz, int CS_PIN, spi_device_handle_t *handle);
esp_err_t spi_remove_device(spi_device_handle_t *handle);

esp_err_t spi_write_bytes(spi_device_handle_t handle, uint8_t reg_addr, size_t length, const uint8_t *data);
esp_err_t spi_read_bytes(spi_device_handle_t handle, uint8_t reg_addr, size_t length, uint8_t *data);

#endif