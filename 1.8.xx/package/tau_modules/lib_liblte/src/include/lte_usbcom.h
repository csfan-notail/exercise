#ifndef __LTE_USBCOM_H__
#define __LTE_USBCOM_H__

extern unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);
extern int lte_file_unlink (const char* pcFileName);
extern int lte_normal_at_cmd (LTE_NORMAL_AT_T* pstAtCmd);
extern int lte_get_cell_register_status(char *pcRegisterCmd,char *pcNeedle, char *pcUsbcom);
extern int lte_normal_at_last_cmd(LTE_NORMAL_AT_T *pstAtCmd, char *pcNeedle);

#endif /* __LTE_USBCOM_H__ */

