#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 7
#define MAX_LINE 256
#define ITERS 1000

int main(int argc, char * argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	char buf[MAX_LINE] = "eyb0sss";
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

	/*
	printf("call from gethostbyname(%s) returned:\n", argv[1]);
	printf("nh_name=%s, h_addrtype=%d, h_lenght=%d\n",
		hp->h_name, hp->h_addrtype, hp->h_length);
	int i;
	for(i=0; i<hp->h_aliases[i] != NULL; i++)
		printf("h_aliases[%d]=%s\n", i, hp->h_aliases[i]);
	for(i=0; i<hp->h_addr_list[i] != NULL; i++)
		printf("h_addr_list[%d]=%s\n", i, hp->h_addr_list[i]);
	printf("\n");
	*/

	/* build address data structure */
	bzero((char*)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char*)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);

	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}

	printf("simplex-talk: main loop started\n");
	/* main loop: get and send lines of text */
	while (fgets(buf, sizeof(buf), stdin)) {
	{
		len = strlen(buf) + 1;
		send(s, buf, len, 0);
	
		printf("sent\n");

		if(len = recv(s, buf, sizeof(buf), 0))
			fputs(buf, stdout);
	}
}
