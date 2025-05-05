
#include <utils.h>
#include <stdlib.h>

//defines what a variable can be.
struct variable {
    char* name;
    char* type; //type of variable. NEEDS TO BE IMPLEMENTED.
    char* value; //the value.
};

//handle the incoming data.
void handlePayload(const char* payload) {

    //setup environment.
    const int envSize = 50;
    struct variable* env[envSize]; 
    int envPointer = 0;

    executeCommands(payload, env, envPointer);
}

void executeCommands(const char* commands, struct variable** env, int envPointer){
    //split commands by ';' and execute each one.
    char** commands = splitPayload(commands, ';', 50);

    free(commands); //no need to keep this in mem.

    //go through and execute each command
    int i = 0;
    while (commands[i] != NULL) {
        printf("Executing command: %s\n", commands[i]);
        identifyAndExecuteCommand(commands[i], env, envPointer); // Execute the command
        i++;
    }
}

//ids and executes a command.
char** identifyAndExecuteCommand(const char* command, struct variable** env, int envPointer){
    if(strcmp(command, "LED_ON") == 0){
        LED_ON_Command(); // Turn the LED on
    }
    else if(strcmp(command, "LED_OFF") == 0){
        LED_OFF_Command();
    }
    else if (startMatches(command, "LET")){
        setVariable(command, env, envPointer);
        envPointer++; //goto next space.
    }
    else if(startMatches(command, "BROADCAST")){

    }
    else if (startMatches(command, "REPEAT")){
        repeatCommand(command, env, envPointer);
    }
    else{
        printf("Unknown command: %s\n", command);
    }
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
    char* buffer = malloc(sizeof(char) * strlen(command) + 1);
    
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
    char* valBuffer = malloc(sizeof(char) * strlen(command) + 1);
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

    char* varValue = malloc(sizeof(char) * lengthOfVarValue + 1); // Allocate memory for the variable value
    copyString(varValue, valBuffer);
    free(valBuffer);


    //add the variable to the environment.
    struct variable* newVar = malloc(sizeof(struct variable)); // Allocate memory for the new variable
    newVar->name = varName;
    newVar->value = varValue; // set name and instruction.
    env[envPointer] = newVar; // Add the variable to the environment
}


void broadcastCommand(const char* command, struct variable** env, int envPointer){
}


void repeatCommand(const char* command, struct variable** env, int envPointer){
    //go to number of repeats
    int startOfNumber = 6; //skip "REPEAT "
    while(command[startOfNumber] == ' '){
        startOfNumber++; 
    }

    //go to start of open bracket.
    int startOfOpenBracket = 0;
    while(command[startOfOpenBracket] != '('){
        startOfOpenBracket++;
    }

    //get number of repeats.
    int numRepeats = 0;
    while(command[startOfNumber] != '(' && command[startOfNumber] != ' '){
        numRepeats = numRepeats * 10 + convertCharToInt(command[startOfNumber]);
        startOfNumber++;
    }

    //execute commands in brackets.
    //get commands
    int commandIndex = startOfOpenBracket + 1; //skip open bracket
    char* buffer = malloc(strlen(command) + 1);
    while(command[commandIndex] != ')'){
        buffer[commandIndex] = command[commandIndex];
        commandIndex++;
    }

    buffer[commandIndex] = '\0'; //add end of string
    char* toExecute = malloc(sizeof(char) * (commandIndex - startOfOpenBracket + 1));
    copyString(toExecute, buffer);
    free(buffer);

    //execute
    for(int i = 0; i < numRepeats; i++){

    }

}

int convertCharToInt(const char chr){
    // Convert the character to an integer value
    int num = chr - '0'; // Subtract the ASCII value of '0' to get the integer value
    
    //likely means its a variable.
    if(num < 0 || num > 9) {
        return -1;
    }
    
    return num;
}


char* readParameter(const char* command, int startOfParam, struct variable** env, int envPointer){
    //get where parameter ends
    int endOfParam = startOfParam;
    while(command[endOfParam] != ' ' && command[endOfParam] != '\0'){
        endOfParam++;
    }

    //return the param
    return partOfString(command, startOfParam, endOfParam);
}

int readNumber(const char* command, int startOfNum, struct variable** env, int envPointer){
    char* strNum = readParameter(command, startOfNum, env, envPointer);
    
    //read the number
    int total = 0;
    int i = 0;
    bool isVar = false;
    while(strNum[i] != '\0'){
        int num = convertCharToInt(strNum[i]);
        if(num == -1){ //not num probably a variable.
            isVar = true;
            break; 
        }
        total = total * 10 + num; //add to total
    }

    if(isVar){
        struct variable* var = readFromEnv(strNum, env, envPointer); //get variable from env.
        return readNumber(var->value, 0, env, envPointer); //read the number in the variable.
    }

    return total;
}


struct variable* readFromEnv(const char* varName, struct variable** env, int envPointer){
    //go through all variables and find the one with the same name.
    for(int i = 0; i < envPointer; i++){
        if(strEq(env[i]->name, varName) == 0){
            return env[i]; //return the variable.
        }
    }

    return NULL; //not found.
}



