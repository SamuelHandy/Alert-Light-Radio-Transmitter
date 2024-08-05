/*******************INCLUDES***********************/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*******************VARIABLES**********************/

//Unique Pipe address
const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL; 

//CSN and CE pins of the NRF
RF24 radio(9, 10);

//0 = OFF
//1 = ON
struct Data_to_be_sent {
  byte light_status = 1; 
};
Data_to_be_sent data_to_be_sent;

//Pin 3
int BUTTON_LED_POWER_PIN = 3;

/*********************SETUP************************/

void setup()
{
  //Setup LED Pin to Output
  pinMode(BUTTON_LED_POWER_PIN,OUTPUT);
  //Setup Radio
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(my_radio_pipe); 
  //Initial Write and Turn LED on
  radio.write(&data_to_be_sent, sizeof(Data_to_be_sent));
  digitalWrite(BUTTON_LED_POWER_PIN, HIGH); 
}

/**********************LOOP************************/

void loop()
{
  radio.write(&data_to_be_sent, sizeof(Data_to_be_sent));
}