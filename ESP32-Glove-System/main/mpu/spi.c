#include "spi.h"

#define MPU_SPI_HOST SPI2_HOST

esp_err_t spi_init(const int MOSI_PIN, const int MISO_PIN, const int SCLK_PIN, const int max_transfer_sz) {
    spi_bus_config_t config;
    config.mosi_io_num = MOSI_PIN;
    config.miso_io_num = MISO_PIN;
    config.sclk_io_num = SCLK_PIN;
    config.quadwp_io_num = -1;  // -1 not used
    config.quadhd_io_num = -1;  // -1 not used
    config.max_transfer_sz = max_transfer_sz;
    return spi_bus_initialize(MPU_SPI_HOST, &config, 0);  // 0 DMA not used
}

esp_err_t spi_deinit() {
    return spi_bus_free(MPU_SPI_HOST);
}

esp_err_t spi_add_device(uint8_t mode, uint32_t clock_speed_hz, int CS_PIN, spi_device_handle_t *handle) {
    spi_device_interface_config_t dev_config;
    dev_config.command_bits = 0;
    dev_config.address_bits = 8;
    dev_config.dummy_bits = 0;
    dev_config.mode = mode;
    dev_config.duty_cycle_pos = 128;  // default 128 = 50%/50% duty
    dev_config.cs_ena_pretrans = 0;   // 0 not used
    dev_config.cs_ena_posttrans = 0;  // 0 not used
    dev_config.clock_speed_hz = clock_speed_hz;
    dev_config.spics_io_num = CS_PIN;
    dev_config.flags = 0;  // 0 not used
    dev_config.queue_size = 1;
    dev_config.pre_cb = NULL;
    dev_config.post_cb = NULL;
    return spi_bus_add_device(MPU_SPI_HOST, &dev_config, handle);
}

esp_err_t spi_remove_device(spi_device_handle_t *handle) {
    return spi_bus_remove_device(*handle);
}

/*******************************************************************************
 * WRITING
 ******************************************************************************/
esp_err_t spi_write_bytes(spi_device_handle_t handle, uint8_t reg_addr, size_t length, const uint8_t *data) {
    spi_transaction_t transaction;
    transaction.flags = 0;
    transaction.cmd = 0;
    transaction.addr = reg_addr & SPIBUS_WRITE;
    transaction.length = length * 8;
    transaction.rxlength = 0;
    transaction.user = NULL;
    transaction.tx_buffer = data;
    transaction.rx_buffer = NULL;
    esp_err_t err = spi_device_transmit(handle, &transaction);
    return err;
}

/*******************************************************************************
 * READING
 ******************************************************************************/
esp_err_t spi_read_bytes(spi_device_handle_t handle, uint8_t reg_addr, size_t length, uint8_t *data) {
    if (length == 0)
        return ESP_ERR_INVALID_SIZE;
    spi_transaction_t transaction;
    transaction.flags = 0;
    transaction.cmd = 0;
    transaction.addr = reg_addr | SPIBUS_READ;
    transaction.length = length * 8;
    transaction.rxlength = length * 8;
    transaction.user = NULL;
    transaction.tx_buffer = NULL;
    transaction.rx_buffer = data;
    esp_err_t err = spi_device_transmit(handle, &transaction);
    return err;
}
