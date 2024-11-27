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
int login(char *username, char *password,int *user_id,int *max_prestiti) {
    PGconn *conn = connetti(DB_STRING);
    PGresult *res;
    char query[1024];
    int exit = 2;  

    if (conn != NULL) {
        printf("Connessione DB ok!\n");

        sprintf(query, "SELECT id_utente,max_prestiti FROM utente WHERE username = $$%s$$ AND password = $$%s$$", username, password);
        res = PQexec(conn, query);

        if (PQresultStatus(res) == PGRES_TUPLES_OK) {
            if (PQntuples(res) > 0){ 
                *user_id = atoi(PQgetvalue(res, 0, 0));
                *max_prestiti = atoi(PQgetvalue(res, 0, 1)); 
                exit = 1;  // Login riuscito
            }else 
                exit = 0;  // Credenziali non valide   
      }else 
            printf("Errore nella query di login: %s\n", PQresultErrorMessage(res));

        PQclear(res);  
    } else
        printf("Error: Connessione al DB Fallita!\n");

    disconnetti(conn);
    return exit;
}

//checkgiaregistrato
int check(char *username){
    PGconn *conn = connetti(DB_STRING);
    PGresult *res;
    char query[1024];
    int exit = 2;  

    if (conn != NULL) {
        printf("Connessione DB ok!\n");

        sprintf(query, "SELECT * FROM utente WHERE username = $$%s$$", username);
        res = PQexec(conn, query);

        if (PQresultStatus(res) == PGRES_TUPLES_OK) {
            if (PQntuples(res) > 0){  
                exit = 1;  // utente già registrato
            }else 
                exit = 0;  // utente non registrato   
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
        
        if (testo == NULL || strlen(testo) == 0)
        sprintf(query, "SELECT id_libro, titolo, autore ,num_copie_disponibili,genere  FROM libro");
        else
        sprintf(query, "SELECT  id_libro, titolo, autore ,num_copie_disponibili,genere  FROM libro WHERE titolo ILIKE '%%%s%%' OR autore ILIKE '%%%s%%'", testo, testo);
        
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

        sprintf(query, "SELECT  id_libro, titolo, autore ,num_copie_disponibili,genere  FROM libro WHERE genere = '%s'", genere);
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

        const char *query = "SELECT  id_libro, titolo, autore ,num_copie_disponibili,genere FROM libro WHERE num_copie_disponibili > 0";
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
int addCarrello(int userId, int libroId) {
    PGconn *conn = connetti(DB_STRING);
    PGresult *res = NULL;
    char query[1024];
    int exit = 2;

    if (conn != NULL) {
        // Verifica se il libro è già presente nel carrello
        sprintf(query, "SELECT 1 FROM carrello WHERE user_id = %d AND libro_id = %d;", userId, libroId);
        res = PQexec(conn, query);

        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
            // Il libro è già presente nel carrello
            exit = 0; // Duplicato
        } else {
            // Il libro non è presente nel carrello, quindi inseriamolo
            sprintf(query, "INSERT INTO carrello (user_id, libro_id) "
                           "VALUES (%d, %d) "
                           "ON CONFLICT (user_id, libro_id) DO NOTHING;", 
                           userId, libroId);
            res = PQexec(conn, query);

            if (PQresultStatus(res) == PGRES_COMMAND_OK) {
                printf("Libro aggiunto al carrello con successo!\n");
                exit = 1; // Successo
            } else {
                printf("Errore nell'aggiunta al carrello: %s\n", PQresultErrorMessage(res));
                exit = 2; // Errore nell'esecuzione della query
            }
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
  sprintf(query,"SELECT l.id_libro, l.titolo, l.autore ,l.num_copie_disponibili, l.genere "
             "FROM carrello c "
             "JOIN libro l ON c.libro_id = l.id_libro "
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
int checkout(int userId) {
    PGconn *conn = connetti(DB_STRING);
    PGresult *res = NULL;
    char query[2048];
    int exit = 1;  // successo

    if (conn != NULL) {
        printf("Connessione al database riuscita!\n");

        // Query per verificare se il checkout supera il limite massimo
        sprintf(query, 
                "WITH prestiti_correnti AS ("
                "    SELECT COUNT(*) AS num_prestiti "
                "    FROM prestito "
                "    WHERE id_utente = %d AND restituito = FALSE"
                "), "
                "carrello_corrente AS ("
                "    SELECT COUNT(*) AS num_carrello "
                "    FROM carrello "
                "    WHERE user_id = %d AND is_checkout = FALSE"
                ") "
                "SELECT "
                "    (SELECT num_prestiti FROM prestiti_correnti) AS prestiti_attivi, "
                "    (SELECT num_carrello FROM carrello_corrente) AS libri_carrello, "
                "    u.max_prestiti "
                "FROM utente u "
                "WHERE u.id_utente = %d;",
                userId, userId, userId);

        res = PQexec(conn, query);

        if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
            printf("Errore nel calcolo del numero massimo di prestiti: %s\n", PQresultErrorMessage(res));
            PQclear(res);
            disconnetti(conn);
            return 2; // Errore durante il controllo
        }

        int prestitiAttivi = atoi(PQgetvalue(res, 0, 0));
        int libriCarrello = atoi(PQgetvalue(res, 0, 1));
        int maxPrestiti = atoi(PQgetvalue(res, 0, 2));

        if (prestitiAttivi + libriCarrello > maxPrestiti) {
            printf("Errore: Numero massimo di prestiti raggiunto (%d). Non puoi prendere altri %d libri.\n",
                   maxPrestiti, maxPrestiti - prestitiAttivi);
            PQclear(res);
            disconnetti(conn);
            return 3; // Superato il limite massimo di prestiti
        }

        PQclear(res);

     //setto is_checkout a true per far scatenare il trigger
      sprintf(query, 
                "UPDATE carrello "
                "SET is_checkout = TRUE "
                "WHERE user_id = %d AND is_checkout = FALSE;", 
                userId);
        res = PQexec(conn, query);

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Errore nell'aggiornamento del carrello: %s\n", PQresultErrorMessage(res));
            PQclear(res);
            disconnetti(conn);
            return 4;  // Errore nell'aggiornamento del carrello
        }

        printf("Checkout completato con successo!\n");
    } else {
        printf("Errore: Connessione al DB fallita!\n");
        exit = 2; // Errore connessione al DB
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
                "WHERE id_utente = %d AND password = $$%s$$;", 
                nuovaPassword, userId, vecchiaPassword);

        res = PQexec(conn, query);

        if (PQresultStatus(res) == PGRES_COMMAND_OK) {
            if (atoi(PQcmdTuples(res)) > 0) {
                printf("Password aggiornata con successo!\n");
                exit = 1; // Successo
            } else {
                printf("Errore: la vecchia password non è corretta o l'utente non esiste!\n");
                exit = 3; // Vecchia password non corretta
            }
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

