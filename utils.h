#ifndef utils_h
#define utils_h

char** splitPayload(const char* payload, const char delimiter, const int maxCommands);
int startMatches(const char* str1, const char * str2);
char* removeWhiteSpace(char* str);
int strlen(const char* str);
void copyString(char* dest, const char* src);
char* partOfString(char* string, int start, int end);
int strEq(const char* str1, const char* str2);

#endif