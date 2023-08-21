#include <ezButton.h>

#define VRX_PIN  36 // ESP32 pin GPIO36 (ADC0) connected to VRX pin
#define VRY_PIN  39 // ESP32 pin GPIO39 (ADC0) connected to VRY pin
#define SW_PIN   17 // ESP32 pin GPIO17 connected to SW  pin

ezButton button(SW_PIN);

int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value
int bValue = 0; // To store value of the button

int valueXhigh = 0;
int valueXlow = 0;
int valueYhigh = 0;
int valueYlow = 0;

uint32_t timer;

void setup() {
  Serial.begin(115200) ;
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  button.loop(); // MUST call the loop() function first

  // read X and Y analog values
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  valueXhigh = map(valueX, 1900, 4095, 1, 9);
    valueXlow = map(valueX, 0, 1900, -9, 0);
    valueYhigh = map(valueY, 1900, 4095, 1, 9);
    valueYlow = map(valueY, 0, 1900, -9, 0);

  // Read the button value
  bValue = button.getState();

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
     if (valueX > 1900) {
       Serial.print("x = ");
       Serial.print(valueXhigh);
     } else {
       Serial.print("x = ");
       Serial.print(valueXlow);
     }
     if (valueY > 1900) {
       Serial.print(", y = ");
       Serial.print(valueYhigh);
     } else {
       Serial.print(", y = ");
       Serial.print(valueYlow);
     }
     Serial.print(" : button = ");
     Serial.println(bValue);
    }

  /*if (millis() - timer > 300) {
    timer = millis();
    Serial.print("x = ");
    Serial.print(valueX);
    Serial.print(", y = ");
    Serial.print(valueY);
    Serial.print(" : button = ");
    Serial.println(bValue);
  }*/


}
