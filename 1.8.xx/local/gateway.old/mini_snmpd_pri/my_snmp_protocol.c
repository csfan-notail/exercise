/* -----------------------------------------------------------------------------
 * Copyright (C) 2012 feng jing <fengjing@gbcom.com.cn>
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See COPYING for GPL licensing information.
 */



#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "mini_snmpd.h"

#include "my_snmp_def.h"
#include "my_snmp_data.h"
//#include "my_snmp_oid.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"

/* BEGIN: Added by zoucaihong, 2012/11/16 */
extern OPEN_MIB_COMMUNITY_CONFIG_T g_stSnmpV2Config;
/* END: Added by zoucaihong, 2012/11/16 */


/* -----------------------------------------------------------------------------
 * Helper functions for encoding values
 */

int encode_snmp_element_integer(value_t *value, int integer_value)
{
	unsigned char *buffer;
	int length;

	buffer = value->data.buffer;
	if (integer_value < -8388608 || integer_value > 8388607) {
		length = 4;
	} else if (integer_value < -32768 || integer_value > 32767) {
		length = 3;
	} else if (integer_value < -128 || integer_value > 127) {
		length = 2;
	} else {
		length = 1;
	}
	*buffer++ = BER_TYPE_INTEGER;
	*buffer++ = length;
	while (length--) {
		*buffer++ = ((unsigned int)integer_value >> (8 * length)) & 0xFF;
	}
	value->data.encoded_length = buffer - value->data.buffer;
	return 0;
}

int encode_snmp_element_string(value_t *value, const char *string_value)
{
	unsigned char *buffer;
	int length;

	buffer = value->data.buffer;
	length = strlen(string_value);
	*buffer++ = BER_TYPE_OCTET_STRING;
	if (length > 65535) {
		lprintf(LOG_ERR, "could not encode '%s': string overflow\n", string_value);
		return -1;
	} else if (length > 255) {
		*buffer++ = 0x82;
		*buffer++ = (length >> 8) & 0xFF;
		*buffer++ = length & 0xFF;
	} else if (length > 127) {
		*buffer++ = 0x81;
		*buffer++ = length & 0xFF;
	} else {
		*buffer++ = length & 0x7F;
	}
	while (*string_value) {
		*buffer++ = (unsigned char)(*string_value++);
	}
	value->data.encoded_length = buffer - value->data.buffer;
	return 0;
}

int encode_snmp_element_ip(value_t *value, const char *ip_value)
{
	unsigned char *buffer;
	int length;
	int i=0;

	buffer = value->data.buffer;
	length = 4;
	*buffer++ = BER_TYPE_IP;
	if (length > 65535) {
		return -1;
	} else if (length > 255) {
		*buffer++ = 0x82;
		*buffer++ = (length >> 8) & 0xFF;
		*buffer++ = length & 0xFF;
	} else if (length > 127) {
		*buffer++ = 0x81;
		*buffer++ = length & 0xFF;
	} else {
		*buffer++ = length & 0x7F;
	}
	while (i<4) {
		*buffer++ = (unsigned char)(*ip_value++);
		i++;
	}
	value->data.encoded_length = buffer - value->data.buffer;
	return 0;
}

int encode_snmp_element_octetstring(value_t *value, const char *string_value, int value_len)
{
	unsigned char *buffer;
	int length;
	int i=0;

	buffer = value->data.buffer;
	length = value_len;
	*buffer++ = BER_TYPE_OCTET_STRING;
	if (length > 65535) {
		lprintf(LOG_ERR, "could not encode '%s': string overflow\n", string_value);
		return -1;
	} else if (length > 255) {
		*buffer++ = 0x82;
		*buffer++ = (length >> 8) & 0xFF;
		*buffer++ = length & 0xFF;
	} else if (length > 127) {
		*buffer++ = 0x81;
		*buffer++ = length & 0xFF;
	} else {
		*buffer++ = length & 0x7F;
	}
	while (i<value_len) {
		*buffer++ = (unsigned char)(*string_value++);
		i++;
	}
	value->data.encoded_length = buffer - value->data.buffer;
	return 0;
}

int encode_snmp_element_oid(value_t *value, const oid_t *oid_value)
{
	unsigned char *buffer;
	int length;
	int i;

	buffer = value->data.buffer;
	length = 1;
	for (i = 2; i < oid_value->subid_list_length; i++) {
		if (oid_value->subid_list[i] >= (1 << 28)) {
			length += 5;
		} else if (oid_value->subid_list[i] >= (1 << 21)) {
			length += 4;
		} else if (oid_value->subid_list[i] >= (1 << 14)) {
			length += 3;
		} else if (oid_value->subid_list[i] >= (1 << 7)) {
			length += 2;
		} else {
			length += 1;
		}
	}
	*buffer++ = BER_TYPE_OID;
	if (length > 0xFFFF) {
		lprintf(LOG_ERR, "could not encode '%s': oid overflow\n", oid_ntoa(oid_value));
		return -1;
	} else if (length > 0xFF) {
		*buffer++ = 0x82;
		*buffer++ = (length >> 8) & 0xFF;
		*buffer++ = length & 0xFF;
	} else if (length > 0x7F) {
		*buffer++ = 0x81;
		*buffer++ = length & 0xFF;
	} else {
		*buffer++ = length & 0x7F;
	}
	*buffer++ = oid_value->subid_list[0] * 40 + oid_value->subid_list[1];
	for (i = 2; i < oid_value->subid_list_length; i++) {
		if (oid_value->subid_list[i] >= (1 << 28)) {
			length = 5;
		} else if (oid_value->subid_list[i] >= (1 << 21)) {
			length = 4;
		} else if (oid_value->subid_list[i] >= (1 << 14)) {
			length = 3;
		} else if (oid_value->subid_list[i] >= (1 << 7)) {
			length = 2;
		} else {
			length = 1;
		}
		while (length--) {
			if (length) {
				*buffer++ = ((oid_value->subid_list[i] >> (7 * length)) & 0x7F) | 0x80;
			} else {
				*buffer++ = (oid_value->subid_list[i] >> (7 * length)) & 0x7F;
			}
		}
	}
	value->data.encoded_length = buffer - value->data.buffer;
	return 0;
}

int encode_snmp_element_unsigned(value_t *value, int type, unsigned int ticks_value)
{
	unsigned char *buffer;
	int length;

	buffer = value->data.buffer;
    #ifdef MGSOFT
    if ( ticks_value & 0x80000000 )
    {
        ticks_value &=~0x80000000;//首位1去掉
    }
    if ( ticks_value > 0x007FFFFF ) { 
        length = 4;
    }else if ( ticks_value > 0x00007FFF ) {
        length = 3;
	} else if (ticks_value > 0x0000007F) {
		length = 2;
	} else {
		length = 1;
	}
    #else
	if (ticks_value & 0xFF000000) {
		length = 4;
	} else if (ticks_value & 0x00FF0000) {
		length = 3;
	} else if (ticks_value & 0x0000FF00) {
		length = 2;
	} else {
		length = 1;
	}
    #endif
	*buffer++ = type;
	*buffer++ = length;
	while (length--) {
		*buffer++ = (ticks_value >> (8 * length)) & 0xFF;
	}
	value->data.encoded_length = buffer - value->data.buffer;
	return 0;
}


int encode_snmp_element_unsigned_longlong(value_t *value, int type, unsigned long long ticks_value)
{
	unsigned char *buffer;
	int length;

	buffer = value->data.buffer;
    /* BEGIN: Modified by zoucaihong, 2013/1/24 */
    #ifdef MGSOFT
    if ( ticks_value & 0x8000000000000000LL )
    {
        ticks_value &=~0x8000000000000000LL;//首位1去掉
    }
    if ( ticks_value > 0x007FFFFFFFFFFFFFLL ) { 
        length = 8;
    }else if ( ticks_value > 0x00007FFFFFFFFFFFLL ) {
        length = 7;
	} else if (ticks_value > 0x0000007FFFFFFFFFLL) {
		length = 6;
	} else if (ticks_value > 0x000000007FFFFFFFLL) {
		length = 5;
	} else if (ticks_value > 0x00000000007FFFFFLL) {
		length = 4;
	} else if (ticks_value > 0x0000000000007FFFLL) {
		length = 3;
	} else if (ticks_value > 0x000000000000007FLL) {
		length = 2;
	} else {
		length = 1;
	}
    #else
	if (ticks_value & 0xFF00000000000000LL) {
		length = 8;
	} else if (ticks_value & 0x00FF000000000000LL) {
		length = 7;
	} else if (ticks_value & 0x0000FF0000000000LL) {
		length = 6;
	} else if (ticks_value & 0x000000FF00000000LL) {
		length = 5;
	} else if (ticks_value & 0x00000000FF000000LL) {
		length = 4;
	} else if (ticks_value & 0x0000000000FF0000LL) {
		length = 3;
	} else if (ticks_value & 0x000000000000FF00LL) {
		length = 2;
	} else {
		length = 1;
	}
    #endif
    /* END: Modified by zoucaihong, 2013/1/24 */
	*buffer++ = type;
	*buffer++ = length;
	while (length--) {
		*buffer++ = (ticks_value >> (8 * length)) & 0xFF;
	}
	value->data.encoded_length = buffer - value->data.buffer;
    agent_printf(LOG_INFO,"encoded_length:%d",value->data.encoded_length);
	return 0;
}


/* -----------------------------------------------------------------------------
 * Helper functions for decoding
 */

static int decode_snmp_element_type_length(const unsigned char *packet,
	size_t size, size_t *pos, int *type, int *length)
{
	int length_of_length;

	/* Fetch the ASN.1 element type (only subset of universal tags supported) */
	if (*pos < size) {
		switch (packet[*pos]) {
			case BER_TYPE_BOOLEAN:
			case BER_TYPE_INTEGER:
			case BER_TYPE_BIT_STRING:
			case BER_TYPE_OCTET_STRING:
			case BER_TYPE_NULL:
			case BER_TYPE_OID:
			case BER_TYPE_SEQUENCE:
			case BER_TYPE_IP:
			case BER_TYPE_COUNTER:
			case BER_TYPE_GAUGE:
			case BER_TYPE_TIME_TICKS:
			case BER_TYPE_COUNTER64:
			case BER_TYPE_NO_SUCH_OBJECT:
			case BER_TYPE_NO_SUCH_INSTANCE:
			case BER_TYPE_END_OF_MIB_VIEW:
			case BER_TYPE_SNMP_GET:
			case BER_TYPE_SNMP_GETNEXT:
			case BER_TYPE_SNMP_RESPONSE:
			case BER_TYPE_SNMP_SET:
			case BER_TYPE_SNMP_GETBULK:
			case BER_TYPE_SNMP_INFORM:
			case BER_TYPE_SNMP_TRAP:
				*type = packet[*pos];
				*pos = *pos + 1;
				break;
			default:
				lprintf(LOG_DEBUG, "unsupported element type %02X\n", packet[*pos]);
				errno = EINVAL;
				return -1;
		}
	} else {
		lprintf(LOG_DEBUG, "underflow for element type\n");
		errno = EINVAL;
		return -1;
	}

	/* Fetch the ASN.1 element length (only lengths up to 16 bit supported) */
	if (*pos < size) {
		if (!(packet[*pos] & 0x80)) {
			*length = packet[*pos];
			*pos = *pos + 1;
		} else {
			length_of_length = packet[*pos] & 0x7F;
			if (length_of_length > 2) {
				lprintf(LOG_DEBUG, "overflow for element length\n");
				errno = EINVAL;
				return -1;
			}
			*pos = *pos + 1;
			*length = 0;
			while (length_of_length--) {
				if (*pos < size) {
					*length = (*length << 8) + packet[*pos];
					*pos = *pos + 1;
				} else {
					lprintf(LOG_DEBUG, "underflow for element length\n");
					errno = EINVAL;
					return -1;
				}
			}
		}
	} else {
		lprintf(LOG_DEBUG, "underflow for element length\n");
		errno = EINVAL;
		return -1;
	}

	return 0;
}

static int decode_snmp_element_value_integer(const unsigned char *packet,
	size_t size, size_t *pos, int length, int *value)
{
	unsigned int tmp_value;

	/* Fetch the value as unsigned integer (copy sign bit into all bytes first) */
	if (*pos < (size - length + 1)) {
		memset(&tmp_value, (packet[*pos] & 0x80) ? 0xFF : 0x00, sizeof (tmp_value));
		while (length--) {
			tmp_value = (tmp_value << 8) | packet[*pos];
			*pos = *pos + 1;
		}
		*(int *)value = tmp_value;
	} else {
		lprintf(LOG_DEBUG, "underflow for integer\n");
		errno = EINVAL;
		return -1;
	}

	return 0;
}

static int decode_snmp_element_value_unsigned(const unsigned char *packet,
	size_t size, size_t *pos, int length, unsigned int *value)
{
	/* Fetch the value as unsigned integer (copy sign bit into all bytes first) */
	if (*pos < (size - length + 1)) {
		*value = 0;
		while (length--) {
			*value = (*value << 8) | packet[*pos];
			*pos = *pos + 1;
		}
	} else {
		lprintf(LOG_DEBUG, "underflow for unsigned\n");
		errno = EINVAL;
		return -1;
	}

	return 0;
}

static int decode_snmp_element_value_string(const unsigned char *packet,
	size_t size, size_t *pos, int length, char *value, size_t value_size)
{
	/* Fetch the value as C string (user must have made sure the length is ok) */
	if (*pos < (size - length + 1)) {
		snprintf(value, value_size, "%.*s", length, &packet[*pos]);
		*pos = *pos + length;
	} else {
		lprintf(LOG_DEBUG, "underflow for string\n");
		errno = EINVAL;
		return -1;
	}

	return 0;
}

static int decode_snmp_element_value_oid(const unsigned char *packet,
	size_t size, size_t *pos, int length, oid_t *value)
{
	/* Fetch the value as C string (user must have made sure the length is ok) */
	if (*pos < (size - length + 1)) {
		value->encoded_length = length;
		if (length > 0xFFFF) {
			lprintf(LOG_ERR, "could not decode: internal error\n");
			return -1;
		} else if (length > 0xFF) {
			value->encoded_length += 4;
		} else if (length > 0x7F) {
			value->encoded_length += 3;
		} else {
			value->encoded_length += 2;
		}
		value->subid_list_length = 0;
		if (length) {
			if (!(packet[*pos] & 0x80)) {
				value->subid_list[value->subid_list_length++] = packet[*pos] / 40;
				value->subid_list[value->subid_list_length++] = packet[*pos] % 40;
				*pos = *pos + 1;
			} else {
				lprintf(LOG_DEBUG, "unsupported OID startbyte %02X\n", packet[*pos]);
				errno = EINVAL;
				return -1;
			}
			length--;
		} else {
			lprintf(LOG_DEBUG, "underflow for OID startbyte\n");
			errno = EINVAL;
			return -1;
		}
		while (length) {
			if (value->subid_list_length < MAX_NR_SUBIDS) {
				value->subid_list[value->subid_list_length] = 0;
			} else {
				lprintf(LOG_DEBUG, "overflow for OID byte\n");
				errno = EFAULT;
				return -1;
			}
			while (length--) {
				value->subid_list[value->subid_list_length]
					= (value->subid_list[value->subid_list_length] << 7) + (packet[*pos] & 0x7F);
				if (packet[*pos] & 0x80) {
					if (length == 0) {
						lprintf(LOG_DEBUG, "underflow for OID byte\n");
						errno = EINVAL;
						return -1;
					}
					*pos = *pos + 1;
				} else {
					*pos = *pos + 1;
					break;
				}
			}
			value->subid_list_length++;
		}
	} else {
		lprintf(LOG_DEBUG, "underflow for oid\n");
		errno = EINVAL;
		return -1;
	}

	return 0;
}

static int decode_snmp_element_value_void(const unsigned char *packet,
	size_t size, size_t *pos, int type, int length, data_t *data)
{
	/* Fetch the value as pointer (user must make sure not to overwrite packet) */
	if (*pos < (size - length + 1)) {
		/* feng jing add: */
		data->buffer = malloc(length+1);
		data->buffer[length] = 0;
		memcpy(data->buffer, &packet[*pos], length);
		/* temperary save */
		data->max_length = type;
		data->encoded_length = length;
		*pos = *pos + length;
	} else {
		lprintf(LOG_DEBUG, "underflow for ptr\n");
		errno = EINVAL;
		return -1;
	}

	return 0;
}

static int decode_snmp_request(request_t *request, client_t *client)
{
	size_t pos = 0;
	int length;
	int type;

	/* The SNMP message is enclosed in a sequence */
	if (decode_snmp_element_type_length(client->packet, client->size, &pos,
		&type, &length) == -1) {
		return -1;
	} else if (type != BER_TYPE_SEQUENCE || length != (client->size - pos)) {
		lprintf(LOG_DEBUG, "unexpected SNMP header type %02X length %d\n",
			type, length);
		errno = EINVAL;
		return -1;
	}

	/* The first element of the sequence is the version */
	if (decode_snmp_element_type_length(client->packet, client->size, &pos,
		&type, &length) == -1) {
		return -1;
	} else if (type != BER_TYPE_INTEGER || length != 1) {
		lprintf(LOG_DEBUG, "unexpected SNMP version type %02X length %d\n",
			type, length);
		errno = EINVAL;
		return -1;
	} else if (decode_snmp_element_value_integer(client->packet, client->size,
		&pos, length, &request->version) == -1) {
		return -1;
	} else if (request->version != SNMP_VERSION_1 && request->version != SNMP_VERSION_2C) {
		lprintf(LOG_DEBUG, "unsupported SNMP version %d\n", request->version);
		errno = EINVAL;
		return -1;
	}

	/* The second element of the sequence is the community string */
	if (decode_snmp_element_type_length(client->packet, client->size, &pos,
		&type, &length) == -1) {
		return -1;
	} else if (type != BER_TYPE_OCTET_STRING || length >= sizeof (request->community)) {
		lprintf(LOG_DEBUG, "unexpected SNMP community type %02X length %d\n",
			type, length);
		errno = EINVAL;
		return -1;
	} else if (decode_snmp_element_value_string(client->packet, client->size,
		&pos, length, request->community, sizeof (request->community)) == -1) {
		return -1;
	} else if (strlen(request->community) < 1) {
		lprintf(LOG_DEBUG, "unsupported SNMP community '%s'\n", request->community);
		errno = EINVAL;
		return -1;
	}

	/* The third element of the sequence is the SNMP request */
	if (decode_snmp_element_type_length(client->packet, client->size, &pos,
		&type, &length) == -1) {
		return -1;
	} else if (length != (client->size - pos)) {
		lprintf(LOG_DEBUG, "unexpected SNMP request type type %02X length %d\n",
			type, length);
		errno = EINVAL;
		return -1;
	}
	request->type = type;

	/* The first element of the SNMP request is the request ID */
	if (decode_snmp_element_type_length(client->packet, client->size, &pos,
		&type, &length) == -1) {
		return -1;
	} else if (type != BER_TYPE_INTEGER || length < 1) {
		lprintf(LOG_DEBUG, "unexpected SNMP request id type %02X length %d\n",
			type, length);
		errno = EINVAL;
		return -1;
	} else if (decode_snmp_element_value_integer(client->packet, client->size,
		&pos, length, &request->id) == -1) {
		return -1;
	}

	/* The second element of the SNMP request is the error state / non repeaters */
	if (decode_snmp_element_type_length(client->packet, client->size, &pos,
		&type, &length) == -1) {
		return -1;
	} else if (type != BER_TYPE_INTEGER || length < 1) {
		lprintf(LOG_DEBUG, "unexpected SNMP error state type %02X length %d\n",
			type, length);
		errno = EINVAL;
		return -1;
	} else if (decode_snmp_element_value_integer(client->packet, client->size,
		&pos, length, &request->non_repeaters) == -1) {
		return -1;
	}

	/* The third element of the SNMP request is the error index / max repetitions */
	if (decode_snmp_element_type_length(client->packet, client->size, &pos,
		&type, &length) == -1) {
		return -1;
	} else if (type != BER_TYPE_INTEGER || length < 1) {
		lprintf(LOG_DEBUG, "unexpected SNMP error index type %02X length %d\n",
			type, length);
		errno = EINVAL;
		return -1;
	} else if (decode_snmp_element_value_integer(client->packet, client->size,
		&pos, length, &request->max_repetitions) == -1) {
		return -1;
	}

	/* The fourth element of the SNMP request are the variable bindings */
	if (decode_snmp_element_type_length(client->packet, client->size, &pos,
		&type, &length) == -1) {
		return -1;
	} else if (type != BER_TYPE_SEQUENCE || length != (client->size - pos)) {
		lprintf(LOG_DEBUG, "unexpected SNMP varbindings type %02X length %d\n",
			type, length);
		errno = EINVAL;
		return -1;
	}

	/* Loop through the variable bindings */
	request->oid_list_length = 0;
	while (pos < client->size) {
		/* If there is not enough room in the OID list, bail out now */
		if (request->oid_list_length >= MAX_NR_OIDS) {
			lprintf(LOG_DEBUG, "overflow for OID list\n");
			errno = EFAULT;
			return -1;
		}
		/* Each variable binding is a sequence describing the variable */
		if (decode_snmp_element_type_length(client->packet, client->size, &pos,
			&type, &length) == -1) {
			return -1;
		} else if (type != BER_TYPE_SEQUENCE || length < 1) {
			lprintf(LOG_DEBUG, "unexpected SNMP varbinding type %02X length %d\n",
				type, length);
			errno = EINVAL;
			return -1;
		}
		/* The first element of the variable binding is the OID */
		if (decode_snmp_element_type_length(client->packet, client->size, &pos,
			&type, &length) == -1) {
			return -1;
		} else if (type != BER_TYPE_OID || length < 1) {
			lprintf(LOG_DEBUG, "unexpected SNMP varbinding OID type %02X length %d\n",
				type, length);
			errno = EINVAL;
			return -1;
		} else if (decode_snmp_element_value_oid(client->packet, client->size,
			&pos, length, &request->oid_list[request->oid_list_length]) == -1) {
			return -1;
		}
		/* The second element of the variable binding is the new type and value */
		if (decode_snmp_element_type_length(client->packet, client->size, &pos,
			&type, &length) == -1) {
			return -1;
		} else if ((type == BER_TYPE_NULL && length != 0)
		    /* BEGIN: Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
			//|| (type != BER_TYPE_NULL && length == 0)) {
			) {
		    /* END:   Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
			lprintf(LOG_DEBUG, "unexpected SNMP varbinding value type %02X length %d\n",
				type, length);
			errno = EINVAL;
			return -1;
		} else if (decode_snmp_element_value_void(client->packet, client->size,
			&pos, type, length, &request->data_list[request->oid_list_length]) == -1) {
			return -1;
		}
		/* Now the OID list has one more entry */
		request->oid_list_length++;
	}

	return 0;
}



/* -----------------------------------------------------------------------------
 * Helper functions for encoding
 */

static int get_integer_length(int integer_value)
{
	if (integer_value < -8388608 || integer_value > 8388607) {
		return 6;
	} else if (integer_value < -32768 || integer_value > 32767) {
		return 5;
	} else if (integer_value < -128 || integer_value > 127) {
		return 4;
	} else {
		return 3;
	}
}

static int get_string_length(const char *string_value)
{
	size_t length;

	length = strlen(string_value);
	if (length > 0xFFFF) {
		return MAX_PACKET_SIZE;
	} else if (length > 0xFF) {
		return length + 4;
	} else if (length > 0x7F) {
		return length + 3;
	} else {
		return length + 2;
	}
}

static int get_sequence_header_length(int length)
{
	if (length > 0xFFFF) {
		return MAX_PACKET_SIZE;
	} else if (length > 0xFF) {
		return 4;
	} else if (length > 0x7F) {
		return 3;
	} else {
		return 2;
	}
}

static int encode_snmp_integer(unsigned char *buffer, int integer_value)
{
	int length;

	if (integer_value < -8388608 || integer_value > 8388607) {
		length = 4;
	} else if (integer_value < -32768 || integer_value > 32767) {
		length = 3;
	} else if (integer_value < -128 || integer_value > 127) {
		length = 2;
	} else {
		length = 1;
	}
	*buffer++ = BER_TYPE_INTEGER;
	*buffer++ = length;
	while (length--) {
		*buffer++ = ((unsigned int)integer_value >> (8 * length)) & 0xFF;
	}
	return 0;
}

static int encode_snmp_string(unsigned char *buffer, const char *string_value)
{
	size_t length;

	length = strlen(string_value);
	if (length > 0xFFFF) {
		return -1;
	}
	*buffer++ = BER_TYPE_OCTET_STRING;
	if (length > 0xFF) {
		*buffer++ = 0x82;
		*buffer++ = (length >> 8) & 0xFF;
		*buffer++ = length & 0xFF;
	} else if (length > 0x7F) {
		*buffer++= 0x81;
		*buffer++ = length & 0xFF;
	} else {
		*buffer++ = length & 0x7F;
	}
	memcpy(buffer, string_value, length);
	return 0;
}

static int encode_snmp_sequence_header(unsigned char *buffer, int length, int type)
{
	if (length > 0xFFFF) {
		return -1;
	}
	*buffer++ = type;
	if (length > 0xFF) {
		*buffer++ = 0x82;
		*buffer++ = (length >> 8) & 0xFF;
		*buffer++ = length & 0xFF;
	} else if (length > 0x7F) {
		*buffer++= 0x81;
		*buffer++ = length & 0xFF;
	} else {
		*buffer++ = length & 0x7F;
	}
	return 0;
}

static int encode_snmp_oid(unsigned char *buffer, const oid_t *oid)
{
	int length;
	int i;

	length = 1;
	for (i = 2; i < oid->subid_list_length; i++) {
		if (oid->subid_list[i] >= (1 << 28)) {
			length += 5;
		} else if (oid->subid_list[i] >= (1 << 21)) {
			length += 4;
		} else if (oid->subid_list[i] >= (1 << 14)) {
			length += 3;
		} else if (oid->subid_list[i] >= (1 << 7)) {
			length += 2;
		} else {
			length += 1;
		}
	}
	*buffer++ = BER_TYPE_OID;
	if (length > 0xFFFF) {
		lprintf(LOG_ERR, "could not encode '%s': OID overflow\n", oid_ntoa(oid));
		return -1;
	} else if (length > 0xFF) {
		*buffer++ = 0x82;
		*buffer++ = (length >> 8) & 0xFF;
		*buffer++ = length & 0xFF;
	} else if (length > 0x7F) {
		*buffer++ = 0x81;
		*buffer++ = length & 0xFF;
	} else {
		*buffer++ = length & 0x7F;
	}
	*buffer++ = oid->subid_list[0] * 40 + oid->subid_list[1];
	for (i = 2; i < oid->subid_list_length; i++) {
		if (oid->subid_list[i] >= (1 << 28)) {
			length = 5;
		} else if (oid->subid_list[i] >= (1 << 21)) {
			length = 4;
		} else if (oid->subid_list[i] >= (1 << 14)) {
			length = 3;
		} else if (oid->subid_list[i] >= (1 << 7)) {
			length = 2;
		} else {
			length = 1;
		}
		while (length--) {
			if (length) {
				*buffer++ = ((oid->subid_list[i] >> (7 * length)) & 0x7F) | 0x80;
			} else {
				*buffer++ = (oid->subid_list[i] >> (7 * length)) & 0x7F;
			}
		}
	}
	return 0;
}

static int encode_snmp_varbind(unsigned char *buffer, int *pos, const value_t *value)
{
	int length;

	/* The value of the variable binding (NULL for error responses) */
	length = value->data.encoded_length;
	if (*pos >= length) {
		memcpy(&buffer[*pos - length], value->data.buffer, length);
		*pos = *pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode '%s': DATA overflow\n", oid_ntoa(&value->oid));
		return -1;
	}

	/* The OID of the variable binding */
	length = value->oid.encoded_length;
	if (*pos >= length) {
		encode_snmp_oid(&buffer[*pos - length], &value->oid);
		*pos = *pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode '%s': OID overflow\n", oid_ntoa(&value->oid));
		return -1;
	}

	/* The sequence header (type and length) of the variable binding */
	length = get_sequence_header_length(value->oid.encoded_length + value->data.encoded_length);
	if (*pos >= length) {
		encode_snmp_sequence_header(&buffer[*pos - length],
			value->oid.encoded_length + value->data.encoded_length, BER_TYPE_SEQUENCE);
		*pos = *pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode '%s': VARBIND overflow\n", oid_ntoa(&value->oid));
		return -1;
	}

	return 0;
}

static int encode_snmp_response(request_t *request, response_t *response, client_t *client)
{
	int length;
	int pos;
	int i;

	/* If there was an error, we have to encode the original varbind list, but
	 * omit any varbind values (replace them with NULL values)
	 */
	if (response->error_status != SNMP_STATUS_OK) {
		if (request->oid_list_length > MAX_NR_VALUES) {
			lprintf(LOG_ERR, "could not encode SNMP response: value list overflow\n");
			return -1;
		}
		for (i = 0; i < request->oid_list_length; i++) {
			memcpy(&response->value_list[i].oid, &request->oid_list[i],
				sizeof (request->oid_list[i]));
			response->value_list[i].data.max_length = m_null.max_length;
			response->value_list[i].data.encoded_length = m_null.encoded_length;
			response->value_list[i].data.buffer = malloc(m_null.max_length);
			memcpy(response->value_list[i].data.buffer,
					m_null.buffer,
					m_null.max_length);
			//memcpy(&response->value_list[i].data, &m_null, sizeof (m_null));
		}
		response->value_list_length = request->oid_list_length;
	}

	/* Dump the response for debugging purposes */
#ifdef DEBUG
	dump_response(response);
#endif

	/* To make the code more compact and save processing time, we are encoding the
	 * data beginning at the last byte of the buffer backwards. Thus, the encoded
	 * packet will not be positioned at offset 0..(size-1) of the client's packet
	 * buffer, but at offset (bufsize-size..bufsize-1)!
	 */
	pos = MAX_PACKET_SIZE;
	for (i = response->value_list_length -1; i >= 0; i--) {
		if (encode_snmp_varbind(client->packet, &pos, &response->value_list[i]) == -1) {
			return -1;
		}
	}
	length = get_sequence_header_length(MAX_PACKET_SIZE - pos);
	if (pos >= length) {
		encode_snmp_sequence_header(&client->packet[pos - length],
			MAX_PACKET_SIZE - pos, BER_TYPE_SEQUENCE);
		pos = pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode response: VARBINDS overflow\n");
		return -1;
	}
	length = get_integer_length(response->error_index);
	if (pos >= length) {
		encode_snmp_integer(&client->packet[pos - length], response->error_index);
		pos = pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode response: ERROR INDEX overflow\n");
		return -1;
	}
	length = get_integer_length(response->error_status);
	if (pos >= length) {
		encode_snmp_integer(&client->packet[pos - length], response->error_status);
		pos = pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode response: ERROR STATUS overflow\n");
		return -1;
	}
	length = get_integer_length(request->id);
	if (pos >= length) {
		encode_snmp_integer(&client->packet[pos - length], request->id);
		pos = pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode response: ID overflow\n");
		return -1;
	}
	length = get_sequence_header_length(MAX_PACKET_SIZE - pos);
	if (pos >= length) {
		encode_snmp_sequence_header(&client->packet[pos - length],
			MAX_PACKET_SIZE - pos, BER_TYPE_SNMP_RESPONSE);
		pos = pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode response: PDU overflow\n");
		return -1;
	}
	length = get_string_length(request->community);
	if (pos >= length) {
		encode_snmp_string(&client->packet[pos - length], request->community);
		pos = pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode response: COMMUNITY overflow\n");
		return -1;
	}
	length = get_integer_length(request->version);
	if (pos >= length) {
		encode_snmp_integer(&client->packet[pos - length], request->version);
		pos = pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode response: VERSION overflow\n");
		return -1;
	}
	length = get_sequence_header_length(MAX_PACKET_SIZE - pos);
	if (pos >= length) {
		encode_snmp_sequence_header(&client->packet[pos - length],
			MAX_PACKET_SIZE - pos, BER_TYPE_SEQUENCE);
		pos = pos - length;
	} else {
		lprintf(LOG_ERR, "could not encode response: RESPONSE overflow\n");
		return -1;
	}

	/* Now move the packet to the start of the buffer so that the caller does not have
	 * to deal with this messy detail (the CPU cycles needed are worth their money!)
	 * and set up the packet size.
	 */
	if (pos > 0) {
		memmove(&client->packet[0], &client->packet[pos], MAX_PACKET_SIZE - pos);
	}
	client->size = MAX_PACKET_SIZE - pos;

	return 0;
}


/* -----------------------------------------------------------------------------
 * Interface functions
 */

int snmp_packet_complete(const client_t *client)
{
	size_t pos = 0;
	int length;
	int type;

	/* The SNMP message must be at least have a header containing sequence,
	 * version, community, sequence, request id, 2 integers, sequence, oid
	 * and null value.
	 */
	if (client->size < 25) {
		return 0;
	}

	/* The SNMP message is enclosed in a sequence */
	if (decode_snmp_element_type_length(client->packet, client->size, &pos, &type, &length) == -1) {
		return -1;
	} else if (type != BER_TYPE_SEQUENCE || length < 1 || length > (client->size - pos)) {
		lprintf(LOG_DEBUG, "unexpected SNMP header type %02X length %d\n", type, length);
		errno = EINVAL;
		return -1;
	}

	/* Return whether we received the whole packet */
	return ((client->size - pos) == length) ? 1 : 0;
}

int snmp(client_t *client)
{
	response_t response;
	request_t request;

	int i=0;

	/* Setup request and response (other code only changes non-defaults) */
	memset(&request, 0, sizeof (request));
	memset(&response, 0, sizeof (response));

	/* Decode the request (only checks for syntax of the packet) */
	if (decode_snmp_request(&request, client) == -1) {
		goto fail;
	}

	/* If we are using SNMP v2c or require authentication, check the community
	 * string for length and validity.
	 */
	if (request.version == SNMP_VERSION_2C) {
		if (BER_TYPE_SNMP_SET == request.type)
		{
            if ( strcmp(g_stSnmpV2Config.acWriteCommunity, request.community) )
            {
				response.error_status = (request.version == SNMP_VERSION_2C)
					? SNMP_STATUS_NO_ACCESS : SNMP_STATUS_GEN_ERR;
				response.error_index = 0;
				goto done;
            }
			/*if (strcmp(g_rwcommunity, request.community)) {				
				response.error_status = (request.version == SNMP_VERSION_2C)
					? SNMP_STATUS_NO_ACCESS : SNMP_STATUS_GEN_ERR;
				response.error_index = 0;
				goto done;
			}*/
		}
        else if ( strcmp(g_stSnmpV2Config.acReadCommunity, request.community) )
        {
			response.error_status = (request.version == SNMP_VERSION_2C)
				? SNMP_STATUS_NO_ACCESS : SNMP_STATUS_GEN_ERR;
			response.error_index = 0;
			goto done;
        }
		/*else if (strcmp(g_rocommunity, request.community)) {
			response.error_status = (request.version == SNMP_VERSION_2C)
				? SNMP_STATUS_NO_ACCESS : SNMP_STATUS_GEN_ERR;
			response.error_index = 0;
			goto done;
		}*/
	} else if (g_auth) {
		response.error_status = SNMP_STATUS_GEN_ERR;
		response.error_index = 0;
		goto done;
	}

	/* Now handle the SNMP requests depending on their type */
	switch (request.type) {
		case BER_TYPE_SNMP_GET:
			if (handle_snmp_get(&request, &response, client) == -1) {
				goto fail;
			}
			break;
		case BER_TYPE_SNMP_GETNEXT:
			if (handle_snmp_getnext(&request, &response, client) == -1) {
				goto fail;
			}
			break;
		case BER_TYPE_SNMP_SET:
			if (handle_snmp_set(&request, &response, client) == -1) {
				goto fail;
			}
			break;
		case BER_TYPE_SNMP_GETBULK:
			if (handle_snmp_getbulk(&request, &response, client) == -1) {
				goto fail;
			}
			break;
		default:
			client->size = 0;
			return 0;
	}

done:
	/* Encode the request (depending on error status and encode flags) */
	if (encode_snmp_response(&request, &response, client) == -1) {
		goto fail;
	}
//#if 0
	/* feng jing add: */
	for(i=0;i<request.oid_list_length;i++)
	{
		if(NULL != request.data_list[i].buffer)
		{
			free(request.data_list[i].buffer);
		}
	}	

	for(i=0;i<response.value_list_length;i++)
	{
		if(NULL != response.value_list[i].data.buffer)
		{
			free(response.value_list[i].data.buffer);
		}
	}
//#endif
	return 0;

	/* feng jing add: */
fail:
//#if 0
	for(i=0;i<request.oid_list_length;i++)
	{
		if(NULL != request.data_list[i].buffer)
		{
			free(request.data_list[i].buffer);
		}
	}	

	for(i=0;i<response.value_list_length;i++)
	{
		if(NULL != response.value_list[i].data.buffer)
		{
			free(response.value_list[i].data.buffer);
		}
	}
//#endif
	return -1;
}

int snmp_element_as_string(const data_t *data, char *buffer, size_t size)
{
	size_t pos = 0;
	unsigned int unsigned_value;
	int integer_value;
	oid_t oid_value;
	int length;
	int type;
	int i;

	/* Decode the element type and length */
	if (decode_snmp_element_type_length(data->buffer, data->encoded_length,
		&pos, &type, &length) == -1) {
		return -1;
	}

	/* Depending on type and length, decode the data */
	switch (type) {
		case BER_TYPE_INTEGER:
			if (decode_snmp_element_value_integer(data->buffer, data->encoded_length,
				&pos, length, &integer_value) == -1) {
				return -1;
			}
			snprintf(buffer, size, "%d", integer_value);
			break;
		case BER_TYPE_OCTET_STRING:
			snprintf(buffer, size, "%.*s", length, &data->buffer[pos]);
			break;
		case BER_TYPE_OID:
			if (decode_snmp_element_value_oid(data->buffer, data->encoded_length,
				&pos, length, &oid_value) == -1) {
				return -1;
			}
			snprintf(buffer, size, "%s", oid_ntoa(&oid_value));
			break;
		case BER_TYPE_COUNTER:
		case BER_TYPE_GAUGE:
		case BER_TYPE_TIME_TICKS:
			if (decode_snmp_element_value_unsigned(data->buffer, data->encoded_length,
				&pos, length, &unsigned_value) == -1) {
				return -1;
			}
			snprintf(buffer, size, "%u", unsigned_value);
			break;
		case BER_TYPE_NO_SUCH_OBJECT:
			snprintf(buffer, size, "noSuchObject");
			break;
		case BER_TYPE_NO_SUCH_INSTANCE:
			snprintf(buffer, size, "noSuchInstance");
			break;
		case BER_TYPE_END_OF_MIB_VIEW:
			snprintf(buffer, size, "endOfMibView");
			break;
		default:
			for (i = 0; i < length && i < ((size - 1) / 3); i++) {
				snprintf(buffer + 3 * i, 4, "%02X ", data->buffer[pos + i]);
			}
			if (length > 0) {
				buffer[length * 3 - 1] = '\0';
			} else {
				buffer[0] = '\0';
			}
	}

	return 0;
}

/* BEGIN: Added by zoucaihong, 2013/9/25 */
int get_snmp_request_type(request_t *request, client_t *client)
{
	/* Decode the request (only checks for syntax of the packet) */
	if (decode_snmp_request(request, client) == -1) {
		return -1;
	}
    return 0;
}
/* END: Added by zoucaihong, 2013/9/25 */

/* vim: ts=4 sts=4 sw=4 nowrap
 */
