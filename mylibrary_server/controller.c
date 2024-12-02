#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "richieste.h"
#include "database.h"
#include "controller.h"
#include "server.h"

int processa_input_client(char *richiesta, char *risposta, int socket_fd)
{

 int comando = atoi(strtok(richiesta,"|"));
 char *dati = strtok(NULL,"");
 
 switch(comando){
 
 case LOGIN:
   loginC(dati,risposta);
   break;
   
 case REG:
   reg(dati,risposta);
   break;
   
 case CERCA_LIBRO:
   cerca_libro(dati,risposta);
   break;
   
 case CERCA_LIBRO_GENERE:
   cerca_libro_genere(dati,risposta);
   break;
   
 case CERCA_LIBRO_DISPONIBILI:
   cerca_libro_disponibile(dati,risposta);
   break;
   
 case AGGIUNGI_CARRELLO:
   aggiungiCarrello(dati,risposta);
   break;
   
 case RIMUOVI_CARRELLO:
   remCarrello(dati,risposta);
   break;
   
 case VISUALIZZA_CARRELLO:
   visualizza_carrello(dati,risposta);
   break;
   
 case CHECKOUT:
   check_out(dati,risposta);
   break;

 case MODPASS:
   modpass(dati,risposta);
   break;
   
default:
   sprintf(risposta, "Comando non riconosciuto: %d", comando);
   break;
 
 }
 
 return comando;
 
}


void loginC(char *dati, char *risposta){

char *username = strtok(dati,"|");
char *password = strtok(NULL,"|");

    int user_id = 0;
    int max_prestiti = 0;
    int esito = login(username, password,&user_id,&max_prestiti);


if (esito == 1)
     crea_risposta_login(LOGIN_OK,username,user_id,max_prestiti,risposta);
else if (esito == 0)
    crea_risposta_login(LOGINNONTROVATO,username,0,0,risposta);
else 
   crea_risposta_login(LOGIN_ERR,username,0,0,risposta);

}

void reg(char *dati, char *risposta){

char *username = strtok(dati,"|");
char *password = strtok(NULL,"|");

int check_registrato = check(username);

if(check_registrato == 1)
   crea_risposta_registrazione(GIAREGISTRATO,risposta,username);
else{
   int esito = registra_utente(username,password);
   if(esito == 1)
   crea_risposta_registrazione(REG_OK,risposta,username);
   else
   crea_risposta_registrazione(REG_ERR,risposta,username);
}
   
}

void cerca_libro(char *dati, char *risposta){

char *testo = strtok(dati,"|");

PGresult *esito = cercaLibro(testo);

if(esito != NULL)
 crea_risposta_cerca_libro(CERCA_LIBRO_OK,esito,risposta);
else
 crea_risposta_cerca_libro(CERCA_LIBRO_ERR,esito,risposta);

}

void cerca_libro_genere(char *dati, char *risposta){

char *genere = strtok(dati,"|");

PGresult *esito = cercaLibroPerGenere(genere);

if(esito != NULL)
  crea_risposta_cerca_libro_genere(CERCA_LIBRO_GENERE_OK,esito,risposta);
else
  crea_risposta_cerca_libro_genere(CERCA_LIBRO_GENERE_ERR,esito,risposta);
  
  
}

void cerca_libro_disponibile(char *dati, char *risposta){

PGresult *esito = cercaLibroDisponibile();

if(esito != NULL)
  crea_risposta_cerca_libro_disponibili(CERCA_LIBRO_DISPONIBILI_OK,esito,risposta);
else
  crea_risposta_cerca_libro_disponibili(CERCA_LIBRO_DISPONIBILI_ERR,esito,risposta);
  
  
}

void aggiungiCarrello(char *dati, char *risposta){

int userId = atoi(strtok(dati,"|"));
int libroId = atoi(strtok(NULL,"|"));
int maxCopie = 0;
int quantitaCorrente = 0;

int esito = addCarrello(userId,libroId,&maxCopie,&quantitaCorrente);


if(esito == 1)
crea_risposta_aggiungi_carrello(ADD_CARRELLO_OK,risposta,maxCopie,quantitaCorrente);
else 
crea_risposta_aggiungi_carrello(ADD_CARRELLO_ERR,risposta,maxCopie,quantitaCorrente);

}

void remCarrello(char *dati, char *risposta){

int userId = atoi(strtok(dati,"|"));
int libroId = atoi(strtok(NULL,"|"));

int esito = removeCarrello(userId,libroId);

if(esito == 1)
crea_risposta_rimuovi_carrello(REM_CARRELLO_OK,risposta);
else
crea_risposta_rimuovi_carrello(REM_CARRELLO_ERR,risposta);

}


void visualizza_carrello(char *dati, char *risposta){

int userId = atoi(strtok(dati,"|"));

PGresult *esito = visualizzaCarrello(userId);

if(esito != NULL)
crea_risposta_visualizza_carrello(VISUALIZZA_CARRELLO_OK,esito,risposta);
else
crea_risposta_visualizza_carrello(VISUALIZZA_CARRELLO_ERR,esito,risposta);

}


void check_out(char *dati, char *risposta){

int userId = atoi(strtok(dati,"|"));

int maxPrestiti = 0;
int esito = checkout(userId,&maxPrestiti);

if(esito == 1)
crea_risposta_checkout(CHECKOUT_OK,risposta,maxPrestiti);
else if(esito == 3)
crea_risposta_checkout(MAXPRESTITI_ERR,risposta,maxPrestiti);
else if(esito == 7)
crea_risposta_checkout(LIBRONONDISPONIBILE,risposta,maxPrestiti);
else
crea_risposta_checkout(CHECKOUT_ERR,risposta,maxPrestiti);


}

void modpass(char *dati, char *risposta){

int userId = atoi(strtok(dati,"|"));
char *oldPass = strtok(NULL,"|");
char *newPass = strtok(NULL,"|");

int esito = aggiornaPassword(userId,oldPass,newPass);

if(esito == 1)
crea_risposta_modifica_password(MODPASS_OK,risposta);
else if(esito == 3)
crea_risposta_modifica_password(VECCHIAPASSWORDERRATA,risposta);
else
crea_risposta_modifica_password(MODPASS_ERR,risposta);

}

