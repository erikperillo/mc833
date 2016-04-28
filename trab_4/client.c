#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 31472
#define MAX_LINE 256

int main(int argc, char * argv[])
{
	struct hostent *hp;
	struct sockaddr_in sin;
	struct sockaddr_in addr;
	char ip[INET_ADDRSTRLEN];
	socklen_t addrlen;
	char *host;
	char buf[MAX_LINE];
	int s;
	int len;

	if (argc==2) {
		host = argv[1];
	}
	else {
		fprintf(stderr, "usage: ./client host\n");
	exit(1);
	}

	/* translate host name into peer’s IP address */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
		exit(1);
	}

	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);

	/* active open */
	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}

	if(connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}

	addrlen = sizeof(addr);
	if(getsockname(s, (struct sockaddr*)&addr, &addrlen) < 0) {
		perror("simplex-talk: getsockname");
		close(s);
		exit(1);
	}
	inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
	printf("created socket on port #%d, IP %s\n", addr.sin_port, ip);

	/* main loop: get and send lines of text */
	while (fgets(buf, sizeof(buf), stdin)) {
		buf[MAX_LINE-1] = '\0';
		len = strlen(buf) + 1;
		send(s, buf, len, 0);
	}
}
