#define FASTLED_INTERNAL
#include <FastLED.h>
#include <math.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


//
// Hardware Definitions
//
#define SPI_PIN 0
#define LED_PIN 1
#define MAX_LEDS 60
#define MAX_SEC 60
#define MAX_MIN 60
#define MAX_HRS 12
#define LED_OFFSET 30     // Used to shift the LED '12' point along the LED string

//
// User Preference Variables
//
#define COLOUR_HOUR CRGB::Red
#define COLOUR_MINUTE CRGB::Green
#define COLOUR_SECOND CRGB::Blue
#define COLOUR_FACE CRGB::DimGray

int led_hour_width = 9;         // odd numbers are equally distrbuted - Default 9
int led_minute_width = 3;       // odd numbers are equally distrbuted - Default 3
int led_second_tail = 0;        // Enable a faded tail of this length - Address Cloning required to fix this
int setting_brightness = 6;     // From 1-255 brightness (8 default)
int ntp_interval = 60;          // How often an NTP request is made to update time
int ntp_time_offset = 36000;    // +10 GMT


//
// Initial Clock Hardware & CLock Time State Values
//
int led_hour = 0;     // Hour LED position
int led_minute = 0;   // Minute LED position
int led_second = 0;   // Second LED position
int clock_second = 0; // Initial state, Updated by running Clock
int clock_minute = 0; // Initial state, Updated by running Clock
int clock_hour = 0;   // Initial state, updated by running Clock
int ntp_minutes = 60; //


//
// INIT Variables
//
CRGB leds[MAX_LEDS];    // The FastPixel Array of LEDs
int system_tick = LOW;  // Debug Activity LED
unsigned long last_runtime_millisecconds = 0; // Used to semi-multitask
unsigned long last_runtime_wait_time = 10;   // One second should be 1000

WiFiServer  server(80);  // the Webserver
//WiFiClient  client;
WiFiUDP     ntpUDP;
NTPClient   timeClient(ntpUDP, "0.au.pool.ntp.org");



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // BLANK at BOOT
  fill_solid( leds, MAX_LEDS, CRGB::Black );  FastLED.show();
  Serial.begin(115200);
  delay(100);
  Serial.println("ClockRing Started");
  pinMode (LED_PIN,  OUTPUT);
  FastLED.addLeds<NEOPIXEL, SPI_PIN>(leds, MAX_LEDS);
  fill_solid( leds, MAX_LEDS, CRGB::Black ); FastLED.show(); // Blank Everything

  connectWiFi();
  //startupAnimation();
  timeClient.begin();
  timeClient.setTimeOffset(ntp_time_offset);

}

void loop() {
  system_tick = (system_tick) ? LOW : HIGH;
  //digitalWrite(LED_PIN, system_tick);
  digitalWrite(LED_PIN, LOW);

  if ( (millis() >= (last_runtime_millisecconds + last_runtime_wait_time)) || (millis() < last_runtime_millisecconds) ) {
    last_runtime_millisecconds = millis();
    fill_solid( leds, MAX_LEDS, CRGB::Black ); // Reset LEDS with a Black out - Turn this off for funzies
    advanceClock();
    updateClockFace();
  }
  FastLED.setBrightness(setting_brightness);  // Dynamic Brightness (Using a LDR) disabled for now
  FastLED.show();

  if (ntp_minutes > ntp_interval) {
    ntpUpdateTime();
  }

  WiFiClient client = server.available();
  if (!client) {
    return;
  } else {
    webClient(client);
  }

  //delay(50); // DEBUGGING to slow down ticks
}

void ntpUpdateTime() {
  // Reminder: Hours = 1-12, Minutes = 0-59, Seconds = 0-59
  timeClient.update();
  int hours = timeClient.getHours();
  clock_hour    =  (hours % 12);
  clock_minute   = timeClient.getMinutes();
  clock_second  = timeClient.getSeconds();
}

void connectWiFi() {
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  // Connect
  fill_solid( leds, MAX_LEDS, CRGB::Black );  FastLED.show();
  for (int l = 0; l < MAX_LEDS; l++) {
    fill_gradient_RGB(leds, l, CRGB::White, 0, CRGB::Black ); FastLED.show();
    Serial.print(".");
    if (WiFi.status() != WL_CONNECTED) {
      break;
    }
    delay(5);
  }
  Serial.println("");
  Serial.println("Connected!");

  server.begin();
  Serial.print("Connect to http://");
  Serial.println(WiFi.localIP());
}




// EOF //
