#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define DEF_PORT 7532
#define MAX_LINE 256

//displays error message and quits
void error(const char* msg)
{
	fprintf(stderr, "[udp_client] ");
	perror(msg);
	exit(1);
}

int main(int argc, char * argv[])
{
	struct hostent *hp;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	struct sockaddr_in sender_addr;
	socklen_t sender_addr_len;
	char *host;
	char buf[MAX_LINE];
	unsigned short port;
	int sock;
	int len;
	int ret;
	int opt;

	if(argc > 2)
	{
		host = argv[1];
		port = (unsigned short)atoi(argv[2]);
	}
	else 
	{
		fprintf(stderr, "[udp_client] usage: ./client <host> <port>\n");
		exit(1);
	}

	//translating host name into peer IP address
	hp = gethostbyname(host);
	if(!hp) 
	{
		fprintf(stderr, "[udp_client] unknown host: %s\n", host);
		exit(1);
	}

	//setting up target server address structure
	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	memcpy((char*)&serv_addr.sin_addr, hp->h_addr, hp->h_length);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	//creating socket
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock < 0) 
		error("could not open socket");

	//allowing socket to be reused fast
	opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(int));

	//first populating len
	sender_addr_len = sizeof(sender_addr);

	//connecting to server socket
	ret = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret < 0)
		error("error on connect");

	//main echo loop
	while(1)
	{
		//getting message from console
		printf(">>> ");
		if(!fgets(buf, sizeof(buf), stdin))
			break;

		//sending message to server
		buf[MAX_LINE-1] = '\0';
		len = strlen(buf) + 1;
		ret = send(sock, buf, len, 0);
		if(ret < 0)
			error("error sending message to server!\n");

		//receiving answer from server
		ret = recv(sock, buf, MAX_LINE, 0);
		if(ret < 0)
			error("error receiving message!\n");

		//printing server message
		printf("received from server: %s\n", buf);
	}
	
	return 0;
}
