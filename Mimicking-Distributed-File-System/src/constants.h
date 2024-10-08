#define N 3 // Number of clients
#define L 1000 // Number of lines to receive
#define PORT 9803 // Self port that connects to the server
#define PORTS 8080 // Base port for communication with other clients
#define BUFFER_SIZE 40960 // Buffer size for receiving data
static const char* client_ips[N] = {"10.194.17.172", "10.194.10.202", "10.194.36.225"}; // IP of clients
// static const char* client_ips[N] = {"10.194.23.67"}; // IP of clients
#define client_id 1 // Client id of this client
#define submitID "cs1200394@bitmask\n" // ID used to submit the assignment
#define serverIP "10.17.7.218"
// #define DEBUG // Comment this line to disable debug messages
// #define PLOT
#define DEV // This is for the SESSION RESET command that will not be avaiable in final submission
#define RETURN(x) do \
{                                       \
    static int a = x;                     \
	int *b = &a;                 \
	void *c = (void *)b;   \
	return c;                      \
} while (0);
