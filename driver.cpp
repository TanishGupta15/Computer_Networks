#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include "constants.h"
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

extern void* client(void* args);

int main(){

    // assert((int)client_ips.size() == N);
    vector<pthread_t> clients(N);
    vector<struct Client_data> args;
    vector<int> ports;
    for(int i = 0; i < N;i++){
        struct Client_data temp;
        args.push_back(temp);
        args[i].complete = 0;
        args[i].clientid = i;
        args[i].ips[i] = client_ips[i];
    }
    client((void*) &args[client_id]);
    cout << "Completed Session\n";
    return 0;
}