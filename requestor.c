#include <stdio.h>
#include <curl/curl.h>

#define OK          0
#define INIT_ERR    1
#define REQ_ERR     2

#define URL         "http://localhost:8000"

int main(void) {
  
    CURL    *curl;
    CURLcode res;

    curl = curl_easy_init();

    if(curl) {
        curl_easy_septopt(curl, CURLOPT_URL, URL);
        curl_easy_septopt(curl, CURLOPT_FOLLOWLCOATION, 1L);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            return REQ_ERR;
        }
        curl_easy_cleanup(curl);
    }
    else {
        return INIT_ERR;
    }

    return OK;

}
