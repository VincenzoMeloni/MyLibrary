#ifndef DATABASE_H
#define DATABASE_H

#include <libpq-fe.h>

#define DB_STRING "user=postgres dbname=MyLibrary"

PGconn *connetti(char *connessione);
void disconnetti(PGconn *conn);
int registra_utente(char *username, char *password);
int login(char *username, char *password);
PGresult *cercaLibro(char *testo);
PGresult *cercaLibroPerGenere(char *genere);
PGresult *cercaLibroDisponibile();
int addCarrello(int userId,int libroId);
int removeCarrello(int userId,int libroId);
PGresult *visualizzaCarrello(int userId);
int checkout(int userId);
int aggiornaPassword(int userId, const char *vecchiaPassword, const char *nuovaPassword);


#endif
