#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include "constants.h"
#include <pthread.h>
#define PORT 9801
using namespace std;

struct mydata
{
	int checkpoints[L];
	string data[L];
	int complete;
	int port[N - 1];
	char *ips[N - 1];
};
struct updated
{
	struct mydata neededdata;
	int socket;
	char* buffer;
};

void *updating(void* args){
	string reading;
	struct updated* updating = (struct updated*)args;
	int sock = updating->socket;
	char* buff = updating->buffer;
	while(updating->neededdata.complete==0){
		int valread = read(sock, buff, 2048);
		reading += buff;
		// Update data
	}
	int a = 2;
	int *b = &a;
	void *c = (void *)b;
	return c;
}

void *clientrecv(void *args)
{
	struct mydata *needdata = (struct mydata *)args;
	int status, valread;
	int receivers[N - 1] = {0};
	vector<struct sockaddr_in> serv_addrs(N - 1);
	vector<char *> buffers;
	for (int i = 0; i < N - 1; i++)
	{
		char buffer[1024] = {0};
		buffers[i] = buffer;
	}
	for (int i = 0; i < N - 1; i++)
	{
		if ((receivers[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			printf("\n Socket creation error \n");
			int a = 2;
			int *b = &a;
			void *c = (void *)b;
			return c;
		}
		serv_addrs[i].sin_family = AF_INET;
		serv_addrs[i].sin_port = htons(needdata->port[i]);
	}

	for (int i = 0; i < N - 1; i++)
	{
		if (inet_pton(AF_INET, needdata->ips[i], &serv_addrs[i].sin_addr) <= 0)
		{
			int a = 2;
			int *b = &a;
			void *c = (void *)b;
			return c;
		}
	}

	for (int i = 0; i < N - 1; i++)
	{
		if ((status = connect(receivers[i], (struct sockaddr *)&serv_addrs[i],
							  sizeof(serv_addrs[i]))) < 0)
		{
			printf("\nConnection Failed \n");
			int a = 2;
			int *b = &a;
			void *c = (void *)b;
			return c;
		}
	}

	vector<pthread_t> updaters(N-1);
	vector<struct updated> arguments(N-1);
	for(int i = 0; i < N-1; i++){
		arguments[i].socket = receivers[i];
		arguments[i].buffer = buffers[i];
		pthread_create(&updaters[i],NULL,updating,&arguments[i]);
	}
	for(int i = 0; i < N-1; i++){
		pthread_join(updaters[i],NULL);
	}

	int a = 2;
	int *b = &a;
	void *c = (void *)b;
	return c;
}
