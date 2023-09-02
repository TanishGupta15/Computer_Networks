#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include "constants.h"
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

extern void* client(void* args);

int main(){

    vector<pthread_t> clients(N);
    vector<struct mydata> args;
    vector<int> ports;
    for(int i = 0; i < N;i++){
        struct mydata temp;
        args.push_back(temp);
        args[i].complete = 0;
    }

    string myIP = "127.0.0.1";

    for(int i = 0; i < N; i++){
        args[i].clientid = i;
        #ifdef SINGLE
            args[i].ips[i] = myIP.c_str();
        #endif
    }

    #ifndef SINGLE
        args[0].ips[0] = IP0;
        args[1].ips[1] = IP1;
        args[2].ips[2] = IP2;
    #endif


    #ifdef SINGLE
        for(int i = 0; i < N;i++){
            // printf("this %p",&args[0]);
            pthread_create(&clients[i],NULL,client, (void*) &args[i]);
        }

        for(int i = 0; i < N; i++){
            pthread_join(clients[i],NULL);
        }
    #endif

    #ifndef SINGLE
        client((void*) &args[0]);
    #endif
    
    cout << "Completed Session\n";
    return 0;
}