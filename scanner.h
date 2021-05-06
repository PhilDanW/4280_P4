/***********************************************************
* Author: Philip Wright                                    *
* Project 4: Code Generation                               *
* scanner.h holds function definitions for scanner.cpp     *
***********************************************************/
#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"
#include <string>
#include <map>
#include <fstream>

// scanner
Token scanner(std::ifstream& in_file,unsigned int& lineNum);

// finds col of char in FSA table
int setFSAcol(char current_char);

// sets token
Token setToken(int state, std::string word, unsigned int lineNum);

#endif
