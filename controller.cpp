#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include "constants.h"
using namespace std;
struct mydata
{
    int checkpoints[L];
    string data[L];
    int complete;
    int port[N-1];
    char* ips[N-1];
};

void *controller(void *args)
{
    struct mydata *mydat = (struct mydata *)args;
    while (mydat->complete == 0)
    {
        // printf("here\n");
        int chk = 0;
        for (int i = 0; i < L; i++)
        {
            if (mydat->checkpoints[i] == 0)
            {
                chk = 0;
                break;
            }
        }
        if (chk == 1)
        {
            mydat->complete = 1;
        }
    }
    int a = 2;
    int *b = &a;
    void *c = (void *)b;
    return c;
}