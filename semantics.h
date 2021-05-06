#ifndef SEMANTICS_H
#define SEMANTICS_H
#include "token.h"
#include "node.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>


extern std::ofstream file;


using std::vector;

typedef struct symbol_table
{
	Token identifier;
	Token value;
	//Can add more expansion later...
} SymbolTable;



typedef struct var_hold
{
	std::string ID;
	std::string value;
};

//SymbolTable st_holder;


void push(Token tk);
void pop(int scope_begin);
//void build_stack();
int topOfStackD(Token tk);
bool var_exists(Token tk);
void show_stack();
void semantic_check(Node* node, int index);
void check_children(Node* node, int index);
void error_declared(std::string tokenString);

std::string get_temp_var();
void codeGen(Node* node, int index, std::string output_file_name);

#endif
