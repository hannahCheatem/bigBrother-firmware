#pragma once
#include "Globals.h"
#include "HardwareSerial.h"

void gpsInit( void );       // Set up and initialize GPS Module

void serial_debug_info( uint8_t status_GPS); // Sends out location info on Serial port

uint8_t updateGPS( void );  // Use this function to update GPS data
                            // returns 1 if GPS was successful, 0 if un-successful
