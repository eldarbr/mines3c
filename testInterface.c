#include <stdio.h>
#include "Interface/Interface.h"


int main(void) {

    system("pause");

    ClearScreen();

    for (size_t i = 0; i != 12  ; ++i) {
        printf("%zu\n", i);
    }


    system("pause");


    ClearScreen();

    for (size_t i = 0; i != 12  ; ++i) {
        printf("%zu\n", i+30);
    }

    return 0;
}
