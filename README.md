
#  Cypress CY8CMBR3116

A library for the Arduino IDE to control a Cpyress Cy8CMBR3116 Capsense Touch IC with I2C and the Wire.h library. 



## Definition

```cpp
#include <YourLibrary.h>

#define I2C_ADDRESS 0x51 //Default value    
#define REQUEST_TIMEOUT 10

//Create an instance of the IC
CY8CMBR3116 touchIC(I2C_ADDRESS, REQUEST_TIMEOUT)

void setup()
{
    Wire.begin();
}
```
I2C_ADDRESS is the address of the CY8CMBR3116 IC you want to control. Default value is 0x51. 
REQUEST_TIMEOUT is the number of connection attemps before the connection is aborted. 

## Requesting data
### Predefined methods
```cpp
uint8_t resultBuffer[1];
uint8_t errorCode = touchIC.get_I2C_ADDR(resultBuffer);
```
Request the data from the predefined register and writes it into the result buffer. How big the buffer should be depends on the register that is requested. 
For further details [look here](#Questions).

### Request  bytecount from register address
```cpp
uint8_t registerAddress = 0x08;
uint8_t byteCount = 4;
uint8_t resultBuffer[byteCount];
uint8_t errorCode = touchIC.requestDataFromAddress(registerAddress,byteCount,resultBuffer);
````
Request byteCount number of bytes from the CY8CMBR3116. Start register is registerAddress and increments over the bytes. 

### Set register then request number of bytes
```cpp
uint8_t registerAddress = 0x08;
uint8_t byteCount = 4;
uint8_t resultBuffer[byteCount];
uint8_t errorCode = touchIC.setPointer(registerAddress); //Set the register from where data is read
errorCode = touchIC.requestData(byteCount, resultBuffer); //Set the number of bytes that get's read
````

First sets the registerAddress from wich bytes are read, then request byteCount number of bytes.

## Writing data
### Predefined methods
```cpp
uint8_t dataBuffer[1] = {255};   //Data that gets written to the IC
uint8_t errorCode = touchIC.set_I2C_ADDR(registerAddress);
````
Writes the data to the predefined register. How big the buffer should be depends on the register that is written to. For further details [look here](#Questions).

### Write bytecount to register address
```cpp
uint8_t registerAddress = 0x08;
uint8_t byteCount = 4;
uint8_t dataBuffer[byteCount] = {255,255,255,255};   //Data that gets written to the IC
uint8_t errorCode = touchIC.write(registerAddress, byteCount, dataBuffer);
```
Writes byteCount number of bytes to register. Starting register is registerAddress and it increments. 

## Error codes
Every mehod returns an uint8_t value that is the error code. Depending on how the communiction with the CY8CMBR3116 worked out. The Error codes are the same as in the Arduino Wire library. 
- 0: success.
- 1: data too long to fit in transmit buffer.
- 2: received NACK on transmit of address.
- 3: received NACK on transmit of data.
- 4: other error.
- 5: timeout

## Special functions
```cpp
uint8_t errorCode = touchIC.applyRegister();
uint8_t errorCode = touchIC.calculateCRC();
uint8_t errorCode = touchIC.enterLowPower();
uint8_t errorCode = touchIC.resetIC();
uint8_t errorCode = touchIC.activateSettings();
```
Look into the register Datasheet for the CMD_CTRL register. these functions send commands to the IC. The activateSettings() method does actually apply the changed register, but the IC must be reseted to actually apply the changes.

## Example code
```cpp
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
```
This code request every 0.5 seconds the status of the touch buttons. It is asumend that all 16 sensors are configured as touch button. 

## Questions 
When not sure what kind of buffer to request/send from/to the IC look into the register documentation from Cypress that is found here: [Register Map](https://www.infineon.com/dgdl/Infineon-CY8CMBR3xxx_CapSense_Express_Controllers_Registers_TRM-AdditionalTechnicalInformation-v06_00-EN.pdf?fileId=8ac78c8c7d0d8da4017d0f90b2ad7da7). 

Also look into the [CY8CMBR3116.cpp](https://github.com/sebastianregelmann/CypressCY8CMBR3116/blob/main/src/CypressCY8CMBR3116.cpp) to check what kind of buffers are used and how they are used. Some methods return uint16_t buffers because the register is in this format. 
## Installation

Install my-project with npm

```bash
  npm install my-project
  cd my-project
```
    
