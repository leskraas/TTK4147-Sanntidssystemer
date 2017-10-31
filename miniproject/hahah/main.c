#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../client/miniproject.h"

void timespec_add_us(struct timespec *t, long us);
void* mini(void*);
void* udp_listener(void*);
void* sig_ack(void*);

struct udp_conn UDP;

char IP[] = "192.168.0.1";
int PERIOD_US = 3000;
int RUNTIME = 0;
const int Kp = 10;
const int Ki = 800;
const int LENGTH = 128;
double Y = 0;
int SIG = 0;

pthread_mutex_t RSS_Y;
pthread_mutex_t RSS_SIG;
pthread_mutex_t RSS_UDP;



int main(void)
{
	char buf[128];
	udp_init_client(&UDP, 9999, &IP);


	
	strcpy(buf, "START\0");
	udp_send(&UDP, buf, 6);

	pthread_t doer;	
	pthread_t listener;
	pthread_t sig;

	pthread_create(&doer, NULL, mini, NULL);
	pthread_create(&listener, NULL, udp_listener, NULL);
	pthread_create(&sig, NULL, sig_ack, NULL);

	pthread_join(&sig, NULL);
	pthread_join(&listener, NULL);
	pthread_join(&doer, NULL);
	printf("Ferdig\n");
    return 0;
}

void* udp_listener(void* arg){
	char buf[128];
	printf("Starting udp_listener\n");


	strcpy(buf, "GET\0");
	udp_send(&UDP, buf, 4);


	while(1){
		strcpy(buf, "GET_ACK\0");
		udp_receive(&UDP, buf, LENGTH);
		pthread_mutex_lock(&RSS_Y);
		char *temp;
		temp = strtok(buf, ":");
		if(!strcmp(temp, "GET_ACK")){
			Y = atof(strtok(NULL, ":"));
		}
		pthread_mutex_unlock(&RSS_Y);

		//char buf2[128];
		strcpy(buf, "SIGNAL\0");
		udp_receive(&UDP, buf, LENGTH);
		pthread_mutex_lock(&RSS_Y);
		temp = strtok(buf, ":");
		if(!strcmp(temp, "SIGNAL")){
			SIG = 1;
		}
		pthread_mutex_unlock(&RSS_Y);
	}
}



void* mini(void* arg){
	printf("Starting mini\n");
	
	int ref = 1;
	double integral = 0;
	char buf[128];

	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	timespec_add_us(&now, PERIOD_US);
	int i = 0;

	while(i < ((0.5*1e6)/(PERIOD_US))){
		strcpy(buf, "GET\0");
		pthread_mutex_lock(&RSS_UDP);
		udp_send(&UDP, buf, 4);
		pthread_mutex_unlock(&RSS_UDP);

		pthread_mutex_lock(&RSS_Y);
		double error = ref - Y;
		pthread_mutex_unlock(&RSS_Y);

		integral = integral + error * PERIOD_US/1e6;
		double u = Kp * error + Ki * integral;


		//printf("Going into set with u = %.2f\n", u);
		sprintf(buf, "SET:%f", u);
		pthread_mutex_lock(&RSS_UDP);
		udp_send(&UDP, buf, (4 + strlen(buf)));
		pthread_mutex_unlock(&RSS_UDP);


		clock_nanosleep(&now);
		timespec_add_us(&now, PERIOD_US);
		i = i + 1;
	}
	strcpy(buf, "STOP\0");
	udp_send(&UDP, buf, strlen(buf));

	udp_close(&UDP);
}


void* sig_ack(void* arg){
	while(1){
		pthread_mutex_lock(&RSS_SIG);
		if(SIG == 1){
			pthread_mutex_lock(&RSS_UDP);
			udp_send(&UDP, "SIGNAL_ACK", 11);
			pthread_mutex_unlock(&RSS_UDP);
			SIG = 0;
		}
		pthread_mutex_unlock(&RSS_SIG);
	}
}
