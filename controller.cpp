#include <stdio.h>
#include <string.h>
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

//TODO: Log data to some file to draw graphs
void *controller(void *args) {
    struct mydata* mydat = (struct mydata*) args;
    while (mydat->complete == 0) {
        int chk = 1, cnt = 0;
        for (int i = 0; i < L; i++){
            if (mydat->checkpoints[i] == 0){
                chk = 0;
                // break;
            }
            else cnt++;
        }
        if (chk == 1){
            mydat->complete = 1;
        }
        if(cnt % 100 == 0)
            cout << "Received " << cnt << " packets\n";
    }
    int a = 2;
    int *b = &a;
    void *c = (void *)b;
    return c;
}