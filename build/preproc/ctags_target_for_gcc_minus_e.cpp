# 1 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino"
# 1 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino"
# 2 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 3 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 4 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 5 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 6 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 7 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 8 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2

# 10 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2

# 12 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
//#include <base64.hpp>
# 14 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 15 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 16 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 17 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 18 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 19 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 20 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 21 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 22 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 23 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 24 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 25 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 26 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2

#define DEBUG 



#define TRACE(s) Serial.print(s)
#define TRACE_HEX(s) Serial.print(s, HEX)
#define TRACELN(s) Serial.println(s)
#define TRACE_HEXLN(s) Serial.println(s, HEX)
# 45 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino"
# 46 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 47 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 48 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 49 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 50 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 51 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 52 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2
# 53 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2

#define NO_OF_GPIO_PINS 8

TaskHandle_t Task1;
TaskHandle_t Task2;

struct CommandDescription commandDescriptionListSetup[] =
    {
        "device",
        "device settings",
        deviceOptionDecodeItems,
        sizeof(deviceOptionDecodeItems) / sizeof(OptionDecodeItems),
        (unsigned char *)&deviceSettings,
        sizeof(struct DeviceSettings),
        reset_device,
        setup_settings,
        loop_settings,
        loop_settings_proc1,

        "commands",
        "device command control",
        commandOptionDecodeItems,
        sizeof(commandOptionDecodeItems) / sizeof(OptionDecodeItems),
        (unsigned char *)&commandSettings,
        sizeof(struct CommandSettings),
        reset_commands,
        setup_commands,
        loop_commands,
        loop_commands_proc1,

        "mqtt",
        "Mqtt settings",
        mqttOptionDecodeItems,
        sizeof(mqttOptionDecodeItems) / sizeof(OptionDecodeItems),
        (unsigned char *)&mqttSettings,
        sizeof(struct MqttSettings),
        reset_MQTT_settings,
        setup_mqtt,
        loop_mqtt,
        loop_mqtt_proc1,

        "wifi",
        "Wifi configuration",
        WiFiOptionDecodeItems,
        sizeof(WiFiOptionDecodeItems) / sizeof(OptionDecodeItems),
        (unsigned char *)&wiFiSettings,
        sizeof(struct WiFiSettings),
        reset_wifi_settings,
        setup_wifi,
        loop_wifi,
        loop_wifi_proc1,

        "M5Stack",
        "M5STack display control",
        m5stackdisplayOptionDecodeItems,
        sizeof(m5stackdisplayOptionDecodeItems) / sizeof(OptionDecodeItems),
        (unsigned char *)&m5stackdisplaySettings,
        sizeof(struct M5stackdisplaySettings),
        reset_m5stackdisplay,
        setup_m5stackdisplay,
        loop_m5stackdisplay,
        loop_m5stackdisplay_proc1};

struct CommandDescription *commandDescriptionList = commandDescriptionListSetup;

int noOfCommands = sizeof(commandDescriptionListSetup) / sizeof(struct CommandDescription);

void setup_services()
{
    for (int i = 0; i < noOfCommands; i++)
    {
        Serial.printf("Setting up %s\n", commandDescriptionList[i].commandName);
        commandDescriptionList[i].setup();
    }
}

void loop_services(void *pvParameters)
{
    while (true)
    {
        for (int i = 0; i < noOfCommands; i++)
        {
            commandDescriptionList[i].loop();
        }
        delay(1);
        vTaskDelay(1);
    }
}

void loop_services_proc1(void *pvParameters)
{
    while (true)
    {
        for (int i = 0; i < noOfCommands; i++)
        {
            commandDescriptionList[i].proc1loop();
        }
        delay(1);
        vTaskDelay(1);
    }
}

// New background colour
#define TFT_BROWN 0x38E0

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 1000

# 162 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 2

unsigned long targetTime = 0; // Used for testing draw times

void setup()
{
    Serial.begin(115200);
    Serial.println("M5Stack display starting");

    setup_services();

    list_command_help();

    xTaskCreatePinnedToCore(
        loop_services, /* Task function. */
        "Task1", /* name of task. */
        10000, /* Stack size of task */
        
# 178 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 3 4
       __null
# 178 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino"
           , /* parameter of the task */
        1, /* priority of the task */
        &Task1, /* Task handle to keep track of created task */
        0); /* pin task to core 0 */

    xTaskCreatePinnedToCore(
        loop_services_proc1, /* Task function. */
        "Task2", /* name of task. */
        10000, /* Stack size of task */
        
# 187 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino" 3 4
       __null
# 187 "c:\\Users\\Rob\\Documents\\GitHub\\ESP32SensorPlatform\\M5Stack\\M5StackController.ino"
           , /* parameter of the task */
        1, /* priority of the task */
        &Task2, /* Task handle to keep track of created task */
        1); /* pin task to core 1 */
}

void loop()
{
    delay(1);
}
