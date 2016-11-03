#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "structure.h"

const char *ERROR_MESSAGE = "ERROR";

int main(int argc, char **argv) {
   bool debug = false;

   if (argc > 2) {
      puts(ERROR_MESSAGE);
      return 1;
   }
   else if (argc == 2) {
      if (strcmp("-v", argv[1]) == 0) {
         debug = true;
      }
      else {
         puts(ERROR_MESSAGE);
         return 1;
      }
   }

   Database *database = initializeDatabase();

   ParsedInput *input = malloc(sizeof(ParsedInput));
   input->atr1 = malloc((MAX_LINE_LENGTH + 1) * sizeof(char));
   input->atr2 = malloc((MAX_LINE_LENGTH + 1) * sizeof(char));

   while (parseLine(input)) {
      switch (input->function) {
         case NewDiseaseEnterDescription:
            newDiseaseEnterDescription(input->atr1, input->atr2, database, debug);
            break;
         case NewDiseaseCopyDescription:
            newDiseaseCopyDescription(input->atr1, input->atr2, database, debug);
            break;
         case ChangeDescription:
            changeDescription(input->atr1, input->n, input->atr2, database, debug);
            break;
         case PrintDescription:
            printDescription(input->atr1, input->n, database, debug);
            break;
         case DeletePatientData:
            deletePatientData(input->atr1, database, debug);
            break;
      }
   }

   free(input->atr1);
   free(input->atr2);
   free(input);
   deleteDatabase(database);

   return 0;
}
