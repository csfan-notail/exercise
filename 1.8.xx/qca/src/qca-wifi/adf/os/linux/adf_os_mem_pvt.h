/*
 * Copyright (c) 2010, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */

#ifndef ADF_CMN_OS_MEM_PVT_H
#define ADF_CMN_OS_MEM_PVT_H

#ifdef __KERNEL__
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17)
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 33)
#include <linux/autoconf.h>
#else
#include <generated/autoconf.h>
#endif
#endif
#include <linux/slab.h>
#include <linux/hardirq.h>
#include <linux/vmalloc.h>
#include <linux/pci.h> /* pci_alloc_consistent */
#else
/*
 * Provide dummy defs for kernel data types, functions, and enums
 * used in this header file.
 */
#define GFP_KERNEL 0
#define GFP_ATOMIC 0
#define kzalloc(size, flags) NULL
#define vmalloc(size)        NULL
#define kfree(buf)
#define vfree(buf)
#define pci_alloc_consistent(dev, size, paddr) NULL
#endif /* __KERNEL__ */

void *bus_alloc_consistent(void *, size_t, dma_addr_t *);
void bus_free_consistent(void *, size_t, void *, dma_addr_t);

static inline void *
__adf_os_mem_alloc(adf_os_device_t osdev, size_t size)
{
    int flags = GFP_KERNEL;

    if(in_interrupt() || irqs_disabled())
        flags = GFP_ATOMIC;

    return kzalloc(size, flags);
}


static inline void
__adf_os_mem_free(void *buf)
{
    kfree(buf);
}


static inline void *
__adf_os_mem_alloc_consistent(
    adf_os_device_t osdev, adf_os_size_t size, adf_os_dma_addr_t *paddr, adf_os_dma_context_t memctx)
{
#if defined(A_SIMOS_DEVHOST)
    static int first = 1;
    void *vaddr;

    if (first) {
        first = 0;
        printk("Warning: bypassing %s\n", __func__);
    }
    vaddr = __adf_os_mem_alloc(osdev, size);
    *paddr = ((adf_os_dma_addr_t) vaddr);
    return vaddr;
#else
    return bus_alloc_consistent(osdev->drv, size, paddr);
#endif
}

static inline void
__adf_os_mem_free_consistent(
    adf_os_device_t osdev,
    adf_os_size_t size,
    void *vaddr,
    adf_os_dma_addr_t paddr,
    adf_os_dma_context_t memctx)
{
#if defined(A_SIMOS_DEVHOST)
    static int first = 1;

    if (first) {
        first = 0;
        printk("Warning: bypassing %s\n", __func__);
    }
    __adf_os_mem_free(vaddr);
    return;
#else
    bus_free_consistent(osdev->drv, size, vaddr,paddr);
    //pci_free_consistent(osdev->drv_hdl, size, vaddr, paddr);
#endif
}

/* move a memory buffer */
static inline void
__adf_os_mem_copy(void *dst, const void *src, size_t size)
{
    memcpy(dst, src, size);
}

/* set a memory buffer */
static inline void
__adf_os_mem_set(void *buf, uint8_t b, size_t size)
{
    memset(buf, b, size);
}

/* zero a memory buffer */
static inline void
__adf_os_mem_zero(void *buf, size_t size)
{
    memset(buf, 0, size);
}

/* compare two memory buffers */
static inline int
__adf_os_mem_cmp(const void *buf1, const void *buf2, size_t size)
{
    return memcmp(buf1, buf2, size);
}

/**
 * @brief  Unlike memcpy(), memmove() copes with overlapping
 *         areas.
 * @param src
 * @param dst
 * @param size
 */
static inline void
__adf_os_mem_move(void *dst, const void *src, size_t size)
{
    memmove(dst, src, size);
}

/**
 * @brief Compare two strings
 *
 * @param[in] str1  First string
 * @param[in] str2  Second string
 *
 * @retval    0     equal
 * @retval    >0    not equal, if  str1  sorts lexicographically after str2
 * @retval    <0    not equal, if  str1  sorts lexicographically before str2
 */
static inline a_int32_t
__adf_os_str_cmp(const char *str1, const char *str2)
{   
    return strcmp(str1, str2);
}

/**
 * @brief Copy from one string to another
 *
 * @param[in] dest  destination string
 * @param[in] src   source string
 * @param[in] bytes limit of num bytes to copy
 *
 * @retval    0     returns the initial value of dest
 */
static inline char *
__adf_os_str_ncopy(char *dest, const char *src, a_uint32_t bytes)
{   
    return strncpy(dest, src, bytes);
}

/**
 * @brief Returns the length of a string
 *
 * @param[in] str   input string
 *
 * @retval    length of string
 */
static inline adf_os_size_t
__adf_os_str_len(const char *str)
{   
    return strlen(str);
}

#endif /*ADF_OS_MEM_PVT_H*/
