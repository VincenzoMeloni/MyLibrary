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

        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }

    free(tmp);
}


void crea_risposta_aggiungi_carrello(int comando, char *risposta,int maxCopie,int quantitaCorrente)
{
    switch(comando) {
    
        case ADD_CARRELLO_OK:
            sprintf(risposta, "Comando: %d | Il libro è stato aggiunto al carrello con successo! | Max Copie: %d | Quantita: %d", comando,maxCopie,quantitaCorrente);
            break;
            
        case ADD_CARRELLO_ERR:
            sprintf(risposta, "Comando: %d | Errore durante l'aggiunta del libro al carrello.", comando);
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

        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }
}


void crea_risposta_checkout(int comando, char *risposta,int maxPrestiti)
{
    switch(comando) {
        case CHECKOUT_OK:
            sprintf(risposta, "Comando: %d | Checkout completato con successo! | Max Prestiti aggiornati: %d", comando,maxPrestiti);
            break;

        case CHECKOUT_ERR:
            sprintf(risposta, "Comando: %d | Errore durante il checkout! Riprova più tardi. | Max Prestiti aggiornati: %d", comando,maxPrestiti);
            break;
            
        case MAXPRESTITI_ERR:
            sprintf(risposta, "Comando: %d | Errore durante il checkout! Hai superato il limite massimo di prestiti! | Max Prestiti aggiornati: %d", comando,maxPrestiti);
            break;
        
        case LIBRONONDISPONIBILE:
        sprintf(risposta, "Comando: %d | Errore! Alcuni libri nel carrello non sono più disponibili! | Max Prestiti aggiornati: %d", comando,maxPrestiti);
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
                    int quantita = atoi(PQgetvalue(res, i , 5));
                    sprintf(tmp + strlen(tmp), "ID: %d, Titolo: %s, Autore: %s, Copie disponibili: %d, Genere: %s, Quantita: %d\n",
                            id,  // id libro
                            PQgetvalue(res, i, 1), // Titolo
                            PQgetvalue(res, i, 2), // Autore
                            copie, // Copie disponibili
                            PQgetvalue(res, i, 4),  //Genere
                            quantita); 
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

void crea_risposta_visualizza_prestiti(int comando, PGresult *res, char *risposta)
{
    int num = PQntuples(res);
    char *tmp = malloc(sizeof(char) * 1000);
    
    if (tmp == NULL) {
        sprintf(risposta, "Errore nell'allocazione della memoria.");
        return;
    }
    tmp[0] = '\0';

    switch (comando) {
        case VISUALIZZA_PRESTITI_OK: {

                sprintf(risposta, "Comando: %d | Risultati dei libri presi in prestito:\n", comando);
                for (int i = 0; i < num; i++) {
                    int id_libro = atoi(PQgetvalue(res, i, 0));
                    int copie_prestito = atoi(PQgetvalue(res, i, 4)); 
                    char *data_scadenza = PQgetvalue(res, i, 5);
                    
                    sprintf(tmp + strlen(tmp), 
                            "ID: %d, Titolo: %s, Autore: %s, Genere: %s, Copie in prestito: %d, Data di scadenza: %s\n",
                            id_libro,                             
                            PQgetvalue(res, i, 1),                // Titolo
                            PQgetvalue(res, i, 2),                // Autore
                            PQgetvalue(res, i, 3),                // Genere
                            copie_prestito,                      
                            data_scadenza);                       
                }
                strcat(risposta, tmp);
            break;
        }

        case VISUALIZZA_PRESTITI_ERR:
            sprintf(risposta, "Comando: %d | Errore durante il recupero dei prestiti.", comando);
            break;

        default:
            sprintf(risposta, "Comando sconosciuto: %d", comando);
            break;
    }

    free(tmp);
}

