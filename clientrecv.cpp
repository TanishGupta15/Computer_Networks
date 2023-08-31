#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include "constants.h"
#include <pthread.h>
using namespace std;

struct mydata {
	int checkpoints[L];
	string data[L];
	int complete;
	int port[N];
	const char *ips[N];
	int broadcasted[L];
	int clientid;
};

struct updated {
	struct mydata *needed_data;
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
	// char sizeforbuf[2048];
	// buff = sizeforbuf;
	while (updating->needed_data->complete == 0)
	{
		int valread = read(sock, buff, BUFFER_SIZE);
		if(valread < 0){
			perror("read");
			continue;
		}
		reading = buff;
		if (reading != "")
		{
			int res = 0;
			int i = 0;
			// cout << "Reading " << reading << endl;
			while (i < reading.size() && reading[i] != '\n')
			{
				res = res * 10 + (reading[i] - '0');
				i++;
			}
			i++;
			string resdata;
			for (int j = i; j < (int)reading.length() - 1; j++)
			{
				resdata += reading[j];
			}
			updating->needed_data->checkpoints[res] = 1;
			updating->needed_data->data[res] = resdata;
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
	struct mydata *need_data = (struct mydata *)args;

	// int status, valread;
	int status;
	int receivers[N];

	for (int i = 0; i < N; i++)
		receivers[i] = 0;
	vector<struct sockaddr_in> serv_addrs(N);
	vector<char *> buffers;
	for (int i = 0; i < N; i++){
		char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
		buffers.push_back(buffer);
	}
	for (int i = 0; i < N; i++){
		if (i != need_data->clientid){
			if ((receivers[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0){
				printf("\n Socket creation error \n");
				int a = 2;
				int *b = &a;
				void *c = (void *)b;
				return c;
			}

			serv_addrs[i].sin_family = AF_INET;
			cout << "For client " << need_data->clientid << " " << i << " " << PORTS + (i) * (N) + (need_data->clientid) << endl;
			serv_addrs[i].sin_port = (PORTS + (i) * (N) + (need_data->clientid));
		}
	}

	for (int i = 0; i < N; i++){
		if (i != need_data->clientid) {
			// For now, just hardcoded, giving error;
			string str = "10.194.46.195";
			const char* x = str.c_str();
			if (inet_pton(AF_INET, x, &serv_addrs[i].sin_addr) <= 0){
				int a = 2;
				int *b = &a;
				void *c = (void *)b;
				return c;
			}
		}
	}

	int connectionchk = 0;
	vector<bool> connected(N, false);
	while (connectionchk != N-1){
		for (int i = 0; i < N; i++){
			if (i != need_data->clientid && !connected[i]){
				cout << "Trying to connect " << serv_addrs[i].sin_port << endl;
				if ((status = connect(receivers[i], (struct sockaddr *)&serv_addrs[i],
									  sizeof(serv_addrs[i]))) < 0){
					cout << "Trying again\n";
					perror("connect");
					// exit(0);
					continue;
					// connectionchk = 0;
				}
				else{
					connected[i] = true;
					connectionchk++;
				}
			}
		}
	}

	cout << "Connected yay :)\n";

	vector<pthread_t> updaters(N);
	vector<struct updated> arguments(N);
	for (int i = 0; i < N; i++){
		if (i != need_data->clientid){
			arguments[i].socket = receivers[i];
			arguments[i].buffer = buffers[i];
			arguments[i].needed_data = need_data;
			pthread_create(&updaters[i], NULL, updatin, &arguments[i]);
		}
	}
	for (int i = 0; i < N; i++){
		if (i != need_data->clientid)
			pthread_join(updaters[i], NULL);
	}

	int a = 2;
	int *b = &a;
	void *c = (void *)b;
	return c;
}
