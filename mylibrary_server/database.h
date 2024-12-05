#ifndef DATABASE_H
#define DATABASE_H

#include <libpq-fe.h>

#define DB_STRING "user=postgres password=postgres dbname=MyLibrary"

PGconn *connetti(char *connessione);
void disconnetti(PGconn *conn);
int registra_utente(char *username, char *password);
int login(char *username, char *password,int *user_id,int *max_prestiti);
int check(char *username);
PGresult *cercaLibro(char *testo);
PGresult *cercaLibroPerGenere(char *genere);
PGresult *cercaLibroDisponibile();
int addCarrello(int userId,int libroId,int *maxCopie,int *quantitaCorrente);
int removeCarrello(int userId,int libroId);
PGresult *visualizzaCarrello(int userId);
int checkout(int userId,int *maxPrestiti);
int aggiornaPassword(int userId, const char *vecchiaPassword, const char *nuovaPassword);
PGresult *visualizzaPrestiti(int userId);

#endif
