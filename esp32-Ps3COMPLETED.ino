#include <Ps3Controller.h>
#include <ESP32Servo.h>

// Rear motor
// OUT 1 & 2
// Positive out2 & Negative out1
int rearMotorPin1 = 2;
int rearMotorPin2 = 4;

// Front motor
// OUT 3 & 4
// Positive out3 & Negative out4
int frontMotorPin1 = 12;
int frontMotorPin2 = 13;

int servoPin = 5;  // Pin for the servo motor

#define MAX_MOTOR_SPEED 255

Servo myServo;

void notify() {
  // Motor control code here...
  int leftJoyY = Ps3.data.analog.stick.ly;  // Left stick - y axis - forward/backward car movement
  int rightJoyX = Ps3.data.analog.stick.rx;
  
  if (leftJoyY <= -50) {  // Move car Forward
    rotateMotor(MAX_MOTOR_SPEED, MAX_MOTOR_SPEED, rearMotorPin1, rearMotorPin2);
    Serial.println("Move car Forward");
  } else if (leftJoyY >= 50) {  // Move car Backward
    rotateMotor(-MAX_MOTOR_SPEED, -MAX_MOTOR_SPEED, rearMotorPin1, rearMotorPin2);
    Serial.println("Move car Backward");
  } else {  // Stop the car
    rotateMotor(0, 0, rearMotorPin1, rearMotorPin2);
    // rotateMotor(0, 0, frontMotorPin1, frontMotorPin2);
    Serial.println("Stop the car");
  }
  
  if (rightJoyX >= 50) {  // Move car Right
    rotateMotor(-MAX_MOTOR_SPEED, MAX_MOTOR_SPEED, frontMotorPin1, frontMotorPin2);
    Serial.println("Move car Right");
  } else if (rightJoyX <= -50) {  // Move car Left
    rotateMotor(MAX_MOTOR_SPEED, -MAX_MOTOR_SPEED, frontMotorPin1, frontMotorPin2);
    Serial.println("Move car Left");
  } else {  // Stop the car
    // rotateMotor(0, 0, rearMotorPin1, rearMotorPin2);
    rotateMotor(0, 0, frontMotorPin1, frontMotorPin2);
    Serial.println("Stop the car");
  }

  if (Ps3.data.button.r2) {  // Right trigger pressed (lock)
    rotateServo(90);  // Rotate servo 90 degrees clockwise
    Serial.println("Right trigger pressed (lock) - Rotate servo 90 degrees clockwise");
  } else if (Ps3.data.button.l2) {  // Left trigger pressed (unlock)
    rotateServo(-90);  // Rotate servo 90 degrees counter-clockwise
    Serial.println("Left trigger pressed (unlock) - Rotate servo 90 degrees counter-clockwise");
  }
}

void rotateMotor(int motorSpeed1, int motorSpeed2, int motorPin1, int motorPin2) {
  // Motor control code here...
  //car moves backward
  if (motorSpeed1 < 0) {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
  // car moves forward
  } else if (motorSpeed1 > 0) {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
  } else {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }

  if (motorSpeed2 < 0) {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
  } else if (motorSpeed2 > 0) {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
  } else {
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }
}

void rotateServo(int angle) {
  myServo.write(angle);
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisConnect() {
  // Motor control code here...
  rotateMotor(0, 0, rearMotorPin1, rearMotorPin2);
  rotateMotor(0, 0, frontMotorPin1, frontMotorPin2);
  rotateServo(0);  // Set the servo to its initial position
  Serial.println("Disconnected!");
}

void setUpPinModes() {
  // pinMode(rearEnablePin, OUTPUT);
  pinMode(rearMotorPin1, OUTPUT);
  pinMode(rearMotorPin2, OUTPUT);
  // pinMode(frontEnablePin, OUTPUT);
  pinMode(frontMotorPin1, OUTPUT);
  pinMode(frontMotorPin2, OUTPUT);
}

void setup() {
  setUpPinModes();
  Serial.begin(115200);
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.attachOnDisconnect(onDisConnect);
  Ps3.begin();
  Serial.println("Ready.");

  myServo.setPeriodHertz(50);  // Set the servo PWM frequency (typically 50Hz)
  myServo.attach(servoPin);  // Attach the servo to the specified pin
  rotateServo(0);  // Set the servo to its initial position
}

void loop() {
  // Leave empty for now
}

