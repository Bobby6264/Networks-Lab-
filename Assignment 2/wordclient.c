/* CS39006: Computer Networks Laboratory
 * A sample datagram socket client program
 */

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PORT 5000
#define MAXLINE 1000

int main()
{
    char buffer[100];
    char datahello[100];
    char datafinish[100];
    char *message = "22CS30034_FILE2.txt";
    int sockfd, n;
    struct sockaddr_in servaddr;

    // clear servaddr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    // create datagram socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    FILE *file = NULL;
    while (1)
    {
        sendto(sockfd, message, MAXLINE, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        printf("length : %d\n", n);
        for (int i = 0; i < 5; i++)
        {
            datahello[i] = buffer[i];
        }
        datahello[5] = '\0';
        for (int i = 0; i < 6; i++)
        {
            datafinish[i] = buffer[i];
        }
        datafinish[6] = '\0';
        buffer[n] = '\0';
        if (strcmp(datahello, "HELLO") == 0)
        {
            file = fopen("getdata.txt", "w");
            if (file == NULL)
            {
                perror("Error creating file");
                exit(EXIT_FAILURE);
            }
            // fclose(file);
            message = "GETNEXT";
            // print data
            printf("\nReceived from Server:%s\n", buffer);
        }
        else if (strcmp(datafinish, "FINISH") == 0)
        {
            printf("\nReceived from Server:%s\n", buffer);

            if (file != NULL)
            {
                fclose(file);
            }
            break;
        }
        else if (strcmp(buffer, "File not found") == 0)
        {
            printf("\nReceived from Server:%s\n", buffer);
            break;
        }
        else
        {
            printf("\nData Receive: %s\n", buffer);

            if (file != NULL)
            {
                fprintf(file, "%s", buffer);
            }
        }
    }
    // printf("\nReceived from Server:%s\n",buffer);

    // close the descriptor
    close(sockfd);
}
