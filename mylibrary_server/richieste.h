#ifndef RICHIESTE_H
#define RICHIESTE_H

#include <libpq-fe.h>

// server OK
#define LOGIN_OK 101
#define REG_OK 102
#define CERCA_LIBRO_OK 103
#define CERCA_LIBRO_GENERE_OK 104
#define CERCA_LIBRO_DISPONIBILI_OK 105
#define ADD_CARRELLO_OK 106
#define REM_CARRELLO_OK 107
#define CHECKOUT_OK 108
#define MODPASS_OK 109
#define VISUALIZZA_CARRELLO_OK 110

// server ERR
#define LOGIN_ERR 201
#define REG_ERR 202
#define CERCA_LIBRO_ERR 203
#define CERCA_LIBRO_GENERE_ERR 204
#define CERCA_LIBRO_DISPONIBILI_ERR 205
#define ADD_CARRELLO_ERR 206
#define REM_CARRELLO_ERR 207
#define CHECKOUT_ERR 208
#define MODPASS_ERR 209
#define VISUALIZZA_CARRELLO_ERR 210
#define MAXPRESTITI_ERR 211

// client
#define LOGIN 1
#define REG 2
#define CERCA_LIBRO 3
#define CERCA_LIBRO_GENERE 4
#define CERCA_LIBRO_DISPONIBILI 5
#define AGGIUNGI_CARRELLO 6
#define RIMUOVI_CARRELLO 7
#define CHECKOUT 8
#define MODPASS 9
#define VISUALIZZA_CARRELLO 10

//ERRORI GENERALI
#define LOGINNONTROVATO 301
#define GIAREGISTRATO 302
#define LIBRONONTROVATO 303
#define CARRELLOVUOTO 304
#define LIBRODUPLICATO 305
#define VECCHIAPASSWORDERRATA 306


//funzioni da mandare al client
void crea_risposta_login(int comando, char *username,int user_id,int max_prestiti,char *risposta);
void crea_risposta_registrazione(int comando, char *risposta, char *username);
void crea_risposta_cerca_libro(int comando, PGresult *res, char *risposta);
void crea_risposta_cerca_libro_genere(int comando, PGresult *res, char *risposta);
void crea_risposta_cerca_libro_disponibili(int comando, PGresult *res, char *risposta);
void crea_risposta_aggiungi_carrello(int comando, char *risposta);
void crea_risposta_rimuovi_carrello(int comando, char *risposta);
void crea_risposta_checkout(int comando, char *risposta);
void crea_risposta_modifica_password(int comando, char *risposta);
void crea_risposta_visualizza_carrello(int comando, PGresult *res, char *risposta);


#endif


