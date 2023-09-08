#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include "constants.h"
#include <pthread.h>
#include <fstream>
using namespace std;

struct Client_data {
	bool received[L];
	string data[L];
	bool complete;
	int port[N];
	const char *ips[N];
	vector<int> broadcast;
	int clientid;
};

struct P2P_connection {
	struct Client_data *needed_data;
	int socket;
	char *buffer;
	int clientid;
};

void *p2p_recv(void *args){

	string reading = "";
	struct P2P_connection *data = (struct P2P_connection *)args;
	#ifdef DEBUG
		ofstream fout("../logs/recv_log_" + to_string(data->clientid) + ".txt");
	#endif
	string temp1 = "ack";
	const char *ack = temp1.c_str();
	int sock = data->socket;
	char *buff = data->buffer;
	while (!data->needed_data->complete){
		int count = 0; //number of line breaks
		// int countofdoubleslash = 0;
		reading = "";
		while(count < 2){
            int x = recv(sock, buff, BUFFER_SIZE, 0);
            if(x < 0){
                perror("recv");
                continue;
            }
            for(int i = 0; i < x; i++){
                if(buff[i] == '\n') {
					count++;
				}
				else{
					count = 0;
				}
                reading += buff[i];
                if(count == 2){
					break;
				}
            }
        }
		#ifdef DEBUG
			fout << "Received: " << reading << endl;
			fout << "Buffer: " << buff << endl;
		#endif

		if (reading != ""){
			int i = 0;
			while(i < (int)reading.size()-1){
				int line_num = 0;
				string line_data = "";
				if(reading[i] == '-') continue; // Shouldn't happen
				while (i < (int)reading.size()-1 && reading[i] != '\n'){
					line_num = line_num * 10 + int(reading[i] - '0');
					i++;
				}
				i++;
				while (i < (int)reading.size()-1 && reading[i] != '\n'){
					line_data += reading[i];
					i++;
				}
				line_data += "\n";
				i++;
				if(!data->needed_data->received[line_num]){
					data->needed_data->received[line_num] = true;
					data->needed_data->data[line_num] = line_data;

				}
				// cout << line_num << endl;
				#ifdef DEBUG
					fout << "Received line_num = " << line_num << endl;
				#endif
			}	
			send(data->socket, ack, strlen(ack), MSG_NOSIGNAL);

		}
	}
	#ifdef DEBUG
		// close file
		fout.close();
	#endif
	RETURN(0);
}

void *clientrecv(void *args){
	struct Client_data *need_data = (struct Client_data*)args;

	int status;
	int receivers[N];

	for (int i = 0; i < N; i++) receivers[i] = 0;
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
				RETURN(2);
			}
			serv_addrs[i].sin_family = AF_INET;
			serv_addrs[i].sin_port = (PORTS + (i) * (N) + (need_data->clientid));
		}
	}

	for (int i = 0; i < N; i++){
		if (i != need_data->clientid) {
			if (inet_pton(AF_INET, client_ips[i], &serv_addrs[i].sin_addr) <= 0){
				RETURN(2);
			}
		}
	}

	int connectionchk = 0;
	vector<bool> connected(N, false);
	while (connectionchk != N-1){
		for (int i = 0; i < N; i++){
			if (i != need_data->clientid && !connected[i]){
				cout << "Trying to connect " << serv_addrs[i].sin_port << endl;
				if ((status = connect(receivers[i], (struct sockaddr *)&serv_addrs[i], sizeof(serv_addrs[i]))) < 0){
					cout << "Couldn't connect. Trying again...\n";
					perror("connect");
					continue;
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
	vector<struct P2P_connection> arguments(N);
	for (int i = 0; i < N; i++){
		if (i != need_data->clientid){
			arguments[i].socket = receivers[i];
			arguments[i].buffer = buffers[i];
			arguments[i].needed_data = need_data;
			arguments[i].clientid = i;
			pthread_create(&updaters[i], NULL, p2p_recv, &arguments[i]);
		}
	}
	for (int i = 0; i < N; i++){
		if (i != need_data->clientid)
			pthread_join(updaters[i], NULL);
	}

	//Close all sockets
	for (int i = 0; i < N; i++){
		if (i != need_data->clientid)
			while(close(receivers[i]) < 0){
				perror(("close " + to_string(i)).c_str());
				continue;
			}
	}

	//delete all buffers
	for (int i = 0; i < N; i++){
		if (i != need_data->clientid)
			delete[] buffers[i];
	}

	RETURN(0);
}
