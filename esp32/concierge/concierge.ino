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

#define ECHO_TEST_TXD  (GPIO_NUM_4)
#define ECHO_TEST_RXD  (GPIO_NUM_5)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

// sync with <communicator.h>
#define COMMUNICATOR_MAX_LEN 1024

AsyncWebServer server(80);
QueueList<String> receive_queue;
QueueList<String> send_queue;

const char* ssid      = "EANABMaker";
const char* mdns_name = "eanab";

void setup() {

  // ----------
  // UART SETUP
  // ----------

  Serial.begin(115200); // UART baud is 115,200 bps
  xTaskCreate(uart_echo_task, "uart_echo_task", 1024, NULL, 10, NULL);

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
  server.on("/", HTTP_POST, [](AsyncWebServerRequest * request) {
    request->send(200);
  });
  server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    String message = (char *)data;
    receive_queue.push(message);
  });

  /**
   * Polled by Maker in order to read updates.
   */
  server.on("/read", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!send_queue.isEmpty()) {
      String message = send_queue.pop();
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

/**
 * Separate thread for uart transmission.
 */
static void uart_echo_task(void *pvParameters)
{
  // Configure parameters of a UART driver & communication pins, and install the driver.
  uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(UART_NUM_1, &uart_config);
  uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
  uart_driver_install(UART_NUM_1, COMMUNICATOR_MAX_LEN * 2, 0, 0, NULL, 0);

  // Temporary buffer for the incoming data
  char *data = (char *) malloc(COMMUNICATOR_MAX_LEN);

  while (1) {

    // If there is anything in the receive queue (from WiFi), send it on UART
    if (!receive_queue.isEmpty()) {
      String str = receive_queue.pop();
      uart_write_bytes(UART_NUM_1, str.c_str(), str.length());
    }

    // If there is any response waiting, send it back over WiFi
    int len = uart_read_bytes(UART_NUM_1, (uint8_t *)data, COMMUNICATOR_MAX_LEN, 20 / portTICK_RATE_MS);
    if (len > 0) {
      data[len] = '\0';
      send_queue.push(data);
    }

    // Delay to keep from flooding
    delay(10);
  }
}

void loop() {}
