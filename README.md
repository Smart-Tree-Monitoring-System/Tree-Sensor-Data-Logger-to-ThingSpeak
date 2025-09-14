Tree Sensor Data Logger to ThingSpeak

This project uses an ESP32 to collect environmental data from multiple sensors and upload it to ThingSpeak for monitoring and logging.

The system measures:

Soil Moisture (Analog sensor)

Distance/Water Level (HC-SR04 ultrasonic sensor)

Tilt/Orientation (MPU6050 accelerometer + gyroscope)

NPK Fertilizer Values (RS485 Modbus sensor)

Data is sent periodically to ThingSpeak, where it can be visualized and analyzed.

Features

Connects to WiFi automatically.

Reads multiple sensors (analog, I2C, Modbus, ultrasonic).

Converts sensor data into numerical values.

Sends data to ThingSpeak every 15 seconds.

Prints sensor readings and upload status to Serial Monitor.

Hardware Setup
Sensor	Pin/Connection
Soil Moisture	Analog Pin 34
HC-SR04 (Ultrasonic)	TRIG: 5, ECHO: 18
MPU6050	I2C (SDA/SCL default)
NPK Sensor (RS485)	RX: 16, TX: 17

Note: RS485 uses a DE/RE control pin (pin 4 in code) for transmission.

Software Requirements

Arduino IDE or PlatformIO

ESP32 Board support installed

Libraries:

WiFi.h

HTTPClient.h

Wire.h

Adafruit_MPU6050

Adafruit_Sensor

ModbusMaster

Install libraries via Arduino Library Manager.

Configuration

Set your WiFi credentials in the code:

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";


Set your ThingSpeak API key:

const char* apiKey = "your_API_KEY";

Usage

Upload the code to ESP32 via Arduino IDE.

Open Serial Monitor to see sensor readings and ThingSpeak upload status.

Data will be sent every 15 seconds to ThingSpeak fields:

field1: Soil Moisture

field2: Distance

field3: Tilt

field4: NPK

*Notes

Adjust delay(15000) to change upload frequency.

Make sure your sensors are correctly wired and calibrated.

ThingSpeak free account allows 15-second intervals per channel.
