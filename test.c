#include <stdio.h>
#include "Interface/Interface.h"
#include "MineField/MineField.h"


int main(void) {
    
    struct MineField *minefield = mfConstructor(10, 20);

    DrawMineField(minefield, 0);

    return 0;
}
