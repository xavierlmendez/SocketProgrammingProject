#include<pthread.h>
#ifndef MAX
#define MAX 900
#endif
void * func(void* p_client_sock){
    int client_sock = *((int*)p_client_sock);
    int read_size, socket_desc;
    char *client_message = NULL;
    client_message = malloc (2000* sizeof client_message);
    free(p_client_sock);
    while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0) {
        //Send the message back to client
        printf("worker %d: received message from client %d\n", getpid(), getppid());
        printf("received this message: %s\n", client_message);
        int messageWordC = wordCount(client_message);
        int messageCharC = charCount(client_message)-1;

        char str1[100];
        strcpy(str1, client_message);
        //============================================
        char *newmessage;
        char buffer[MAX];

        if (asprintf(&newmessage, " %d %d", messageWordC, messageCharC) == -1) {
            perror("asprintf");

        } else {
            strcat(strcpy(buffer, str1), newmessage);
            free(newmessage);
        }

        //=============================================
        printf("sending back this message: %s\n", buffer);
        write(client_sock, buffer, strlen(buffer));
        memset(client_message, 0, sizeof client_message);
        messageWordC = 0;
        messageCharC = 0;
        char check[] = "exit";
        if((strcmp(check, client_message))==0){
            return NULL;
        }
        if (read_size == 0) {
            printf("worker %d: disconnected from client %d\n", getpid(), getppid());
            fflush(stdout);
            return NULL;
        } else if (read_size == -1) {
            perror("recv failed");
            return NULL;
        }
    }
}
int wordCount(const char sentence[ ]){
    int count = 0;
    const char* str = sentence;
    int inword = 0;

    do switch(*str) {
            case '\0':
            case ' ': case '\t': case '\n': case '\r':
                if (inword) { inword = 0; count++; }
                break;
            default: inword = 1;
        } while(*str++);

    return count;
}

int charCount(const char sentence[]){
    int count = 0;
    const char* str = sentence;
    int i;
    for(i = 0; str[i] != '\0'; i++){
        if(str[i]!=' '){
            count++;
        }
    }
    return count;
}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;

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
        printf("worker %d: established connection with client %d#%d \n", getpid(), getppid(), port);
        //Receive a message from client
        pthread_t t;
        int *pclient = malloc(sizeof(int));
        *pclient = client_sock;
        pthread_create(&t, NULL, func, pclient);


    }
    return 0;
}
