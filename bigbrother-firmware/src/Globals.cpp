#include "Globals.h"

// global objects that can be used by anybody
Adafruit_FONA bigbrother = Adafruit_FONA(BIGBRO_RST);   

// global variables
float latitude, longitude, speed_kph, heading, speed_mph, altitude;
bool gps_success;
int status_GPS = 0; // Make sure the default is zero, i.e. failure