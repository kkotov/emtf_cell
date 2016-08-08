#ifndef _UTCA_DRV_H
#define _UTCA_DRV_H

#ifdef __GNUC__
#define VARIABLE_IS_NOT_USED __attribute__ ((unused))
#else
#define VARIABLE_IS_NOT_USED
#endif

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/capability.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/ioctl.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/random.h>
#include <linux/spinlock.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#include "utca_sp12.h"

#define DRV_MODULE_NAME   "utca_sp12"
#define PFX DRV_MODULE_NAME ": "
#define DRV_MODULE_VERSION  "1.00"
#define DRV_MODULE_RELDATE  "Oct 10, 2012"

static char VARIABLE_IS_NOT_USED version[] __devinitdata =
DRV_MODULE_NAME ".c:v" DRV_MODULE_VERSION " (" DRV_MODULE_RELDATE ")";


#define RUN_AT(x) (jiffies + (x))

extern UINT utca_sp12_major; // Dynamic allocation of MAJOR number
extern UINT utca_sp12_debug; // Debugging info
extern UINT utca_sp12_found;
extern UINT utca_sp12_nr_devs;
extern char utca_sp12_string[255];
extern UINT utca_sp12_hostid;
extern UINT utca_sp12_settime; // 200ms
extern UINT utca_sp12_update;   // 20ms
extern UINT utca_sp12_enable_dma;
extern UINT utca_sp12_policy;
extern UINT utca_sp12_skip_mod_init;
extern UINT utca_sp12_disabled_list[];
extern UINT utca_sp12_rampdown_set_delay;
extern UINT start_delay;  // 1secs timers start delay
extern ULONG exectime;
extern UINT busy;
extern UINT do_detect;
extern UINT ramp_cnt;
extern UINT sum_status;

extern spinlock_t utca_sp12_lock;


//  ==> PCI Initialization and cleanup
extern struct pci_driver utca_sp12_driver;
// <==

//  ==> HV misc support functions
//  -- Loopback Test
unsigned long loopback_test(utca_sp12_card *card);
//  -- Memory I/O performance Test
unsigned long memperf_test(utca_sp12_card *card);
//  -- Start DMA transfer
inline void start_dma_transfer(int card);

// ==> File operations
int utca_sp12_open (struct inode *inode, struct file *filp);
int utca_sp12_release(struct inode *inode, struct file *filp);
ssize_t utca_sp12_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t utca_sp12_write(struct file *filp, const char *buf, size_t count,loff_t *f_pos);
int utca_sp12_ioctl(struct inode *inode, struct file *filp, UINT cmd, ULONG arg);

extern struct file_operations utca_sp12_fops;
// <==


// Data structure for DMA transfer handling
typedef struct utca_dma {
   dma_addr_t           dma_addr;
   UINT*                buf;
} utca_dma;

typedef union
{
  unsigned long longword;
  unsigned char bytes[4];
} union_rnd_data;


extern utca_sp12_card **hostcard;  // one utca card entry

#endif
