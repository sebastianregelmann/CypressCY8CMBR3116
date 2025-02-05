#include "CypressCY8CMBR3116.h"
#include <Wire.h>

#define I2C_ADDRESS 0x51  //I2C Address of the 1. Cypress IC (0x51 is default)
#define I2C_ADDRESS 0x52  //I2C Address of the 2. Cypress IC

#define REQUEST_TIMEOUT 40  //After how many request is a Timeout triggered

//Create instances of the IC
CY8CMBR3116 touchIC_1(I2C_ADDRESS_1, REQUEST_TIMEOUT);
CY8CMBR3116 touchIC_2(I2C_ADDRESS_2, REQUEST_TIMEOUT);


void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Start Programm");
}

void loop() {
  //Request status of the first touch ic
  uint8_t touchStatusBuffer_1[2];
  uint8_t error_1 = touchIC_1.get_BUTTON_STAT(touchStatusBuffer_1);
  if (error_1 != 0) {
    Serial.println("Error for IC 1");
    printError(error_1);
  }

  //Request status of the second touch ic
  uint8_t touchStatusBuffer_2[2];
  uint8_t error_2 = touchIC_2.get_BUTTON_STAT(touchStatusBuffer_2);
  if (error_2 != 0) {
    Serial.println("Error for IC 2");
    printError(error_2);
  }

  //Combine both buffer into one
  uint8_t combinedTouchStatusBuffer[4] = { touchStatusBuffer_1[0], touchStatusBuffer_1[1], touchStatusBuffer_2[0], touchStatusBuffer_2[1] };

  //print the buffer
  printStatus(combinedTouchStatusBuffer);


  delay(500);
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
  for (int i = 0; i < 8; i++) {
    uint8_t pressed = statusBuffer[1] >> i & 0b00000001;
    Serial.print(pressed);
    Serial.print(", ");
  }

  //Third 8 bit
  for (int i = 0; i < 8; i++) {
    uint8_t pressed = statusBuffer[2] >> i & 0b00000001;
    Serial.print(pressed);
    Serial.print(", ");
  }

  //forth 8 bit
  for (int i = 0; i < 7; i++) {
    uint8_t pressed = statusBuffer[3] >> i & 0b00000001;
    Serial.print(pressed);
    Serial.print(", ");
  }
  //last bit
  uint8_t pressed = statusBuffer[3] >> 7 & 0b00000001;
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
