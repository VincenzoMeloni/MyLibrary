#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h> 

#include "controller.h"
#include "richieste.h"
#include "server.h"

#define PORT 5050
#define MAX_CONN 100
#define BUFFER_SIZE 4096


void *gestisci_client(void *arg){

int sock = *(int *) arg;
char buffer[BUFFER_SIZE];
char risposta[BUFFER_SIZE];
int read_size,comando;

while((read_size = recv(sock,buffer,BUFFER_SIZE - 1, 0)) > 0){
buffer[read_size] = '\0';

printf("Richiesta ricevuta: %s\n",buffer);

comando = processa_input_client(buffer, risposta , sock);

if(strlen(risposta) > 0)
  if (send(sock, risposta, strlen(risposta), 0) < 0) {
                perror("Errore nell'invio della risposta");
                break;
            }
else
   printf("Server: Errore durante il recupero delle richieste.\n");   
}

if(read_size < 0)
  perror("Server: Errore durante la lettura della socket!\n");
  
  chiudi_socket(sock);
  
  pthread_exit(NULL);

}

void chiudi_socket(int socket_fd){

if (close(socket_fd) < 0)
        perror("Errore durante la chiusura della socket");
    else
        printf("Socket %d chiusa con successo\n", socket_fd);

}

void starta_server() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;

    // Crea la socket del server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Errore nella creazione della socket");
        exit(EXIT_FAILURE);
    }

       printf("\nStarta_Server: Socket Creata con successo!\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Collega la socket al server
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Errore nel bind della socket");
        exit(EXIT_FAILURE);
    }
    
       printf("\nStarta_Server: Bind riuscito!\n");

    // Ascolta le connessioni in ingresso
    if (listen(server_fd, MAX_CONN) < 0) {
        perror("Errore nell'ascolto delle connessioni");
        exit(EXIT_FAILURE);
    }
    
    printf("Starta_Server: Server in ascolto sulla porta %d...\n", PORT);

    // Accetta le connessioni dei client
    while (1) {
        // Accetta una nuova connessione
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        
        if (client_fd < 0) {
            perror("Errore durante l'accettazione della connessione");
            continue; // Se c'è un errore nell'accept, continua il ciclo
        }
        printf("Server: Connessione accettata!\n");

        // Crea un nuovo thread per gestire la connessione del client
        if (pthread_create(&thread_id, NULL, gestisci_client, (void *)&client_fd) < 0) {
            perror("Errore nella creazione del thread");
            continue; // Se c'è un errore nella creazione del thread, continua il ciclo
        }

        // detach del thread per non dover aspettare il termine prima di continuare
        pthread_detach(thread_id);
    }

    close(server_fd);
}

// Funzione main
int main() {
    starta_server();  // Avvia il server
    return 0;
}
