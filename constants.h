#define N 3 // Number of clients
#define L 1000 // Number of lines to receive
#define PORT 9801 // Self port that connects to the server
#define PORTS 8080 // Base port for communication with other clients
#define BUFFER_SIZE 4096 // Buffer size for receiving data
static const char* client_ips[N] = {"10.194.14.15", "10.194.46.195", "10.194.25.114"}; // IP of clients
// static const char* client_ips[N] = {"10.194.14.15"}; // IP of clients
#define client_id 1 // Client id of this client
#define submitID "cs1200397@bitmask\n" // ID used to submit the assignment
#define serverIP "10.237.26.109"
// #define DEBUG // Comment this line to disable debug messages

#define RETURN(x) do \
{                                       \
    int a = x;                     \
	int *b = &a;                 \
	void *c = (void *)b;   \
	return c;                      \
} while (0);
