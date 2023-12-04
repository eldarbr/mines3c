#include "MineField.h"


struct MineField* mfConstructor(const size_t inpSize, const size_t inpNumMines) {
    
    // new seed each time you want to play
    srand(time(NULL));

    struct MineField *mineField = malloc(sizeof(struct MineField));

    mineField->fieldSize = inpSize;

    unsigned long contentBytes = sizeof(signed char) * 2 * inpSize * inpSize;
    mineField->fieldContent = (signed char*)malloc(contentBytes);
    memset(mineField->fieldContent, 0, contentBytes);

    unsigned long maskBytes = sizeof(bool) * 2 * inpSize * inpSize;
    mineField->fieldMask = (bool*)malloc(maskBytes);
    memset(mineField->fieldMask, 0, maskBytes);

    // generate and save mines
    for (size_t i = 0; i != inpNumMines; ++i) {
        struct Coords newMine = GenerateMine(inpSize);
        if (mfIsThisAMine(mineField, &newMine)) {
            --i;
            continue;
        }
        // tiles with mines get value of -6
        // as the max impact to a tile is +5
        mineField->fieldContent[newMine.z][newMine.x][newMine.y] = -6;

        if (newMine.z)
        mineField->fieldContent[newMine.z-1][newMine.x][newMine.y]++;
        if (!newMine.z)
        mineField->fieldContent[newMine.z+1][newMine.x][newMine.y]++;
        if (newMine.x > 1)
        mineField->fieldContent[newMine.z][newMine.x-1][newMine.y]++;
        if (newMine.x + 1 < inpSize)
        mineField->fieldContent[newMine.z][newMine.x+1][newMine.y]++;
        if (newMine.y > 1)
        mineField->fieldContent[newMine.z][newMine.x][newMine.y-1]++;
        if (newMine.y + 1 < inpSize)
        mineField->fieldContent[newMine.z][newMine.x][newMine.y+1]++;
    }

    return mineField;
}


void mfDestructor(struct MineField *const mineField) {
    const size_t fieldSize = mineField->fieldSize;

    free(mineField->fieldContent);
    free(mineField->fieldMask);

    free(mineField);
}


const signed char ***mfGetContent(const struct MineField *const mineField) {
    return (const signed char ***)mineField->fieldContent;
}


const bool ***mfGetMask(const struct MineField *const mineField) {
    return (const bool ***)mineField->fieldMask;
}


void mfOpenTile(struct MineField *const mineField, const struct Coords *const tileCoords) {
    mineField->fieldMask[(size_t)tileCoords->z][tileCoords->x][tileCoords->y] = true;
}


bool mfIsThisAMine(const struct MineField *const MineField, const struct Coords *const tileCoords) {
    const bool *tileZ = &(tileCoords->z);
    const size_t *tileX = &(tileCoords->x);
    const size_t *tileY = &(tileCoords->y);

    if (MineField->fieldContent[*tileZ][*tileX][*tileY] < 0) {
        return true;
    } else {
        return false;
    }
}


struct Coords GenerateMine(const size_t fieldSize) {
    size_t x = rand() % fieldSize;
    size_t y = rand() % fieldSize;
    bool z = rand() % 2;
    struct Coords c = {z, x, y};
    return c;
}


size_t FindArrPosition(const struct MineField *const mf,
                                const struct Coords *const c) {

    size_t position = 0;

    const size_t fieldSize = mf->fieldSize;

    position += c->y;
    position += fieldSize * c->x;
    if (c->z) {
        position += fieldSize * fieldSize;
    }

    return position;
}
