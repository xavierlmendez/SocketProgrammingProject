#include<stdio.h>
#include<string.h> //strlen
#include<stdlib.h> //strlen
#include<sys/socket.h>
#include<arpa/inet.h>  //inet_addr
#include<unistd.h> //write
#include<netdb.h>
#include<errno.h>
#include<pthread.h> //for threading , link with lpthread
#define MAX 80
int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ( (he = gethostbyname( hostname ) ) == NULL)
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++)
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {

    int sock;
    short port = *argv[2];
    char *message = NULL;
    message = malloc (1000* sizeof *message);
    char *server_reply = NULL;
    server_reply = malloc (2000* sizeof server_reply);
    char *hostname = argv[1];
    char* ip = malloc(sizeof(char*)*100);
    hostname_to_ip(hostname, ip);
    printf("%s resolved to %s", hostname, ip);
    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        printf("could not create socket");
    }
    //puts("socket created");
    //printf("Hello, World!\n");
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = port;
    if (connect( sock, (struct sockaddr *)&server, sizeof(server))< 0){
        perror("connect failed. Error");
        return 1;
    }

    printf(" established connection with server %s\n", ip);

    //keep communicating with server
    while(1){
        //printf("Enter message : ");

        fgets(message, 30, stdin); //gets user command
        //scanf("%s" , message);
        const char *info = message;./
        printf("message is (client side): %s\n", message);



        //send some data
        if(send(sock, info, strlen(message), 0) <0){
            puts("Send failed");
            return 1;
        }
        puts("send success");
        //receive reply from server
        if( recv(sock, server_reply, 2000, 0)<0){
            puts("recv failed");
            break;
        }
        puts("Server reply :");
        puts(server_reply);
        char check[] = "exit";

        if(strcpy(message, check)==0){
            return 0;
        }
        memset(message, 0, sizeof message);
        memset(server_reply, 0, sizeof server_reply);

    }
    function(sock);
    close(sock);
    return 0;
}
