#include "Adafruit_FONA.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_FONA.h"
#include <string.h>

#define FONA_APN             "hologram"  
#define FONA_USERNAME        ""  
#define FONA_PASSWORD        "" 

//establish connection to server
void gsmInit(char* server, int port);

void checkConnection(void);

//send gps data to server
void sendToServer(void);