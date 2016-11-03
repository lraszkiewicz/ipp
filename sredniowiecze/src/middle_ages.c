/** @file
    Implementation of the main function.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "engine.h"

/// The main function.
int main() {
    startGame();
    initParser();

    Command *newCommand;
    while (true) {
        int ret;

        if(isMyTurn()) {
            ret = makeTurn();
            fflush(stdout);
        }
        else {
            newCommand = parseCommand();
            if (!newCommand->isValid) {
                fputs("input error\n", stderr);
                endGame();
                freeParser();
                free(newCommand);
                return INPUT_ERROR;
            }

            if (strcmp(newCommand->name, "INIT") == 0) {
                ret = init(newCommand->data[0], newCommand->data[1],
                           newCommand->data[2], newCommand->data[3],
                           newCommand->data[4], newCommand->data[5],
                           newCommand->data[6]);
            }
            else if (strcmp(newCommand->name, "MOVE") == 0) {
                ret = move(newCommand->data[0], newCommand->data[1],
                           newCommand->data[2], newCommand->data[3]);
            }
            else if (strcmp(newCommand->name, "PRODUCE_KNIGHT") == 0) {
                ret = produceKnight(newCommand->data[0], newCommand->data[1],
                                    newCommand->data[2], newCommand->data[3]);
            }
            else if (strcmp(newCommand->name, "PRODUCE_PEASANT") == 0) {
                ret = producePeasant(newCommand->data[0], newCommand->data[1],
                                     newCommand->data[2], newCommand->data[3]);
            }
            else if (strcmp(newCommand->name, "END_TURN") == 0) {
                ret = endTurn();
            }

            free(newCommand);
        }

        switch(ret) {
            case INPUT_ERROR:
                fputs("input error\n", stderr);
                endGame();
                freeParser();
                return INPUT_ERROR;
            case WON:
                fputs("won\n", stderr);
                break;
            case LOST:
                fputs("lost\n", stderr);
                break;
            case DRAW:
                fputs("draw\n", stderr);
                break;
        }
        if (ret != SUCCESS) { // but not INPUT_ERROR
            endGame();
            freeParser();
            if (ret == WON) {
                return 0;
            }
            else {
                return ret;
            }
        }

    }

    endGame();
    freeParser();

    return SUCCESS;
}
