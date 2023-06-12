# FYP Control remote devices with air gestures

In this project, we develop a smart glove that can control electronic devices wirelessly. We have designed a physical smart glove embedded with sensors and wireless communication technologies. The data collected from the smart glove will be transmitted to a server that is capable of processing the sensor data and classifying these time-series data into pre-defined gestures with machine learning. These gestures can then be sent to applications and perform the task we intended with the gesture in an intuitive way. The project includes several applications to demonstrate the capabilities of the system, including control of PowerPoint and LED control using gestures received.
在這個專案中，我們開發了一款能夠無線控制電子設備的智能手套。我們設計了一款嵌入有感測器和無線通信技術的智能手套。收集到的數據將被上傳到一個能夠處理感測器數據的伺服器上，並且在伺服器上使用深度學習將這些時間序列數據分類為八種預先定義好的手勢。接著這些被辨識的手勢將被發送到不同的後端應用中，這包括使用接收到的手勢來控制 PowerPoint和控制 LED。

![Glove](res/glove.jpg)

## System Architecture
![System Architecture](res/system_arch.jpg)

The smart glove collects motion data and orientation information from the user's hand. This data is then transmitted to a ROS (Robot Operating System) server via a Wi-Fi connection. 
智能手套從使用者手部收集運動數據和方向數據，通過 Wi-Fi 連接將這些數據傳輸到 ROS（機器人作業系統）伺服器。

The ROS server processes the received data by applying filtering and transformation techniques to obtain preprocessed motion data. The preprocessed data is then fed into a trained gesture recognition model for real-time inference. At the end of the server pipeline, the decision module analyzes the detected gesture and combines it with the orientation information from the glove to determine the appropriate command to be sent to the target devices. 
ROS 伺服器通過應用濾波和轉換技術處理接收到的原始數據，獲得預處理的運動數據。接著，預處理的數據將會被傳送到訓練好的手勢識別模型進行實時判斷。在伺服器的最後端，系統的決策模組會分析檢測到的手勢，並結合手套的方向訊息來確定應發送到目標設備的適當指令。

In the end, the application devices receive the command from the ROS server and respond accordingly. These application devices can include PowerPoint software, LEDs, or any other compatible devices. 
最終應用設備從 ROS 伺服器接收指令並作出相應的響應。這些應用設備可以包括 PowerPoint 軟件、LED或其他兼容設備。

## The Hardware Design of the Smart Glove
Our implementation features a normal glove equipped with three IMUs located on the index finger, middle finger, and back of the palm. A microcontroller unit (MCU), an ESP32-S3, is positioned on the back of the hand. With the built-in WI-FI and Bluetooth connectivity of ESP32-S3, it exchanges data using the wireless connections with the server. 
我們的智慧手套配備有三個 IMU（慣性測量單元），分別位於食指、中指和手掌背部。並配有一個位於手背上的ESP32-S3微控制器單元（MCU），此MCU內建了 WI-FI 和藍牙連接功能，可以與伺服器進行通訊。

The three IMUs (MPU9250) are connected to the ESP32-S3 through four data lines and two power lines, following the Serial Peripheral Interface (SPI) protocol which enables access to multiple slave devices of the same type on the data bus. Moreover, a compass (GY26) is connected to ESP32-S3 through two data lines and two power lines, following the Universal Asynchronous Receiver/Transmitter (UART) protocol. 
三個 IMU（MPU9250）遵循串行外設介面（SPI）協議，通過四條數據線和兩條電源線連接到 ESP32-S3。此通訊協議可以在數據總線上訪問多個相同類型的從設備。此外，一個指南針（GY26）遵循通用異步接收器/發射器（UART）協議，通過兩條數據線和兩條電源線連接到 ESP32-S3。

Finally, we use a power bank to power the ESP32-S3.

除此之外，我們使用了一個行動電源作為 ESP32-S3 的電源供應。![Hardware Design](res/hardware_structure.jpg)

## The Server Design
First, we set up a Flask server to receive raw data represented as a JSON string. The server then performs data filtering and transformation calculations (from world coordinates to palm coordinates) using several backend nodes. The processed data is then fed into the model to infer the gesture class based on a pre-trained gesture model. Finally, the decision module retrieves the output from the gesture recognition model and combines it with the orientation information of the gesture to send commands to the target devices.
首先，我們建立一個 Flask 伺服器來接收以 JSON 字符串表示的原始數據。接著伺服器藉由後端節點來進行數據過濾和轉換計算（從對地座標轉為對手掌坐標）等任務。接著被處理好的數據將會被輸入至模型，並且根據預訓練的手勢模型來推斷手勢類別。最後，決策模塊將從手勢識別模型獲取輸出，並結合手勢的方向訊息發送命令給目標設備。

![Server Design](res/server_arch.jpg)

## Submodules
This project consists of two main submodules:

1. FYP-Glove: This submodule handles the communication between the glove and the server. It includes the IMU driver and compass functionalities, ensuring the accurate collection and transmission of motion data.

2. FYP-Server: This submodule contains the server implementation for the FYP pipeline. It includes the receiver, backend processing, gesture recognition, and command generation modules.

## Demo
A demo video showcasing the functionality of the project can be found here: [Demo Video](https://cse.hkust.edu.hk/ug/fyp/posters/gallery/2022-2023/96_CSB1_Media.mp4)

For more implementation detail, please find them here: [Smart Glove](res/fyp_final_report.pdf)