#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 256

//displays error message and quits
void error(const char* msg)
{
	fprintf(stderr, "[udp_server] ");
	perror(msg);
	exit(1);
}

int main(int argc, char * argv[])
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	socklen_t cli_addr_len;
	char buf[BUF_SIZE];
	unsigned short port;
	int opt;
	int sock;
	int ret;

	if(argc > 1)
		port = (unsigned short)atoi(argv[1]);
	else 
	{
		fprintf(stderr, "[udp_server] usage: ./server <port>\n");
		exit(1);
	}

	//setting up server address structure
	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	//creating socket
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock < 0) 
		error("socket");

	//allowing socket to be reused fast
	opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(int));

	//binding server socket
	if(bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
		error("could not bind");

	//populating 
	cli_addr_len = sizeof(cli_addr);

	//main echo loop
	while(1)
	{
		//receiving message (if any)
		ret = recvfrom(sock, buf, BUF_SIZE, 0, 
			(struct sockaddr*)&cli_addr, &cli_addr_len);
		if(ret < 0)
			printf("error receiving message from client");
	
		//printing client message and sending it back
		if(ret > 0)
		{
			printf("[received from %s:%u] %s\n", 
				inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);

			ret = sendto(sock, buf, strlen(buf), 0, 
				(struct sockaddr*)&cli_addr, sizeof(cli_addr));
			if(ret < 0)
				error("error sending message to client");
		}
	}

	return 0;
}
