#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

/* 
 * Init our module. Constructor Power! 
 */

static struct usb_device_id info_table [] = {
	{USB_INTERFACE_INFO
	  (USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
	   USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{},
};

static struct usb_driver hello_hid_drv = {
	.owner =	THIS_MODULE,
	.name = 	"hello_hid"
	.probe =	hid_probe,
	.disconnect =	hid_disconnect,
	.id_table = 	info_table,
};

MODULE_DEVICE_TABLE(usb,info_table);

static int hid_probe(struct usb_interface *intf, 
		const struct usb_device_id *id)
{
	pr_notice("Found a Keyboard!  Hello world!\n");
	return 0;
}

static void hid_disconnect(struct usb_interface *intf)
{
	pr_notice("Hey!@@#$ Come back here :(\n");
}

int init_module(void)
{
	int ret;

	printk(KERN_DEBUG "Loading dummy HID module\n"); 
	ret = usb_register(&hello_hid_drv);
	if (ret)
		pr_warn("usb_register failed.  Error number %d", ret);

	return ret;
}

/* 
 * All good things must come to an end 
 */

void cleanup_module(void)
{
	printk(KERN_DEBUG "I hate to see you leave, but...\n");
	usb_deregister(&hello_hid_drv);
}
