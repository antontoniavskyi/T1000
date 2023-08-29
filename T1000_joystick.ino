//cd Nextcloud/PC_files/Development/Arduino/T1000/T1000_joystick

#include <ezButton.h>
#include "joystickBLE.h"

#define JS1_VRX_PIN 36
#define JS1_VRY_PIN 37
#define JS1_SW_PIN 12

#define JS2_VRX_PIN 38
#define JS2_VRX_PIN 39
#define JS2_SW_PIN 13

struct joystickStruct {
  int VRX_PIN;
  int VRY_PIN;

  int valueX; // to store the X-axis value
  int valueY; // to store the Y-axis value
  int bValue; // To store value of the button

  int valueXhigh;
  int valueXlow;
  int valueYhigh;
  int valueYlow;
};

struct joystickStruct joystick[2];

ezButton button_0(JS1_SW_PIN);
ezButton button_1(JS2_SW_PIN);

uint32_t timer;



void setup() {
  Serial.begin(115200);
  initStructure();

  pinMode(JS1_VRX_PIN, INPUT);
  pinMode(JS1_VRY_PIN, INPUT);
  pinMode(JS1_SW_PIN, INPUT_PULLUP);

  pinMode(JS2_VRX_PIN, INPUT);
  pinMode(JS2_VRX_PIN, INPUT);
  pinMode(JS2_SW_PIN, INPUT_PULLUP);

  button_0.setDebounceTime(50); // set debounce time to 50 milliseconds
  button_1.setDebounceTime(50); // set debounce time to 50 milliseconds

  joystickBLEinit();


  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  button_0.loop(); // MUST call the loop() function first
  button_1.loop();



  if (button_0.isPressed()) {
    Serial.println("The button is pressed");
    // TODO do something here
  }

  if (button_0.isReleased()) {
    Serial.println("The button is released");
    // TODO do something here
  }

  // print data to Serial Monitor on Arduino IDE
  if (deviceConnected) {
    getJoystickAndButtonState();
    if (millis() - timer > 500) {
      Serial.println("**********************************************");
      timer = millis();
      for (byte i = 0; i < 2; i++) {
        if (i == 0) {
          if (joystick[i].valueX > 1900) {
            Serial.printf("JOYSTICK %i: x = ", i);
            Serial.print(joystick[i].valueXhigh);
            joystickXCharacteristic_1.setValue(joystick[i].valueXhigh);
          } else {
            Serial.printf("JOYSTICK %i: x = ", i);
            Serial.print(joystick[i].valueXlow);
            joystickXCharacteristic_1.setValue(joystick[i].valueXlow);
          }
          if (joystick[i].valueY > 1900) {
            Serial.print(", y = ");
            Serial.print(joystick[i].valueYhigh);
            joystickYCharacteristic_1.setValue(joystick[i].valueYhigh);
          } else {
            Serial.print(", y = ");
            Serial.print(joystick[i].valueYlow);
            joystickYCharacteristic_1.setValue(joystick[i].valueYlow);
          }
          Serial.print(" : button = ");
          Serial.println(joystick[i].bValue);
          joystickBTCharacteristic_1.setValue(joystick[i].bValue);
        } else {
          if (joystick[i].valueX > 1900) {
            Serial.printf("JOYSTICK %i: x = ", i);
            Serial.print(joystick[i].valueXhigh);
            joystickXCharacteristic_2.setValue(joystick[i].valueXhigh);
          } else {
            Serial.printf("JOYSTICK %i: x = ", i);
            Serial.print(joystick[i].valueXlow);
            joystickXCharacteristic_2.setValue(joystick[i].valueXlow);
          }
          if (joystick[i].valueY > 1900) {
            Serial.print(", y = ");
            Serial.print(joystick[i].valueYhigh);
            joystickYCharacteristic_2.setValue(joystick[i].valueYhigh);
          } else {
            Serial.print(", y = ");
            Serial.print(joystick[i].valueYlow);
            joystickYCharacteristic_2.setValue(joystick[i].valueYlow);
          }
          Serial.print(" : button = ");
          Serial.println(joystick[i].bValue);
          joystickBTCharacteristic_2.setValue(joystick[i].bValue);
        }


      }
      joystickXCharacteristic_1.notify();
      joystickYCharacteristic_1.notify();
      joystickBTCharacteristic_1.notify();

      joystickXCharacteristic_2.notify();
      joystickYCharacteristic_2.notify();
      joystickBTCharacteristic_2.notify();
      Serial.println(" ");
    }
  }


}

void initStructure() {
  joystick[0].VRX_PIN = 36;
  joystick[0].VRY_PIN = 37;

  joystick[1].VRX_PIN = 38;
  joystick[1].VRY_PIN = 39;

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

    /*Serial.println("DEBUG");
      Serial.println(joystick[i].valueX);
      Serial.println(joystick[i].valueY);*/

    joystick[i].valueXhigh = map(joystick[i].valueX, 1900, 4095, 1, 9);
    joystick[i].valueXlow = map(joystick[i].valueX, 0, 1900, -9, 0);
    joystick[i].valueYhigh = map(joystick[i].valueY, 1900, 4095, 1, 9);
    joystick[i].valueYlow = map(joystick[i].valueY, 0, 1900, -9, 0);

    // Read the button value
    if (i == 0) {
      joystick[i].bValue = button_0.getState();
    } else {
      joystick[i].bValue = button_1.getState();
    }

  }
}
