// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    printf("address of printf=%p\n", (void *)printf);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
	&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );


    if(argc > 1)
    {
	server_fd = atoi(argv[1]);
    	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)	    
    	{
        	perror("accept");
        	exit(EXIT_FAILURE);
    	}
	valread = read( new_socket , buffer, 1024);
    	printf("%s\n",buffer );
	send(new_socket , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	return 0;
    }
    if (bind(server_fd, (struct sockaddr *)&address,
	sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int parent_id = fork();
    if(parent_id == 0)
    {
	struct passwd* pw;    

        char socket_fd[11];
        char *args[] = {"./server",socket_fd,NULL};
        if( ( pw = getpwnam( "nobody" ) ) == NULL )
	{
	     fprintf( stderr, "getpwnam: unknown" );
	     return( EXIT_FAILURE );		     
	}
	setuid(pw->pw_uid);
        
	snprintf(socket_fd,12,"%d\n",server_fd);
	execv("./server",args);
        exit(0);			
        return 0;
    }
    else if(parent_id > 0)
    {
    	wait(NULL);
	exit(0);
    }
    return 0;
}
