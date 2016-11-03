#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdbool.h>

typedef struct Database Database;

// Alkouje pamięć oraz inicjuje strukturę danych.
Database *initializeDatabase();

// Zwalnia pamięć zajmowaną przez database.
void deleteDatabase(Database *database);

// Dodaje informację o chorobie pacjenta o nazwisku name.
void newDiseaseEnterDescription(char *name, char *description, Database *database, bool debug);

/* Dodaje informację o chorobie pacjenta o nazwisku name1.
 * Opis nowej choroby jest taki sam, jak
 * aktualny opis ostatnio zarejestrowanej choroby pacjenta o nazwisku name2. */
void newDiseaseCopyDescription(char *name1, char *name2, Database *database, bool debug);

// Aktualizuje opis n-tej choroby pacjenta o nazwisku name.
void changeDescription(char *name, int n, char *description, Database *database, bool debug);

// Wypisuje na standardowe wyjście opis n-tej choroby pacjenta o nazwisku name.
void printDescription(char *name, int n, Database *database, bool debug);

// Usuwa historię chorób pacjenta o nazwisku name.
void deletePatientData(char *name, Database *database, bool debug);

#endif // STRUCTURE_H
