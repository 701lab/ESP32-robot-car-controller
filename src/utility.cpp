#include "utility.h"

const char* ap_ssid = "Ne robot";
const char* ap_password = "robotpassword";

// Reads content of the given file into the given array. Returns length of the file.
uint16_t read_file_from_spiffs(char *containing_array, uint32_t array_length, char *file_name)
{
    if (array_length < 10)
    {
        return 0;
    }

    File file = SPIFFS.open(file_name);
    if (!file)
    {
        Serial.println("Ti loh!");
        file.close();
        return 0;
    }

    uint32_t index = 0;
    while (file.available())
    {
        containing_array[index] = file.read();
        index += 1;
        if (index > array_length)
        {
            file.close();
            return 0;
        }
    }
    containing_array[index] = 0;

    file.close();
    return index;
}

// Starts ESP32 access point with predefined ssid and password.
void start_access_point()
{
    Serial.println("Setting Access Point \"" + String(ap_ssid) + "\"");
    WiFi.softAP(ap_ssid, ap_password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

// Conects to existing wifi network if possible.
void connect_to_existing_wifi(const char *ssid, const char *password)
{
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
}

const char* get_ssid(void)
{
    return ap_ssid;
}