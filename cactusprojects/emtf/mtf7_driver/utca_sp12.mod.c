#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x14522340, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x4f1939c7, "per_cpu__current_task" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0x1e0c2be4, "ioremap_wc" },
	{ 0x6980fe91, "param_get_int" },
	{ 0xd2037915, "dev_set_drvdata" },
	{ 0xd691cba2, "malloc_sizes" },
	{ 0xa30682, "pci_disable_device" },
	{ 0xfa0d49c7, "__register_chrdev" },
	{ 0xd3364703, "x86_dma_fallback_dev" },
	{ 0x102b9c3, "pci_release_regions" },
	{ 0xff964b25, "param_set_int" },
	{ 0xf8905e86, "dma_get_required_mask" },
	{ 0xaf559063, "pci_set_master" },
	{ 0xde0bdcff, "memset" },
	{ 0xea147363, "printk" },
	{ 0x85f8a266, "copy_to_user" },
	{ 0xb4390f9a, "mcount" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x6dcaeb88, "per_cpu__kernel_stack" },
	{ 0x7dceceac, "capable" },
	{ 0x78764f4e, "pv_irq_ops" },
	{ 0xc5aa6d66, "pci_bus_read_config_dword" },
	{ 0x68f7c535, "pci_unregister_driver" },
	{ 0x2044fa9e, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0x6d090f30, "pci_request_regions" },
	{ 0xedc03953, "iounmap" },
	{ 0x5f07b9f3, "__pci_register_driver" },
	{ 0x74ae34c9, "pci_iomap" },
	{ 0xa12add91, "pci_enable_device" },
	{ 0xb02504d8, "pci_set_consistent_dma_mask" },
	{ 0x3302b500, "copy_from_user" },
	{ 0xa92a43c, "dev_get_drvdata" },
	{ 0x6e9681d2, "dma_ops" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v00005546d00004D54sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "39CB60CB381B8189DA20DE6");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 6,
};
