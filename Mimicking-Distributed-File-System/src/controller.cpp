#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include "constants.h"
using namespace std;

struct Client_data{
    bool received[L];
    string data[L];
    bool complete;
    int broadcast[L];
    int clientid;
    int max_idx;
};

void *controller(void *args) {
    struct Client_data* data = (struct Client_data*) args;
    bool printed = false;
    int checkpoint = L/10;
    #ifdef PLOT
        ofstream fout("../logs/latency.csv");
        fout << "Number of lines received,Time\n";
        // auto start = time(NULL);
        // use chrono
        auto start = std::chrono::high_resolution_clock::now();
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
                // auto end = time(NULL);
                // use chrono
                auto end = std::chrono::high_resolution_clock::now();
                // report the time in milliseconds
                double time_taken = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                time_taken /= (double)1000;
                // cout << time_taken << " ms\n";
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