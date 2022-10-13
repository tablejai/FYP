#include <math.h>
#include <stdio.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "http.h"
#include "mpu6500.h"
#include "mpu6500_cluster.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "spi.h"
#include "wifi.h"

#define LED 38
#define MOSI 35
#define MISO 37
#define SCLK 36
#define CS1 17
#define CS2 16
#define CS3 15
#define CS4 14

#define CLUSTER_SIZE 3
uint8_t whoami = 0;
MPU_t mpu_list[CLUSTER_SIZE];
mpu_data_t mpu_data[CLUSTER_SIZE];
uint8_t CS[CLUSTER_SIZE] = {CS1, CS2, CS3};
spi_device_handle_t spi_handle;
char data_str[100];

void mpu6500_task(void* par) {
    ESP_ERROR_CHECK(spi_init(MOSI, MISO, SCLK, 128));

    for (int i = 0; i < CLUSTER_SIZE; i++) {
        MPU_init(&mpu_list[i], MOSI, MISO, SCLK, CS[i], spi_handle);
    }

    // /// create cluster
    MPU_Cluster_t* cluster = malloc(sizeof(MPU_Cluster_t));
    MPU_create_cluster(cluster, mpu_list, CLUSTER_SIZE);

    /// check who am i
    for (uint8_t i = 0; i < CLUSTER_SIZE; i++) {
        MPU_whoami(MPU_Cluster_get_node(cluster, i), &whoami);
        if (whoami != 0x70) {
            MPU_Cluster_get_node(cluster, i)->is_dead = 1;
            ESP_LOGE("MPU6500", "Wrong device ID[%d]: %02X", i, whoami);
        }
    }

    // /// calibrate
    ESP_LOGI("MPU6500", "MPU Cluster Calibration start");
    for (uint8_t i = 0; i < CLUSTER_SIZE; i++) {
        MPU_calibrate(MPU_Cluster_get_node(cluster, i));
        print_mpu_accel(MPU_Cluster_get_node(cluster, i)->calibration.accel);
        print_mpu_gyro(MPU_Cluster_get_node(cluster, i)->calibration.gyro);
    }
    ESP_LOGI("MPU6500", "MPU Cluster Calibration end");

    float previousTime = 0, currentTime = xTaskGetTickCount();
    const float dt = 100.0;
    while (1) {
        MPU_Cluster_read(cluster, mpu_data);

        previousTime = currentTime;
        currentTime = xTaskGetTickCount();
        float elapsedTime = (currentTime - previousTime) / 1000.0;

        for (int i = 0; i < CLUSTER_SIZE; i++) {
            mpu_data[i].rpy.roll += mpu_data[i].gyro.x * elapsedTime;
            mpu_data[i].rpy.pitch += mpu_data[i].gyro.y * elapsedTime;
            mpu_data[i].rpy.yaw += mpu_data[i].gyro.z * elapsedTime;
        }

        print_Cluster_data(mpu_data, CLUSTER_SIZE);
        sprintf(data_str, "{\n\"mpu0\":{\n%f\n%f\n%f\n%f\n%f\n%f\n}\"\n}",
                mpu_data[0].accel.x,
                mpu_data[0].accel.y,
                mpu_data[0].accel.z,
                mpu_data[0].gyro.x,
                mpu_data[0].gyro.y,
                mpu_data[0].gyro.z);
        post_function(data_str);

        // print_mpu_accel(mpu_data[0].accel);
        // print_mpu_gyro(mpu_data[0].gyro);
        // print_mpu_rpy(mpu_data[0].rpy);
        //     // print_mpu_data(mpu_data[0]);
        vTaskDelay(dt / portTICK_PERIOD_MS);
    }
}

void blink(void* par) {
    // init
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    uint8_t led_state = 0;
    while (1) {
        gpio_set_level(LED, led_state);
        led_state = !led_state;
        ESP_LOGI("blink_task", "stack\t%d byte", uxTaskGetStackHighWaterMark(NULL) * 4);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    res_init();

    wifi_init();

    // printf("Waiting for 2 seconds lmao \r\n");
    xTaskCreate(
        blink,
        "blink_task",
        3072,
        NULL,
        1,
        NULL);

    xTaskCreate(
        mpu6500_task,
        "mpu6500_task",
        6000,
        NULL,
        1,
        NULL);

    vTaskDelay(20000 / portTICK_PERIOD_MS);
}
