import os
import struct
from ctypes import *
import platform

HID_SMBUS_STATUS_Return_Code = {
    0x00: "HID_SMBUS_SUCCESS				",  # SILABS_STATUS_SUCCESS
    0x01: "HID_SMBUS_DEVICE_NOT_FOUND		",  # 0x01
    0x02: "HID_SMBUS_INVALID_HANDLE			",  # 0x02
    0x03: "HID_SMBUS_INVALID_DEVICE_OBJECT	",  # 0x03
    0x04: "HID_SMBUS_INVALID_PARAMETER		",  # 0x04
    0x05: "HID_SMBUS_INVALID_REQUEST_LENGTH	",  # 0x05
    # H:D_SMBUS_DEVICE_ALREADY_OPENED SILABS_ ATUS_DEVICE_ALREADY_OPENED	///< the specified device is already (exclusively) opened
    0x10: "HID_SMBUS_READ_ERROR				",  # 0x10
    0x11: "HID_SMBUS_WRITE_ERROR			",  # 0x11
    0x12: "HID_SMBUS_READ_TIMED_OUT			",  # 0x12
    0x13: "HID_SMBUS_WRITE_TIMED_OUT		",  # 0x13
    0x14: "HID_SMBUS_DEVICE_IO_FAILED		",  # 0x14
    0x15: "HID_SMBUS_DEVICE_ACCESS_ERROR	",  # 0x15
    0x16: "HID_SMBUS_DEVICE_NOT_SUPPORTED	",  # 0x16
}

# Micreosoft types to ctypes
WORD = c_ushort
DWORD = c_ulong
DWORD64 = c_ulonglong
UNIT_PTR = c_ulong
LONG = c_long
BYTE = c_byte
BOOL = c_bool
LPBYTE = POINTER(c_ubyte)
LPTSTR = POINTER(c_char)
HANDLE = c_void_p
PVOID = c_void_p


# BYTE=unsigned char(完全等同)  c_char
# WORD=unsigned short(完全等同)  c_ushort
# DWORD=unsigned long(完全等同) c_ulong

# ptr_deviceString
#
# Definition                     Value Length    Description
#
# HID_SMBUS_GET_VID_STR           0x01 5         Vendor ID
# HID_SMBUS_GET_PID_STR           0x02 5         Product ID
# HID_SMBUS_GET_PATH_STR          0x03 260/512   Device path
# HID_SMBUS_GET_SERIAL_STR        0x04 256       Serial string
# HID_SMBUS_GET_MANUFACTURER_STR  0x05 256       Manufacturer string
# HID_SMBUS_GET_PRODUCT_STR       0x06 256       Product string


class hid_smbus:
    I2C_RECEIVE_BUFFER_SIZE_INT = 61
    I2C_RECEIVE_BUFFER_SIZE = BYTE(I2C_RECEIVE_BUFFER_SIZE_INT)

    def __init__(self):
        if platform.system() == "Windows":
            if struct.calcsize(str('P')) * 8 == 64:
                arch = "x64"
            else:
                arch = "x86"
            dll_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "native\Windows", arch,
                                    "SLABHIDtoSMBus.dll")
        elif platform.system() == "Linux":
            if struct.calcsize(str('P')) * 8 == 64:
                arch = "x86_64"
            else:
                arch = "x86_32"
            dll_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "native\Linux", arch,
                                    "SLABHIDtoSMBus.dll")
        else:
            arch = ""
            dll_path = ""
            print("other system !!!")

        print("os:", platform.system(), ", arch:", arch)
        try:
            self.__dll = cdll.LoadLibrary(dll_path)
        except Exception as e:
            print("[!!! error]", e)

    def HidSmbus_GetNumDevices(self, numDevices: DWORD, vid: WORD, pid: WORD):  # Verify
        ret = self.__dll.HidSmbus_GetNumDevices(pointer(numDevices), vid, pid)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetString(self, deviceNum: DWORD, vid: WORD, pid: WORD,
                           deviceString: c_char, options: DWORD):
        ret = self.__dll.HidSmbus_GetString(deviceNum, vid, pid, pointer(deviceString), options)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetOpenedString(self, device: c_void_p, deviceString: c_char,
                                 options: DWORD):
        ret = self.__dll.HidSmbus_GetOpenedString(device, pointer(deviceString), options)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    if (platform.system() == "Windows"):
        def HidSmbus_GetIndexedString(self, deviceNum: DWORD, vid: WORD, pid: WORD,
                                      stringIndex: DWORD, deviceString: c_char):
            ret = self.__dll.HidSmbus_GetIndexedString(deviceNum, vid, pid, stringIndex, pointer(deviceString))
            return ret, HID_SMBUS_STATUS_Return_Code[ret]

        def HidSmbus_GetOpenedIndexedString(self, device: c_void_p,
                                            stringIndex: DWORD, deviceString: c_char):
            ret = self.__dll.HidSmbus_GetOpenedIndexedString(device, stringIndex, pointer(deviceString))
            return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetAttributes(self, deviceNum: DWORD, vid: WORD, pid: WORD,
                               deviceVid: WORD, devicePid: WORD,
                               deviceReleaseNumber: WORD):
        ret = self.__dll.HidSmbus_GetAttributes(deviceNum, vid, pid,
                                                pointer(deviceVid), pointer(devicePid),
                                                pointer(deviceReleaseNumber))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetOpenedAttributes(self, device: c_void_p,
                                     deviceVid: WORD, devicePid: WORD,
                                     deviceReleaseNumber: WORD):
        ret = self.__dll.HidSmbus_GetOpenedAttributes(device,
                                                      pointer(deviceVid), pointer(devicePid),
                                                      pointer(deviceReleaseNumber))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_Open(self, device: c_void_p, deviceNum: DWORD,  # Verify
                      vid: WORD, pid: WORD):
        ret = self.__dll.HidSmbus_Open(pointer(device), deviceNum, vid, pid)  # !!!!pointer(ptr_device)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_IsOpened(self, device: c_void_p, opened: c_bool):  # Verify
        ret = self.__dll.HidSmbus_IsOpened(device, pointer(opened))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_ReadRequest(self, device: c_void_p, slaveAddress: BYTE, numBytesToRead: WORD):  # Verify
        ret = self.__dll.HidSmbus_ReadRequest(device, slaveAddress, numBytesToRead)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_AddressReadRequest(self, device: c_void_p, slaveAddress: BYTE, numBytesToRead: WORD,  # Verify
                                    targetAddressSize: BYTE, targetAddress: BYTE * 16):
        ret = self.__dll.HidSmbus_AddressReadRequest(device, slaveAddress, numBytesToRead,
                                                     targetAddressSize, targetAddress)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_ForceReadResponse(self, device: c_void_p, numBytesToRead: WORD):  # Verify, but don't know how to use.
        """
        Forces the device to generate and send a read response
        :param device:
        :param numBytesToRead:
        :return:
        """
        ret = self.__dll.HidSmbus_ForceReadResponse(device, numBytesToRead)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetReadResponse(self, device: c_void_p, status: BYTE, buffer: pointer,  # ??????????????????
                                 bufferSize: BYTE, numBytesRead: pointer):

        # HID_SMBUS_STATUS
        # HidSmbus_GetReadResponse(HID_SMBUS_DEVICE
        # device,
        # HID_SMBUS_S0 * status, BYTE * buffer, BYTE
        # bufferSize,
        # BYTE * numBytesRead

        """
        :param device:
        :param status:
        :param buffer: BYTE pointer
        :param bufferSize:
        :param numBytesRead: BYTE pointer
        :return:
        """
        ret = self.__dll.HidSmbus_GetReadResponse(device, status, buffer,
                                                  bufferSize, numBytesRead)
        status_return_Code = {
            0x00: "HID_SMBUS_S0_IDLE		",  # 0x00 No transfers are currently active on the bus.
            0x01: "HID_SMBUS_S0_BUSY		",  # 0x01 A read or write transfer is in progress.
            0x02: "HID_SMBUS_S0_COMPLETE	",  # 0x02 A read or write transfer completed without error and without etry.
            0x03: "HID_SMBUS_S0_ERROR	    ",  # 0x03 A read or write transfer completed with an error.
        }
        print(status_return_Code[status.value])

        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_WriteRequest(self, device: c_void_p, slaveAddress: BYTE, buffer: pointer,  # verify
                              numBytesToWrite: BYTE):
        """
        :param device:
        :param slaveAddress:
        :param buffer: BYTE type pointer
        :param numBytesToWrite:
        :return:
        """
        ret = self.__dll.HidSmbus_WriteRequest(device, slaveAddress, buffer, numBytesToWrite)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_TransferStatusRequest(self, device: c_void_p):
        ret = self.__dll.HidSmbus_TransferStatusRequest(device)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetTransferStatusResponse(self, device: c_void_p, status: BYTE, detailedStatus: BYTE, buffer: pointer,  # ??????????????????
                                           numRetries: WORD, bytesRead: WORD):
        ret = self.__dll.HidSmbus_GetTransferStatusResponse(device, status, pointer(detailedStatus), pointer(buffer),
                                                            pointer(numRetries), pointer(bytesRead))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_CancelTransfer(self, device: c_void_p):  # verify
        ret = self.__dll.HidSmbus_CancelTransfer(device)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_CancelIo(self, device: c_void_p):
        ret = self.__dll.HidSmbus_CancelIo(device)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_SetTimeouts(self, device: c_void_p, responseTimeout: DWORD):
        ret = self.__dll.HidSmbus_SetTimeouts(device, responseTimeout)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetTimeouts(self, device: c_void_p, responseTimeout: DWORD):
        ret = self.__dll.HidSmbus_GetTimeouts(device, pointer(responseTimeout))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_SetSmbusConfig(self, device: c_void_p, bitRate: DWORD, address: BYTE, autoReadRespond: BOOL,  # verify
                                writeTimeout: WORD, readTimeout: WORD, sclLowTimeout:BOOL, transferRetries: WORD):
        ret = self.__dll.HidSmbus_SetSmbusConfig(device, bitRate, address, autoReadRespond,
                                                 writeTimeout, readTimeout, sclLowTimeout, transferRetries)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetSmbusConfig(self, device: c_void_p, bitRate: DWORD, address: BYTE, autoReadRespond: BOOL,  # verify
                                writeTimeout: WORD, readTimeout: WORD, sclLowTimeout:BOOL, transferRetries: WORD):
        ret = self.__dll.HidSmbus_GetSmbusConfig(device, pointer(bitRate), pointer(address), pointer(autoReadRespond),
                                                 pointer(writeTimeout), pointer(readTimeout), pointer(sclLowTimeout),
                                                 pointer(transferRetries))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_Reset(self, device: c_void_p):  # verify
        ret = self.__dll.HidSmbus_Reset(device)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_SetGpioConfig(self, device: c_void_p, direction: BYTE, mode: BYTE,   # verify
                               function: BYTE, clkDiv: BYTE):
        ret = self.__dll.HidSmbus_SetGpioConfig(device, direction, mode, function, clkDiv)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetGpioConfig(self, device: c_void_p, direction: BYTE, mode: BYTE,   #
                               function: BYTE, clkDiv: BYTE):
        ret = self.__dll.HidSmbus_GetGpioConfig(device, pointer(direction), pointer(mode), pointer(function),
                                                pointer(clkDiv))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_WriteLatch(self, device: c_void_p, latchValue: BYTE, latchMask: BYTE):  # verify
        # TODO:latchMask
        ret = self.__dll.HidSmbus_WriteLatch(device, latchValue, latchMask)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_ReadLatch(self, device: c_void_p, latchValue: BYTE):  #
        ret = self.__dll.HidSmbus_ReadLatch(device, pointer(latchValue))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetPartNumber(self, device: c_void_p, partNumber: BYTE, version: BYTE):  # HID_SMBUS_DEVICE_IO_FAILED
        ret = self.__dll.HidSmbus_GetPartNumber(device, pointer(partNumber), pointer(version))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetLibraryVersion(self, device: c_void_p, major: BYTE, minor: BYTE, release: BOOL):  # verify
        ret = self.__dll.HidSmbus_GetLibraryVersion(device, pointer(major), pointer(minor), pointer(release))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def HidSmbus_GetHidLibraryVersion(self, device: c_void_p, major: BYTE, minor: BYTE, release: BOOL):  # verify
        ret = self.__dll.HidSmbus_GetHidLibraryVersion(device, pointer(major), pointer(minor), pointer(release))
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    if (platform.system() == "Windows"):
        def HidSmbus_GetHidGuid(self, guid: c_void_p):  #
            ret = self.__dll.HidSmbus_GetHidGuid(pointer(guid))
            return ret, HID_SMBUS_STATUS_Return_Code[ret]

