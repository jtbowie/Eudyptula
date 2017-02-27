#ifndef USB_DUMMY_H
#define USB_DUMMY_H

static int usb_dummy_hid_probe(struct usb_interface *, 
			const struct usb_device_id *); 
static void usb_dummy_hid_disconnect(struct usb_interface *);

#endif
