
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>

#define PORT 8080
#define OUTPORT 8081
#define N 512

pthread_mutex_t mutex;

void *gestisci(void *arg) {
	printf("gestisci(): creato il thread\n");
	char buf[N];
	int s_fd = *(int*)arg;
	read(s_fd, buf, N);
	printf("Server %s\n", buf);
	char msg[N] = "Non e' vero";
	printf("writing...\n");
	write(s_fd, msg, N);
	printf("written...\n");
	close(s_fd);
	pthread_exit(0);
}


int main(int argc, char const* argv[])
{
	printf("Starting...\n");
	struct sockaddr_in myaddr, client;
	pthread_t tid;
	char buffer[N];
	int mySocket, porta, clientSocket,len;

	mySocket = socket(AF_INET, SOCK_STREAM, 0);

	//host byte order
	myaddr.sin_family = AF_INET;

	//short, network byte order
	myaddr.sin_port = htons(PORT);

	//myaddr.sin_addr.s_addr = htonl("127.0.0.1");

	//long, network byte order
	inet_aton("0.0.0.0", &(myaddr.sin_addr));

	// a zero tutto il resto
	memset(&(myaddr.sin_zero), '\0', 8);
    
	len = sizeof(client);

	printf("Binding...\n");
	bind(mySocket, (struct sockaddr*)&myaddr, sizeof(myaddr));
	printf("Listening...\n");
	listen(mySocket, 5);

	char buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &myaddr.sin_addr, buf, sizeof(buf));


	pthread_mutex_init(&mutex, NULL);
	while (1) {
		printf("Accepting as %s with port %d...\n",buf,myaddr.sin_port);
		clientSocket = accept(mySocket, (struct sockaddr*)&client, &len);
		printf("request incoming...\n");
		inet_ntop(AF_INET, &client.sin_addr, buffer, sizeof(buffer));
		printf("request from client %s\n", buffer);
		
		//gestione richiesta...
		pthread_create(&tid, NULL, gestisci, (void*)&clientSocket);
		pthread_mutex_lock(&mutex);

		pthread_join(tid, NULL);
		pthread_mutex_unlock(&mutex);
		pthread_detach(tid);
	}

	close(mySocket);

    return 0;
}

