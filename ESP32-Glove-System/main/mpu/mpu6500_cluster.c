#include "mpu6500_cluster.h"

esp_err_t MPU_create_cluster(MPU_Cluster_t* cluster, MPU_t* mpu, const int cluster_size) {
    cluster->size = cluster_size;
    cluster->mpu = malloc(sizeof(MPU_t) * cluster_size);
    for (int i = 0; i < cluster_size; i++) {
        cluster->mpu[i] = mpu[i];
    }
    return ESP_OK;
}

esp_err_t MPU_remove_cluster(MPU_Cluster_t* cluster) {
    free(cluster->mpu);
    return ESP_OK;
}

esp_err_t MPU_Cluster_add_node(MPU_Cluster_t* cluster, const MPU_t* mpu, const int node_id) {
    if (node_id >= cluster->size) {
        MPU_Cluster_t new_cluster;
        MPU_t* new_mpu_list = malloc(sizeof(MPU_t) * (node_id + 1));
        for (int i = 0; i < cluster->size; i++) {
            new_mpu_list[i] = cluster->mpu[i];
        }
        new_mpu_list[node_id] = *mpu;
        MPU_create_cluster(&new_cluster, new_mpu_list, node_id + 1);

        cluster->size = node_id + 1;
        MPU_remove_cluster(cluster);
        *cluster = new_cluster;
    } else
        cluster->mpu[node_id] = *mpu;
    return ESP_OK;
}

esp_err_t MPU_Cluster_remove_node(MPU_Cluster_t* cluster, const MPU_t* mpu, const int node_id) {
    // if (node_id >= cluster->size) {
    //     return ESP_ERR_INVALID_ARG;
    // } else {
    //     cluster->mpu[node_id] = NULL;
    //     return ESP_OK;
    // }
    return ESP_OK;
}

MPU_t* MPU_Cluster_get_node(const MPU_Cluster_t* cluster, const int node_id) {
    if (node_id >= cluster->size) {
        return NULL;
    } else {
        return &(cluster->mpu[node_id]);
    }
}

esp_err_t MPU_Cluster_read_node(const MPU_Cluster_t* cluster, const int node_id, mpu_data_t* data) {
    if (node_id >= cluster->size) {
        return ESP_ERR_INVALID_ARG;
    } else {
        return MPU_read(&(cluster->mpu[node_id]), data);
    }
}

esp_err_t MPU_Cluster_read(const MPU_Cluster_t* cluster, mpu_data_t* data) {
    for (int i = 0; i < cluster->size; i++) {
        MPU_read(&(cluster->mpu[i]), &data[i]);
    }
    return ESP_OK;
}

esp_err_t print_Cluster_data(const mpu_data_t* data, const int size){
    for (int i = 0; i < size; i++) {
        printf("MPU[%d]: [%2.2f, %2.2f, %2.2f]\t", i, data[i].rpy.roll, data[i].rpy.pitch, data[i].rpy.yaw);
    }
    printf("\n");
    return ESP_OK;
}