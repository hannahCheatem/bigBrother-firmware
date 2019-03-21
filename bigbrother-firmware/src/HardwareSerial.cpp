#include "HardwareSerial.h"
#include "Globals.h"

SoftwareSerial bigbrotherSS = SoftwareSerial(BIGBRO_TX, BIGBRO_RX);
SoftwareSerial *bigbrotherSerial = &bigbrotherSS;

void hardwareSerialInit ( void ) {
  // Setup HW Serial port for debug info
  while( !Serial );
  Serial.begin(115200); // this baudrate is fast enough that it won't interrupt
                        // The data from the GPS being handled (hopefully)
}
