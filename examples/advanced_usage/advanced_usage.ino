/*
@link https://github.com/voltprojects/arduino-cliProcesser
example for CliProcesser By Rhys Bryant. to use.
type "led on" or "led off" in the serial monitor (without the quotes) and press enter.
Make sure line ending is set to "newline" 
*/
#include <cliProcesser.h>
CliProcesser cli;
/**
* cli callback for set pin command
* @return CmdResult ; the return value or status code 0 = no error
* @param char* data the full line  
**/
CmdResult set_pin(char* cmd) {
    CliHelpers::SetCmd value=CliHelpers::SetCmd(cmd);
    if(value.pin==-1)
        return CmdResult(1);
    if(value.value==1 || value.value==0) {
        digitalWrite(value.pin,value.value==1?HIGH:LOW);
    }
    else
        return CmdResult(2);
    return CmdResult(0);
}

/**
* cli callback for get pin command
* @return CmdResult ; the return value or status code 0 = no error
* @param char* data, the full line  
**/
CmdResult get_pin(char* data) {
    CliHelpers::SetCmd v=CliHelpers::SetCmd(data);
    if(v.pin==-1)
        return CmdResult(1);//if agrument 2 is not a valid number return error 1
    else {
        char tmp[10]="";
        sprintf(tmp,"%d",digitalRead(v.pin));//get the value of the pin x
        return CmdResult(tmp);//non error values must always be char*
    }
}
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
  cli.addCmd("get","pin",get_pin);
  cli.addCmd("set","pin",set_pin);
  cli.addCmd("led","on",led_on);
  cli.addCmd("led","off",led_off);
  Serial.println("welcome, please type a command");
}
void loop(){
  cli.checkSerial();//check for new incoming commands
  delay(10);
  
}

