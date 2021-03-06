/**
* @author Rhys Bryant <rhys@voltprojects.com>
* basic command processing support for Arduino
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
#include "Json.h"
//#include <stdio.h>
//#include <iostream>
//#include <string>
//#include <sstream>
#define COMAND_ARRAY_SIZE 18
#define COMMAND_CHANE_SIZE 5
#define CLIprocessor_H 1
#ifndef CLIprocessor_CPP
	#include "cliProcessor.cpp"
	char CliHelpers::tmp[10]="";//static vars
#endif