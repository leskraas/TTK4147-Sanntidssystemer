#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "client/minIProject.h"

#define PORT 9999
#define IP "127.0.0.1"
#define RUNTIME_NS 500000000 //0,5sek
#define RUNTIME_US 500000 //0,5sek
#define RUNTIME 0.5
#define PERIOD_NS 4000000 //0,004sek
#define PERIOD_US 4000 //0,004sek
#define PERIOD 0.004
#define KP 10
#define KI 800
#define NOISE 3
#define REFERENCE 1

#define ITERATIONS_COUNT (RUNTIME / PERIOD)
static int iterations = 0;


int main(void)
{
	//------------UDP INIT----------
	struct udp_conn udp;
	if(udp_init_client(&udp, PORT, IP)){
		printf("Connection failed\n");
		exit(1);
	}
	


	//-----------START-----------
	char buf[128];
	strcpy(buf, "START");

	udp_send(&udp, buf, 6);


	while(iterations < ITERATIONS_COUNT){

		double integral = 0;
		double u = 0;
		//struct timespec next;
		//int r;
		//r = clock_gettime(CLOCK_REALTIME, &next);


	//-----------GET-----------
		strcpy(buf, "GET");
		udp_send(&udp, buf, 4);


	//-----------GET_ACK-----------
		char msg_received[128];
		char *msg_receivedd_command;
		double msg_receivedd_y;
		double set_val;

		udp_receive(&udp, msg_received, 128);
		msg_receivedd_command = strtok(msg_received, ":");
		msg_receivedd_y = atof(strtok(NULL, ":"));
		printf("y: %f\n", msg_receivedd_y);


		integral += ( (REFERENCE - msg_receivedd_y) * PERIOD);
		u = KP * (REFERENCE - msg_receivedd_y) + KI * integral;

	//-----------SET-----------
		char msg_set[128];
		sprintf(msg_set, "SET:%f", u);
		printf("%s\n", msg_set);
		udp_send(&udp, msg_set, 128);
		//timespec_add_us(&next, PERIOD_US);
		//clock_nanosleep(CLOCK_REALTIME, 0, &next, NULL);
		iterations++;
	}






	//-----------STOP-----------
	udp_send(&udp, "STOP", 5);

	udp_close(&udp);
	return 0;
}


