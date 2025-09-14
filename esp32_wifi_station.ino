#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ModbusMaster.h>

// WiFi Credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "your_API_KEY";

// Sensor Pins
#define SOIL_MOISTURE_PIN 34  // Analog pin for soil moisture sensor
#define TRIG_PIN 5             // HC-SR04 Trig pin
#define ECHO_PIN 18            // HC-SR04 Echo pin
#define RX_PIN 16              // RS485 RX
#define TX_PIN 17              // RS485 TX

Adafruit_MPU6050 mpu;
HardwareSerial mySerial(2);
ModbusMaster node;

void preTransmission() {
    digitalWrite(4, HIGH);
}
void postTransmission() {
    digitalWrite(4, LOW);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    
    Wire.begin();
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }
    
    mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    node.begin(1, mySerial);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);
}

void loop() {
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    float distance = getDistance();
    float tilt = getTilt();
    int npk = readNPK();
    
    Serial.print("Soil Moisture: "); Serial.println(soilMoisture);
    Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
    Serial.print("Tilt: "); Serial.print(tilt); Serial.println(" degrees");
    Serial.print("NPK Value: "); Serial.println(npk);
    
    sendDataToThingSpeak(soilMoisture, distance, tilt, npk);
    delay(15000);  // Send data every 15 seconds
}

float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2;
    return distance;
}

float getTilt() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    return atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
}

int readNPK() {
    uint8_t result = node.readInputRegisters(0x00, 1);
    if (result == node.ku8MBSuccess) {
        return node.getResponseBuffer(0);
    } else {
        Serial.println("Error reading NPK sensor");
        return -1;
    }
}

void sendDataToThingSpeak(int moisture, float distance, float tilt, int npk) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(server) + "?api_key=" + apiKey + "&field1=" + moisture + "&field2=" + distance + "&field3=" + tilt + "&field4=" + npk;
        http.begin(url);
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            Serial.print("Data sent successfully: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Error sending data: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    }
}
