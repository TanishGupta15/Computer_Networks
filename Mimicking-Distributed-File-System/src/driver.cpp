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
	// int port[N];
    // const char *ips[N];
    // vector<int> broadcast;
    int broadcast[L];
    int clientid;
    int max_idx;
};

extern void* client(void* args);

int main(){

    // assert((int)client_ips.size() == N);
    struct Client_data* args = (struct Client_data*) malloc(sizeof(struct Client_data));
    args->complete = 0;
    args->clientid = client_id;
    args->max_idx = -1;
    client((void*) &(*args));
    cout << "Completed Session\n";
    // deallocate memory
    free(args);
    return 0;
}