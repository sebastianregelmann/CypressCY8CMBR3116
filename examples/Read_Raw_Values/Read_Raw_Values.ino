#include "CypressCY8CMBR3116.h"
#include <Wire.h>

#define I2C_ADDRESS 0x51     //I2C Address of the Cypress IC (0x51 is default)
#define REQUEST_TIMEOUT 10 //After how many request is a Timeout triggered

//Create an instance of the IC
CY8CMBR3116 touchIC(I2C_ADDRESS, REQUEST_TIMEOUT);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Start Programm");
}

void loop() {
  readRawValues();
  delay(500);
}

void readRawValues() {
  Serial.println("Requesting Buffer ...");

  //buffer that stores the raw value of each sensor
  uint16_t rawValuesBuffer[16];

  //requests the raw values from each sensor
  uint8_t errorCode = touchIC.get_DIFFERENCE_COUNT_SENSOR(rawValuesBuffer);

  if (errorCode == 0) {
    //Print the Raw Values of all 16 Sensors
    printBuffer(rawValuesBuffer);
  } else {
    //Print the error
    printError(errorCode);
  }
}


void printBuffer(uint16_t *valueBuffer) {
  Serial.println("Raw Values:");
  for (int i = 0; i < 15; i++) {
    Serial.print(valueBuffer[i]);
    Serial.print(", ");
  }
  Serial.println(valueBuffer[15]);
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
