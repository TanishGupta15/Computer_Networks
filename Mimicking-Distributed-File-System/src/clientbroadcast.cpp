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
    int broadcast[L];
    int clientid;
    int max_idx;
};

struct P2P_connection{
    struct Client_data* needed_data;
    int socket;
    char* buffer;
    bool sent;
    int clientid;
    int broadcast_idx;
};

void *p2p_broadcast(void *args){
    struct P2P_connection* data = (struct P2P_connection*) args;
    #ifdef DEBUG
        ofstream fout("../logs/send_log_" + to_string(data->clientid) + ".txt");
    #endif
    int sock = data->socket;
    while (!data->needed_data->complete || (data->broadcast_idx <= data->needed_data->max_idx)){
        if(data->sent){
            int val = recv(sock, data->buffer, BUFFER_SIZE, 0);
            if (val < 0){
                perror("read");
                continue;
            }
            if(val == 0){
                // cout << "Connection closed\n";
                break;
            }
            string reading = data->buffer;
            if(reading == "ack"){
                #ifdef DEBUG
                    fout << "Received ack" << endl;
                #endif
                data->sent = false;
            }
            else continue;
        }
        if(data->broadcast_idx <= data->needed_data->max_idx){
            string temp = "";
            int lengthofvector = data->needed_data->max_idx;
            while(data->broadcast_idx  <= lengthofvector){
                int i = data->needed_data->broadcast[data->broadcast_idx];
                data->broadcast_idx++;
                temp += (to_string(i) + "\n" + data->needed_data->data[i]);
            }
            temp += "\n";
            const char *temp1 = temp.c_str();
            #ifdef DEBUG
                fout << "Sending: " << temp1 << endl;
            #endif
            if(send(sock, temp1, strlen(temp1), MSG_NOSIGNAL) < 0){
                perror("send");
                continue;
            }
            data->sent = true;
        }
    }
    #ifdef DEBUG
        // close file
        fout.close();
    #endif
    RETURN(0);
}

void *clientbroadcast(void *args){

    struct Client_data* needdata = (struct Client_data*) args;
    vector<int> listensockets(N);
    vector<int> newsockets(N);
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
                exit(0);
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
    vector<struct P2P_connection*> arguments(N);
    for (int i = 0; i < N; i++){
        if (i != needdata->clientid){
            arguments[i] = new struct P2P_connection;
            arguments[i]->buffer = new char[BUFFER_SIZE];
            arguments[i]->socket = newsockets[i];
            arguments[i]->needed_data = needdata;
            arguments[i]->sent = false;
            arguments[i]->clientid = i;
            arguments[i]->broadcast_idx = 0;
            pthread_create(&broadcasters[i], NULL, p2p_broadcast, (void*)(arguments[i]));
        }
    }
    for (int i = 0; i < N; i++){
        if (i != needdata->clientid)
            pthread_join(broadcasters[i], NULL);
    }

    for (int i = 0; i < N; i++){
        if (i != needdata->clientid){
            while(close(newsockets[i]) < 0){
                perror("close newsockets");
                continue;
            }
            while(close(listensockets[i]) < 0){
                perror("close listensockets");
                continue;
            }
        }
    }

    // deallocate memory
    for (int i = 0; i < N; i++){
        if (i != needdata->clientid){
            delete[] arguments[i]->buffer;
            delete arguments[i];
        }
    }

    RETURN(0);
}
