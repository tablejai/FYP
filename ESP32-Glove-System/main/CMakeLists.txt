idf_component_register(SRCS "main.c" "wifi/wifi.c" "nvs/nvs.c" "http/http.c" "mpu/mpu6500_cluster.c" "mpu/mpu6500.c" "mpu/spi.c"
                    INCLUDE_DIRS "." "./wifi" "./nvs" "./http" "./mpu")

