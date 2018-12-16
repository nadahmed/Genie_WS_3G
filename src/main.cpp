#include <head.h>


bool resetFlag = false;
bool useLocally = true;

String wsport = "80";
String server = "192.168.2.191";
String url = "/ws/console/";
String user = "Username";
String pass = "Password";



String rawTxt="";


void setup() {
	Serial.begin(38400);
	Serial.setDebugOutput(false);
	Serial.println();
	Serial.println();
	Serial.println();


	for(uint8_t t = 4; t > 0; t--) {
		//Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
		Serial.flush();
		delay(500);
	}

	pinInitialization();

	

	spiffs();
	wifiConnection();
    websocketServerStarter();
	httpServiceStarter();
	mdnsStarter();
	websocketsStarter();
}

unsigned long pre2 = millis();
bool noWifiFlag =true;

void loop() {
	if (shouldReboot){		//If webupdate is Successful then reboot device
		delay(2000);
		ESP.restart();
		delay(5000);
	}
	if (WiFi.status() != WL_CONNECTED) {
		if(noWifiFlag){
			ticker.attach(0.05, tick);
			pre2 = millis();
		}

		if (millis()-pre2 > (120 * 1000)){
			ticker.detach();
			digitalWrite(wifi_search, HIGH);
		}
		noWifiFlag = false;
		WiFi.reconnect();
	} else {
		if(!noWifiFlag){
			ticker.detach();
		    digitalWrite(wifi_search, LOW);
		}
		noWifiFlag = true;
	}
	operate();
}
