/* Generic */
//#define _POSIX_C_SOURCE 199309L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
#include <assert.h>
#include <time.h>
#include <stropts.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/uio.h>
#include <ctype.h>
#include <sys/time.h>
#include <arpa/inet.h>

//#include<linux/time.h>
/* Network */
#include <netdb.h>
#include <sys/socket.h>

#define BUF_SIZE 100

// Get host information (used to establishConnection)
struct addrinfo *getHostInfo(char* host, char* port) {
	int r;
	struct addrinfo hints, *getaddrinfo_res;
	// Setup hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ((r = getaddrinfo(host, port, &hints, &getaddrinfo_res))) {
		fprintf(stderr, "[getHostInfo:21:getaddrinfo] %s\n", gai_strerror(r));
		return NULL;
	}

	return getaddrinfo_res;
}

// Establish connection with host
int establishConnection(struct addrinfo *info) {
	if (info == NULL) return -1;

	int clientfd;
	for (;info != NULL; info = info->ai_next) {
		if ((clientfd = socket(info->ai_family,
						info->ai_socktype,
						info->ai_protocol)) < 0) {
			perror("[establishConnection:35:socket]");
			continue;
		}

		if (connect(clientfd, info->ai_addr, info->ai_addrlen) < 0) {
			close(clientfd);
			perror("[establishConnection:42:connect]");
			continue;
		}

		freeaddrinfo(info);
		return clientfd;
	}

	freeaddrinfo(info);
	return -1;
}

// Send GET request
void GET(int clientfd, char *path) {
	char req[1000] = {0};
	sprintf(req, "GET %s HTTP/1.0\r\n\r\n", path);
	send(clientfd, req, strlen(req), 0);
}

int main(int argc, char **argv) {
	int clientfd;
	char buf[BUF_SIZE];

	FILE *fd = NULL;

	if (argc != 4) {
		fprintf(stderr, "USAGE: ./httpclient <hostname> <port> <request path>\n");
		return 1;
	}

	// Establish connection with <hostname>:<port>
	clientfd = establishConnection(getHostInfo(argv[1], argv[2]));
	if (clientfd == -1) {
		fprintf(stderr,
				"[main:73] Failed to connect to: %s:%s%s \n",
				argv[1], argv[2], argv[3]);
		return 3;
	}

	fd = fopen("fct_log", "w");

	struct timeval time_start, time_finish;

	//clock_gettime(CLOCK_MONOTONIC, &time_start);
	gettimeofday(&time_start, NULL);


	// Send GET request > stdout
	GET(clientfd, argv[3]);
	while (recv(clientfd, buf, BUF_SIZE, 0) > 0) {
		//fputs(buf, stdout);
		memset(buf, 0, BUF_SIZE);
	}

	//clock_gettime(CLOCK_MONOTONIC, &time_finish);
	gettimeofday(&time_finish, NULL);
	//uint64_t nanodiff= time_finish.tv_nsec + time_finish.tv_sec * (1000*1000*1000)- (time_start.tv_nsec + time_start.tv_sec * (1000*1000*1000));

	uint64_t duration_us=(time_finish.tv_sec - time_start.tv_sec) * 1000000 + time_finish.tv_usec - time_start.tv_usec;

	//printf("FCT %" PRIu64 "\n",duration_us);
	fprintf(fd, "%s %"PRIu64 "\n", argv[3],duration_us);

	//int secs = nanodiff / (1000*1000*1000);
	//long nsecs = nanodiff % (1000*1000*1000);

	//double f_msecs = nanodiff / (1000.0 * 1000.0);


	close(clientfd);
	return 0;
}




































