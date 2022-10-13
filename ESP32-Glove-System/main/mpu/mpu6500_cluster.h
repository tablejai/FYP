#ifndef MPU6500_CLUSTER_H
#define MPU6500_CLUSTER_H
#include "mpu6500.h"

typedef struct {
    MPU_t* mpu;
    int size;
} MPU_Cluster_t;

esp_err_t MPU_create_cluster(MPU_Cluster_t* cluster, MPU_t* mpu, const int cluster_size);
esp_err_t MPU_remove_cluster(MPU_Cluster_t* cluster);

esp_err_t MPU_Cluster_add_node(MPU_Cluster_t* cluster, const MPU_t* mpu, const int node_id);
esp_err_t MPU_Cluster_remove_node(MPU_Cluster_t* cluster, const MPU_t* mpu, const int node_id);

MPU_t* MPU_Cluster_get_node(const MPU_Cluster_t* cluster, const int node_id);
esp_err_t MPU_Cluster_read_node(const MPU_Cluster_t* cluster, const int node_id, mpu_data_t* data);

esp_err_t MPU_Cluster_read(const MPU_Cluster_t* cluster, mpu_data_t* data);
esp_err_t print_Cluster_data(const mpu_data_t* data, const int size);

#endif