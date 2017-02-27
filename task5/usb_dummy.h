#ifndef USB_DUMMY_H
#define USB_DUMMY_H

static int hello_hid_probe(struct usb_interface *, 
			const struct usb_device_id *); 
static void hello_hid_disconnect(struct usb_interface *);

#endif
