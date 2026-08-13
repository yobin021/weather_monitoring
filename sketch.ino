#include "secrets.h"


#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// ==================== PIN DEFINITIONS ====================

#define DHT_PIN 4
#define DHT_TYPE DHT22

#define LDR_PIN 34
#define LED_PIN 26
#define BUZZER_PIN 25

// ==================== OLED ====================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// ==================== SENSOR OBJECTS ====================

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP085 bmp;

// ==================== BLYNK TIMER ====================

BlynkTimer timer;

// ==================== VARIABLES ====================

float temperature;
float humidity;
float pressure;
float altitude;

int lightLevel;

// ==================== SETUP ====================

void setup() {

  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Start DHT22
  dht.begin();

  // Start I2C
  Wire.begin(21, 22);

  // Start BMP180
  if (!bmp.begin(0x76)) {

    Serial.println("BMP180 not detected!");

    while (1);
  }

  // Start OLED
  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C
      )) {

    Serial.println("OLED not detected!");

    while (1);
  }

  // Starting screen
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(20, 20);
  display.println("Weather Station");

  display.setCursor(30, 35);
  display.println("Starting...");

  display.display();

  delay(2000);

  // ==================== BLYNK CONNECTION ====================

  Serial.println("Connecting to Blynk...");

  Blynk.begin(
    BLYNK_AUTH_TOKEN,
    ssid,
    pass
  );

  // Send sensor data every 2 seconds
  timer.setInterval(2000L, sendSensorData);

  Serial.println();
  Serial.println("==============================");
  Serial.println("  WEATHER MONITORING STATION");
  Serial.println("==============================");
}

// ==================== READ DHT22 ====================

void readDHT22() {

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {

    Serial.println("DHT22 reading failed!");

    return;
  }
}

// ==================== READ BMP180 ====================

void readBMP180() {

  pressure = bmp.readPressure() / 100.0;

  altitude = bmp.readAltitude(1013.25);
}

// ==================== READ LDR ====================

void readLDR() {

  lightLevel = analogRead(LDR_PIN);
}

// ==================== SEND DATA TO BLYNK ====================

void sendToBlynk() {

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, pressure);
  Blynk.virtualWrite(V3, altitude);
  Blynk.virtualWrite(V4, lightLevel);

  Serial.println(">>> Data sent to Blynk:");
  Serial.print("V0 Temperature = ");
  Serial.println(temperature);

  Serial.print("V1 Humidity = ");
  Serial.println(humidity);

  Serial.print("V2 Pressure = ");
  Serial.println(pressure);

  Serial.print("V3 Altitude = ");
  Serial.println(altitude);

  Serial.print("V4 Light = ");
  Serial.println(lightLevel);
}

// ==================== READ ALL SENSORS ====================

void sendSensorData() {

  readDHT22();

  readBMP180();

  readLDR();

  sendToBlynk();

  printSerialData();
}

// ==================== WEATHER ALERT ====================

void checkAlerts() {

  if (temperature > 35) {

    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    Serial.println("⚠ WARNING: HIGH TEMPERATURE!");

    Blynk.virtualWrite(V5, "HIGH TEMPERATURE");

  } 
  else {

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    Blynk.virtualWrite(V5, "NORMAL");
  }
}
// ==================== SERIAL DISPLAY ====================

void printSerialData() {

  Serial.println();
  Serial.println("------------------------------");

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity    : ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure    : ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Altitude    : ");
  Serial.print(altitude);
  Serial.println(" m");

  Serial.print("Light Level : ");
  Serial.println(lightLevel);

  Serial.println("------------------------------");
}

// ==================== OLED DISPLAY ====================

void updateOLED() {

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("WEATHER STATION");

  display.setCursor(0, 15);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 27);
  display.print("Hum : ");
  display.print(humidity, 1);
  display.println(" %");

  display.setCursor(0, 39);
  display.print("Pres: ");
  display.print(pressure, 1);
  display.println(" hPa");

  display.setCursor(0, 51);
  display.print("Light: ");
  display.println(lightLevel);

  display.display();
}



// ==================== MAIN LOOP ====================

void loop() {

  // Keep Blynk connected
  Blynk.run();

  // Run scheduled functions
  timer.run();

  // Check temperature alert
  checkAlerts();

  // Update OLED
  updateOLED();
}