#include <stddef.h>
#include <curl/curl.h>
#include <unistd.h>

#include "system.h"
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int Http_Download(const char * url, const char * filename, CURL **session, int compare)
{
  // Source : https://curl.se/libcurl/c/url2file.html
  if(access( filename, F_OK ) == 0)
    return 1;
  FILE *pagefile;
	int http_code = 0;

  curl_global_init(CURL_GLOBAL_ALL);
	curl_easy_setopt(session, CURLOPT_URL, url);
  curl_easy_setopt(session, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(session, CURLOPT_FOLLOWLOCATION, 1);
 
  _mkdir(filename);
  pagefile = fopen(filename, "wb");
  if(pagefile) {
    curl_easy_setopt(session, CURLOPT_WRITEDATA, pagefile);
    curl_easy_perform(session);
    fclose(pagefile);
  }
	curl_easy_getinfo (session, CURLINFO_RESPONSE_CODE, &http_code); 
  
  /* curl_easy_cleanup(session); */
  curl_global_cleanup();
	return http_code;
}
// TODO : POST & GET request
