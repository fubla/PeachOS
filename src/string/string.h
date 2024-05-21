#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdbool.h>

char tolower(char c);
size_t strlen(const char* ptr);
size_t strnlen(const char* str, int max);
size_t strnlen_terminator(const char* str, int max, char terminator);
int strncmp(const char* str1, const char* str2, int n);
int istrncmp(const char* str1, const char* str2, int n);
char* strcpy(char* dest, const char* src);
bool isdigit(char c);
int tonumericdigit(char c);

#endif