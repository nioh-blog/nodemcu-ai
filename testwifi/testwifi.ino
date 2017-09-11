/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <EEPROM.h>
//nioh

const char* ssid = "razaodiada";
const char* password = "brujeria666";
static int previousMillis = 0;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server1(80);
WiFiServer server2(23);

void onSTAGotIP(WiFiEventStationModeGotIP event) {
Serial.printf("Got IP: %s\n", event.ip.toString().c_str());
NTP.init((char *)"pool.ntp.org", UTC0100);
NTP.setPollingInterval(60); // Poll every minute
}
//
//// Event Handler when WiFi is disconnected
void onSTADisconnected(WiFiEventStationModeDisconnected event) {
  //Serial.printf("WiFi connection (%s) dropped.\n", event.ssid.c_str());
  //Serial.printf("Reason: %d\n", event.reason);
}


void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  static WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;
  Serial.begin(9600);
  delay(10);
  EEPROM.begin(4096);
  
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(5000);
//  digitalWrite(LED_BUILTIN, HIGH); 

  NTP.onSyncEvent([](NTPSyncEvent_t ntpEvent) {
    switch (ntpEvent) {
    case NTP_EVENT_INIT:
      break;
    case NTP_EVENT_STOP:
      break;
    case NTP_EVENT_NO_RESPONSE:
      Serial.printf("NTP server not reachable.\n");
      break;
    case NTP_EVENT_SYNCHRONIZED:
      Serial.printf("Got NTP time: %s\n", NTP.getTimeDate(NTP.getLastSync()));
      break;
    }
  });
  gotIpEventHandler = WiFi.onStationModeGotIP(onSTAGotIP);
  disconnectedEventHandler = WiFi.onStationModeDisconnected(onSTADisconnected);

  // prepare GPIO2
  
  //digitalWrite(2, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server1.begin();
  Serial.println("Server 80 started");
  server2.begin();
  Serial.println("Server 23 started");
  
  

  // Print the IP address
  Serial.println(WiFi.localIP());
  delay(5000);
  for (int i = 0; i < 4096; i++)
    {
      EEPROM.write(i, 0);
      delay(10);
      Serial.printf("%d\n",i);
    }
  EEPROM.commit();  
}

void loop() {
//static int previousMillis = 0;

while (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    delay(1000);
    time_show();
  }

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    delay(500);
    time_show();
  }  

}

void time_show (){
time_t t = now();

Serial.printf("czas: :%d : %d : %d , %d.%d.%d\n",hour(t), minute(t), second(t), year(t), month(t), day(t) );
 // Update time status every 5 seconds
//  if ((millis() - previousMillis) > 5000) {
//    previousMillis = millis();
//
//    // Output only when time is set
//    if (timeStatus() != timeSet) {
//      return;
//    }
//    //Serial.printf("Got NTP time: %s\n", NTP.getTimeDate(NTP.getLastSync()));
//    Serial.printf("Current time: %s - First synchronized at: %s.\n",NTP.getTimeDate(now()), NTP.getTimeDate(NTP.getFirstSync()));
//                  
//  }


}


