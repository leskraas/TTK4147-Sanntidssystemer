#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "client/miniproject.h"

static int port = 9999;
static char ip[] = "127.0.0.1";

int main(void)
{
	//------------UDP INIT----------
	struct udp_conn udp;
	if(udp_init_client(&udp, port, ip)){
		printf("Connection failed\n");
		exit(1);
	}
	


	//-----------START-----------
	char buf[128];
	strcpy(buf, "START");

	udp_send(&udp, buf, 6);



	//-----------GET-----------
	strcpy(buf, "GET");
	udp_send(&udp, buf, 4);


	//-----------GET_ACK-----------
	char msg_receive[128];
	char *msg_receive_command;
	char *msg_receive_y;
	int res = 0;

	udp_receive(&udp, msg_receive, 128);
	msg_receive_command = strtok(msg_receive, ":");
	msg_receive_y = strtok(NULL, ":");
	printf("%s\n", msg_receive_y);
	



	//-----------SET-----------
	char msg_set[128];
	strcpy(msg_set, "SET:");
	strcat(msg_set, msg_receive_y); 
	printf("%s\n", msg_set);
	udp_send(&udp, msg_set, 128);


	//-----------STOP-----------
	udp_send(&udp, "STOP", 5);

	udp_close(&udp);
	return 0;
}


