# Weather Monitoring Station

## 1. Project Overview

The Weather Monitoring Station is an ESP32-based environmental monitoring system designed to continuously measure and display weather-related parameters.

The mandatory temperature and humidity measurements are obtained using a DHT22 sensor. Additional environmental measurements are obtained using a BMP180 pressure sensor and an LDR for light level measurement.

The system provides information through:

- OLED display
- Serial Monitor
- LED and buzzer temperature alert
- Blynk IoT dashboard

The project is simulated using Wokwi and developed in Visual Studio Code using PlatformIO.

---

## 2. Objectives

The main objectives are:

1. Measure temperature and humidity in real time.
2. Measure atmospheric pressure.
3. Estimate altitude using the BMP180.
4. Measure light intensity using an LDR.
5. Display readings locally on an OLED.
6. Display readings on the Serial Monitor.
7. Upload sensor values to Blynk.
8. Generate an alert when temperature exceeds a predefined threshold.
9. Demonstrate an IoT-enabled environmental monitoring system.

---

## 3. Features

### Mandatory

- Temperature monitoring
- Humidity monitoring
- Continuous sensor reading
- Serial Monitor output
- Real-time display

### Additional

- Atmospheric pressure
- Altitude estimation
- Light level measurement
- OLED display
- High-temperature LED indicator
- High-temperature buzzer
- Blynk cloud dashboard
- Wokwi simulation
- PlatformIO development environment

---

## 4. Components

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 DevKit | 1 | Main controller |
| DHT22 | 1 | Temperature and humidity |
| BMP180 | 1 | Pressure and altitude |
| LDR module | 1 | Light level |
| SSD1306 OLED 128x64 | 1 | Local display |
| LED | 1 | High-temperature indicator |
| Buzzer | 1 | High-temperature alert |

All components are simulated in Wokwi.

---

## 5. Pin Configuration

### DHT22

| Pin | ESP32 |
|---|---:|
| DATA | GPIO 4 |
| VCC | 3.3V/5V according to simulation |
| GND | GND |

### LDR

| Pin | ESP32 |
|---|---:|
| AO | GPIO 34 |
| VCC | 3.3V |
| GND | GND |

The analog output is read using:

```cpp
analogRead(34);
```

The LDR's digital output is not required because the project uses the analog light level.

### OLED

| OLED | ESP32 |
|---|---:|
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| VCC | 3.3V |
| GND | GND |

### BMP180

The BMP180 communicates using I2C.

| BMP180 | ESP32 |
|---|---:|
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| VCC | 3.3V |
| GND | GND |

### Alert Devices

| Device | ESP32 |
|---|---:|
| LED | GPIO 26 |
| Buzzer | GPIO 25 |

---

## 6. Working Principle

### DHT22

The DHT22 provides:

- Temperature in degrees Celsius
- Relative humidity in percentage

The ESP32 reads these values periodically.

### BMP180

The BMP180 measures atmospheric pressure.

The program converts pressure from Pa to hPa:

```cpp
pressure = bmp.readPressure() / 100.0;
```

Altitude is estimated using the reference pressure:

```cpp
altitude = bmp.readAltitude(1013.25);
```

### LDR

The LDR produces an analog signal related to the amount of light.

The ESP32 reads the value using:

```cpp
lightLevel = analogRead(LDR_PIN);
```

The exact ADC value is a relative light-level indication rather than a calibrated lux measurement.

---

## 7. System Flow

```text
DHT22 ─────────┐
BMP180 ────────┤
LDR ───────────┤
               ▼
             ESP32
          ┌────┼─────┐
          │    │     │
          ▼    ▼     ▼
        OLED Serial Blynk
          │    │     │
          └────┴─────┘
               │
       Temperature Alert
          LED + Buzzer
```

---

## 8. OLED Display

The OLED displays:

```text
WEATHER STATION

Temp: 25.4 C
Hum : 52.3 %
Pres: 1012.8 hPa
Light: 1850
```

The display is refreshed periodically.

---

## 9. Serial Monitor

The ESP32 uses:

```cpp
Serial.begin(115200);
```

Example output:

```text
==============================
  WEATHER MONITORING STATION
==============================

Temperature : 25.4 C
Humidity    : 52.3 %
Pressure    : 1012.8 hPa
Altitude    : 4.2 m
Light Level : 1850
------------------------------
```

If a DHT22 reading fails, the program reports:

```text
DHT22 reading failed!
```

---

## 10. Temperature Alert

A high-temperature threshold is defined in the program:

```cpp
if (temperature > 35)
```

When temperature exceeds 35 °C:

- LED turns ON.
- Buzzer turns ON.
- Serial Monitor reports a high-temperature warning.
- Blynk receives an alert status.

When temperature returns to the normal range:

- LED turns OFF.
- Buzzer turns OFF.
- Blynk receives NORMAL status.

---

## 11. Blynk IoT Integration

The ESP32 connects to Wokwi Wi-Fi:

```text
SSID: Wokwi-GUEST
Password: empty
```

### Blynk Datastreams

| Virtual Pin | Parameter |
|---|---|
| V0 | Temperature |
| V1 | Humidity |
| V2 | Pressure |
| V3 | Altitude |
| V4 | Light Level |
| V5 | Weather Alert Status |

Example:

```cpp
Blynk.virtualWrite(V0, temperature);
Blynk.virtualWrite(V1, humidity);
Blynk.virtualWrite(V2, pressure);
Blynk.virtualWrite(V3, altitude);
Blynk.virtualWrite(V4, lightLevel);
```

For V5, the project can report:

```text
NORMAL
```

or:

```text
HIGH TEMPERATURE
```

Do not publish the Blynk Auth Token in a public repository. Replace it with a placeholder in shared code.

---

## 12. Software Requirements

- Visual Studio Code
- PlatformIO IDE
- Wokwi Simulator
- Blynk Cloud account
- ESP32 Arduino framework

Libraries:

- Blynk
- DHT sensor library
- Adafruit Unified Sensor
- Adafruit BMP085/BMP180 library
- Adafruit GFX Library
- Adafruit SSD1306

---

## 13. Project Structure

```text
weather_monitoring/
│
├── src/
│   └── main.cpp
│
├── diagram.json
├── platformio.ini
├── wokwi.toml
└── libraries.txt
```

### `platformio.ini`

The PlatformIO configuration contains the ESP32 Arduino framework and all required libraries.

Example:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

lib_deps =
    blynkkk/Blynk
    adafruit/DHT sensor library
    adafruit/Adafruit Unified Sensor
    adafruit/Adafruit BMP085 Library
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SSD1306
```

---

## 14. Wokwi Configuration

The VS Code Wokwi configuration uses the compiled PlatformIO firmware.

Example `wokwi.toml`:

```toml
[wokwi]
version = 1
firmware = '.pio/build/esp32dev/firmware.bin'
elf = '.pio/build/esp32dev/firmware.elf'
rfc2217ServerPort = 4000
```

---

## 15. Function Organization

The program is divided into logical functions:

```text
setup()
    ↓
Initialize sensors
Initialize OLED
Connect Blynk
Start timers
    ↓
loop()
    ↓
Blynk.run()
timer.run()
checkAlerts()
updateOLED()
```

Sensor functions:

```text
readDHT22()
readBMP180()
readLDR()
```

Communication functions:

```text
sendToBlynk()
sendSensorData()
printSerialData()
```

Display/alert functions:

```text
updateOLED()
checkAlerts()
```

Function declarations should be placed before `setup()` when compiling as `src/main.cpp`.

---

## 16. How to Run

### Step 1: Open the project

Open:

```text
D:\weather_monitoring
```

in VS Code.

### Step 2: Build

Go to:

`PlatformIO → Project Tasks → esp32dev → General → Build`

Wait for:

```text
SUCCESS
```

### Step 3: Start Wokwi

Open `diagram.json` and start the Wokwi simulation.

### Step 4: Check OLED

Verify that the weather readings are displayed.

### Step 5: Open Serial Monitor

Use the TCP serial monitor:

```text
Mode: TCP
Host: localhost
Port: 4000
Baud: 115200
```

### Step 6: Open Blynk

Open the Blynk device dashboard and verify:

- Temperature
- Humidity
- Pressure
- Altitude
- Light Level
- Alert status

---

## 17. Testing

### Test 1: Temperature and Humidity

Change the DHT22 simulated values.

Expected:

```text
Temperature: updated value
Humidity: updated value
```

The OLED, Serial Monitor and Blynk should reflect the new values.

### Test 2: Pressure

Change the BMP180 pressure conditions.

Expected:

```text
Pressure: updated hPa value
Altitude: updated value
```

### Test 3: Light

Change the LDR light level.

Expected:

```text
Light Level: updated ADC value
```

### Test 4: High Temperature

Set temperature above 35 °C.

Expected:

```text
LED: ON
Buzzer: ON
Blynk Alert: HIGH TEMPERATURE
```

### Test 5: Normal Temperature

Set temperature below or equal to 35 °C.

Expected:

```text
LED: OFF
Buzzer: OFF
Blynk Alert: NORMAL
```

---

## 18. Error Handling

The DHT22 reading is validated using:

```cpp
isnan(temperature)
isnan(humidity)
```

If the reading is invalid, the program prints an error instead of using invalid sensor data.

The BMP180 is checked during initialization. If it is not detected, the program reports:

```text
BMP180 not detected!
```

The OLED is also checked during initialization.

---

## 19. Advantages

- Real-time environmental monitoring
- Multiple environmental parameters
- OLED local display
- IoT cloud monitoring
- Temperature alert
- Low-cost embedded-system concept
- Easy to expand with additional sensors
- Suitable for smart agriculture, homes and environmental monitoring

---

## 20. Limitations

- Wokwi simulation does not reproduce all physical sensor characteristics.
- LDR ADC value is not directly calibrated to lux.
- BMP180 altitude is dependent on reference atmospheric pressure.
- DHT22 has slower sampling compared with some modern sensors.
- Blynk requires network connectivity for cloud monitoring.

---

## 21. Future Enhancements

Possible improvements include:

- Rain sensor
- Wind-speed sensor
- Real-time clock
- SD-card data logging
- GPS location
- Weather forecasting
- Historical graphs
- Mobile notifications
- Solar power
- Battery backup
- Automatic fan control
- Automatic irrigation
- Cloud data analytics
- Machine-learning-based weather prediction

---

## 22. Troubleshooting

### `BlynkSimpleEsp32.h` not found

Add Blynk to `platformio.ini`:

```ini
lib_deps =
    blynkkk/Blynk
```

Then build again.

### Function not declared

Because the code is in `src/main.cpp`, add function declarations before `setup()`:

```cpp
void readDHT22();
void readBMP180();
void readLDR();
void sendToBlynk();
void sendSensorData();
void checkAlerts();
void printSerialData();
void updateOLED();
```

### OLED not detected

Check:

```text
SDA → GPIO 21
SCL → GPIO 22
Address → 0x3C
```

### BMP180 not detected

Check the I2C connection and the address used by the Wokwi component.

### Serial Monitor blank

Check:

```cpp
Serial.begin(115200);
```

and the Wokwi serial connections:

```text
ESP32 TX → $serialMonitor RX
ESP32 RX → $serialMonitor TX
```

For the VS Code TCP monitor, use port:

```text
4000
```

### Blynk dashboard not updating

Check:

- Template ID
- Template Name
- Auth Token
- Wi-Fi credentials
- Virtual Pin numbers
- Blynk device status

---

## 23. Conclusion

The Weather Monitoring Station demonstrates an IoT-enabled embedded system capable of continuously measuring temperature, humidity, atmospheric pressure, altitude and light level.

The ESP32 processes sensor data and presents it locally through an OLED and Serial Monitor while simultaneously transmitting the measurements to Blynk for remote monitoring.

The project provides a foundation for a larger environmental monitoring system with data logging, alerts, weather prediction and additional sensors.
