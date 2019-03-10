// #define DEBUG

#include <WiFi.h>
#include <WebSocketServer.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include <QueueList.h>

#define ECHO_TEST_TXD  (GPIO_NUM_4)
#define ECHO_TEST_RXD  (GPIO_NUM_5)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (1024)

WiFiServer server(80);
WiFiClient wifi_client;
WebSocketServer webSocketServer;
QueueList<String> recv_q;
QueueList<String> send_q;

const char* ssid = "EANABMaker";
const char* password =  "eanab";

// start a thread to handle uart send/recv
void app_main()
{
    xTaskCreate(echo_task, "uart_echo_task", 1024, NULL, 10, NULL);
}

void setup() {
    // the UART baud is 115,200 bps
    Serial.begin(115200);

    // The blue LED is used to indicate that there is a websocket connection
    pinMode(LED_BUILTIN, OUTPUT);

    // Set up a Wi-Fi network with SSID and password
    Serial.print("Setting AP (Access Point)…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.begin();
    delay(100);
    
    // start the UART listening routine*/
    app_main();
}

// this is the UART handling routine
static void echo_task(void *pvParameters)
{
    /*  Configure parameters of an UART driver,
        communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);

    // Configure a temporary buffer for the incoming data
    // Also set up a buffer for the base64 data, so we can send all
    // 0-255 byte values
    char *data = (char *) malloc(BUF_SIZE);
    char *base64data = (char *) malloc(BUF_SIZE);

    while (1) {

        // If there is anything in the receive queue (from WiFi), send it on UART
        if (!recv_q.isEmpty()) {
            // need to make this thread safe!
            String base64data = recv_q.pop();
            int len = base64_decode(data, (char *)base64data.c_str(), base64data.length());
            #ifdef DEBUG
            Serial.print("Received data from WiFi, sending on UART: '");
            Serial.print(data);
            Serial.println("'");
            Serial.print("sending: ");
            Serial.println(data);
            #endif
            
            uart_write_bytes(UART_NUM_1, data, len);
        }


        int len = uart_read_bytes(UART_NUM_1, (uint8_t *)data, BUF_SIZE, 20 / portTICK_RATE_MS);
        if (len > 0) {
            int len_encoded = base64_encode(base64data, (char *)data, len);
            send_q.push(base64data);
            #ifdef DEBUG
            Serial.print("Received from UART: '");
            Serial.print((char *)data);
            Serial.println("'");
            #endif
        }
        delay(10);
    }
    free(data);
    free(base64data);
}

void loop() {

    wifi_client = server.available();

    if (wifi_client.connected()) {
        Serial.println("WiFi client connected!");
        if (webSocketServer.handshake(wifi_client)) {
            digitalWrite(LED_BUILTIN, HIGH);
            String base64data;
            char *data = (char *) malloc(BUF_SIZE);

            while (wifi_client.connected()) {
                if (!send_q.isEmpty()) {
                    sendWifiMessage(send_q.pop().c_str());
                }
                base64data = webSocketServer.getData();

                if (base64data.length() > 0) {
                    recv_q.push(base64data);
                }

                delay(10); // Delay needed for receiving the data correctly


            }
            free(data);
        } else {
            Serial.println("handshake failed.");
        }

        Serial.println("The websocket client disconnected");
        delay(100);
    }

    delay(100);

}

void sendWifiMessage(const char *data) {
    if (wifi_client.connected()) {
        webSocketServer.sendData(data);
        #ifdef DEBUG
        Serial.print("Received message on UART, sending the following on WiFi: '");
        Serial.print(data);
        Serial.println("'");
        Serial.println("message sent!");
        #endif
    } else {
        Serial.println("not connected to server");
    }
}
