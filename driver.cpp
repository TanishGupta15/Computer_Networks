#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bits/stdc++.h>
using namespace std;
extern void* client(void* args);

struct mydata
{
    int checkpoints[1000];
    string data[1000];
    int complete;
    int port[3];
    string ips[3];
};

int main(){
    return 0;
}