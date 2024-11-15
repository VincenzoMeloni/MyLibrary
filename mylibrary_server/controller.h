#ifndef CONTROLLER_H
#define CONTROLLER_H

int processa_input_client(char *richiesta, char *risposta, int socket_fd);
void loginC(char *dati, char *risposta);
void reg(char *dati, char *risposta);
void cerca_libro(char *dati, char *risposta);
void cerca_libro_genere(char *dati, char *risposta);
void cerca_libro_disponibile(char *dati, char *risposta);
void aggiungiCarrello(char *dati, char *risposta);
void remCarrello(char *dati, char *risposta);
void visualizza_carrello(char *dati, char *risposta);
void check_out(char *dati, char *risposta);
void modpass(char *dati, char *risposta);
int gestisci_logout(int socket_fd);

#endif
