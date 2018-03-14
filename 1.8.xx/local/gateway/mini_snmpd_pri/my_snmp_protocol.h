/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* ��    ��: 
* �޸���ʷ: 
* 2012-9-10     �뾲              �½�
******************************************************************************/

/******************************* �궨�� ********************************/
#ifndef __MINI_SNMPD_PROTOCOL__
#define __MINI_SNMPD_PROTOCOL__

int encode_snmp_element_integer(value_t *value, int integer_value);
int encode_snmp_element_string(value_t *value, const char *string_value);
int encode_snmp_element_octetstring(value_t *value, const char *string_value, int value_len);
int encode_snmp_element_ip(value_t *value, const char *ip_value);
int encode_snmp_element_oid(value_t *value, const oid_t *oid_value);
int encode_snmp_element_unsigned(value_t *value, int type, unsigned int ticks_value);
int encode_snmp_element_unsigned_longlong(value_t *value, int type, unsigned long long ticks_value);

#endif
