#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

int xAxis = A0;  // Joystick X axis
int yAxis = A1;  // Joystick Y axis
int joyButton = 2;  // Joystick button pin

int servo1Pos = 90;  // Starting at middle position
int servo2Pos = 90;  // Starting at middle position
int servo3Pos = 180; // Starting at open position

bool joyButtonPrevState = HIGH;
bool servo3MovingToClose = false; // Track direction of servo 3 movement
bool servo3Moving = false; // Track if servo 3 is moving

const int deadzone = 50;  // Deadzone for joystick center position
const int joystickCenter = 512;  // Center value for analog read
const int maxSpeed = 3;  // Maximum speed of servo movement per loop
const int servo3MoveSpeed = 5; // Speed for servo 3 movement

void setup() {
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);
  
  pinMode(joyButton, INPUT_PULLUP);

  // Initialize servo positions
  servo1.write(servo1Pos);
  servo2.write(servo2Pos);
  servo3.write(servo3Pos);
}

void loop() {
  // Control Servo 1 with X axis
  int xValue = analogRead(xAxis);
  if (abs(xValue - joystickCenter) > deadzone) {
    if (xValue > joystickCenter) {
      servo1Pos = min(180, servo1Pos + map(xValue - joystickCenter - deadzone, 0, 511 - deadzone, 1, maxSpeed));
    } else {
      servo1Pos = max(0, servo1Pos - map(joystickCenter - xValue - deadzone, 0, 512 - deadzone, 1, maxSpeed));
    }
    servo1.write(servo1Pos);
  }
  
  // Control Servo 2 with Y axis
  int yValue = analogRead(yAxis);
  if (abs(yValue - joystickCenter) > deadzone) {
    if (yValue > joystickCenter) {
      servo2Pos = min(180, servo2Pos + map(yValue - joystickCenter - deadzone, 0, 511 - deadzone, 1, maxSpeed));
    } else {
      servo2Pos = max(0, servo2Pos - map(joystickCenter - yValue - deadzone, 0, 512 - deadzone, 1, maxSpeed));
    }
    servo2.write(servo2Pos);
  }
  
  // Toggle Servo 3 between 0 and 180 degrees when joystick button is pressed
  bool joyButtonState = digitalRead(joyButton);
  if (joyButtonState == LOW && joyButtonPrevState == HIGH && !servo3Moving) {
    // Button just pressed
    servo3MovingToClose = (servo3Pos == 180);
    servo3Moving = true; // Start moving servo 3
  }

  // Move servo 3 gradually
  if (servo3Moving) {
    if (servo3MovingToClose) {
      servo3Pos -= servo3MoveSpeed;
      if (servo3Pos <= 0) {
        servo3Pos = 0;
        servo3Moving = false; // Stop movement
      }
    } else {
      servo3Pos += servo3MoveSpeed;
      if (servo3Pos >= 180) {
        servo3Pos = 180;
        servo3Moving = false; // Stop movement
      }
    }
    servo3.write(servo3Pos);
  }
  
  joyButtonPrevState = joyButtonState;
  
  delay(15); // Small delay for stability
}
