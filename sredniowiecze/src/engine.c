/** @file
    Implementation of game engine.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine.h"

/// Maximum length of a side of the top left corner printed by printTopLeft.
const int MAX_TOP_LEFT_SIZE = 10;

/// Represents the type of a unit.
typedef enum UnitType {
    PEASANT, KING, KNIGHT
} UnitType;

/// Stores information about a unit.
typedef struct Unit {
    UnitType type; //!< Type of the unit.
    int x; //!< Number of column in which the unit is currently placed.
    int y; //!< Number of row in which the unit is currently placed.
    int player; //!< Stores which player owns the unit. Possible values: 1 or 2.
    int lastAction; //!< Stores in which turn did the unit perform its last action (move or production).
} Unit;

/// A list of units.
typedef struct UnitList {
    Unit unit; //!< The unit which is stored in this element of the list.
    struct UnitList *next; //!< Pointer to the next unit on the list.
    struct UnitList **prev_next; //!< Pointer to a pointer pointing to this struct.
} UnitList;

/// Stores information about the currently played game.
typedef struct Game {
    int boardSize; //!< Size of the board on which the game is played.
    int maxTurns; //!< Maximum number of turns which one player can do in a game before a draw is called.
    int currentTurn; //!< Number of current turn.
    int currentPlayer; //!< Which player's turn it currently is.
    int printSize; //!< Size of a side of the square which is printed by printTopLeft.
    int myPlayer; //!< 1 or 2 depending on which player is this program playing as.
    char *topLeft; //!< A text representation of the top left corner of the board.
    bool initialized; //!< True if INIT was already read.
    UnitList *units; //!< List of units on the board.
} Game;

/// Stores game data.
Game game;

/// Returns the bigger of two integers.
int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

/// Returns the smaller of two integers.
int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

/// Returns the absolute value of an integer.
int abs(int a) {
    if (a < 0) {
        return -a;
    }
    return a;
}

/// Returns the sign of an integer (-1 or 0 or 1).
int sgn(int x) {
    return (x < 0) ? -1 : (x > 0);
}

/**
 * Checks if the game has been initialized by both players.
 * @return true if already initialized, false if not.
 */
bool isInitialized() {
    return game.initialized;
}

void startGame() {
    game.initialized = false;
    game.units = NULL;
    game.currentTurn = 1;
    game.currentPlayer = 1;
}

/// Frees memory allocated by all elements of a UnitList.
void freeList(UnitList *unitList) {
    if (unitList != NULL) {
        freeList(unitList->next);
        free(unitList);
    }
}

void endGame() {
    freeList(game.units);
    free(game.topLeft);
}

/// Returns a UnitList containing the unit at position (x, y).
UnitList *atPosition(int x, int y) {
    UnitList *current = game.units;
    while (current != NULL) {
        if (current->unit.x == x && current->unit.y == y) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void printTopLeft() {
    for (int i = 0; i < game.printSize; i++) {
        for (int j = 0; j < game.printSize; j++) {
            printf("%c", game.topLeft[(j * game.printSize) + i]);
        }
        puts("");
    }
    puts("");
}

/// Sets a char in game.topLeft. player = 0 when (x, y) is empty - type is ignored then.
void setTopLeftChar(int x, int y, UnitType type, int player) {
    if (x <= game.printSize && y <= game.printSize) {
        char unitChar;
        if (player == 0) {
            unitChar = '.';
        }
        else {
            switch (type) {
                case KING: unitChar = 'K'; break;
                case KNIGHT: unitChar = 'R'; break;
                case PEASANT: unitChar = 'C'; break;
            }
            if (player == 2) {
                unitChar -= 'A' - 'a';
            }
        }
        game.topLeft[game.printSize * (x - 1) + (y - 1)] = unitChar;
    }
}

/**
 * Adds a unit to game.unitList.
 * Assumes that the target field is empty, this should be checked before calling addUnit.
 */
void addUnit(UnitType type, int x, int y, int player) {
    UnitList *temp = malloc(sizeof(UnitList));
    temp->next = game.units;
    if (temp->next != NULL) {
        temp->next->prev_next = &temp->next;
    }
    game.units = temp;
    temp->prev_next = &game.units;

    temp->unit.type = type;
    temp->unit.x = x;
    temp->unit.y = y;
    temp->unit.player = player;
    temp->unit.lastAction = game.currentTurn-1;

    setTopLeftChar(x, y, type, player);
}

/// Removes an element from a list.
void removeUnit(UnitList *unit) {
    setTopLeftChar(unit->unit.x, unit->unit.y, unit->unit.type, 0);
    *(unit->prev_next) = unit->next;
    if (unit->next != NULL) {
        unit->next->prev_next = unit->prev_next;
    }
    free(unit);
}

int init(int n, int k, int p, int x1, int y1, int x2, int y2) {
    p -= 1;
    if ((p != 0 && p != 1) || isInitialized()) {
        return INPUT_ERROR;
    }

    game.boardSize = n;
    game.maxTurns = k;
    if (game.boardSize <= 8 || game.maxTurns < 1 ||
        max(abs(x1 - x2), abs(y1 - y2)) < 8 ||
        x1 > game.boardSize - 3 || x2 > game.boardSize - 3 ||
        x1 < 1 || x2 < 1 || y1 < 1 || y2 < 1 ||
        y1 > game.boardSize || y2 > game.boardSize) {
            return INPUT_ERROR;
    }
    game.printSize = min(game.boardSize, MAX_TOP_LEFT_SIZE);
    int square = game.printSize * game.printSize;
    game.topLeft = malloc(square * sizeof(char));
    for (int i = 0; i < square; i++) {
        game.topLeft[i] = '.';
    }
    addUnit(KING, x1, y1, 1);
    addUnit(PEASANT, x1+1, y1, 1);
    addUnit(KNIGHT, x1+2, y1, 1);
    addUnit(KNIGHT, x1+3, y1, 1);
    addUnit(KING, x2, y2, 2);
    addUnit(PEASANT, x2+1, y2, 2);
    addUnit(KNIGHT, x2+2, y2, 2);
    addUnit(KNIGHT, x2+3, y2, 2);

    game.initialized = true;
    game.myPlayer = p+1;
    return SUCCESS;
}

/// Assuming that (int player) won, returns WON or LOST depending on which player is this program controlling.
int whoWon(int player) {
    if (player == game.myPlayer) {
        return WON;
    }
    else {
        return LOST;
    }
}

int move(int x1, int y1, int x2, int y2) {
    if (!isInitialized()) {
        return INPUT_ERROR;
    }
    UnitList *unit = atPosition(x1, y1);
    if (unit == NULL ||
        unit->unit.player != game.currentPlayer ||
        unit->unit.lastAction == game.currentTurn ||
        abs(x2-x1) > 1 || abs(y2-y1) > 1 || (x1 == x2 && y1 == y2) ||
        x2 < 1 || x2 > game.boardSize || y2 < 1 || y2 > game.boardSize) {

        return INPUT_ERROR;
    }

    UnitList *unit2 = atPosition(x2, y2);
    int returnCode = SUCCESS;
    bool unitRemoved = false;
    if (unit2 != NULL) {
        if (unit->unit.player == unit2->unit.player) {
            return INPUT_ERROR;
        }

        if (unit->unit.type == unit2->unit.type) {
            if (unit->unit.type == KING) {
                returnCode = DRAW;
            }
            removeUnit(unit);
            unitRemoved = true;
            removeUnit(unit2);
        }
        else if (unit2->unit.type > unit->unit.type) {
            if (unit->unit.type == KING) {
                if (unit->unit.player == 1) {
                    returnCode = whoWon(2);
                }
                else {
                    returnCode = whoWon(1);
                }
            }
            removeUnit(unit);
            unitRemoved = true;
        }
        else if (unit->unit.type > unit2->unit.type) {
            if (unit2->unit.type == KING) {
                if (unit2->unit.player == 1) {
                    returnCode = whoWon(2);
                }
                else {
                    returnCode = whoWon(1);
                }
            }
            removeUnit(unit2);
        }
    }

    if (!unitRemoved) {
        setTopLeftChar(x1, y1, unit->unit.type, 0);
        unit->unit.x = x2;
        unit->unit.y = y2;
        unit->unit.lastAction = game.currentTurn;
        setTopLeftChar(x2, y2, unit->unit.type, unit->unit.player);
    }
    return returnCode;
}

/// Produces a peasant or a knight.
int produceUnit(int x1, int y1, int x2, int y2, UnitType type) {
    if (!isInitialized()) {
        return INPUT_ERROR;
    }
    UnitList *unit = atPosition(x1, y1);
    if (unit == NULL ||
        unit->unit.player != game.currentPlayer ||
        unit->unit.lastAction > game.currentTurn - 3 ||
        abs(x2-x1) > 1 || abs(y2-y1) > 1 || (x1 == x2 && y1 == y2) ||
        unit->unit.type != PEASANT ||
        x2 < 1 || x2 > game.boardSize || y2 < 1 || y2 > game.boardSize) {
            return INPUT_ERROR;
    }

    UnitList *unit2 = atPosition(x2, y2);
    if (unit2 != NULL) {
        return INPUT_ERROR;
    }
    addUnit(type, x2, y2, game.currentPlayer);
    unit->unit.lastAction = game.currentTurn;
    return SUCCESS;
}

int produceKnight(int x1, int y1, int x2, int y2) {
    return produceUnit(x1, y1, x2, y2, KNIGHT);
}

int producePeasant(int x1, int y1, int x2, int y2) {
    return produceUnit(x1, y1, x2, y2, PEASANT);
}

int endTurn() {
    if (!isInitialized()) {
        return INPUT_ERROR;
    }
    if (game.currentPlayer == 1) {
        game.currentPlayer = 2;
    }
    else {
        game.currentPlayer = 1;
        game.currentTurn++;
    }
    if (game.currentTurn == game.maxTurns+1) {
        return DRAW;
    }
    return SUCCESS;
}


// AI starts here

bool isMyTurn() {
    if (game.currentPlayer == game.myPlayer) {
        return true;
    }
    return false;
}

/// Stores information used by the AI to make a move. Describes info about the field passed to generateTurnInfo().
typedef struct TurnInfo {
    int myPeasants; //!< How many peasants does the controlled player have.
    int nearbyFields[3][3]; //!< 3x3 grid of fields near the field, nearbyFields[2][2] is that field. 0 - empty, 1 - friendly unit or terrain, 2 - enemy unit
    Unit nearestEnemyUnit; //!< The enemy unit which is the closest to the field.
} TurnInfo;

/// Fills a TurnInfo struct with data about the field passed to this function.
TurnInfo generateTurnInfo(int x, int y) {
    TurnInfo ret;

    ret.myPeasants = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ret.nearbyFields[i][j] = 0;
        }
    }
    ret.nearbyFields[1][1] = 1;
    if (x == 1 || x == game.boardSize) {
        for (int i = 0; i < 3; i++) {
            ret.nearbyFields[(x == 1) ? 0 : 2][i] = 1;
        }
    }
    if (y == 1 || y == game.boardSize) {
        for (int i = 0; i < 3; i++) {
            ret.nearbyFields[i][(y == 1) ? 0 : 2] = 1;
        }
    }

    UnitList *current = game.units;
    ret.nearestEnemyUnit = current->unit;

    while (current != NULL) {
        if (abs(current->unit.x - x) <= 1 && abs(current->unit.y - y) <= 1) {
            ret.nearbyFields[current->unit.x - x + 1][current->unit.y - y + 1] =
                (game.myPlayer == current->unit.player) ? 1 : 2;
        }
        if (current->unit.player == game.myPlayer && current->unit.type == PEASANT) {
            ret.myPeasants++;
        }
        if (ret.nearestEnemyUnit.player == game.myPlayer
            && current->unit.player != game.myPlayer) {

            ret.nearestEnemyUnit = current->unit;
        }
        if (game.myPlayer != current->unit.player
            && max(abs(current->unit.x - x), abs(current->unit.y - y))
               <= max(abs(ret.nearestEnemyUnit.x - x), abs(ret.nearestEnemyUnit.y - y))
            && (abs(current->unit.x - x) < abs(ret.nearestEnemyUnit.x - x)
                || abs(current->unit.y - y) < abs(ret.nearestEnemyUnit.y - y))) {

            ret.nearestEnemyUnit = current->unit;
        }

        current = current->next;
    }

    return ret;
}

/// Prints a MOVE command and calls the move() function.
int moveAI(int x1, int y1, int x2, int y2) {
    printf("MOVE %d %d %d %d\n", x1, y1, x2, y2);
    return move(x1, y1, x2, y2);
}

/// Prints a PRODUCE_X command and calls the produceUnit() function.
int produceAI(int x1, int y1, int x2, int y2, UnitType type) {
    if (type == KNIGHT) {
        printf("PRODUCE_KNIGHT %d %d %d %d\n", x1, y1, x2, y2);
    }
    else if (type == PEASANT) {
        printf("PRODUCE_PEASANT %d %d %d %d\n", x1, y1, x2, y2);
    }
    produceUnit(x1, y1, x2, y2, type);
}

int makeTurn() {
    UnitList *current = game.units;
    bool producedUnit = false;

    while (current != NULL) {
        // current->next might later be unavailable if a knight is lost in a fight.
        UnitList *nextUnit = current->next;

        if (current->unit.player == game.myPlayer) {
            if (current->unit.lastAction == game.currentTurn) {
                /* If the loop reached a friendly unit which already make its move,
                   this must be the second call of makeTurn() in the same turn
                   and all units after this one were already processed in then
                   previous call of makeTurn(). */
                break;
            }

            // The value returned later by move() or produceUnit().
            int ret = -1;

            TurnInfo turnInfo = generateTurnInfo(current->unit.x, current->unit.y);
            if (current->unit.type == KNIGHT) {
                int diffX = sgn(turnInfo.nearestEnemyUnit.x - current->unit.x),
                    diffY = sgn(turnInfo.nearestEnemyUnit.y - current->unit.y);
                bool moved = false;

                /* Try to make a move where both (i==2) directions (up and left etc.)
                   match the direction to nearestEnemyUnit. If not possible,
                   make a move where one (i==1) direction matches and
                   if that's not possible either, make a move anywhere (i==0). */
                for (int i = 2; i >= 0 && !moved; i--) {
                    for (int dx = -1; dx <= 1 && !moved; dx++) {
                        for (int dy = -1; dy <= 1 && !moved; dy++) {
                            if ((dx == diffX) + (dy == diffY) == i
                                && turnInfo.nearbyFields[dx + 1][dy + 1] != 1) {

                                if (nextUnit->unit.x == current->unit.x + dx
                                    && nextUnit->unit.y == current->unit.y + dy) {

                                    nextUnit = nextUnit->next;
                                }
                                ret = moveAI(current->unit.x, current->unit.y,
                                    current->unit.x + dx, current->unit.y + dy);
                                moved = true;
                            }
                        }
                    }
                }
            }

            else if (current->unit.type == PEASANT
                     && current->unit.lastAction <= game.currentTurn - 3) {

                UnitType toProduce = KNIGHT;
                if (turnInfo.myPeasants < 2) {
                    toProduce = PEASANT;
                }
                int diffX = sgn(turnInfo.nearestEnemyUnit.x - current->unit.x),
                    diffY = sgn(turnInfo.nearestEnemyUnit.y - current->unit.y);
                bool moved = false;

                // Same as with moving a knight, just producing instead of moving.
                for (int i = 2; i >= 0 && !moved; i--) {
                    for (int dx = -1; dx <= 1 && !moved; dx++) {
                        for (int dy = -1; dy <= 1 && !moved; dy++) {
                            if ((dx == diffX) + (dy == diffY) == i
                                && turnInfo.nearbyFields[dx + 1][dy + 1] == 0) {

                                ret = produceAI(current->unit.x, current->unit.y,
                                    current->unit.x + dx, current->unit.y + dy,
                                    toProduce);
                                moved = true;
                            }
                        }
                    }
                }
            }

            // The game has ended.
            if (ret != -1 && ret != SUCCESS) {
                return ret;
            }
        }
        current = nextUnit;
    }

    if (producedUnit) {
        /* If a unit was produced in this turn, it was added to the beginning of
           game.unitList, so makeTurn() needs to be called again to make moves
           with those just produced units.  */
        return makeTurn();
    }
    else {
        puts("END_TURN");
        return endTurn();
    }
}
