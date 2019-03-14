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

#define POLL_DELAY_MS 1000;

// sync with <communicator.h>
#define COMMUNICATOR_MAX_LEN 1024

PS2dev pi(PS2_ECHO_CLOCK, PS2_ECHO_DATA);

WiFiMulti wifiMulti;
QueueList<String> receive_queue;
QueueList<char> send_queue;

const char* ssid         = "EANABMaker";
const char* fetch_target = "http://eanab.local/read";

void setup() {

  // ----------
  // UART SETUP
  // ----------

  Serial.begin(115200); // UART baud is 115,200 bps
  xTaskCreate(ps2_echo_task, "ps2_echo_task", 1024, NULL, 10, NULL);

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
  if (wifiMulti.run() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(fetch_target);
    if (http.GET() == HTTP_CODE_OK) {
      String payload = http.getString();
      receive_queue.push(payload);
    } else {
      Serial.println("No new ingredients!");
    }
  }

  delay(POLL_DELAY_MS);
}

/**
   Separate thread for uart transmission.
*/
static void ps2_echo_task(void *pvParameters)
{
  // Wait for the line to be ready
  while (pi.write(0x00) != 0) {
    delay(10);
  }

  while (1) {

    // Read character from pi if pending
    unsigned char c;
    if (digitalRead(PS2_ECHO_CLOCK) == LOW || digitalRead(PS2_ECHO_DATA) == LOW) {
      while (pi.read(&c));
      send_queue.push(c);
    }

    // If there is anything in the receive queue (from WiFi), send it
    if (!receive_queue.isEmpty()) {
      String str = receive_queue.pop();
      char *raw_str = str.c_str();
      int len = str.length();
      for (int i = 0; i < len; i++) pi.write(raw_str[i]);
    }

    // Delay to keep from flooding
    delay(10);
  }
}
