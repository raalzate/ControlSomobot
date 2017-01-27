

#include "IManager.h"


ESP8266WebServer server(80);

IManager::IManager(){}

void IManager::setup()
{
  delay(1000);
  Serial.begin(9600);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  settingWifi();
}

bool IManager::isConfigurable(void)
{
 	return !_interrupt;
}

void IManager::loop()
{
	 server.handleClient();     
}

void IManager::setCallback(CALLBACK_WEBSERVICE, char* path)
{
   if(isConfigurable()){
      this->callback = callback;
      settingWebService(path);
   } else {
    settingAccessPoint(); 
   }
}

void IManager::settingWifi()
{
  Serial.println("\nStartup");  
  findSsidAndPassword();
  if (_essid.length() > 3 ) {
     // WiFi.config(ip, gateway, subnet);
      WiFi.begin(_essid.c_str(), _epass.c_str());
      if (isValidWifi() == 20 ) { 
         Serial.print("\nConnected WIFI: ");
         Serial.print(WiFi.localIP());
         Firebase.setString(SUMO_USERNAME,WiFi.localIP().toString());
         // handle error
         if (Firebase.failed()) {
             Serial.print("\nrepor ip failed:");
             Serial.println(Firebase.error());  
             return;
         }
      } else {
         Serial.println("\nNo Connected. {"+_essid+"}");
         _interrupt = 1;
      }
  }  else {
      _interrupt = 1;
      Serial.println("\nNo Connected.");
  }

}
  
int IManager::isValidWifi() 
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");  
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { 
      return(20); 
    }
    delay(500);
    Serial.print("."); 
    c++;
  }
  Serial.println("\nConnect timed out");
  return(10);
} 

void IManager::findSsidAndPassword()
{

  EEPROM.begin(512);
  for (int i = 0; i < 32; ++i){
    _essid+=char(EEPROM.read(i));    
  }
  for (int i = 32; i < 62; ++i){
    _epass+=char(EEPROM.read(i));
  }
  EEPROM.end();
}



void IManager::settingAccessPoint()
{

  WiFi.softAP(AP_SSID, AP_PASSWORD);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/settingWifi", std::bind(&IManager::handleRegisterAuthWiFi, this));
  server.begin();
  Serial.println("HTTP server started");
}

void IManager::settingWebService(char* path)
{
  server.on(path, [&](){
    String message;
    for (uint8_t i = 0; i < server.args(); i++) {
      message += server.argName(i) + ":" + server.arg(i) + ";";
    }
    server.send(200, "application/json", callback(message));
  }); 
 
  server.onNotFound(std::bind(&IManager::handleNotFound, this));
  server.begin();
  Serial.print("\nWebService online");
}


void IManager::cleanDataWifi()
{
   EEPROM.begin(512);
   Serial.println("\n*** Clearing eeprom");
   for (int i = 0; i < 68; ++i) { EEPROM.write(i,0); }
   EEPROM.commit(); 
   EEPROM.end();
}

void IManager::saveWifi(String ssid,  String password)
{
  EEPROM.begin(512);  
  for (int i = 0; i < ssid.length(); ++i){ 
    EEPROM.write(i, ssid[i]);
  }
  for (int i = 0; i < password.length(); ++i){
    EEPROM.write(32+i, password[i]);
  }

  Serial.println("Save OK"); 
  EEPROM.commit();
  EEPROM.end(); 
}


void IManager::handleRegisterAuthWiFi() 
{

  String argSSID     = server.arg("ssid");
  String argPassword = server.arg("password");
 
  cleanDataWifi();
  saveWifi(argSSID,argPassword);
  String reqJson = "{\"status\":\"success\",\"ssid\":\""+argSSID+"\",\"password\":\""+argPassword+"\"}";
  server.send(200, "application/json",  reqJson);
  WiFi.disconnect();
  delay(1000);
  ESP.restart();

}

void IManager::handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}



