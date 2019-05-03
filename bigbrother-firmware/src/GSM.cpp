#include "GSM.h"
#include "Globals.h"

#define halt(s) { Serial.println(F( s )); while(1);  }


uint8_t type;

double deg2rad(double deg) {
  return deg * (PI/180);
}

boolean FONAconnect(const __FlashStringHelper *apn, const __FlashStringHelper *username, const __FlashStringHelper *password) {
  
  bigbrotherSerial->println("AT+CMEE=2");
  
  Serial.println(F("Checking for network..."));

  uint8_t n;
  boolean success=false;
  do {
    n = bigbrother.getNetworkStatus();
    Serial.print(F("Network status "));
    Serial.print(n);
    Serial.print(F(": "));
    if (n == 0) Serial.println(F("Not registered"));
    if (n == 1) Serial.println(F("Registered (home)"));
    if (n == 2) Serial.println(F("Not registered (searching)"));
    if (n == 3) Serial.println(F("Denied"));
    if (n == 4) Serial.println(F("Unknown"));
    if (n == 5) Serial.println(F("Registered roaming"));
    delay(500);
    if ((n == 1) || (n==5))
      success = true;
  } while(success == false);
  
  delay(5000);  // wait a few seconds to stabilize connection

   // replaced "enableGPRS"
   //
   bigbrother.jamesStart();
  return true;
}

//establish connection to server
void gsmInit(char* server, int port){
    Serial.println(F("beginning GSM connection"));

   while(!FONAconnect(F(FONA_APN), F(FONA_USERNAME), F(FONA_PASSWORD))){}

//
    if(bigbrother.TCPconnect(server, port)){
        //if connected successfully
        Serial.println(F("Server Connection Successful"));
        sendToServer();
    } else {
        Serial.println(F("Failed to connect to server"));
        while(!bigbrother.TCPconnected()){
            Serial.println(F("reattempting connection"));
            Serial.println(bigbrother.GPRSstate());
            bigbrother.TCPconnect(server, port);
        }
    }
   

}

void checkConnection(char* server, int port){
    if(bigbrother.TCPconnected()){
        Serial.println(F("Connected"));
    } else {
        Serial.println(F("No connection - reattempting connection"));
        if(bigbrother.GPRSstate() != 1)
            bigbrother.enableGPRS(true);

        while(!bigbrother.TCPconnect(server, port)){}

    }

}

//send gps data to server
void sendToServer(void){
    char lat[11] = {0}, lon[11] = {0};
    //alt[10] = {0};

    char location[22] = {0};

    char *temp;

    temp = dtostrf(latitude, 2, 6, lat);
    if (latitude < 0){
        lat[9] = 'S';
        lat[0] = '0';
    } else {
         lat[9] = 'N';
    }
    
    strcat(location, temp);

    location[10] = ',';

    temp = dtostrf(longitude, 2, 6, lon);
    if (longitude < 0) lon[9] = 'W'; else lon[9] = 'E';
    
    strcat(location, temp);

    location[21] = '\0';

    //dont need to send altitude
    //temp = dtostrf(altitude, 2, 6, alt);
    //strcat(location, temp);

    char instruction[20] = {"  setLocation BB_0 "};
    char sentString[40] = {0};
    strcat(sentString, instruction);
    strcat(sentString, location);

    if (bigbrother.TCPconnected()){
        //bigbrother.TCPsend("40", 2);
        bigbrother.TCPsend(sentString, 40);
        //Serial.println(sentString);
        Serial.println("GPS data sent");
    }
    
}

int recieveZoneData(void){
    int led = 0; //0 = red, 1 = yellow, 2 = green
    uint8_t center[40] = {0};
    if (bigbrother.TCPconnected()){
        bigbrother.TCPsend("  getZone BB_0", 15);
        bigbrother.TCPread(center, 40);
        double location[3] = {0};
        char temp[40] = {0};
        int k = 0;

        for (int i = 0; i < 40; i++){
            int j = 0;
            while (i < 40 && center[i] != ','){
                temp[j] = center[i];
                j++;
                i++;
            }
            bool negative = false;
            if(temp[j - 1] == 'S' || temp[j-1] == 'W'){
                negative = true;
            }
            temp[j-1] = '0';
            double t = atof(temp);
            if (negative){
                t = t * -1;
            }
            location[k] = t;
            k++;
        }
        double lat = location[0];
        double lon = location[1];
        double radius = location[2];

////

        int R = 6371000;                      // Radius of the earth in m
        double dLat = deg2rad(lat-latitude);     // deg2rad below
        double dLon = deg2rad(lon-longitude); 
        double a = 
            sin(dLat/2) * sin(dLat/2) +
            cos(deg2rad(latitude)) * cos(deg2rad(lat)) * 
            sin(dLon/2) * sin(dLon/2)
            ; 
        double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
        double d = R * c; // Distance in m

  ////
        //double distance = sqrt((longitude - lon)^2 + (latitude - lat)^2);
        if (d >= radius)
            led = 0;
        else if (d > radius - 5)
            led = 1;
        else if (d < radius - 5)
            led = 2;

    }

    return led;
}