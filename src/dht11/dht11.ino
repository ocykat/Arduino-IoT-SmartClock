#include <DHT.h>

const int DHTPin = 8;

DHT dht(DHTPin, DHT11);

void setup() {
    Serial.begin(9600);
    dht.begin();
}

void loop() {
    float temp = dht.readTemperature();
    float rh = dht.readHumidity();
    Serial.println("Temperature: " + String(temp));
    Serial.println("Humidity: " + String(rh));
    delay(2000);
}