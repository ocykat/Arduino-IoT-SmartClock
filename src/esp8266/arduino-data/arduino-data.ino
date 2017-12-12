#include <SoftwareSerial.h>
#include <DHT.h>

#define debug Serial.println

/* ***** ESP8266 ***** */
const int esp_rx_pin = A8; // -> esp's rx
const int esp_tx_pin = A9; // -> esp's tx

SoftwareSerial esp(esp_tx_pin, esp_rx_pin);

// AP
const char SSID[] = "*********";
const char PASS[] = "*********";

// Port
const char PORT[] = "80";

// Server
const char THINGSPEAK_SERVER[] = "api.thingspeak.com";

// Arduino data
const char API_KEY[] = "**********";


/* ***** DHT11 ***** */
const int dht_pin = 8;

DHT dht(dht_pin, DHT11);


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

    // Debug
    debug(response);
    debug("Len: " + String(len));

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

    // Debug
    debug(response);
    debug("Len: " + String(len));

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

    // Debug
    debug(response);
    debug("Len: " + String(len));

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

    /* ***** DHT11 ***** */

    debug("*****");
    int localTemp = (int) dht.readTemperature();
    int localRH = (int) dht.readHumidity();
    debug(localTemp);
    debug(localRH);
    delay(1000);
    char localTempStr[8];
    char localRHStr[8];
    sprintf(localTempStr, "%d", localTemp);
    sprintf(localRHStr, "%d", localRH);
    debug("local temp:");
    debug(localTempStr);
    debug("local rh:");
    debug(localRHStr);


    /* ***** Send arduino data ***** */

    if (esp_startTCPConnection()) {

        delay(1000);

        debug("Sending arduino data...");

        // Generate command with request length
        char arduinoData_cmd[16] = "AT+CIPSEND=";

        char arduinoData_request[256] = "GET https://api.thingspeak.com/update?api_key=";
        strcat(arduinoData_request, API_KEY);
        strcat(arduinoData_request, "&field1=");
        strcat(arduinoData_request, localTempStr);
        strcat(arduinoData_request, "&field2=");
        strcat(arduinoData_request, localRHStr);
        strcat(arduinoData_request, " HTTP/1.1\r\nHost:api.thingspeak.com\r\n\r\n");

        char arduinoData_requestLength[4];
        sprintf(arduinoData_requestLength, "%d", strlen(arduinoData_request));

        strcat(arduinoData_cmd, arduinoData_requestLength);

        // Execute command
        esp.println(arduinoData_cmd);
        delay(1000);

        // Get response
        char arduinoData_response[256];
        int len = 0;

        while (esp.available()) {
            char c = esp.read();
            arduinoData_response[len] = c;
            len++;
        }

        if (len != 0) {
            arduinoData_response[len] = '\0';
        }

        // Wait for ">" character
        if (strstr(arduinoData_response, ">") != NULL) {
            esp.println(arduinoData_request);
            delay(2000);
            debug("Send finished");

            // Get data
            char arduinoData_result[512];
            long startWaitTime = millis();
            len = 0;

            while (millis() - startWaitTime < 12000) { // Time-out check
                if (esp.find("Date")) {
                    long startReadTime = millis();
                    while (millis() - startReadTime < 10000) {
                        while (esp.available()) {
                            arduinoData_result[len] = esp.read();
                            len++;
                        }
                    }
                }
            }

            if (len != 0) {
                // Append null character
                arduinoData_result[len] = '\0';

                // Debug
                debug("Data:");
                debug(arduinoData_result);
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
	
	debug(3000);
}
