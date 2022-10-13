#ifndef MPU_H
#define MPU_H

#include <stdint.h>
#include <stdio.h>

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "spi.h"

#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02
#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define RAW_DATA_START ACCEL_XOUT_H

#define WHO_AM_I 0x75

#define CLK_FREQ 1 * 1000000

typedef struct {
    float x;
    float y;
    float z;
} mpu_accel_t;

typedef struct {
    float x;  // pitch
    float y;  // roll
    float z;  // yaw
} mpu_gyro_t;

typedef struct {
    float roll;
    float pitch;
    float yaw;
} mpu_rpy_t;

typedef struct {
    mpu_accel_t accel;
    mpu_gyro_t gyro;
    mpu_rpy_t rpy;
} mpu_data_t;

typedef struct {
    spi_device_handle_t spi_handle;
    mpu_data_t calibration;
    uint32_t last_tick;
    uint8_t is_dead;
    uint8_t CS;
} MPU_t;

esp_err_t MPU_init(MPU_t* mpu, const int MOSI, const int MISO, const int SCLK, const int CS, spi_device_handle_t spi_handle);
esp_err_t MPU_whoami(const MPU_t* mpu, uint8_t* whoami);
esp_err_t MPU_read(const MPU_t* mpu, mpu_data_t* data);
esp_err_t MPU_filter(const MPU_t* mpu, mpu_data_t* data);
esp_err_t MPU_calibrate(MPU_t* mpu);

void print_mpu_accel(const mpu_accel_t data);
void print_mpu_gyro(const mpu_gyro_t data);
void print_mpu_rpy(const mpu_rpy_t data);
void print_mpu_data(const mpu_data_t data);
#endif