#define DEBUG

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "SPIFFS.h"
#include <ESPmDNS.h>
#include <QueueList.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "ps2dev.h"

#define PS2_ECHO_CLOCK  (GPIO_NUM_4)
#define PS2_ECHO_DATA  (GPIO_NUM_5)

// sync with <communicator.h>
#define COMMUNICATOR_MAX_LEN 1024

PS2dev pi(PS2_ECHO_CLOCK, PS2_ECHO_DATA);

AsyncWebServer server(80);
QueueList<String> receive_queue;
QueueList<unsigned char> send_queue;

const char* ssid      = "EANABMaker";
const char* mdns_name = "eanab";

void setup() {

  // ----------
  // UART SETUP
  // ----------

  Serial.begin(115200); // UART baud is 115,200 bps
  // xTaskCreate(ps2_echo_task, "ps2_echo_task", 1024, NULL, 10, NULL);

  // ------------
  // SPIFFS SETUP
  // ------------

  SPIFFS.begin();

  // --------
  // AP SETUP
  // --------

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  if (MDNS.begin(mdns_name)) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  // ------------
  // SERVER SETUP
  // ------------

  /**
   * Returns the public-facing EANAB Concierge config screen.
   */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  /**
   * Handles POSTed ingredient configuration.
   */
  // server.on("/", HTTP_POST, [](AsyncWebServerRequest * request) {
  //   request->send(200);
  // });
  server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    String message = "";
    char *temp = (char *)data;
    for (int i = 0; i < len; i++) message += temp[i];
    Serial.print("Received new request: ");
    Serial.println(message);
    receive_queue.push(message);
    request->send(200);
  });

  /**
   * Polled by Maker in order to read updates.
   */
  server.on("/read", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!send_queue.isEmpty()) {
      char *message = (char *)malloc(COMMUNICATOR_MAX_LEN);
      char *ptr = message;
      while (!send_queue.isEmpty()) {
        *ptr++ = (char)send_queue.pop();
      }
      ptr = '\0';
      request->send(200, "text/plain", message);
    } else {
      request->send(404, "text/plain", "no pending message in queue");
    }
  });

  server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(400, "text/plain", "not found! :(");
  });

  server.begin();
  Serial.println("HTTP server started!");

  delay(100);
}
void loop() {
  Serial.print("A state:");
  Serial.print(digitalRead(PS2_ECHO_CLOCK));
  Serial.print(",");
  Serial.print(digitalRead(PS2_ECHO_DATA));
  Serial.println();

  // Wait for the line to be ready
  while (pi.write(0x00) != 0) {
    Serial.print("State:");
    Serial.print(digitalRead(PS2_ECHO_CLOCK));
    Serial.print(",");
    Serial.print(digitalRead(PS2_ECHO_DATA));
    Serial.println();
    delay(100);
  }

  Serial.println("Echo: established connection to pi!");

  while (1) {

    // Serial.print("A state:");
    // Serial.print(digitalRead(PS2_ECHO_CLOCK));
    // Serial.print(",");
    // Serial.print(digitalRead(PS2_ECHO_DATA));
    // Serial.println();

    // Read character from pi if pending
    unsigned char c;
    if (digitalRead(PS2_ECHO_CLOCK) == LOW || digitalRead(PS2_ECHO_DATA) == LOW) {
      pi.read(&c);
      send_queue.push(c);
      Serial.println(c);
    }

    // If there is anything in the receive queue (from WiFi), send it
    if (!receive_queue.isEmpty()) {
      String str = receive_queue.pop();
      const char *raw_str = str.c_str();
      int len = str.length();
      for (int i = 0; i < len; i++) {
        pi.write((unsigned char)raw_str[i]);
        delay(20);
      }
      Serial.print("Wrote to pi: ");
      Serial.println(str);
    }

    // Delay to keep from flooding
    delay(10);
  }  
}
