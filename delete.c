//
// Created by mario on 18/07/2022.
//

#include "funcs.h"

int wmain() {

    wchar_t test[500] = {0};

    wcscpy_c(test, L"Bro! What the fuck are you doing here?");

    wprintf(L"Test before: %s\n\n", test);

    wprintf(L"Retval: %d\n", replaceWithTime(test, 70));

    wprintf(L"Test after: %s\n", test);

    for (size_t i = 0; i < 50; ++i) {
        wprintf(L"The char: %d\n", *(test + i));
    }

    wprintf(L"GetDeviceCaps() retval: %d\n", GetDeviceCaps(NULL, RC_STRETCHBLT));
    wprintf(L"%d\n", MAX_PATH);

    return 0;
}