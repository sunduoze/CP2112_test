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

# typedef unsigned long       DWORD;
# typedef int                 BOOL;
#c_uint8  typedef unsigned char       BYTE;
#c_uint16 typedef unsigned short      WORD;
# typedef float               FLOAT;

class HidSmbus:
    I2C_RECEIVE_BUFFER_SIZE = c_byte(61)
    I2C_RECEIVE_BUFFER_SIZE_INT = 61

    def __init__(self):
        """__init__"""
        if (platform.system() == "Windows"):
            # アーキテクチャ(32bit?64bit?)を判定し、対応するDLLをロードする
            if struct.calcsize(str('P')) * 8 == 64:
                arch = "x64"
            else:
                arch = "x86"
            dllPath = os.path.join(os.path.dirname(os.path.abspath(__file__)), "native\Windows", arch,
                                   "SLABHIDtoSMBus.dll")
        elif (platform.system() == "Linux"):
            if struct.calcsize(str('P')) * 8 == 64:
                arch = "x86_64"
            else:
                arch = "x86_32"
            dllPath = os.path.join(os.path.dirname(os.path.abspath(__file__)), "native\Linux", arch,
                                   "SLABHIDtoSMBus.dll")
        else:
            print("other system")

        print("os:", platform.system(), ", arch:", arch)

        self.__dll = cdll.LoadLibrary(dllPath)

    def GetNumDevices(self, refNumDevices: pointer, vid: c_ushort, pid: c_ushort):
        """GetNumDevices
      refNumDevice is c_uint pointer
    """

        ret = self.__dll.HidSmbus_GetNumDevices(refNumDevices, vid, pid)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def Open(self, refDevice: pointer, deviceNum: c_uint, vid: c_ushort, pid: c_ushort):
        """Open
      refDevice is c_int pointer
    """
        ret = self.__dll.HidSmbus_Open(refDevice, deviceNum, vid, pid)
        return ret
    def is_open(self, refDevice: pointer, deviceNum: c_uint, vid: c_ushort, pid: c_ushort):
        """Open
      refDevice is c_int pointer
    """
        ret = self.__dll.HidSmbus_Open(refDevice, deviceNum, vid, pid)
        return ret
    def SetGpioConfig(self, refDevice: pointer, direction: c_byte, mode: c_byte, function: c_byte, clkDiv: c_byte):
        """SetGpioConfig
    """
        ret = c_int()
        ret = self.__dll.HidSmbus_SetGpioConfig(refDevice, direction, mode, function, clkDiv)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def WriteLatch(self, refDevice: pointer, latchValue: c_byte, latchMask: c_byte):
        """WriteLatch
    """
        ret = c_int()
        ret = self.__dll.HidSmbus_WriteLatch(refDevice, latchValue, latchMask)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def WriteRequest(self, refDevice: pointer, slaveAddress: c_byte, refBuffer: pointer, numBytesToWrite: c_ushort):
        """WriteRequest
      refBuffer: is c_byte pointer
    """
        ret = self.__dll.HidSmbus_WriteRequest(refDevice, slaveAddress, refBuffer, numBytesToWrite)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def ReadRequest(self, refDevice: pointer, slaveAddress: c_byte, numBytesToRead: c_ushort):
        """ReadRequest
    """
        ret = self.__dll.HidSmbus_ReadRequest(refDevice, slaveAddress, numBytesToRead)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def ForceReadResponse(self, refDevice: pointer):
        """ForceReadResponse
    """
        ret = self.__dll.HidSmbus_ForceReadResponse(refDevice, self.I2C_RECEIVE_BUFFER_SIZE)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def GetReadResponse(self, refDevice: pointer, refStatus: pointer, refBuffer: pointer, refNumBytesRead):
        """GetReadResponse
      refStatus is c_byte pointer
      refBuffer is c_byte pointer
      refNumBytesRead is c_byte pointer
    """
        ret = self.__dll.HidSmbus_GetReadResponse(refDevice, refStatus, refBuffer, self.I2C_RECEIVE_BUFFER_SIZE,
                                                  refNumBytesRead)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def AddressReadRequest(self, refDevice: pointer, slaver_addr: c_uint8, n_bytes_to_read: c_uint16,
                           target_addr_size: c_uint8, target_addr: c_uint8 * 16):

        ret = self.__dll.HidSmbus_AddressReadResponse(refDevice, slaver_addr, n_bytes_to_read, target_addr_size,
                                                      target_addr)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def get_library_version(self, major: pointer, minor: pointer, bIsRelease: pointer):
        ret = self.__dll.HidSmbus_GetLibraryVersion(major, minor, bIsRelease)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    def set_smbus_config(self, refDevice: pointer, bitRate: c_int, address: c_byte, autoReadRespond: c_bool,
                         writeTimeout: c_int, readTimeout: c_int, sclLowTimeout: c_bool, transferRetries: c_int):
        ret = self.__dll.HidSmbus_SetSmbusConfig(refDevice, bitRate, address, autoReadRespond,
                                                 writeTimeout, readTimeout, sclLowTimeout, transferRetries)
        return ret, HID_SMBUS_STATUS_Return_Code[ret]

    # def HidSmbus_GetSmbusConfig

    # def GetHidLibraryVersion(self, major: pointer, minor: pointer, bIsRelease: pointer):
    #     ret = self.__dll.HidSmbus_GetLibraryVersion(major, minor, bIsRelease)
    #     return ret, HID_SMBUS_STATUS_Return_Code[ret]
