#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

/*
 * Use USB_INTERFACE_INFO macro to populate our usb_device "object".
 */

static struct usb_device_id info_table [] = {
	{USB_INTERFACE_INFO
	  (USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
	   USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{},
};

/*
 * Add ourselves to the usb device list.
 */

MODULE_DEVICE_TABLE(usb,info_table);

/*
 * These will never be called.  Sad panda is sad.
 */

int usb_dummy_hid_init(void)
{
	pr_debug("Loading dummy HID(Keyboard) module\n"); 

	return 0;
}

/* 
 * All good things must come to an end 
 */

void usb_dummy_hid_cleanup(void)
{
	pr_debug("I hate to see you leave, but...\n");
}

module_init(usb_dummy_hid_init);
module_exit(usb_dummy_hid_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("1be5456d23a8");
MODULE_DESCRIPTION("Simple USB hotplug module for keyboards");
