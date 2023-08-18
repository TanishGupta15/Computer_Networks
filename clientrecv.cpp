#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include "constants.h"
#include <pthread.h>
using namespace std;

struct mydata
{
	int checkpoints[L];
	string data[L];
	int complete;
	int port[N - 1];
	const char *ips[N - 1];
	int broadcasted[L];
};
struct updated
{
	struct mydata neededdata;
	int socket;
	char *buffer;
};

void *updatin(void *args)
{
	string reading;
	struct updated *updating = (struct updated *)args;
	string temp1 = "ack";
	const char *ack = temp1.c_str();
	int sock = updating->socket;
	char *buff = updating->buffer;
    char sizeforbuf[2048];
    buff = sizeforbuf;
	while (updating->neededdata.complete == 0)
	{
		int valread = read(sock, buff, 2048);
		reading = buff;
		if (reading != "")
		{
			int res = 0;
			int i = 0;
			while (reading[i] != '\n')
			{
				res = res * 10 + (reading[i] - '0');
				i++;
			}
			i++;
			string resdata;
			for (int j = i; j < reading.length() - 1; j++)
			{
				resdata += reading[j];
			}
			updating->neededdata.checkpoints[res] = 1;
			updating->neededdata.data[res] = resdata;
			send(updating->socket, ack, strlen(ack), 0);
		}
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
		while (1)
		{
			if ((status = connect(receivers[i], (struct sockaddr *)&serv_addrs[i],
								  sizeof(serv_addrs[i]))) < 0)
			{
				printf("\nConnection Failed \n");
				continue;
				int a = 2;
				int *b = &a;
				void *c = (void *)b;
				return c;
			}
			else
			{
				break;
			}
		}
	}

	vector<pthread_t> updaters(N - 1);
	vector<struct updated> arguments(N - 1);
	for (int i = 0; i < N - 1; i++)
	{
		arguments[i].socket = receivers[i];
		arguments[i].buffer = buffers[i];
		arguments[i].neededdata = *needdata;
		pthread_create(&updaters[i], NULL, updatin, &arguments[i]);
	}
	for (int i = 0; i < N - 1; i++)
	{
		pthread_join(updaters[i], NULL);
	}

	int a = 2;
	int *b = &a;
	void *c = (void *)b;
	return c;
}
