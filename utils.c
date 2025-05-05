//removes whitespace from a string
char* removeWhiteSpace(char* str) {
    char* newStr = malloc(strlen(str) + 1); // Allocate memory for the new string
    int newPtr = 0;
    
    //add all non-whitespace characters to the new string.
    for(int oldPtr = 0; oldPtr < strlen(str); oldPtr++){
        if(str[oldPtr] != ' '){
            newStr[newPtr] = str[oldPtr]; 
            newPtr++;
        }
    }
    newStr[newPtr] = '\0'; //add end of string
    return newStr;
}


//checks if the start of a string matches another string.
bool startMatches(const char* str1, const char * str2) {
    
    //go through characters until there not equal.
    int i = 0;
    while(str1[i] == str2[i]){
        i++;
    }

    //if we have fully consumed the second string then str2 is a prefix of str1.
    if(str2[i] == '\0'){
        return true; 
    }
    else{
        return false; 
    }
}


//split string by delimiter.
//i have NO clue who made some of these C functions but they suck. why make the inbuilt split function not reentrant?
char** splitPayload(const char* payload, const char delimiter, const int maxCommands) {
    //split payload into commands
    char* command = strtok(payload, delimiter);
    char** commandList[maxCommands]; // Assuming a maximum of 50 commands

    //go through all commands and add them to the list.
    int i = 0;
    while (command != NULL && i < maxCommands) {
        commandList[i] = command;
        command = strtok(NULL, delimiter);
        i++;
    }

    commandList[i] = NULL;
    return commandList; // Return the list of commands
}

int strlen(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}


void copyString(char* dest, char* src) {
    int i = 0; 
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}


//gets the string between 2 indexes.
char* partOfString(char* string, int start, int end){
    // Allocate memory for the substring
    char* substring = malloc(end - start + 1); // +1 for null terminator

    // Copy the characters from the original string to the substring
    int j = 0;
    for (int i = start; i < end; i++) {
        substring[j] = string[i];
        j++;
    }
    substring[j] = '\0'; // Null-terminate the substring

    return substring; // Return the substring
}

//checks if 2 strings are equal.
int strEq(const char* str1, const char* str2){
    //go through characters until there not equal.
    int i = 0;
    while(str1[i] == str2[i] ){
        i++;
    }

    //no need for +1 as we check null terminator.
    if(i == strlen(str1) && i == strlen(str2)){
        return true; 
    }
    else{
        return false; 
    }
}