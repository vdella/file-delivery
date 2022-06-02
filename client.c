#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define SIZE 1024

void show_sent_content_from(int sockfd){
    /** Receives content from @param sockfd and prints it
     * to the end user. */
    int n;
    char buffer[SIZE];

    while (1) {
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0){
            break;
        }
        fprintf(stdout, "%s", buffer);
        bzero(buffer, SIZE);
    }
    printf("\n");
    return;
}

void request(char file_number, int sockfd){
    /** Sends a request to the server @param sockfd asking for
     * the content from @param filename.*/
    if (send(sockfd, &file_number, sizeof(file_number), 0) == -1) {
        perror("Could not send file.");
        exit(1);
    }
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        printf("Type the number of the wanted file.\n");
        printf("1. avengers.txt\n");
        printf("2. captain_america.txt\n");
        printf("3. spiderman.txt\n");
        exit(0);
    }

    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("Server socket unavailable.");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = 8080;

    int connection_status = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(connection_status == -1) {
        perror("Could not make connection.");
        exit(1);
    }
    printf("Connected to server.\n");

    request(*argv[1], sockfd);
    printf("Successful request.\n");

    show_sent_content_from(sockfd);

    printf("Closing the connection.\n");
    close(sockfd);

    return 0;
}
