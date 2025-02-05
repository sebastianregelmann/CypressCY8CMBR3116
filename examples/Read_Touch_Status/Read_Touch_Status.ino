#include "CypressCY8CMBR3116.h"
#include <Wire.h>

#define I2C_ADDRESS 0x51      //I2C Address of the Cypress IC (0x51 is default)
#define REQUEST_TIMEOUT 40  //After how many request is a Timeout triggered

//Create an instance of the IC
CY8CMBR3116 touchIC(I2C_ADDRESS, REQUEST_TIMEOUT);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Start Programm");
}

void loop() {
  requestTouchStatus();
  delay(500);
}

void requestTouchStatus() {
  Serial.println("Requesting touch status...");

  //Request the Status of the buttons
  uint8_t touchStatusBuffer[2];
  uint8_t error = touchIC.get_BUTTON_STAT(touchStatusBuffer);

  //Print the Status/Error
  if (error != 0) {
    printError(error);
  } else {
    printStatus(touchStatusBuffer);
  }
}

void printStatus(uint8_t *statusBuffer) {
  Serial.println("Status: ");
  //First 8 bit
  for (int i = 0; i < 8; i++) {
    uint8_t pressed = statusBuffer[0] >> i & 0b00000001;
    Serial.print(pressed);
    Serial.print(", ");
  }
  //Second 8 bit
  for (int i = 0; i < 7; i++) {
    uint8_t pressed = statusBuffer[1] >> i & 0b00000001;
    Serial.print(pressed);
    Serial.print(", ");
  }

  //last bit
  uint8_t pressed = statusBuffer[1] >> 7 & 0b00000001;
  Serial.println(pressed);
}

void printError(uint8_t errorCode) {
  Serial.println("Error in communication:");
  switch (errorCode) {
    case 1:
      Serial.println("Error 1: data too long to fit in transmit buffer");
      break;
    case 2:
      Serial.println("Error 2: received NACK on transmit of address");
      break;
    case 3:
      Serial.println("Error 3: received NACK on transmit of data");
      break;
    case 4:
      Serial.println("Error 4: undefined Error");
      break;
    case 5:
      Serial.println("Error 5: timeout Error");
      break;
    default:
      Serial.println("Error not a Wire Error");
      break;
  }
}
