#include "Interface.h"


void ClearScreen(void) {
    RepositionCursor(1, 1);
    printf("\033[J");
}

void RepositionCursor(size_t x, size_t y) {
    if (x < 1 || y < 1) {
        Exception("ERR: illegal argument (Interface.RepositionCursor)", 1);
    }
    printf("\033[%zu;%zuH", x, y);
}


void Exception(const char message[], int q) {
    printf("%s", message);
    exit(q);
}
