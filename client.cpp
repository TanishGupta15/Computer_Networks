#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include "constants.h"
using namespace std;
extern void *clientburst(void *args);
extern void *clientrecv(void *args);
extern void *controller(void *args);
extern void *clientbroadcast(void *args);

struct mydata
{
    int checkpoints[L];
    string data[L];
    int complete;
    int port[N-1];
    char* ips[N-1];
    int broadcasted[L];
};

void* client(void* arg)
{
    struct mydata* args = (struct mydata*)arg;
    pthread_t clientburster, clientrecver, clientcontroller, clientbroadcaster;
    for (int i = 0; i < L; i++)
    {
        args->checkpoints[i] = 0;
        args->data[i] = "0";
    }
    args->complete = 0;
    pthread_create(&clientburster, NULL, clientburst, &args);
    pthread_create(&clientrecver, NULL, clientrecv, &args);
    pthread_create(&clientbroadcaster, NULL, clientbroadcast, &args);
    pthread_create(&clientcontroller, NULL, controller, &args);

    pthread_join(clientburster, NULL);
    pthread_join(clientrecver,NULL);
    pthread_join(clientbroadcaster, NULL);
    pthread_join(clientcontroller, NULL);

    printf("\nCompleted File transfer\n");

	int a = 2;
	int *b = &a;
	void *c = (void *)b;
	return c;
}

// int main(){
//     return 0;
// }


