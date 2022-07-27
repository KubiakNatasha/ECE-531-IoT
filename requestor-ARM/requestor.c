/*Natasha Kubiak		*/
/*ECE 531-SUMMER 2022	*/
/*Information on argp.h used from tutorial : http://nongnu.askapache.com/argpbook/step-by-step-into-argp.pdf */
/*HTTP Requests using libcurl:  https://curl.se/libcurl/c/http-post.html */

/* command */
/* ./hw --get --url http://localhost:8080 */

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <argp.h>



#define OK          0
#define INIT_ERR    1
#define REQ_ERR     2
#define URL         "https://example.com" 
#define default		"default"



/*********PROTOTYPE**************/
void HELP();
void GET(CURL *curl, char *postdata);
void PUT(CURL *curl, char *postdata);
void POST(CURL *curl, char *postdata);
void DELETE(CURL *curl, char *postdata);
static int parse_opt (int key, char *arg, struct argp_state *state);
char newURL[50];

/*********MAIN*******************/

int main(int argc, char **argv) {
	
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();

	struct argp_option options[] =
	{
		{"url", 'u', "URL", 0, "Set URL"},
		{"put", 'p',"default", 0, "Put"},
		{"post", 'o',"default", 0, "Post"},
		{"get", 'g',0, 0, "Get"},
		{"delete", 'd',"default", 0, "Delete"}, 
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




void GET(CURL *curl, char *postdata) {
	/*GET method means retrieve whatever information 
	(in the form of an entity) is identified by the Request-URI.*/

	
	CURLcode res;
	curl = curl_easy_init();
	int httpStatus = 0;

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, URL);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                fprintf(stderr, "Curl unable to http GET %s\n", 
                        curl_easy_strerror(res));       
            }
				else if(res == CURLE_OK) {
				long response_code;
				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
				printf("\nHTTP RESPONSE CODE: %ld\n", response_code);
			}
		curl_easy_cleanup(curl);
	}
}


void PUT(CURL *curl, char *postdata) {
	/* UT method requests that the enclosed entity
	 be stored under the supplied Request-URI.  */
	const char *postdata = "any_data";
	CURLcode res;
	curl = curl_easy_init();
	int httpStatus = 0;
	

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, URL);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata); 
		res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                fprintf(stderr, "Curl unable to HTTP PUT %s\n", 
                curl_easy_strerror(res));
            }
				else if(res == CURLE_OK) {
				long response_code;
				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
				printf("\nHTTP RESPONSE CODE: %ld\n", response_code);
			}

		curl_easy_cleanup(curl);
	}
}


void POST (CURL *curl, char *postdata) {
	/* POST method is used to request that the origin server
	 accept the entity enclosed in the request as a new 
	 subordinate of the resource identified by the Request-URI
	  in the Request-Line.*/

	CURLcode res;
	curl = curl_easy_init();
	int httpStatus = 0;


	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, URL);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postdata));
		res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                fprintf(stderr, "Curl unable to HTTP POST %s\n", 
                curl_easy_strerror(res));
            }
				else if(res == CURLE_OK) {
				long response_code;
				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
				printf("\nHTTP RESPONSE CODE: %ld\n", response_code);
			}

		curl_easy_cleanup(curl);
	}
}


void DELETE(CURL *curl, char *postdata) {
	/* DELETE method requests that the origin server
	 delete the resource identified by the Request-URI.*/ 
	
	CURLcode res;
	curl = curl_easy_init();
	int httpStatus = 0;

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, URL);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, *postdata);
		res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                fprintf(stderr, "Curl unable to HTTP DELETE %s\n", 
                curl_easy_strerror(res));
            }
				else if(res == CURLE_OK) {
				long response_code;
				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
				printf("\nHTTP RESPONSE CODE: %ld\n", response_code);
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
	CURL *curl;
	

  switch (key)
  {
	/* URL */
    case 'u':
	  strcpy(newURL, arg);
      break;

	/*POST*/
    case 'o':
      printf("POST\n");
	  POST(curl, arg);
	  break;

	/*GET*/
    case 'g':
	  printf("GET\n");
      GET(curl, arg); 
      break;

	/*PUT*/
    case 'p':
      printf("PUT\n");
	  PUT(curl, arg);
	  break;

	/*DELETE*/
    case 'd':
      printf("DELETE\n");
      DELETE(curl, arg);
      break;

	/*HELP*/
	case 'h':
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
		printf("To display help again,	./hw -h\n");
		break;

	// default:
	// 	printf("Invalid or Unkown Argument.\n"); 
  }
  return 0;
}

