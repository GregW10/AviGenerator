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

    char test[50];

    strcpy_c(test, "Bro! What the fuck are you doing here?");

    printf("The string: %s\n", test);

    return 0;
}