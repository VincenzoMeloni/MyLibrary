#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "richieste.h"

void crea_risposta_login(int comando, char *username,int user_id,int max_prestiti,char *risposta)
{
    switch (comando) {
        case LOGIN_OK:
            sprintf(risposta, "Comando: %d | Utente: %s loggato con successo! | ID: %d | Max Prestiti: %d",LOGIN_OK, username, user_id, max_prestiti);
            break;
        case LOGIN_ERR:
            sprintf(risposta, "Comando: %d | Errore durante il Login", comando);
            break;
        case LOGINNONTROVATO:
            sprintf(risposta, "Comando: %d | Utente: %s non registrato, registrati!", comando, username);
            break;
        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }
}

void crea_risposta_registrazione(int comando, char *risposta, char *username)
{
    switch (comando) {
        case REG_OK:
            sprintf(risposta, "Comando: %d | Utente: %s registrato con successo!", comando, username);
            break;
        case REG_ERR:
            sprintf(risposta, "Comando: %d | Errore durante la registrazione", comando);
            break;
        case GIAREGISTRATO:
            sprintf(risposta, "Comando: %d | Attenzione! Utente: %s è già registrato, effettua il login!", comando, username);
            break;
        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }
}

void crea_risposta_cerca_libro(int comando, PGresult *res, char *risposta)
{
   int num = PQntuples(res);
   char *tmp = malloc(sizeof(char) * 1000);
    if (tmp == NULL) {
        sprintf(risposta, "Errore nell'allocazione della memoria.");
        return;
    }
    tmp[0] = '\0'; 
   
     switch(comando) {
        case CERCA_LIBRO_OK: {
            if (num == 0)
                sprintf(risposta, "Comando: %d | Nessun libro trovato.", comando);
            else {
                sprintf(risposta, "Comando: %d | Risultato ricerca libri:\n",comando);
                for (int i = 0; i < num; i++) {
                    int copie = atoi(PQgetvalue(res, i, 3));
                    int id = atoi(PQgetvalue(res, i, 0));
                    sprintf(tmp + strlen(tmp), "ID: %d ,Titolo: %s, Autore: %s, Copie disponibili: %d, Genere: %s\n", 
                            id, //ID
                            PQgetvalue(res, i, 1), // Titolo
                            PQgetvalue(res, i, 2), // Autore
                            copie, // Num_copie_disponibili
                            PQgetvalue(res, i, 4)); //Genere
                }
                strcat(risposta, tmp);
            }
            break;
        }

        case CERCA_LIBRO_ERR:
            sprintf(risposta, "Comando: %d | Errore durante la ricerca del libro.", comando);
            break;

        case LIBRONONTROVATO:
            sprintf(risposta, "Comando: %d | Nessun libro corrisponde ai criteri di ricerca.", comando);
            break;

        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }
    
    free(tmp);
}

void crea_risposta_cerca_libro_genere(int comando, PGresult *res, char *risposta)
{
    int num = PQntuples(res);
    char *tmp = malloc(sizeof(char) * 1000); 

    if (tmp == NULL) {
        sprintf(risposta, "Errore nell'allocazione della memoria.");
        return;
    }

    tmp[0] = '\0';  

    switch(comando) {
        case CERCA_LIBRO_GENERE_OK: {
            if (num == 0) {
                sprintf(risposta, "Comando: %d | Nessun libro trovato per il genere.", comando);
            } else {
                sprintf(risposta, "Comando: %d | Risultato ricerca libri genere:\n",comando);
                for (int i = 0; i < num; i++) {
                    int copie = atoi(PQgetvalue(res, i, 3)); 
                    int id = atoi(PQgetvalue(res, i, 0));
                    sprintf(tmp + strlen(tmp), "ID: %d ,Titolo: %s, Autore: %s, Copie disponibili: %d, Genere: %s\n", 
                            id, //ID
                            PQgetvalue(res, i, 1),  // Titolo
                            PQgetvalue(res, i, 2),  // Autore
                            copie,      // Copie disponibili
                            PQgetvalue(res, i, 4)); // Genere
                }
                strcat(risposta, tmp);  
            }
            break;
        }

        case CERCA_LIBRO_GENERE_ERR:
            sprintf(risposta, "Comando: %d | Errore durante la ricerca del libro per genere.", comando);
            break;

        case LIBRONONTROVATO:
            sprintf(risposta, "Comando: %d | Nessun libro corrisponde al genere richiesto.", comando);
            break;

        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }

    free(tmp);
}


void crea_risposta_cerca_libro_disponibili(int comando, PGresult *res, char *risposta)
{
    int num = PQntuples(res);
    char *tmp = malloc(sizeof(char) * 1000);

    if (tmp == NULL) {
        sprintf(risposta, "Errore nell'allocazione della memoria.");
        return;
    }

    tmp[0] = '\0';

    switch(comando) {
        case CERCA_LIBRO_DISPONIBILI_OK: {
            if (num == 0) {
                sprintf(risposta, "Comando: %d | Nessun libro disponibile trovato.", comando);
            } else {
                sprintf(risposta, "Comando: %d | Risultato ricerca libri disponibili:\n",comando);
                for (int i = 0; i < num; i++) {
                    int copie = atoi(PQgetvalue(res, i, 3));
                    int id = atoi(PQgetvalue(res, i, 0));
                     sprintf(tmp + strlen(tmp), "ID: %d ,Titolo: %s, Autore: %s, Copie disponibili: %d, Genere: %s\n", 
                                id, //ID
                                PQgetvalue(res, i, 1),  // Titolo
                                PQgetvalue(res, i, 2),  // Autore
                                copie,     // Copie disponibili
                                PQgetvalue(res, i, 4)); //Genere
                }
                strcat(risposta, tmp);  
            }
            break;
        }

        case CERCA_LIBRO_DISPONIBILI_ERR:
            sprintf(risposta, "Comando: %d | Errore durante la ricerca dei libri disponibili.", comando);
            break;

        case LIBRONONTROVATO:
            sprintf(risposta, "Comando: %d | Nessun libro disponibile trovato.", comando);
            break;

        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }

    free(tmp);
}


void crea_risposta_aggiungi_carrello(int comando, char *risposta)
{
    switch(comando) {
    
        case ADD_CARRELLO_OK:
            sprintf(risposta, "Comando: %d | Il libro è stato aggiunto al carrello con successo!", comando);
            break;
            
        case ADD_CARRELLO_ERR:
            sprintf(risposta, "Comando: %d | Errore durante l'aggiunta del libro al carrello.", comando);
            break;
            
        case LIBRODUPLICATO:
            sprintf(risposta, "Comando: %d | Errore! Libro già presente nel carrello!", comando);
            break;
            
        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }
}


void crea_risposta_rimuovi_carrello(int comando, char *risposta)
{
    switch(comando) {
        case REM_CARRELLO_OK:
            sprintf(risposta, "Comando: %d | Il libro è stato rimosso dal carrello con successo!", comando);
            break;

        case REM_CARRELLO_ERR:
            sprintf(risposta, "Comando: %d | Errore durante la rimozione del libro dal carrello.", comando);
            break;

        case LIBRONONTROVATO:
            sprintf(risposta, "Comando: %d | Il libro non è presente nel carrello.", comando);
            break;

        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }
}


void crea_risposta_checkout(int comando, char *risposta)
{
    switch(comando) {
        case CHECKOUT_OK:
            sprintf(risposta, "Comando: %d | Checkout completato con successo! I libri sono stati presi in prestito!.", comando);
            break;

        case CHECKOUT_ERR:
            sprintf(risposta, "Comando: %d | Errore durante il checkout. Riprova più tardi.", comando);
            break;

        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }
}


void crea_risposta_modifica_password(int comando, char *risposta)
{
    switch(comando) {
        case MODPASS_OK:
            sprintf(risposta, "Comando: %d | Password modificata con successo.", comando);
            break;

        case MODPASS_ERR:
            sprintf(risposta, "Comando: %d | Errore nella modifica della password. Riprova più tardi.", comando);
            break;
            
       case VECCHIAPASSWORDERRATA:
        sprintf(risposta, "Comando: %d | Errore! Vecchia password non corretta!", comando);
            break;
       
        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }
}

void crea_risposta_visualizza_carrello(int comando, PGresult *res, char *risposta)
{
    int num = PQntuples(res);
    char *tmp = malloc(sizeof(char) * 1000);
    
    if (tmp == NULL) {
        sprintf(risposta, "Errore nell'allocazione della memoria.");
        return;
    }
    tmp[0] = '\0';

    switch (comando) {
        case VISUALIZZA_CARRELLO_OK:{
               
               sprintf(risposta, "Comando: %d | Risultati dei libri nel carrello:\n", comando);
                for (int i = 0; i < num; i++) {
                    int copie = atoi(PQgetvalue(res, i, 3));
                    int id = atoi(PQgetvalue(res, i, 0));
                    sprintf(tmp + strlen(tmp), "ID: %d, Titolo: %s, Autore: %s, Copie disponibili: %d, Genere: %s\n",
                            id,  // id libro
                            PQgetvalue(res, i, 1), // Titolo
                            PQgetvalue(res, i, 2), // Autore
                            copie, // Copie disponibili
                            PQgetvalue(res, i, 4)); //Genere
                }
                strcat(risposta, tmp);
            break;
        }
        
        case VISUALIZZA_CARRELLO_ERR:
            sprintf(risposta, "Comando: %d | Errore durante il recupero del carrello.", comando);
            break;

        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }

    free(tmp);
}
