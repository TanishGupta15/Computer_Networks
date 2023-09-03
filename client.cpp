#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include "constants.h"
using namespace std;
extern void* clientburst(void *args);
extern void* clientrecv(void *args);
extern void* controller(void *args);
extern void* clientbroadcast(void *args);

struct Client_data{
    bool received[L];
    string data[L];
    bool complete;
	int port[N];
	const char *ips[N];
    // bool broadcasted[L];
    queue<int> broadcast;
    int clientid;
};

void* client(void* arg){
    
    struct Client_data* args = (struct Client_data*) arg;
    pthread_t clientburster, clientrecver, clientcontroller, clientbroadcaster;
    for (int i = 0; i < L; i++) {
        args->received[i] = false;
        args->data[i] = "0";
        // args->broadcasted[i] = false;
    }
    args->complete = 0;

    pthread_create(&clientburster, NULL, clientburst, (void*) &(*args));
    pthread_create(&clientrecver, NULL, clientrecv, (void*) &(*args));
    pthread_create(&clientbroadcaster, NULL, clientbroadcast, (void*) &(*args));
    pthread_create(&clientcontroller, NULL, controller, (void*) &(*args));

    pthread_join(clientburster, NULL);
    pthread_join(clientrecver,NULL);
    pthread_join(clientbroadcaster, NULL);
    pthread_join(clientcontroller, NULL);

    printf("\nCompleted File transfer\n");
	RETURN(0);
}