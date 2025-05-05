#ifndef utils_h
#define utils_h

char** splitPayload(const char* payload, const char delimiter, const int maxCommands);
bool startMatches(const char* str1, const char * str2);
char* removeWhiteSpace(char* str);

#endif