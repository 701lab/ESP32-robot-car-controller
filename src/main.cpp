#include <Arduino.h>
#include "SPIFFS.h"
#include <WiFiClientSecure.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "utility.h"

#include "device/device.h"
#include "device/device_web_server.h"
#include "device/device_global_variables.h"

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

AsyncWebServer ws_server(80);
AsyncWebSocket ws("/ws");

uint8_t uart_message[3] = {97, 0, 0};

void setup()
{
  esp32_setup_peripherals();

  start_access_point();

  ws.onEvent(onWsEvent);
  ws_server.addHandler(&ws);

  esp32_setup_web_server(&ws_server);
}

void loop()
{
  delay(100);
  Serial2.write(uart_message, 3);

  // if(Serial2.available())
  // {
  //   uint8_t data = Serial2.read();
  //   Serial.println("New value: " + String(data));
  //   delay(500);
  //   Serial2.write(data);
  // }
  // digitalWrite(16, !digitalRead(16));
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  String payload_string = (const char *)data;

  if (type == WS_EVT_CONNECT)
  {
    Serial.println("Websocket client connection received");
    char json_message[50] = "{\"hostName\":\"";
    int32_t index = 0;
    const char* ssid = get_ssid();
    while (ssid[index])
    {
      json_message[index + 13] = ssid[index];
      index += 1;
    }
    json_message[index + 13] = '\"';
    json_message[index + 14] = '}';
    Serial.println(json_message);
    ws.textAll(json_message);
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.println("Client disconnected");

    uart_message[1] = 0 + 97;
    uart_message[2] = 0 + 97;
  }
  else if (type == WS_EVT_DATA) // receive text from client
  {

    char direction_1 = payload_string[2];
    char direction_2 = payload_string[0];

    if (direction_1 == 'U')
    {
      uart_message[1] = 1 + 97;
    }
    else if (direction_1 == 'D')
    {
      uart_message[1] = 2 + 97;
    }
    else
    {
      uart_message[1] = 0 + 97;
    }

    if (direction_2 == 'L')
    {
      uart_message[2] = 1 + 97;
    }
    else if (direction_2 == 'R')
    {
      uart_message[2] = 2 + 97;
    }
    else
    {
      uart_message[2] = 0 + 97;
    }

    Serial.println("direction 1: " + String(direction_1) + " direction 2: " + String(direction_2));
  }

}