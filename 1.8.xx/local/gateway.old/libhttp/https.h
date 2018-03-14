
#ifndef HTTPS__h
#define HTTPS__h

#define HTTPS_RESPONSE_NO      0
#define HTTPS_RESPONSE_STRING  1
#define HTTPS_RESPONSE_FILE    2

/* get ip */
char * get_ip(char *hostname);

/* https download */
int https_download(char          *host, 
					   unsigned short port, 
					   char			  *requestpath, 
					   char 		  *post_data,
					   char           *cert_file,
					   char           *key_file,
					   char 		  *out_file_name);
/* send https request */
int send_https_request(char			*host, 
							 unsigned short port, 
							 char           *requestpath, 
							 char 			*post_data,
					   		 char           *cert_file,
					   		 char           *key_file,
							 char 			*out_file_name,
							 int 			has_respone);

#endif


