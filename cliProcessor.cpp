#define CLIprocessor_CPP 1
#ifndef CLIprocessor_H
#include "cliProcessor.h"
#endif
/**
* @author Rhys Bryant <rhys@voltprojects.com>
* basic command processing support for Arduino
* I make no clam to be a (real) c++ programmer. if you can see a better way please let me know or report a bug
* @copyright Rhys Bryant <rhys@voltprojects.com>
*
* This code is provided "AS IS" without warranty of any kind.
*   In no event shall the author be held liable for any damages arising from the
*   use of this code.
* @licence LGPL
* this notice MUST not be removed
*/

/**
* command result. contains return value from the command (if any) and a error Code
**/


class CmdResult {
public:
    char* value;
    int errorCode;
    CmdResult(int errCode) {
        errorCode=errCode;
        value="";
    }
    CmdResult(char* _value) {
        //copy the refenced value into the new memory location
        value=new char[strlen(_value)+1];
        strcpy(value,_value);
        errorCode=0;
    }
};
class ChanedCommand {
public:
    char* command;
    unsigned long startTime;
    ChanedCommand(char* cmd,int length) {
        //copy the command
        command=new char[strlen(cmd)+1];
        strcpy(command,cmd);
        //dose not have logic for millis overflow
        startTime=millis()+length;
    }
    ChanedCommand() {
        startTime=0;
    }
    bool isTimeUp() {
        bool tiu=(startTime>0 && startTime<=millis());
        if(tiu)
            startTime=0;
        return  tiu;
    }

};
class CliHelpers {
private:
    static char tmp[10];//temp ref used by getValue
public:
    /**
    * class for key value pares i.e pin number=value
    **/
    class SetCmd {
    public:
        int value;
        int pin;
        /**
        * create an object with the values for x=y
        * @param char* data the data to parse
        **/
        SetCmd(char* data) {
            char tmp[10];
            value=-1;
            pin=-1;
            sscanf(data,"%s %s %d=%d",tmp,tmp,&pin,&value);
        }
    };
    /**
    * @return char* a ref to the value as char. this ref is updated on the next call to this function
    * @param float Ivalue to value to convert
    **/
    static char* getValue(float Ivalue) {
        int dValue = (int)Ivalue;
        int value = (int)(((float)(Ivalue - dValue) * 100));
        sprintf(tmp,"%d.%d",dValue,value);
        return tmp;
    };
    static char* getValue(int value) {
        sprintf(tmp,"%d",value);
        return tmp;
    };
    /**
    * @return the value in the passed string converted to a float
    * @param char* the char array to scan for the float
    **/
    static float getValue(char* value) {
        int val=0;
        int val1=0;
        return atof(value);
        int count=sscanf(value,"%d.%d",&val,&val1);
        if(count==1)
            return (float)val;
        else if(count==2)
            return(float)(val)+(val1/100);
        else
            return 0.0;
    }
};
typedef CmdResult (*cli_cmd_cb)(char* cmd);

class CliProcessor {
private:
    class Cmd {
    public:
        char* cmd;
        char* subCmd;
        cli_cmd_cb callback;
    };
    Cmd cmds[COMAND_ARRAY_SIZE];
    ChanedCommand chanedCommand[COMMAND_CHANE_SIZE];
    int index;
public:
    int bufferSize;
    int bufferLength;
    char buffer[90];
    CliProcessor() {
        index=0;
        bufferLength=0;
        bufferSize=77;
        strcpy(buffer,"");
        //buffer=new char[bufferLength];
    };
    /**
    * @param cmd the command
    * @param the sub command
    * @param cmd_cb the method to call. must return CmdResult and have one param, char*
    * @example CmdResult(char* data){return cmdResult("there was no error");}
    **/
    void addCmd(char* cmd,char* subCmd,cli_cmd_cb cmd_cb) {
        if(index>=COMAND_ARRAY_SIZE) {
            return;
        }
        cmds[index].cmd=cmd;
        cmds[index].subCmd=subCmd;
        cmds[index].callback=cmd_cb;
        index++;
    }
    /**
    * @return the first unused array index from the fixed size array
    **/
    int findFreeChaneIndex() {
        for(int i=0; i<COMMAND_CHANE_SIZE; i++)
            if(chanedCommand[i].startTime==0)
                return i;
        return 0;
    }
    /**
    * checks for delayed commands to run
    **/
    void checkCommandChane() {
        for(int i=0; i<COMMAND_CHANE_SIZE; i++) {
            if(chanedCommand[i].command && chanedCommand[i].isTimeUp()) {
                processCommand(chanedCommand[i].command);//run the command
                if(chanedCommand[i].command) {
                    delete[] chanedCommand[i].command;
                    chanedCommand[i].command=NULL;
                }
            }
        }

    }
    /**
    * check for new incoming data
    * @return void
    **/
    void checkSerial() {
        int length=Serial.available();
        char tmp[2]="";
        if(length>0) {
            for(int l=0; l<length && bufferLength<bufferSize; l++) {
                tmp[0]=Serial.read();
                strcat(buffer,tmp);
            }
            tmp[0]='\0';
            strcat(buffer,tmp);
        }
        if(strlen(buffer)>1 && strpbrk(buffer,"\n")) {
            //we have data in our buffer lets check it out

            char * line;
            strcat(buffer,"END");//append END to our buffer so we know when we've hit the end
            line = strtok (buffer,"\n");
            while (true)
            {

                if(line) {
                    //is this line the last?
                    char* p=strstr(line,"END");
                    if(p && strcmp(p,"END")==0) {
                        if(strcmp(line,"END")==0)
                            strcpy(buffer,"");
                        else {
                            //the buffer dose not end with \n we are still waiting on the rest of the command
                            strcpy(buffer,"");
                            strncat(buffer,line,strlen(line)-3);
                        }
                    }
                    else {
                        //we have recived a full line. parse it.
                        processCommand(line);
                    }
                }
                else
                    break;
                line = strtok (NULL, "\n");
            }
        }
        //check for delayed commands
        checkCommandChane();
    }
    /**
    * proceses the command looks up the name and subname and calls the callback
    * @return void
    * @param the command data to parse.
    **/
    void processCommand(char* data) {
        char cmd[10]="";
        char subCmd[10]="";
        char v[20];
        char delayCmd[50];
        int delayLength=0;
        int count=sscanf(data,"%s %s %s delay %d %[^\n]",cmd,subCmd,v,&delayLength,delayCmd);
        bool found=false;
        for(int i=0; i<index; i++) {
            if(strcmp(cmds[i].cmd,cmd)==0) {
                if(strcmp(cmds[i].subCmd,subCmd)==0) {
                    //when we have a mach for the command and sub command call the callback
                    found=true;
                    CmdResult result=cmds[i].callback(data);
                    char out[30];
                    if(result.errorCode>0) {//if the error code is 0 then there was no error
                        sprintf(out,"%s%s err,%d",cmd,subCmd,result.errorCode);
                    }
                    else {
                        //if the result contains a value. add the value to the returned result. and free up the memory
                        if(*result.value) {
                            sprintf(out,"%s%s ok,%s",cmd,subCmd,result.value);
                            delete[] result.value;
                        }
                        else {
                            //the result dose not contain a value
                            sprintf(out,"%s%s ok",cmd,subCmd);
                        }

                    }
                    //send the result
                    Serial.println(out);

                }

            }

        }
        //if no nmaching command was found send an error status
        if(!found) {
            char out[30];
            sprintf(out,"%s%s err,1",cmd,subCmd);
            Serial.println(out);
        }
        if(count==5) {
            //add a delayed cmd
            int index=findFreeChaneIndex();
            chanedCommand[index]=ChanedCommand(delayCmd,delayLength);
        }
    }
};