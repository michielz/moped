#include <Arduino.h>

// Pin definitions
const int hallSensorPin = PA0; // Pin connected to Hall Effect sensor
const int relayPin = PA1;      // Pin connected to relay

// Constants
const long wheelCircumferenceMM = 1750; // Wheel circumference in millimeters
int pulsesPerRevolution = 1;            // Number of magnets on the wheel
const int speedTriggerMMPerS = 1389;    // Speed to trigger the relay in mm/s (30 km/h = 30000 m/h = 8333 mm/s)
// 5 km/h = 1389 mm/s

// Variables
volatile int pulseCount = 0; // Counts the number of pulses
unsigned long previousMillis = 0;
const unsigned long interval = 1000; // Update speed every second

// Function declarations
void countPulse();
long calculateSpeed();
void setPulsesPerRevolution(int pulses);

void setup() {
    Serial.begin(9600);
    pinMode(hallSensorPin, INPUT);
    pinMode(relayPin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(hallSensorPin), countPulse, RISING);
}

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Calculate speed in mm/s
        long speedMMPerS = calculateSpeed();
        Serial.print("Speed: ");
        Serial.print(speedMMPerS * 3.6 / 1000); // Convert mm/s to km/h for display
        Serial.println(" km/h");

        // Check if speed exceeds trigger value
        if (speedMMPerS >= speedTriggerMMPerS) {
            // digitalWrite(relayPin, HIGH); // Turn on relay
            digitalWrite(LED_BUILTIN, HIGH);

        } else {
            // digitalWrite(relayPin, LOW);  // Turn off relay
            digitalWrite(LED_BUILTIN, LOW);
        }

        // Reset pulse count for next interval
        pulseCount = 0;

        // initialize LED digital pin as an output.
        pinMode(LED_BUILTIN, OUTPUT);
    }
}

// Interrupt service routine to count pulses
void countPulse() {
    pulseCount++;
}

// Function to calculate speed in mm/s
long calculateSpeed() {
    long distancePerPulseMM = wheelCircumferenceMM / pulsesPerRevolution; // Distance per pulse in mm
    long distanceTravelledMM = distancePerPulseMM * pulseCount;           // Total distance travelled in mm
    long speedMMPerS = distanceTravelledMM / (interval / 1000);           // Speed in mm/s
    return speedMMPerS;
}

// Function to set pulses per revolution
void setPulsesPerRevolution(int pulses) {
    pulsesPerRevolution = pulses;
}
