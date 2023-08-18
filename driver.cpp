#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include "constants.h"

using namespace std;
extern void* client(void* args);


int main(){
    pthread_t client1, client2, client3 ,client4;
    // broadcast ports
    int port1 = 8080;
    int port2 = 8081;
    int port3 = 8082;
    int port4  = 8083;
    struct mydata args1, args2, args3, args4;
    args1.port[0] = port2; args1.port[1] = port3; args1.port[2] = port4;
    args2.port[0] = port1; args2.port[1] = port3; args2.port[2] = port4;
    args3.port[0] = port1; args3.port[1] = port2; args3.port[2] = port4;
    args4.port[0] = port1; args4.port[1] = port2; args4.port[2] = port3;
    args1.ips[0] = "127.0.0.1"; args1.ips[1] = "127.0.0.1"; args1.ips[2] = "127.0.0.1";
    args2.ips[0] = "127.0.0.1"; args2.ips[1] = "127.0.0.1"; args2.ips[2] = "127.0.0.1";
    args3.ips[0] = "127.0.0.1"; args3.ips[1] = "127.0.0.1"; args3.ips[2] = "127.0.0.1";
    args4.ips[0] = "127.0.0.1"; args4.ips[1] = "127.0.0.1"; args4.ips[2] = "127.0.0.1";
    pthread_create(&client1, NULL, client, &args1);
    pthread_create(&client2, NULL, client, &args2);
    pthread_create(&client3, NULL, client, &args3);
    pthread_create(&client4, NULL, client, &args4);
    pthread_join(client1, NULL);
    pthread_join(client2, NULL);
    pthread_join(client3, NULL);
    pthread_join(client4, NULL);
    cout << "Completed Session\n";
    return 0;
}