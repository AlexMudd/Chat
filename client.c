#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>



typedef struct dgram{
    uint32_t color;
    uint32_t magic1;
    char name[32];
    uint32_t magic2;
    char msg[1024];
}dgram;



int main(){
    int cd;
	struct sockaddr_in addr;
    struct sockaddr_in sender_addr;
    socklen_t sender_size;
    dgram User;
    dgram Sender;

	cd = socket(AF_INET, SOCK_DGRAM, 0);
	if(cd < 0){
		perror("Error calling socket");
		return __LINE__;
	
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(31005);
	addr.sin_addr.s_addr = inet_addr("25.53.151.122"); ///тут айпи сервера

    fd_set fds;
    printf("Enter color(31 - 37): ");
    scanf("%d", &User.color);
    memset(User.name, '\0', 32);
    printf("Enter your name: ");
    scanf("%s", User.name);
    while(getchar() != '\n');
    User.magic1 = 0xffeeddcc;
    User.magic2 = 0x98765432;

    while(1){
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(cd, &fds);
        select(5, &fds, NULL, NULL, NULL);

        if(FD_ISSET(0, &fds)){
            //printf("Select stdin\n");
            fgets(User.msg, 1024, stdin);
            printf("\n");
            sendto(cd, &User, 1068, 0, (struct sockaddr*)&addr, sizeof(addr));
            //printf("Msg was sent: %s\n", User.msg);
        }
        if(FD_ISSET(cd, &fds)){
            //printf("Select socket\n");
            recvfrom(cd, &Sender, 1068, 0, (struct sockaddr*)&sender_addr, &sender_size);
            printf("\e[%dm%s\e[0m: %s\n",Sender.color, Sender.name, Sender.msg);
        }

    }




    return 0;
}