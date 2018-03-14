#include "iwlib.h"

static int stam_parse_usr_info(int skfd,
	   char *ifname,
	   char *args[],
	   int count)
{
	struct wireless_info info;
	int i, val, rc = 0;
	char *ptrstr = (char *)args[0];

	int (*func)(char *) = NULL;
	unsigned int addr = 0;

	for(i = 0; i < 8; ++i){
		if((ptrstr[i] >= '0') && (ptrstr[i] <= '9'))
			val = ptrstr[i] - '0';
		else if((ptrstr[i] >= 'a') && (ptrstr[i] <= 'f'))
			val = ptrstr[i] - 'a' + 10;
		else if((ptrstr[i] >= 'A') && (ptrstr[i] <= 'F'))
			val = ptrstr[i] - 'A' + 10;
		else
			return -1;

		addr = ((addr << 4) + val);
	}

	if((func = addr) != NULL){
		if(iw_get_basic_config(skfd, ifname, &(info.b)) == 0)
			rc = func(ifname);
	}
	
	return rc;
}



int stam_get_vap_dev(char *argv[], int count)
{
	int skfd;		/* generic raw socket desc.	*/
	int goterr = 0;

	/* Create a channel to the NET kernel. */
	if((skfd = iw_sockets_open()) < 0)
	{
		perror("socket");
		exit(-1);
	}

	iw_enum_devices(skfd, &stam_parse_usr_info, argv, count);

	/* Close the socket. */
	iw_sockets_close(skfd);

	return(goterr);
}

