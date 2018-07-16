// Arduino G-code Interpreter
// v1.0 by Mike Ellery - initial software (mellery@gmail.com)
// v1.1 by Zach Hoeken - cleaned up and did lots of tweaks (hoeken@gmail.com)
// v1.2 by Chris Meighan - cleanup / G2&G3 support (cmeighan@gmail.com)
// v1.3 by Zach Hoeken - added thermocouple support and multi-sample temp readings. (hoeken@gmail.com)
#include <HardwareSerial.h>
//chip >> https://www.google.it/search?q=nodemcu+v3+lua
//Arduino flash:DIO Flash size:16M Reset method:Nodemcu Upload speed:921600 Erase Flash:All flash contents
//Resolve bugs in stepper_control line 147 i insert yield() because esp8266 activate watchdog reset after a long loop
//Resolve bug in stepper_control line 79 If movement is 0 the line 
//310 return ((distance * 60000000.0) / feedrate) / master_steps;  return NAN because all is 0 ... 
//and returned a hudge number 2,147,483,647 for delayMicroseconds(2,147,483,647)
/*
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
*/
#define max1(a,b) ((a)>(b)?(a):(b))
//our command string
#define COMMAND_SIZE 128

unsigned long interval = 3600000;

char word1[COMMAND_SIZE];
byte serial_count;
int no_data = 0;

void setup()
{
/*
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  /*DDRD=0B111100;
  DDRC=0B00001111;*/
	//Do startup stuff here
	Serial.begin(250000);
	Serial.println("start");
	
	//other initialization.
	init_process_string();
	init_steppers();

}

void loop()
{
  char c;
  
  //keep it hot!
 // extruder_manage_temperature();

  //read in characters if we got them.
  if (Serial.available() > 0)
  {
    c = Serial.read();
    no_data = 0;
    
    //newlines are ends of commands.
    if (c != '\r') //  adaugat de mine (c != '\r')
    {
      word1[serial_count] = c;
      serial_count++;
    }
  }
  //mark no data.
  else
  {
    no_data++;
    delayMicroseconds(100);
  }

  //if theres a pause or we got a real command, do it
  if (serial_count && (c == '\n' || no_data > 100))
  {
    //process our command!
     
    process_string(word1, serial_count);
    interval= millis()-interval;
  Serial.println(interval);
  interval=millis();
    //clear command.
    
    init_process_string();
     
  }

  //no data?  turn off steppers
  if (no_data > 1000)

  
    disable_steppers();
}
