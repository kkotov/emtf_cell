/* utca_sp12 UF PCIe microTCA SP Card device driver.
*
* Copyright (C) 2012 Victor Barashko (barvic@ufl.edu)
*/
#include "utca_sp12_drv.h"

static struct pci_device_id utca_sp12_pci_tbl[] =
{
  {
    PCI_VENDOR_ID_XILINX, PCI_DEVICE_ID_UTCA,
    PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0UL
  },
  { } // terminate list with empty entry
};

MODULE_DEVICE_TABLE(pci, utca_sp12_pci_tbl);

static int __devinit utca_sp12_init_one(struct pci_dev *pdev,
                                     const struct pci_device_id *ent)
{
  static int utca_sp12_version_printed = 0;
  ULONG utcareg_base[2], utcareg_len[2], pci_flags;
  UINT *data = NULL;
  struct utca_sp12_card *dev;
  int err,i = 0;
  int revision;
  int rmask;
  u64 reqmask;
  int pres;

  if (utca_sp12_version_printed++ == 0)
    printk(KERN_INFO "%s installed\n", version);


  pci_read_config_dword(pdev, PCI_REVISION_ID, &revision);
  revision &= 0xFF;
  if (utca_sp12_debug)
    {
      printk(KERN_INFO PFX "vendor: 0x%X, device: 0x%X\n", pdev->vendor, pdev->device);
      printk(KERN_INFO PFX "bus: %i (%s)\n", pdev->bus->number,pdev->bus->name);
      printk(KERN_INFO PFX "slot: %s\n", pci_name(pdev));
      printk(KERN_INFO PFX "devfn: 0x%0X\n", pdev->devfn);
      printk(KERN_INFO PFX "revision: %i\n", revision);
    }

  err = pci_enable_device(pdev);
  if (err)
    {
      printk(KERN_ERR PFX "Can not enable PCI device, "
             "aborting.\n");
      return err;
    }

  if (!(pci_resource_flags(pdev, 0) & IORESOURCE_MEM))
    {
      printk(KERN_ERR PFX "Unable to find proper PCI device "
             "base address, aborting.\n");
      err = -ENODEV;
      goto err_out_disable_pdev;
    }

  err = pci_request_regions(pdev, DRV_MODULE_NAME);
  if (err)
    {
      printk(KERN_ERR PFX "Unable to obtain PCI resources, "
             "aborting.\n");
      goto err_out_disable_pdev;
    }

  pci_set_master(pdev);

  for (pres = 0; pres < 2; pres++)
    {
      pci_flags = pci_resource_flags(pdev, pres);
      utcareg_base[pres] = pci_resource_start(pdev, pres);
      utcareg_len[pres] = pci_resource_len(pdev, pres);
      if (pci_flags && IORESOURCE_MEM) 
	printk(KERN_INFO PFX "PCI:%d:%d Memory region start: %08X, length: %d\n", 
	       pdev->bus->number, pdev->devfn>>3, utcareg_base[pres], utcareg_len[pres]);
    }

  dev = kmalloc(sizeof(struct utca_sp12_card), GFP_ATOMIC);
  if (!dev)
    {
      printk(KERN_ERR PFX "HVCard device data structure memory allocation failed, aborting.\n");
      err = -ENOMEM;
      goto err_out_free_res;
    }
  memset(dev, 0, sizeof(*dev));
  dev->pdev = pdev;
  dev->bus = pdev->bus->number;
  dev->slot = pdev->devfn>>3;
  dev->revision = revision;

  dev->ioaddr[0] = (ULONG) ioremap_wc(utcareg_base[0], utcareg_len[0]); // memory 
  //dev->ioaddr[1] = (ULONG) ioremap   (utcareg_base[1], utcareg_len[1]); // registers
  dev->ioaddr[1] = (ULONG) pci_iomap(pdev, 1, utcareg_len[1]);

  if (dev->ioaddr[0] == 0UL || dev->ioaddr[1] == 0UL)
    {
      printk(KERN_ERR PFX "Unable to map device registers, "
             "aborting.\n");
      err = -ENOMEM;
      goto err_out_free_dev;
    }

  if (utca_sp12_debug)
    {
      for (pres = 0; pres < 2; pres++)
	printk(KERN_INFO PFX "%ld bytes of I/O Memory at 0x%08lX mapped to 0x%08lX\n", 
	       utcareg_len[pres], utcareg_base[pres], dev->ioaddr[pres]);
    }

  dev->dataslot = NA;
  for (i=0; i<utca_sp12_nr_devs; i++)
    {
      if (!hostcard[i]->pdev)
        {
          dev->dataslot = i;
          break;
        }
    }

  if (dev->dataslot == NA)
    {
      printk(KERN_ERR PFX "Unable to allocate data slot for device pci:%d:%d. "
             "Check SLOTS parameter in /etc/utca_sp12.cfg\n", dev->bus, dev->slot);
      err = -ENOMEM;
      goto err_out_iounmap;
    }

  // Allocate for utca_dma structure
  dev->dma = kmalloc(sizeof(struct utca_dma), GFP_ATOMIC);
  if (!dev->dma)
    {
      printk(KERN_ERR PFX "dataslot#%d DMA alloc failed, aborting.\n", dev->dataslot);
      err = -ENOMEM;
      goto err_out_iounmap;
    }

  reqmask = dma_get_required_mask(pdev);
  printk(KERN_INFO PFX "required mask: %llX\n", reqmask);

  rmask = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(32));
  printk(KERN_INFO PFX "pci_set_consistent_dma_mask returned %d\n", rmask);

  // Allocate buffer for DMA transfers
  dev->dma->buf = pci_alloc_consistent(pdev, DMA_BUF_SIZE, &(dev->dma->dma_addr));
  if (!dev->dma->buf)
    {
      printk(KERN_ERR PFX "dataslot#%d DMA buffer pci_alloc_consistent failed, aborting.\n", dev->dataslot);
      err = -ENOMEM;
      goto err_out_dma;
    }

  printk(KERN_INFO PFX "dma buf: %llX dma_addr: %llX\n", dev->dma->buf, dev->dma->dma_addr);


  // Initial DMA reading
  //  writel(dev->dma->dma_addr,(volatile void *)(dev->ioaddr+DMA_INIT_OFFSET));
  //  udelay(10000);

  //utca_sp12_init_modules(dev);
    memcpy(hostcard[dev->dataslot], dev, sizeof(*dev));
    //    loopback_test(hostcard[dev->dataslot]);
 //   memperf_test(hostcard[dev->dataslot]);
  //  if (!utca_sp12_skip_mod_init) utca_sp12_init_modules(hostcard[dev->dataslot]);

  if (utca_sp12_debug)
    printk(KERN_INFO PFX "data slot:%i allocated\n",dev->dataslot);

  utca_sp12_found++;
  printk(KERN_INFO PFX "pci:%d:%d dataslot:%d rev.%d %s mode installed\n", 
         dev->bus, dev->slot, dev->dataslot, dev->revision, (dev->revision>1 && utca_sp12_enable_dma)?"dma":"non-dma");
  /*
  spin_lock_init(&bp->lock);

  */
  pci_set_drvdata(pdev, hostcard[dev->dataslot]);
  kfree(dev);
  return 0;


err_out_dma:
  kfree(dev->dma);
err_out_iounmap:
  iounmap((void *) dev->ioaddr);
err_out_free_dev:
  kfree(dev);
err_out_free_res:
  pci_release_regions(pdev);
err_out_disable_pdev:
  pci_set_drvdata(pdev, NULL);
  pci_disable_device(pdev);
  return err;

}

static void __devexit utca_sp12_remove_one(struct pci_dev *pdev)
{
  struct utca_sp12_card *dev = pci_get_drvdata(pdev);
  UINT dataslot, bus, slot;
  if (dev!=NULL)
    {
      /* Set All data to 0 */
      // utca_sp12_init_modules(dev);
      pci_free_consistent(pdev, MAX_HVDATA_SIZE, dev->dma->buf, dev->dma->dma_addr);
      kfree(dev->dma);
      iounmap((void *) dev->ioaddr[0]);
      iounmap((void *) dev->ioaddr[1]);
      dataslot = dev->dataslot;
      bus = dev->bus;
      slot = dev->slot;
      if ((dev->dataslot >=0) && (dev->dataslot < utca_sp12_nr_devs))
        {
          memset(hostcard[dev->dataslot],0,sizeof(*dev));
          if (utca_sp12_debug)
            printk(KERN_INFO PFX "data slot:%i released\n", dev->dataslot);
        }
      else
        {
          printk(KERN_ERR PFX "invalid data slot %i\n", dev->dataslot);
        }
      printk(KERN_INFO PFX "pci:%d:%d dataslot:%d removed\n", bus, slot, dataslot);

    }
  pci_release_regions(pdev);
  pci_disable_device(pdev);
  pci_set_drvdata(pdev, NULL);
  //    printk(KERN_INFO PFX "removed\n");
  utca_sp12_found--;
}


struct pci_driver utca_sp12_driver =
{
  .name   = DRV_MODULE_NAME,
  .id_table = utca_sp12_pci_tbl,
  .probe    = utca_sp12_init_one,
  .remove   = __devexit_p(utca_sp12_remove_one),
};

