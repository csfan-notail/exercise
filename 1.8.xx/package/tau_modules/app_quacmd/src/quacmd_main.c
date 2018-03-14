
#include <sys/un.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <pwd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "qua_include/zebra.h"
#include "qua_include/version.h"
#include "qua_include/getopt.h"
#include "qua_include/command.h"
#include "qua_include/memory.h"
#include "qua_include/vtysh.h"
#include "qua_include/vtysh_user.h"
#include "qua_include/privs.h"
#include "qua_include/ospfd.h"


char *progname;
char action[8];
int	 execute_flag = 0;
struct thread_master *master;

zebra_capabilities_t _caps_p [] = 
{
  ZCAP_NET_RAW,
  ZCAP_BIND,
  ZCAP_NET_ADMIN,
};
struct zebra_privs_t ospfd_privs =
{
#if defined(QUAGGA_USER) && defined(QUAGGA_GROUP)
  .user = QUAGGA_USER,
  .group = QUAGGA_GROUP,
#endif
#if defined(VTY_GROUP)
  .vty_group = VTY_GROUP,
#endif
  .caps_p = _caps_p,
  .cap_num_p = sizeof(_caps_p)/sizeof(_caps_p[0]),
  .cap_num_i = 0
};

static void usage (int status)
{
  if (status != 0)
    fprintf (stderr, "Try `%s --help' for more information.\n", progname);
  else
    printf ("Usage : %s [OPTION...]\n\n" \
	    "--zebra            quacmd zebra [add|del] [interface] [ipaddress] \n" \
	    "                   eg: quacmd zebra add br-lan 172.16.0.1/16 \n" \
	    "--ospfd            quacmd ospfd [add|del] [router-id] [network] [area] [interface] [cost]\n" \
	    "                   eg: quacmd ospfd del 172.16.0.1 172.16.0.1/16 0 br-lan 1 \n" \
	    "                   eg: quacmd ospfd add 172.16.0.1 172.16.0.1/16 0 br-lan 1 \n" \
	    , progname, ZEBRA_BUG_ADDRESS);

  exit (status);
}

struct option longopts[] = 
{
  { "help",                 no_argument,             NULL, 'h'},
  { 0 }
};

static void zebra_config(int argc, char *argv[])
{
	char int_type[32] = {0};
	char zebra_ip[64] = {0};
	
	if( argc < 5 ){
		printf("[%s|%s:%d] zebra argc error ! \n",__FILE__,__FUNCTION__,__LINE__);
		syslog(3,"quacmd:[%d] zebra argc error !\n",__LINE__);
		return;
	}
	
	vtysh_execute ("enable");
	vtysh_execute ("config t");
	
	snprintf(int_type,sizeof(int_type),"int %s",argv[3]);
	vtysh_execute (int_type);
	printf("[%s|%s:%d] int_type=%s\n",__FILE__,__FUNCTION__,__LINE__,int_type);
	snprintf(zebra_ip,sizeof(zebra_ip),"%s ip address %s",action,argv[4]);
	vtysh_execute (zebra_ip);
	printf("[%s|%s:%d] zebra_ip=%s\n",__FILE__,__FUNCTION__,__LINE__,zebra_ip);
	vtysh_execute ("exit");
	vtysh_execute ("exit");
	vtysh_execute ("write");
	vtysh_execute ("quit");
}
static void ospfd_config(int argc, char *argv[])
{
	char ospf_rid[32] = {0};
	char ospf_net[64] = {0};
	char ospf_inter[32] = {0};
	char ospf_cost[32] = {0};
	
	if( argc < 7 ){
		printf("[%s|%s:%d] ospfd argc error ! \n",__FILE__,__FUNCTION__,__LINE__);
		syslog(3,"quacmd:[%d] ospfd argc error !\n",__LINE__);
		return;
	}		
	
	vtysh_execute ("enable");
	vtysh_execute ("config t");
	
	vtysh_execute ("router ospf");
	vtysh_execute ("timers throttle spf 5 100 500");
	printf("[%s|%s:%d] router ospf \n",__FILE__,__FUNCTION__,__LINE__);
	snprintf(ospf_rid,sizeof(ospf_rid),"ospf router-id %s",argv[3]);
	vtysh_execute (ospf_rid);
	printf("[%s|%s:%d] ospf_rid=%s\n",__FILE__,__FUNCTION__,__LINE__,ospf_rid);
	//vtysh_execute ("ip ospf cost 1");
	//printf("[%s|%s:%d] ip ospf cost 1 \n",__FILE__,__FUNCTION__,__LINE__);
	snprintf(ospf_net,sizeof(ospf_net),"%s network %s area %s",action,argv[4],argv[5]);
	vtysh_execute (ospf_net);
	printf("[%s|%s:%d] ospf_net=%s\n",__FILE__,__FUNCTION__,__LINE__,ospf_net);	
	
	snprintf(ospf_inter,sizeof(ospf_inter),"int %s",argv[6]);
	vtysh_execute (ospf_inter);
	printf("[%s|%s:%d] %s \n",__FILE__,__FUNCTION__,__LINE__,ospf_inter);
	snprintf(ospf_cost,sizeof(ospf_cost),"%s ip ospf cost %s",action,argv[7]);
	vtysh_execute (ospf_cost);
	printf("[%s|%s:%d] %s \n",__FILE__,__FUNCTION__,__LINE__,ospf_cost);

	vtysh_execute ("exit");
	vtysh_execute ("exit");
	vtysh_execute ("write");
	vtysh_execute ("quit");	
}

int main (int argc, char **argv, char **env)
{
	char *p;
	int opt;
	const char *daemon_name = NULL;
	struct cmd_rec {
	  const char *line;
	  struct cmd_rec *next;
	} *cmd = NULL;
	struct cmd_rec *tail = NULL;
	int no_error = 0;
	char *homedir = NULL;
	
	progname = ((p = strrchr (argv[0], '/')) ? ++p : argv[0]);

	while (1) 
	{
		opt = getopt_long (argc, argv, "be:c:d:nEhC", longopts, 0);
		if (opt == EOF)
			break;
		switch (opt) 
		{
			case 'h':
				usage (0);
				break;
			default:
				usage (1);
				break;
		}
    }

	setlinebuf(stdout);

	vtysh_init_vty ();
	vtysh_init_cmd ();
	vtysh_user_init ();
	vtysh_config_init ();
	vty_init_vtysh ();
	vtysh_auth ();

	if (vtysh_connect_all (daemon_name) <= 0)
	{
		fprintf(stderr, "Exiting: failed to connect to any daemons.\n");
		exit(1);
	}

	if (cmd)
    {
		while (cmd != NULL)
		{
			int ret;
			char *eol;
			while ((eol = strchr(cmd->line, '\n')) != NULL)
			{
	      		*eol = '\0';
	      		add_history (cmd->line);
				ret = vtysh_execute_no_pager(cmd->line);
	      		if (!no_error && !(ret == CMD_SUCCESS || ret == CMD_SUCCESS_DAEMON ||ret == CMD_WARNING))
					exit(1);
				cmd->line = eol+1;
			}

			add_history (cmd->line);
	  		ret = vtysh_execute_no_pager(cmd->line);
	  		if (!no_error &&! (ret == CMD_SUCCESS ||ret == CMD_SUCCESS_DAEMON ||ret == CMD_WARNING))
	    		exit(1);

	    	struct cmd_rec *cr;
	    	cr = cmd;
	    	cmd = cmd->next;
	    	XFREE(0, cr);
        }
    }

	vtysh_pager_init ();
  	vtysh_readline_init ();
  
    if( argc < 3 ){
		printf("[%s|%s:%d] argc error ! help: quacmd -h \n\n",__FILE__,__FUNCTION__,__LINE__);
		exit (0);
	}
	if( strcmp("add",argv[2]) && strcmp("del",argv[2]) ){
		printf("[%s|%s:%d] argv add/del error ! \n\n",__FILE__,__FUNCTION__,__LINE__);
		exit (0);
	}
	syslog(3,"[%d] quacmd %s %s %s %s %s \n", \
			argc,argv[1],argv[2],argv[3],argv[4],argv[5]);	
	printf("[%d] quacmd %s %s %s %s %s \n", \
			argc,argv[1],argv[2],argv[3],argv[4],argv[5]);			
	if( !strcmp("del",argv[2]) )
		snprintf(action,sizeof(action),"%s","no");
	if( !strcmp("zebra",argv[1]) )
		zebra_config(argc, argv);
	else if( !strcmp("ospfd",argv[1]) )
		ospfd_config(argc, argv);
	else
	{
		printf("[%s|%s:%d] command not found ! \n",__FILE__,__FUNCTION__,__LINE__);
	}
  	printf ("\n");
  	exit (0);
}
