#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int svrSocket, cliSocket;
    struct sockaddr_in svrAddress, cliAddress;
    socklen_t cliLength = sizeof(cliAddress);
    char buffer[BUFFER_SIZE];

   
    svrSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (svrSocket == -1) {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

   
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = INADDR_ANY;
    svrAddress.sin_port = htons(PORT);

if (bind(svrSocket, (struct sockaddr*)&svrAddress, sizeof(svrAddress)) < 0) {
        perror("Error al vincular el socket del servidor");
        exit(EXIT_FAILURE);
    }

   
    if (listen(svrSocket, 1) < 0) {
        perror("Error al escuchar las conexiones entrantes");
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d...\n", PORT);

   
    cliSocket = accept(svrSocket, (struct sockaddr*)&cliAddress, &cliLength);
    if (cliSocket < 0) {
        perror("Error al establecer la conexión del cliente");
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead;
    while ((bytesRead = recv(cliSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        printf("Mensaje Recibido: %s\n", buffer);
    }

   if (bytesRead == 0) {
        printf("Cliente desconectado\n");
    } else {
        perror("Error al recibir datos del cliente");
    }

    close(cliSocket);
    close(svrSocket);

    return 0;
}
