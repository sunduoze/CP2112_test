#!/usr/bin/env python

from __future__ import print_function
import argparse
import string
import struct
import sys
from base64 import decode

import win32api
import win32file
import pywintypes

def CTL_CODE(DeviceType, Function, Method, Access):
    return (DeviceType << 16) | (Access << 14) | (Function << 2) | Method


def USB_CTL(id):
    # CTL_CODE(FILE_DEVICE_USB, (id), METHOD_BUFFERED, FILE_ANY_ACCESS)
    return CTL_CODE(0x22, id, 0, 0)


IOCTL_USB_GET_ROOT_HUB_NAME = USB_CTL(258)  # HCD_GET_ROOT_HUB_NAME
IOCTL_USB_GET_NODE_INFORMATION = USB_CTL(258)  # USB_GET_NODE_INFORMATION
IOCTL_USB_GET_NODE_CONNECTION_INFORMATION = USB_CTL(259)  # USB_GET_NODE_CONNECTION_INFORMATION
IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME = USB_CTL(264)  # USB_GET_NODE_CONNECTION_DRIVERKEY_NAME
IOCTL_USB_GET_NODE_CONNECTION_NAME = USB_CTL(261)  # USB_GET_NODE_CONNECTION_NAME
IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION = USB_CTL(260)  # USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION

USB_CONFIGURATION_DESCRIPTOR_TYPE = 2
USB_STRING_DESCRIPTOR_TYPE = 3
USB_INTERFACE_DESCRIPTOR_TYPE = 4
MAXIMUM_USB_STRING_LENGTH = 255


def open_dev(name):
    try:
        handle = win32file.CreateFile(name,
                                      win32file.GENERIC_WRITE,
                                      win32file.FILE_SHARE_WRITE,
                                      None,
                                      win32file.OPEN_EXISTING,
                                      0,
                                      None)
    except pywintypes.error as e:
        return None
    return handle


def get_root_hub_name(handle):
    buf = win32file.DeviceIoControl(handle,
                                    IOCTL_USB_GET_ROOT_HUB_NAME,
                                    None,
                                    6,
                                    None)
    act_len, _ = struct.unpack('LH', buf)
    buf = win32file.DeviceIoControl(handle,
                                    IOCTL_USB_GET_ROOT_HUB_NAME,
                                    None,
                                    act_len,
                                    None)
    return buf[4:].decode('utf-16le')


def get_driverkey_name(handle, index):
    key_name = chr(index) + '\0' * 9
    try:
        buf = win32file.DeviceIoControl(handle,
                                        IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
                                        key_name,
                                        10,
                                        None)
    except pywintypes.error as e:
        print(e.strerror, index)
        sys.exit(1)
    _, act_len, _ = struct.unpack('LLH', buf)
    buf = win32file.DeviceIoControl(handle,
                                    IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
                                    key_name,
                                    act_len,
                                    None)
    return buf[8:].decode('utf-16le')


def get_ext_hub_name(handle, index):
    hub_name = chr(index) + '\0' * 9
    buf = win32file.DeviceIoControl(handle,
                                    IOCTL_USB_GET_NODE_CONNECTION_NAME,
                                    hub_name,
                                    10,
                                    None)
    _, act_len, _ = struct.unpack('LLH', buf)
    buf = win32file.DeviceIoControl(handle,
                                    IOCTL_USB_GET_NODE_CONNECTION_NAME,
                                    hub_name,
                                    act_len,
                                    None)
    return buf[8:].decode('utf-16le')


def get_str_desc(handle, conn_idx, str_idx):
    req = struct.pack('LBBHHH',
                      conn_idx,
                      0,
                      0,
                      (USB_STRING_DESCRIPTOR_TYPE << 8) | str_idx,
                      win32api.GetSystemDefaultLangID(),
                      12 + MAXIMUM_USB_STRING_LENGTH)
    try:
        buf = win32file.DeviceIoControl(handle,
                                        IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
                                        req,
                                        12 + MAXIMUM_USB_STRING_LENGTH,
                                        None)
    except pywintypes.error as e:
        return 'ERROR: no String Descriptor for index {}'.format(str_idx)
    if len(buf) > 16:
        return buf[14:].decode('utf-16le')
    return ''


def exam_hub(name, verbose, level):
    handle = open_dev(r'\\.\{}'.format(name))
    if not handle:
        print('Failed to open device {}'.format(name))
        return
    buf = win32file.DeviceIoControl(handle,
                                    IOCTL_USB_GET_NODE_INFORMATION,
                                    None,
                                    76,
                                    None)
    print_hub_ports(handle, ord(buf[6]), verbose, level)
    handle.close()


def print_str_or_hex(to_be_print):
    if all(c in string.printable for c in to_be_print):
        print('"{}"'.format(to_be_print))
        return
    print('Hex: ', end='')
    for x in to_be_print:
        print('{:02x} '.format(ord(x)), end='')
    print('')


def print_hub_ports(handle, num_ports, verbose, level):
    for idx in range(1, num_ports + 1):
        info = chr(idx) + '\0' * 34
        try:
            buf = win32file.DeviceIoControl(handle,
                                            (IOCTL_USB_GET_NODE_CONNECTION_INFORMATION),
                                            info,
                                            34 + 11 * 30,
                                            None)
        except pywintypes.error as e:
            print(e.winerror, e.funcname, e.strerror)
            return

        _, vid, pid, vers, manu, prod, seri, _, ishub, _, stat = struct.unpack('=12sHHHBBB3s?6sL', buf[:35])

        if ishub:
            if verbose:
                print('{}  [Port{}] {}'.format('  ' * level, idx, 'USB Hub'))
            exam_hub(get_ext_hub_name(handle, idx), verbose, level)
        elif stat == 0 and verbose:
            print('{}  [Port{}] {}'.format('  ' * level, idx, 'NoDeviceConnected'))
        elif stat == 1:
            if verbose or (manu != 0 or prod != 0 or seri != 0):
                print('{}  [Port{}] {}'.format('  ' * level, idx, get_driverkey_name(handle, idx)))
                print('{}    Vendor ID:    0x{:04X}'.format('  ' * level, vid))
                print('{}    Product ID:  0x{:04X}'.format('  ' * level, pid))
                print('{}    Device BCD:  0x{:04X}'.format('  ' * level, vers))
                if manu != 0:
                    print('{}    Manufacturer (0x{:x}) -> '.format('  ' * level, manu), end='')
                    print_str_or_hex(get_str_desc(handle, idx, manu))
                if prod != 0:
                    print('{}    Product      (0x{:x}) -> '.format('  ' * level, prod), end='')
                    print_str_or_hex(get_str_desc(handle, idx, prod))
                if seri != 0:
                    print('{}    Serial No    (0x{:x}) -> '.format('  ' * level, seri), end='')
                    print_str_or_hex(get_str_desc(handle, idx, seri))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', '--verbose', action='store_true',
                        help="Increase output verbosity.")
    args = parser.parse_args()

    for i in range(10):
        name = r"\\.\HCD{}".format(i)
        handle = open_dev(name)
        if not handle:
            continue

        root = get_root_hub_name(handle)
        print('{}RootHub: {}'.format('\n' if i != 0 else '', root))

        # ext = get_ext_hub_name(handle, index)
        # print('{}ExtHub: {}'.format('\n' if i != 0 else '', ext))

        dev_name = r'\\.\{}'.format(root)
        dev_handle = open_dev(dev_name)
        if not dev_handle:
            print('Failed to open device {}'.format(dev_name))
            continue

        buf = win32file.DeviceIoControl(dev_handle,
                                        IOCTL_USB_GET_NODE_INFORMATION,
                                        None,
                                        76,
                                        None)

        num = buf[6]
        print_hub_ports(dev_handle, num, args.verbose, 0)
        dev_handle.close()
        handle.close()


if __name__ == '__main__':
    main()
