#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
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
	int broadcasted[L];
};

void *clientburst(void *args)
{
	struct mydata *needdata = (struct mydata *)args;
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	const char *hello = "SENDLINE\n";
	char buffer[1024] = {0};
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		int a = 2;
		int *b = &a;
		void *c = (void *)b;
		return c;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "10.237.26.109", &serv_addr.sin_addr) <= 0)
	{
		int a = 2;
		int *b = &a;
		void *c = (void *)b;
		return c;
	}

	if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
						  sizeof(serv_addr))) < 0)
	{
		printf("\nConnection Failed \n");
		int a = 2;
		int *b = &a;
		void *c = (void *)b;
		return c;
	}

	while (needdata->complete == 0)
	{
		// printf("loop2\n");
		send(client_fd, hello, strlen(hello), 0);
		valread = read(client_fd, buffer, 2048);
		string mystring = buffer;
		// printf("%s\n",mystring);
		int res = 0;
		int i = 0;
		while (mystring[i] != '\n')
		{
			// printf("here\n");
			if (mystring[0] == '-')
			{
				break;
			}
			res = 10 * res + (mystring[i] - '0');
			i++;
		}
		printf("%d Complete value", needdata->complete);
		if (res >= 1 && res <= L)
		{
			printf("%d Number filled\n", res);
			needdata->checkpoints[res - 1] = 1;
		}
	}
	close(client_fd);
	int a = 2;
	int *b = &a;
	void *c = (void *)b;
	return c;
}
