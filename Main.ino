#include <LedControl.h>
#include <Stepper.h>

int DIN = 12;
int CS = 11;
int CLK = 10;
LedControl lc(DIN, CLK, CS, 1); // 8x8 LED Matrix Object

// LED Matrix Symbols for floors
byte floor0[8] = {0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C};
byte floor1[8] = {0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38}; // Centered "1" from top to bottom
byte upArrow[8] = {0x08,0x1C,0x2A,0x08,0x08,0x08,0x08,0x08};  // Displays an up arrow
byte downArrow[8] = {0x08,0x08,0x08,0x08,0x08,0x2A,0x1C,0x08}; // Displays a down arrow

// Button and Buzzer Setup
int buttonUp = 2;      // Up button on pin 2
int buttonDown = 3;    // Down button on pin 3
int buzzerPin = 4;     // Buzzer on pin 4
int currentFloor = 0;  // Start at floor 0

// Stepper Setup
const int stepsPerRevolution = 2038;
Stepper myStepper(stepsPerRevolution, 5, 7, 6, 8); // Stepper motor pins

void setup() {
  // Initialize LED matrix
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);

  // Initialize buttons and buzzer
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  // Display initial floor
  drawImage(floor0);

  // Set initial stepper speed (RPM)
  myStepper.setSpeed(5);
}

void loop() {
  // Check for up button press
  if (digitalRead(buttonUp) == LOW && currentFloor == 0) {
    beep(100);              // Beep for 0.1 seconds
    moveLiftToFloor(1);
  }
  
  // Check for down button press
  if (digitalRead(buttonDown) == LOW && currentFloor == 1) {
    beep(100);              // Beep for 0.1 seconds
    moveLiftToFloor(0);
  }
}

void moveLiftToFloor(int floor) {
  int stepsToMove = (floor == 1) ? stepsPerRevolution : -stepsPerRevolution;

  // Display direction arrow
  drawImage(floor == 1 ? upArrow : downArrow);

  // Move the stepper motor in steps over 1 second
  myStepper.setSpeed(15); // Set speed to 5 RPM for smooth movement
  myStepper.step(stepsToMove);
  
  // Display floor number after movement
  drawImage(floor == 1 ? floor1 : floor0);
  currentFloor = floor;                    // Update current floor
  delay(500);                              // Delay to avoid quick repeated button presses
}

void drawImage(byte image[]) {
  for (int i = 0; i <= 7; i++) {
    lc.setRow(0, i, image[i]);
  }
}

void beep(int duration) {
  tone(buzzerPin, 1000); // Beep at 1000 Hz
  delay(duration);
  noTone(buzzerPin);
}
