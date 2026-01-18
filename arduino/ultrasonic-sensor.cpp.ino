/*
 * ULTRASONIC DISTANCE MEASUREMENT EXPERIMENT
 * Hardware: HC-SR04 Ultrasonic Sensor + Arduino
 * 
 * HOW IT WORKS:
 * 1. Arduino sends a 10 microsecond pulse to the sensor's TRIG pin
 * 2. Sensor emits 8 ultrasonic sound bursts at 40kHz
 * 3. Sound waves travel through air, bounce off an object, and return
 * 4. Sensor detects the echo and sends a HIGH pulse on ECHO pin
 * 5. The duration of the ECHO pulse = round-trip travel time of sound
 * 6. Arduino calculates distance using: distance = (time × speed_of_sound) / 2
 * 
 * CONNECTIONS:
 * - Sensor VCC  → Arduino 5V
 * - Sensor TRIG → Arduino Pin 12 (gpioTrig)
 * - Sensor ECHO → Arduino Pin 13 (gpioEcho)
 * - Sensor GND  → Arduino GND
 * 
 * MEASUREMENT RANGE: 2cm to 400cm
 * ACCURACY: ±3mm
 */

// Pin definitions
const int gpioTrig = 12;  // Arduino OUTPUT pin - sends trigger signal TO sensor
const int gpioEcho = 13;  // Arduino INPUT pin - receives echo signal FROM sensor
const int gpioLed = 8;     // Arduino OUTPUT pin - controls LED (connect LED + resistor to this pin)

// Global variables to store measurement results
long duration;     // Time in microseconds that echo pulse stays HIGH
float distanceM;   // Calculated distance in meters

// Threshold distance for LED control
const float thresholdDistance = 0.5;  // Turn off LED when object closer than 0.5 meter

void setup() {
  // Configure gpioTrig as OUTPUT so Arduino can send signals to sensor
  pinMode(gpioTrig, OUTPUT);
  
  // Configure gpioEcho as INPUT so Arduino can receive signals from sensor
  pinMode(gpioEcho, INPUT);
  
  // Configure LED pin as OUTPUT so Arduino can control the LED
  pinMode(gpioLed, OUTPUT);
  
  // Initialize LED to ON state (object far away)
  digitalWrite(gpioLed, HIGH);
  
  // Initialize serial communication at 9600 baud rate
  // (baud = number of signal changes per second on the communication line)
  // This allows us to send distance data to the computer via USB
  Serial.begin(9600);
}

void loop() {
  // ========== STEP 0: RESET THE SENSOR ==========
  // Arduino sets trigger pin LOW to ensure clean signal
  // Sensor is now in idle state, ready to receive trigger
  digitalWrite(gpioTrig, LOW);
  delayMicroseconds(2);  // Short stabilization period
  
  // ========== STEP 1: ARDUINO TRIGGERS THE SENSOR ==========
  // Arduino sends a HIGH pulse on trigger pin
  // This tells the sensor: "start measuring now!"
  digitalWrite(gpioTrig, HIGH);
  
  // ========== STEP 2: ARDUINO MAINTAINS HIGH SIGNAL ==========
  // Keep trigger HIGH for 10 microseconds (sensor requires at least 10μs)
  // During this time, sensor prepares to emit ultrasonic burst
  delayMicroseconds(10);
  
  // ========== STEP 3: ARDUINO COMPLETES TRIGGER SIGNAL ==========
  // Set trigger back to LOW - this falling edge tells sensor to transmit
  // Sensor now emits 8 ultrasonic pulses at 40kHz frequency
  digitalWrite(gpioTrig, LOW);
  
  // ========== STEP 4: SENSOR EMITS SOUND & ARDUINO WAITS FOR ECHO ==========
  // What the SENSOR does:
  //   - Emits ultrasonic sound waves toward object
  //   - Waits for echo to bounce back
  //   - When echo detected, sets ECHO pin HIGH
  //   - Keeps ECHO pin HIGH for duration proportional to distance
  //   - Sets ECHO pin back to LOW when measurement complete
  //
  // What ARDUINO does (via pulseIn function):
  //   - Waits for gpioEcho to go HIGH (echo starts)
  //   - Starts internal timer
  //   - Waits for gpioEcho to go LOW (echo ends)
  //   - Stops timer and returns elapsed time in microseconds
  duration = pulseIn(gpioEcho, HIGH);
  
  // ========== STEP 5: ARDUINO CALCULATES DISTANCE ==========
  // Speed of sound in air at 20°C = 343 m/s = 0.000343 m/μs
  // Formula: distance = (time × speed) / 2
  // - Multiply duration by speed of sound to get total travel distance
  // - Divide by 2 because sound travels TO object and BACK (round trip)
  // Using 0.0003435 for slightly better accuracy (343.5 m/s)
  distanceM = duration * 0.0003435 / 2;
  
  // ========== STEP 6: ARDUINO OUTPUTS RESULT ==========
  // Send measurement to Serial Monitor so we can view it on computer
  Serial.print("Distance: ");
  Serial.print(distanceM);
  Serial.println(" meters");
  
  // ========== STEP 7: CONTROL LED BASED ON DISTANCE ==========
  // If object is closer than threshold (1 meter), turn LED ON
  // Otherwise, keep LED OFF
  if (distanceM > thresholdDistance) {
    digitalWrite(gpioLed, LOW);   // Turn LED OFF (object too far!)
  } else {
    digitalWrite(gpioLed, HIGH);  // Turn LED ON (object too near!)
    Serial.println("LED: ON (Object detected within 1 meter)");
  }
  
  // Wait 100 milliseconds before next measurement
  // This prevents overwhelming the serial output and gives sensor time to reset
  delay(100);
}