#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include "constants.h"
using namespace std;

struct Client_data{
    bool received[L];
    string data[L];
    bool complete;
	int broadcast[L];
	int clientid;
	int max_idx;
};

void *clientburst(void *args){
	struct Client_data *needdata = (struct Client_data*) args;
	int status, client_fd;
	struct sockaddr_in serv_addr;
	const char *sendline = "SENDLINE\n";

	char buffer[BUFFER_SIZE];
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\nSocket creation error \n");
		RETURN(2);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, serverIP, &serv_addr.sin_addr) <= 0){
		printf("\nInvalid address/ Address not supported \n");
		RETURN(2);
	}

	if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
						  sizeof(serv_addr))) < 0){
		printf("\nConnection Failed \n");
		RETURN(2);
	}

	#ifdef DEV
		int count = 0; //number of line breaks
		const char* session_reset = "SESSION RESET\n";
		while(send(client_fd, session_reset, strlen(session_reset), 0) < 0){
			perror("send");
			continue;
		}
		string str = "";
		while(count < 1){
			int x = recv(client_fd, buffer, BUFFER_SIZE, 0);
			if(x < 0){
				perror("recv3");
				continue;
			}
			for(int i = 0; i < x; i++){
				if(buffer[i] == '\n') count++;
				str += buffer[i];
				if(count == 1) break;
			}
		}
		assert(str == "Ok\n");
	#endif
	
	int cnt = 0;
	while (!needdata->complete) {
		if(send(client_fd, sendline, strlen(sendline), 0) < 0){
			perror("send");
			continue;
		}

		int count = 0; //number of line breaks
		string mystring = "";
		while(count < 2){
            int x = recv(client_fd, buffer, BUFFER_SIZE, 0);
            if(x < 0){
                perror("recv4");
                continue;
            }
            for(int i = 0; i < x; i++){
                if(buffer[i] == '\n') count++;
                mystring += buffer[i];
                if(count == 2) break;
            }
        }

		int line_num = 0;
		int i = 0;
		if((int)mystring.size() && mystring[0] == '-'){
			// This means that the server has sent a -1
			continue;
		}
		while (i < (int)mystring.size() && mystring[i] != '\n'){
			line_num = 10 * line_num + (mystring[i] - '0');
			i++;
		}
		if (line_num >= 0 && line_num < L && !needdata->received[line_num]){
			string data = "";
			for (int j = i + 1; j < (int)mystring.length(); j++){
				data += mystring[j];
				if(mystring[j] == '\n') break;
			}
			cnt++;
			// cout << line_num << endl;
			needdata->data[line_num] = data;
			needdata->received[line_num] = true;
			needdata->broadcast[++needdata->max_idx] = line_num;
		}
	}

	// #ifdef DEBUG
		cout << "Received: \n" << cnt << " lines from server\n" << L - cnt << " lines from other clients\n";
	// #endif

	const char* submit = "SUBMIT\n";
	while(send(client_fd, submit, strlen(submit), 0) < 0){
		perror("send");
		continue;
	}
	// no need to read anything 
	const char* id = submitID;
	while(send(client_fd, id, strlen(id), 0) < 0){
		perror("send");
		continue;
	}

	// now send the number of lines to be sent, and then send the lines
	const char* numlines = (to_string(L) + "\n").c_str();
	cout << "Sending " << L << " lines to server\n";
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
		// cout << "Sent line_num = " << i << endl;
	}

	cout << "Done sending, yay :)\n";

	string reading = "";
	cnt = 0;
	while(cnt < 1){
		int x = recv(client_fd, buffer, BUFFER_SIZE, 0);
		if(x < 0){
			perror("recv1");
			continue;
		}
		for(int i = 0; i < x; i++){
			if(buffer[i] == '\n') cnt++;
			reading += buffer[i];
			if(cnt == 1) break;
		}
	}
	cout << reading << endl;
	if(close(client_fd) < 0){
		cout << "Unable to close the socket to the server\n";
		perror("close");
	}
	RETURN(0);
}
