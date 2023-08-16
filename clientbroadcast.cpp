#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "constants.h"
#include <bits/stdc++.h>
using namespace std;
struct mydata
{
    int checkpoints[L];
    string data[L];
    int complete;
    int port[N - 1];
    char *ips[N - 1];
};

void *cleintbroadcast(void *args)
{
    struct mydata *neededdata = (struct mydata *)args;
    vector<int> listensockets(N - 1);
    vector<int> newsockets(N - 1);
    vector<struct sockaddr_in> serveraddrs(N - 1), newcleintaddrs(N - 1);
    for (int i = 0; i < N - 1; i++)
    {
        listensockets[i] = socket(AF_INET, SOCK_STREAM, 0);
        serveraddrs[i].sin_addr.s_addr = INADDR_ANY;
        serveraddrs[i].sin_port = PORTS + i;
        serveraddrs[i].sin_family = AF_INET;
        bind(listensockets[i], (struct sockaddr *)&serveraddrs[i], sizeof(serveraddrs[i]));
        listen(listensockets[i], 3);
        while (1)
        {
            socklen_t addrlen = sizeof(newcleintaddrs[i]);
            if (newsockets[i] = accept(listensockets[i], (struct sockaddr *)&newcleintaddrs[i],&addrlen) < 0){
                cout << "Connection failed\n";
                continue;
            }
            else{
                break;
            }
        }
    }
}
