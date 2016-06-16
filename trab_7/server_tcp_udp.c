#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVER_PORT 31472
#define MAX_PENDING 5
#define BUF_SIZE 256

#define max(a, b) (((a)>(b))?(a):(b))

//displays error message and quits
void error(const char* msg)
{
	fprintf(stderr, "[server] ");
	perror(msg);
	exit(1);
}

int main(int argc, char * argv[])
{
	struct sockaddr_in sin;
	struct sockaddr_in addr;
	fd_set rset, allset;
	socklen_t addrlen;	
	socklen_t len;
	char ip[INET_ADDRSTRLEN];
	char buf[BUF_SIZE];
	unsigned short port;
	int ret;
	int opt;
	int maxfd;
	int nready;
	int udp_sock, tcp_sock, new_tcp_sock;
	pid_t self, child;

	if(argc > 1)
		port = (unsigned short)atoi(argv[1]);
	else 
	{
		fprintf(stderr, "[udp_server] usage: ./server <port>\n");
		exit(1);
	}

	//building address data structure 
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);

	//opening TCP and UDP sockets
	tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (tcp_sock < 0)
		error("tcp socket");
	udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (udp_sock < 0)
		error("udp socket");

	//allowing sockets to be reused
	opt = 1;
	setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, 
		sizeof(int));
	setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, 
		sizeof(int));

	//binding sockets to address
	ret = bind(udp_sock, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
		error("bind");
	ret = bind(tcp_sock, (struct sockaddr *)&sin, sizeof(sin));
	if (ret < 0)
		error("bind");
	//listening on port for tcp
	if(listen(tcp_sock, MAX_PENDING) < 0)
	{
		close(tcp_sock);
		error("listen");
	}

	self = getpid();
	printf("server PID is %d, listening on port #%d\n\n", self, port);

	maxfd = max(tcp_sock, udp_sock);
	FD_ZERO(&allset);
	FD_SET(tcp_sock, &allset);
	FD_SET(udp_sock, &allset);

	//waits for connection, then receives and print text
	while(1) 
	{
		rset = allset;		
		//checking sockets to be read
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		if(nready < 0)
		{
			close(tcp_sock);
			close(udp_sock);
			error("select");
		}

		//TCP
		if(FD_ISSET(tcp_sock, &rset))
		{
			//connecting to new client
			len = sizeof(sin);
			new_tcp_sock = accept(tcp_sock, (struct sockaddr *)&sin, &len);
			if (new_tcp_sock < 0) 
				error("accept");

			//getting new connection information
			addrlen = sizeof(addr);
			ret = getpeername(new_tcp_sock, (struct sockaddr*)&addr, &addrlen);
			if(ret < 0) 
			{
				close(tcp_sock);
				error("getpeername");
			}
			inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
			printf("connected to IP %s, port #%d\n", ip, ntohs(addr.sin_port));

			//spanning new process for connection
			child = fork();
			if(child == 0) 
			{
				self = getpid();
				close(tcp_sock);
				while(1) 
				{
					ret = recv(new_tcp_sock, buf, sizeof(buf), 0);
					if(ret > 0)
					{	
						printf("[TCP][PID %d][%s:%d] %s", 
							self, ip, ntohs(addr.sin_port), buf);
						ret = send(new_tcp_sock, buf, strlen(buf)+1, 0);
						if(ret < 0)
							error("send");
					}
					else if(ret == 0)
						break;
					else
						error("recv");
				}
				close(new_tcp_sock);
				exit(0);
			}	

			close(new_tcp_sock);
		}
		//UDP
		if(FD_ISSET(udp_sock, &rset))
		{
			//receiving message (if any)
			len = sizeof(sin);
			ret = recvfrom(udp_sock, buf, BUF_SIZE, 0, 
				(struct sockaddr*)&sin, &len);
			if(ret < 0)
				printf("error receiving message from client");
		
			//printing client message and sending it back
			if(ret > 0)
			{
				printf("[UDP][%s:%u] %s", 
					inet_ntoa(sin.sin_addr), ntohs(sin.sin_port), buf);

				ret = sendto(udp_sock, buf, strlen(buf)+1, 0, 
					(struct sockaddr*)&sin, sizeof(sin));
				if(ret < 0)
					error("error sending message to client");
			}
		}
	}
}
