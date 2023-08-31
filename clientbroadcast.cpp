#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "constants.h"
#include <bits/stdc++.h>
using namespace std;

struct mydata{
    int checkpoints[L];
    string data[L];
    int complete;
    int port[N];
    const char *ips[N];
    int broadcasted[L];
    int clientid;
};

struct updated{
    struct mydata *neededdata;
    int socket;
    char *buffer;
    int start;
};

void *updating(void *args){
    struct updated *updating = (struct updated *)args;
    int sock = updating->socket;
    updating->buffer = new char[BUFFER_SIZE];
    while (updating->neededdata->complete == 0)
    {
        if (updating->start == 1){
            for (int i = 0; i < L; i++){
                if (updating->neededdata->broadcasted[i] == 0 && updating->neededdata->checkpoints[i] == 1){
                    updating->neededdata->broadcasted[i] = 1;
                    string temp = to_string(i) + "\n" + updating->neededdata->data[i];
                    const char *temp1 = temp.c_str();
                    send(sock, temp1, strlen(temp1), 0);
                    updating->start = 0;
                    break;
                }
            }
        }
        else{
            int val = read(sock, updating->buffer, BUFFER_SIZE);
            if (val < 0){
                perror("read");
                continue;
            }
            string reading = updating->buffer;
            if (reading == "ack"){
                cout << "Received ack\n";
                for (int i = 0; i < L; i++){
                    if (updating->neededdata->broadcasted[i] == 0 && updating->neededdata->checkpoints[i] == 1){
                        updating->neededdata->broadcasted[i] = 1;
                        string temp = to_string(i) + "\n" + updating->neededdata->data[i];
                        const char *temp1 = temp.c_str();
                        send(sock, temp1, strlen(temp1), 0);
                        break;
                    }
                }
            }
        }
    }
    int a = 2;
    int *b = &a;
    void *c = (void *)b;
    return c;
}

void *clientbroadcast(void *args){

    struct mydata *needdata = (struct mydata *)args;
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
                // Handle error
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

    // cout << "Connected\n";
    vector<pthread_t> broadcasters(N);
    vector<struct updated> arguments(N);
    for (int i = 0; i < N; i++){
        if (i != needdata->clientid){
            arguments[i].buffer = buffers[i];
            arguments[i].socket = newsockets[i];
            arguments[i].neededdata = needdata;
            arguments[i].start = 1;
            pthread_create(&broadcasters[i], NULL, updating, &arguments[i]);
        }
    }
    for (int i = 0; i < N; i++){
        if (i != needdata->clientid)
            pthread_join(broadcasters[i], NULL);
    }
    int a = 2;
    int *b = &a;
    void *c = (void *)b;
    return c;
}
