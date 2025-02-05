#include <CypressCY8CMBR3116.h>
#include <Wire.h>

#define I2C_START_ADDRESS 0     //I2C Address of the Cypress IC
#define I2C_END_ADDRESS 127     //I2C Address of the Cypress IC
#define REQUEST_TIMEOUT 10 //After how many request is a Timeout triggered

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Start Programm");
}

void loop() {
bool foundIC = false;

  Serial.println("Start scanning ...");
  for(int I2C_ADDRESS = I2C_START_ADDRESS; I2C_ADDRESS <I2C_END_ADDRESS; I2C_ADDRESS++)
  {
    CY8CMBR3116 touchIC(I2C_ADDRESS, REQUEST_TIMEOUT);

    //request the Sensors DEVICE_ID
    uint16_t ID_BUFFER[1];
    uint8_t error = touchIC.get_DEVICE_ID(ID_BUFFER);

    if(error == 0)
    {
      Serial.print("CY8CMBR3116 at address: ");
      Serial.print(I2C_ADDRESS);
      Serial.print(" with DEVICE_ID: ");
      Serial.println(ID_BUFFER[0]);
  
      foundIC = true;
    }
  }

  if(foundIC == false){
    Serial.println("No Cypress touch ic found");
  }

    Serial.println("Stop scanning ...");

  delay(1000);
}
