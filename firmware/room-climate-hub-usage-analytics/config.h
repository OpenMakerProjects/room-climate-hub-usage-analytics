#pragma once
#if __has_include("config.private.h")
#include "config.private.h"
#else
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define MQTT_HOST ""
#endif
#define MQTT_PORT 1883
#define SOUND_THRESHOLD 150
