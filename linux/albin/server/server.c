#include "server.h"

static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
static struct list *clients, *messages;
static short id_count = 100;
static int sockfd;
int initiated = 0;
int running = 1;

static void error(const char *msg)
{
	perror(msg);
	exit(1);
}

static void process(unsigned short id, char *msg) {
	printf("In process\n");
	pthread_mutex_lock(&mutex1);
	struct message *message = malloc(sizeof(struct message));
	int k = strlen(msg);
	message->text = malloc(k + 1);
	memset(message->text, 0, k); 
	char* p = message->text;
	while (*msg)
		*(p++) = *(msg++);

	message->uid = id;
	struct node *n = malloc(sizeof(struct node));
	n->item = (void*) message;
	n->next = NULL;
	messages->last = n;
	messages->size++;
	if (messages->first == NULL)
		messages->first = n;

	printf("User with id %hu sent message '%s'\n", id, msg);
	pthread_mutex_unlock(&mutex1);
}

static void* client_listener(void* ptr) {
	printf("In client_listener\n");

	struct client* c = (struct client*) ptr;
	char buffer[256];
	int n;

	//Sets a 5 second timeout for read operation
	//struct timeval tv;
	//tv.tv_sec = 5;
	//tv.tv_usec = 0;
	//setsockopt(c->socketfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));

	printf("Listening thread for client with id %hu on socketfd %d\n", c->uid, c->socketfd);
	while (1) {
		memset(buffer, 0, 256);
		n = 255;
		n = read(c->socketfd,buffer,255);
		if (n < 0) 
			error("ERROR reading from socket");
		process(c->uid, buffer);
	}

	return NULL;
}

static void* listen_thread(void* var)
{
	printf("In listen\n");

	int newsockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	struct client* c;
	int port;

	port = *((int*)var);
	printf("listen to port %d\n", port);

	clients = malloc(sizeof(struct list));
	clients->size = 0;
	clients->first = NULL;
	clients->last = NULL;
	messages = malloc(sizeof(struct list));
	messages->size = 0;
	messages->first = NULL;
	messages->last = NULL;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	listen(sockfd, 5);

	initiated = 1;

	while (running) {
		clilen = sizeof(cli_addr);
		printf("Accepting connections on sockfd = %d\n", sockfd);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		printf("Someone connected and newsockfd = %d\n", newsockfd);
		if (newsockfd < 0) 
			error("ERROR on accept");
		
		c = malloc(sizeof(struct client));
		c->uid = id_count++;
		c->socketfd = newsockfd;
		printf("here socketfd = %d\n", c->socketfd);
		c->timestamp = clock();
		c->cli_addr = cli_addr;
		int rc1;
		if ((rc1 = pthread_create(&(c->thread), NULL, &client_listener, (void*)c))) {
			error("ERROR: Thread creation failed");
		}
		struct node* n = malloc(sizeof(struct node));
		n->item = c;
		n->next = clients->first;
		clients->first = n;
		if (clients->first == NULL)
			printf("Wtf, clients is null");

	      //  char buffer[256];
	      //  int v = read(newsockfd,buffer,255);
	      //  if (v < 0)
	      //          error("ERROR could not read");
	      //  process(1, buffer);
	}

	return NULL;
}

void serv_listen(int port)
{
	pthread_t thread;
	int ret;
	int* p = malloc(sizeof(int));
	*p = port;
	ret = pthread_create(&thread, NULL, &listen_thread, p);
	if (ret)
		error("ERROR Thread creation failed");
}

void serv_send(char* msg)
{
	printf("In send\n");

	int k;
	struct node* n = clients->first;
	while (n != NULL) {
		printf("Sending message '%s' to socketfd %d\n", msg, ((struct client*)(n->item))->socketfd);
		k = write(((struct client*)(n->item))->socketfd, msg, strlen(msg));
		if (k < 0)
			error("ERROR writing to socket");
		n = n->next;
	}
	printf("After send\n");
}

struct message* serv_get_msg()
{
	printf("In get_msg\n");

	struct message *msg;
	if (messages->first == NULL)
		return NULL;
	msg = (struct message*)messages->first->item;
	if (messages->first != messages->last)
		messages->first = messages->first->next;
	else
		messages->first = messages->last = NULL;
	return msg; 
}

void serv_disconnect(struct client c)
{
	printf("In server dc\n");
	//send some dc to client and stop thread and delete client from list. maybe delete all msg from them also?
}

void serv_stop() {
	printf("In server stop\n");

	struct node* node = clients->first;
	struct node* k;
	while (node != NULL) {
		close(((struct client*) (node->item))->socketfd);
		k = node;
		node = node->next;
		free(k);
	}
	free(clients);
	//free all messages also?
	close(sockfd);
	running = 0;
	printf("Server stopped\n");
}

