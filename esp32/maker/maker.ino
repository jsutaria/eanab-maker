#define DEBUG

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "SPIFFS.h"
#include <QueueList.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include "ps2dev.h"

#define PS2_ECHO_CLOCK  (GPIO_NUM_4)
#define PS2_ECHO_DATA  (GPIO_NUM_5)

#define POLL_DELAY_MS 1000

// sync with <communicator.h>
#define COMMUNICATOR_MAX_LEN 1024

PS2dev pi(PS2_ECHO_CLOCK, PS2_ECHO_DATA);

WiFiMulti wifiMulti;

const char* ssid         = "EANABMaker";
const char* fetch_target = "http://eanab.local/read";

void setup() {

  // ------------
  // SERIAL SETUP
  // ------------

  Serial.begin(115200); // UART baud is 115,200 bps

  // --------
  // WIFI SETUP
  // --------

  wifiMulti.addAP(ssid);

  Serial.println();
  Serial.print("Waiting for WiFi...");

  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
   Loop continuously polls for updated ingredients.
*/
void loop() {

  // Wait for the PS/2 line to be ready
  while (pi.write(0x00) != 0) {

    #ifdef DEBUG
      Serial.print("Waiting state:");
      Serial.print(digitalRead(PS2_ECHO_CLOCK));
      Serial.print(",");
      Serial.print(digitalRead(PS2_ECHO_DATA));
      Serial.println();
    #endif

    delay(100);
  }

  while (1) {

    Serial.print("Polling now!");

    if (wifiMulti.run() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(fetch_target);
      if (http.GET() == HTTP_CODE_OK) {

        String str = http.getString();
        const char *raw_str = str.c_str();
        int len = str.length();
        for (int i = 0; i < len; i++) {
          pi.write((unsigned char)raw_str[i]);
          delay(20);
        }

        #ifdef DEBUG
          Serial.print("Wrote to pi: ");
          Serial.println(str);
        #endif

      } else {
        Serial.println("No new ingredients!");
      }
    }

    delay(POLL_DELAY_MS);
  }
}
