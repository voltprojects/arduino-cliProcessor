/*
@link https://github.com/voltprojects/arduino-cliProcesser
example for CliProcesser By Rhys Bryant. to use.
type "led on" or "led off" in the serial monitor (without the quotes) and press enter.
Make sure line ending is set to "newline" 
*/
#include <cliProcesser.h>
CliProcesser cli;

//basic examples
/**
* callback for led_on
*/
CmdResult led_on(char* data){
   digitalWrite(13,HIGH);
   return CmdResult(0);//0 error code 0 means the command ran successfully
}
/**
* callback for led_off
*/
CmdResult led_off(char* data){
   digitalWrite(13,LOW);
   return CmdResult(0);//0 error code 0 means the command ran successfully
}
void setup(){
  Serial.begin(9600);//setup the serial port
  pinMode(13,OUTPUT);//set pin 13 to output mode
  cli.addCmd("led","on",led_on);
  cli.addCmd("led","off",led_off);
  Serial.println("welcome, please type a command");
}
void loop(){
  cli.checkSerial();//check for new incoming commands
  delay(10);
  
}

