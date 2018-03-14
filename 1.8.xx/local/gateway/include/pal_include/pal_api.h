#ifndef __PAL_API_H__
#define __PAL_API_H__


/* api define for all modules */

int get_customer_prosn(char *sn);
int get_oem_company(char *company);
int device_cpu_get(int *cpu);
int device_mem_get(int *mem);

#endif
