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

#define DAEMON_NAME     "thermostatd"
#define URL             "http://ec2-35-160-6-245.us-west-2.compute.amazonaws.com/project.php" /*URL FOR SERVER?*/
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
int ReadTemp();
void HeaterStatus();
int TimeHour();
int TimeMin();
// void GET(CURL *curl, char *postdata);
// void POST(CURL *curl, char *postdata);
// void DELETE(CURL *curl, char *postdata);
/******************************************/


int main(int argc, char **argv) {

  
    // CURL *curl;

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
  

    while(1){
        ReadTemp();
        HeaterStatus();
       // syslog(LOG_INFO, "iteration:%d", i);
        sleep(1);
        /* Wait one second so tempurature can increase */
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

/*****************TEMPURATURE FUNCTIONS***************************/
/**** Read from file /tmp/temp from thermo couple and also   *****/
/**** Write to file to change status to ON or OFF for heater *****/
/*****************************************************************/
int ReadTemp() {
    
    char *p;
    char temp[100];
    syslog(LOG_INFO, "Read Thermocouple File from /tmp/temp\n");
	FILE* fp = fopen("/tmp/temp", "rb");

	if(fp == NULL) {
		syslog(LOG_INFO, "Error Reading File");

		return 1;
	}

	fgets(temp, sizeof(temp), fp);
	fclose(fp);

	int tempurature = strtol(temp, &p, 10);
    printf("\nTemp Read: %d\n", tempurature);

	return tempurature;
}


void HeaterStatus()
{
        int hour = TimeHour();
        int min  = TimeMin();
		int temp = ReadTemp();
/*****As tempurature is read, check for celcius value above certain threshold, if so turn OFF heater*******/
/*****Celcius above 30, or 86 F , heater OFF *****/
        if(temp >= 30) {
			FILE *filep;
			filep = fopen("/tmp/status", "wb");
			char *status = "OFF";
			fprintf(filep, "%s at time:%02d:%02d", status, hour,min);
			fclose(filep);
			syslog(LOG_INFO, "OFF\n"); 

		}
     
		else if (temp <= 30) {
			FILE *filep;
			filep = fopen("/tmp/status", "wb");
			char *status = "ON";
			fprintf(filep, "%s at time:%02d:%02d", status, hour,min);
			fclose(filep);
			syslog(LOG_INFO, "ON\n"); 
                    /* need a timestamp */
		}
}




int TimeHour() {

    int hours;


    time_t now;
    /*Get the current time*/
    time(&now);
    struct tm *local = localtime(&now);
 
    hours = local->tm_hour;         
    return (hours);
 
}


int TimeMin() {

    int minutes;


    time_t now;
    /*Get the current time*/
    time(&now);
    struct tm *local = localtime(&now);
 
    minutes = local->tm_min;         
    return (minutes);
 
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



// void GET(CURL *curl, char *postdata) {
// 	/*GET method means retrieve whatever information 
// 	(in the form of an entity) is identified by the Request-URI.*/


// 	CURLcode res;
// 	curl = curl_easy_init();
// 	int httpStatus = 0;

// 	if(curl) {
// 		curl_easy_setopt(curl, CURLOPT_URL, URL);
// 		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
// 		res = curl_easy_perform(curl);

//             if(res != CURLE_OK) {
//                 fprintf(stderr, "Curl unable to http GET\n %s\n", 
//                         curl_easy_strerror(res));       
//             }
// 				else if(res == CURLE_OK) {
// 				long response_code;
// 				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
// 				printf("\nHTTP RESPONSE CODE: %ld\n", response_code);
// 			}
// 		curl_easy_cleanup(curl);
// 	}
// }


// void POST (CURL *curl, char *postdata) {
// 	/* POST method is used to request that the origin server
// 	 accept the entity enclosed in the request as a new 
// 	 subordinate of the resource identified by the Request-URI
// 	  in the Request-Line.*/

// 	CURLcode res;
// 	curl = curl_easy_init();
// 	int httpStatus = 0;


// 	if(curl) {
// 		curl_easy_setopt(curl, CURLOPT_URL, URL);
// 		curl_easy_setopt(curl, CURLOPT_POST, 1L);
// 		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
// 		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postdata);
// 		res = curl_easy_perform(curl);

//             if(res != CURLE_OK) {
//                 fprintf(stderr, "Curl unable to HTTP POST\n %s\n", 
//                 curl_easy_strerror(res));
//             }
// 				else if(res == CURLE_OK) {
// 				long response_code;
// 				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
// 				printf("\nHTTP RESPONSE CODE: %ld\n", response_code);
// 			}

// 		curl_easy_cleanup(curl);
// 	}
// }


// void DELETE(CURL *curl, char *postdata) {
// 	/* DELETE method requests that the origin server
// 	 delete the resource identified by the Request-URI.*/ 
	
// 	CURLcode res;
// 	curl = curl_easy_init();
// 	int httpStatus = 0;

// 	if(curl) {
// 		curl_easy_setopt(curl, CURLOPT_URL, URL);
// 		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
// 		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
// 		res = curl_easy_perform(curl);

//             if(res != CURLE_OK) {
//                 fprintf(stderr, "Curl unable to HTTP DELETE\n %s\n", 
//                 curl_easy_strerror(res));
//             }
// 				else if(res == CURLE_OK) {
// 				long response_code;
// 				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
// 				printf("\nHTTP RESPONSE CODE: %ld\n", response_code);
// 			}
			

// 		curl_easy_cleanup(curl);
// 	}
// }