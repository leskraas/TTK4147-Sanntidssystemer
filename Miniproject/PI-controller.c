#include <stdio.h>
#include <string.h>
#include "client/miniproject.h"

static int port = 9999;
static char ip[] = "127.0.0.1";

int main(void)
{
	//----------------------
	struct udp_conn udp;
	udp_init_client(&udp, port, ip);


	//-----------START-----------
	int len = 6;
	char buf[6] = "START";

	udp_send(&udp, buf, len);



	//-----------GET-----------
	udp_send(&udp, "GET", 4);


	//-----------GET_ACK-----------
	char msg_receive[100];
	char *found;
	udp_receive(&udp, msg_receive, 100);
	
	found = strtok(msg_receive, ":");
	while(found != NULL){
		found = strtok(NULL, ":");
	}
	

	
        //printf("%s\n",found);


	//-----------SET-----------
	char set[100];
	strcpy(set, "SET:");
	strcat(set, found); 
	udp_send(&udp, set, 100);


	//-----------STOP-----------
	udp_send(&udp, "STOP", 5);

	udp_close(&udp);
	return 0;
}


