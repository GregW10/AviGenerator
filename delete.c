//
// Created by mario on 18/07/2022.
//

#include "funcs.h"

int main() {

    char *one = "Harry";
    char *two = "Harry";

    printf("Compare: %d\n", strcmp_c(one, two));

    printf("time: %s\n", getFormattedTime());

    return 0;
}