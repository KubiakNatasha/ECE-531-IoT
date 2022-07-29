#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <argp.h>



#define DAEMON_NAME     "thermostat-projectd"
#define URL             "" /*URL FOR SERVER?*/

#define OK              0
#define ERR_SETSID      1
#define ERROR           2
#define ERR_FORK        3
#define ERR_CHDIR       4
#define true            0
/*Case Values*/
#define SIGTERM         2
#define SIGHUP          3
/*Error Format*/
#define INIT_ERR        5
#define REQ_ERR         6   
char *ERROR_FORMAT = "Format Error";



/****************Define functions**********/
void _signal_handler(const int signal);
void _do_work(void);
void HELP();
int tempurature_get();
/******************************************/

/******************STRUCT******************/
typedef struct {
	int hr;
	int min;
}time_format;



int main(int argc, char *argv[]) {

 openlog(DAEMON_NAME, LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_DAEMON);
 syslog(LOG_INFO, "starting thermostat-projectd");

 

/*****Want to Read Tempurature Continuously*****/
    while(1){
		int temp = read_temp();
/*****As tempurature is read, check for celcius value above certain threshold, if so turn OFF heater*******/
/*****Celcius above 30, or 86 F , heater OFF *****/
        if(temp >= 30) {
			FILE *filep;
			filep = fopen("/tmp/status", "wb");
			char *status = "OFF";
			fprintf(filep, "%s", status);
			fclose(filep);
			syslog(LOG_INFO, "OFF\n");

		}
		else if (temp <= 30) {
			FILE *filep;
			filep = fopen("/tmp/status", "wb");
			char *status = "ON";
			fprintf(filep, "%s", status);
			fclose(filep);
			syslog(LOG_INFO, "ON\n");
		}



    }

  
    syslog(LOG_INFO, "starting thermostat-projectd");
    

    /* fork off the parent process*/
    pid_t pid = fork();
    
    //check if child
    if (pid < 0){
        syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
        return ERR_FORK;
    }
    // check if parent process, continue
    if (pid > 0){
        return OK;
    }



    if(setsid() < -1) {
        syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
        return ERR_SETSID;
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);


    umask(S_IRUSR | S_IWUSR | S_IRGRP |S_IROTH);

    //Set the working directory.
    //Set to Root to make navigation easy

    if(chdir("/") < 0) {
        syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
        return ERR_CHDIR;
    }

    //signal handling

    signal(SIGTERM, _signal_handler);
    signal(SIGHUP, _signal_handler);
    
    _do_work();


}

    //work will be done by daemon
    //counts and sleeps
    //declared as non-static
void _do_work(void){
  

    for (int i = 0; 100; i++){

        syslog(LOG_INFO, "iteration:%d", i);
        sleep(1);
    }
}



    //signal handler which will handle any signals passed
    //declared as non static
void _signal_handler(const int signal) {

    switch(signal) {

        case SIGHUP:
            break;

        case SIGTERM:
            syslog(LOG_INFO, "recieved SIGTERM, exiting.");
            closelog();
            exit(OK);
            break;

        default:
            syslog(LOG_INFO, "recieved unhandled signal.");
            closelog();
            exit(OK);

    }
}

int tempurature_get() {

    char *p;
    char temp[100];
	FILE* fp = fopen("/tmp/temp", "rb");

	if(fp == NULL) {
		printf("Error accessing file\n");
		return 1;
	}

	fgets(temp, sizeof(temp), fp);
	fclose(fp);

	int tempurature = strtol(temp, &p, 10);
    printf("\nTemp Read: %d\n", temp);

	return temp;
}



void HELP() {
        printf("\nTHERMOSTAT HELP:\n");
		printf("-------------------------\n");
        printf("EXIT DEFINITIONS\n");
        printf("-------------------------\n");
        printf("ERR_SETSID      1\n");
        printf("ERROR           2\n");
        printf("ERR_FORK        3\n");
        printf("ERR_CHDIR       4\n");
        printf("INIT_ERR        5\n");
        printf("REQ_ERR         6\n");
        printf("-------------------------\n");
        printf("COMMAND LINE INPUT FORMAT\n");
        printf("-------------------------\n");
        printf("START:\t./project\n");
        printf("HELP:\t./project -h\t OR\t ./project --help\n");
}