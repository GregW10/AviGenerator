//
// created by Gregor Hartl Watters on 17/07/2022
//

#ifndef AVI_GENERATOR_FUNCS
#define AVI_GENERATOR_FUNCS

#include <stdio.h>
#include <time.h>
#include <shlobj.h>

typedef enum {
    False, True
} bool;

size_t strlen_c(const char *str) {
    if (str == NULL || *str == 0) {
        return 0;
    }
    size_t count = 0;
    while (*str++) ++count;
    return count;
}

size_t strcat_c(char *destStr, const char *srcStr) {
    if (destStr == NULL || srcStr == NULL || *srcStr == 0 || *destStr == 0) {
        return 0;
    }
    size_t count = 0;
    while (*destStr++);
    --destStr;
    while ((*destStr++ = *srcStr++)) ++count;
    return count;
}

short strcmp_c(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL || *str1 == 0 || *str2 == 0) {
        return 0;
    }
    while (*str1 && *str2) {
        if (*str1++ != *str2++) {
            return *--str1 - *--str2;
        }
    }
    if ((!*str1 && *str2) || (*str1 && !*str2)) {
        return *str1 - *str2;
    }
    return 0;
}

char *getHomePath(bool reallocate) {
    char *retPath = malloc(PATH_MAX);
    if (SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, SHGFP_TYPE_CURRENT, retPath) != S_OK) {
        free(retPath);
        return NULL;
    }
    if (reallocate) {
        retPath = realloc(retPath, strlen_c(retPath) + 1);
    }
    return retPath;
}

char *getFormattedTime() {
    static char retStr[20] = {0};
    char *ptr = retStr;
    time_t t = time(NULL);
    char *tm = ctime(&t);
    char *ot = tm + 4;
    *ptr++ = *(tm + 8); *ptr++ = *(tm + 9); *ptr++ = '_';
    *ptr++ = *ot == 'O' || *ot == 'N' || *ot == 'D' ? '1' : '0';
    switch (*ot) {
        case 'J':
            if (*(tm + 5) == 'u') *ptr++ = *(tm + 6) == 'n' ? '6' : '7';
            else *ptr++ = '1';
            break;
        case 'F':
            *ptr++ = '2';
            break;
        case 'M':
            if (*(tm + 6) == 'r') *ptr++ = '3';
            else *ptr++ = '5';
            break;
        case 'A':
            if (*(tm + 5) == 'p') *ptr++ = '4';
            else *ptr++ = '8';
            break;
        case 'S':
            *ptr++ = '9';
            break;
        case 'O':
            *ptr++ = '0';
            break;
        case 'N':
            *ptr++ = '1';
            break;
        case 'D':
            *ptr++ = '2';
            break;
    }
    *ptr++ = '_'; ot = tm + 20;
    while ((*ptr++ = *ot++));
    --ptr;
    *--ptr = '_'; ot = tm + 11; char *end = tm + 19;
    while (ot < end) {
        *++ptr = *ot++;
    }
    return retStr;
}

#endif