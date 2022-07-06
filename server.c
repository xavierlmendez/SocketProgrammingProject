#include<stdio.h>

#include<stdlib.h>

#include<string.h>	//strlen

#include<sys/socket.h>

#include<arpa/inet.h>	//inet_addr

#include<unistd.h>	//write




int main(int argc , char *argv[])

{

    int socket_desc , client_sock , c , read_size;

    struct sockaddr_in server , client;

    char *client_message = NULL;

    client_message = malloc (2000* sizeof client_message);




    //Create socket

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);

    if (socket_desc == -1)

    {

        printf("Could not create socket");

    }

    puts("Socket created");




    //Prepare the sockaddr_in structure

    short port = *argv[1];

    printf("%u\n", (short)port);

    server.sin_family = AF_INET;

    server.sin_addr.s_addr = INADDR_ANY;

    server.sin_port = port;




    //Bind

    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)

    {

        //print the error message

        perror("bind failed. Error");

        return 1;

    }

    puts("bind done");

    int cont = 0;

    while(cont == 0) {

        //Listen

        listen(socket_desc, 3);




        //Accept and incoming connection




        c = sizeof(struct sockaddr_in);




        //accept connection from an incoming client

        client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t * ) & c);

        if (client_sock < 0) {

            perror("accept failed");

            return 1;

        }

        printf("worker %d: established connection with client %d#%d \n", client_sock, socket_desc, port);

        //Receive a message from client

        while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0) {

            //Send the message back to client

            printf("worker %d: received message from client %d", client_sock, socket_desc);

            write(client_sock, client_message, strlen(client_message));

            memset(client_message, 0, sizeof client_message);

        }




        if (read_size == 0) {

            printf("worker %d: disconnected from client %d", client_sock, socket_desc);

            fflush(stdout);

        } else if (read_size == -1) {

            perror("recv failed");

        }

    }

    return 0;

}
