#include "string.h"

char tolower(char c)
{
    if (c >= 65 && c <= 90) 
    {
        c += 32;
    }

    return c;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while(str[len])
    {
        len++;
    }
    return len;
}


size_t strnlen(const char* str, int max)
{
    size_t i = 0;
    for (i = 0; i < max; i++)
    {
        if (str[i] == '\0') 
        {
            break;
        }
    }
    return i;
}

size_t strnlen_terminator(const char* str, int max, char terminator)
{
    size_t i = 0;
    for (i = 0; i < max; i++)
    {
        if (str[i] == '\0' || str[i] == terminator) 
        {
            break;
        }
    }
    return i;
}

int strncmp(const char* str1, const char* str2, int n)
{
    unsigned char u1, u2;

    while (n-- > 0)
    {
        u1 = (unsigned char)*str1++;
        u2 = (unsigned char)*str2++;
        if (u1 != u2)
        {
            return u1 - u2;
        }
        if (u1 == '\0')
        {
            return 0;
        }
    }
    return 0;
}

int istrncmp(const char* str1, const char* str2, int n)
{
    unsigned char u1, u2;

    while (n-- > 0)
    {
        u1 = (unsigned char)*str1++;
        u2 = (unsigned char)*str2++;
        if (u1 != u2 && tolower(u1) != tolower(u2))
        {
            return u1 - u2;
        }
        if (u1 == '\0')
        {
            return 0;
        }
    }
    return 0;
}

char* strcpy(char* dest, const char* src)
{
    char* res = dest;
    while(*src != 0)
    {
        *(dest++) = *(src++);
    }

    *dest = 0x00;
    
    return res;
}

bool isdigit(char c)
{
    return c >= '0' && c <= '9';

}
int tonumericdigit(char c)
{
    return c - '0';
}