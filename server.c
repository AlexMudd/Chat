#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct dgram{
    uint32_t color;
    uint32_t magic1;
    char name[32];
    uint32_t magic2;
    char msg[1024];
}dgram;

int main(){
    int sd;
	struct sockaddr_in server_addr;
	struct sockaddr_in cAddr[30];
	int clientsCount = 0;
	socklen_t addr_size;

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0){
		perror("Error calling socket");
		return __LINE__;
	
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(31005);
	server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    
	if(bind(sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("bind");
		return __LINE__;
	}

	dgram user;

	while(1){
		struct sockaddr_in senderS;
		memset(&senderS, 0, sizeof(senderS));
		recvfrom(sd, &user, 1068, 0, (struct sockaddr*)&senderS, &addr_size);
		printf("Recieved\n");

		if(user.magic1 != 0xffeeddcc){
			continue;
		}
		if(user.magic2 != 0x98765432){
			continue;
		}


		//Проверка на нового пользователя
		if(clientsCount == 0){
			cAddr[0] = senderS;
			clientsCount++;
		}
		int flagNew = 1;
		for(int i = 0; i < clientsCount; i++){
			if(cAddr[i].sin_addr.s_addr == senderS.sin_addr.s_addr){
				flagNew = 0;
			}
		}
		if(flagNew == 1){
			cAddr[clientsCount] = senderS;
			clientsCount++;
		}


		for(int i = 0; i < clientsCount; i++){
			if(cAddr[i].sin_addr.s_addr != senderS.sin_addr.s_addr){
				sendto(sd, &user, 1068, 0, (struct sockaddr*)&cAddr[i], addr_size);
				printf("Sent to %u\n", cAddr[i].sin_addr.s_addr);
			}
			else{
				printf("didnt send to sender\n");
			}
		}
	}


















    return 0;
}
