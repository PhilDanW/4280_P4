#include "semantics.h"


#define MAX_STACK 100

Token stack[MAX_STACK];

bool debug1 = false;
bool debugger2 = true;

int var_count = 0, scope = 0;  //initialize the var count and the level of scope

static int temp_var_count = 0;
/* total number of labels */
static int label_count = 0;
/* array to hold temp variables */
std::string temp_vars[MAX_STACK];
unsigned int current_temp_vars_num = 0;
unsigned int carry_labels_num = 0;
unsigned int current_labels_num = 0;

vector<var_hold> VARIABLE;

void codeGen(Node* node, int index, std::string output_file_name) {

    file.open("file.asm", std::ios_base::trunc);
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
        //file << "\tPUSH\n";
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
void semantic_check(Node* node, int index)
{
    
    if (node == nullptr)
        return;
    if (debug1) {
        if (node->name == "<vars>" || node->name == "<assign>") { std::cout << "Working on: " << std::setw(10) << std::left << node->name << "| Token: " << std::setw(9) << std::left << node->token_2.token_string << "| "; }  // for debugging
        else { std::cout << "Working on: " << std::setw(10) << std::left << node->name << "| Token: " << std::setw(9) << std::left << node->token_1.token_string << "| "; }  // for debugging
        show_stack();
    }


    if (node->name == "<program>")
    {
        int vars = 0;
        check_children(node, vars);
        //Program stopping point
        file << "STOP" << std::endl;


        for (unsigned int i = 0; i < VARIABLE.size(); i++) {
            file << VARIABLE[i].ID << " " << VARIABLE[i].value << std::endl;
        }

        for (int i = 0; i < MAX_STACK; i++)
        {
            if (temp_vars[i] != "")
                file << temp_vars[i] << " 0\n";
        }


    }


    else if (node->name == "<vars>")
    {
        int tos_distance = topOfStackD(node->token_2);
        scope = var_count;

        if (tos_distance == -1 || tos_distance > index){

            push(node->token_2);
            index++;
        }
        else if (tos_distance < index){
            error_declared(node->token_2.token_string);
            exit(EXIT_FAILURE);
        }

        if (node->child_1 != nullptr)
            semantic_check(node->child_1, index);
    }


    else if (node->name == "<block>"){
        unsigned int vars = 0;
        scope = var_count;

        check_children(node, vars);
        pop(scope);
    }


    else if (node->name == "<expr>"){
        if (node->token_1.token_ID == MINUS_TK){
            int vars_num = current_temp_vars_num++;
            file << "SUB T" << vars_num << std::endl;
            check_children(node, index);
        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, index);
    }


    else if (node->name == "<N>"){
        int vars_num = current_temp_vars_num++;
        file << "STORE T" << vars_num << std::endl;
        if (node->token_1.token_ID == SLASH_TK || node->token_1.token_ID == ASTERISK_TK){
            
            
            
            if (node->token_1.token_ID == SLASH_TK) {
                file << "DIV T" << vars_num << std::endl;
            }
            else if (node->token_1.token_ID == ASTERISK_TK) {
                file << "MULT T" << vars_num << std::endl;
            }
            check_children(node, index);
        }
        else if (node->child_1 != nullptr){
            
            semantic_check(node->child_1, index);
            
        }
            
    }


    else if (node->name == "<M>"){
        if (node->token_1.token_ID == ASTERISK_TK){
            check_children(node, index);
            file << "MULT -1" << std::endl;
        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, index);
    }


    else if (node->name == "<A>"){
        if (node->token_1.token_ID == PLUS_TK){
            int vars_num = current_temp_vars_num++;
            file << "ADD T" << vars_num << std::endl;
            check_children(node, index);

        }
        else if (node->child_1 != nullptr)
            semantic_check(node->child_1, index);
    }


    else if (node->name == "<R>"){
        if (node->token_1.token_ID == ID_TK){
            if (!var_exists(node->token_1)){
                error_declared(node->token_1.token_string);
                exit(EXIT_FAILURE);
            }
            file << "LOAD " << node->token_1.token_string << std::endl;
        }
        else if (node->child_1 != nullptr) {
            
            semantic_check(node->child_1, index);
        }
    }

    else if (node->name == "<out>")
    {
        if (node->child_1 != nullptr)
            semantic_check(node->child_1, index);
        std::string temp_var = get_temp_var();
        file << "STORE " << temp_var << "\n";
        file << "WRITE " << temp_var << "\n";
    }

    else if (node->name == "<in>"){
        if (!var_exists(node->token_2)){
            error_declared(node->token_1.token_string);
            exit(EXIT_FAILURE);
        }
        else {
            int var_location = var_exists(node->token_1);
            if (var_location == -1)
            {
                std::cout << "ERROR: " << node->token_1.token_string << " has not been declared in this scope" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string temp_var = get_temp_var();
            file << "\t\tREAD " << temp_var << "\n";
            file << "\t\tLOAD " << temp_var << "\n";
            file << "\t\tSTACKW " << var_location << "\n";
        }
    }

    else if (node->name == "<loop>") {
        int loop_num = current_labels_num++;
        int vars_num = current_temp_vars_num++;

        file << "L" << loop_num << ": NOOP" << std::endl;
        semantic_check(node->child_1, index);
        file << "STORE T" << vars_num << std::endl;
        semantic_check(node->child_3, index);
        file << "SUB T" << vars_num << std::endl;

        int exit_loop_num = current_labels_num;
        carry_labels_num = current_labels_num++;
        semantic_check(node->child_2, index);
        semantic_check(node->child_4, index);
        file << "BR L" << loop_num << std::endl;
        file << "L" << exit_loop_num << ": NOOP" << std::endl;
    }


    else if (node->name == "<assign>"){
        if (!var_exists(node->token_2)){
            error_declared(node->token_2.token_string);
            exit(EXIT_FAILURE);
        }
        if (node->child_1 != nullptr) {
            file << "STORE " << node->token_2.token_string << std::endl;
            semantic_check(node->child_1, index);

        }
    }
    else {
    check_children(node, index);
    }
}

// continues semantic check through children
void check_children(Node* node, int index)
{
    if (node->child_1 != nullptr)
        semantic_check(node->child_1, index);
    if (node->child_2 != nullptr)
        semantic_check(node->child_2, index);
    if (node->child_3 != nullptr)
        semantic_check(node->child_3, index);
    if (node->child_4 != nullptr)
        semantic_check(node->child_4, index);
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

std::string get_temp_var()
{
    /* create the temp_var name */
    std::string temp_var = "T" + std::to_string(temp_var_count + 1);
    /* put the temp_var in the temp_var array */
    temp_vars[temp_var_count] = temp_var;
    /* created new temp_var so increment the count of the variables */
    temp_var_count++;
    /* return the temp_var name */
    return temp_var;
}
