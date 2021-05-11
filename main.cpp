/*********************************************************
* Author: Philip Wright                                  *
* Project 4: Code Generation                             *
* main.cpp processes the command line arguments (a file) *
* or will take keyboard input if no file is given.       *
* After opening the file or taking keyboard input it     * 
* will call parser() to scan and build the parse tree    *
* then the parsed tokens are put thru semantics check    *
*********************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include "parseTree.h"
#include "semantics.h"

std::ofstream file;

int main(int argc, char** argv) {
    std::string file_name;         // filename
    std::string file_n;

    //no file given so take user input from the keyboard
    if (argc == 1) {
        //create variables necessary for user input
        std::string input;
        std::ofstream tempFile;                 
        file = "stdin.temp";

        //use trunc to overwrite the file every time the program is run
        tempFile.open(file_name, std::ios::trunc);
        // empty string for reading input
        std::string string = "";                
        std::cout << "Pressing \"Enter\" is the same as EOF" << std::endl;

        do {
            std::cout << std::endl << "Type your input here: ";
            // read user input
            getline(std::cin, input);  
            // write input to temp file
            tempFile << input << "\n";  
        // Pressing "Enter" on empty line will sim EOF    
        } while (!input.empty());         
        // close file
        tempFile.close();                   
        inputFile.open(file_name);
        out_file_name = "kb.asm"
    }

    // File given on the command line
    else if (argc == 2) {
        file_name = argv[1];
        file_n = argv[1];
        file_name += ".sp21";
      
        out_file_name = file_n + ".asm";

        //open the file and prepare it for parsing
        std::ofstream outfile;
        outfile.open(file_name, std::ios_base::app);
        outfile << " ";
        inputFile.open(file_name);
    }
    //if there is more than one argument, quit the program
    else {
        std::cout << "Only one argument supported, you entered " << argc << "arguments." << std::endl;
        exit(EXIT_FAILURE);
    }

    //Try to open the file to begin the parsing process
    if (!inputFile) {
        std::cout << "ERROR: Could not open " << file << " for reading" << std::endl;
        inputFile.close();
        exit(EXIT_FAILURE);
    }
    //parse the tokens in the file and then print the tree to the screen
    else { 
        std::cout << "Beginning to build the Code Generation" << std::endl;
        treeNode* root = parser(); // run parser
        //printTree(root); // print the tree
        //semantic_check(root, 0);      // check parsed tokens semantics
        codeGen(root, 0, out_file_name); // check semantics here now
        inputFile.close(); // close file    
    }
    return 0;
}
