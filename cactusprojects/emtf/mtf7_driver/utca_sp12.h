#ifndef _UTCA_H
#define _UTCA_H

#ifdef __GNUC__
#define VARIABLE_IS_NOT_USED __attribute__ ((unused))
#else
#define VARIABLE_IS_NOT_USED
#endif

#include <linux/ioctl.h>
#include "types.h"


#define PCI_VENDOR_ID_XILINX    0x5546 // Vendor ID (UF)
#define PCI_DEVICE_ID_UTCA      0x4d54 // uTCA Device ID 
#define MAX_HVDATA_SIZE 	3072   // Maximum data buffer size in bytes for host card 
#define NA                      -1

#define DMA_BUF_SIZE            0x40000
#define DMA_INIT_OFFSET         0xFFC
#define LOOPBACK_ADDR           0x0

#define UTCA_IOC_MAGIC        'H'

#define UTCA_IOCRESET 	_IO(UTCA_IOC_MAGIC,0)
#define UTCA_IOQNRDEVS        _IO(UTCA_IOC_MAGIC,1)
#define UTCA_IOQDEVSFOUND     _IO(UTCA_IOC_MAGIC,2)
#define UTCA_IOQDEVBUS        _IO(UTCA_IOC_MAGIC,4)
#define UTCA_IOTDEBUG         _IO(UTCA_IOC_MAGIC,5)
#define UTCA_IOQDEBUG		_IO(UTCA_IOC_MAGIC,6)
#define UTCA_IOQIOADDR        _IO(UTCA_IOC_MAGIC,7)
#define UTCA_IOTSETTIME	_IO(UTCA_IOC_MAGIC,8)
#define UTCA_IOTUPDATE	_IO(UTCA_IOC_MAGIC,9)

#define UTCA_IOC_MAXNR	9	


typedef struct utca_sp12_card {
   struct pci_dev       *pdev;
   struct utca_dma         *dma;
   UINT                 revision;
   ULONG                ioaddr[2];
   UINT                 bus;
   UINT                 slot;
   UINT                 dataslot;
} utca_sp12_card;


#endif /* _UTCA_H */
