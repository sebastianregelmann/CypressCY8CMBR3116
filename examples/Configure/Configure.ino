#include <CypressCY8CMBR3116.h>
#include <Wire.h>

#define I2C_ADDRESS 0x51      //I2C Address of the Cypress IC (0x51 is default)
#define REQUEST_TIMEOUT 40  //After how many request is a Timeout triggered
#define NEW_I2C_ADDRESS 0x51 //I2C Address after programm. IC needs to be reseted for changes to apply

//Create an instance of the IC
CY8CMBR3116 touchIC(I2C_ADDRESS, REQUEST_TIMEOUT);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Start Programm");
  configueTouchIC();
}

void loop()
{
}

void configueTouchIC() {
  Serial.println("Configuring sensor settings ...");

  delay(250);
  //Configure sensor settings
  uint8_t error = configureSensors();
  if (error != 0) {
    Serial.println("Error while configuring sensor settings");
    printError(error);
    return;
  } else {
    Serial.println("Configuring sensor settings succsessfull");
  }

  delay(250);
  //Configure I2C Address
  error = configureAddress();
  if (error != 0) {
    Serial.println("Error while configuring I2C address");
    printError(error);
    return;
  } else {
    Serial.println("Configuring I2C address succsessfull");
  }

  delay(250);
  //apply the changes and restart the touch IC
  error = touchIC.activateSettings();
  if (error != 0) {
    Serial.println("Error while activating new register");
    printError(error);
    return;
  } else {
    Serial.println("Activating new register succsessfull");
  }
  Serial.println("Configuring succsessfull ...");
}

uint8_t configureSensors() {
  Serial.println("Set SENSOR_EN settings");
  //Enables/Disables senosrs 1 = Enabled, 0 = Disabled
  uint8_t SENSOR_0_7 = 0b11111111;   //Configures sensor enabled MSB = 7 LSB = 0
  uint8_t SENSOR_8_15 = 0b11111111;  //Configures sensor enabled MSB = 15 LSB = 8

  //Sends the databuffer to the touch IC
  uint8_t SENSOR_EN_BUFFER[2] = { SENSOR_0_7, SENSOR_8_15 };
  uint8_t error = touchIC.set_SENSOR_EN(SENSOR_EN_BUFFER);

  //when something went wrong return the error
  if (error != 0) {
    return error;
  }


  Serial.println("Set FFS_EN settings");
  //Enables/Disables FFS
  SENSOR_0_7 = 0b11111111;   //Configures sensor enabled MSB = 7 LSB = 0
  SENSOR_8_15 = 0b11111111;  //Configures sensor enabled MSB = 15 LSB = 8

  //Sends the databuffer to the touch IC
  uint8_t SENSOR_FFS_BUFFER[2] = { SENSOR_0_7, SENSOR_8_15 };
  error = touchIC.set_FFS_EN(SENSOR_FFS_BUFFER);

  //when something went wrong return the error
  if (error != 0) {
    return error;
  }

  Serial.println("Set SENSITIVITY settings");
  //Sets the Sensitivity for each sensor
  uint8_t SENSITIVITY_0_3 = 0b11111111;    //first two bytes sensitivity for sensor 3; second two bytes sensitivity for sensor 2; ...
  uint8_t SENSITIVITY_4_7 = 0b11111111;    //first two bytes sensitivity for sensor 4; second two bytes sensitivity for sensor 7; ...
  uint8_t SENSITIVITY_8_11 = 0b11111111;   //first two bytes sensitivity for sensor 8; second two bytes sensitivity for sensor 11; ...
  uint8_t SENSITIVITY_12_15 = 0b11111111;  //first two bytes sensitivity for sensor 12; second two bytes sensitivity for sensor 15; ...

  //Sends the settings to the touch IC
  uint8_t SENSITIVITY_BUFFER[4] = { SENSITIVITY_0_3, SENSITIVITY_4_7, SENSITIVITY_8_11, SENSITIVITY_12_15 };
  error = touchIC.set_SENSITIVITY_all(SENSITIVITY_BUFFER);
  if (error != 0) {
    return error;
  }

  Serial.println("Set THRESHOLD settings");
  //Send thrshold settings to touch IC
  uint8_t THRESHOLD_BUFFER[16] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160 };  //From index 0-15 the threshold value for sensor 0-15
  error = touchIC.set_THRESHOLD(THRESHOLD_BUFFER);

  //Return the final error Code
  return error;
}

uint8_t configureAddress() {
  Serial.println("Set I2C_ADDRESS settings");
  //sets a new I2C Address
  uint8_t I2C_ADDRESS_BUFFER[1] = {NEW_I2C_ADDRESS};
  uint8_t error = touchIC.set_I2C_ADDR(I2C_ADDRESS_BUFFER);
  return error;
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
