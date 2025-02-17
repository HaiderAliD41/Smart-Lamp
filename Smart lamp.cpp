// Smart Lamp with TM1637 Clock, Touch Sensors, and ESP32
// Features: Time adjustment via touch sensors, control via Bluetooth

#include <TM1637Display.h>
#include <BluetoothSerial.h>

// Pins for TM1637
#define CLK 14 // Clock pin
#define DIO 27 // Data pin

// Pins for touch sensors
#define TOUCH_UP 32
#define TOUCH_DOWN 33
#define TOUCH_CONFIRM 25

// LED Pin for lamp control
#define LED_PIN 26

// Initialize TM1637 display
TM1637Display display(CLK, DIO);

// Initialize Bluetooth
BluetoothSerial SerialBT;

// Time variables
int hours = 12;
int minutes = 0;

// Function to update display
void updateDisplay() {
    int time = hours * 100 + minutes;
    display.showNumberDecEx(time, 0b01000000, true);
}

// Function to handle touch sensors
void handleTouch() {
    if (touchRead(TOUCH_UP) < 30) { // Adjust threshold as needed
        hours = (hours + 1) % 24;
        updateDisplay();
        delay(300); // Debounce
    }

    if (touchRead(TOUCH_DOWN) < 30) { // Adjust threshold as needed
        minutes = (minutes + 1) % 60;
        updateDisplay();
        delay(300); // Debounce
    }

    if (touchRead(TOUCH_CONFIRM) < 30) {
        SerialBT.println("Time set to: " + String(hours) + ":" + String(minutes));
        delay(300); // Debounce
    }
}

// Bluetooth callback
void handleBluetooth() {
    if (SerialBT.available()) {
        String command = SerialBT.readString();
        if (command == "ON") {
            digitalWrite(LED_PIN, HIGH);
            SerialBT.println("Lamp turned ON");
        } else if (command == "OFF") {
            digitalWrite(LED_PIN, LOW);
            SerialBT.println("Lamp turned OFF");
        } else {
            SerialBT.println("Unknown command");
        }
    }
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    SerialBT.begin("SmartLamp");
    Serial.println("Bluetooth started as SmartLamp");

    // Initialize display
    display.setBrightness(7);
    updateDisplay();

    // Initialize LED pin
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Print initialization message
    Serial.println("Smart Lamp Initialized");
}

void loop() {
    handleTouch();
    handleBluetooth();
}
