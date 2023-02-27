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
  
  Wire.beginTransmission(0x28); // sensor I2C addr on startup
  Wire.write(0x3D); // select angle register
  Wire.write(0x0C); // NDOF
  Wire.endTransmission(); // end setup
}


void loop() {
  Wire.beginTransmission(0x28); // start transmission loop
  Wire.write(0x1A); // select mode
  Wire.endTransmission(); // end loop start
  
  Wire.requestFrom(0x28, 6); // call for 6 bytes from 0x03 reg
  while (Wire.available() < 6) {} // wait for 6 bytes
  
  int16_t heading = Wire.read() | (Wire.read() << 8); // read heading into 16 bit var
  int16_t roll = Wire.read() | (Wire.read() << 8); // read roll into 16 bit var
  int16_t pitch = Wire.read() | (Wire.read() << 8); // read pitch into 16 bit var
  
  float heading_degrees = heading / 16.0; // get normal data in deg
  float roll_degrees = roll / 16.0; // get normal data in deg
  float pitch_degrees = pitch / 16.0; // get normal data in deg
  
  int pitch_angle = constrain((int) floor(pitch_degrees) * -1, -30, 30); // constrain to not damage servos
  int roll_angle = constrain((int) floor(roll_degrees), -30, 30); // constrain to not damage servos

  // check if different

  if (abs(roll - prev_roll) > 16 || abs(pitch - prev_pitch) > 16) {
    prev_roll = roll; // update last measurement
    prev_pitch = pitch; // update last measurement
    pitch_servo.write(pitch_angley + 90); // servos adapt
    roll_servo.write(roll_angle + 90); // servos adapt
    // user readability
    Serial.print("Heading: ");
    Serial.print(heading_degrees);
    Serial.print(" degrees, Roll: ");
    Serial.print(roll_degrees);
    Serial.print(" degrees, Pitch: ");
    Serial.print(pitch_degrees);
    Serial.println(" degrees");
  }

  delay(50); // dont over stress the I2C bus
}
