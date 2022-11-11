
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include<bits/stdc++.h>
#include <unistd.h> // read(), write(), close()
using namespace std;
#define MAX 80
#define PORT 3000
#define SA struct sockaddr
void func(int sockfd)
{
    string exitMsg = "exit";
	
	int buff = 50;
    char buffer[1024] = {0};
	read(sockfd, &buffer, sizeof(buffer));
	cout<<buffer<<'\n';
	int r = 1e6 ;
	//cin >> r;
	r = htonl(r);
	send(sockfd  , &r , sizeof(r) , 0);
	for (;;) {
		while(read(sockfd, &buffer, sizeof(buffer))){
			cout<<buffer<<"\n\n";
			if(strcmp(buffer,exitMsg.c_str()))break;
			memset(buffer,0,1024);
			if(strcmp(buffer,exitMsg.c_str()))break;
        }
		
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}