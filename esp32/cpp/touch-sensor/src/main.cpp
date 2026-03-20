/*
 * TTP223 Touch Sensor - LED Toggle
 * 
 * Description:
 * This program uses a TTP223 capacitive touch sensor to toggle an LED on and off.
 * When the touch pad is pressed, the LED switches state (ON to OFF or OFF to ON).
 * The program uses edge detection to ensure one touch = one toggle, preventing
 * multiple triggers from a single touch.
 * 
 * HOW IT WORKS:
 * - The TTP223 outputs HIGH (1) when touched and LOW (0) when not touched
 * - We track the previous touch state to detect the moment of touch (rising edge)
 * - Only when the state changes from LOW to HIGH do we toggle the LED
 * - A 50ms delay prevents false triggers from electrical noise (debouncing)
 * 
 * CIRCUIT DIAGRAM:
 * 
 *  ESP32 Dev V1
 *  ┌─────────────────┐
 *  │                 │
 *  │ 3.3V ───────────┼──── TTP223 VCC
 *  │ GND  ───────────┼──── TTP223 GND
 *  │ GPIO4 ──────────┼──── TTP223 IO (signal)
 *  │                 │
 *  │ GPIO2 ──────────┼──── LED(+)
 *  │ GND  ───────────┼──── LED(-)
 *  │                 │
 *  └─────────────────┘
 * 
 * HARDWARE:
 * - ESP32 Dev Module V1
 * - TTP223 capacitive touch sensor module
 * - 1x LED (any color)
 * - Jumper wires
 * 
 * 
 * NOTE:
 * The ESP32 GPIO pins have a small internal current limiting capability
 * which protects the LED without a resistor for basic use. However for
 * long term projects it is still recommended to use a 220ohm resistor
 * to protect both the LED and the GPIO pin.
 * 
 * WHY GPIO4 FOR TOUCH:
 * GPIO4 is a reliable digital input pin on ESP32 Dev V1.
 * It works well with the TTP223 which outputs a clean HIGH/LOW signal.
 * 
 * 
 * Author: Zaeem Akhtar
 * Date: 25-02-2026
 */

#include <Arduino.h>

// Pin definitions
#define TOUCH_PIN 4   // TTP223 IO pin connected to GPIO4
#define LED_PIN 2     // LED connected directly to GPIO2

// State tracking variables
bool ledState = false;       // Current LED state (false = OFF, true = ON)
bool lastTouchState = false; // Previous touch state for edge detection

void setup() {
  Serial.begin(115200);          // Start serial communication at 115200 baud
  pinMode(LED_PIN, OUTPUT);      // Set LED pin as output
  pinMode(TOUCH_PIN, INPUT);     // Set touch pin as input

  digitalWrite(LED_PIN, LOW);    // Make sure LED starts OFF
  Serial.println("TTP223 Touch Sensor Ready!");
  Serial.println("Touch the pad to toggle the LED...");
}

void loop() {
  // Read current touch state
  // HIGH = touched, LOW = not touched
  bool touched = digitalRead(TOUCH_PIN);

  // Edge detection - only act when touch goes from LOW to HIGH
  // This means only the moment you first touch the pad triggers the toggle
  if (touched && !lastTouchState) {
    ledState = !ledState;                    // Flip LED state
    digitalWrite(LED_PIN, ledState);         // Apply new state to LED

    // Print current state to serial monitor for debugging
    if (ledState) {
      Serial.println("Touched! -> LED ON");
    } else {
      Serial.println("Touched! -> LED OFF");
    }
  }

  // Save current touch state for comparison in next loop iteration
  lastTouchState = touched;

  // 50ms delay for debouncing
  // Prevents false triggers caused by electrical noise on the touch pad
  delay(50);
}