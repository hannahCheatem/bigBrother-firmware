/**
 * ____  _       ____            _   _               
 *|  _ \(_)     |  _ \          | | | |              
 *| |_) |_  __ _| |_) |_ __ ___ | |_| |__   ___ _ __ 
 *|  _ <| |/ _` |  _ <| '__/ _ \| __| '_ \ / _ \ '__|
 *| |_) | | (_| | |_) | | | (_) | |_| | | |  __/ |   
 *|____/|_|\__, |____/|_|  \___/ \__|_| |_|\___|_|   
 *          __/ |                                    
 *         |___/                                     
 * 
 * Code for the BigBrother Hardware!
 * GPS+GSM Module: FONA 808
 * Microcontroller: ATMega328P
 * Platform: Arduino Uno
 * 
 * Library Copyright:
 * Copyright: 2015 Adafruit
 * Author:    Todd Treece
 * License:   MIT
 * 
 * This project copyright:
 * Copyright: 2019 Minoriteam
 * Author(s): Ben Fradella
 *            Hannah Cheatem
 *            Tyler Whitson
 * 
 * License:  MIT
 * 
 * v0.2:
 *      Set up state machine instead of delay:
 *          Use millis() to count seconds between sending data to free
 *          up microcontroller to do whatever else we might want in the future
 *      Split code up into header files, team can change them if they seem 
 *          unnecessary, there isn't a whole lot in each header.
 */

// Non-Locals
#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

// Local Headers
#include "GSM.h"
#include "Globals.h"
#include "GPS.h"
#include "HardwareSerial.h"

// gps_interval and cell_interval control
// the update interval for GPS location updating 
// and transmist interval for GSM to bb_server, respectively
// They are in milliseconds 
uint16_t gps_interval  = 10000; // 10 seconds
uint16_t cell_interval = 2500;  // 1500 milliseconds

// Counters for GPS and GSM updates to use with millis()
int16_t previousMillis_GPS = 0;
int16_t currentMillis_GPS  = 0;

int16_t previousMillis_GSM = 0;
int16_t currentMillis_GSM  = 0;

// server information
char* server = "206.189.199.185";
int port = 6969;
int led = 0;

//led info
short int greenled = 6;
short int yellowled = 7;
short int redled = 8;

// Arduino peripheral setup
void setup() 
{
  hardwareSerialInit();
  gpsInit();
  gsmInit(server, port);
}

// Main loop
void loop() 
{
    // Set counters to number of current timer overflows
    currentMillis_GPS = millis();
    currentMillis_GSM = millis();

    if( currentMillis_GPS - previousMillis_GPS == gps_interval)
    {   
        previousMillis_GPS = currentMillis_GPS; // Reset time difference
        //status_GPS = updateGPS();

        // Uncomment below for terminal debugging
        // Print out location data for GPS latitude, longitude and altitude
        // serial_debug_info(status_GPS);
    }
    else if (currentMillis_GSM - previousMillis_GSM == cell_interval)
    {
        previousMillis_GSM = currentMillis_GSM; // Reset time difference
        
        checkConnection(server, port);

        sendToServer();

        led = recieveZoneData();

        switch (led){
            case 0: default:
                //light red led
                digitalWrite(greenled, LOW);
                digitalWrite(yellowled, LOW);
                digitalWrite(redled, HIGH);
                break;
            case 1:
                //light yellow led
                digitalWrite(greenled, LOW);
                digitalWrite(yellowled, HIGH);
                digitalWrite(redled, LOW);
                break;
            case 2:
                //light green led
                digitalWrite(greenled, HIGH);
                digitalWrite(yellowled, LOW);
                digitalWrite(redled, LOW);
                break;

        }

    }
}