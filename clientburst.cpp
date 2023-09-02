#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
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

void *clientburst(void *args){
	struct mydata *needdata = (struct mydata *)args;
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	const char *hello = "SENDLINE\n";

	char buffer[BUFFER_SIZE];
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
		int a = 2;
		int *b = &a;
		void *c = (void *)b;
		return c;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "10.237.26.109", &serv_addr.sin_addr) <= 0){
		int a = 2;
		int *b = &a;
		void *c = (void *)b;
		return c;
	}

	if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
						  sizeof(serv_addr))) < 0){
		printf("\nConnection Failed \n");
		int a = 2;
		int *b = &a;
		void *c = (void *)b;
		return c;
	}

	int cnt = 0;

	while (needdata->complete == 0) {
		// sleep(2);
		if(send(client_fd, hello, strlen(hello), 0) < 0){
			perror("send");
			continue;
		}

		// recv the data from the server
		int cnt = 0;
		// while(true){
		// 	int recv_data = recv(client_fd, buffer + cnt, BUFFER_SIZE, 0);
		// 	if(recv_data < 0){
		// 		perror("recv");
		// 		continue;
		// 	}
		// 	if(buffer[recv_data - 1] == '\n') break;
		// 	cnt += recv_data;
		// }
		//TODO: Change this to while loop
		int recv_data = recv(client_fd, buffer, BUFFER_SIZE, 0);
		if(recv_data < 0){
			perror("recv");
			continue;
		}

		string mystring = buffer;
		int res = 0;
		int i = 0;
		// if((int)mystring.size() && mystring[0] != -1) cout << "mystring = " << mystring << endl;
		if((int)mystring.size() && mystring[0] == '-'){
			// This means that the server has sent a -1
			continue;
		}
		while (i < (int)mystring.size() && mystring[i] != '\n'){
			res = 10 * res + (mystring[i] - '0');
			i++;
		}
		if (res >= 0 && res < L){
			string data = "";
			for (int j = i + 1; j < (int)mystring.length(); j++){
				data += mystring[j];
				if(mystring[j] == '\n') break;
			}
			if(needdata->checkpoints[res] == 0){
				cnt++;
			}
			needdata->data[res] = data;
			needdata->checkpoints[res] = 1;
		}
	}

	cout << "Received " << cnt << " packets from server\n";

	const char* submit = "SUBMIT\n";
	while(send(client_fd, submit, strlen(submit), 0) < 0){
		perror("send");
		continue;
	}
	// no need to read anything 
	const char* id = "cs1200397@bitmask\n";
	while(send(client_fd, id, strlen(id), 0) < 0){
		perror("send");
		continue;
	}

	// now send the number of lines to be sent, and then send the lines
	const char* numlines = (to_string(L) + "\n").c_str();
	cout << "Sending " << numlines << " to server\n";
	while(send(client_fd, numlines, strlen(numlines), 0) < 0){
		perror("send");
		continue;
	}

	for(int i = 0; i < L; i++){
		const char* temp = (to_string(i) + "\n").c_str();
		while(send(client_fd, temp, strlen(temp), 0) < 0){
			perror("send");
			continue;
		}
		const char* data = (needdata->data[i]).c_str();
		while(send(client_fd, data, strlen(data), 0) < 0){
			perror("send");
			continue;
		}
		cout << "Sent packet_num = " << i << endl;
	}

	cout << "Done sending, yay :)\n";

	string reading = "";
	int count = 0;
	while(count < 1){
		int x = recv(client_fd, buffer, BUFFER_SIZE, 0);
		if(x < 0){
			perror("recv");
			continue;
		}
		for(int i = 0; i < x; i++){
			if(buffer[i] == '\n') count++;
			reading += buffer[i];
			if(count == 1) break;
		}
	}

	string mystring = buffer;
	for(int i = 0; i < (int)mystring.size(); i++){
		cout << mystring[i];
		if(mystring[i] == '\n') break;
	}

	close(client_fd);
	int a = 2;
	int *b = &a;
	void *c = (void *)b;
	return c;
}
