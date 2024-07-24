/* Tranmsitter code for the Arduino Radio control with PWM output
 * Install the NRF24 library to your IDE
 * Upload this code to the Arduino UNO, NANO, Pro mini (5V,16MHz)
 * Connect a NRF24 module to it:
 
    Module // Arduino UNO,NANO
    
    GND    ->   GND
    Vcc    ->   3.3V
    CE     ->   D9
    CSN    ->   D10
    CLK    ->   D13
    MOSI   ->   D11
    MISO   ->   D12

This code transmits 1 channels with data from pins A0 POTENTIOMETER
Please, like share and subscribe : https://www.youtube.com/c/ELECTRONOOBS
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>

/**************************************************/

/*Create a unique pipe out. The receiver has to 
  wear the same unique code*/  
const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL; //Remember that this code should be the same for the receiver

/**************************************************/

RF24 radio(9, 10);

// The sizeof this struct should not exceed 32 bytes
struct Data_to_be_sent {
  byte pot_value; 
};
Data_to_be_sent pot_data;

/*//////////////////////////////////////////////////////*/

//This function will only set the value to  0 if the connection is lost...
void resetData() 
{
  pot_data.pot_value = 0;  
}

/**************************************************/

void setup()
{
  radio.begin();
  //Serial.begin(9600);
  //Serial.println("STARTED TRANSMITTER");
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(my_radio_pipe);  
  resetData();
  
}

/**************************************************/
/*
void loop()
{
  //If your channel is reversed, just swap 0 to 255 by 255 to 0 below
  EXAMPLE:
  Normal:    data.ch1 = map( analogRead(A0), 0, 1024, 0, 255);
  Reversed:  data.ch1 = map( analogRead(A0), 0, 1024, 255, 0);  //
  
  pot_data.ch1 = map( analogRead(A0), 0, 1024, 0, 255);

  radio.write(&sent_data, sizeof(Data_to_be_sent));
}
*/
// Returns a corrected value for a potentiometer analog read
// It will map the value from 0 to 1024 to 1 to 255
int mapPotentiometers(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 128);
    //val = 255;
  else
    val = map(val, middle, upper, 128, 255);
    //val = 255;
  return ( reverse ? 255 - val : val );
}

void loop()
{
  pot_data.pot_value = mapPotentiometers(analogRead(A2), 0, 512, 1024, true);  
  radio.write(&pot_data, sizeof(Data_to_be_sent));
}