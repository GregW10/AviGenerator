//
// created by Gregor Hartl Watters on 17/07/2022
//

#ifndef AVI_GENERATOR_FUNCS
#define AVI_GENERATOR_FUNCS

#ifdef _WIN32

#include <stdio.h>
#include <time.h>
#include <shlobj.h>

typedef enum {
    False, True
} bool;

size_t wcslen_c(const wchar_t *str) {
    if (str == NULL || *str == 0) {
        return 0;
    }
    size_t count = 0;
    while (*str++) ++count;
    return count;
}

size_t wcscat_c(wchar_t *destStr, const wchar_t *srcStr) {
    if (destStr == NULL || srcStr == NULL || *srcStr == 0 || *destStr == 0) {
        return 0;
    }
    size_t count = 0;
    while (*++destStr);
    while ((*destStr++ = *srcStr++)) ++count;
    return count;
}

size_t wcscpy_c(wchar_t *destStr, const wchar_t *srcStr) { // copies the null character
    if (destStr == NULL || srcStr == NULL || *srcStr == 0) {
        return 0;
    }
    size_t count = 0;
    while ((*destStr++ = *srcStr++)) ++count;
    return count;
}

short wcscmp_c(const wchar_t *str1, const wchar_t *str2) {
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

static inline wchar_t *getHomePath() {
    static wchar_t retPath[MAX_PATH];
    if (SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, SHGFP_TYPE_CURRENT, retPath) != S_OK) {
        return NULL;
    }
    return retPath;
}

const wchar_t *getFormattedTime() {
    static wchar_t retStr[20] = {0};
    wchar_t *ptr = retStr;
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

int replaceWithTime(wchar_t *str, size_t pos) {
    if (str == NULL) {
        return 1;
    }
    const wchar_t *time = getFormattedTime();
    size_t len;
    if ((len = wcslen_c(str)) < pos) {
        pos = len;
    }
    str += pos;
    while (*time) {
        *str++ = *time++;
    }
    if (len - pos < wcslen_c(time)) {
        *str = 0;
    }
    return 0;
}

void printAllChars(const wchar_t *str, size_t num) {
    if (str == NULL) {
        return;
    }
    if (num != 0) {
        num = -1;
        goto start;
    }
    if (*str == 0) {
        return;
    }
    size_t count = 0;
    start:
    while (*str || count++ <= num) wprintf(L"%lc", *str++);
    wprintf(L"\n");
}

long double getBMPheightToWidth(const wchar_t *path) {
    FILE *fp = _wfopen(path, L"rb");
    if (fp == NULL) {
        return 0;
    }
    unsigned startingAddressOffset;
    fseek(fp, 10, SEEK_SET);
    fread(&startingAddressOffset, sizeof(unsigned), 1, fp);
    if (startingAddressOffset != 54) {
        return 0;
    }
    unsigned width;
    unsigned height;
    fseek(fp, 18, SEEK_SET);
    fread(&width, sizeof(unsigned), 1, fp);
    fread(&height, sizeof(unsigned), 1, fp);
    fclose(fp);
    return ((long double) height) / ((long double) width);
}
#endif
#endif