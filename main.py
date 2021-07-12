from ctypes import *
from slab_hid_to_smbus import hid_smbus
import time


# try:
cp2112 = hid_smbus()
cp2112_vid = c_ushort(0x10C4)
cp2112_pid = c_ushort(0xEA90)
# except Exception as e:
#     print("[!!! error]", e)

dev = c_void_p()


def cp2112_gpio_test(dev):
    # Set all GPIO to OUTPUT
    ret, error_info = cp2112.HidSmbus_SetGpioConfig(dev, c_byte(0xFF), c_byte(0x00), c_byte(0x00), c_byte(0x00))
    print(ret, error_info)

    # Set or Reset GPIO status
    for i in range(10):
        ret = cp2112.HidSmbus_WriteLatch(dev, c_byte(0xFF), c_byte(0xFF))
        time.sleep(0.05)
        ret = cp2112.HidSmbus_WriteLatch(dev, c_byte(0x00), c_byte(0xFF))
        time.sleep(0.05)
    return ret, error_info

def cp2112_get_lib_version(dev: c_void_p):
    status1 = c_byte()
    status2 = c_byte()
    status3 = c_bool()
    ret, error_info = cp2112.HidSmbus_GetLibraryVersion(dev, status1, status2, status3)
    print("lib version:", status1.value, ".", status2.value, ".", status3.value)

    # error: HID_SMBUS_DEVICE_IO_FAILED
    # part_number = c_byte()
    # version = c_byte()
    # ret, error_info = cp2112.HidSmbus_GetPartNumber(dev, part_number, version)
    # print("error info:", ret, error_info)
    # print("part number:", part_number.value, "version:", version.value)

    ret, error_info = cp2112.HidSmbus_GetHidLibraryVersion(dev, status1, status2, status3)
    # print("error info:", ret, error_info)
    print("hid lib version:", status1.value, ".", status2.value, ".", status3.value)


def set_smbus_config(dev_x: c_void_p, speed: c_ulong, time_out: c_ushort, retry: c_ushort):
    # set smbus config ( baudrate addr autoReadRespond... )
    # when set autoReadRespond, after HidSmbus_AddressReadRequest, need send HidSmbus_ForceReadResponse &
    # HidSmbus_GetReadResponse

    # when set autoReadRespond = 0, after HidSmbus_AddressReadRequest, use HidSmbus_GetReadResponse get val,
    # but find out need read 2 times to get complete val
    ret, error_info = cp2112.HidSmbus_SetSmbusConfig(dev_x, speed, c_byte(0x16), c_bool(False),
                                                     time_out, time_out, c_bool(False),
                                                     retry)  # send retry-send times = 3
    if (ret != 0):
        print("config error info:", ret, error_info)

    # get smbus config
    bitRate = c_ulong()
    address = c_byte()
    autoReadRespond = c_bool()
    writeTimeout = c_ushort()
    readTimeout = c_ushort()
    sclLowTimeout = c_bool()
    transferRetries = c_ushort()
    ret, error_info = cp2112.HidSmbus_GetSmbusConfig(dev_x, bitRate, address, autoReadRespond,
                                                     writeTimeout, readTimeout, sclLowTimeout, transferRetries)
    print("smbus config: bitRate:", bitRate.value, "address:0x%x" % address.value, "transferRetries:",
          transferRetries.value)
    if (ret != 0):
        print("HidSmbus_GetSmbusConfig error info:", ret, error_info)
    return ret


def reg_addr_read_and_read(dev_x: c_void_p, dev_addr: c_byte, reg_addr_len: c_byte, reg_addr: c_byte * 16,
                           n_byte_to_read: c_ushort):
    # dev_addr = c_byte(0x16)
    # n_byte_to_read = c_ushort(6)
    # reg_addr_len = c_byte(1)
    # byte_array = c_byte * 16
    # reg_addr_buf = byte_array(0x3D, 0x00)
    ret0, error_info = cp2112.HidSmbus_AddressReadRequest(dev_x, dev_addr, n_byte_to_read,
                                                          reg_addr_len, reg_addr)
    # print("2 error info:", ret0, error_info)

    # ForceReadResponse
    ret1, error_info = cp2112.HidSmbus_ForceReadResponse(dev_x, n_byte_to_read)
    # print("3 error info:", ret1, error_info)

    # get read response
    status = c_ubyte()
    BufferArray = c_ubyte * cp2112.I2C_RECEIVE_BUFFER_SIZE_INT
    buffer = BufferArray()
    numBytesRead = c_byte()
    while status.value != 0x02:
        ret2, error_info = cp2112.HidSmbus_GetReadResponse(dev_x, pointer(status), pointer(buffer), pointer(numBytesRead))
    # print("A error info:", ret2, error_info)
    ret = ret0 + ret1 + ret2
    if numBytesRead.value == n_byte_to_read.value:
        print("-----recv completed-----")
        print("0x%x" % buffer[0])
        print("0x%x" % buffer[1])
        print("0x%x" % buffer[2])
        print("0x%x" % buffer[3])
        print("0x%x" % buffer[4])
        print("0x%x" % buffer[5])
    else:
        print("-----recv fail-----")
        print(ret)

    if ret != 0:
        print("HidSmbus_GetSmbusConfig error info:", ret, error_info)
    return ret, buffer

# bug, please discard it
def reg_addr_read_and_read_auto_response(dev_addr: c_byte, reg_addr_len: c_byte, reg_addr: c_byte * 16,
                           n_byte_to_read: c_ushort):

    ret, error_info = cp2112.HidSmbus_AddressReadRequest(dev, dev_addr, n_byte_to_read,
                                                         reg_addr_len, reg_addr)
    print("2 error info:", ret, error_info)

    # get read response
    dev_addr = c_ubyte(0x16)
    status = c_ubyte()
    BufferArray = c_ubyte * cp2112.I2C_RECEIVE_BUFFER_SIZE_INT
    buffer = BufferArray()
    # buffer = c_byte * 2
    numBytesRead = c_byte()

    ret, error_info = cp2112.HidSmbus_GetReadResponse(dev, pointer(status), pointer(buffer), pointer(numBytesRead))
    print("0x%x" % buffer[0])
    print("numBytesRead:", numBytesRead.value)

    # # read request
    dev_addr = c_byte(0x16)
    # # address = c_byte(int("16", 16))
    # n_byte_to_read = c_ushort(2)
    ret, error_info = cp2112.HidSmbus_ReadRequest(dev, dev_addr, n_byte_to_read)
    print("1 error info:", ret, error_info)


    while status.value != 0x02:
        ret, error_info = cp2112.HidSmbus_GetReadResponse(dev, pointer(status), pointer(buffer), pointer(numBytesRead))
        # print("A error info:", ret, error_info)
        time.sleep(0.01)
        print(status.value)
    print("-----recv completed-----")
    print("0x%x" % buffer[0])
    print("0x%x" % buffer[1])
    print("0x%x" % buffer[2])
    print("0x%x" % buffer[3])
    print("0x%x" % buffer[4])
    print("0x%x" % buffer[5])
    print("numBytesRead:", numBytesRead.value)
    return buffer


# # Micreosoft types to ctypes
# WORD = c_ushort
# DWORD = c_ulong
# DWORD64 = c_ulonglong
# UNIT_PTR = c_ulong
# LONG = c_long
# BYTE = c_byte
# BOOL = c_bool
# LPBYTE = POINTER(c_ubyte)
# LPTSTR = POINTER(c_char)
# HANDLE = c_void_p
# PVOID = c_void_p


if __name__ == '__main__':



    # judge cp2112 dev status & get num Devices
    numDevices = c_ulong()
    cp2112.HidSmbus_GetNumDevices(numDevices, cp2112_vid, cp2112_pid)
    if numDevices.value != 1:
        print("ERROR: No Device Connected or device over 1, dev_num:", numDevices.value)
        exit()



    # Open cp2112 device
    is_opened = c_bool(False)
    ret, error_info = cp2112.HidSmbus_Open(dev, c_ulong(numDevices.value - 1), cp2112_vid, cp2112_pid)
    cp2112.HidSmbus_IsOpened(dev, is_opened)
    if is_opened.value:
        print("cp2112 is opened")
    else:
        print("cp2112 is not opened")

    # reset smbus
    # ret, error_info = cp2112.HidSmbus_Reset(dev)
    # print("0 error info:", ret, error_info)

    set_smbus_config(dev, c_ulong(100000), c_ushort(100), c_ushort(3))

    # reg addr address read
    """ BM191 test
        ADDR_W:0x16
        W:0x3C
        ADDR_R:0x16
        R:0xxx
    """
    byte_array = c_byte * 16
    reg_addr_buf = byte_array(0x3C)
    reg_addr_read_and_read(dev, c_byte(0x16), c_byte(1), reg_addr_buf, c_ushort(2))

    # # test cp2112 gpio
    # cp2112_gpio_test(dev)


    # # read request
    # dev_addr = c_byte(0x16)
    # # address = c_byte(int("16", 16))
    # n_bytes = c_ushort(2)
    # ret, error_info = cp2112.HidSmbus_ReadRequest(dev, dev_addr, n_bytes)
    # print("1 error info:", ret, error_info)


    # write request, need add cancel tansfer ?
    dev_addr = c_byte(0x16)
    buffer_array = c_byte * 16
    buffer = buffer_array(0x3C, 0x00, 0x01, 0x02)
    n_bytes = c_byte(1)
    ret, error_info = cp2112.HidSmbus_WriteRequest(dev, dev_addr, pointer(buffer), n_bytes)
    print("5 error info:", ret, error_info)
    ret, error_info = cp2112.HidSmbus_CancelTransfer(dev)
    print("6 error info:", ret, error_info)


    ret, error_info = cp2112.HidSmbus_TransferStatusRequest(dev)
    print("7 error info:", ret, error_info)



    # get part number & version
    cp2112_get_lib_version(dev)
