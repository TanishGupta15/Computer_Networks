#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include "constants.h"
using namespace std;

struct mydata
{
    int checkpoints[L];
    string data[L];
    int complete;
    int port[N-1];
    const char* ips[N-1];
    int broadcasted[L];
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
        ports.push_back(PORTS+i);
    }

    string myIP = "127.0.0.1";
    const char* ips = myIP.c_str();

    for(int i = 0; i < N;i++){
        for(int j = 0; j < N;j++){
            if(i!=j){
                int x = (j < i) ? j: j - 1;
                args[i].port[x] = ports[j];
                args[i].ips[x] = ips;
            }
        }
    }

    for(int i = 0; i < N;i++){
        pthread_create(&clients[i],NULL,client, (void*) &args[i]);
    }

    for(int i = 0; i < N;i++){
        pthread_join(clients[i],NULL);
    }
    
    cout << "Completed Session\n";
    return 0;
}