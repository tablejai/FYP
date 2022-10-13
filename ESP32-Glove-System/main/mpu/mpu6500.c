#include "mpu6500.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t MPU_init(MPU_t* mpu, const int MOSI, const int MISO, const int SCLK, const int CS, spi_device_handle_t spi_handle) {
    ESP_ERROR_CHECK(spi_add_device(0, CLK_FREQ, CS, &spi_handle));
    mpu->spi_handle = spi_handle;
    mpu->calibration = (mpu_data_t){
        .accel = {
            .x = 0,
            .y = 0,
            .z = 0,
        },
        .gyro = {
            .x = 0,
            .y = 0,
            .z = 0,
        }};
    mpu->CS = CS;
    gpio_set_direction(mpu->CS, GPIO_MODE_OUTPUT);
    gpio_set_level(mpu->CS, 1);
    return ESP_OK;
}

esp_err_t MPU_whoami(const MPU_t* mpu, uint8_t* whoami) {
    gpio_set_level(mpu->CS, 0);
    ESP_ERROR_CHECK(spi_read_bytes(mpu->spi_handle, WHO_AM_I, 1, whoami));
    gpio_set_level(mpu->CS, 1);
    return ESP_OK;
}

esp_err_t MPU_read(const MPU_t* mpu, mpu_data_t* data) {
    int8_t err_flag = 0;
    gpio_set_level(mpu->CS, 0);

    uint8_t buffer[14] = {0};
    err_flag |= spi_read_bytes(mpu->spi_handle, RAW_DATA_START, 14, buffer) != ESP_OK;

    if (err_flag) {
        gpio_set_level(mpu->CS, 1);
        return ESP_FAIL;
    }

    data->accel.x = (buffer[0] << 8 | buffer[1]) / 16383.75;
    data->accel.y = (buffer[2] << 8 | buffer[3]) / 16383.75;
    data->accel.z = (buffer[4] << 8 | buffer[5]) / 16383.75;
    data->gyro.x = (buffer[8] << 8 | buffer[9]) / 131.0;
    data->gyro.y = (buffer[10] << 8 | buffer[11]) / 131.0;
    data->gyro.z = (buffer[12] << 8 | buffer[13]) / 131.0;

    MPU_filter(mpu, data);

    data->accel.x -= mpu->calibration.accel.x;
    data->accel.y -= mpu->calibration.accel.y;
    data->accel.z -= mpu->calibration.accel.z;
    data->gyro.x -= mpu->calibration.gyro.x;
    data->gyro.y -= mpu->calibration.gyro.y;
    data->gyro.z -= mpu->calibration.gyro.z;
    gpio_set_level(mpu->CS, 1);
    return ESP_OK;
}

esp_err_t MPU_filter(const MPU_t* mpu, mpu_data_t* data) {
    data->accel.x = data->accel.x > 2 ? data->accel.x - 4 : data->accel.x;
    data->accel.y = data->accel.y > 2 ? data->accel.y - 4 : data->accel.y;
    data->accel.z = data->accel.z > 2 ? data->accel.z - 4 : data->accel.z;
    data->gyro.x = data->gyro.x > 250 ? data->gyro.x - 500 : data->gyro.x;
    data->gyro.y = data->gyro.y > 250 ? data->gyro.y - 500 : data->gyro.y;
    data->gyro.z = data->gyro.z > 250 ? data->gyro.z - 500 : data->gyro.z;
    return ESP_OK;
}

esp_err_t MPU_calibrate(MPU_t* mpu) {
    mpu_data_t data = {0};
    mpu_data_t cal = {0};
    for (int i = 0; i < 200; i++) {
        MPU_read(mpu, &data);
        cal.accel.x += data.accel.x;
        cal.accel.y += data.accel.y;
        cal.accel.z += data.accel.z;
        cal.gyro.x += data.gyro.x;
        cal.gyro.y += data.gyro.y;
        cal.gyro.z += data.gyro.z;
        vTaskDelay(1);
    }
    mpu->calibration.accel.x = cal.accel.x / 200;
    mpu->calibration.accel.y = cal.accel.y / 200;
    mpu->calibration.accel.z = cal.accel.z / 200;
    mpu->calibration.gyro.x = cal.gyro.x / 200;
    mpu->calibration.gyro.y = cal.gyro.y / 200;
    mpu->calibration.gyro.z = cal.gyro.z / 200;
    return ESP_OK;
}

void print_mpu_accel(const mpu_accel_t data) {
    ESP_LOGI("MPU6500", "accel: %3.2f %3.2f %3.2f", data.x, data.y, data.z);
    // ESP_LOGI("MPU6500", "accel x: %3.2f", data.x);
    // ESP_LOGI("MPU6500", "accel y: %3.2f", data.y);
    // ESP_LOGI("MPU6500", "accel z: %3.2f", data.z);
}

void print_mpu_gyro(const mpu_gyro_t data) {
    ESP_LOGI("MPU6500", "gyro:  %3.2f %3.2f %3.2f", data.x, data.y, data.z);
    // ESP_LOGI("MPU6500", "gyro  x: %3.2f", data.x);
    // ESP_LOGI("MPU6500", "gyro  y: %3.2f", data.y);
    // ESP_LOGI("MPU6500", "gyro  z: %3.2f", data.z);
}

void print_mpu_rpy(const mpu_rpy_t data) {
    ESP_LOGI("MPU6500", "rpy:   %3.2f %3.2f %3.2f", data.roll, data.pitch, data.yaw);
    // ESP_LOGI("MPU6500", "rpy   x: %3.2f", data.roll);
    // ESP_LOGI("MPU6500", "rpy   y: %3.2f", data.pitch);
    // ESP_LOGI("MPU6500", "rpy   z: %3.2f", data.yaw);
}

void print_mpu_data(const mpu_data_t data) {
    // print_mpu_accel(data.accel);
    // print_mpu_gyro(data.gyro);
    // print_mpu_rpy(data.rpy);
    ESP_LOGI("MPU6500", "accel: %3.2f %3.2f %3.2f", data.accel.x, data.accel.y, data.accel.z);
    ESP_LOGI("MPU6500", "gyro:  %3.2f %3.2f %3.2f", data.gyro.x, data.gyro.y, data.gyro.z);
    ESP_LOGI("MPU6500", "rpy:   %3.2f %3.2f %3.2f", data.rpy.roll, data.rpy.pitch, data.rpy.yaw);
}
