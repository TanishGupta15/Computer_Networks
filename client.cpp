#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bits/stdc++.h>
using namespace std;
extern void *clientburst(void *args);
extern void *clientrecv(void *args);
extern void *controller(void *args);
extern void *clientbroadcast(void *args);

struct mydata
{
    int checkpoints[1000];
    string data[1000];
    int complete;
};

void client(int checkpoints[], string data[])
{
    pthread_t clientburster, clientrecver, clientcontroller, clientbroadcaster;
    struct mydata args;
    for (int i = 0; i < 1000; i++)
    {
        args.checkpoints[i] = 0;
        args.data[i] = "0";
    }
    args.complete = 0;
    pthread_create(&clientburster, NULL, clientburst, &args);
    // pthread_create(&clientrecver, NULL, clientrecv, &args);
    // pthread_create(&clientbroadcaster, NULL, clientbroadcast, &args);
    pthread_create(&clientcontroller, NULL, controller, &args);

    pthread_join(clientburster, NULL);
    // pthread_join(clientrecver,NULL);
    // pthread_join(clientbroadcaster, NULL);
    pthread_join(clientcontroller, NULL);

    printf("\nCompleted File transfer\n");

    return;
}

int main()
{
    int checkpoints[1000] = {0};
    string data[1000] = {"0"};
    client(checkpoints, data);
    return 0;
}
