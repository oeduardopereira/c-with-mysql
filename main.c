#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024


int main() {

    struct sockaddr_in address;
    int server_fd, new_socket, addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    const char *http_response ="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<!DOCTYPE html><html><body><h1>Ola! Este e um servidor em C.</h1></body></html>";

    // create socket 
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error to create socket\n");
        exit(EXIT_FAILURE);
    }
 
    // if the server shutdown, will say to SO kill the port and reopens imediatilly to server use it
    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0   ) {
        perror("Error on setsockopt\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // binds server with socket
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error to bind!\n");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0){
         perror("Error on listen!\n");
         exit(EXIT_FAILURE);
    }

    printf("Server is running on http://localhost:%d!\n", PORT);

    while(1) {

        printf("\nWaiting conn...\n");

        if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Error to accept conn!\n");
            continue;
        }

        read(new_socket, buffer, BUFFER_SIZE);
        printf("Received data!\n");

        send(new_socket, http_response, strlen(http_response), 0);
        printf("Reponse sent!\n");

        close(new_socket);
    }

    return 0;
}