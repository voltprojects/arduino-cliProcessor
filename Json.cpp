#define JSON_CPP 1
#ifndef JSON_H
#include "Json.h"
#endif
/**
* @author Rhys Bryant <code@voltprojects.com>
* basic json support for Arduino
* if you can see an issue or better way please report a bug
* @copyright Rhys Bryant <code@voltprojects.com>
*
* === Legal Jargen  ===
* This code is provided "AS IS" without warranty of any kind.
*   In no event shall the author be held liable for any damages arising from the
*   use of this code.
* @licence LGPL
* this notice MUST not be removed
*/
class Json{
 protected:
    char buffer[JSON_OBJECT_BUFFER_SIZE];
    int brace;
    bool firstEntry;
    bool closedBrace;
	bool bufferingEnabled;
    void addCommer(){
        if(!firstEntry){
            strOutput(",");
        }
        else
            firstEntry=false; 
    }
    Json(int b){
        brace=b;
		//open the brace
        sprintf(buffer,brace==0?"{":"[");
		//default values
        firstEntry=true;
        closedBrace=false;
		bufferingEnabled=true;
    };
	void strOutput(char* buff){
		if(bufferingEnabled)
			strcat(buffer,buff);
		else
			Serial.print(buff);
	}
	void strOutput(const char* buff){
		if(bufferingEnabled)
			strcat(buffer,buff);
		else
			Serial.print(buff);
	}
public:
	/**
	* instead of adding the items to a buffer outputs then directly
	**/
	void disableBuffer(){
		bufferingEnabled=false;
		//the first char has not been sent sent it now
		Serial.print(brace==0?"{":"[");
	}
    void close(){
        if(!closedBrace){
         strOutput(brace==0?"}":"]");
         closedBrace=true;
        }
    };
    char* getBuffer(){
        close();//close off the brace before returning the pointer
        return buffer;
    }
};
/**
	JsonObject class output char*, {"name":"value",...}
**/
class JsonObject: public Json{
public:
    JsonObject():Json(0){
        
    }
	/**
	* adds an nested jsonObject|JsonArray to the output buffer
	**/
    void addEntry(char* name,Json& obj){
        char tmp[ENTRY_BUFFER_SIZE];//create a buffer for the formatted string (char*)
        sprintf(tmp,"\"%s\":",name);//format the string 

        addCommer();//adds an , if this is not the first item
        
        strOutput(tmp);//add to the output buffer
        strOutput(obj.getBuffer()); //add to the output buffer        
    }
	/**
	*	adds a named entry to the output buffer
	**/
    void addEntry(char* name,char* value){
        char tmp[ENTRY_BUFFER_SIZE];//create a buffer for the formatted string (char*)
        sprintf(tmp,"\"%s\":\"%s\"",name,value);//format the string
        
        addCommer();//adds an , if this is not the first item
        
        strOutput(tmp);//add to the output buffer             
    }    
};
/**
	JsonArray class output char*, ["value",...]
**/
class JsonArray: public Json{
public:
    JsonArray():Json(1){//brace type 2
        
    }
	/**
	* adds an nested jsonObject|JsonArray to the output buffer
	**/
    void addEntry(Json& obj){
        addCommer();//adds an , if this is not the first item
        strOutput(obj.getBuffer());//add to the output buffer
    }
	/**
	* add an entry of type char* 
	**/
    void addEntry(char* value){
        addCommer();//adds a , if this is not the first item
        char tmp[ENTRY_BUFFER_SIZE];//create a buffer for the formatted string (char*)
        sprintf(tmp,"\"%s\"",value); //format the string       
        strOutput(tmp);//add to output buffer
    }
};