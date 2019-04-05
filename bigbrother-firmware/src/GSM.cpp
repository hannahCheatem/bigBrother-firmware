#include "GSM.h"
#include "Globals.h"

//establish connection to server
void gsmInit(char* server, int port){

    Serial.println(F("beginning GSM connection"));

    bigbrother.setGPRSNetworkSettings(F("hologram"), F(""), F(""));
    bigbrother.enableGPRS(true);

//
    // read the network/cellular status
        uint8_t n = bigbrother.getNetworkStatus();
    while (n != 1 && n != 5){
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
    }

      
//

    if(bigbrother.TCPconnect(server, port)){
        //if connected successfully
        Serial.println("Server Connection Successful");
    } else {
        int i = 0;
        Serial.println("Failed to connect to server");
        while(!bigbrother.TCPconnected() && i < 25){
            Serial.println("reattempting connection");
            
            bigbrother.TCPconnect(server, port);
            //check 25 times
            i++;
        }
        if (i == 25){
            Serial.println("server connection failed");
        }
    }
}

void checkConnection(void){
    if(bigbrother.TCPconnected()){
        Serial.println("Connected");
    } else {
        Serial.println("No connection");
    }

}

//send gps data to server
void sendToServer(void){
    char lat[10], lon[10], alt[10];
    dtostrf(latitude, 2, 6, lat);
    dtostrf(longitude, 2, 6, lon);
    dtostrf(altitude, 2, 6, alt);
    char location[30];
    strcat(location, lat);
    strcat(location, lon);
    strcat(location, alt);

    if (bigbrother.TCPconnected()){
        bigbrother.TCPsend(location, 30); 
        Serial.println("GPS data sent");
    }
    
}