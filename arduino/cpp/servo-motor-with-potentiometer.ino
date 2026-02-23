/*
 * Servo Motor Control with Potentiometer
 * 
 * Description:
 * This program controls a servo motor's position using a potentiometer.
 * The potentiometer acts as an analog input, and its position is mapped
 * to the servo's angle (0-180 degrees). As you turn the potentiometer,
 * the servo motor rotates accordingly in real-time.
 * 
 * HARDWARE:
 * - Arduino board (Uno, Nano, etc.)
 * - Servo motor (e.g., SG90)
 * - Potentiometer (10K ohm recommended)
 * 
 * CONNECTIONS:
 * - Servo signal wire -> Arduino pin 9
 * - Servo power (red) -> 5V
 * - Servo ground (brown/black) -> GND
 * - Potentiometer middle pin -> Arduino pin A0
 * - Potentiometer outer pins -> 5V and GND
 * 
 * Author: Zaeem Akhtar
 * Date: 25-01-2026
 */

#include <Servo.h>  // Include the Servo library for controlling servo motors

// Create a Servo object to control the servo motor
Servo Servo1;

// Pin definitions
int servoPin = 9;    // Digital pin connected to servo signal wire
int potPin = A0;     // Analog pin connected to potentiometer middle pin

void setup() {
  Servo1.attach(servoPin);  // Attach the servo to pin 9
}

void loop() {
  // Read the analog value from the potentiometer (0-1023)
  // This means the ADC can represent values using 10 binary digits (bits)
  // 10 bits = 2^10 = 1024 different values
  int reading = analogRead(potPin);
  
  // Map the potentiometer reading (0-1023) to servo angle (0-180 degrees)
  // analogRead returns values from 0 to 1023
  // Servo angles range from 0 to 180 degrees
  int angle = map(reading, 0, 1023, 0, 180);
  
  // Command the servo to move to the calculated angle
  Servo1.write(angle);
  
  // Optional: Add a small delay for stability (uncomment if needed)
  // delay(15);
}