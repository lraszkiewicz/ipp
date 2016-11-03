#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

typedef struct Disease {
   char *description;
   int counter;
} Disease;

typedef struct DiseaseList {
   Disease *disease;
   struct DiseaseList *next;
} DiseaseList;

typedef struct Patient {
   char *name;
   DiseaseList *diseaseList, *lastDiseaseList;
   struct Patient *next;
} Patient;

typedef struct Database {
   Patient *patientList;
   int descriptions;
} Database;

const char *OK_MESSAGE = "OK";
const char *IGNORED_MESSAGE = "IGNORED";

/* Znajduje i zwraca pacjenta o nazwisku name w patientList.
 * Jeżeli nie ma takiego pacjenta, zwraca NULL. */
Patient *findPatient(Patient *patientList, char *name) {
   while (patientList != NULL) {
      if (strcmp(name, patientList->name) == 0) {
         return patientList;
      }
      patientList = patientList->next;
   }
   return NULL;
}

// Dodaje pacjenta newPatient na koniec database->patientList.
void pushPatient(Patient *newPatient, Database *database) {
   if (database->patientList == NULL) {
      database->patientList = newPatient;
   }
   else {
      Patient *lastPatient = database->patientList;
      while (lastPatient->next != NULL) {
         lastPatient = lastPatient->next;
      }
      lastPatient->next = newPatient;
   }
}

// Dodaje chorobę disease na koniec patient->diseaseList.
void pushDisease(Disease *disease, Patient *patient) {
   disease->counter++;
   if (patient->diseaseList == NULL) {
      patient->diseaseList = malloc(sizeof(DiseaseList));
      patient->diseaseList->disease = disease;
      patient->diseaseList->next = NULL;
      patient->lastDiseaseList = patient->diseaseList;
   }
   else {
      DiseaseList *newDiseaseList = malloc(sizeof(DiseaseList));
      newDiseaseList->disease = disease;
      newDiseaseList->next = NULL;
      patient->lastDiseaseList->next = newDiseaseList;
      patient->lastDiseaseList = newDiseaseList;
   }
}

/* Zwraca ostatnią chorobę pacjetna patient.
 * Jeżeli historia chorób jest pusta, zwraca NULL. */
Disease *getLastDisease(Patient *patient) {
   if (patient->diseaseList == NULL) {
      return NULL;
   }
   DiseaseList *lastDisease = patient->diseaseList;
   if (lastDisease == NULL) {
      return NULL;
   }
   while (lastDisease->next != NULL) {
      lastDisease = lastDisease->next;
   }
   return lastDisease->disease;
}

/* Zmniejsza licznik referencji do choroby disease,
 * a jeżeli wynosi 0, usuwa ją z pamięci. */
void removeDisease(Disease *disease, Database *database) {
   disease->counter--;
   if (disease->counter == 0) {
      database->descriptions--;
      free(disease->description);
      free(disease);
   }
}

/* Usuwa wszystkie elementy z diseaseList
 * i na każdym z nich wywołuje removeDisease. */
void removeDiseaseList(DiseaseList *diseaseList, Database *database) {
   if (diseaseList == NULL) {
      return;
   }
   removeDiseaseList(diseaseList->next, database);
   removeDisease(diseaseList->disease, database);
   free(diseaseList);
}

// Usuwa z pamięci wszystkich pacjentów z patientList oraz ich choroby.
void removePatientList(Patient *patientList, Database *database) {
   if (patientList == NULL) {
      return;
   }
   removePatientList(patientList->next, database);
   removeDiseaseList(patientList->diseaseList, database);
   free(patientList->name);
   free(patientList);
}

// Wypisuje komunikat DESCRIPTIONS na stderr jeżeli debug == true.
void printDebug(Database *database, bool debug) {
   if (debug) {
      fprintf(stderr, "DESCRIPTIONS: %d\n", database->descriptions);
   }
}

// Funkcje poniżej tego komentarza są opisane w structure.h.

Database *initializeDatabase() {
   Database *database = malloc(sizeof(Database));
   database->patientList = NULL;
   database->descriptions = 0;
   return database;
}

void deleteDatabase(Database *database) {
   removePatientList(database->patientList, database);
   free(database);
}

void newDiseaseEnterDescription(char *name, char *description, Database *database, bool debug) {
   Patient *patient = findPatient(database->patientList, name);

   if (patient == NULL) {
      patient = malloc(sizeof(Patient));
      patient->name = malloc((strlen(name) + 1) * sizeof(char));
      strcpy(patient->name, name);
      patient->diseaseList = NULL;
      patient->next = NULL;

      pushPatient(patient, database);
   }

   Disease *newDisease = malloc(sizeof(Disease));
   newDisease->description = malloc((strlen(description) + 1) * sizeof(char));
   strcpy(newDisease->description, description);
   newDisease->counter = 0;
   pushDisease(newDisease, patient);
   database->descriptions++;

   puts(OK_MESSAGE);
   printDebug(database, debug);
}

void newDiseaseCopyDescription(char *name1, char *name2, Database *database, bool debug) {
   Patient *patient = findPatient(database->patientList, name1);
   Patient *oldPatient = findPatient(database->patientList, name2);
   if (oldPatient == NULL || oldPatient->diseaseList == NULL) {
      puts(IGNORED_MESSAGE);
      printDebug(database, debug);
      return;
   }

   if (patient == NULL) {
      patient = malloc(sizeof(Patient));
      patient->name = malloc((strlen(name1) + 1) * sizeof(char));
      strcpy(patient->name, name1);
      patient->diseaseList = NULL;
      patient->next = NULL;

      pushPatient(patient, database);
   }

   pushDisease(getLastDisease(oldPatient), patient);

   puts(OK_MESSAGE);
   printDebug(database, debug);
}

void changeDescription(char *name, int n, char *description, Database *database, bool debug) {
   Patient *patient = findPatient(database->patientList, name);
   if (patient == NULL) {
      puts(IGNORED_MESSAGE);
      printDebug(database, debug);
      return;
   }

   DiseaseList *diseaseList = patient->diseaseList;
   for (int i = 1; i < n; i++) {
      if (diseaseList == NULL) {
         puts(IGNORED_MESSAGE);
         printDebug(database, debug);
         return;
      }
      diseaseList = diseaseList->next;
   }
   if (diseaseList == NULL) {
      puts(IGNORED_MESSAGE);
      printDebug(database, debug);
      return;
   }

   Disease *newDisease = malloc(sizeof(Disease));
   newDisease->description = malloc((strlen(description) + 1) * sizeof(char));
   strcpy(newDisease->description, description);
   newDisease->counter = 1;

   removeDisease(diseaseList->disease, database);
   diseaseList->disease = newDisease;

   database->descriptions++;

   puts(OK_MESSAGE);
   printDebug(database, debug);
}

void printDescription(char *name, int n, Database *database, bool debug) {
   Patient *patient = findPatient(database->patientList, name);
   if (patient == NULL) {
      puts(IGNORED_MESSAGE);
      printDebug(database, debug);
      return;
   }

   DiseaseList *diseaseList = patient->diseaseList;
   for (int i = 1; i < n; i++) {
      if (diseaseList == NULL) {
         puts(IGNORED_MESSAGE);
         printDebug(database, debug);
         return;
      }
      diseaseList = diseaseList->next;
   }
   if (diseaseList == NULL) {
      puts(IGNORED_MESSAGE);
      printDebug(database, debug);
      return;
   }

   printf("%s\n", diseaseList->disease->description);
   printDebug(database, debug);
}

void deletePatientData(char *name, Database *database, bool debug) {
   Patient *patient = findPatient(database->patientList, name);
   if (patient == NULL) {
      puts(IGNORED_MESSAGE);
      printDebug(database, debug);
      return;
   }

   removeDiseaseList(patient->diseaseList, database);
   patient->diseaseList = NULL;

   puts(OK_MESSAGE);
   printDebug(database, debug);
}
