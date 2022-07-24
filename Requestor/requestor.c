#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

#define OK          0
#define INIT_ERR    1
#define REQ_ERR     2

#define URL         "http://localhost:8000"

/*********PROTOTYPE**************/
void HELP();
void GET(char *url, CURL *curl, CURLcode res);
void PUT(char *url, CURL *curl, CURLcode res ,char *postdata);
void POST(char *url, CURL *curl, CURLcode res, char *postdata);
void DELETE(char *url, CURL *curl, CURLcode res, char *postdata)


int main() {
	
	CURL *curl;
	CURLcode res;

    /* Handle bad arguments*/
    if(argc <= 1) {
			printf("Empty Argument:\n");
			
		}

	
	
	return OK;
}


void HELP()
{
	printf("REQUESTOR HELP:\n");
    printf("-------------------------\n\n");
    printf("Options:\n");
	printf("HTTP REQUESTS:\n");
	printf("  -g  --get    GET      \n");
	printf("  -o  --post   POST     \n");
	printf("  -p  --put    PUT      \n");
	printf("  -d  --delete DELETE   \n");
    printf(" HTTP REQUESTS should be followed with a valid URL\n");
    printf(" -u   --url    URL \n");
    printf("-------------------------\n\n");
    printf("COMMAND LINE INPUT FORMAT\n");
    printf("hw [REQUEST] --url [URL]\n");
    printf("EXAMPLES\n");
    printf("hw --post --url http://localhost:8080 \n");
    printf("hw --get --url http://www.cnn.com\n");
    printf("hw --put --url http://localhost:8080\n");
    printf("hw --delete --url http://localhost:8080 78392\n");
    printf("-------------------------\n\n");
    printf("Exit definitions:\n");
    printf("0 OK\n");
    printf("1 INIT_ERR\n");
    printf("2 REQ_ERR\n");
    printf("-------------------------\n\n");
    printf("To display help again:");
    printf("hw -h --help");

}


/***************************************/
/* curl_easy_setopt is used to tell libcurl how to behave.

/***************************************/

void GET(char *url, CURL *curl, CURLcode res) {
	
	curl = curl_easy_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                fprintf(stderr, "Curl unable to http GET %s\n", 
                        curl_easy_strerror(res))        
            }
		curl_easy_cleanup(curl);
	}
}


void PUT(char *url, CURL *curl, CURLcode res, char *postdata) {
	
	curl = curl_easy_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
		res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                fprintf(stderr, "Curl unable to HTTP PUT %s\n", 
                curl_easy_strerror(res));
            }
		curl_easy_cleanup(curl);
	}
}


void POST(char *url, CURL *curl, CURLcode res, char *postdata) {

	curl = curl_easy_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postdata));
		res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                fprintf(stderr, "Curl unable to HTTP POST %s\n", 
                curl_easy_strerror(res));
            }
        
		curl_easy_cleanup(curl);
	}
}


void DELETE(char *url, CURL *curl, CURLcode res, char *postdata) {
	
	curl = curl_easy_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content);
		res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                fprintf(stderr, "Curl unable to HTTP DELETE %s\n", 
                curl_easy_strerror(res));
            }
		curl_easy_cleanup(curl);
	}
}