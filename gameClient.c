#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define BUFFER_SIZE 2048
#define PORT 27568

int main()
{
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	char buf[BUFFER_SIZE];
	int s;
	size_t len;

	host = "localhost";

	hp= gethostbyname(host);
	if(!hp) {
	fprintf(stderr, "unknown host: %s\n", host);
	exit(1);
	}

	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(PORT);

	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("simplex-talk: socket");
		exit(1);
	}
	if(connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}
	char goodBye[] = "Goodbye\n";	
	while(1)
	{
		len = recv(s, buf, sizeof(buf), 0);
		write(1, buf, len);
		fflush(stdout);
		if (strncmp(buf, goodBye, len) == 0) exit(1);
		else{
		len = read(0, buf, BUFFER_SIZE);
		buf[len-1]='\0';
		send(s, buf, strlen(buf), 0);
		}
	}
}
