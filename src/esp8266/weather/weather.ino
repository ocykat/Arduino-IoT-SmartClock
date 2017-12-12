#include <SoftwareSerial.h>

#define debug Serial.println

/* ***** ESP8266 ***** */
const int esp_rx_pin = 2; // -> esp's rx (A8 on MEGA)
const int esp_tx_pin = 3; // -> esp's tx (A9 on MEGA)

SoftwareSerial esp(esp_tx_pin, esp_rx_pin);

// AP
const char SSID[] = ******;
const char PASS[] = ******;

// Port
const char PORT[] = "80";

// Server
const char THINGSPEAK_SERVER[] = "api.thingspeak.com";

// Weather
const char WEATHER_GET_REQUEST[] = "GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=****** HTTP/1.1\r\nHost:api.thingspeak.com\r\n\r\n";

int hcmcTemp = 0;
int hcmcRH = 0;


void esp_flush() {
    while (esp.available()) {
        esp.read();
    }
}


bool esp_test() {
    delay(1000);

    // Execute command
    esp.println("AT");
    delay(1000);

    // Get response
    char response[128];
    int len = 0;

    while (esp.available()) {
        char c = esp.read();
        response[len] = c;
        len++;
    }
    if (len != 0) response[len] = '\0';

    // Return response
    if (strstr(response, "OK") != NULL) {
        return true;
    }
    else {
        return false;
    }
}


bool esp_reset() {
    delay(1000);
    
    // Execute command
    esp.println("AT+RST");
    delay(120);

    // Get response
    char response[256];
    int len = 0;

    long startTime = millis();
    while (millis() - startTime < 3000) {
        while (esp.available()) {
            char c = esp.read();
            response[len] = c;
            len++;
        }
    }
    if (len != 0) response[len] = '\0';

    // Debug
    debug(response);
    debug("Len: " + String(len));

    // Flush the buffer
    esp_flush();

    // Return response
    if (strstr(response, "OK") != NULL) {
        debug("ESP: reset successfully");
        return true;
    }
    else {
        debug("ESP: cannot reset");
        return false;
    }
}


bool esp_setStationMode() {
    delay(1000);

    // Execute command
    esp.println("AT+CWMODE=1");
    delay(1000);
    
    // Get response
    char response[128];
    int len = 0;

    while (esp.available()) {
        char c = esp.read();
        response[len] = c;
        len++;
    }
    if (len != 0) response[len] = '\0';

    // Debug
    debug(response);
    debug("Len: " + String(len));

    // Flush the buffer
    esp_flush();

    // Return response
    if ((strstr(response, "OK") != NULL) || (strstr(response, "no change") != NULL)) {
        debug("ESP: Mode set");
        return true;
    }
    else {
        debug("ESP: Mode not set");
        return false;
    }
}


bool esp_connectToAP(const char* ssid, const char* pass) {
    delay(1000);

    // Execute command
    char cmd[128] = "AT+CWJAP=";
    strcat(cmd, "\"");
    strcat(cmd, ssid);
    strcat(cmd, "\",\"");
    strcat(cmd, pass);
    strcat(cmd, "\"");
    esp.println(cmd);
    delay(15000);

    // Get response
    char response[128];
    int len = 0;

    while (esp.available()) {
        char c = esp.read();
        response[len] = c;
        len++;
    }
    if (len != 0) response[len] = '\0';

    // Flush the buffer
    delay(2000);
    esp_flush();

    // Return result
    if ((strstr(response, "OK") != NULL) || (strstr(response, "no change") != NULL)) {
        debug("Connected to AP!");
        return true;
    }
    else {
        debug("Could not connect to AP!");
        return false;
    }
}


bool esp_setSingleConnection() {
    delay(1000);

    // Execute command
    esp.println("AT+CIPMUX=0");
    delay(1000);

    // Get response
    char response[128];
    int len = 0;

    while (esp.available()) {
        char c = esp.read();
        response[len] = c;
        len++;
    }
    if (len != 0) response[len] = '\0';

    // Flush the buffer
    esp_flush();

    // Return result
    if ((strstr(response, "OK") != NULL) || (strstr(response, "no change") != NULL)) {
        debug("ESP: Single connection set.");
        return true;
    }
    else {
        debug("ESP: Single connection could not set.");
        return false;
    }
}


bool esp_startTCPConnection() {
    delay(1000);

    // Execute command
    esp.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
    delay(4000);

    // Get response
    char response[256];
    int len = 0;

    while (esp.available()) {
        char c = esp.read();
        response[len] = c;
        len++;
    }
    if (len != 0) response[len] = '\0';


    // Flush the buffer
    esp_flush();

    // Return result
    if ((strstr(response, "OK") != NULL) || (strstr(response, "CONNECT") != NULL)) {
        /*debug("ESP: Connected to server!");*/
        return true;
    }
    else {
        /*debug("ESP: Could not connect to server!");*/
        return false;
    }
}


int esp_getTemperature(const char * data) {

    // Find the pointer to the substring
    char* p = strstr(data, "Temp");
    if (p == NULL) return 0;
    p += 5;

    // Find the length of the substring
    char* q = p;
    while (*q != '<') {
        if (q == NULL) return 0;
        q++;
    }
    int len = q - p;

    // Extract substring for result
    char resStr[3];
    strncpy(resStr, p, len);
    int res = atoi(resStr);
    return res;
}


int esp_getHumidity(const char* data) {
    
    // Find the pointer to the substring
    char* p = strstr(data, "RH");
    if (p == NULL) return 0;
    p += 3;
    
    // Find the length of the substring
    char* q = p;
    while (*q != '<') {
        if (q == NULL) return 0;
        q++;
    }
    int len = q - p;

    // Extract substring for result
    char resStr[3];
    strncpy(resStr, p, len);
    int res = atoi(resStr);
    return res;
}


void setup() {
    Serial.begin(9600);
    esp.begin(9600);
    while (!esp_test());
    while (!esp_reset());
    while (!esp_setStationMode());
    while (!esp_connectToAP(SSID, PASS));
    while (!esp_setSingleConnection());
}


void loop() {
    debug("\n\n_____________________________________________\n\n");

    char data[2048];

    /* ***** Request weather data ***** */
    if (esp_startTCPConnection()) {

        // Generate command with request length
        char cmd[16] = "AT+CIPSEND=";
        char requestLength[4];
        sprintf(requestLength, "%d", strlen(WEATHER_GET_REQUEST));
        strcat(cmd, requestLength);
        /*strcat(cmd, "\r\n");*/

        // Execute command
        esp.println(cmd);
        delay(1000);

        // Get response
        char response[256];
        int len = 0;

        while (esp.available()) {
            char c = esp.read();
            response[len] = c;
            len++;
        }

        if (len != 0) {
            response[len] = '\0';
        }

        // Wait for ">" character
        if (strstr(response, ">") != NULL) {
            
            // Send GET request
            esp.println(WEATHER_GET_REQUEST);
            delay(1000);

            // Get data
            long startWaitTime = millis();
            len = 0;

            while (millis() - startWaitTime < 12000) { // Time-out check
                if (esp.find("Date")) {
                    long startReadTime = millis();
                    while (millis() - startReadTime < 10000) {
                        while (esp.available()) {
                            data[len] = esp.read();
                            len++;
                        }
                    }
                }
            }

            if (len != 0) {
                // Append null character
                data[len] = '\0';

                // Debug
                debug("Data:");
                debug(data);

                // Update
                int resultTemp = esp_getTemperature(data);
                int resultRH = esp_getHumidity(data);
                hcmcTemp = (resultTemp == 0) ? (hcmcTemp) : (resultTemp);
                hcmcRH = (resultRH == 0) ? (hcmcRH): (resultRH);

            }
            else {
                debug("Error: Request timed out!");
            }
        }
        else {
            debug("Error: Could not send request. Trying again...");
        }
    }
    else {
        debug("Error: Could not start connection. Trying again...");
    }

    debug(hcmcTemp);
    debug(hcmcRH);
    delay(3000);
}
