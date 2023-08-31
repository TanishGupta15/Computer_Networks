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
        // args[i].ips[i] = myIP.c_str();
    }

    string ip0 = "10.194.46.195";
    args[0].ips[0] = ip0.c_str();
    string ip1 = "10.194.14.15";
    args[1].ips[1] = ip1.c_str();



    // for(int i = 0; i < N;i++){
    //     // printf("this %p",&args[0]);
    //     pthread_create(&clients[i],NULL,client, (void*) &args[i]);
    // }

    // for(int i = 0; i < N; i++){
    //     pthread_join(clients[i],NULL);
    // }

    client((void*) &args[1]);
    
    cout << "Completed Session\n";
    return 0;
}