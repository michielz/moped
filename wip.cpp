#include <Arduino.h>

#define HALL_SENSOR PB9
#define LIMIT_TRIGGER PA4  // Triggers LOW relay 2ph63083a, flipped High/Low logic

// Variables
volatile unsigned long pulseCount = 0; // Counts the number of pulses
const int rpmThreshold = 500; // Example threshold for RPM
const int pulsesPerRevolution = 1; // Number of pulses per revolution (adjust as necessary)
unsigned long previousMillis = 0;
const unsigned long interval = 1000; // 1 second interval for RPM calculation
const unsigned long limitOnDuration = 5000; // limit on duration in milliseconds (5 seconds)
unsigned long limitOnStartTime = 0;

// Function declarations
void countPulse();
void controlLedLimit(unsigned long rpm);

void setup() {
  pinMode(HALL_SENSOR, INPUT);
  pinMode(LIMIT_TRIGGER, OUTPUT);
  // Ensure the LEDs start off
  digitalWrite(LIMIT_TRIGGER, HIGH);
  attachInterrupt(digitalPinToInterrupt(HALL_SENSOR), countPulse, RISING);
}

void loop() {
  unsigned long currentMillis = millis();

  // Calculate RPM every second
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Calculate RPM using integer math
    unsigned long pulses = pulseCount;
    pulseCount = 0; // Reset pulse count after reading
    unsigned long rpm = (pulses * 60UL) / pulsesPerRevolution;

    // Control LED based on RPM
    controlLedLimit(rpm);
  }
}

void countPulse() {
  pulseCount++;
}

void controlLedLimit(unsigned long rpm) {
  unsigned long currentMillis = millis();

  if (rpm >= rpmThreshold) {
    digitalWrite(LIMIT_TRIGGER, LOW);
    limitOnStartTime = currentMillis;
  } else if (currentMillis - limitOnStartTime >= limitOnDuration) {
    digitalWrite(LIMIT_TRIGGER, HIGH);
  }
}
