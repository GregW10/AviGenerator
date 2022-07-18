//
// Created by mario on 18/07/2022.
//

#include "funcs.h"

int main() {

    char *one = "Harry";
    char *two = "Harry";

    printf("Compare: %d\n", strcmp_c(one, two));

    char *str;

    printf("time: %s\n", str = getFormattedTime());

    while (*str) {
        printf("Char: %d\n", *str++);
    }

    return 0;
}