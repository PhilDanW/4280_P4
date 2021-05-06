/*********************************************************
* Author: Philip Wright                                  *
* Project 3: Semantics                                   *
* parseTree takes the tokens from the input file or user *
* and builds the parse tree by scanning each token       *
* and defining the rules for the BNF in the functions    * 
* given below. It then prints the parse tree to screen   *
**********************************************************/
#include "parseTree.h"
#include "scanner.h"
#include <iostream>

std::ifstream inputFile;
unsigned int lineNumber = 1;
Token the_token;

std::string myTokens[] = {"BEGIN_KEYWORD","END_KEYWORD","LOOP_KEYWORD","WHILE_KEYWORD", "VOID_KEYWORD", "EXIT_KEYWORD", 
                        "GETTER_KEYWORD", "OUTTER_KEYWORD", "MAIN_KEYWORD", "IF_KEYWORD", "THEN_KEYWORD", "ASSIGN_KEYWORD", 
                        "DATA_KEYWORD", "PROC_KEYWORD", "EQUALS_OPERATOR", "EQUALS_LESS_OPERATOR", 
                        "EQUALS_GREATER_OPERATOR", "EQUALS_EQUALS_OPERATOR", "COLON_OPERATOR", "COLON_EQUALS_OPERATOR",
                        "ADD_OPERATOR", "SUBTRACT_OPERATOR", "ASTERISK_OPERATOR", "DIVIDE_OPERATOR", "MODULUS_OPERATOR", 
                        "PERIOD_OPERATOR", "LEFT_PARENT_DELIM", "RIGHT_PARENT_DELIM", "COMMA_DELIM", "LEFT_BRACE_DELIM", 
                        "RIGHT_BRACE_DELIM", "SEMICOLON_DELIM", "LEFT_BRACKET_DELIM", "RIGHT_BRACKET_DELIM", "IDENTIFIER", 
                        "INTEGER", "EOF_TOKEN", "ERROR"
};

treeNode* parser() {
    treeNode* rootNode;
    the_token = scanner(inputFile, lineNumber);
  
    rootNode = program();
    if(the_token.tokenID == EOF_TOKEN) {
        std::cout << "Parser is done with its job!! You did it!!" << std::endl;
        return rootNode;
    }
    else
        errorSingle(EOF_TOKEN, the_token);
}

treeNode* program() {
    int nodeDepth = 0;
    treeNode* progNode = new treeNode("<program>", nodeDepth);
    progNode->leaf1 = vars(nodeDepth);
    if(the_token.tokenID == MAIN_KEYWORD) {
        progNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        progNode->leaf2 = block(nodeDepth);
    }
    else
        errorSingle(MAIN_KEYWORD, the_token);
    return progNode;
}

treeNode* block(unsigned int nodeDepth) {
    nodeDepth++;
    treeNode* blockNode = new treeNode("<block>", nodeDepth);
    if(the_token.tokenID == BEGIN_KEYWORD) {
        blockNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        blockNode->leaf1 = vars(nodeDepth);
        blockNode->leaf2 = stats(nodeDepth);
      
        if(the_token.tokenID == END_KEYWORD) {
            blockNode->token2 = the_token;
            the_token = scanner(inputFile, lineNumber);
            return blockNode;
        }
        else
            errorSingle(END_KEYWORD, the_token);
    }
    else
        errorSingle(BEGIN_KEYWORD, the_token);
}

treeNode* vars(unsigned int nodeDepth) {
    nodeDepth++;
    treeNode* varNode = new treeNode("<vars>", nodeDepth);
  
    if(the_token.tokenID == DATA_KEYWORD) 
    {
          varNode->token1 = the_token;
          the_token = scanner(inputFile, lineNumber);
          if (the_token.tokenID == IDENTIFIER) 
          {
                varNode->token2 = the_token;
                the_token = scanner(inputFile, lineNumber);
                if (the_token.tokenID == COLON_EQUALS_OPERATOR)
                {
                      varNode->token3 = the_token;
                      the_token = scanner(inputFile, lineNumber);
                      if (the_token.tokenID == INTEGER)
                      {
                            varNode->token4 = the_token;
                            the_token = scanner(inputFile, lineNumber);
                            if (the_token.tokenID == SEMICOLON_DELIM)
                            {
                                  varNode->token5 = the_token;
                                  the_token = scanner(inputFile, lineNumber);
                                  varNode->leaf1 = vars(nodeDepth);
                                  return varNode;
                            }
                            else {
                                errorSingle(SEMICOLON_DELIM, the_token);
                            }
                      }
                      else
                          errorSingle(INTEGER, the_token);
                }
                else
                    errorSingle(COLON_EQUALS_OPERATOR, the_token);
          }
          else
              errorSingle(IDENTIFIER, the_token);
    }
    else
        return NULL;
}

treeNode* expr(unsigned int nodeDepth) {
    nodeDepth++;
    treeNode* expNode = new treeNode("<expr>", nodeDepth);
    expNode->leaf1 = N(nodeDepth);

    if(the_token.tokenID == SUBTRACT_OPERATOR) {
       expNode->token1 = the_token;
       the_token = scanner(inputFile, lineNumber);
       expNode->leaf2 = expr(nodeDepth);
       return expNode;
    }
    else
        return expNode;
}

treeNode* N(unsigned int nodeDepth) {
   nodeDepth++;
   treeNode* nNode = new treeNode("<N>", nodeDepth);
   nNode->leaf1 = A(nodeDepth);
  
   if(the_token.tokenID == DIVIDE_OPERATOR) {
      nNode->token1 = the_token;
      the_token = scanner(inputFile, lineNumber);
      nNode->leaf2 = N(nodeDepth);
      return nNode;
   }
   else if (the_token.tokenID == ASTERISK_OPERATOR) {
     nNode->token1 = the_token;
     the_token = scanner(inputFile, lineNumber);
     nNode->leaf2 = N(nodeDepth);
     return nNode;
   }
   else
      return nNode;
}

treeNode* A(unsigned int nodeDepth) {
    nodeDepth++;
    treeNode* aNode = new treeNode("<A>", nodeDepth);
    aNode->leaf1 = M(nodeDepth);
    if (the_token.tokenID == ADD_OPERATOR)
    {
        /* store operator */
        aNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        aNode->leaf2 = A(nodeDepth);
        return aNode;
    }
    else { //4-11 fix
        return aNode;
    }// 4-11 fix
        
}

treeNode* M(unsigned int nodeDepth)
{
    nodeDepth++;
    treeNode* mNode = new treeNode("<M>", nodeDepth);
    if (the_token.tokenID == ASTERISK_OPERATOR)
    {
        /* strore operator */
        mNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber); 
        mNode->leaf1 = M(nodeDepth);
        return mNode;
    }
    else
    {
        mNode->leaf1 = R(nodeDepth);
        return mNode;        
    }
}

treeNode* R(unsigned int nodeDepth)
{
    nodeDepth++;
    treeNode* rNode = new treeNode("<R>", nodeDepth);
  
    if (the_token.tokenID == LEFT_PARENT_DELIM)
    {
        rNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        rNode->leaf1 = expr(nodeDepth);
        
        if (the_token.tokenID == RIGHT_PARENT_DELIM)
        {
            rNode->token2 = the_token;
            the_token = scanner(inputFile, lineNumber);
            return rNode;
        }
        else
            errorSingle(RIGHT_PARENT_DELIM, the_token);
    }
    else if (the_token.tokenID == IDENTIFIER)
    {
        rNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        return rNode;
    }
    else if (the_token.tokenID == INTEGER)
    {
        rNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        return rNode;
    }
    else
        errorTriple(LEFT_PARENT_DELIM, IDENTIFIER, INTEGER, the_token);
}

treeNode* stats(unsigned int nodeDepth) {
    nodeDepth++;
    treeNode* statNode = new treeNode("<stats>", nodeDepth);
    statNode->leaf1 = stat(nodeDepth);
    statNode->leaf2 = mstat(nodeDepth);
    return statNode;
}

treeNode* mstat(unsigned int nodeDepth) {
    nodeDepth++;
    treeNode* mstatNode = new treeNode("<mstat>", nodeDepth);
    
    if(the_token.tokenID == GETTER_KEYWORD || the_token.tokenID == OUTTER_KEYWORD || the_token.tokenID == BEGIN_KEYWORD || the_token.tokenID == IF_KEYWORD ||
       the_token.tokenID == LOOP_KEYWORD || the_token.tokenID == PROC_KEYWORD || the_token.tokenID == ASSIGN_KEYWORD || the_token.tokenID == VOID_KEYWORD)
    {
        mstatNode->leaf1 = stat(nodeDepth);
        mstatNode->leaf2 = mstat(nodeDepth);
        return mstatNode;
    }
    else
        return mstatNode;
}

treeNode* stat(unsigned int nodeDepth) {
    nodeDepth++;
    treeNode* statNode = new treeNode("<stat>", nodeDepth);
  
    if(the_token.tokenID == GETTER_KEYWORD) {
        statNode->leaf1 = In(nodeDepth);
      
        if(the_token.tokenID == SEMICOLON_DELIM) {
            statNode->token1 = the_token;
            the_token = scanner(inputFile, lineNumber);
            return statNode;
        }
        else
            errorSingle(SEMICOLON_DELIM, the_token);
    }
    else if(the_token.tokenID == OUTTER_KEYWORD) {
        statNode->leaf1 = Out(nodeDepth);
        
        if(the_token.tokenID == SEMICOLON_DELIM) {
            statNode->token1 = the_token;
            the_token = scanner(inputFile, lineNumber);
            return statNode;
        }
        else
            errorSingle(SEMICOLON_DELIM, the_token);
    }
    else if(the_token.tokenID == BEGIN_KEYWORD) {
        statNode->leaf1 = block(nodeDepth);
        return statNode;
    }
    else if(the_token.tokenID == IF_KEYWORD) {
        statNode->leaf1 = If(nodeDepth);
          
        if(the_token.tokenID == SEMICOLON_DELIM) { 
            statNode->token1 = the_token;
            the_token = scanner(inputFile, lineNumber);
            return statNode;
        }
        else
            errorSingle(SEMICOLON_DELIM, the_token);
    }
    else if(the_token.tokenID == LOOP_KEYWORD) {
        statNode->leaf1 = loop(nodeDepth);
      
        if(the_token.tokenID == SEMICOLON_DELIM) {
            statNode->token1 = the_token;
            the_token = scanner(inputFile, lineNumber);
            return statNode;
        }
        else
            errorSingle(SEMICOLON_DELIM, the_token);
    }
    else if(the_token.tokenID == ASSIGN_KEYWORD) {
        statNode->leaf1 = assign(nodeDepth);
        
        if(the_token.tokenID == SEMICOLON_DELIM) {
            statNode->token1 = the_token;
            the_token = scanner(inputFile, lineNumber);
            return statNode;
        }
        else
            errorSingle(SEMICOLON_DELIM, the_token);
    }
    else
        errorN(the_token);
}

treeNode* In(unsigned int nodeDepth) {
    nodeDepth++;
    treeNode* inNode = new treeNode("<in>", nodeDepth);
    if(the_token.tokenID == GETTER_KEYWORD) {
        the_token = scanner(inputFile, lineNumber);
        
        if(the_token.tokenID == IDENTIFIER) {
            inNode->token1 = the_token;
            the_token = scanner(inputFile, lineNumber);
            return inNode;
        }
        else
            errorSingle(IDENTIFIER, the_token);
    }
    else
        errorSingle(GETTER_KEYWORD, the_token);
}

treeNode* Out(unsigned int nodeDepth) {
    nodeDepth++;
    treeNode* outNode = new treeNode("<out>", nodeDepth);
  
    if(the_token.tokenID == OUTTER_KEYWORD) {
        the_token = scanner(inputFile, lineNumber);
        outNode->leaf1 = expr(nodeDepth);
        return outNode;
    }
    else
        errorSingle(OUTTER_KEYWORD, the_token);
}

treeNode* If(unsigned int nodeDepth) {
    nodeDepth++;
    treeNode* ifNode = new treeNode("<if>", nodeDepth);
    if (the_token.tokenID == IF_KEYWORD)
    {
        ifNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        if (the_token.tokenID == LEFT_BRACKET_DELIM)
        {
            ifNode->token2 = the_token;
            the_token = scanner(inputFile, lineNumber); 
            ifNode->leaf1 = expr(nodeDepth);
            ifNode->leaf2 = RO(nodeDepth);
            ifNode->leaf3 = expr(nodeDepth);
            if (the_token.tokenID == RIGHT_BRACKET_DELIM)
            {
                ifNode->token3 = the_token;
                if (the_token.tokenID == THEN_KEYWORD)
                {
                    ifNode->token4 = the_token;
                    the_token = scanner(inputFile, lineNumber); 
                    ifNode->leaf4 = stat(nodeDepth);
                    return ifNode;
                }
                else
                    errorSingle(THEN_KEYWORD, the_token);
            }
            else
                errorSingle(RIGHT_BRACKET_DELIM, the_token);
        }
        else
            errorSingle(LEFT_BRACKET_DELIM, the_token);
    }
    else
        errorSingle(LOOP_KEYWORD, the_token);
}

treeNode* loop(unsigned int nodeDepth)
{
    nodeDepth++;
    treeNode* loopNode = new treeNode("<loop>", nodeDepth);
    if(the_token.tokenID == LOOP_KEYWORD)
    {
        loopNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        if(the_token.tokenID == LEFT_BRACKET_DELIM)
        {
            loopNode->token2 = the_token;
            the_token = scanner(inputFile, lineNumber);
            loopNode->leaf1 = expr(nodeDepth);
            loopNode->leaf2 = RO(nodeDepth);
            loopNode->leaf3 = expr(nodeDepth);
            if(the_token.tokenID == RIGHT_BRACKET_DELIM)
            {
                loopNode->token3 = the_token;
                the_token = scanner(inputFile, lineNumber);
                loopNode->leaf4 = stat(nodeDepth);
                return loopNode;
            }
            else
                errorSingle(RIGHT_BRACKET_DELIM, the_token);
        }
        else
            errorSingle(LEFT_BRACKET_DELIM, the_token);
    }
    else
        errorSingle(LOOP_KEYWORD, the_token);
}

treeNode* assign(unsigned int nodeDepth)
{
    nodeDepth++;
    treeNode* assignNode = new treeNode("<assign>", nodeDepth);
    
    if (the_token.tokenID == ASSIGN_KEYWORD)
    {
        the_token = scanner(inputFile, lineNumber);
        if (the_token.tokenID == IDENTIFIER)
        {
            assignNode->token1 = the_token;
            the_token = scanner(inputFile, lineNumber);
            if (the_token.tokenID == COLON_EQUALS_OPERATOR)
            {
                the_token = scanner(inputFile, lineNumber);
                assignNode->leaf1 = expr(nodeDepth);
                return assignNode;
            }
            else
                errorSingle(COLON_EQUALS_OPERATOR, the_token);
        }
        else
            errorSingle(IDENTIFIER, the_token);
    }
    else
        errorSingle(ASSIGN_KEYWORD, the_token);
}

treeNode* RO(unsigned int nodeDepth)
{
    nodeDepth++;
    treeNode* roNode = new treeNode("<RO>", nodeDepth);

    if (the_token.tokenID == EQUALS_LESS_OPERATOR)
    {
        roNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        return roNode;
    }

    else if (the_token.tokenID == EQUALS_GREATER_OPERATOR)
    {
        roNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        return roNode;
    }
    else if (the_token.tokenID == EQUALS_EQUALS_OPERATOR)
    {
        roNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        return roNode;
    }
    else if (the_token.tokenID == DIVIDE_OPERATOR)
    {
        roNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        return roNode;
    }
    else if (the_token.tokenID == LEFT_BRACKET_DELIM)
    {
        roNode->token1 = the_token;
        the_token = scanner(inputFile, lineNumber);
        if (the_token.tokenID == EQUALS_EQUALS_OPERATOR)
        {
            roNode->token2 = the_token;
            the_token = scanner(inputFile, lineNumber);
            if (the_token.tokenID == RIGHT_BRACKET_DELIM)
            {
                // take any 3 tokens
                roNode->token3 = the_token;
                the_token = scanner(inputFile, lineNumber); 
                roNode->token4 = the_token;
                the_token = scanner(inputFile, lineNumber);
                roNode->token5 = the_token;
                the_token = scanner(inputFile, lineNumber);
                roNode->token6 = the_token;
                return roNode;
            }
            else
                errorSingle(RIGHT_BRACKET_DELIM, the_token);
        }
        else
            errorSingle(EQUALS_EQUALS_OPERATOR, the_token);
    }
    else
        errorN(the_token);
}

treeNode* label(unsigned int nodeDepth)
{
    nodeDepth++;
    treeNode* labelNode = new treeNode("<label>", nodeDepth);
    if (the_token.tokenID == VOID_KEYWORD)
    {
        the_token = scanner(inputFile, lineNumber);
        if (the_token.tokenID == IDENTIFIER)
        {
            labelNode->token1 = the_token;
            the_token = scanner(inputFile, lineNumber);
            return labelNode;

        }
        else
            errorSingle(IDENTIFIER, the_token);
    }
    else
        errorSingle(VOID_KEYWORD, the_token);
}

treeNode* Goto(unsigned int nodeDepth)
{
    nodeDepth++;
    treeNode* gtNode = new treeNode("<goto>", nodeDepth);
    if (the_token.tokenID == PROC_KEYWORD)
    {
        the_token = scanner(inputFile, lineNumber);
        if (the_token.tokenID == IDENTIFIER)
        {
            gtNode->token1 = the_token;
            the_token = scanner(inputFile, lineNumber);
            return gtNode;
        }
        else
            errorSingle(IDENTIFIER, the_token);
    }
    else
        errorSingle(PROC_KEYWORD, the_token);
}

//error for single token
void errorSingle(tokens needed, Token have)
{
    std::cout << "ERROR" << std::endl;
    std::cout << "Needed token: " << myTokens[needed] << std::endl;
    std::cout << "But have token: " << myTokens[have.tokenID];
    std::cout << " LINE NUMBER: " << have.lineNum<< std::endl;
    exit(EXIT_FAILURE);
}

// error for 2 tokens
void errorDouble(tokens needed1, tokens needed2, Token have)
{
    std::cout << "ERROR" << std::endl;
    std::cout << "Needed tokens: " << myTokens[needed1] << " or " << myTokens[needed2] << std::endl;
    std::cout << "But have token: " << myTokens[have.tokenID];
    std::cout << " LINE NUMBER: " << have.lineNum << std::endl;
    exit(EXIT_FAILURE);
}

// error for 3 tokens
void errorTriple(tokens needed1, tokens needed2, tokens needed3, Token have)
{
    std::cout << "ERROR" << std::endl;
    std::cout << "Needed tokens: " << myTokens[needed1] << ", " << myTokens[needed2];
    std::cout << ", or " << myTokens[needed3] << std::endl;
    std::cout << "But have token: " << myTokens[have.tokenID];
    std::cout << " LINE NUMBER: " << have.lineNum << std::endl;
    exit(EXIT_FAILURE);
}
// multiple tokens (4+) error
void errorN(Token recieved)
{
    std::cout << "Token broke the rules: " << myTokens[recieved.tokenID];
    std::cout << " LINE NUMBER: " << recieved.lineNum << std::endl;
    exit(EXIT_FAILURE);
}

void printTree(treeNode* node)
{
    if(node == nullptr)
    {
        std::cout << "Empty node" << std::endl;
        return;
    }
    else
    {
        std::string treeSpace((node->nodeDepth * 2), ' ');
        std::cout << treeSpace;
        std::cout << node->label << "  ";
        std::cout << node->token1.token_string << " " << node->token2.token_string << " " << node->token3.token_string << " " << node->token4.token_string << " " << node->token5.token_string << " " << node->token6.token_string << std::endl;

        if(node->leaf1 != nullptr)
            printTree(node->leaf1);
        if(node->leaf2 != nullptr)
            printTree(node->leaf2);
        if(node->leaf3 != nullptr)
            printTree(node->leaf3);
        if(node->leaf4 != nullptr)
            printTree(node->leaf4);
    }
    return;
}
