#include<stdio.h>	//printf
#include<string.h>	//strlen
#include<sys/socket.h>	//socket
#include<arpa/inet.h>	//inet_addr
#include <sys/time.h>

struct timeval tv1,tv2,dtv;
struct timezone tz;


int time_start() {
	gettimeofday(&tv1, &tz);
	return tv1.tv_sec*1000+tv1.tv_usec/1000;
}

long time_stop()
{ gettimeofday(&tv2, &tz);
dtv.tv_sec= tv2.tv_sec -tv1.tv_sec;
dtv.tv_usec=tv2.tv_usec-tv1.tv_usec;
if(dtv.tv_usec<0) { dtv.tv_sec--; dtv.tv_usec+=1000000; }
return dtv.tv_usec/1000;
}


int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000] = {0} ;
	char server_reply[2000] = {0};
	int count_ok = 0;
	int start = 0;
	int old = 0;

	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr("192.168.1.14");
	server.sin_family = AF_INET;
	server.sin_port = htons( 80 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

	puts("Connected\n");




	printf("Enter message : ");
	scanf("%s" , message);

	start = old = time_start();

	memset(message, '1', 64);

	//keep communicating with server
	while(1)
	{
		//		printf("message : %s\n", message);
		//		printf("size : %d\n", strlen(message));

		//Send some data
		if( send(sock , message , 64 , 0) < 0)
		{
			//			puts("Send failed");
			//			return 1;
		}
		else
		{

			count_ok++;
		}

		start = time_start();

		if ((start - old) >= 5000) {
			old = start;
			printf("recieve = %d kB/sec\n", count_ok*64 / 5 / 1000);

			count_ok = 0;
		}

		//		//Receive a reply from the server
		//		if( recv(sock , server_reply , 2000 , 0) < 0)
		//		{
		//			puts("recv failed");
		//			break;
		//		}
		//
		//		puts("Server reply :");
		//		puts(server_reply);
	}

	close(sock);
	return 0;
}
