#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include "constants.h"
using namespace std;

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

//TODO: Log data to some file to draw plots
void *controller(void *args) {
    struct Client_data* data = (struct Client_data*) args;
    bool printed = false;
    int checkpoint = L/10;
    while (!data->complete) {
        int received_cnt = 0;
        for (int i = 0; i < L; i++){
            if (data->received[i]){
                received_cnt++;
            }
        }
        if (received_cnt == L){
            data->complete = true;
        }
        if(received_cnt % checkpoint == 0){
            if(!printed){
                cout << "Received " << received_cnt << " lines\n";
                printed = true;
            }
        }
        else printed = false;
    }
    RETURN(0);
}