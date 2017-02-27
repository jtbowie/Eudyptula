#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>
#include "usb_dummy.h"

/*
 * Use USB_INTERFACE_INFO macro to populate our usb_device "object".
 */

static struct usb_device_id info_table [] = {
	{USB_INTERFACE_INFO
	  (USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
	   USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{},
};

static struct usb_driver usb_dummy_hid_drv = {
	.name =		"usb_dummy_hid",
	.probe =	usb_dummy_hid_probe,
	.disconnect = 	usb_dummy_hid_disconnect,
	.id_table =	info_table,
};

/*
 * Add ourselves to the usb device list.
 */

MODULE_DEVICE_TABLE(usb,info_table);

/*
 * These will never be called.  Sad panda is sad.
 */

static int usb_dummy_hid_probe(struct usb_interface *intf, 
		const struct usb_device_id *id)
{
	pr_alert("Found a Keyboard!  Hello World!\n");
	return 0;
}

static void usb_dummy_hid_disconnect(struct usb_interface *intf)
{
	pr_alert("Hey!@@#$ Come back here :(\n");
}

int usb_dummy_hid_init(void)
{
	int ret;

	pr_debug("Loading dummy HID(Keyboard) module\n"); 
	ret = usb_register(&usb_dummy_hid_drv);
	if (ret)
		pr_warn("usb_register failed.  Error number %d", ret);

	return ret;
}

/* 
 * All good things must come to an end 
 */

void usb_dummy_hid_cleanup(void)
{
	pr_debug("I hate to see you leave, but...\n");
	usb_deregister(&usb_dummy_hid_drv);
}

module_init(usb_dummy_hid_init);
module_exit(usb_dummy_hid_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("1be5456d23a8");
MODULE_DESCRIPTION("Simple USB hotplug module for keyboards");
