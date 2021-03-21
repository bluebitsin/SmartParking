/*
 * Hoopti Check-in Facility 
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Set WiFi Credentials
const char *ssid ="Mkn88";
const char *password ="12345678";
//const char *ssid ="Venom2.4";
//const char *password ="Th0$3188";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

// Give static ip to ESP Optional


// Set api server base url
const String host = "http://digitalinhouse.in/apis/";

void setup() {

  delay(1000);
  Serial.begin(9600);

  //connecting to network
  connectToNetwork();

  //initialize the parking facility
  initializingSensors();
   
}

void loop() {
  // put your main code here, to run repeatedly:
  
  String response = httpGetLiveData("update_led_indicators.php");
  Serial.print("Response: ");
  //Serial.println(response);
  parseJsonData(response);
  delay(3000);

}

/*
 * This function defines how to connect to your nearest wifi network
 */
void connectToNetwork(){
  WiFi.mode(WIFI_OFF); //prevent reconnection issue (taking too long to connect)
   delay(1000);
   WiFi.mode(WIFI_STA); //this line hides the viewing of ESP as wifi hotspot

   WiFi.begin(ssid, password);
   Serial.println("");
   Serial.print("Connecting...");

   //wait for connection
   while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print("...");
    }

    //If connecting is successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //gives local IP address assigned to ESP
  }

  /*
   * This function initializing all the hardware sensors 
   * before been get updated with new values
   */
   void initializingSensors(){

      updateLedIndicators();
      
    }
   

  /*
   * This function defines the 
   */
  void updateLedIndicators(){
    
    
    }


    /*
     * 
     * Http Get request to get live information 
     */
     String httpGetLiveData(String apiName){

        String completeURL = host+apiName;
        HTTPClient http;
        http.begin(completeURL); 

        //send http get request
        int httpResponseCode = http.GET();

        String payload = "{}";
        
        if(httpResponseCode > 0){
            Serial.println("Response Code: ");
            Serial.print(httpResponseCode);
            payload = http.getString();
          }
          else{
              Serial.print("Error code");
              Serial.println(httpResponseCode);
            }

         //free resources
         http.end();

         return payload;
      
      }

      /*
       * 
       * Parsing Json Object
       */
       bool parseJsonData(String sensorReadings){

          //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
              
      Serial.println(sensorReadings);
      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, sensorReadings);

      if(error){

          Serial.print("Error: ");
          Serial.println(error.c_str());

          return false;
        
        }

        const char* apiStatus = doc["status"];
        int ledOn = doc["indicator"];

        Serial.print("Api Status: ");
        Serial.println(apiStatus);
        Serial.print("Indicator: ");
        Serial.println(ledOn);

      /*
       * 1 led indicators - input/read
       * 2 servo gate open / close - input/read - need 2 apis one for open gate another for close
       * 3 lcd display - input/read - occupide booked vaccant
       * 4 ir sensors - output/update
       * 5 
       */
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
        
}



/*
int latchPin = D2;
int clockPin = D0;
int dataPin = D1;

byte leds = 0;

void setup() {
  
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  Serial.begin(9600);

}

void loop() {

  leds = 0;
  updateShiftRegister();
  delay(1000);
  for (int i = 0; i < 8; i++)
  {
    bitSet(leds, i);
    updateShiftRegister();

    for (int i = 7; i >= 0; i--)
    {
        bool b = bitRead(leds, i);
        Serial.print(b);
    }
    
    delay(1000);
    Serial.println(" ");
  }

}

void updateShiftRegister() {
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}

*/
