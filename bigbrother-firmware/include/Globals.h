#pragma once
#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

// // standard pins for the shield, adjust as necessary
#define BIGBRO_RX 2     // Receive Pin for Software Serial Emulation
#define BIGBRO_TX 3     // Transmit Pin for Software Serial Emulation
#define BIGBRO_RST 4    // Reset Pin for 808 Module control

// objects
extern Adafruit_FONA bigbrother;    // This is the main object for Fona 808 control
                                    // Includes most (all?) needed methods for GPS+GSM
                                    // TODO: Read through this, we might not need to write
                                    //       hardly any code to send GSM data

// These are mostly used for serial debugging, won't be used in final product
extern SoftwareSerial bigbrotherSS;
extern SoftwareSerial *bigbrotherSerial;

// global variables
extern float latitude, longitude, speed_kph, heading, speed_mph, altitude;
extern bool gps_success;
extern int status_GPS;