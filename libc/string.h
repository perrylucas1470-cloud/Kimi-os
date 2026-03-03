#ifndef STRING_H
#define STRING_H

#include "../include/types.h"

size_t strlen(const char* str);
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, const void* src, size_t num);
int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);

#endif
