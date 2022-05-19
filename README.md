# ESP32 robot car controller
ESP32 robot car controller is a small web-server + web page project for ESP32. It is designed to control a small differential drive mobile robot prototype. This is how web-page looks:

<p align="center">
<img src="https://github.com/701lab/ESP32-robot-car-controller/blob/main/images/joystick_controller_example.jpg" alt="borderChangeExample" width="100%">
</p>

You can watch a project demo [here](https://photos.app.goo.gl/gQUQkC9ZNbU3YpYm9).

**Project is designed using:**
- platformIO;
- arduino framework for ESP32;
- mostly C code wrapped around default C++ stuff for arduino.
- plain HTML/CSS/JS;
- [FlexJoystick](https://github.com/Trimple/FlexJoystick) library for interactable joysticks.

## Main features
* ESP32 is configured as a wi-fi access point;
* name and password of access point are generated automatically based on how many other similar robots (wi-fi access points) are around;
* only a single device can be connected to an access point at a time;
* ESP32 can serve a single web-page, styles file and js file to a client;
* client web-page has 2 joysticks to control differential drive robot;
* client and server communicate using WebSockets;
* WebSockets are implemented using **ESPAsyncWebServer.h** library;
* client constantly tries to reconnect to the server if connection is lost;
* web-page displays the name of the robot it is connected to and connection status;
* motors on the robot are controlled by STM32 that is not a part of this project;
* ESP32 communicates with STM32 using simple UART-based protocol.

## How to test the web-page
1. upload the firmware to any ESP32 controller using PlatformIO;
2. upload the filesystem image to your ESP32. If you don't know how to do it [here is a guide](https://randomnerdtutorials.com/esp32-vs-code-platformio-spiffs/);
3. using any device connect to a new wi-fi access point. It will be called **esp_robot_1** and will have password **password_1**;
5. open any browser and go to 192.168.4.1;
6. plary around!

## Contributions

This project is free to use by anyone brave enough to figure it out.

This project is mostly a prototype for our team. We will be grateful for suggestions and ideas, but help is probably not needed.
