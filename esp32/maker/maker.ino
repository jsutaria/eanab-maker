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

#define ECHO_TEST_TXD  (GPIO_NUM_4)
#define ECHO_TEST_RXD  (GPIO_NUM_5)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define POLL_DELAY_MS 1000;

// sync with <communicator.h>
#define COMMUNICATOR_MAX_LEN 1024

WiFiMulti wifiMulti;
QueueList<String> receive_queue;
QueueList<String> send_queue;

const char* ssid         = "EANABMaker";
const char* fetch_target = "http://eanab.local/read";

void setup() {

  // ----------
  // UART SETUP
  // ----------

  Serial.begin(115200); // UART baud is 115,200 bps
  xTaskCreate(uart_echo_task, "uart_echo_task", 1024, NULL, 10, NULL);

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
