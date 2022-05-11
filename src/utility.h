#pragma once

#include <WiFi.h>
#include "SPIFFS.h"

// Reads content of the given file into the given array. Returns length of the file.
uint16_t read_file_from_spiffs(char *containing_array, uint32_t array_length, char *file_name);

// Starts ESP32 access point with predefined ssid and password.
void start_access_point();

// Conects to existing wifi network if possible.
void connect_to_existing_wifi(const char* ssid, const char* password);