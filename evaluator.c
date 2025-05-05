
#include <utils.h>
#include <stdlib.h>

//defines what a variable can be.
struct variable {
    char* name;
    char* instr; //the instruction the variable represents.
};

struct variable* env[50]; //environment. For now not a jlist but a fixed size array.
int envPointer = 0; //points to next empty space.


void handlePayload(const char* payload) {
    //split payload into commands
    char** commands = splitPayload(payload, ';', 50);

    //go through and execute each command
    int i = 0;
    while (commands[i] != NULL) {
        printf("Executing command: %s\n", commands[i]);
        identifyAndExecuteCommand(commands[i]); // Execute the command
        i++;
    }
}

//ids and executes a command.
char** identifyAndExecuteCommand(const char* command){
    if(strcmp(command, "LED_ON") == 0){
        LED_ON_Command(); // Turn the LED on
    }
    else if(strcmp(command, "LED_OFF") == 0){
        LED_OFF_Command();
    }
    else if (startMatches(command, "LET")){
        //set var
    }
    else{
        printf("Unknown command: %s\n", command);
    }
}




//sets a variable.
char** setVariable(const char* command){
    //get the 'other side' of the var
    char* varParts = strtok(command, "=");
    char* varName = varParts[0];
}

//youll never guess what this does
void LED_ON_Command(){
    pico_set_led(true); // Turn the LED on
}

void LED_OFF_Command(){
    pico_set_led(false); // Turn the LED off
}

void setVariable(const char* command, struct variable** env, int envPointer){
    //isolate variable.
    //LET <varname> = func
    int startOfVarName = 3; //skips space and LET

    //skip any extra white space
    while(command[startOfVarName] == ' '){
        startOfVarName++; 
    }
    
    //find end of var name.
    int lengthOfVarName = 0;
    char* buffer = malloc(strlen(command) + 1);
    
    while(command[startOfVarName] != ' ' && command[startOfVarName] != '='){
        buffer[lengthOfVarName] = command[startOfVarName];
        lengthOfVarName++;
        startOfVarName++;
    }
    buffer[lengthOfVarName + 1] = '\0'; //add end of string

    char* varName = malloc(lengthOfVarName + 1); // Allocate memory for the variable name
    copyString(varName, buffer);
    free(buffer);

    //get variable value.
    char* valBuffer = malloc(strlen(command) + 1);
    int startOfVarValue = startOfVarName + lengthOfVarName + 1; //skips '='
    int lengthOfVarValue = 0;

    //skip rededudent whitespace
    while(command[startOfVarValue] == ' '){
        startOfVarValue++; 
    }

    //find end of var value and do same as above.
    while(command[startOfVarValue] != '\0'){
        valBuffer[lengthOfVarValue] = command[startOfVarValue];
        lengthOfVarValue++;
        startOfVarValue++;
    }

    char* varValue = malloc(lengthOfVarValue + 1); // Allocate memory for the variable value
    copyString(varValue, valBuffer);
    free(valBuffer);


    //add the variable to the environment.
    struct variable* newVar = malloc(sizeof(struct variable)); // Allocate memory for the new variable
    newVar->name = varName;
    newVar->instr = varValue; // set name and instruction.
    env[envPointer] = newVar; // Add the variable to the environment
}