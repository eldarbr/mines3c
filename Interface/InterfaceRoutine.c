#include "InterfaceRoutine.h"


void InterfaceRoutine(void) {
    ClearScreen();

    Greetings();

    RepositionCursor(3,1);
    ClearPartOfScreen();

    AskForDifficulty();
    int difficulty = GetUserChoiceDiff();
    while (difficulty < 1 || difficulty > 2) {
        RepositionCursor(3,1);
        ClearPartOfScreen();
        AskForDifficulty();
        difficulty = GetUserChoiceDiff();
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

    bool zState = false;

    while(1) {
        DrawMineField(mf, zState);

        int next = GetUserCommand(3+size);

        if (next == 0) {
            zState = !zState;
            continue;
        } else {
            int absNext;
            if (next <0) absNext = -next;
            else absNext = next;

            int xC, yC;
            if (absNext >= 100) {
                xC = absNext / 100;
                yC = absNext % 100;
            } else {
                xC = absNext / 10;
                yC = absNext % 10;
            }
            xC--;
            yC--;

            if (xC >= (int)size || yC >= (int)size || xC < 0 || yC < 0) {
                continue;
            }

            struct Coords choiceTile = {zState, xC, yC};
            if (next > 0) {
                if (mf->numberOpenTiles == 0) {
                    mfFloodOpenQueue(mf, &choiceTile, mf->numberTotalTiles/2);
                } else {
                    mfOpenTile(mf, &choiceTile);
                }
            } else {
                mfSwitchMarkTile(mf, &choiceTile);
            }
        }

    }

    mfDestructor(mf);
}
