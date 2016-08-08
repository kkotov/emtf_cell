/* utca_sp12 UF PCIe microTCA SP Card device driver.
*
* Copyright (C) 2012 Victor Barashko (barvic@ufl.edu)
*/
#include "utca_sp12_drv.h"

int utca_sp12_ioctl(struct inode *inode, struct file *filp, UINT cmd, ULONG arg)
{
  int err = 0;
  int ret = 0;
  utca_sp12_card *dev = NULL;
  UINT dataslot = MINOR(inode->i_rdev);

  if (!filp->private_data)
    {
      printk(KERN_ERR PFX "Cannot find data slot\n");
      return -EFAULT;
    }
  dev = filp->private_data;

  if ((dataslot >= utca_sp12_nr_devs) || (hostcard[dataslot]->pdev == 0) || (dataslot != dev->dataslot))
    return -ENODEV;

  if (_IOC_TYPE(cmd) != UTCA_IOC_MAGIC)
    {
      if (utca_sp12_debug)
        printk(KERN_ERR PFX "Invalid ioctl type %d\n",_IOC_TYPE(cmd));
      return -ENOTTY;
    }
  if (_IOC_NR(cmd) > UTCA_IOC_MAXNR)
    {
      if (utca_sp12_debug)
        printk(KERN_ERR PFX "Invalid ioctl command %d\n",_IOC_NR(cmd));
      return -ENOTTY;
    }
  if (_IOC_DIR(cmd) & _IOC_READ)
    err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
  else if (_IOC_DIR(cmd) & _IOC_WRITE)
    err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
  if (err) return -EFAULT;

  switch (cmd)
    {
    case UTCA_IOCRESET:
      break;
    case UTCA_IOQNRDEVS:
      return utca_sp12_nr_devs;
    case UTCA_IOQDEVSFOUND:
      return utca_sp12_found;
    case UTCA_IOQDEVBUS:
      return dev->pdev->bus->number;
    case UTCA_IOTDEBUG:
      if (!capable (CAP_SYS_ADMIN))
        return -EPERM;
      utca_sp12_debug = arg;
      printk(KERN_INFO PFX "Set utca_sp12_debug to %i\n", utca_sp12_debug);
      break;
    case UTCA_IOQDEBUG:
      return utca_sp12_debug;
    case UTCA_IOQIOADDR:
      return dev->ioaddr[1];
    case UTCA_IOTSETTIME:
      if (!capable (CAP_SYS_ADMIN))
        return -EPERM;
      utca_sp12_settime = arg;
      printk(KERN_INFO PFX "utca set time delay is %ims\n", utca_sp12_settime);
      break;
    case UTCA_IOTUPDATE:
      if (!capable (CAP_SYS_ADMIN))
        return -EPERM;
      utca_sp12_update = arg;
      printk(KERN_INFO PFX "utca set update delay is %ims\n", utca_sp12_settime);
      break;
    default:
      return -ENOTTY;
    }
  return ret;
}

