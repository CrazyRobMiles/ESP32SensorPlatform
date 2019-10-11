#pragma once

#define M5STACK_DEVICE_ID_LENGTH 120
#define M5STACK_DISPLAY_MESSAGE_LENGTH 20

struct M5stackdisplaySettings
{
	bool settings_display_enabled;
	char message[M5STACK_DISPLAY_MESSAGE_LENGTH + 1];
	char deviceID[M5STACK_DEVICE_ID_LENGTH + 1];
} m5stackdisplaySettings;

// {"v":1, "t" : "Sensor01", "c" : "M5Stack", "o" : "state", "val" : "on"}
// {"v":1, "t" : "Sensor01", "c" : "M5Stack", "o" : "state", "val" : "off"}
void do_m5stackdisplay_state(JsonObject &root, char *resultBuffer)
{
	int reply = checkTargetDeviceName(root);

	if (reply == WORKED_OK)
	{
		const char *option = root["val"];
		if (!option)
		{
			// no option - just a status request
			if (m5stackdisplaySettings.settings_display_enabled)
			{
				build_text_value_command_reply(WORKED_OK, "on", root, resultBuffer);
			}
			else
			{
				build_text_value_command_reply(WORKED_OK, "off", root, resultBuffer);
			}
			return;
		}

		if (!root["val"].is<char *>())
		{
			TRACELN("Value is missing or not a string");
			reply = INVALID_M5STACK_DISPLAY_ON_OFF_SETTING;
		}
		else
		{
			const char *option = root["val"];
			if (strcasecmp(option, "on") == 0)
			{
				m5stackdisplaySettings.settings_display_enabled = true;
			}
			else
			{
				if (strcasecmp(option, "off") == 0)
				{
					m5stackdisplaySettings.settings_display_enabled = false;
				}
				else
				{
					reply = INVALID_STATUS_LED_SETTING;
				}
			}
		}
	}

	if (reply == WORKED_OK)
		save_settings();

	build_command_reply(reply, root, resultBuffer);
}

// {"v":1, "t" : "Sensor01", "c" : "M5Stack", "o" : "devid", "val" : "rob-sensor"}
void do_m5stackdisplay_device_id(JsonObject &root, char *resultBuffer)
{
	int reply = checkTargetDeviceName(root);

	if (reply == WORKED_OK)
	{
		const char *option = root["val"];
		if (!option)
		{
			// no option - just a status request
			build_text_value_command_reply(WORKED_OK, m5stackdisplaySettings.deviceID, root, resultBuffer);
			return;
		}

		if (!root["val"].is<char *>())
		{
			TRACELN("Value is missing or not a string");
			reply = MISSING_M5STACK_DISPALY_DEV_ID;
		}
		else
		{
			const char *option = root["val"];
			if (strlen(option) > M5STACK_DEVICE_ID_LENGTH)
			{
				reply = INVALID_M5STACK_DISPLAY_DEVICE_ID_LENGTH;
			}
			else
			{
				strcpy(m5stackdisplaySettings.deviceID, option);
			}
		}
	}

	if (reply == WORKED_OK)
		save_settings();

	build_command_reply(reply, root, resultBuffer);
}

OptionDecodeItems m5stackdisplayOptionDecodeItems[] = {
	{"status",
	 "Controls the display of the status led",
	 "{\"v\":1, \"t\" : \"sensor01\", \"c\" : \"M5Stack\",  \"o\" : \"state\", \"val\":\"on\"}",
	 do_m5stackdisplay_state},
	{"devid",
	 "Sets the name of the M5Stack device to display",
	 "{\"v\":1, \"t\" : \"sensor01\", \"c\" : \"M5Stack\",  \"o\" : \"devid\", \"val\":\"rob-sensor\"}",
	 do_m5stackdisplay_device_id}};

int paxCountValue;

void act_TTN_Message(char *json, void (*deliverResult)(char *resultText))
{
	//TRACE("Received TTN message:");

	//TRACELN(json);

	// Clear any previous elements from the buffer

	jsonBuffer.clear();

	JsonObject &root = jsonBuffer.parseObject(json);

	if (!root.success())
	{
		TRACELN("JSON could not be parsed");
		abort_json_command(JSON_COMMAND_COULD_NOT_BE_PARSED, root, deliverResult);
		return;
	}

	const char *appID = root["app_id"];

	if (!appID)
	{
		TRACELN("Missing app_id");
		return;
	}

	if (strcasecmp(appID, "hull-pax-counter") != 0)
	{
		TRACELN("Wrong app");
		return;
	}

	const char *deviceID = root["dev_id"];

	if (!deviceID)
	{
		TRACELN("Missing device ID");
		return;
	}

	if (strcasecmp(deviceID, m5stackdisplaySettings.deviceID) != 0)
	{
		TRACELN("Wrong device");
		return;
	}

	int ble = root["payload_fields"]["ble"];

	int wifi = root["payload_fields"]["wifi"];

	TRACE("BLE:");
	TRACELN(ble);
	TRACE("WiFi:");
	TRACELN(wifi);

	paxCountValue = ble + wifi;
}

#define DISPLAY_UPDATE_MILLIS 20
#define DISPLAY_MESSAGE_LENGTH 18

unsigned long milliseconds_at_last_display_update;

char statusMessage[DISPLAY_MESSAGE_LENGTH];
char activeStatusMessage[DISPLAY_MESSAGE_LENGTH];

void setStatusMessage(char *message)
{
	Serial.printf("status: %s\n", message);
	strcpy(statusMessage, message);
}

int count;

void display_starting_wifi_scan()
{
  TRACE("==> Starting WiFi scan");
  setStatusMessage("Starting WiFi scan");
}

void display_connecting_to_wifi()
{
  TRACE("==> Connecting to WiFi");
  setStatusMessage("Connecting to WiFi");
}

void display_no_wifi_networks_found()
{
  TRACE("==> No WiFi networks found");
  setStatusMessage("No WiFi networks found");
}

void display_wifi_connected_ok()
{
  TRACE("==> WiFi connected OK");
  setStatusMessage("WiFi connected OK");
}

void display_wifi_connect_failed()
{
  TRACE("==> WiFi connect failed");
  setStatusMessage("WiFi connect failed");
}

void display_wifi_failed_in_use()
{
  TRACE("==> WiFi network failed in use");
  setStatusMessage("WiFi network failed in use");
}

void display_mqtt_failed_in_use()
{
  TRACE("==> MQTT failed in use");
  setStatusMessage("MQTT failed in use");
}

void display_mqtt_connecting ()
{
  TRACE("==> MQTT connecting");
  setStatusMessage("MQTT connecting");
}

void display_mqtt_connected_ok()
{
  TRACE("==> MQTT connected OK");
  setStatusMessage("MQTT connected OK");
}

void display_mqtt_connect_failed()
{
  TRACE("==> MQTT connect failed");
  setStatusMessage("MQTT connect failed");
}

void display_mqtt_message_received()
{
  TRACE("==> MQTT connect failed");
  setStatusMessage("MQTT message received");
}

void bind_messages()
{
	wifiStatusMessages.display_starting_wifi_scan = display_starting_wifi_scan;
	wifiStatusMessages.display_connecting_to_wifi = display_connecting_to_wifi;
	wifiStatusMessages.display_no_wifi_networks_found = display_no_wifi_networks_found;
	wifiStatusMessages.display_wifi_connected_ok = display_wifi_connected_ok;
	wifiStatusMessages.display_wifi_connect_failed = display_wifi_connect_failed;
	wifiStatusMessages.display_wifi_failed_in_use = display_wifi_failed_in_use;

	mQTTStatusMessages.display_mqtt_connecting = display_mqtt_connecting;
	mQTTStatusMessages.display_mqtt_connected_ok = display_mqtt_connected_ok;
	mQTTStatusMessages.display_mqtt_connect_failed = display_mqtt_connect_failed;
	mQTTStatusMessages.display_mqtt_failed_in_use = display_mqtt_failed_in_use;
	mQTTStatusMessages.display_mqtt_message_received = display_mqtt_message_received;
}

void redrawDisplay()
{
	if (strcasecmp(statusMessage, activeStatusMessage) != 0)
	{
		// got a new status message - need to redraw it
		M5.Lcd.setTextSize(2);
		M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);

		char buffer[DISPLAY_MESSAGE_LENGTH];

		snprintf(buffer, DISPLAY_MESSAGE_LENGTH-1, "%s                           ", statusMessage);
		M5.Lcd.drawString(buffer, 0, 0, 2);
		strcpy(activeStatusMessage,statusMessage);
		Serial.println(activeStatusMessage);
	}
}


void updateDisplay()
{
	unsigned long time_in_millis = millis();

	unsigned long time_since_update = time_in_millis - milliseconds_at_last_display_update;

	if (time_since_update < DISPLAY_UPDATE_MILLIS)
		return;

	redrawDisplay();
}

void reset_m5stackdisplay()
{
}

void setup_m5stackdisplay()
{
	bind_messages();

	M5.begin();
	M5.Lcd.setTextSize(2);
	M5.Lcd.fillScreen(TFT_BLACK);
	M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
}

void loop_m5stackdisplay()
{
	updateDisplay();	
}

void loop_m5stackdisplay_proc1()
{
}
