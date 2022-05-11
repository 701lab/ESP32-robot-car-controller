#pragma once

#include "Arduino.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "utility.h"

void esp32_setup_web_server(AsyncWebServer *server);