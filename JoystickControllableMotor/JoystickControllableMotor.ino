/**
 * Arduino MEGA2560 Controlled DC Motor
 * By: Jimmaphy
 *
 * Using a joystick controller to manage the speed of a motor.
 * The information is displayed on an LCD display.
 */

// Library for controlling the LCD display
// Initialize the library with the correct pins
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Pins for the joystick
#define SWPIN 12
#define XPIN 0
#define YPIN 1

// Pins for motor
#define MOTORPIN 8
#define DIRECTIONAPIN 9
#define DIRECTIONBPIN 10

// Speed of the motor
// Requires a value between 0 and 255 to work
// Negative numbers are used to put the motor in reverse
int speed = 128;


/* Setup functions, runs once at the start */
void setup() {
  // Initialize the display with the number of columns and rows
  lcd.begin(16, 2);

  // Setup the joystick
  pinMode(SWPIN, INPUT);
  digitalWrite(SWPIN, HIGH);

  // Setup the motor and make it spin
  pinMode(MOTORPIN, OUTPUT);
  pinMode(DIRECTIONAPIN, OUTPUT);
  pinMode(DIRECTIONBPIN, OUTPUT);
  analogWrite(MOTORPIN, speed); // enable on
  digitalWrite(DIRECTIONAPIN,HIGH); //one way
  digitalWrite(DIRECTIONBPIN,LOW);
}


/* loop functions, runs continuously */
void loop() {
  // Handle the updates to the speed of the motor
  resetClick();
  updateSpeed();
  limitChecks();

  // Print the status of the program
  printStatus();

  // Make the motor spin at the desired spin
  analogWrite(MOTORPIN, abs(speed));

  // Wait for half a second
  delay(500);
}


/* Reset the speed to 0 when the joystick is clicked */
void resetClick() {
  if (digitalRead(SWPIN) == 0) {
    speed = 0;
  }
}


/** 
 * Update the speed based on the position of the controller
 * 516 is retracted from the x-position since that's the center position
 * Devided by 25 to make the steps more gradual for the range of -255 to 255
 */
void updateSpeed() {
  int stickPositionX = analogRead(XPIN);
  int speedChange = (stickPositionX - 516) / 25;
  speed += speedChange;
}


/* Change direction of the motor and keep the speed value within the bounds -255 and 255 */
void limitChecks() {
  if (speed < 0) {
    digitalWrite(DIRECTIONAPIN,HIGH); //one way
    digitalWrite(DIRECTIONBPIN,LOW);
  } else {
    digitalWrite(DIRECTIONAPIN,LOW); //one way
    digitalWrite(DIRECTIONBPIN,HIGH);
  }

  if (speed < -255) {
    speed = -255;
  }

  if (speed > 255) {
    speed = 255;
  }
}


/* Print information of the program to the screen: current speed and joystick position */
void printStatus() {
  // Clear the screen
  lcd.clear();

  // Print the click
  lcd.print("Speed: ");
  lcd.print(speed);

  // Print the location of the joystick
  lcd.setCursor(0, 1);
  lcd.print("X: ");
  lcd.print(analogRead(XPIN));
  lcd.print(" Y: ");
  lcd.print(analogRead(YPIN));
}
