/** @file
    Interface of parser.
*/

#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>

/// Used to pass data about a command read from input.
typedef struct Command {
    char name[16]; //!< Name of the command.
    int data[7]; //!< Data read from input.
    bool isValid; //!< True if the command is valid, false if input error.
} Command;

/**
 * Reads a command.
 * @return data read from input in a 'Command' structure.
 */
Command *parseCommand();

/// Initializes global variables required by parseCommand.
void initParser();

/// Frees memory used by parse.c.
void freeParser();

#endif /* PARSE_H */
