#ifndef MINEFIELD_PROD
#define MINEFIELD_PROD


#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


struct Coords {
    /*
    *   Coordinates in the game are:
    *   z (flag) - first or second layer
    *   x (unsigned, [0;]) - vertical, 0-top
    *   y (unsigned, [0;]) - horizontal, 0-left
    */

    bool z;
    size_t x;
    size_t y;
};


struct MineField {
    signed char *fieldContent;
    bool *fieldMask;  // false = tile is closed (default)
    size_t fieldSize;
};
struct MineField* mfConstructor(const size_t size, const size_t numMines);
void mfDestructor(struct MineField *const);

bool mfGetTileMask(const struct MineField *const mf, const struct Coords *const c);
void mfOpenTile(struct MineField *const mf, const struct Coords *const c);
bool mfGetTileContent(const struct MineField *const mf, const struct Coords *const c);


struct Coords GenerateMine(const size_t);

size_t FindArrPosition(const struct MineField *const mf, const struct Coords *const c);


#endif
