//this file is named as trial2.ino on my pc, i renamed it to upload it on github

#include <Servo.h>

// Define pin constants
#define echopin 11
#define trigpin 13
#define buzzerPin 8
#define signalPin 2

// Servo pre code
Servo servo1;
Servo servo2;

// Water motor pre code
const int motorPin1 = 13;  // Motor control pins
const int motorPin2 = 9;

unsigned long servoStartTime = 0;
unsigned long servo1EndTime = 0;
unsigned long servoRunTime = 3000; //  servo for 3 seconds

void setup() {
  Serial.begin(9600);
  // Initialize pin modes
  pinMode(echopin, INPUT);
  pinMode(trigpin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(signalPin, INPUT);
  
  // Servo setup code
  servo1.attach(10);
  servo2.attach(12);
  
  // Initialize servo position
  servo1.write(0);
  servo2.write(0);

  // Water motor setup code
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
}

void loop() {
  // Ultrasonic loop code
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  long duration = pulseIn(echopin, HIGH);

  // distance in centimeters
  int distance = duration * 0.034 / 2;

  // Print distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(2000);

  // Buzzer loop code
  if (distance > 10 && distance <= 100) { // If ultrasonic sensor detects someone
    Serial.println("HUMAN DETECTED 10-100cm");
    if (digitalRead(signalPin) == LOW) { // If proximity sensor detects dish
      Serial.println("");
      Serial.println("PLATE IN NOW BUZZ");
      int kbuzzer = 2;
      unsigned char i, j;

      while (kbuzzer > 0) {
        for (i = 0; i < 100; i++) {
          digitalWrite(buzzerPin, HIGH); // Send beep
          delay(1);
          digitalWrite(buzzerPin, LOW);  // No beep
          delay(1);
        }

        kbuzzer = kbuzzer - 1;
      }

      // Activate the water motor for 5 seconds
      digitalWrite(motorPin2, LOW);
      delay(1000);
      digitalWrite(motorPin1, HIGH);
      delay(5000);
      
      // Deactivate the water motor
      digitalWrite(motorPin1, LOW);

      // Start servo motor 1
      servoStartTime = millis();
      while (millis() - servoStartTime < servoRunTime) {
        servo1.write(360); // Set servo1 to 180 degrees.
        delay(1000);
        servo1.write(0); // Set servo1 to 180 degrees
        delay(1000);
        servo1.write(360); // Set servo1 to 180 degrees

      }
      servo1.write(0); // Set servo1 to 0 degrees

      // Delay before starting servo motor 2 
      delay(500);
      
      // Start servo motor 2
      servo2.write(360); // Set servo2 to 180 degrees
      delay(1000); // Run servo2 for 2 seconds 
      servo2.write(0); // Set servo2 to 0 degrees
      delay(1000);
      servo2.write(360);
      delay(1000);
      servo2.write(0);
      delay(1000);
      
      // Beep the buzzer again after servo2 is done
      for (i = 0; i < 100; i++) {
        digitalWrite(buzzerPin, HIGH); // Send tone
        delay(1);
        digitalWrite(buzzerPin, LOW);  // No tone
        delay(1);
      }
    } else {
      Serial.println("PLATE ?"); // Prox sensor detects no dish
      Serial.println("");
    }
  } else {
    Serial.println("NO Human detected 10-100cm"); // Ultrasonic sensor detects nothing
    Serial.println("");
    }
}
