/** @file
    Implementation of parser.
*/

#include <ctype.h>
#include <errno.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

/// Maximum length of a line read which will not cause input error.
const int MAX_LINE_LENGTH = 100;

/// Regex for the INIT command.
regex_t regex1;
/// Regex for commands MOVE and PROCUDE_X.
regex_t regex2;
/// Regex for the END_TURN command.
regex_t regex3;
/// String to which lines will be read.
char *line;

void initParser() {
    regcomp(&regex1, "^[A-Z_]+( [0-9]+){7}\n", REG_EXTENDED);
    regcomp(&regex2, "^[A-Z_]+( [0-9]+){4}\n", REG_EXTENDED);
    regcomp(&regex3, "^[A-Z_]+\n", REG_EXTENDED);
    line = malloc((MAX_LINE_LENGTH + 3) * sizeof(char));
}

void freeParser() {
    regfree(&regex1);
    regfree(&regex2);
    regfree(&regex3);
    free(line);
}

Command *parseCommand() {
    Command *commandRead = malloc(sizeof(Command));
    commandRead->isValid = true;

    if (fgets(line, MAX_LINE_LENGTH + 3, stdin) == NULL || strlen(line) > MAX_LINE_LENGTH + 1) {
        commandRead->isValid = false;
        return commandRead;
    }

    char commandName[MAX_LINE_LENGTH+1];
    sscanf(line, "%s", commandName);

    if (strcmp(commandName, "INIT") == 0) {
        if (regexec(&regex1, line, 0, NULL, 0)) {
            commandRead->isValid = false;
            return commandRead;
        }
        char numbers[7][MAX_LINE_LENGTH+1];
        sscanf(line, "%*s %s %s %s %s %s %s %s",
               numbers[0], numbers[1], numbers[2], numbers[3],
               numbers[4], numbers[5], numbers[6]);
        for (int i = 0; i < 7; i++) {
            commandRead->data[i] = strtol(numbers[i], NULL, 10);
            if (errno) {
                errno = 0;
                commandRead->isValid = false;
                return commandRead;
            }
        }
    }
    else if (strcmp(commandName, "MOVE") == 0 ||
             strcmp(commandName, "PRODUCE_KNIGHT") == 0 ||
             strcmp(commandName, "PRODUCE_PEASANT") == 0) {
        if (regexec(&regex2, line, 0, NULL, 0)) {
            commandRead->isValid = false;
            return commandRead;
        }
        char numbers[4][MAX_LINE_LENGTH+1];
        sscanf(line, "%*s %s %s %s %s",
               numbers[0], numbers[1], numbers[2], numbers[3]);
        for (int i = 0; i < 4; i++) {
            commandRead->data[i] = strtol(numbers[i], NULL, 10);
            if (errno) {
                errno = 0;
                commandRead->isValid = false;
                return commandRead;
            }
        }
    }
    else if (strcmp(commandName, "END_TURN") == 0) {
        if (regexec(&regex3, line, 0, NULL, 0)) {
            commandRead->isValid = false;
            return commandRead;
        }
    }
    else {
        commandRead->isValid = false;
        return commandRead;
    }

    strcpy(commandRead->name, commandName);
    return commandRead;
}
