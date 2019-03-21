#include "GPS.h"

void gpsInit(void)
{
    uint8_t startup_gps_success = 0; // small local variable for GPS test lock

    // Establish serial communication with GPS module
    bigbrotherSerial->begin(4800) /* code */;
    if (!bigbrother.begin(*bigbrotherSerial))
    {
        // TODO: Add code here to blink all LEDs to show BigBrother is not ready
        while (1)
            ;
    }

    bigbrother.enableGPS(true); // turn this bad boy on

    // Make sure GPS has an actual lock before we let it move on
    do
    {
        startup_gps_success = updateGPS();
    } 
    while (!startup_gps_success);
}

void serial_debug_info( uint8_t status_GPS)
{
    if (status_GPS)
    {
        // More data is available, but this should be all we need
        Serial.print("GPS lat:");
        Serial.println(latitude, 2);
        Serial.print("GPS long:");
        Serial.println(longitude, 2);
        Serial.print("GPS altitude:");
        Serial.println(altitude);
    }
    else
    {
        Serial.print("Location was lost!");
    }
    
}
uint8_t updateGPS(void)
{
    gps_success = bigbrother.getGPS(&latitude, &longitude, &altitude);

    if (gps_success)
        return 1;
    else
        return 0;
}