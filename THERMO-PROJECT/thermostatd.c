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
#include <curl/curl.h>
#include <argp.h>

#define DAEMON_NAME     "thermostat-projectd"
#define URL             "" /*URL FOR SERVER?*/
/* I was having a hard time figuring out AWS and mysql*/

#define OK              0
#define ERR_SETSID      2
#define ERROR           3
#define ERR_FORK        4
#define ERR_CHDIR       5
#define INIT_ERR        6
#define true            0
/*Case Values*/
#define SIGTERM         15
#define SIGHUP          1
/*Error Format*/
char *ERROR_FORMAT = "Format Error";



/****************Define functions**********/
void _signal_handler(const int signal);
void _do_work(void);
void HELP();
int readtemp();
/******************************************/


int main(int argc, char **argv) {


    openlog(DAEMON_NAME, LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_DAEMON);
    syslog(LOG_INFO, "starting sampled");
     syslog(LOG_INFO, "Recieving Arguments");

    if(argc < 1) {
			printf("Empty Argument.\n");
			printf("Exiting...\n");
            HELP();
			return INIT_ERR;
			
		}

	if(argc == 1) {
    /* Only one argument ( the starting argument )*/

		syslog(LOG_INFO, "Running Thermostatd!\n");


	}
    /* more than one argument? this must mean help was called */
	else if (argc > 1) {
		syslog(LOG_INFO, "Recieved Argument!\n");

		for(int i = 1; i < argc; i++) {
            /* check for -h, or --h */
			if((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) 
            {
				HELP();
                exit(OK);
			}
            else{
            printf("Unknown Argument. See -h, or --help, for assistance.\n");
            HELP();
            exit(INIT_ERR);
            }
        }
    }
    

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


    return ERROR;

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

int ReadTemp() {

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
    printf("\nTemp Read: %d\n", tempurature);

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
        printf("HELP: ./project -h OR ./project --help\n");
}