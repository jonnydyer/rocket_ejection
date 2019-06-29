import usb.core
import usb.util

# find our device
dev = usb.core.find(idVendor=0x16c0, idProduct=0x05dc)

# was it found?
if dev is None:
    raise ValueError('Device not found')

# set the active configuration. With no arguments, the first
# configuration will be the active one
dev.set_configuration()

dev.ctrl_transfer(0x01, )

