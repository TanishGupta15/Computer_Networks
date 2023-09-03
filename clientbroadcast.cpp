#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "constants.h"
#include <bits/stdc++.h>
#include <fstream>
using namespace std;

struct Client_data{
    bool received[L];
    string data[L];
    bool complete;
    int port[N];
    const char *ips[N];
    bool broadcasted[L];
    int clientid;
};

struct P2P_connection{
    struct Client_data* needed_data;
    int socket;
    char *buffer;
    bool sent;
    int clientid;
};

void *p2p_broadcast(void *args){
    struct P2P_connection* data = (struct P2P_connection*) args;
    #ifdef DEBUG
        ofstream fout("send_log_" + to_string(data->clientid) + ".txt");
    #endif
    int sock = data->socket;
    while (!data->needed_data->complete){
        if(data->sent){
            int val = recv(sock, data->buffer, BUFFER_SIZE, 0);
            if (val < 0){
                perror("read");
                continue;
            }
            string reading = data->buffer;
            if(reading == "ack"){
                #ifdef DEBUG
                    fout << "Received ack" << endl;
                #endif
                data->sent = false;
            }
        }
        for (int i = 0; i < L; i++){
            if (!data->needed_data->broadcasted[i] && data->needed_data->received[i]){
                data->needed_data->broadcasted[i] = true;
                string temp = to_string(i) + "\n" + data->needed_data->data[i];
                const char *temp1 = temp.c_str();
                #ifdef DEBUG
                    fout << "Sending: " << temp1 << endl;
                #endif
                send(sock, temp1, strlen(temp1), 0);
                data->sent = true;
                break;
            }
        }
    }
    RETURN(0);
    #ifdef DEBUG
        // close file
        fout.close();
    #endif
}

void *clientbroadcast(void *args){

    struct Client_data* needdata = (struct Client_data*) args;
    vector<int> listensockets(N);
    vector<int> newsockets(N);
    vector<char *> buffers(N);
    vector<struct sockaddr_in> serveraddrs(N), newcleintaddrs(N);
    for (int i = 0; i < N; i++){
        if (i != needdata->clientid){
            listensockets[i] = socket(AF_INET, SOCK_STREAM, 0);
            if(listensockets[i] < 0){
                perror("socket");
                cout << "Socket creation failed\n";
                exit(0);
            }
            bzero((char *) &serveraddrs[i], sizeof(serveraddrs[i]));
            serveraddrs[i].sin_addr.s_addr = INADDR_ANY;
            serveraddrs[i].sin_port = PORTS + (needdata->clientid) * (N) + (i);
            serveraddrs[i].sin_family = AF_INET;
            int reuse = 1;
            if (setsockopt(listensockets[i], SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
                perror("setsockopt");
                //TODO: Handle error
            }

            if(bind(listensockets[i], (struct sockaddr *)&serveraddrs[i], sizeof(serveraddrs[i])) < 0){
                perror("bind");
                cout << "Bind failed\n";
                exit(0);
            }
            if(listen(listensockets[i], 5) < 0){
                perror("listen");
                cout << "Listen failed\n";
                exit(0);
            }
            cout << "Listening on port " << serveraddrs[i].sin_port << "\n";
            buffers[i] = new char[BUFFER_SIZE]; //TODO: Need to deallocate this buffer
        }
    }

    for (int i = 0; i < N; i++){
        if (i != needdata->clientid){
            socklen_t addrlen = sizeof(newcleintaddrs[i]);
            if((newsockets[i] = accept(listensockets[i], (struct sockaddr*) &newcleintaddrs[i], &addrlen)) < 0){
                perror("accept");
                cout << "Connection failed\n";
                continue;
            }
        }
    }

    vector<pthread_t> broadcasters(N);
    vector<struct P2P_connection> arguments(N);
    for (int i = 0; i < N; i++){
        if (i != needdata->clientid){
            arguments[i].buffer = buffers[i];
            arguments[i].socket = newsockets[i];
            arguments[i].needed_data = needdata;
            arguments[i].sent = false;
            arguments[i].clientid = i;
            pthread_create(&broadcasters[i], NULL, p2p_broadcast, &arguments[i]);
        }
    }
    for (int i = 0; i < N; i++){
        if (i != needdata->clientid)
            pthread_join(broadcasters[i], NULL);
    }
    RETURN(0);
}
