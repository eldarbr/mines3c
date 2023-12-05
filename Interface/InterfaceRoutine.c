#include "InterfaceRoutine.h"


void InterfaceRoutine(void) {
    PauseExec();

    ClearScreen();

    Greetings();

    RepositionCursor(3,1);
    ClearPartOfScreen();

    AskForDifficulty();
    int difficulty = GetUserChoice();
    while (difficulty < 1 || difficulty > 2) {
        RepositionCursor(3,1);
        ClearPartOfScreen();
        AskForDifficulty();
        difficulty = GetUserChoice();
    }

    size_t size, mines;
    switch (difficulty) {
        case 1:
            size = 8;
            mines = 15;
            break;
        case 2:
            size = 10;
            mines = 25;
            break;
        default:
            Exception("difficulty not in range", 22);
            return;
    }

    ClearScreen();

    struct MineField* mf = mfConstructor(size, mines);

    DrawMineField(mf, 0);

    PauseExec();

}
