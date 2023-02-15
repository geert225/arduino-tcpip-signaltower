#include <Arduino.h>
#include <timedtask.c>
#include <pincontrol.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Ethernet.h>
#include <debug.h>

#define STATE_OFF 0
#define STATE_ERROR 1
#define STATE_STOPPED 2
#define STATE_STARTED 3
#define STATE_STOPPING 4
#define STATE_BLUE 5
#define STATE_YELLOW 6
#define STATE_PURPLE 7
#define STATE_ORANGE 8
#define STATE_CLEAR 9

#define MAXDATA 10

#define MODULES 2
#define LEDS MODULES * 8

//Ethernet variable
byte mac[] = { 0x65, 0x6C, 0x72, 0xDF, 0x0C, 0x6B };
IPAddress Local(10,0,100,20);
EthernetServer server(3050);

//led variavle
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LEDS, 2, NEO_GRB + NEO_KHZ800);
bool flashState = false;
byte lightState = 0;

//internal functions
void(* resetArduino) (void) = 0x00;

//timed tasks
timedtask LampControlTask;

void taskUpdateLamp(){
  flashState = !flashState;
  uint32_t colorcode = 0;
  switch (lightState)
  {
  case STATE_ERROR:
      if (flashState)
        colorcode = leds.Color(255,0,0);
    break;
  case STATE_STOPPED:
      colorcode = leds.Color(255,0,0);
    break;
  case STATE_STARTED:
      colorcode = leds.Color(0,255,0);
    break;
  case STATE_STOPPING:
      if (flashState)
        colorcode = leds.Color(0,255,0);
      else
        colorcode = leds.Color(255,0,0);
    break;
  case STATE_BLUE:
      colorcode = leds.Color(0,0,255);
    break;
  case STATE_PURPLE:
      colorcode = leds.Color(255,0,255);
    break;
  case STATE_YELLOW:
      colorcode = leds.Color(255,200,0);
    break;
  case STATE_ORANGE:
      colorcode = leds.Color(255,100,0);
    break;
  default:
      colorcode = leds.Color(0,0,0);
    break;
  }
  
  for (int i = 0; i < leds.numPixels(); i++)
  {
      leds.setPixelColor(i, colorcode);
  }
  leds.show();
}



void setup() {
  #ifdef GVG_DEBUG
    Serial.begin(9600);
    while (!Serial)
    {
      delay(10);
    }
    debug("serial ok!");
  #endif

  //init ethernet
  Ethernet.init(10);
  Ethernet.begin(mac, Local);
  debug(Ethernet.localIP());
  //init leds
  leds.begin();
  leds.setBrightness(255);
  leds.show();
  //setup tasks
  LampControlTask = timedtask_create_task(500, &taskUpdateLamp);
  //start server
  server.begin();
}

void loop() {
  EthernetClient client = server.available();

  if (client) {
    debug("Client connected!");
    timedtask_enable_task(&LampControlTask);
    while (client.connected())
    {
      if(client.available()){
        byte data = client.read();
        debug(data);
        if(data < MAXDATA){
          lightState = data;
        }
      }
      timedtask_check_task(&LampControlTask);
    }
    timedtask_disable_task(&LampControlTask);
  }
}
