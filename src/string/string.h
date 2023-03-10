#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdbool.h>

size_t strlen(const char* ptr);
size_t strnlen(const char* str, int max);
bool isdigit(char c);
int tonumericdigit(char c);

#endif