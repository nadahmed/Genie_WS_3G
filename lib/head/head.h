#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "ESPAsyncDNSServer.h"
#include <ESP8266mDNS.h>
#include <ESPAsyncWifiManager.h>
#include <WebSocketsClient.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <Ticker.h>

extern String wsport;
extern String server;
extern String url;
extern String user;
extern String pass;
extern bool resetFlag;
extern AsyncWebServer httpServer;
extern WebSocketsClient webSocket;
extern AsyncWebSocket ws;
extern bool useLocally;
extern Ticker ticker;
extern String ipStr;
extern bool shouldReboot;

extern const uint8_t relay[];
extern const uint8_t sizeOfRtss;
extern const uint8_t wifi_search;//D0;

extern uint8_t id;
extern uint8_t power[3];
extern bool enabled[3];
extern bool isOn[3];
extern bool isActivated[3];
extern bool searchLight;
extern bool keepLEDon;
extern bool reset;
extern bool swState[3];
extern uint32_t blinkTime;

void tick();
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
void websocketsStarter();
void wifiConnection();
void configModeCallback (AsyncWiFiManager *myWiFiManager);
void saveConfigCallback ();
void spiffs();
void mdnsStarter();
void handleSwitchOn(AsyncWebServerRequest *request);
void handleSwitchOff(AsyncWebServerRequest *request);
void handleRoot(AsyncWebServerRequest *request);
void handleNotFound(AsyncWebServerRequest *request);
void httpServiceStarter();
void accessPoint(AsyncWiFiManager wifiManager);
void websocketServerStarter();
void operate();
void pinInitialization();
void printValues();
