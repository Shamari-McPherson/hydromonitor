// //##################################################################################################################
// //##                                      ELET2415 DATA ACQUISITION SYSTEM CODE                                   ##
// //##                                                                                                              ##
// //##################################################################################################################

// // LIBRARY IMPORTS
// #include <rom/rtc.h> 
// #include <math.h>  // https://www.tutorialspoint.com/c_standard_library/math_h.htm 
// #include <ctype.h>

// // ADD YOUR IMPORTS HERE
// #include<DHT.h>
// #include<FastLED.h>



// #ifndef _WIFI_H 
// #include <WiFi.h>
// #endif

// #ifndef STDLIB_H
// #include <stdlib.h>
// #endif

// #ifndef STDIO_H
// #include <stdio.h>
// #endif

// #ifndef ARDUINO_H
// #include <Arduino.h>
// #endif 
 
// #ifndef ARDUINOJSON_H
// #include <ArduinoJson.h>
// #endif

 

// // DEFINE VARIABLES
// #define ARDUINOJSON_USE_DOUBLE      1 

// // DEFINE THE CONTROL PINS FOR THE DHT22 
// #define DHTPIN 32
// #define DHTTYPE DHT22

// #define DATA_PIN 22
// #define  NUM_LEDS 14




// // MQTT CLIENT CONFIG  
// static const char* pubtopic      = "620152241";                    // Add your ID number here
// static const char* subtopic[]    = {"620152241_sub","/elet2415"};  // Array of Topics(Strings) to subscribe to
// static const char* mqtt_server   = "dbs.msjrealtms.com";         // Broker IP address or Domain name as a String 
// static uint16_t mqtt_port        = 1883;

// // WIFI CREDENTIALS
// const char* ssid       = "MonaConnect";     // Add your Wi-Fi ssid
// const char* password   = ""; // Add your Wi-Fi password 

// //const char* ssid       = "CWC-6848515";     // Add your Wi-Fi ssid
// //const char* password   = "ddcdwwSWg4cb"; // Add your Wi-Fi password 



// // TASK HANDLES 
// TaskHandle_t xMQTT_Connect          = NULL; 
// TaskHandle_t xNTPHandle             = NULL;  
// TaskHandle_t xLOOPHandle            = NULL;  
// TaskHandle_t xUpdateHandle          = NULL;
// TaskHandle_t xButtonCheckeHandle    = NULL;  

// // FUNCTION DECLARATION   
// void checkHEAP(const char* Name);   // RETURN REMAINING HEAP SIZE FOR A TASK
// void initMQTT(void);                // CONFIG AND INITIALIZE MQTT PROTOCOL
// unsigned long getTimeStamp(void);   // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
// void callback(char* topic, byte* payload, unsigned int length);
// void initialize(void);
// bool publish(const char *topic, const char *payload); // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
// void vButtonCheck( void * pvParameters );
// void vUpdate( void * pvParameters );  
// bool isNumber(double number);
 

// /* Declare your functions below */ 
// double convert_Celsius_to_fahrenheit(double c);
// double convert_fahrenheit_to_Celsius(double f);
// double calcHeatIndex(double Temp, double Humid);


// /* Init class Instances for the DHT22 etcc */
// DHT dht(DHTPIN,DHTTYPE);
// CRGB leds[NUM_LEDS];
  

// //############### IMPORT HEADER FILES ##################
// #ifndef NTP_H
// #include "NTP.h"
// #endif

// #ifndef MQTT_H
// #include "mqtt.h"
// #endif

// // Temporary Variables 
// //double temp=0;
// //double humid=0;
// //double index=0;


// void setup() {
//   Serial.begin(115200);  // INIT SERIAL  

//   // INITIALIZE ALL SENSORS AND DEVICES
  
//   /* Add all other necessary sensor Initializations and Configurations here */
//   LEDS.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // Compensate for GRB ordering
//   dht.begin();

//   initialize();     // INIT WIFI, MQTT & NTP 
//   // vButtonCheckFunction(); // UNCOMMENT IF USING BUTTONS INT THIS LAB, THEN ADD LOGIC FOR INTERFACING WITH BUTTONS IN THE vButtonCheck FUNCTION
//  }
  


// void loop() {
//     // put your main code here, to run repeatedly:       
//     vTaskDelay(1000 / portTICK_PERIOD_MS);    
// }




  
// //####################################################################
// //#                          UTIL FUNCTIONS                          #       
// //####################################################################
// void vButtonCheck( void * pvParameters )  {
//     configASSERT( ( ( uint32_t ) pvParameters ) == 1 );     
      
//     for( ;; ) {
//         // Add code here to check if a button(S) is pressed
//         // then execute appropriate function if a button is pressed  

//         vTaskDelay(200 / portTICK_PERIOD_MS);  
//     }
// }

// void vUpdate( void * pvParameters )  {
//     configASSERT( ( ( uint32_t ) pvParameters ) == 1 );    
           
//     for( ;; ) {
//           // #######################################################
//           // ## This function must PUBLISH to topic every second. ##
//           // #######################################################
   
//           // 1. Read Humidity and save in variable below
//           double h = dht.readHumidity();
           
//           // 2. Read temperature as Celsius   and save in variable below
//           double t = dht.readTemperature();    
 

//           if(isNumber(t)){
//               // ##Publish update according to ‘{"id": "student_id", "timestamp": 1702212234, "temperature": 30, "humidity":90, "heatindex": 30}’
//               // 1. Create JSon object
//               StaticJsonDocument<1000> doc;
//               // 2. Create message buffer/array to store serialized JSON object
//               char message[1100]  = {0};
//               // 3. Add key:value pairs to JSon object based on above schema
//               doc["id"] = "620155827";
//               doc["timestamp"] = getTimeStamp();
//               doc["temperature"] =t;
//               doc["humidity"]= h;
//               doc["heat Index"]=calcHeatIndex(t, h);
//               // 4. Seralize / Covert JSon object to JSon string and store in message array
//               serializeJson(doc, message);
//               // 5. Publish message to a topic sobscribed to by both backend and frontend                
//               if(mqtt.connected()){
//                 publish(pubtopic, message);
//               }
//           }

          
            
//         vTaskDelay(1000 / portTICK_PERIOD_MS);  
//     }
// }

 

// unsigned long getTimeStamp(void) {
//           // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
//           time_t now;         
//           time(&now); // Retrieve time[Timestamp] from system and save to &now variable
//           return now;
// }


// void callback(char* topic, byte* payload, unsigned int length) {
//   // ############## MQTT CALLBACK  ######################################
//   // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TO
  
//   Serial.printf("\nMessage received : ( topic: %s ) \n",topic ); 
//   char *received = new char[length + 1] {0}; 
  
//   for (int i = 0; i < length; i++) { 
//     received[i] = (char)payload[i];    
//   }

//   // PRINT RECEIVED MESSAGE
//   Serial.printf("Payload : %s \n",received);

 
//   // CONVERT MESSAGE TO JSON
//   StaticJsonDocument<1000> doc;
//   DeserializationError error = deserializeJson(doc, received);  

//   if (error) {
//     Serial.print("deserializeJson() failed: ");
//     Serial.println(error.c_str());
//     return;
//   }


//   // PROCESS MESSAGE
//   const char* type = doc["type"]; 

//   if (strcmp(type, "controls") == 0){
//     // 1. EXTRACT ALL PARAMETERS: NODES, RED,GREEN, BLUE, AND BRIGHTNESS FROM JSON OBJECT
//     int nodes= doc["leds"];
//     int brightness= doc["brightness"];
//     int red = doc["color"]["r"];
//     int green= doc["color"]["g"];
//     int blue = doc["color"]["b"];
//     // 2. ITERATIVELY, TURN ON LED(s) BASED ON THE VALUE OF NODES. Ex IF NODES = 2, TURN ON 2 LED(s)
//     for(unsigned char y=0;y<nodes; y++){
//       leds[y]=CRGB(red,green,blue);
//       FastLED.setBrightness(brightness);
//       FastLED.show();
//       delay(30);
//     }
//     // 3. ITERATIVELY, TURN OFF ALL REMAINING LED(s).
//     for(unsigned char i=nodes; i<NUM_LEDS;i++){
//       leds[i]=  CRGB::Black;
//       FastLED.setBrightness(brightness);
//       FastLED.show();
//       delay(30);
//     }
   
//   }
// }

// bool publish(const char *topic, const char *payload){   
//      bool res = false;
//      try{
//         res = mqtt.publish(topic,payload);
//         // Serial.printf("\nres : %d\n",res);
//         if(!res){
//           res = false;
//           throw false;
//         }
//      }
//      catch(...){
//       Serial.printf("\nError (%d) >> Unable to publish message\n", res);
//      }
//   return res;
// }



// //** Complete the util functions below ***

// double convert_Celsius_to_fahrenheit(double c){    
//     // CONVERTS INPUT FROM °C TO °F. RETURN RESULTS 
//     double far_val=(c*(9.0/5.0))+32;
//     return far_val;   
// }

// double convert_fahrenheit_to_Celsius(double f){    
//     // CONVERTS INPUT FROM °F TO °C. RETURN RESULT  
//     double cel_val=(5.0/9.0)*(f-32);
//     return cel_val;  
// }

// double calcHeatIndex(double Temp, double Humid) {
//   // CALCULATE AND RETURN HEAT INDEX USING EQUATION FOUND AT https://byjus.com/heat-index-formula/#:~:text=The%20heat%20index%20formula%20is,an%20implied%20humidity%20of%2020%25

//   double f = convert_Celsius_to_fahrenheit(Temp);

//   double c1 = -42.379;
//   double c2 = 2.04901523;
//   double c3 = 10.14333127;
//   double c4 = -0.22475541;
//   double c5 = -(6.83783 * pow(10, -3));
//   double c6 = -(5.481717 * pow(10, -2));
//   double c7 = (1.22874 * pow(10, -3));
//   double c8 = (8.5282 * pow(10, -4));
//   double c9 = -(1.99 * pow(10, -6));

//   double HI = c1 + (c2 * f) + (c3 * Humid) + (c4 * f * Humid) + (c5 * pow(f, 2)) + (c6 * pow(Humid, 2)) + (c7 * pow(f, 2) * Humid) + (c8 * f * pow(Humid, 2)) + (c9 * pow(f, 2) * pow(Humid, 2));  
//   return convert_fahrenheit_to_Celsius(HI);

// }

// bool isNumber(double number){       
//         char item[20];
//         snprintf(item, sizeof(item), "%f\n", number);
//         if( isdigit(item[0]) )
//           return true;
//         return false; 
// }
// ##################################################################################################################
// ##                                      ELET2415 DATA ACQUISITION SYSTEM CODE                                   ##
// ##                                                                                                              ##
// ##################################################################################################################

//LIBRARY IMPORTS
#include <rom/rtc.h>
#include <math.h>  // https://www.tutorialspoint.com/c_standard_library/math_h.htm
#include <ctype.h>
#include <FastLED.h>
#include "DHT.h"
#include <string>


// ADD YOUR IMPORTS HERE



#ifndef _WIFI_H
#include <WiFi.h>
#endif

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef ARDUINO_H
#include <Arduino.h>
#endif

#ifndef ARDUINOJSON_H
#include <ArduinoJson.h>
#endif



// DEFINE VARIABLES
#define ARDUINOJSON_USE_DOUBLE 1

#define DHTTYPE DHT22 
#define LEDPIN 14
#define DHTPIN 22


#define NUM_LEDS 7

// DEFINE THE CONTROL PINS FOR THE DHT22




// MQTT CLIENT CONFIG
static const char* pubtopic = "620152241";                         // Add your ID number here
static const char* subtopic[] = { "620152241_sub", "/elet2415" };  // Array of Topics(Strings) to subscribe to
static const char* mqtt_server = "dbs.msjrealtms.com";             // Broker IP address or Domain name as a String
//static const char* mqtt_server = "www.yanacreations.com";
static uint16_t mqtt_port = 1883;

// WIFI CREDENTIALS
const char* ssid = "MonaConnect";  // Add your Wi-Fi ssid Digicel_WiFi_A8eG MonaConnect
const char* password = "";         // Add your Wi-Fi password Nu5TSQ5h




// TASK HANDLES
TaskHandle_t xMQTT_Connect = NULL;
TaskHandle_t xNTPHandle = NULL;
TaskHandle_t xLOOPHandle = NULL;
TaskHandle_t xUpdateHandle = NULL;
TaskHandle_t xButtonCheckeHandle = NULL;

// FUNCTION DECLARATION
void checkHEAP(const char* Name);  // RETURN REMAINING HEAP SIZE FOR A TASK
void initMQTT(void);               // CONFIG AND INITIALIZE MQTT PROTOCOL
unsigned long getTimeStamp(void);  // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
void callback(char* topic, byte* payload, unsigned int length);
void initialize(void);
bool publish(const char* topic, const char* payload);  // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
void vButtonCheck(void* pvParameters);
void vUpdate(void* pvParameters);
bool isNumber(double number);


/* Declare your functions below */
double convert_Celsius_to_fahrenheit(double c);
double convert_fahrenheit_to_Celsius(double f);
double calcHeatIndex(double Temp, double Humid);

/* Init class Instances for the DHT22 etcc */
DHT dht(DHTPIN, DHTTYPE);

// Define the array of leds
CRGB leds[NUM_LEDS];


//############### IMPORT HEADER FILES ##################
#ifndef NTP_H
#include "NTP.h"
#endif

#ifndef MQTT_H
#include "mqtt.h"
#endif

// Temporary Variables


void setup() {
  Serial.begin(115200);  // INIT SERIAL

  // INITIALIZE ALL SENSORS AND DEVICES

  /* Add all other necessary sensor Initializations and Configurations here */
  //FastLED.addLeds<NEOPIXEL, 14>(leds, LEDS);
  LEDS.addLeds<WS2812, LEDPIN, GRB>(leds, NUM_LEDS);
  dht.begin();

  initialize();  // INIT WIFI, MQTT & NTP
                 // vButtonCheckFunction(); // UNCOMMENT IF USING BUTTONS INT THIS LAB, THEN ADD LOGIC FOR INTERFACING WITH BUTTONS IN THE vButtonCheck FUNCTION
}



void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}





//####################################################################
//#                          UTIL FUNCTIONS                          #
//####################################################################
void vButtonCheck(void* pvParameters) {

  configASSERT(((uint32_t)pvParameters) == 1);

  for (;;) {
    // Add code here to check if a button(S) is pressed
    // then execute appropriate function if a button is pressed

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void vUpdate(void* pvParameters) {

  configASSERT(((uint32_t)pvParameters) == 1);

  for (;;) {
    // #######################################################
    // ## This function must PUBLISH to topic every second. ##
    // #######################################################

    // 1. Read Humidity and save in variable below
    double h = dht.readHumidity();

    // 2. Read temperature as Celsius   and save in variable below
    double t = dht.readTemperature();


    if (isNumber(t)) {
      // ##Publish update according to ‘{"id": "student_id", "timestamp": 1702212234, "temperature": 30, "humidity":90, "heatindex": 30}’

      // 1. Create JSon object
      StaticJsonDocument<1000> doc;

      // 2. Create message buffer/array to store serialized JSON object
      char message[1100] = { 0 };

      // 3. Add key:value pairs to JSon object based on above schema

      doc["id"] = "620152241";
      doc["timestamp"] = getTimeStamp();
      doc["temperature"] = t;
      doc["humidity"] = h;
      doc["heatindex"] = calcHeatIndex(t, h);

      // 4. Seralize / Covert JSon object to JSon string and store in message array
      serializeJson(doc, message);

      // 5. Publish message to a topic sobscribed to by both backend and frontend
     if (mqtt.connected()) {
        publish(pubtopic, message);
      }
    }
     
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }



  unsigned long getTimeStamp(void) {
    // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
    time_t now;
    time(&now);  // Retrieve time[Timestamp] from system and save to &now variable
    return now;
  }


  void callback(char* topic, byte* payload, unsigned int length) {
    // ############## MQTT CALLBACK  ######################################
    // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TO

    Serial.printf("\nMessage received : ( topic: %s ) \n", topic);
    char* received = new char[length + 1]{ 0 };

    for (int i = 0; i < length; i++) {
      received[i] = (char)payload[i];
    }

    // PRINT RECEIVED MESSAGE
    Serial.printf("Payload : %s \n", received);


    // CONVERT MESSAGE TO JSON
    StaticJsonDocument<1000> doc;
    DeserializationError error = deserializeJson(doc, received);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }


    // PROCESS MESSAGE
    const char* type = doc["type"];

    if (strcmp(type, "controls") == 0) {
      // 1. EXTRACT ALL PARAMETERS: NODES, RED,GREEN, BLUE, AND BRIGHTNESS FROM JSON OBJECT ‘{"type": "controls", "brightness": 255, "leds": 7, "color": { "r": 255, "g": 255,"b": 255, "a": 1 } }’

      int nodes       = doc["leds"];
      int brightness  = doc["brightness"];
      int red         = doc["color"]["r"]; //map<char, int> color = doc["color"];
      int green       = doc["color"]["g"]; 
      int blue        =doc["color"]["b"]; 

      // 2. ITERATIVELY, TURN ON LED(s) BASED ON THE VALUE OF NODES. Ex IF NODES = 2, TURN ON 2 LED(s)
      for (int x = 0; x < nodes; x++){
        leds[x] = CRGB(red , green, blue);
        FastLED.setBrightness(brightness);
        FastLED.show();
        delay(50);
    }
    // 3. ITERATIVELY, TURN OFF ALL REMAINING LED(s).
    for (int x = nodes; x < NUM_LEDS; x++) {
      leds[x] = CRGB::Black;
      FastLED.setBrightness(brightness);
      FastLED.show();
      delay(50);
    }
//     StaticJsonDocument<768> doc;  // Create JSon object
//     char message[800] = { 0 };
// //may need to add sumn
//       // doc["id"] = "620152241";
//       // doc["timestamp"] = getTimeStamp();
//       // doc["temperature"] = t;
//       // doc["humidity"] = h;
//       // doc["heatindex"] = calcHeatIndex(t, h);

//     serializeJson(doc, message);  // Seralize / Covert JSon object to JSon string and store in char* array
//     publish(pubtopic, message);   // Publish to a topic that only the Frontend subscribes to.
  }
}

bool publish(const char* topic, const char* payload) {
  bool res = false;
  try {
    res = mqtt.publish(topic, payload);
    // Serial.printf("\nres : %d\n",res);
    if (!res) {
      res = false;
      throw false;
    }
  } catch (...) {
    Serial.printf("\nError (%d) >> Unable to publish message\n", res);
  }
  return res;
}



//***** Complete the util functions below ******

double convert_Celsius_to_fahrenheit(double c) {
  return (c * (9 / 5)) + 32;
  // CONVERTS INPUT FROM °C TO °F. RETURN RESULTS
}

double convert_fahrenheit_to_Celsius(double f) {
  return (5.0 / 9.0) * (f - 32);
  // CONVERTS INPUT FROM °F TO °C. RETURN RESULT
}

double calcHeatIndex(double Temp, double Humid) {
  // CALCULATE AND RETURN HEAT INDEX USING EQUATION FOUND AT https://byjus.com/heat-index-formula/#:~:text=The%20heat%20index%20formula%20is,an%20implied%20humidity%20of%2020%25

  double f = convert_Celsius_to_fahrenheit(Temp);

  double c1 = -42.379;
  double c2 = 2.04901523;
  double c3 = 10.14333127;
  double c4 = -0.22475541;
  double c5 = -(6.83783 * pow(10, -3));
  double c6 = -(5.481717 * pow(10, -2));
  double c7 = (1.22874 * pow(10, -3));
  double c8 = (8.5282 * pow(10, -4));
  double c9 = -(1.99 * pow(10, -6));

  double HI = c1 + (c2 * f) + (c3 * Humid) + (c4 * f * Humid) + (c5 * pow(f, 2)) + (c6 * pow(Humid, 2)) + (c7 * pow(f, 2) * Humid) + (c8 * f * pow(Humid, 2)) + (c9 * pow(f, 2) * pow(Humid, 2));  
  return convert_fahrenheit_to_Celsius(HI);

}

bool isNumber(double number) {
  char item[20];
  snprintf(item, sizeof(item), "%f\n", number);
  if (isdigit(item[0]))
    return true;
  return false;
}

//##################################################################################################################
//##                                      ELET2415 DATA ACQUISITION SYSTEM CODE                                   ##
//##                                                                                                              ##
//##################################################################################################################

// // LIBRARY IMPORTS
// #include <rom/rtc.h>
// #include <math.h>  // https://www.tutorialspoint.com/c_standard_library/math_h.htm
// #include <ctype.h>

// // ADD YOUR IMPORTS HERE
// #include <DHT.h>;
// #include <FastLED.h>


// #ifndef _WIFI_H
// #include <WiFi.h>
// #endif

// #ifndef STDLIB_H
// #include <stdlib.h>
// #endif

// #ifndef STDIO_H
// #include <stdio.h>
// #endif

// #ifndef ARDUINO_H
// #include <Arduino.h>
// #endif

// #ifndef ARDUINOJSON_H
// #include <ArduinoJson.h>
// #endif


// // DEFINE VARIABLES
// #define ARDUINOJSON_USE_DOUBLE 1
// #define DHTTYPE DHT22
// #define NUM_LEDS 7
// #define DATA_PIN_LED 14

// // DEFINE THE CONTROL PINS FOR THE DHT22
// #define DHT22_PIN 22



// // MQTT CLIENT CONFIG
// static const char* pubtopic = "620152241";                         // Add your ID number here
// static const char* subtopic[] = { "620152241_sub", "/elet2415" };  // Array of Topics(Strings) to subscribe to
// static const char* mqtt_server = "dbs.msjrealtms.com";                          // Broker IP address or Domain name as a String
// static uint16_t mqtt_port = 1883;

// // WIFI CREDENTIALS
// //const char* ssid = "CWC-9128576 2.4";   // Add your Wi-Fi ssid
// //const char* password = "sb3Rkgqzxtcy";  // Add your Wi-Fi password

// const char* ssid = "MonaConnect";   // Add your Wi-Fi ssid
// const char* password = "";  // Add your Wi-Fi password




// // TASK HANDLES
// TaskHandle_t xMQTT_Connect = NULL;
// TaskHandle_t xNTPHandle = NULL;
// TaskHandle_t xLOOPHandle = NULL;
// TaskHandle_t xUpdateHandle = NULL;
// TaskHandle_t xButtonCheckeHandle = NULL;

// // FUNCTION DECLARATION
// void checkHEAP(const char* Name);  // RETURN REMAINING HEAP SIZE FOR A TASK
// void initMQTT(void);               // CONFIG AND INITIALIZE MQTT PROTOCOL
// unsigned long getTimeStamp(void);  // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
// void callback(char* topic, byte* payload, unsigned int length);
// void initialize(void);
// bool publish(const char* topic, const char* payload);  // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
// void vButtonCheck(void* pvParameters);
// void vUpdate(void* pvParameters);
// bool isNumber(double number);


// /* Declare your functions below */
// double convert_Celsius_to_fahrenheit(double c);
// double convert_fahrenheit_to_Celsius(double f);
// double calcHeatIndex(double Temp, double Humid);


// /* Init class Instances for the DHT22 etcc */
// DHT dht(DHT22_PIN, DHTTYPE);



// //############### IMPORT HEADER FILES ##################
// #ifndef NTP_H
// #include "NTP.h"
// #endif

// #ifndef MQTT_H
// #include "mqtt.h"
// #endif

// // Temporary Variables
// CRGB leds[NUM_LEDS];


// void setup() {
//   Serial.begin(115200);  // INIT SERIAL

//   // INITIALIZE ALL SENSORS AND DEVICES
//   dht.begin();
//   FastLED.addLeds<NEOPIXEL, DATA_PIN_LED>(leds, NUM_LEDS); //GRB

//   /* Add all other necessary sensor Initializations and Configurations here */

//   initialize();  // INIT WIFI, MQTT & NTP
//                  // vButtonCheckFunction(); // UNCOMMENT IF USING BUTTONS INT THIS LAB, THEN ADD LOGIC FOR INTERFACING WITH BUTTONS IN THE vButtonCheck FUNCTION
// }



// void loop() {
//   // put your main code here, to run repeatedly:
//   vTaskDelay(1000 / portTICK_PERIOD_MS);
// }





// //####################################################################
// //#                          UTIL FUNCTIONS                          #
// //####################################################################
// void vButtonCheck(void* pvParameters) {
//   configASSERT(((uint32_t)pvParameters) == 1);

//   for (;;) {
//     // Add code here to check if a button(S) is pressed
//     // then execute appropriate function if a button is pressed

//     vTaskDelay(200 / portTICK_PERIOD_MS);
//   }
// }

// void vUpdate(void* pvParameters) {
//   configASSERT(((uint32_t)pvParameters) == 1);

//   for (;;) {
//     // #######################################################
//     // ## This function must PUBLISH to topic every second. ##
//     // #######################################################

//     // 1. Read Humidity and save in variable below
//     double h = dht.readHumidity();

//                // 2. Read temperature as Celsius   and save in variable below
//                double t = dht.readTemperature();


//                           if (isNumber(t)) {
//       // ##Publish update according to ‘{"id": "student_id", "timestamp": 1702212234, "temperature": 30, "humidity":90, "heatindex": 30}’

//       // 1. Create JSon object

//       // 2. Create message buffer/array to store serialized JSON object

//       // 3. Add key:value pairs to JSon object based on above schema

//       // 4. Seralize / Covert JSon object to JSon string and store in message array

//       // 5. Publish message to a topic sobscribed to by both backend and frontend
//       StaticJsonDocument<768> doc;  // Create JSon object
//       char message[800] = { 0 };

//       // Add key:value pairs to Json object according to below schema
//       // ‘{"id": "student_id", "timestamp": 1702212234, "temperature": 30, "humidity":90, "heatindex": 30}’
//       doc["id"] = "620152241";
//       doc["timestamp"] = getTimeStamp();
//       doc["temperature"] = t;
//       doc["humidity"] = h;
//         doc["heatindex"] = calcHeatIndex(t, h);

//       serializeJson(doc, message);  // Seralize / Covert JSon object to JSon string and store in char* array
//       publish(pubtopic, message);   // Publish to a topic that only the Frontend subscribes to.
//     }

//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//   }
// }



// unsigned long getTimeStamp(void) {
//   // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
//   time_t now;
//   time(&now);  // Retrieve time[Timestamp] from system and save to &now variable
//   return now;
// }


// void callback(char* topic, byte* payload, unsigned int length) {
//   // ############## MQTT CALLBACK  ######################################
//   // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TOhardware | Arduino IDE 2.2.1



//   Serial.printf("\nMessage received : ( topic: %s ) \n", topic);

//   char* received = new char[length + 1]{ 0 };

//   for (int i = 0; i < length; i++) {
//     received[i] = (char)payload[i];
//   }

//   // PRINT RECEIVED MESSAGE
//   Serial.printf("Payload : %s \n", received);


//   // CONVERT MESSAGE TO JSON
//   StaticJsonDocument<1000> doc;
//   DeserializationError error = deserializeJson(doc, received);

//   if (error) {
//     Serial.print("deserializeJson() failed: ");
//     Serial.println(error.c_str());
//     return;
//   }


//   // PROCESS MESSAGE
//   const char* type = doc["type"];

//   if (strcmp(type, "controls") == 0) {
//     // 1. EXTRACT ALL PARAMETERS: NODES, RED,GREEN, BLUE, AND BRIGHTNESS FROM JSON OBJECT

//     // 2. ITERATIVELY, TURN ON LED(s) BASED ON THE VALUE OF NODES. Ex IF NODES = 2, TURN ON 2 LED(s)


//     // 3. ITERATIVELY, TURN OFF ALL REMAINING LED(s).
//     //‘{"type": "controls", "brightness": 255, "leds": 7, "color": { "r": 255, "g": 255, "b": 255, "a": 1 } }’
//     int brightness = doc["brightness"];
//     int node = doc["leds"];
//     int red = doc["color"]["r"];
//     int green = doc["color"]["g"];
//     int blue = doc["color"]["b"];
    
//     if (isNumber(node) && isNumber(red) && isNumber(green) && isNumber(blue) && isNumber(brightness)) {
//       for (int x = 0; x < node; x++) {
//         leds[x] = CRGB(red, green, blue);   // R, G, B range for each value is 0 to 255
//         FastLED.setBrightness(brightness);  // Ranges from 0 to 255     
//         FastLED.show();            // Send changes to LED array
//         vTaskDelay(50);
//       }

//       for (int x = node; x < 7; x++) {
//         leds[x] = CRGB(0, 0, 0);   // R, G, B range for each value is 0 to 255
//         FastLED.setBrightness(brightness);  // Ranges from 0 to 255
//         FastLED.show();            // Send changes to LED array
//         vTaskDelay(50);
        
//       }
//     }
     
//   }
// }

// bool publish(const char* topic, const char* payload) {
//   bool res = false;
//   try {
//     res = mqtt.publish(topic, payload);
//     // Serial.printf("\nres : %d\n",res);
//     if (!res) {
//       res = false;
//       throw false;
//     }
//   } catch (...) {
//     Serial.printf("\nError (%d) >> Unable to publish message\n", res);
//   }
//   return res;
// }



// //***** Complete the util functions below ******

// double convert_Celsius_to_fahrenheit(double c) {
//   // CONVERTS INPUT FROM °C TO °F. RETURN RESULTS
//   return (c * 1.8) + 32;
// }

// double convert_fahrenheit_to_Celsius(double f) {
//   // CONVERTS INPUT FROM °F TO °C. RETURN RESULT
//   return (f - 32) * 0.555555555;
// }

// double calcHeatIndex(double Temp, double Humid) {
//   // CALCULATE AND RETURN HEAT INDEX USING EQUATION FOUND AT
//   const double c1 = -42.379;
//   const double c2 = 2.04901523;
//   const double c3 = 10.14333127;
//   const double c4 = -0.22475541;
//   const double c5 = -6.83783e-03;
//   const double c6 = -5.481717e-02;
//   const double c7 = 1.22874e-03;
//   const double c8 = 8.5282e-04;
//   const double c9 = -1.99e-06;

//   double T = convert_Celsius_to_fahrenheit(Temp);
//   double RH = Humid;

//   double heatIndex = c1 + (c2 * T) + (c3 * RH) + (c4 * T * RH) + (c5 * T * T) + (c6 * RH * RH) + (c7 * T * T * RH) + (c8 * T * RH * RH) + (c9 * T * T * RH * RH);
//   double far = convert_fahrenheit_to_Celsius(heatIndex);
//    return far;
// }


// bool isNumber(double number) {
//   char item[20];
//   snprintf(item, sizeof(item), "%f\n", number);
//   if (isdigit(item[0]))
//     return true;
//   return false;
// }