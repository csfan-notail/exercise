
#include "pal_common.h"

int creat_socket(void)
{
    int skdf;

    skdf = -1;
    skdf = socket(AF_INET, SOCK_DGRAM, 0);
    return skdf;
}



void close_socket( int skdf )
{
    if(-1 == skdf)
    {
        gw_log(APP_LOG_ERR, "the skdf = -1");
        return;
    }
    close(skdf);
    return;
}
