/** 
Code from http://www.unpbook.com/
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <string.h>

#define LISTENQ 5
#define MAXLINE 64
#define SERV_PORT 56789

#define lower(c) ((c >= 97) && (c <= 122))
#define upper(c) ((c >= 65) && (c <= 90))
#define invc(c) (lower(c)?(c - 32):(upper(c)?(c + 32):c))

int main(int argc, char **argv)
{
	int	i, k, maxi, maxfd, listenfd, connfd, sockfd;
	int	nready, client[FD_SETSIZE];
	ssize_t	n;
	fd_set rset, allset;
	char buf[MAXLINE];
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("socket error");
		return 1;
	}

	//added by erik
	int en = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) < 0)
	{
		perror("setsockopt()");
		close(listenfd);
		return 1;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)  
	{
		perror("bind error");
		close(listenfd);
		return 1;
	}

	if(listen(listenfd, LISTENQ) < 0) 
	{
		perror("listen error");
		close(listenfd);
		return 1;
	}

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	for (i=0; i<FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	for(;;) 
	{
		rset = allset;		/* structure assignment */
		if((nready = select(maxfd+1, &rset, NULL, NULL, NULL)) < 0)
		{
			perror("select()");
			for(i=0; i<FD_SETSIZE; i++)
				if(client[i] >= 0)
					close(client[i]);
			close(listenfd);
			return 1;
		}

		if (FD_ISSET(listenfd, &rset)) 
		{	/* new client connection */
			clilen = sizeof(cliaddr);
			if((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, 
				&clilen)) < 0)
			{
				perror("accept()");
				for(i=0; i<FD_SETSIZE; i++)
					if(client[i] >= 0)
						close(client[i]);
				close(listenfd);
				return 1;
			}

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) 
				{
					client[i] = connfd;	/* save descriptor */
					break;
				}
			if (i == FD_SETSIZE) 
			{
				perror("too many clients");
                exit(1);
      		}

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for(i=0; i<=maxi; i++) 
		{	/* check all clients for data */
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) 
			{
				if((n = read(sockfd, buf, MAXLINE)) == 0) 
				{
					/* connection closed by client */
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} 
				else if(n < 0)
				{
					perror("read()");
					for(k=0; k<FD_SETSIZE; k++)
						if(client[k] >= 0)
							close(client[k]);
					close(listenfd);
					return 1;
				}
				else if(send(sockfd, buf, n, 0) < 0)
				{
					perror("send()");
					for(k=0; k<FD_SETSIZE; k++)
						if(client[k] >= 0)
							close(client[k]);
					close(listenfd);
					return 1;
				}

				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}

	return 0;
}
