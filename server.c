#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024
#define MAX_REQUESTS 5

void content(char* filename, int sockfd){
    char data[SIZE] = {0};

    FILE* file_stream = fopen(filename, "r");
    printf("Opened file.\n");

    while(fgets(data, SIZE, file_stream) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            perror("Error while sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

char* map(int file_number) {
    if (file_number == 1)
        return "quotes/avengers.txt";
    else if (file_number == 2)
        return "quotes/captain_america.txt";
    else return "quotes/spiderman.txt";
}

int main(){
    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("Could not create server socket.");
        exit(1);
    }
    printf("Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = 8080;

    int binding_status = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(binding_status < 0) {
        perror("Binding error.");
        exit(1);
    }
    printf("Binding successful.\n");

    if (listen(sockfd, MAX_REQUESTS) == 0){
        printf("Listening...\n");
    } else {
        perror("Server listening error.");
        exit(1);
    }

    char file_name;
    recv(sockfd, &file_name, SIZE, 0);

    char* wanted_file = map(file_name);

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
    content(wanted_file, new_sock);
    printf("File content sent.\n");

    printf("Data sent successfully.\n");

    return 0;
}
