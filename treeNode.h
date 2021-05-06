/*********************************************************
* Author: Philip Wright                                  *
* Project 4: Code Generation                             *
* treeNode.h has the definition of the treeNode struct   *
*********************************************************/
#ifndef TREENODE_H
#define TREENODE_H

#include "token.h"

struct treeNode {
  
    //class variable definitions
    std::string label;
    int nodeDepth;
    
    Token token1;
    Token token2;
    Token token3;
    Token token4;
    Token token5;
    Token token6;
  
    treeNode* leaf1; 
    treeNode* leaf2;
    treeNode* leaf3;
    treeNode* leaf4;
  
    //class contructor
    treeNode() {
      
        this->label = "";
        this->nodeDepth = 0;
        this->token1 = Token();
        this->token2 = Token();
        this->token3 = Token();
        this->token4 = Token();
        this->token5 = Token();
        this->token6 = Token();
      
        this->leaf1 = nullptr;
        this->leaf2 = nullptr;
        this->leaf3 = nullptr;
        this->leaf4 = nullptr;
    }
  
    //class contructor definition
    treeNode(std::string label, int nodeDepth) {
          this->label = label;
          this->nodeDepth = nodeDepth;
          this->nodeDepth = 0;
          this->token1 = Token();
          this->token2 = Token();
          this->token3 = Token();
          this->token4 = Token();
          this->token5 = Token();
          this->token6 = Token();

          this->leaf1 = nullptr;
          this->leaf2 = nullptr;
          this->leaf3 = nullptr;
          this->leaf4 = nullptr;
    }
};

#endif
