/*********************************************************
* Author: Philip Wright                                  *
* Project 4: Code Generation                             *
* token.h has the enum values or the token types as well *
* as the struct definition of the Token type             *
*********************************************************/
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum tokens {
                        BEGIN_KEYWORD,END_KEYWORD,LOOP_KEYWORD,WHILE_KEYWORD, VOID_KEYWORD, EXIT_KEYWORD, 
                        GETTER_KEYWORD, OUTTER_KEYWORD, MAIN_KEYWORD, IF_KEYWORD, THEN_KEYWORD, ASSIGN_KEYWORD, 
                        DATA_KEYWORD, PROC_KEYWORD, EQUALS_OPERATOR, EQUALS_LESS_OPERATOR, 
                        EQUALS_GREATER_OPERATOR, EQUALS_EQUALS_OPERATOR, COLON_OPERATOR, COLON_EQUALS_OPERATOR,
                        ADD_OPERATOR, SUBTRACT_OPERATOR, ASTERISK_OPERATOR, DIVIDE_OPERATOR, MODULUS_OPERATOR, 
                        PERIOD_OPERATOR, LEFT_PARENT_DELIM, RIGHT_PARENT_DELIM, COMMA_DELIM, LEFT_BRACE_DELIM, 
                        RIGHT_BRACE_DELIM, SEMICOLON_DELIM, LEFT_BRACKET_DELIM, RIGHT_BRACKET_DELIM, IDENTIFIER, 
                        INTEGER, EOF_TOKEN, ERROR                  
};

struct Token {
    unsigned int lineNum;      // line number
    tokens tokenID;           // token ID
    std::string token_string;  // string of token

    Token(){
        this->tokenID = ERROR;
        this->token_string = "";
        this->lineNum = 0;
    }
    
    // Assign token info
    Token(tokens ID, std::string string, unsigned int lineNum)
    {   
        this->lineNum = lineNum;
        this->tokenID = ID;
        this->token_string = string;
    }
};

#endif
