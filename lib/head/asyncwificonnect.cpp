#include <head.h>

Ticker ticker;
AsyncDNSServer dns;
AsyncWebServer httpServer(8266);
AsyncWebServer managerServer(80);

String ipStr;

void tick()
{
  //toggle state
  int state = digitalRead(wifi_search);  // get the current state of GPIO1 pin
  digitalWrite(wifi_search, !state);     // set pin to the opposite state
}

void configModeCallback (AsyncWiFiManager *myWiFiManager) {
    ticker.attach(0.2, tick);
}

bool shouldSaveConfig = false;

void saveConfigCallback () {
  shouldSaveConfig = true;
}

void wifiConnection(){

	AsyncWiFiManagerParameter text_user("<br><b>Username</b>");
	AsyncWiFiManagerParameter console_user("user", "Username", "", 30);
	AsyncWiFiManagerParameter text_pass("<br><b>Password</b>");
	AsyncWiFiManagerParameter console_pass("pass", "Password","", 30);
	AsyncWiFiManagerParameter pass_onload("<script>document.getElementById(\"pass\").type = \"password\";</script>");
	AsyncWiFiManagerParameter text_server("<br><b>Server</b>");
	AsyncWiFiManagerParameter console_server("server", "Server", server.c_str(), 30);

    AsyncWiFiManager wifiManager(&managerServer, &dns);
    wifiManager.setDebugOutput(false);
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.setAPCallback(configModeCallback);

    wifiManager.addParameter(&text_user);
    wifiManager.addParameter(&console_user);
    wifiManager.addParameter(&text_pass);
    wifiManager.addParameter(&console_pass);
    wifiManager.addParameter(&pass_onload);
    wifiManager.addParameter(&text_server);
    wifiManager.addParameter(&console_server);

	if (resetFlag || WiFi.SSID().equals("")){
        wifiManager.setConfigPortalTimeout(120);
		if (!wifiManager.startConfigPortal("Genie", "pass1234")) {
  			delay(3000);
  			ESP.restart();
  			delay(5000);
	}
	}
	else if (!resetFlag){
        //wifiManager.setConfigPortalTimeout(1);
        ticker.attach(0.05, tick);
		if (!wifiManager.autoConnect("Genie", "pass1234")) {
			delay(3000);
			ESP.restart();
			delay(5000);
		}
	}
    ticker.detach();
    digitalWrite(wifi_search, LOW);
    IPAddress ip = WiFi.localIP();
    ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    //Serial.print("local ip: ");
	//Serial.println(WiFi.localIP());

	if (shouldSaveConfig) {

		server = console_server.getValue();
		user = console_user.getValue();
		pass = console_pass.getValue();

		//Serial.println("saving config");
		DynamicJsonBuffer jsonBuffer;
		JsonObject& json = jsonBuffer.createObject();
		json["user"] = user;
		json["pass"] = pass;
		json["server"] = server;
		json["URL"] = url;
		json["wsport"] = wsport;
        if(SPIFFS.begin()){
            SPIFFS.remove("/config.json");
    		File configFile = SPIFFS.open("/config.json", "w");
    		if (!configFile) {
    			Serial.println("failed to open config file for writing");
    		}
            else{
                json.printTo(Serial);
                Serial.println("");
        		json.printTo(configFile);
        		configFile.close();
            }
        }


	}
}

void mdnsStarter(){
	char hostString[16] = {0};
	sprintf(hostString, "GenieOne%06X", ESP.getChipId());
	if (!MDNS.begin(hostString)) {
		//Serial.println("Error setting up MDNS responder!");
	  	}
	else {
		//Serial.println("mDNS responder started");
		MDNS.addService("genie", "tcp", 8267);	  // Add service to MDNS-SD
	}
}
