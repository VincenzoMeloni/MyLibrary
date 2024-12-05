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


int addCarrello(int userId, int libroId, int *maxCopie, int *quantitaCorrente) {
    PGconn *conn = connetti(DB_STRING);
    PGresult *res = NULL;
    char query[1024];
    int exit = 2; // Default: errore generico

    if (conn != NULL) {
        // Recupera il numero massimo di copie disponibili per il libro
        sprintf(query, "SELECT num_copie_disponibili FROM libro WHERE id_libro = %d;", libroId);
        res = PQexec(conn, query);

        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
            *maxCopie = atoi(PQgetvalue(res, 0, 0));
            PQclear(res);

            // Controlla se il libro è già presente nel carrello
            sprintf(query, "SELECT quantita FROM carrello WHERE user_id = %d AND libro_id = %d;", userId, libroId);
            res = PQexec(conn, query);

            if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
                *quantitaCorrente = atoi(PQgetvalue(res, 0, 0));
                PQclear(res);

                // Aggiorna la quantità e ritorna il valore aggiornato
                sprintf(query, 
                    "UPDATE carrello SET quantita = quantita + 1 WHERE user_id = %d AND libro_id = %d RETURNING quantita;", 
                    userId, libroId);
                res = PQexec(conn, query);

                if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
                    *quantitaCorrente = atoi(PQgetvalue(res, 0, 0));
                    printf("Quantità aggiornata nel carrello: %d\n", *quantitaCorrente);
                    exit = 1; // Successo
                } else {
                    printf("Errore nell'aggiornamento della quantità nel carrello: %s\n", PQresultErrorMessage(res));
                    exit = 2; // Errore nell'esecuzione della query
                }
            } else {
                *quantitaCorrente = 0;
                PQclear(res);
                // Aggiungi libro al carrello con quantità 1 e ritorna il valore
                sprintf(query, 
                    "INSERT INTO carrello (user_id, libro_id, quantita) VALUES (%d, %d, 1) RETURNING quantita;", 
                    userId, libroId);
                res = PQexec(conn, query);

                if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
                    *quantitaCorrente = atoi(PQgetvalue(res, 0, 0));
                    printf("Libro aggiunto al carrello con successo! Quantità: %d\n", *quantitaCorrente);
                    exit = 1; // Successo
                } else {
                    printf("Errore nell'aggiunta al carrello: %s\n", PQresultErrorMessage(res));
                    exit = 2; // Errore nell'esecuzione della query
                }
            }
        } else {
            printf("Errore nel recupero delle copie disponibili: %s\n", PQresultErrorMessage(res));
            exit = 2; // Errore nella query di recupero delle copie
        }

        PQclear(res);
    } else {
        printf("Errore: Connessione al DB fallita!\n");
        exit = 2; // Errore di connessione
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
  sprintf(query,"SELECT l.id_libro, l.titolo, l.autore ,l.num_copie_disponibili, l.genere , quantita "
             "FROM carrello c "
             "JOIN libro l ON c.libro_id = l.id_libro "
             "WHERE c.user_id = %d AND l.num_copie_disponibili > 0;",userId);
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
int checkout(int userId, int *maxPrestiti) {
    PGconn *conn = connetti(DB_STRING);
    PGresult *res = NULL;
    char query[2048];
    int exit = 1;  // Successo

    if (conn != NULL) {
        printf("Connessione al database riuscita!\n");

        // Verifica se ci sono libri senza copie disponibili
        sprintf(query,
                "SELECT l.id_libro "
                "FROM carrello c "
                "JOIN libro l ON c.libro_id = l.id_libro "
                "WHERE c.user_id = %d AND c.is_checkout = FALSE AND l.num_copie_disponibili = 0;",
                userId);

        res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("Errore nella verifica delle copie disponibili: %s\n", PQresultErrorMessage(res));
            PQclear(res);
            disconnetti(conn);
            return 2;  // Errore durante il controllo
        }

        int libriNonDisponibili = PQntuples(res);
        if (libriNonDisponibili > 0) {
            printf("Attenzione: Alcuni libri nel carrello non sono più disponibili. Rimozione in corso...\n");
            for (int i = 0; i < libriNonDisponibili; i++) {
                int libroId = atoi(PQgetvalue(res, i, 0));
                int rimuovi = removeCarrello(userId, libroId);
                if (rimuovi != 1) {
                    printf("Errore durante la rimozione del libro ID: %d dal carrello.\n", libroId);
                    exit = 2; // Errore durante la rimozione
                }
            }
            printf("Rimozione completata. Verifica prestiti disponibili...\n");
            exit = 7;  // Copie terminate, non procedere
        }
        PQclear(res); // Liberiamo la memoria

        // Query per verificare il numero di prestiti attivi, libri nel carrello e max prestiti
        sprintf(query, 
                "WITH prestiti_correnti AS ("
                "    SELECT COUNT(*) AS num_prestiti "
                "    FROM prestito "
                "    WHERE id_utente = %d AND restituito = FALSE"
                "), "
                "carrello_corrente AS ("
                "    SELECT SUM(quantita) AS num_carrello "
                "    FROM carrello c "
                "    JOIN libro l ON c.libro_id = l.id_libro "
                "    WHERE c.user_id = %d AND c.is_checkout = FALSE AND l.num_copie_disponibili > 0"
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
            return 2;  // Errore durante il calcolo
        }

        int libriCarrello = atoi(PQgetvalue(res, 0, 1));
        *maxPrestiti = atoi(PQgetvalue(res, 0, 2));

        printf("Libri nel carrello (disponibili): %d\n", libriCarrello);  // Debug
        printf("Max prestiti: %d\n", *maxPrestiti);  // Debug

        // Verifica se il numero di libri nel carrello supera il numero massimo di prestiti
        if (libriCarrello > *maxPrestiti) {
            printf("Errore: Numero massimo di prestiti raggiunto (%d). Non puoi prendere altri libri.\n", *maxPrestiti);
            PQclear(res);
            disconnetti(conn);
            return 3;  // Superato il limite massimo di prestiti
        }

        printf("Checkout possibile. Libri nel carrello: %d, Max prestiti: %d\n",
                libriCarrello, *maxPrestiti);

        PQclear(res); // Liberiamo la memoria

        // Esegui il checkout aggiornando lo stato del carrello
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
        PQclear(res); 
        
        sprintf(query,
        "SELECT u.max_prestiti "
        "FROM utente u "
        "WHERE u.id_utente = %d;",
        userId);
        
        res = PQexec(conn, query);
if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
    printf("Errore nel recupero di max_prestiti: %s\n", PQresultErrorMessage(res));
    PQclear(res);
    disconnetti(conn);
    return 5;  // Errore durante il recupero del max_prestiti
}

*maxPrestiti = atoi(PQgetvalue(res, 0, 0));

printf("Nuovo max_prestiti: %d\n", *maxPrestiti);

PQclear(res);

    } else {
        printf("Errore: Connessione al DB fallita!\n");
        return 2;  // Errore connessione al DB
    }

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

// Visualizza prestiti
PGresult *visualizzaPrestiti(int userId) {
    PGconn *conn = connetti(DB_STRING);
    PGresult *res = NULL;
    char query[1024];

    if (conn != NULL) {
         sprintf(query, 
            "SELECT p.id_libro, l.titolo, l.autore, l.genere, COUNT(p.id_libro) AS copie_prestito, p.data_scadenza "
            "FROM prestito p "
            "LEFT JOIN libro l ON p.id_libro = l.id_libro "
            "WHERE p.id_utente = %d AND p.restituito = false "
            "GROUP BY p.id_libro, l.titolo, l.autore, l.genere, p.data_scadenza;", 
            userId);

        res = PQexec(conn, query);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("Errore nella ricerca dei prestiti: %s\n", PQresultErrorMessage(res));
            PQclear(res);
            res = NULL;
        }
    } else 
        printf("Errore: Connessione al DB fallita!\n");
    

    disconnetti(conn);
    return res;
}


