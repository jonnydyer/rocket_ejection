import usb.core
from usb.util import *
import binascii
import time
import struct
from math import *

channels = (
    ('Time', 'I', 'sec', 1e-2),
    ('Temperature', 'i', 'C', 1e-2),
    ('Pressure', 'i', 'Pa', 1e0),
    ('Accel (X-Hi)', 'h', 'g', 49e-3/16),
    ('Accel (Y-Hi)', 'h', 'g', 49e-3/16),
    ('Accel (Z-Hi)', 'h', 'g', 49e-3/16),
    ('Accel (X-Lo)', 'h', 'g', 7.32e-4),
    ('Accel (Y-Lo)', 'h', 'g', 7.32e-4),
    ('Accel (Z-Lo)', 'h', 'g', 7.32e-4),
    ('Mag (X)', 'h', 'gauss', 1.6e-4),
    ('Mag (Y)', 'h', 'gauss', 1.6e-4),
    ('Mag (Z)', 'h', 'gauss', 1.6e-4),
)

def parse_all(fmt, buff):
    '''
    Applys struct.unpack_from on a buffer iteratively
    until there is insufficient data to read
    '''
    fmt_size = struct.calcsize(fmt)
    N = int(floor(len(buff) / fmt_size))

    packets = []
    for i in range(N):
        packets.append(struct.unpack_from(fmt, buff, i*fmt_size))

    return packets


class RocketTimerUsb(object):
    def __init__(self, vendor_id=0x16c0, product_id=0x05dc):
        self.struct_str = '>'
        for c in channels:
            self.struct_str += c[1]

        self.struct_len = struct.calcsize(self.struct_str)

        self.dev = usb.core.find(idVendor=vendor_id, idProduct=product_id)

        if self.dev is None:
            raise ValueError('Device not found!')

        print 'Found Device!'

        self.dev.set_configuration()

    def __repr__(self):
        return self.dev

    def set_led(self, on):
        ret = self.dev.ctrl_transfer(CTRL_TYPE_VENDOR | CTRL_RECIPIENT_DEVICE | CTRL_OUT, 1,
                        int(on), 0, [])
    def custom_rq_get_bytes(self, rq_id, indx=0, val=0, nmax=20):
        return self.dev.ctrl_transfer(CTRL_TYPE_VENDOR | CTRL_RECIPIENT_DEVICE | CTRL_IN,
                                rq_id, val, indx, nmax*self.struct_len)

    def get_packets(self):
        ret = self.custom_rq_get_bytes(3)
        if len(ret) > 0:
            return parse_all(self.struct_str, ret)
        else:
            return None


def main():
    rtu = RocketTimerUsb()
    #msg = 'test'
    #wValue = 0xcc
    #wIndex = 0x55
    #ret = dev.ctrl_transfer(CTRL_TYPE_VENDOR | CTRL_RECIPIENT_DEVICE | CTRL_IN, 0,
    #                        wValue, wIndex, msg)

    print 'Reading device'
    rtu.set_led(True)

    while(1):
        ttp = rtu.get_packets()
        if ttp != None:
            for x in ttp:
                print x
        # print binascii.hexlify(ret)
        time.sleep(0.05)

    rtu.set_led(0)
    print 'Done'

if __name__ == "__main__": main()
