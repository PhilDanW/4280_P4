#include "semantics.h"


#define MAX_STACK 100

Token stack[MAX_STACK];

bool debug1 = false;
bool debugger2 = true;

int var_count = 0, scope = 0;  //initialize the var count and the level of scope

static int var_count_t = 0;
/* total number of labels */
static int label_count = 0;
/* array to hold temp variables */
std::string temp_vars[MAX_STACK];
unsigned int t_var_index = 0;
unsigned int loop_index = 0;

vector<var_hold> VARIABLE(0);

int var_ = 0;
int var_value = 0;

void codeGen(treeNode* node, int index, std::string output_file_name) {

    file.open(output_file_name, std::ios_base::trunc);
    semantic_check(node, index);  //checks semantics
    file.close();
}


// for pushing a new varibale onto stack, also checks if var already exists in scope
void push(Token tk) {
    if (var_count >= MAX_STACK){
        std::cout << "Stack has reached size of 100 \nExiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
    else{
        for (int i = scope; i < var_count; i++){
            if (stack[i].token_string == tk.token_string){
                std::cout << "Semantics error... \'" << tk.token_string << "\' has already been declared in this scope" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        stack[var_count] = tk; if (debugger2) { std::cout << "Adding \'" << tk.token_string << "\' to the stack\n"; }  // for debugging
        file << "PUSH" << std::endl;
        var_count++; if (debugger2) { std::cout << "VAR COUNT: " << var_count << "\n"; }  // for debugging
        //show_stack();
    }
}

// pops stack
void pop(int scope_start) {
    for (int i = var_count; i > scope_start; i--){
        if (debugger2) { std::cout << "Popping \'" << stack[i-1].token_string << "\' from the stack\n"; }  // for debugging
        stack[i-1].token_string = "";
        //file << "\tPOP\n";
        var_count--; if (debugger2) { std::cout << "VAR COUNT: " << var_count << "\n"; }  // for debugging
        //show_stack();
    }
}

// return the distance from top of stack
int topOfStackD(Token tk) {
    for (int i = var_count; i >= scope; i--){
        if (stack[i].token_string == tk.token_string){
            int tos_distance = var_count - 1 - i;
            return tos_distance;
        }
    }

    return -1;
}

// checks if variable already exists
bool var_exists(Token tk)
{
    for (int i = var_count - 1; i > -1; i--){
        if (stack[i].token_string == tk.token_string) return true;
    }
    return false;
}

// goes through parsetree to check semantics
void semantic_check(treeNode* node, int index)
{
    
    if (node == nullptr)
        return;
    if (debug1) {
        if (node->label == "<vars>" || node->label == "<assign>") { std::cout << "Working on: " << std::setw(10) << std::left << node->label << "| Token: " << std::setw(9) << std::left << node->token2.token_string << "| "; }  // for debugging
        else { std::cout << "Working on: " << std::setw(10) << std::left << node->label << "| Token: " << std::setw(9) << std::left << node->token1.token_string << "| "; }  // for debugging
        show_stack();
    }


    if (node->label == "<program>")
    {
        int vars = 0;
        if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, index);
        if (node->leaf2 != nullptr)
            semantic_check(node->leaf2, index);
        if (node->leaf3 != nullptr)
            semantic_check(node->leaf3, index);
        if (node->leaf4 != nullptr)
            semantic_check(node->leaf4, index);
        //Program stopping point
        file << "STOP" << std::endl;

        for (unsigned int i = 0; i < VARIABLE.size(); i++) {
            file << VARIABLE[i].ID << " " << VARIABLE[i].value << std::endl;
        }

        for (int i = 0; i < t_var_index; i++)
        {
            file << "T" << i << " 0\n";
        }
    }


    else if (node->label == "<vars>")
    {
        int tos_distance = topOfStackD(node->token2);
        scope = var_count;

        if (tos_distance == -1 || tos_distance > index){
            var_hold tempVar;
            tempVar.ID = node->token2.token_string;
            tempVar.value = node->token4.token_string;
            VARIABLE.resize(++var_, tempVar);
            push(node->token2);
            index++;
        }
        else if (tos_distance < index){
            error_declared(node->token2.token_string);
            exit(EXIT_FAILURE);
        }

        if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, index);
    }


    else if (node->label == "<block>"){
        unsigned int vars = 0;
        scope = var_count;

        if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, index);
        if (node->leaf2 != nullptr)
            semantic_check(node->leaf2, index);
        if (node->leaf3 != nullptr)
            semantic_check(node->leaf3, index);
        if (node->leaf4 != nullptr)
            semantic_check(node->leaf4, index);
        pop(scope);
    }


    else if (node->label == "<expr>"){
        if (node->token1.tokenID == SUBTRACT_OPERATOR){
            int vars_num = t_var_index++;
            file << "SUB T" << vars_num << std::endl;
            if (node->leaf1 != nullptr)
                semantic_check(node->leaf1, index);
            if (node->leaf2 != nullptr)
                semantic_check(node->leaf2, index);
            if (node->leaf3 != nullptr)
                semantic_check(node->leaf3, index);
            if (node->leaf4 != nullptr)
                semantic_check(node->leaf4, index);
        }
        else if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, index);
    }


    else if (node->label == "<N>"){
        int vars_num = t_var_index++;
        file << "STORE T" << vars_num << std::endl;
        if (node->token1.tokenID == DIVIDE_OPERATOR || node->token1.tokenID == ASTERISK_OPERATOR){

            if (node->token1.tokenID == DIVIDE_OPERATOR) {
                file << "DIV T" << vars_num << std::endl;
            }
            else if (node->token1.tokenID == ASTERISK_OPERATOR) {
                file << "MULT T" << vars_num << std::endl;
            }
            if (node->leaf1 != nullptr)
                semantic_check(node->leaf1, index);
            if (node->leaf2 != nullptr)
                semantic_check(node->leaf2, index);
            if (node->leaf3 != nullptr)
                semantic_check(node->leaf3, index);
            if (node->leaf4 != nullptr)
                semantic_check(node->leaf4, index);
        }
        else if (node->leaf1 != nullptr){
            semantic_check(node->leaf1, index);    
              
    }

    else if (node->label == "<M>"){
        if (node->token1.tokenID == ASTERISK_OPERATOR){
            if (node->leaf1 != nullptr)
                semantic_check(node->leaf1, index);
            if (node->leaf2 != nullptr)
                semantic_check(node->leaf2, index);
            if (node->leaf3 != nullptr)
                semantic_check(node->leaf3, index);
            if (node->leaf4 != nullptr)
                semantic_check(node->leaf4, index);
                file << "MULT -1" << std::endl;
        }
        else if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, index);
    }


    else if (node->label == "<A>"){
        if (node->token1.tokenID == ADD_OPERATOR){
            
            if (node->leaf1 != nullptr)
                semantic_check(node->leaf1, index);
            int vars_num = t_var_index++;
            
            file << "STORE T" << vars_num << std::endl;
            file << "ADD T" << vars_num << std::endl;
            
            if (node->leaf2 != nullptr)
                semantic_check(node->leaf2, index);
            if (node->leaf3 != nullptr)
                semantic_check(node->leaf3, index);
            if (node->leaf4 != nullptr)
                semantic_check(node->leaf4, index);
        }
        else if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, index);
    }


    else if (node->label == "<R>"){
        if (node->leaf1 != nullptr) {
        semantic_check(node->leaf1, index);
        }
        
        else if (node->token1.tokenID == IDENTIFIER){
            if (!var_exists(node->token1)){
                error_declared(node->token1.token_string);
                exit(EXIT_FAILURE);
            }
            file << "LOAD " << node->token1.token_string << std::endl;
        }
        else if (node->token1.tokenID == INTEGER) {
            file << "LOAD " << node->token1.token_string << std::endl;
        }
    }

    else if (node->label == "<out>")
    {
        if (node->leaf1 != nullptr)
            semantic_check(node->leaf1, index);
        std::string temp_var = make_t_var();
        file << "STORE " << temp_var << "\n";
        file << "WRITE " << temp_var << "\n";
    }

    else if (node->label == "<in>"){
        if (!var_exists(node->token2)){
            error_declared(node->token1.token_string);
            exit(EXIT_FAILURE);
        }
        else {
            int var_location = var_exists(node->token1);
            if (var_location == -1)
            {
                std::cout << "ERROR: " << node->token1.token_string << " has not been declared in this scope" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string temp_var = make_t_var();
            file << "\t\tREAD " << temp_var << "\n";
            file << "\t\tLOAD " << temp_var << "\n";
            file << "\t\tSTACKW " << var_location << "\n";
        }
    }

    else if (node->label == "<loop>") {
        int loop_num = loop_index++;
        int vars_num = t_var_index++;

        file << "L" << loop_num << ": NOOP" << std::endl;
        semantic_check(node->leaf1, index);
        file << "STORE T" << vars_num << std::endl;
        semantic_check(node->leaf3, index);
        file << "SUB T" << vars_num << std::endl;

        int exit_loop_num = loop_index;
        
        semantic_check(node->leaf2, index);
        semantic_check(node->leaf4, index);
        file << "BR L" << loop_num << std::endl;
        file << "L" << exit_loop_num << ": NOOP" << std::endl;
    }


    else if (node->label == "<assign>"){
        if (!var_exists(node->token2)){
            error_declared(node->token2.token_string);
            exit(EXIT_FAILURE);
        }
        if (node->leaf1 != nullptr) {
            file << "STORE " << node->token2.token_string << std::endl;
            semantic_check(node->leaf1, index);

        }
    }
    else {
            if (node->leaf1 != nullptr)
                semantic_check(node->leaf1, index);
            if (node->leaf2 != nullptr)
                semantic_check(node->leaf2, index);
            if (node->leaf3 != nullptr)
                semantic_check(node->leaf3, index);
            if (node->leaf4 != nullptr)
                semantic_check(node->leaf4, index);
    }
}

// continues semantic check through children
void check_children(treeNode* node, int index)
{
    if (node->leaf1 != nullptr)
        semantic_check(node->leaf1, index);
    if (node->leaf2 != nullptr)
        semantic_check(node->leaf2, index);
    if (node->leaf3 != nullptr)
        semantic_check(node->leaf3, index);
    if (node->leaf4 != nullptr)
        semantic_check(node->leaf4, index);
}

// error for declared variables
void error_declared(std::string tokenString){
    std::cout << "Semantics error... \'" << tokenString << "\' has not been declared in this scope." << std::endl;
}

// prints stack
void show_stack() {
    std::cout << "Stack: ";
    int i;
    for (i = 0; i < 100; i++) {
        std::cout << stack[i].token_string << "";
        if (stack[i].token_string == "")break;
    }

    std::cout << std::endl;
}

std::string make_t_var()
{
    /* create the temp_var name */
    std::string temp_var = "T" + std::to_string(var_count_t + 1);
    /* put the temp_var in the temp_var array */
    temp_vars[var_count_t] = temp_var;
    /* created new temp_var so increment the count of the variables */
    var_count_t++;
    /* return the temp_var name */
    return temp_var;
}
