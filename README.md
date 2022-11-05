# ESP32-FYP-Glove
## Submodules
1. ESP32-Glove-System: including communication between esp and pc, imu driver
2. FYP-ROS: including all the ros nodes
3. Web-Post-Server: the server for esp to post data to (to be converted into a submodule)

rest of the folder should be removed in the future

## Connect to EC2 
```bash
ssh -i FYP-Glove.pem ubuntu@ec2-16-163-138-9.ap-east-1.compute.amazonaws.com
```