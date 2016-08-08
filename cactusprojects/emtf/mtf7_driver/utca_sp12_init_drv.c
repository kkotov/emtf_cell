/* utca_sp12 UF PCIe microTCA SP Card device driver.
*
* Copyright (C) 2012 Victor Barashko (barvic@ufl.edu)
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/version.h>

#include "utca_sp12_drv.h"

UINT utca_sp12_major = 0; // Dynamic allocation of MAJOR number
UINT utca_sp12_debug = 1; // Debugging info
UINT utca_sp12_found=0;
UINT utca_sp12_nr_devs = 6;
char utca_sp12_string[255];
UINT utca_sp12_hostid = 0;
UINT utca_sp12_settime=200; // 200ms
UINT utca_sp12_update=20;   // 20ms
UINT utca_sp12_enable_dma = 1;
UINT busy=0;
UINT do_detect = 0;
UINT sum_status = 0;
spinlock_t utca_sp12_lock = SPIN_LOCK_UNLOCKED;

utca_sp12_card **hostcard;  // one hostcard entry
UINT *buf;

module_param(utca_sp12_debug, int, 0);
module_param(utca_sp12_hostid, int, 0);
module_param(utca_sp12_nr_devs, int, 0);

module_param(utca_sp12_settime, int, 0);
module_param(utca_sp12_update, int, 0);
module_param(utca_sp12_enable_dma, int, 0);

MODULE_AUTHOR("Victor Barashko (barvic@ufl.edu)");
MODULE_DESCRIPTION("UTCA Card PCI-e driver");
MODULE_LICENSE("GPL");

static int __init utca_sp12_init(void)
{
  int i, result, datasize=0;

  result = register_chrdev(utca_sp12_major, DRV_MODULE_NAME, &utca_sp12_fops);
  if (result < 0)
    {
      printk(KERN_WARNING PFX "Unable to get major %d\n",utca_sp12_major);
      return result;
    }
  if (utca_sp12_major == 0) utca_sp12_major = result; /* dynamic */

  hostcard = kmalloc(utca_sp12_nr_devs*sizeof(ULONG), GFP_ATOMIC);
  if (!hostcard)
    {
      result = -ENOMEM;
      goto fail;
    }
  memset(hostcard, 0, utca_sp12_nr_devs*sizeof(ULONG));

  for (i=0; i< utca_sp12_nr_devs; i++)
    {
      hostcard[i] = kmalloc(sizeof(utca_sp12_card), GFP_ATOMIC);
      if (!hostcard[i])
        {
          result = -ENOMEM;
          while (i--) kfree(hostcard[i]);
          goto fail_hostcard;
        }
      memset(hostcard[i], 0, sizeof(utca_sp12_card));
      datasize += sizeof(utca_sp12_card);
    }

  if (utca_sp12_debug)
    {
      printk(KERN_INFO PFX "hostcard list size - %ld bytes\n", utca_sp12_nr_devs*sizeof(ULONG));
      printk(KERN_INFO PFX "all hostcards data - %d bytes\n", datasize);
    }


  /* Allocate static buffer for HV data from one hostcard */
  buf = kmalloc(MAX_HVDATA_SIZE, GFP_ATOMIC);
  if (!buf)
    {
      result = -ENOMEM;
      goto fail_buf;
    }
  memset(buf, 0, MAX_HVDATA_SIZE);

  result = pci_register_driver(&utca_sp12_driver);

  if (result == 0)
    {
      return result;
    } 
  else 
    {
	printk (KERN_ERR PFX "Unable to initialize PCI driver. Check for physical card(s) presense.\n");
    }
//   return result;

fail_hist:

fail_buf:
  for (i=0; i<utca_sp12_nr_devs; i++)
    {
      if (hostcard[i]) kfree(hostcard[i]);
    }
fail_hostcard:
  if (hostcard) kfree(hostcard);
fail:
  unregister_chrdev(utca_sp12_major, DRV_MODULE_NAME);
  return result;
}

static void __exit utca_sp12_cleanup(void)
{
  int i;
  unregister_chrdev(utca_sp12_major, DRV_MODULE_NAME);
  pci_unregister_driver(&utca_sp12_driver);

  if (buf)    kfree(buf);

  for (i=0; i<utca_sp12_nr_devs; i++)
    {
      if (hostcard[i]) kfree(hostcard[i]);
    }
  if (hostcard)   kfree(hostcard);
}

module_init(utca_sp12_init);
module_exit(utca_sp12_cleanup);

