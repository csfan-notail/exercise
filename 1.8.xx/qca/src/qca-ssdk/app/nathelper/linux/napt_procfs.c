/*
 * Copyright (c) 2012, The Linux Foundation. All rights reserved.
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


/**
 *  napt_procfs.c -  create files in /proc
 *
 */
#ifdef KVER32
#include <generated/autoconf.h>
#else
#include <linux/autoconf.h>
#include <linux/kernel.h>
#endif
#include <linux/proc_fs.h>
#include <linux/if_ether.h>
#include <asm/uaccess.h>    /* for copy_from_user */
#include "aos_types.h"

#ifdef AUTO_UPDATE_PPPOE_INFO
#define NF_PROCFS_PERM  0444
#else
#define NF_PROCFS_PERM  0644
#endif

#define ATHRS17_MAC_LEN         13      // 12+1
#define ATHRS17_IP_LEN     9       // 8+1
#define ATHRS17_CHAR_MAX_LEN ATHRS17_MAC_LEN

#define NF_PROCFS_DIR                        "qca_switch"

#define NF_ATHRS17_HNAT_NAME                 "nf_athrs17_hnat"
#define NF_ATHRS17_HNAT_WAN_TYPE_NAME        "nf_athrs17_hnat_wan_type"
#define NF_ATHRS17_HNAT_PPP_ID_NAME          "nf_athrs17_hnat_ppp_id"
#define NF_ATHRS17_HNAT_UDP_THRESH_NAME      "nf_athrs17_hnat_udp_thresh"
#define NF_ATHRS17_HNAT_WAN_IP_NAME          "nf_athrs17_hnat_wan_ip"
#define NF_ATHRS17_HNAT_PPP_PEER_IP_NAME     "nf_athrs17_hnat_ppp_peer_ip"
#define NF_ATHRS17_HNAT_PPP_PEER_MAC_NAME    "nf_athrs17_hnat_ppp_peer_mac"
#define NF_ATHRS17_HNAT_WAN_MAC_NAME         "nf_athrs17_hnat_wan_mac"

#define NF_ATHRS17_HNAT_PPP_ID2_NAME          "nf_athrs17_hnat_ppp_id2"
#define NF_ATHRS17_HNAT_PPP_PEER_MAC2_NAME    "nf_athrs17_hnat_ppp_peer_mac2"

/* for PPPoE */
int nf_athrs17_hnat = 1;
int nf_athrs17_hnat_wan_type = 0;
int nf_athrs17_hnat_ppp_id = 0;
int nf_athrs17_hnat_udp_thresh = 0;
a_uint32_t nf_athrs17_hnat_wan_ip = 0;
a_uint32_t nf_athrs17_hnat_ppp_peer_ip = 0;
unsigned char nf_athrs17_hnat_ppp_peer_mac[ETH_ALEN] = {0};
unsigned char nf_athrs17_hnat_wan_mac[ETH_ALEN] = {0};

/* for IPv6 over PPPoE (only for S17c)*/
int nf_athrs17_hnat_ppp_id2 = 0;
unsigned char nf_athrs17_hnat_ppp_peer_mac2[ETH_ALEN] = {0};

/**
 * This structure hold information about the /proc file
 *
 */
static struct proc_dir_entry *qca_switch_dir;

static struct proc_dir_entry *nf_athrs17_hnat_file;
static struct proc_dir_entry *nf_athrs17_hnat_wan_type_file;
static struct proc_dir_entry *nf_athrs17_hnat_ppp_id_file;
static struct proc_dir_entry *nf_athrs17_hnat_udp_thresh_file;
static struct proc_dir_entry *nf_athrs17_hnat_wan_ip_file;
static struct proc_dir_entry *nf_athrs17_hnat_ppp_peer_ip_file;
static struct proc_dir_entry *nf_athrs17_hnat_ppp_peer_mac_file;
static struct proc_dir_entry *nf_athrs17_hnat_wan_mac_file;

static struct proc_dir_entry *nf_athrs17_hnat_ppp_id2_file;
static struct proc_dir_entry *nf_athrs17_hnat_ppp_peer_mac2_file;

/**
 * This function is called then the /proc file is read
 *
 */
static int procfile_read_int(char *page, char **start, off_t off, int count,  int *eof, void *data)
{
    int ret;
    int *prv_data = (int *)data;

    // printk("[read] prv_data 0x%p -> 0x%08x\n", prv_data, *prv_data);
    ret = sprintf(page, "%d\n", *prv_data);

    return ret;
}

static int procfile_read_ip(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret;
	unsigned char *prv_data = (unsigned char *)data;

	ret = sprintf(page, "%d.%d.%d.%d\n", prv_data[0], prv_data[1], prv_data[2], prv_data[3]);

	return ret;
}

static int procfile_read_mac(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret;
	unsigned char *prv_data = (unsigned char *)data;
	unsigned long long *ptr_ull;

	ret = sprintf(page, "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n", prv_data[0], prv_data[1], prv_data[2], prv_data[3], prv_data[4], prv_data[5]);

	ptr_ull = (unsigned long long *)prv_data;

	return ret;
}

/**
 * This function is called with the /proc file is written
 *
 */
#ifdef AUTO_UPDATE_PPPOE_INFO
#define procfile_write_int NULL
#define procfile_write_ip NULL
#define procfile_write_mac NULL
#else
static int procfile_write_int(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int len;
    uint8_t tmp_buf[9] = {'0', '0', '0', '0', '0', '0', '0', '0', '0'};
    unsigned int *prv_data = (unsigned int *)data;
    
    if(count > sizeof(tmp_buf))
        len = sizeof(tmp_buf);
    else
        len = count;

    if(copy_from_user(tmp_buf, buffer, len))
        return -EFAULT;

    *prv_data = simple_strtol((const char *)tmp_buf, NULL, 10);

    // printk("[write] prv_data 0x%p -> 0x%08x\n", prv_data, *prv_data);

    return len;
}

static int procfile_write_ip(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int ret;
    int len;
    unsigned char tmp_buf[ATHRS17_IP_LEN];
    unsigned long *prv_data = (unsigned long *)data;

    if(count > ATHRS17_IP_LEN)
        len = ATHRS17_IP_LEN;
    else
        len = count;
		
    if(copy_from_user(tmp_buf, buffer, len))
		return -EFAULT;

    tmp_buf[len-1] = '\0';

    *prv_data = simple_strtoul((const char *)tmp_buf, NULL, 16);

    return ret;
}

static int procfile_write_mac(struct file *file, const char *buffer, unsigned long count, void *data)
{
    int ret;
    int len;
    unsigned char tmp_buf[ATHRS17_MAC_LEN];
    unsigned char *ptr_char;
    unsigned long long *prv_data = (unsigned long long *)data;

    if(count > ATHRS17_MAC_LEN)
        len = ATHRS17_MAC_LEN;
    else
        len = count;
		
    if(copy_from_user((void *)tmp_buf, buffer, len))
		return -EFAULT;

    tmp_buf[len-1] = 't';

    *prv_data = simple_strtoull((const char *)tmp_buf, NULL, 16);
    *prv_data = cpu_to_be64p(prv_data);
    ptr_char = (unsigned char *)prv_data;
    ptr_char[0] = ptr_char[2];
    ptr_char[1] = ptr_char[3];
    ptr_char[2] = ptr_char[4];
    ptr_char[3] = ptr_char[5];
    ptr_char[4] = ptr_char[6];
    ptr_char[5] = ptr_char[7];

    return ret;
}
#endif // ifdef AUTO_UPDATE_PPPOE_INFO

static void setup_proc_entry(void)
{
    nf_athrs17_hnat = 1;
    nf_athrs17_hnat_wan_type = 0;
    nf_athrs17_hnat_ppp_id = 0;
    memset(&nf_athrs17_hnat_ppp_peer_mac, 0, ETH_ALEN);
    memset(&nf_athrs17_hnat_wan_mac, 0, ETH_ALEN);
    nf_athrs17_hnat_ppp_peer_ip = 0;
    nf_athrs17_hnat_wan_ip = 0;

    nf_athrs17_hnat_ppp_id2 = 0;
    memset(&nf_athrs17_hnat_ppp_peer_mac2, 0, ETH_ALEN);
}

int napt_procfs_init(void)
{
    int ret = 0;

    setup_proc_entry();

    /* create directory */
    qca_switch_dir = proc_mkdir(NF_PROCFS_DIR, NULL);
    if(qca_switch_dir == NULL)
    {
        ret = -ENOMEM;
        goto err_out;
    }

    /* create the /proc file */
    nf_athrs17_hnat_file = create_proc_entry(NF_ATHRS17_HNAT_NAME, 0644, qca_switch_dir);
    if (NULL == nf_athrs17_hnat_file)
    {
        printk("Error: Can not create /proc/%s/%s\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_NAME);
        goto no_athrs17_hnat;
    }
    nf_athrs17_hnat_file->data = &nf_athrs17_hnat;
    nf_athrs17_hnat_file->read_proc  = procfile_read_int;
    nf_athrs17_hnat_file->write_proc = procfile_write_int;
    printk("/proc/%s/%s is created\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_NAME);

    nf_athrs17_hnat_wan_type_file = create_proc_entry(NF_ATHRS17_HNAT_WAN_TYPE_NAME, NF_PROCFS_PERM, qca_switch_dir);
    if (NULL == nf_athrs17_hnat_wan_type_file)
    {
        printk("Error: Can not create /proc/%s/%s\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_WAN_TYPE_NAME);
        goto no_athrs17_hnat_wan_type;
    }
    nf_athrs17_hnat_wan_type_file->data = &nf_athrs17_hnat_wan_type;
    nf_athrs17_hnat_wan_type_file->read_proc  = procfile_read_int;
    nf_athrs17_hnat_wan_type_file->write_proc = procfile_write_int;
    printk("/proc/%s/%s is created\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_WAN_TYPE_NAME);

    nf_athrs17_hnat_ppp_id_file = create_proc_entry(NF_ATHRS17_HNAT_PPP_ID_NAME, NF_PROCFS_PERM, qca_switch_dir);
    if (NULL == nf_athrs17_hnat_ppp_id_file)
    {
        printk("Error: Can not create /proc/%s/%s\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_PPP_ID_NAME);
        goto no_athrs17_hnat_ppp_id;
    }
    nf_athrs17_hnat_ppp_id_file->data = &nf_athrs17_hnat_ppp_id;
    nf_athrs17_hnat_ppp_id_file->read_proc  = procfile_read_int;
    nf_athrs17_hnat_ppp_id_file->write_proc = procfile_write_int;
    printk("/proc/%s/%s is created\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_PPP_ID_NAME);

    nf_athrs17_hnat_udp_thresh_file = create_proc_entry(NF_ATHRS17_HNAT_UDP_THRESH_NAME, NF_PROCFS_PERM, qca_switch_dir);
    if (NULL == nf_athrs17_hnat_udp_thresh_file)
    {
        printk("Error: Can not create /proc/%s/%s\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_UDP_THRESH_NAME);
        goto no_athrs17_hnat_udp_thresh;
    }
    nf_athrs17_hnat_udp_thresh_file->data = &nf_athrs17_hnat_udp_thresh;
    nf_athrs17_hnat_udp_thresh_file->read_proc  = procfile_read_int;
    nf_athrs17_hnat_udp_thresh_file->write_proc = procfile_write_int;
    printk("/proc/%s/%s is created\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_UDP_THRESH_NAME);

    nf_athrs17_hnat_wan_ip_file = create_proc_entry(NF_ATHRS17_HNAT_WAN_IP_NAME, NF_PROCFS_PERM, qca_switch_dir);
    if (NULL == nf_athrs17_hnat_wan_ip_file)
    {
        printk("Error: Can not create /proc/%s/%s\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_WAN_IP_NAME);
        goto no_athrs17_hnat_wan_ip;
    }
    nf_athrs17_hnat_wan_ip_file->data = &nf_athrs17_hnat_wan_ip;
    nf_athrs17_hnat_wan_ip_file->read_proc  = procfile_read_ip;
    nf_athrs17_hnat_wan_ip_file->write_proc = procfile_write_ip;
    printk("/proc/%s/%s is created\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_WAN_IP_NAME);

    nf_athrs17_hnat_ppp_peer_ip_file = create_proc_entry(NF_ATHRS17_HNAT_PPP_PEER_IP_NAME, NF_PROCFS_PERM, qca_switch_dir);
    if (NULL == nf_athrs17_hnat_ppp_peer_ip_file)
    {
        printk("Error: Can not create /proc/%s/%s\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_PPP_PEER_IP_NAME);
        goto no_athrs17_hnat_ppp_peer_ip;
    }
    nf_athrs17_hnat_ppp_peer_ip_file->data = &nf_athrs17_hnat_ppp_peer_ip;
    nf_athrs17_hnat_ppp_peer_ip_file->read_proc  = procfile_read_ip;
    nf_athrs17_hnat_ppp_peer_ip_file->write_proc = procfile_write_ip;
    printk("/proc/%s/%s is created\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_PPP_PEER_IP_NAME);

    nf_athrs17_hnat_ppp_peer_mac_file = create_proc_entry(NF_ATHRS17_HNAT_PPP_PEER_MAC_NAME, NF_PROCFS_PERM, qca_switch_dir);
    if (NULL == nf_athrs17_hnat_ppp_peer_mac_file)
    {
        printk("Error: Can not create /proc/%s/%s\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_PPP_PEER_MAC_NAME);
        goto no_athrs17_hnat_ppp_peer_mac;
    }
    nf_athrs17_hnat_ppp_peer_mac_file->data = &nf_athrs17_hnat_ppp_peer_mac;
    nf_athrs17_hnat_ppp_peer_mac_file->read_proc  = procfile_read_mac;
    nf_athrs17_hnat_ppp_peer_mac_file->write_proc = procfile_write_mac;
    printk("/proc/%s/%s is created\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_PPP_PEER_MAC_NAME);

    nf_athrs17_hnat_wan_mac_file = create_proc_entry(NF_ATHRS17_HNAT_WAN_MAC_NAME, NF_PROCFS_PERM, qca_switch_dir);
    if (NULL == nf_athrs17_hnat_wan_mac_file)
    {
        printk("Error: Can not create /proc/%s/%s\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_WAN_MAC_NAME);
        goto no_athrs17_hnat_wan_mac;
    }
    nf_athrs17_hnat_wan_mac_file->data = &nf_athrs17_hnat_wan_mac;
    nf_athrs17_hnat_wan_mac_file->read_proc  = procfile_read_mac;
    nf_athrs17_hnat_wan_mac_file->write_proc = procfile_write_mac;
    printk("/proc/%s/%s is created\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_WAN_MAC_NAME);

    nf_athrs17_hnat_ppp_id2_file = create_proc_entry(NF_ATHRS17_HNAT_PPP_ID2_NAME, NF_PROCFS_PERM, qca_switch_dir);
    if (NULL == nf_athrs17_hnat_ppp_id2_file)
    {
        printk("Error: Can not create /proc/%s/%s\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_PPP_ID2_NAME);
        goto no_athrs17_hnat_ppp_id;
    }
    nf_athrs17_hnat_ppp_id2_file->data = &nf_athrs17_hnat_ppp_id2;
    nf_athrs17_hnat_ppp_id2_file->read_proc  = procfile_read_int;
    nf_athrs17_hnat_ppp_id2_file->write_proc = procfile_write_int;
    printk("/proc/%s/%s is created\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_PPP_ID2_NAME);

    nf_athrs17_hnat_ppp_peer_mac2_file = create_proc_entry(NF_ATHRS17_HNAT_PPP_PEER_MAC2_NAME, NF_PROCFS_PERM, qca_switch_dir);
    if (NULL == nf_athrs17_hnat_ppp_peer_mac2_file)
    {
        printk("Error: Can not create /proc/%s/%s\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_PPP_PEER_MAC2_NAME);
        goto no_athrs17_hnat_ppp_peer_mac;
    }
    nf_athrs17_hnat_ppp_peer_mac2_file->data = &nf_athrs17_hnat_ppp_peer_mac2;
    nf_athrs17_hnat_ppp_peer_mac2_file->read_proc  = procfile_read_mac;
    nf_athrs17_hnat_ppp_peer_mac2_file->write_proc = procfile_write_mac;
    printk("/proc/%s/%s is created\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_PPP_PEER_MAC_NAME);

    return 0;

no_athrs17_hnat_wan_mac:
    remove_proc_entry(NF_ATHRS17_HNAT_PPP_PEER_MAC_NAME, qca_switch_dir);
no_athrs17_hnat_ppp_peer_mac:
    remove_proc_entry(NF_ATHRS17_HNAT_PPP_PEER_IP_NAME, qca_switch_dir);
no_athrs17_hnat_ppp_peer_ip:
    remove_proc_entry(NF_ATHRS17_HNAT_WAN_IP_NAME, qca_switch_dir);
no_athrs17_hnat_wan_ip:
    remove_proc_entry(NF_ATHRS17_HNAT_UDP_THRESH_NAME, qca_switch_dir);
no_athrs17_hnat_udp_thresh:
    remove_proc_entry(NF_ATHRS17_HNAT_PPP_ID_NAME, qca_switch_dir);
no_athrs17_hnat_ppp_id:
    remove_proc_entry(NF_ATHRS17_HNAT_WAN_TYPE_NAME, qca_switch_dir);
no_athrs17_hnat_wan_type:
    remove_proc_entry(NF_ATHRS17_HNAT_NAME, qca_switch_dir);
no_athrs17_hnat:
    remove_proc_entry(NF_PROCFS_DIR, NULL);
err_out:
    return ret;
}

void napt_procfs_exit(void)
{
    remove_proc_entry(NF_ATHRS17_HNAT_NAME, qca_switch_dir);
    remove_proc_entry(NF_ATHRS17_HNAT_WAN_TYPE_NAME, qca_switch_dir);
    remove_proc_entry(NF_ATHRS17_HNAT_PPP_ID_NAME, qca_switch_dir);
    remove_proc_entry(NF_ATHRS17_HNAT_UDP_THRESH_NAME, qca_switch_dir);
    remove_proc_entry(NF_ATHRS17_HNAT_WAN_IP_NAME, qca_switch_dir);
    remove_proc_entry(NF_ATHRS17_HNAT_PPP_PEER_IP_NAME, qca_switch_dir);
    remove_proc_entry(NF_ATHRS17_HNAT_PPP_PEER_MAC_NAME, qca_switch_dir);
    remove_proc_entry(NF_ATHRS17_HNAT_WAN_MAC_NAME, qca_switch_dir);
    remove_proc_entry(NF_ATHRS17_HNAT_PPP_ID2_NAME, qca_switch_dir);
    remove_proc_entry(NF_ATHRS17_HNAT_PPP_PEER_MAC2_NAME, qca_switch_dir);
    remove_proc_entry(NF_PROCFS_DIR, NULL);
    printk(KERN_INFO "/proc/%s/%s removed\n", NF_PROCFS_DIR, NF_ATHRS17_HNAT_NAME);
}


