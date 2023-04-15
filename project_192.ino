#include <WiFi.h>
#include <Adafruit_BMP085.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Khurram_Rashid"
#define WLAN_PASS       "industrialization"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME  "khurram123"
#define AIO_KEY       "aio_guLL90mzZHU6RmNGrryXbWdPsgwU"




WiFiClient client;



Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'temperature' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>

Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, "khurram123/feeds/Temperature");

Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, "khurram123/feeds/Humidity");

// Setup a feed called 'sw1' for subscribing to changes.
Adafruit_MQTT_Subscribe red = Adafruit_MQTT_Subscribe(&mqtt, "khurram123/feeds/Slider");

Adafruit_MQTT_Subscribe green = Adafruit_MQTT_Subscribe(&mqtt, "khurram123/feeds/Slider");

Adafruit_MQTT_Subscribe blue = Adafruit_MQTT_Subscribe(&mqtt, "khurram123/feeds/Slider");

/*************************** Sketch Code ************************************/



void MQTT_connect();



const int led1 = 21;
const int led2 = 22;
const int led3 = 18;

float p;
float q;

String stringOne, stringTwo;

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for sw1 feed.
  mqtt.subscribe(&red);
  
  // Setup MQTT subscription for sw2 feed.
  mqtt.subscribe(&green);
  mqtt.subscribe(&blue);

  if (!bmp.begin()){
    Serial.println("BMP180 Sensor not found ! ! !");
    while (1){}
  }
}

uint32_t x=0;

void loop() {

  
  p = bmp.readPressure();
  q = bmp.readTemperature();
  Serial.println(p);
  Serial.println(q);
  delay(100);
  
  MQTT_connect();



  Adafruit_MQTT_Subscribe *subscription;
  


  if(! temperature.publish(q)){
   
  }
  
  if(! humidity.publish(p)){
  
  }
  
  
}


void MQTT_connect() 
{
  int8_t ret;
  if (mqtt.connected()){
    return;
  
  }

  //Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  
}
