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


void DrawMineField(const struct MineField *const mineField, const bool zCoord) {
    const signed char** fieldContent = mfGetContent(mineField)[zCoord];
    // const bool** fieldMask = mfGetMask(mineField)[zCoord];
    const size_t fieldSize = mineField->fieldSize;

    for (size_t ix = 0; ix != fieldSize; ++ix) {
        for (size_t iy = 0; iy != fieldSize; ++iy) {
            const signed char* currentVal = &fieldContent[ix][iy];
            if (*currentVal > 0) {
                printf("%d ", *currentVal);
            } else if (*currentVal == 0) {
                printf("  ");
            } else {
                printf("x ");
            }
        }
        printf("\n");
    }
}
