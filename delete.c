//
// Created by mario on 18/07/2022.
//

#include "funcs.h"

int main() {

    char test[500] = {0};

    strcpy_c(test, "Bro! What the fuck are you doing here?");

    printf("Test before: %s\n\n", test);

    printf("Retval: %d\n", replaceWithTime(test, 70));

    printf("Test after: %s\n", test);

    for (size_t i = 0; i < 50; ++i) {
        printf("The char: %d\n", *(test + i));
    }

    printf("GetDeviceCaps() retval: %d\n", GetDeviceCaps(NULL, RC_STRETCHBLT));

    return 0;
}