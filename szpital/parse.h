#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>

// Długośc linii na wejściu jest nie dłuższa niż 100 000.
#define MAX_LINE_LENGTH 100000

typedef enum FunctionType {
   NewDiseaseEnterDescription,
   NewDiseaseCopyDescription,
   ChangeDescription,
   PrintDescription,
   DeletePatientData
} FunctionType;

typedef struct ParsedInput {
   FunctionType function;
   int n;
   char *atr1, *atr2;
} ParsedInput;

/* Zwraca false, jeżeli wczytano EOF.
 * W przeciwnym przypadku zwraca true, parsuje jeden wiersz z wejścia i
 * umieszcza wczytane informacje w strukturze input. */
bool parseLine(ParsedInput *input);

#endif // PARSE_H
