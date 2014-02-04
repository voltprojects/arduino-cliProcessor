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
#include "Arduino.h"

#define ENTRY_BUFFER_SIZE 50
#define JSON_OBJECT_BUFFER_SIZE 80

#define JSON_H 1
#ifndef JSON_CPP
	#include "Json.cpp"
#endif