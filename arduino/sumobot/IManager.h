
#ifndef IManager_h

#define IManager_h

#include <EEPROM.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>



#include "IConstants.h"

#define CALLBACK_WEBSERVICE String (*callback)(String)
class IManager
{
  
	public: IManager();
	  void setup();
	  bool isConfigurable(void);
	  void loop();
	  void setCallback(CALLBACK_WEBSERVICE, char* path);
	  void cleanDataWifi();

  private: 
    CALLBACK_WEBSERVICE;
	  void settingWifi();
    int isValidWifi();
    void findSsidAndPassword();
	  void settingAccessPoint();
    void settingWebService(char* path);
	  void saveWifi(String ssid,  String password);
	  void handleRegisterAuthWiFi();
    void handleNotFound();
	  String _essid; 
    String _epass;
	  bool _interrupt;
	
};

#endif

