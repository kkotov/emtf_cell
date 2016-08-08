/* utca_sp12 UF PCIe microTCA SP Card device driver.
*
* Copyright (C) 2012 Victor Barashko (barvic@ufl.edu)
*/
#include "utca_sp12_drv.h"
#include "linux/jiffies.h"


unsigned long loopback_test(utca_sp12_card *hostcard)
{
    unsigned int res=0;
    unsigned int ret=0;
    int i;
    union_rnd_data rnd;

    return res;

    // if (hostcard->revision >= 1 /* && utca_sp12_enable_dma == 1 */)
        {
	    printk(KERN_INFO PFX "Performing Loopback test...\n" );


            for (i=0; i<32; i++)
                {
                    writel(1<<i, (unsigned int *) (hostcard->ioaddr+LOOPBACK_ADDR));
                    ret = readl((unsigned int *)(hostcard->ioaddr+LOOPBACK_ADDR));
                    // res |= (~ret)^(1<<i);
                    res |= (ret)^(1<<i);
                }

            if (res) printk(KERN_INFO PFX "Loopback test Running 1's failed 0x%X at hostcard %d\n", res, hostcard->dataslot);

            res = 0;
            for (i=0; i<1000; i++)
                {
                    get_random_bytes(rnd.bytes, 4);
                    writel(rnd.longword, (unsigned int *)(hostcard->ioaddr+LOOPBACK_ADDR));
                    ret = readl((unsigned int *)(hostcard->ioaddr+LOOPBACK_ADDR));
                    // if ((ret%200)==0) printk(KERN_INFO PFX "Random Number - %lX\n", ret);
                    // res |= (~ret)^(rnd.longword);
                    res |= (ret)^(rnd.longword);
                }
            if (res) printk(KERN_INFO PFX "Loopback test Random Numbers failed 0x%X at hostcard %d\n", res, hostcard->dataslot);


        }

    return res;
}

unsigned long memperf_test(utca_sp12_card *hostcard)
{
    unsigned int res=0;
    size_t count = 2048;
    int ncycles=100;
    UINT* data = NULL;
    size_t i;
    union_rnd_data rnd;
    unsigned long t0, t1;

    printk(KERN_INFO PFX "Memory I/O performance test for hostcard %d\n", hostcard->dataslot);

    return res;

    data  = kmalloc(count, GFP_KERNEL);
    if (!data)
        {
            printk (KERN_ERR PFX "Failed to alloc write data buffer\n");
        }
    else
        {
            memset(data, 0, count);

            for (i=0; i<count/sizeof(UINT); i++)
                {
                    get_random_bytes(rnd.bytes, 4);
                    data[i] = rnd.longword;
                }

 	    t0 = jiffies;

	    // writel(0, (unsigned int *) (hostcard->ioaddr));

	   //  return res;

	    for (i=0; i<ncycles; i++) {
	      memcpy_toio((uint32_t *)(hostcard->ioaddr), data, count);
	      readl((unsigned int *)(hostcard->ioaddr));
 	    }

	    // return res;

            t1 = jiffies;
            printk(KERN_INFO PFX "%d Write cycles time: %ld msec, rate: %ld KB/sec\n", ncycles, (t1-t0)*1000/HZ, 
		(ncycles*count)*HZ/(1024*(t1-t0)));

	    t0 = jiffies;
            for (i=0; i<ncycles; i++) {
              memcpy_fromio(data, (uint32_t *)(hostcard->ioaddr), count);
            }
            t1 = jiffies;
            printk(KERN_INFO PFX "%d Read cycles time: %ld msec, rate: %ld KB/sec\n", ncycles, (t1-t0)*1000/HZ, 
		(ncycles*count)*HZ/(1024*(t1-t0)));

	    t0 = jiffies;
            for (i=0; i<ncycles; i++) {
              memcpy_toio((uint32_t *)(hostcard->ioaddr), data, count);
              memcpy_fromio(data, (uint32_t *)(hostcard->ioaddr), count);
            }
            t1 = jiffies;
            printk(KERN_INFO PFX "%d Read/Write cycles time: %ld msec\n", ncycles, (t1-t0)*1000/HZ);


	    memset(data, 0, count);
            memcpy_toio((uint32_t *)(hostcard->ioaddr), data, count);
            kfree(data);

        }


    return res;
}

void start_dma_transfer(int card)
{
    if ((card < utca_sp12_found) && hostcard[card]->revision >= 2 && utca_sp12_enable_dma == 1)
        {
            memset(hostcard[card]->dma->buf, 0, DMA_BUF_SIZE);
            writel(hostcard[card]->dma->dma_addr,(UINT *)(hostcard[card]->ioaddr+DMA_INIT_OFFSET));
        }
}
