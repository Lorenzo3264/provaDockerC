#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define N 512
#define PORT 8081
#define OUTPORT 8080

int main(int argc, char argv[]) {
	int c_fd;
	struct sockaddr_in sin;
	char buffer[N] = "puzzi di fragoline";
	
	c_fd = socket(AF_INET, SOCK_STREAM, 0);

	sin.sin_family = AF_INET;
	sin.sin_port = htons(OUTPORT);
	inet_aton("127.0.0.1", &sin.sin_addr);
	
	char buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &sin.sin_addr, buf, sizeof(buf));
	printf("Connecting to %s with port %d...\n",buf, sin.sin_port);
	if (connect(c_fd, (struct sockaddr*)&sin, sizeof(sin))) {
		printf("connect() failed cazzo\n");
		return 1;
	}
	
	printf("Writing...\n");
	write(c_fd, buffer, strlen(buffer));
	printf("Client %d ha inviato il messaggio\n", getpid());
	read(c_fd, buffer, strlen(buffer));
	printf("Server %s\n", buffer);


	close(c_fd);
	return 0;
}