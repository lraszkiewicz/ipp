/** @file
    Interface of game engine.
*/

#ifndef ENGINE_H
#define ENGINE_H

// Using defines instead of consts so they can be used in a switch/case statement.

/// Return code returned by functions which ended succesfully.
#define SUCCESS 0
/// Return code returned by functions in case of an input error.
#define INPUT_ERROR 42
/// Return code returned by functions when this AI won the game.
#define WON 3
/// Return code returned by functions when this AI lost the game.
#define LOST 2
/// Return code returned by functions when there is a draw.
#define DRAW 1

/**
 * Initializes a game. Needed before first INIT.
 */
void startGame();

/**
 * Frees memory. Needed after finishing game.
 */
void endGame();

/**
 * Initializes a game with size of a board, number of rounds and positions of kings.
 * @return INPUT_ERROR or SUCCESS
 */
int init(int n, int k, int p, int x1, int y1, int x2, int y2);

/**
 * Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number before a move.
 * @return INPUT_ERROR or SUCCESS or WON/DRAW/LOST
 */
int move(int x1, int y1, int x2, int y2);

/**
 * Produces a knight.
 * @param[in] x1 Column number of a peasant who should produce the knight.
 * @param[in] y1 Row number of a peasant who should produce the knight.
 * @param[in] x2 Number of column at which the knight should be produced.
 * @param[in] y2 Number of row at which the knight should be produced.
 * @return INPUT_ERROR or SUCCESS
 */
int produceKnight(int x1, int y1, int x2, int y2);

/**
 * Produces a peasant.
 * @param[in] x1 Column number of a peasant who should produce the peasant.
 * @param[in] y1 Row number of a peasant who should produce the peasant.
 * @param[in] x2 Number of column at which the peasant should be produced.
 * @param[in] y2 Number of row at which the peasant should be produced.
 * @return INPUT_ERROR or SUCCESS
 */
int producePeasant(int x1, int y1, int x2, int y2);

/**
 * Ends a player's turn.
 * @return INPUT_ERROR or SUCCESS or WON/DRAW/LOST
 */
int endTurn();

/**
 * Prints (to stdout) top-left corner of the board of size m x m where m = min(n, 10).
 */
void printTopLeft();

/**
 * AI makes a move.
 * @return SUCCESS or WON/DRAW/LOST
 */
int makeTurn();

/**
 * Returns true or false, depenging on whose move it is.
 */
bool isMyTurn();

#endif /* ENGINE_H */
