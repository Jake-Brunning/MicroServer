


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




//defines what a variable can be.
struct variable {
    char* name;
    char* instr; //the instruction the variable represents.
};