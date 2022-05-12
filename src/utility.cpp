#include "utility.h"

const char* default_ap_ssid = "esp32_robot_";

const char* default_ap_password = "password_";

#define MAX_ROBOT_NUMBER        (255U)

char robot_ssid[32] = "";
char robot_password[32] = "";

const char* ap_password = "password";

static uint32_t evaluate_robot_number(void);
static void generate_ap_name(uint32_t robot_number, char *target_array);

static void generate_ap_ssid(uint32_t number, const char *default_ssid, char *destination);

static int32_t add_number_to_string(uint32_t number, const char *input_string, char *target_string, uint32_t target_string_len);

static uint32_t find_smallest_empty_index(uint8_t *input_array, uint32_t array_len);
static uint32_t count_robots_ssids(uint8_t *target_array, uint32_t target_array_len, uint32_t number_of_ssids);


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
        Serial.println("Wrong file name!");
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
    WiFi.mode(WIFI_AP);

    uint32_t robot_number = evaluate_robot_number();
    add_number_to_string(robot_number, default_ap_ssid, robot_ssid, 32);
    add_number_to_string(robot_number, default_ap_password, robot_password, 32);

    Serial.println("Setting Access Point \"" + String(robot_ssid) + "\" with password: " + robot_password);

    WiFi.softAP(robot_ssid, robot_password, 1, 0, 1);
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
    return robot_ssid;
}

/*!
 * @brief Starts access point with unique ssid and password.
 * 
 * @return uint32_t 
 */
static uint32_t evaluate_robot_number(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    Serial.println("Scan started.");
    int n = WiFi.scanNetworks();
    Serial.println("Scan finished.");
    if (n == 0)
    {
        Serial.println("no networks found");
        return 1;
    }
    Serial.println(String(n) + " networks found");

    uint8_t robot_indexes[MAX_ROBOT_NUMBER + 1] = {0};
    int32_t number_of_robots = count_robots_ssids(robot_indexes, MAX_ROBOT_NUMBER + 1, n);

    uint32_t default_ssid_len = strlen(default_ap_ssid);

    Serial.println("number of esp robots: " + String(number_of_robots));

    uint32_t value = find_smallest_empty_index(robot_indexes, MAX_ROBOT_NUMBER + 1);
    Serial.println("minimum availiable number is " + String(value));
    return value;
}

static int32_t add_number_to_string(uint32_t number, const char *input_string, char *target_string, uint32_t target_string_len)
{
    uint32_t input_string_len = strlen(input_string);

    if(input_string_len > target_string_len || input_string_len == 0)
    {
        target_string[0] = '\0';
        return -1;
    }

    for(uint32_t i = 0; i < input_string_len; i += 1)
    {
        target_string[i] = input_string[i];
    }

    char number_as_string[12] = "";
    itoa(number, number_as_string, 10); 

    if(strlen(number_as_string) + input_string_len > target_string_len)
    {
        target_string[0] = '\0';
        return -1;
    }

    uint32_t index = 0;
    while(number_as_string[index])
    {
        target_string[index + input_string_len] = number_as_string[index];
        index += 1;
    }
    target_string[index + input_string_len] = '\0';

    return 0;
}

static uint32_t find_smallest_empty_index(uint8_t *input_array, uint32_t array_len)
{
    for(uint32_t i = 1; i < array_len; i += 1)
    {
        if(input_array[i] == 0)
        {
            return i;
        }
    }

    return 0;
}

static uint32_t count_robots_ssids(uint8_t *target_array, uint32_t target_array_len, uint32_t number_of_ssids)
{
    uint32_t default_ssid_len = strlen(default_ap_ssid);
    uint32_t number_of_robots_ssids = 0;

    for (uint32_t i = 0; i < number_of_ssids; i += 1)
    {
        const char* ssid = WiFi.SSID(i).c_str();

        uint32_t index = 0;
        while (default_ap_ssid[index])
        {
            if (default_ap_ssid[index] != ssid[index])
            {
                break;
            }
            index += 1;
        }
        if (index != default_ssid_len)
        {
            continue;
        }
        if (default_ap_ssid[index - 1] != ssid[index - 1]) // check if the last char is right
        {
            continue;
        }

        const char *string_ending = &ssid[index];
        uint32_t number = atoi(string_ending);
        if (number > target_array_len || number == 0) // 0 means that input was either 0 or not a number. We don't need both.
        {
            continue;
        }
        number_of_robots_ssids += 1;
        target_array[number] = 1;

        delay(10);
    }

    return number_of_robots_ssids;
}