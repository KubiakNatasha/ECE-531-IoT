#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <argp.h>
/* Information on argp.h used from tutorial : http://nongnu.askapache.com/argpbook/step-by-step-into-argp.pdf */


#define OK          0
#define INIT_ERR    1
#define REQ_ERR     2
#define default		0
/*DEFAULT URL*/
#define URL         "https://example.com" 



/*********PROTOTYPE**************/
void HELP();
void GET();
void PUT(char *url, CURL *curl, CURLcode res ,char *postdata);
void POST(char *url, CURL *curl, CURLcode res, char *postdata);
void DELETE(char *url, CURL *curl, CURLcode res, char *postdata);
static int parse_opt (int key, char *arg, struct argp_state *state);
char newURL[100];

int main(int argc, char **argv) {
	
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();

	struct argp_option options[] =
	{
		{"url", 'u',0, 0, "Set URL"},
		{"put", 'p',0, 0, "Put"},
		{"post", 'o',0, 0, "Post"},
		{"get", 'g',0, 0, "Get"},
		{"delete", 'd',0, 0, "Delete"}, 
		{"help", 'h',0, 0, "Help"},
		{0}
	};

    /* Handle bad arguments*/
	/* If number if Arguments is less than 1*/
    if(argc <= 1) {
			printf("Empty Argument.\n");
			printf("Exiting...\n");
			return INIT_ERR;
			
		}
	/* If Arguments is greater than 6*/
	if (argc >= 6) {
		printf("Invalid or too many arguments.\n");
		printf("Number of Arguments:%d\n",argc);
		printf("Exiting...\n");
		return INIT_ERR;
	}

	
	
	struct argp argp = { options, parse_opt };
	return argp_parse (&argp, argc, argv, 0, 0, 0);
	return OK;
}



/***************************************/
/* curl_easy_setopt is used to tell libcurl how to behave.

/***************************************/

void GET() {

	CURL    *curl;
	CURLcode res;
	curl = curl_easy_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, URL);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                fprintf(stderr, "Curl unable to http GET %s\n", 
                        curl_easy_strerror(res));       
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
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
		res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                fprintf(stderr, "Curl unable to HTTP DELETE %s\n", 
                curl_easy_strerror(res));
            }
		curl_easy_cleanup(curl);
	}
}

/**************************************/
/* PARSE OPT */
/*************************************/
static int 
parse_opt (int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
	struct arguments *a = state->input;

  switch (key)
  {
	/* URL */
    case 'u':
      printf("The Default URL is set to:	%s\n", URL);
	  printf("New URL has been set.\n");
	  printf("Entered Argument = %s\n", *arg);
	  strcpy(newURL, *arg);
	//   printf("New URL = %s\n", newURL);
	
      break;

	/*POST*/
    case 'o':
      printf("Post\n");
	  break;

	/*GET*/
    case 'g':
	  printf("Get\n");
      GET(); 
      break;

	/*PUT*/
    case 'p':
      printf("PUT\n");
	  break;

	/*DELETE*/
    case 'd':
      printf("Delete\n");
      break;

	case 'h':
	// case 'help':
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
		printf("hw -h --help\n");
		break;

	// default:
	// 	printf("Invalid or Unkown Argument.\n"); 
	// 	printf("For HELP enter: ./hw -h , or ./hw --help\n"); 
	// 	break;
  }
  return 0;
}

