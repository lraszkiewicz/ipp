#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"

#define TO_STRING(x) #x
#define STR(x) TO_STRING(x)
#define LINE_LIMIT STR(MAX_LINE_LENGTH)

/* Długość nazwy funkcji na wejściu (np. NEW_DISEASE_ENTER_DESCRIPTION)
 * nie będzie dłuższa niż 50. */
#define MAX_FUNCTION_NAME_LENGTH 50

bool parseLine(ParsedInput *input) {
   char functionName[MAX_FUNCTION_NAME_LENGTH + 1];

   if (scanf("%" STR(MAX_FUNCTION_NAME_LENGTH) "s", functionName) == EOF) {
      return false;
   }

   if (strcmp(functionName, "NEW_DISEASE_ENTER_DESCRIPTION") == 0) {
      input->function = NewDiseaseEnterDescription;
      scanf(" %" LINE_LIMIT "s %" LINE_LIMIT "[^\n]s", input->atr1, input->atr2);
   }
   else if (strcmp(functionName, "NEW_DISEASE_COPY_DESCRIPTION") == 0) {
      input->function = NewDiseaseCopyDescription;
      scanf(" %" LINE_LIMIT "s %" LINE_LIMIT "s", input->atr1, input->atr2);
   }
   else if (strcmp(functionName, "CHANGE_DESCRIPTION") == 0) {
      input->function = ChangeDescription;
      scanf(" %" LINE_LIMIT "s %d %" LINE_LIMIT "[^\n]s", input->atr1, &input->n, input->atr2);
   }
   else if (strcmp(functionName, "PRINT_DESCRIPTION") == 0) {
      input->function = PrintDescription;
      scanf(" %" LINE_LIMIT "s %d", input->atr1, &input->n);
   }
   else if (strcmp(functionName, "DELETE_PATIENT_DATA") == 0) {
      input->function = DeletePatientData;
      scanf(" %" LINE_LIMIT "s", input->atr1);
   }

   return true;
}
