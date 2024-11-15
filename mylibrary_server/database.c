#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "database.h"

//connessione db
PGconn *connetti(char *connessione){

PGconn *conn = PQconnectdb(connessione);

if(PQstatus(conn) == CONNECTION_BAD){
  fprintf(stderr,"Connessione al db fallita: %s\n",PQerrorMessage(conn));

  PQfinish(conn);
  return NULL;
}

return conn;

}

//disconnessione db
void disconnetti(PGconn *conn){
 if(conn != NULL){
  PQfinish(conn);
  printf("Connessione al db chiusa!\n");
 }

}

//registrazione
int registra_utente(char *username, char *password){
PGconn *conn = connetti(DB_STRING);
PGresult *res;
char query[1024],error[1024];
int exit = 2;

if(conn != NULL){
         sprintf(query,"INSERT INTO utente (username, password) VALUES ($$%s$$,$$%s$$)",username,password);
         res = PQexec(conn,query);
         strcpy(error, PQresultErrorMessage(res));
         if(strlen(error) > 0){
         printf("%s\n",error);
         printf("Errore durante la registrazione!\n");
         exit = 0;
         }
         else{
        printf("Utente registrato con successo!\n");
        exit = 1;
        }
        PQclear(res);
   }
   else
      printf("Error: Connessione al DB Fallita!\n");
      
   disconnetti(conn);
   return exit;
}

//login
int login(char *username, char *password) {
    PGconn *conn = connetti(DB_STRING);
    PGresult *res;
    char query[1024];
    int exit = 2;  

    if (conn != NULL) {
        printf("Connessione DB ok!\n");

        sprintf(query, "SELECT * FROM utente WHERE username = $$%s$$ AND password = $$%s$$", username, password);
        res = PQexec(conn, query);

        if (PQresultStatus(res) == PGRES_TUPLES_OK) {
            if (PQntuples(res) > 0) 
                exit = 1;  // Login riuscito
            else 
                exit = 0;  // Credenziali non valide   
      }else 
            printf("Errore nella query di login: %s\n", PQresultErrorMessage(res));

        PQclear(res);  
    } else
        printf("Error: Connessione al DB Fallita!\n");

    disconnetti(conn);
    return exit;
}


//cercaTutto
PGresult* cercaLibro(char *testo) {
    PGconn *conn = connetti(DB_STRING);
    PGresult *res = NULL;
    char query[1024];

    if (conn != NULL) {
        printf("Connessione DB ok!\n");

        sprintf(query, "SELECT * FROM libro WHERE titolo LIKE '%%%s%%' OR autore LIKE '%%%s%%'", testo, testo);
        res = PQexec(conn, query);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("Errore nella ricerca del libro: %s\n", PQresultErrorMessage(res));
            PQclear(res);
            res = NULL;
        }
    } else 
        printf("Errore: Connessione al DB fallita!\n");

    disconnetti(conn);
    return res;
}


//cerca libro per genere
PGresult* cercaLibroPerGenere(char *genere) {
    PGconn *conn = connetti(DB_STRING);
    PGresult *res = NULL;
    char query[1024];

    if (conn != NULL) {
        printf("Connessione DB ok!\n");

        sprintf(query, "SELECT * FROM libro WHERE genere = '%s'", genere);
        res = PQexec(conn, query);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("Errore nella ricerca dei libri per genere: %s\n", PQresultErrorMessage(res));
            PQclear(res);
            res = NULL;
        }
    } else
        printf("Errore: Connessione al DB fallita!\n");

    disconnetti(conn);
    return res;
}



//cerca libro disponibile
PGresult* cercaLibroDisponibile() {
    PGconn *conn = connetti(DB_STRING);
    PGresult *res = NULL;

    if (conn != NULL) {
        printf("Connessione DB ok!\n");

        const char *query = "SELECT * FROM libro WHERE num_copie_disponibili > 0";
        res = PQexec(conn, query);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("Errore nella ricerca dei libri disponibili: %s\n", PQresultErrorMessage(res));
            PQclear(res);
            res = NULL;
        }
    } else
        printf("Errore: Connessione al DB fallita!\n");

    disconnetti(conn);
    return res;
}


//aggiungi al carrello
int addCarrello(int userId,int libroId){
PGconn *conn = connetti(DB_STRING);
PGresult *res = NULL;
char query[1024];
int exit = 2;

if(conn != NULL){
  sprintf(query,"INSERT INTO carrello (user_id, libro_id) "
                 "SELECT %d, %d WHERE NOT EXISTS ("
                 "SELECT 1 FROM carrello WHERE user_id = %d AND libro_id = %d);",
                 userId, libroId, userId, libroId);
  res = PQexec(conn,query);
  
   if (PQresultStatus(res) == PGRES_COMMAND_OK) {
            if (PQntuples(res) > 0) {  
                printf("Libro aggiunto al carrello con successo!\n");
                exit = 1; // Successo
            } else {
                printf("Il libro è già presente nel carrello.\n");
                exit = 0; //errore duplicato
            }
        } else {
            printf("Errore nell'aggiunta al carrello: %s\n", PQresultErrorMessage(res));
            exit = 2; // Errore nell'esecuzione della query
        }

        PQclear(res);
    } else {
        printf("Errore: Connessione al DB fallita!\n");
        exit = 2; // Errore nella connessione al DB
    }

    disconnetti(conn);
    return exit;
}


//elimina dal carrello
int removeCarrello(int userId,int libroId){
PGconn *conn = connetti(DB_STRING);
PGresult *res = NULL;
char query[1024];
int exit = 2;

   if(conn != NULL){
     sprintf(query,"DELETE FROM carrello WHERE user_id = %d AND libro_id = %d;",
     userId,libroId);
     res = PQexec(conn,query);
     
     if(PQresultStatus(res) == PGRES_COMMAND_OK){
  printf("Libro ID : %d rimosso dal carrello con successo!\n",libroId);
  exit = 1; //successo
  }
  else
     printf("Errore nella rimozione dal carrello: %s\n",PQresultErrorMessage(res));
     PQclear(res);
}else
    printf("Errore: Connessione al DB fallita!\n");
     
disconnetti(conn);
return exit;
}

//visualizza carrello
PGresult *visualizzaCarrello(int userId){
PGconn *conn = connetti(DB_STRING);
PGresult *res = NULL;
char query[1024];

if(conn != NULL){
  sprintf(query,"SELECT l.id_libro, l.titolo, l.autore ,l.num_copie_disponibili "
             "FROM carrello c "
             "JOIN libro l ON c.libro_id = l.id "
             "WHERE c.user_id = %d;",userId);
  res = PQexec(conn,query);
   if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("Errore nella ricerca dei libri disponibili: %s\n", PQresultErrorMessage(res));
            PQclear(res);
            res = NULL;
        }
    } else 
        printf("Errore: Connessione al DB fallita!\n");
    

disconnetti(conn);
return res;
}

//checkout
int checkout(int userId){
    PGconn *conn = connetti(DB_STRING);
    PGresult *res = NULL;
    char query[1024];
    int exit = 1;  // successo

    if (conn != NULL) {
        printf("Connessione al database riuscita!\n");

        sprintf(query, "SELECT l.id_libro, l.num_copie_disponibili FROM carrello c "
                        "JOIN libro l ON c.libro_id = l.id_libro "
                        "WHERE c.user_id = %d;", userId);
        res = PQexec(conn, query);
        
        if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
            printf("Errore: Carrello vuoto o errore nella selezione dei libri.\n");
            exit = 3;  // Errore carrello vuoto || errore select
            PQclear(res);
            return exit;
        }

        int numLibri = PQntuples(res);

        // check copie disponibili
        for (int i = 0; i < numLibri; i++) {
            int libroId = atoi(PQgetvalue(res, i, 0));  // ID del libro
            int numCopieDisponibili = atoi(PQgetvalue(res, i, 4));  // Numero di copie disponibili

            if (numCopieDisponibili <= 0) {
                printf("Errore: Il libro con ID %d non ha copie disponibili!\n", libroId);
                exit = 4;  // Errore se il libro non ha copie disponibili
                PQclear(res);
                return exit;
            }
        }

        // tutti i libri disponibili -> update
        for (int i = 0; i < numLibri; i++) {
            int libroId = atoi(PQgetvalue(res, i, 0));

            sprintf(query, "UPDATE libro SET num_copie_disponibili = num_copie_disponibili - 1 "
                           "WHERE id_libro = %d AND num_copie_disponibili > 0;", libroId);
            PQclear(res);
            res = PQexec(conn, query);

            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                printf("Errore nell'aggiornamento delle copie per il libro ID %d: %s\n", libroId, PQresultErrorMessage(res));
                exit = 5;  // Errore nell'aggiornamento delle copie disponibili
                PQclear(res);
                return exit;
            }

            sprintf(query, "INSERT INTO prestito (user_id, libro_id) VALUES (%d, %d);", userId, libroId);
            PQclear(res);
            res = PQexec(conn, query);

            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                printf("Errore nell'inserimento del prestito per il libro ID %d: %s\n", libroId, PQresultErrorMessage(res));
                exit = 6;  // Errore nell'inserimento del prestito
                PQclear(res);
                return exit;
            }

            // rimuovi dal carrello
            if (removeCarrello(userId, libroId) != 1) {
                printf("Errore nella rimozione del libro ID %d dal carrello\n", libroId);
                exit = 7;  // Errore nella rimozione del libro dal carrello
                PQclear(res);
                return exit;
            }
        }
        printf("Checkout completato con successo!\n");
    } else {
        printf("Errore: Connessione al DB fallita!\n");
        exit = 2;  // Errore connessione al DB
    }

    PQclear(res);
    disconnetti(conn);
    return exit;
}

//aggiorna password
int aggiornaPassword(int userId, const char *vecchiaPassword, const char *nuovaPassword){
    PGconn *conn = connetti(DB_STRING);
    PGresult *res = NULL;
    char query[1024];
    int exit = 2; // errore connessione db

    if (conn != NULL) {
        printf("Connessione al database riuscita!\n");
        
        sprintf(query, 
                "UPDATE utente SET password = $$%s$$ "
                "WHERE id = %d AND password = $$%s$$;", 
                nuovaPassword, userId, vecchiaPassword);

        res = PQexec(conn, query);

        if (PQresultStatus(res) == PGRES_COMMAND_OK && PQntuples(res) > 0) {
            printf("Password aggiornata con successo!\n");
            exit = 1; // Successo
        } else if (PQresultStatus(res) == PGRES_COMMAND_OK && PQntuples(res) == 0) {
            printf("Errore: la vecchia password non è corretta!\n");
            exit = 3; // Vecchia password non corretta
        } else {
            printf("Errore nell'aggiornamento della password: %s\n", PQresultErrorMessage(res));
            exit = 4; // Errore nell'aggiornamento
        }

        PQclear(res);
    } else 
        printf("Errore: Connessione al DB fallita!\n");

    disconnetti(conn);
    return exit;
}

