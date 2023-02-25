#include <Wire.h>
#include <Servo.h>

Servo pitch_servo;
Servo roll_servo;
int16_t prev_roll = 0;
int16_t prev_pitch = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pitch_servo.attach(9);
  roll_servo.attach(10);
  
  Wire.beginTransmission(0x28);
  Wire.write(0x3D);
  Wire.write(0x0C);
  Wire.endTransmission();
}


void loop() {
  Wire.beginTransmission(0x28);
  Wire.write(0x1A);
  Wire.endTransmission();
  
  Wire.requestFrom(0x28, 6);
  while (Wire.available() < 6) {}
  
  int16_t heading = Wire.read() | (Wire.read() << 8);
  int16_t roll = Wire.read() | (Wire.read() << 8);
  int16_t pitch = Wire.read() | (Wire.read() << 8);
  
  float heading_degrees = heading / 16.0;
  float roll_degrees = roll / 16.0;
  float pitch_degrees = pitch / 16.0;
  
  int pitch_angle = constrain((int) floor(pitch_degrees) * -1, -30, 30);
  int roll_angle = constrain((int) floor(roll_degrees), -30, 30);

  if (abs(roll - prev_roll) > 16 || abs(pitch - prev_pitch) > 16) {
    prev_roll = roll;
    prev_pitch = pitch;
    pitch_servo.write(pitch_angle + 90);
    roll_servo.write(roll_angle + 90);
    Serial.print("Heading: ");
    Serial.print(heading_degrees);
    Serial.print(" degrees, Roll: ");
    Serial.print(roll_degrees);
    Serial.print(" degrees, Pitch: ");
    Serial.print(pitch_degrees);
    Serial.println(" degrees");
  }

  delay(50);
}
