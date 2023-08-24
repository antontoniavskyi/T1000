#include <ezButton.h>

struct joystickStruct {
  int VRX_PIN;
  int VRY_PIN;
  int SW_PIN;

  int valueX; // to store the X-axis value
  int valueY; // to store the Y-axis value
  int bValue; // To store value of the button

  int valueXhigh;
  int valueXlow;
  int valueYhigh;
  int valueYlow;
};

struct joystickStruct joystick[2];

joystick[0].VRX_PIN = 36;
joystick[0].VRY_PIN = 37;
joystick[0].SW_PIN = 12;

joystick[1].VRX_PIN = 38;
joystick[1].VRY_PIN = 39;
joystick[1].SW_PIN = 13;

ezButton button1(joystick[0].SW_PIN);
ezButton button2(joystick[1].SW_PIN);


uint32_t timer;

void setup() {
  Serial.begin(115200);
  initStructure();

  pinMode(joystick[0].VRX_PIN, INPUT);
  pinMode(joystick[0].VRY_PIN, INPUT);
  pinMode(joystick[0].SW_PIN, INPUT_PULLUP);

  pinMode(joystick[1].VRX_PIN, INPUT);
  pinMode(joystick[1].VRY_PIN, INPUT);
  pinMode(joystick[1].SW_PIN, INPUT_PULLUP);

  button1.setDebounceTime(50); // set debounce time to 50 milliseconds
  button2.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  button1.loop(); // MUST call the loop() function first
  button2.loop();

  getJoystickAndButtonState();

  if (button.isPressed()) {
    Serial.println("The button is pressed");
    // TODO do something here
  }

  if (button.isReleased()) {
    Serial.println("The button is released");
    // TODO do something here
  }

  // print data to Serial Monitor on Arduino IDE

  if (millis() - timer > 300) {
    timer = millis();
    for (byte i = 0; i < 2; i++) {
      if (joystick[i].valueX > 1900) {
        Serial.printf("JOYSTICK %i: x = ", i);
        Serial.print(joystick[i].valueXhigh);
      } else {
        Serial.print("JOYSTICK %i: x = ", i);
        Serial.print(joystick[i].valueXlow);
      }
      if (joystick[i].valueY > 1900) {
        Serial.print(", y = ");
        Serial.print(joystick[i].valueYhigh);
      } else {
        Serial.print(", y = ");
        Serial.print(joystick[i].valueYlow);
      }
      Serial.print(" : button = ");
      Serial.println(joystick[i].bValue);
    }
  }
}

void initStructure() {


  for (byte i = 0; i < 2; i++) {
    joystick[i].valueX = 0; // to store the X-axis value
    joystick[i].valueY = 0; // to store the Y-axis value
    joystick[i].bValue = 0; // To store value of the button

    joystick[i].valueXhigh = 0;
    joystick[i].valueXlow = 0;
    joystick[i].valueYhigh = 0;
    joystick[i].valueYlow = 0;
  }
}

void getJoystickAndButtonState() {
  for (byte i = 0; i < 2; i++) {
    joystick[i].valueX = analogRead(joystick[i].VRX_PIN);
    joystick[i].valueY = analogRead(joystick[i].VRY_PIN);

    joystick[i].valueXhigh = map(joystick[i].valueX, 1900, 4095, 1, 9);
    joystick[i].valueXlow = map(joystick[i].valueX, 0, 1900, -9, 0);
    joystick[i].valueYhigh = map(joystick[i].valueY, 1900, 4095, 1, 9);
    joystick[i].valueYlow = map(joystick[i].valueY, 0, 1900, -9, 0);

    // Read the button value
    joystick[i].bValue = button.getState();
  }
}
