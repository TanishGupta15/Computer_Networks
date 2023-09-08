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
    vector<int> broadcast;
    int clientid;
};

void *controller(void *args) {
    struct Client_data* data = (struct Client_data*) args;
    bool printed = false;
    int checkpoint = L/10;
    #ifdef PLOT
        ofstream fout("../logs/latency.csv");
        fout << "Number of lines received,Time\n";
        clock_t start = clock();
        int last_cnt = 0;
    #endif
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
        #ifdef PLOT
            if(received_cnt > last_cnt){
                clock_t end = clock();
                // report the time in milliseconds
                double time_taken = (double(end - start) * 1000) / double(CLOCKS_PER_SEC);
                fout << received_cnt << "," << time_taken << endl;
                last_cnt = received_cnt;
            }
        #endif

        if(received_cnt % checkpoint == 0){
            if(!printed){
                cout << "Received " << received_cnt << " lines\n";
                printed = true;
            }
        }
        else printed = false;
    }
    #ifdef PLOT
        fout.close();
    #endif
    RETURN(0);
}