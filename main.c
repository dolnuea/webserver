/*
 * Project 4: Web server
 * Group: Starling Sanchez, Luna Dagci
 * This group project has been done in equal collaboration from both partners.
 * This program is a webserver which uses port 8000, and handles request with one thread per request.
 * This program receives the path entered during the program call as a parameter. EX: ./webserver /home/phipps/secret/data.html
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <libgen.h>
#include <pthread.h>

#define SA struct sockaddr
#define PORT 8000       //assigned by professor
#define SIZE 1000       //Used for buffer
#define true 1
#define false 0
char* path;             //makes the given path global
char* fileName;         //used to make the name of the file available to all

int contentLength(char*);   //finds the length/size of the file
int pathFinder(char *);     //checks if the given path is available
void * handle_request(void *); //handles the request from the client
void response_200OK(int, long);//sends the 200ok response
void response_404NOTFOUND(int, long);//sends the 404 not found response

int main(int argc, char *argv[]){
    if(argc == 1){ //path is missing
        printf("No Parameters were entered\n");
        return 0;
    }
    else{
        path = argv[1];
        int server_socket, clientfd, info;  //creates socket
        struct sockaddr_in servaddr;        //creates socket structre

        server_socket = socket(AF_INET, SOCK_STREAM, 0);//creates the socket
        if( server_socket < 0 ){//verifies the socket creation
            printf("Socket Creation Failed!!!\n");
            exit(-1);
        }
        bzero(&servaddr, sizeof(servaddr));//Assigns the socket structures with null values.
        servaddr.sin_family = AF_INET;//Type(same as in the domain of socket creation)
        servaddr.sin_addr.s_addr = INADDR_ANY;//IP address
        servaddr.sin_port = htons(PORT);//Port

        //binds the socket to the given IP address
        if( (bind(server_socket, (SA *) &servaddr, sizeof(servaddr))) < 0 ){
            printf("Socket Bind Failed!!\n");
            exit(-1);
        }
        if( (listen(server_socket, 10)) <0 ){//allows the server to listen and verify connection
            printf("Listen failed!!!\n");
            exit(-1);
        }
        //handles the connection between client and server
        int length_servaddr = sizeof(servaddr);
        while(true){
            clientfd = accept(server_socket, (SA *) &servaddr, &length_servaddr);//accepts the connection
            if(clientfd < 0){
                printf("failed to accept");
                exit(-1);
            }
            int *ptrClient = (int*)malloc(sizeof(int));
            pthread_t client_thread; //create a thread per request
            *ptrClient = clientfd;//sets the connection to the thread
            pthread_create(&client_thread, NULL, handle_request, ptrClient);//begin work for request
        }

    }
    return 0;
}
/*
 * This function is the thread's job: handles request from the client
 */
void * handle_request( void* ptrClient){
    int client_socket;
    client_socket = *((int*)ptrClient);
    int info;
    char recvmsg[SIZE];                 //Receives the request message
    memset(recvmsg, 0, SIZE);
    info = read(client_socket, recvmsg, SIZE-1);//reads the info from the client
    while( info > 0){//reads the request
        printf("%s", recvmsg);
        if(recvmsg[info-1] == '\n')
            break;
        memset(recvmsg, 0, SIZE);
    }
    long length = contentLength(path);//finds the length of the file
    int pathCheck = pathFinder(path);//finds if the file exist or not
    if(pathCheck == true){//sends the response to the client(webserver)
        response_200OK(client_socket, length);
    }else
        response_404NOTFOUND(client_socket, length);
    close(client_socket);//closes the clientfd
}
/*
 * This function sends the HTTP/1.0 200 OK client response
 */
void response_200OK(int client_socket, long length){//sends the 200ok response
    int file;
    int info;
    char buffer[SIZE];
    char sendmessage[SIZE];
    snprintf(buffer, sizeof(buffer), "HTTP/1.0 200 OK\r\n" );//set the response
    write(client_socket, buffer, strlen((char*)buffer));//writes the response to the clientfd(browser)
    sprintf (buffer,"Content-Length: %ld\n\n", length);
    send(client_socket,buffer,strlen(buffer),0);
    file = open(fileName, O_RDONLY);
    if(file < 0 )
        perror("File could not be open");
    info = read(file, sendmessage, SIZE);
    while(info > 0){//reads the info from the file and writes it back the client
        snprintf(buffer, sizeof(buffer), "%s\n", sendmessage);
        write(client_socket, buffer, strlen((char*)buffer));//writes the response to the clientfd(browser)
        if(sendmessage[info-1]== '\n');
        break;
    }
    close(file);
}
/*
 * This function sends the HTTP/1.0 404 Not Found client response
 */
void response_404NOTFOUND(int client_socket, long length){
    char buffer[SIZE];
    const char *sendmessage = "404 File Not Found ! :-(\n";
    snprintf(buffer, sizeof(buffer), "HTTP/1.0 404 Not Found\r\n\r\n%s\n", sendmessage);
    write(client_socket, buffer, strlen((char*)buffer));//writes the response to the clientfd(browser)
}
/*
 * This function returns the length of the file
 */
int contentLength(char* path){
    struct stat info;           //creates a stat structure
    char *ptr;
    ptr = basename(path);       //takes the filename out of the path
    if(stat(ptr, &info) == 0){   //checks the file exits in the given path
        fileName = ptr;         //assigns the filename to the global variable
        return info.st_size;    //returns the length of the content
    }
}
/*
 * This function checks if file exists. Returns True if exists, False if DNE
 */
int pathFinder(char * filePath){
    struct stat info;                //creates a stat structure
    if(stat(filePath, &info) == 0)  //if the path exits
        return true;                //returns true(1)
    else                            //if the file doesn't exist
        return false;               //returns false(0)
}
