#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// *Pin Definitions*
#define HEARTBEAT_SENSOR A1 // Heartbeat sensor connected to A1
#define GAS_SENSOR A0       // Gas sensor connected to A0
#define BUZZER_PIN 3        // Buzzer connected to pin 3
#define IR_SENSOR 2         // IR sensor connected to pin 2
#define RELAY 4             // Relay module connected to pin 4

// *Threshold Values*
#define HEARTBEAT_THRESHOLD 550 // Adjust based on sensor readings
#define GAS_THRESHOLD 200       // Gas detection threshold

// *LCD Setup*
LiquidCrystal_I2C lcd(0x27, 16, 2);

// *Heartbeat Detection Variables*
int bpm = 0;
unsigned long lastBeatTime = 0;
int beatCount = 0;
const int sampleTime = 10000; // 10 seconds for BPM calculation
const int timeout = 5000;     // 5 seconds timeout to reset BPM
unsigned long lastUpdateTime = 0;
bool pulseDetected = false;

void setup() {
    // *Initialize Components*
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(GAS_SENSOR, INPUT);
    pinMode(IR_SENSOR, INPUT);
    pinMode(RELAY, OUTPUT);
    pinMode(HEARTBEAT_SENSOR, INPUT);

    digitalWrite(RELAY, HIGH);    // Ensure relay starts OFF
    digitalWrite(BUZZER_PIN, LOW); // Buzzer OFF initially

    // *Initialize LCD*
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Heart Rate: ");
}

void loop() {
    unsigned long currentTime = millis();

    // **Gas Sensor Reading & Buzzer Control**
    int gasLevel = 0;
    for (int i = 0; i < 10; i++) { // Take 10 readings and average them
        gasLevel += analogRead(GAS_SENSOR);
        delay(5);
    }
    gasLevel /= 10;

    if (gasLevel > GAS_THRESHOLD) {
        digitalWrite(BUZZER_PIN, HIGH); // Alert if gas detected
    } else {
        digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer stays off
    }

    // **IR Sensor & Relay Control**
    int sensorState = digitalRead(IR_SENSOR);
    digitalWrite(RELAY, sensorState == LOW ? LOW : HIGH); // Object detected -> Turn ON relay

    // **Heartbeat Sensor Reading**
    int sensorValue = analogRead(HEARTBEAT_SENSOR);

    if (sensorValue > HEARTBEAT_THRESHOLD && !pulseDetected) {
        if (currentTime - lastBeatTime > 300) { // 300ms debounce to avoid false counts
            beatCount++;
            lastBeatTime = currentTime;
            pulseDetected = true;
        }
    }
    if (sensorValue < HEARTBEAT_THRESHOLD - 20) {
        pulseDetected = false;
    }

    // **Reset BPM if No Beat Detected for Timeout Period**
    if (currentTime - lastBeatTime > timeout) {
        bpm = 0;
        beatCount = 0;
    }

    // **Update BPM Every 10 Seconds**
    if (currentTime - lastUpdateTime >= sampleTime) {
        bpm = beatCount * 6; // Convert beats to BPM
        lastUpdateTime = currentTime;
        beatCount = 0;

        // **Display BPM on LCD**
        lcd.setCursor(0, 1);
        lcd.print("BPM: ");
        lcd.print(bpm);
        lcd.print("    "); // Clear any previous extra characters
    }

    delay(100); // Small delay for stability
}