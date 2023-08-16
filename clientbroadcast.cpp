#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "constants.h"
#include <bits/stdc++.h>
using namespace std;
struct mydata
{
    int checkpoints[L];
    string data[L];
    int complete;
    int port[N - 1];
    char *ips[N - 1];
    int broadcasted[L];
};
struct updated
{
	struct mydata neededdata;
	int socket;
	char *buffer;
    int start;
};

void* updating(void* args){
    struct updated *updating = (struct updated *)args;
    int sock = updating->socket;
    char* buff = updating->buffer;
    char sizeforbuf[2048];
    buff = sizeforbuf;
    while(updating->neededdata.complete==0){
        if(updating->start==1){
            updating->start=0;
            for(int i = 0; i < L;i++){
                if(updating->neededdata.broadcasted[i]==0 && updating->neededdata.checkpoints[i]==1){
                    updating->neededdata.broadcasted[i] = 1;
                    string temp = to_string(i) + "\n" + updating->neededdata.data[i];
                    const char* temp1 = temp.c_str();
                    send(sock,temp1,strlen(temp1),0);
                    break;
                }
            }
        }
        else{
            int val = read(sock,buff,2048);
            string reading = buff;
            if(reading=="ack"){
                for(int i = 0; i < L;i++){
                        if(updating->neededdata.broadcasted[i]==0 && updating->neededdata.checkpoints[i]==1){
                            updating->neededdata.broadcasted[i] = 1;
                            string temp = to_string(i) + "\n" + updating->neededdata.data[i];
                            const char* temp1 = temp.c_str();
                            send(sock,temp1,strlen(temp1),0);
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

void *cleintbroadcast(void *args)
{
    struct mydata *needdata = (struct mydata *)args;
    vector<int> listensockets(N - 1);
    vector<int> newsockets(N - 1);
    vector<char*> buffers(N-1);
    vector<struct sockaddr_in> serveraddrs(N - 1), newcleintaddrs(N - 1);
    for (int i = 0; i < N - 1; i++)
    {
        listensockets[i] = socket(AF_INET, SOCK_STREAM, 0);
        serveraddrs[i].sin_addr.s_addr = INADDR_ANY;
        serveraddrs[i].sin_port = PORTS + i;
        serveraddrs[i].sin_family = AF_INET;
        bind(listensockets[i], (struct sockaddr *)&serveraddrs[i], sizeof(serveraddrs[i]));
        listen(listensockets[i], 3);
        while (1)
        {
            socklen_t addrlen = sizeof(newcleintaddrs[i]);
            if (newsockets[i] = accept(listensockets[i], (struct sockaddr *)&newcleintaddrs[i],&addrlen) < 0){
                cout << "Connection failed\n";
                continue;
            }
            else{
                break;
            }
        }
    }
    vector<pthread_t> broadcasters(N-1);
    vector<struct updated> arguments(N-1);
    for(int i = 0; i < N-1; i++){
        arguments[i].buffer = buffers[i];
        arguments[i].socket = newsockets[i];
        arguments[i].neededdata = *needdata;
        arguments[i].start = 1;
        pthread_create(&broadcasters[i], NULL, updating, &arguments[i]);
    }
    for (int i = 0; i < N - 1; i++)
    {
            pthread_join(broadcasters[i], NULL);
    }
	int a = 2;
	int *b = &a;
	void *c = (void *)b;
	return c;

}
