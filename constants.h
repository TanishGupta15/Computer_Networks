#define N 4
#define L 1000
#define PORT 9801
#define PORTS 8080
struct mydata
{
    int checkpoints[L];
    string data[L];
    int complete;
    int port[N-1];
    char* ips[N-1];
    int broadcasted[L];
};
