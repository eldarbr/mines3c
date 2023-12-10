#include "MineField.h"


struct MineField* mfConstructor(const size_t inpSize, const size_t inpNumMines) {
    
    // new seed each time you want to play
    srand(time(NULL));

    struct MineField *mineField = malloc(sizeof(struct MineField));

    mineField->fieldSize = inpSize;
    mineField->numberTotalTiles = 2 * inpSize * inpSize;

    unsigned long contentBytes = sizeof(signed char) * 2 * inpSize * inpSize;
    mineField->fieldContent = (signed char*)malloc(contentBytes);
    if (mineField->fieldContent == NULL) {
        free(mineField);
        return NULL;
    }
    memset(mineField->fieldContent, 0, contentBytes);

    unsigned long maskBytes = sizeof(bool) * 2 * inpSize * inpSize;
    mineField->fieldMask = (bool*)malloc(maskBytes);
    if (mineField->fieldMask == NULL) {
        free(mineField->fieldContent);
        free(mineField);
        return NULL;
    }
    memset(mineField->fieldMask, 0, maskBytes);

    unsigned long markingBytes = sizeof(bool) * 2 * inpSize * inpSize;
    mineField->fieldMarking = (bool*)malloc(markingBytes);
    if (mineField->fieldMarking == NULL) {
        free(mineField->fieldContent);
        free(mineField->fieldMask);
        free(mineField);
        return NULL;
    }
    memset(mineField->fieldMarking, 0, markingBytes);


    // generate and save mines
    mineField->numberTotalMines = inpNumMines;
    for (size_t i = 0; i != inpNumMines; ++i) {
        struct Coords newMine = GenerateMine(inpSize);
        if (mfGetTileContent(mineField, &newMine) < 0) {
            --i;
            continue;
        }
        // tiles with mines get value of -6
        // as the max impact to a tile is +5
        mineField->fieldContent[FindArrPosition(mineField, &newMine)] = -6;

        // save the impact of the mine
        struct Coords tempCoords;

        if (newMine.z) {
            tempCoords = newMine;
            tempCoords.z--;
            mineField->fieldContent[FindArrPosition(mineField, &tempCoords)]++;
        }
        if (!newMine.z) {
            tempCoords = newMine;
            tempCoords.z++;
            mineField->fieldContent[FindArrPosition(mineField, &tempCoords)]++;
        }
        if (newMine.x > 1) {
            tempCoords = newMine;
            tempCoords.x--;
            mineField->fieldContent[FindArrPosition(mineField, &tempCoords)]++;
        }
        if (newMine.x + 1 < inpSize) {
            tempCoords = newMine;
            tempCoords.x++;
            mineField->fieldContent[FindArrPosition(mineField, &tempCoords)]++;
        }
        if (newMine.y > 1) {
            tempCoords = newMine;
            tempCoords.y--;
            mineField->fieldContent[FindArrPosition(mineField, &tempCoords)]++;
        }
        if (newMine.y + 1 < inpSize) {
            tempCoords = newMine;
            tempCoords.y++;
            mineField->fieldContent[FindArrPosition(mineField, &tempCoords)]++;
        }
    }

    return mineField;
}


void mfDestructor(struct MineField *const mineField) {
    free(mineField->fieldContent);
    free(mineField->fieldMask);
    free(mineField->fieldMarking);

    free(mineField);
}


void mfOpenTile(struct MineField *const mf, const struct Coords *const tile) {
    if (mfGetTileMarking(mf, tile)) {
        return;
    }
    size_t arrPos = FindArrPosition(mf, tile);
    if (!mf->fieldMask[arrPos]) {
        mf->numberOpenTiles++;
        mf->fieldMask[arrPos] = true;
    }
}


bool mfGetTileMask(const struct MineField *const mf, const struct Coords *const c) {
    return mf->fieldMask[FindArrPosition(mf, c)];
}

signed char mfGetTileContent(const struct MineField *const mf, const struct Coords *const c) {
    return mf->fieldContent[FindArrPosition(mf, c)];
}

bool mfGetTileMarking(const struct MineField *const mf, const struct Coords *const c) {
    return mf->fieldMarking[FindArrPosition(mf, c)];
}


void mfSwitchMarkTile(struct MineField *const mf, const struct Coords *const tile) {
    if (mfGetTileMask(mf, tile)) {
        return;
    }
    size_t arrPos = FindArrPosition(mf, tile);
    if (mfGetTileContent(mf, tile) < 0) {
        if (!mf->fieldMarking[arrPos]) {
            mf->numberCorrectlyMarkedMines++;
        } else {
            mf->numberCorrectlyMarkedMines--;
        }
    }
    mf->fieldMarking[arrPos] = !mf->fieldMarking[arrPos];
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


typedef struct FloodQueueElement FloodQueueElement;

struct FloodQueueElement {
    struct Coords tileCoords;
    FloodQueueElement *nextElement;
};

void fqPush(FloodQueueElement **head, struct Coords *coords) {
    if (head == 0) {
        return;
    }

    FloodQueueElement *newElement =
                (FloodQueueElement*)malloc(sizeof(FloodQueueElement));
    newElement->tileCoords = *coords;
    newElement->nextElement = 0;

    if ((*head) == 0) {
        (*head) = newElement;
    } else {
        FloodQueueElement *tail = *head;
        while (tail->nextElement != 0) {
            tail = tail->nextElement;
        }
        tail->nextElement = newElement;
    }
}

struct Coords fqPop(FloodQueueElement **head) {
    if (head == 0 || (*head) == 0) {
        struct Coords x;
        return x;
    }
    struct Coords value = (*head)->tileCoords;
    FloodQueueElement *toFreee = (*head);
    (*head) = (*head)->nextElement;
    free(toFreee);
    return value;
}


void mfFloodOpenQueue(struct MineField *const mf, struct Coords *startTile) {
    if (mfGetTileMask(mf, startTile)) {
        return;
    }

    FloodQueueElement *head = 0;
    fqPush(&head, startTile);

    while (head != 0) {
        struct Coords currentCoords = fqPop(&head);

        if (mfGetTileMask(mf, &currentCoords)) {
            continue;
        }

        if (mfGetTileContent(mf, &currentCoords) == 0) {
            struct Coords tmpC;

            if (currentCoords.x > 0) {
                tmpC = *startTile;
                tmpC.x--;
                fqPush(&head, &tmpC);
            }
            if (currentCoords.x + 1 < mf->fieldSize) {
                tmpC = *startTile;
                tmpC.x++;
                fqPush(&head, &tmpC);
            }
            if (currentCoords.y > 0) {
                tmpC = *startTile;
                tmpC.y--;
                fqPush(&head, &tmpC);
            }
            if (currentCoords.y + 1 < mf->fieldSize) {
                tmpC = *startTile;
                tmpC.y++;
                fqPush(&head, &tmpC);
            }

            tmpC = *startTile;
            tmpC.z = !tmpC.z;
            fqPush(&head, &tmpC);
        }
    }
}


void mfFloodOpen(struct MineField *const mf, const struct Coords *const startTile) {

    if (mfGetTileMask(mf, startTile)) {
        return;
    }

    mfOpenTile(mf, startTile);

    if (mfGetTileContent(mf, startTile) == 0) {

        struct Coords tmpC;

        if (startTile->x > 0) {
            tmpC = *startTile;
            tmpC.x--;
            mfFloodOpen(mf, &tmpC);
        }
        if (startTile->x + 1 < mf->fieldSize) {
            tmpC = *startTile;
            tmpC.x++;
            mfFloodOpen(mf, &tmpC);
        }
        if (startTile->y > 0) {
            tmpC = *startTile;
            tmpC.y--;
            mfFloodOpen(mf, &tmpC);
        }
        if (startTile->y + 1 < mf->fieldSize) {
            tmpC = *startTile;
            tmpC.y++;
            mfFloodOpen(mf, &tmpC);
        }
    }
}
