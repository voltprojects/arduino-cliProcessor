#include "Arduino.h"
//#include <stdio.h>
//#include <iostream>
//#include <string>
//#include <sstream>
#define COMAND_ARRAY_SIZE 18
#define COMMAND_CHANE_SIZE 5
#define CLIPROCESSER_H 1
#ifndef CLIPROCESSER_CPP
	#include "cliProcesser.cpp"
	char CliHelpers::tmp[10]="";//static vars
#endif