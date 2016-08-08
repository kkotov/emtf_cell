/* utca_sp12 UF PCIe microTCA SP Card device driver.
*
* Copyright (C) 2012 Victor Barashko (barvic@ufl.edu)
*/
#include "utca_sp12_drv.h"
#include <linux/timex.h>

struct file_operations utca_sp12_fops =
{
  .owner        = THIS_MODULE,
  .read         = utca_sp12_read,
  .write        = utca_sp12_write,
  .open         = utca_sp12_open,
  .release      = utca_sp12_release
};

static unsigned long read_cycles = 0;
static unsigned long sumT = 0;

int utca_sp12_open (struct inode *inode, struct file *filp)
{
  UINT dev = MINOR(inode->i_rdev);

  if ((dev >= utca_sp12_nr_devs) || (hostcard[dev]->pdev == 0) )
    return -ENODEV;

  filp->f_op = &utca_sp12_fops;
  filp->private_data = hostcard[dev];
  if (utca_sp12_debug)
    printk (KERN_INFO PFX "data slot:%i is opened by "
            "\"%s\" (pid %i)\n",dev, current->comm, current->pid);
  return 0;
}

int utca_sp12_release(struct inode *inode, struct file *filp)
{
  UINT dev = MINOR(inode->i_rdev);

  if ((dev >= utca_sp12_nr_devs) || (hostcard[dev]->pdev == 0) )
    return -ENODEV;

  filp->private_data = NULL;
  if (utca_sp12_debug)
    printk (KERN_INFO PFX "data slot:%i is released by "
            "\"%s\" (pid %i)\n", dev, current->comm, current->pid);
  return 0;
}



ssize_t utca_sp12_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
  ssize_t ret = 0;
  UINT *data = NULL;
  uint64_t ioaddr_reg = 0, ioaddr_mem = 0;
  utca_sp12_card *dev = NULL;
  dma_addr_t bmaddr;
  unsigned long avgT = 0;
  unsigned long read_thresh = 5000;
  struct timespec ts0, ts1;
  int i;

  uint32_t treg;

  if (!filp->private_data)
    {
      printk(KERN_ERR PFX "Cannot find data slot\n");
      return -EFAULT;
    }
  dev = filp->private_data;
  ioaddr_reg = dev->ioaddr[1]; 
  ioaddr_mem = dev->ioaddr[0];

  data = kmalloc(count, GFP_KERNEL);
  if (!data)
    {
      printk (KERN_ERR PFX "Failed to alloc read data buffer\n");
      return -ENOMEM;
    }

  if (count == 4)
    {
      // single register read
      treg = readl((uint32_t *)(ioaddr_reg + f_pos));
    }
  else
    {
      // memory access
    }


  // write beam length, in 32-bit words
  writel((count >> 2), (uint32_t *)(ioaddr_reg + 0x18)); 

  bmaddr = dev->dma->dma_addr; // physical memory address for dma transfer
  
  preempt_disable();
  ktime_get_ts(&ts0);	// HRT high res timer API

  // command to beam data
  writel(bmaddr, (uint32_t *)(ioaddr_reg + 4));

  // Now read beam complete register. 
  // readl will return when beaming is done.

  treg = readl((uint32_t *)(ioaddr_reg + 0x14));

  ktime_get_ts(&ts1);

/*
  ktime_get_ts(&ts0);  // single read latency measurement
  treg = readl((uint32_t *)(ioaddr_reg + 0x14));
  ktime_get_ts(&ts1);
*/

  preempt_enable();
  read_cycles++;

  sumT += (ts1.tv_sec - ts0.tv_sec)*1000000000 + (ts1.tv_nsec - ts0.tv_nsec);
   
  if ( ((read_cycles/read_thresh)>0) && ((read_cycles%read_thresh)==0) ) 
   {
       avgT = sumT / read_cycles;
       printk(KERN_INFO PFX "%d read cycles, average read latency: %ld nano-sec\n", read_cycles, avgT);
       read_cycles = 0;
       sumT = 0;
   }

  // check that beam complete register returned what it's supposed to
  if (treg != 0xbabeface)
    {
      printk(KERN_ERR PFX "beam complete register mismatch: %08x\n", treg);
    }


  ret = count;
  if (copy_to_user(buf, dev->dma->buf, count))
    {
      printk (KERN_ERR PFX "Failed to copy data to user space\n");
      ret = -EFAULT;
    }

  kfree(data);
  return ret;
}

ssize_t utca_sp12_write(struct file *filp, const char *buf, size_t count,loff_t *f_pos)
{
  ssize_t ret = 0;
  uint32_t *data = NULL;
  uint64_t ioaddr_mem = 0;
  utca_sp12_card *dev = NULL;
  int i;

  if (!filp->private_data)
    {
      printk(KERN_ERR PFX "Cannot find data slot\n");
      return -EFAULT;
    }
  dev = filp->private_data;
  ioaddr_mem = dev->ioaddr[0] + 0x40000;

  data  = kmalloc(count, GFP_KERNEL);
  if (!data)
    {
      printk (KERN_ERR PFX "Failed to alloc write data buffer\n");
      return -ENOMEM;
    }

  ret = count;
  if (copy_from_user(data, buf, count))
    {
      printk (KERN_ERR PFX "Failed to copy data from user space\n");
      ret = -EFAULT;
    }

  // write data to board

  // using writeq transfers here to make sure all data packages are 64-bit aligned
  // root complex combines single writes into block transfers
  for (i = 0; i < count/8; i++)
    {
      writeq(((uint64_t*)data)[i], ((uint64_t *)ioaddr_mem) + i);
    }

  kfree(data);
  return ret;
}
