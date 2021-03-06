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


def reg_addr_read(dev_x: c_void_p, dev_addr: c_byte, reg_addr_len: c_byte, reg_addr: c_byte * 16,
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
    cnt = 0
    while status.value != 0x02:
        ret2, error_info = cp2112.HidSmbus_GetReadResponse(dev_x, pointer(status), pointer(buffer),
                                                           pointer(numBytesRead))
        # print("A error info:", ret2, error_info)
        cnt = cnt + 1
        if cnt > 5:
            break

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


def read(dev_x: c_void_p, dev_addr: c_byte, n_byte_to_read: c_ushort):
    # read request
    ret, error_info = cp2112.HidSmbus_ReadRequest(dev_x, dev_addr, n_byte_to_read)
    # print("1 error info:", ret, error_info)
    # ForceReadResponse
    ret1, error_info = cp2112.HidSmbus_ForceReadResponse(dev_x, n_byte_to_read)
    # print("3 error info:", ret1, error_info)

    # get read response
    status = c_ubyte()
    BufferArray = c_ubyte * cp2112.I2C_RECEIVE_BUFFER_SIZE_INT
    buffer = BufferArray()
    numBytesRead = c_byte()
    cnt = 0
    while status.value != 0x02:
        ret2, error_info = cp2112.HidSmbus_GetReadResponse(dev_x, pointer(status), pointer(buffer),
                                                           pointer(numBytesRead))
        # print("A error info:", ret2, error_info)
        cnt = cnt + 1
        if cnt > 5:
            break

    # print("A error info:", ret2, error_info)
    # ret = ret0 + ret1 + ret2
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
    # return ret, buffer
    return ret, error_info


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


def get_status(dev: c_void_p):
    print("-------------get_status--")
    ret, error_info = cp2112.HidSmbus_TransferStatusRequest(dev)
    print("7 error info:", ret, error_info)

    status = c_byte()
    detail_status = c_byte()
    buffer_array = c_byte * 16
    buffer = buffer_array()
    num_retrys = c_ushort()
    bytes_read = c_ushort()
    ret, error_info = cp2112.HidSmbus_GetTransferStatusResponse(dev, status, detail_status, num_retrys, bytes_read)
    # print("A error info:", ret, error_info)
    print(status.value, cp2112.status_return_Code[status.value])
    if status.value == 0x01:  # HID_SMBUS_S0_BUSY
        print(detail_status.value, cp2112.HID_SMBUS_S0_BUSY[detail_status.value])
    elif status.value == 0x03:  # HID_SMBUS_S0_ERROR
        print(detail_status.value, cp2112.HID_SMBUS_S0_ERROR[detail_status.value])
    # else:
    #     print(detail_status.value)

    print("num_retrys", num_retrys.value)
    print("bytes_read", bytes_read.value)
    print("-------end of get_status----------")
    return ret, error_info



def addr_write(dev_addr, reg_addr, reg_addr_len, data, data_len):
    time_out = 100
    # write request, need add cancel tansfer ?
    dev_addr = c_byte(dev_addr)
    buffer_array = c_byte * 64
    if reg_addr_len != 0:
        buffer = buffer_array(reg_addr)
        n_bytes = c_byte(reg_addr_len)
        ret, error_info = cp2112.HidSmbus_WriteRequest(dev, dev_addr, pointer(buffer), n_bytes)
        print("5 error info:", ret, error_info)
        time.sleep(time_out * 0.001)
        # ret, error_info = cp2112.HidSmbus_CancelTransfer(dev)

    buffer = data # buffer_array(0x03, 0x84, 0x01, 0x4B, 0x0D)
    n_bytes = c_byte(data_len) # c_byte(5)
    ret, error_info = cp2112.HidSmbus_WriteRequest(dev, dev_addr, pointer(buffer), n_bytes)
    print("5 error info:", ret, error_info)
    time.sleep(time_out * 0.001)

    return ret

def aslan_pack_beacon_config():
    buffer_array = c_byte * 64


# Beacon data
    buf = buffer_array(0x0A,
                       0x1F, 0x86, 0x02, 0x1B, 0xFF, 0x01, 0xF1, 0xBE,
                       0xAC, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                       0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
                       0x1A, 0xEB, 0xEB, 0xEC, 0xDD, 0xC3, 0x00, 0x1A,
                       0x0D)
    addr_write(0x42, 0x0A, 0, buf, 34)
    time.sleep(0.1)

# Tx power set
    buf = buffer_array(0x0A,
                       0x03, 0x82, 0x01, 0x4B, 0x0D)
    addr_write(0x42, 0x0A, 0, buf, 6)
    time.sleep(0.1)

# Tx rate set
    buf = buffer_array(0x0A,
                       0x04, 0x83, 0x00, 0x01, 0x65, 0x0D)
    addr_write(0x42, 0x0A, 0, buf, 7)
    time.sleep(0.1)

# # beacon enable
#     buf = buffer_array(0x0A,
#                        0x03, 0x84, 0x01, 0x4B, 0x0D)
#     addr_write(0x42, 0x0A, 0, buf, 6)
#     time.sleep(0.1)

# # beacon disable
#     buf = buffer_array(0x0A,
#                        0x03, 0x84, 0x00, 0x02, 0x0D)
#     addr_write(0x42, 0x0A, 0, buf, 6)

def aslan_pack_beacon_status(status):
    if(status is True):
        buffer_array = c_byte * 64
        # beacon enable
        buf = buffer_array(0x0A,
                           0x03, 0x84, 0x01, 0x4B, 0x0D)
        addr_write(0x42, 0x0A, 0, buf, 6)
        time.sleep(0.1)
        print("beacon is opened")
    else:
        buffer_array = c_byte * 64
        # beacon disable
        buf = buffer_array(0x0A,
                           0x03, 0x84, 0x00, 0x02, 0x0D)
        addr_write(0x42, 0x0A, 0, buf, 6)
        print("beacon is closed")

def aslan_pack_beacon_thermal_pin(status):
    # Set all GPIO to OUTPUT
    #0xF0: GPIO 0:3 ->input 4:7 ->output
    ret, error_info = cp2112.HidSmbus_SetGpioConfig(dev, c_byte(0xF0), c_byte(0x00), c_byte(0x00), c_byte(0x00))
    # print(ret, error_info)
    if(status is True):
        # 0xF0: GPIO 0:3 ->low 4:4 -> high 5:7 ->low
        ret = cp2112.HidSmbus_WriteLatch(dev, c_byte(0x10), c_byte(0xFF))
        print("pin pull high, i2c cmd mode")
    else:
        ret = cp2112.HidSmbus_WriteLatch(dev, c_byte(0x00), c_byte(0xFF))
        print("default(internel pull down 100k), beacon boardcast mode")
    return ret, error_info


# dev_addr:21
# reg_addr:0A
#
# beacon enable W:03 84 01 4B 0D
# Tx rate set   W:04 83 00 01 65 0D
# Tx power set  W:03 82 01 0B 0D
# Beacon data   W:1F 86 02 1B FF 01 F1 BE AC 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF 1A EB EB EC DD C3 00 1A 0D


if __name__ == '__main__':

    # 1. judge cp2112 dev status & get num Devices
    numDevices = c_ulong()
    cp2112.HidSmbus_GetNumDevices(numDevices, cp2112_vid, cp2112_pid)
    if numDevices.value != 1:
        print("ERROR: No Device Connected or device over 1, dev_num:", numDevices.value)
        exit()

    # 2. Open cp2112 device
    is_opened = c_bool(False)
    ret, error_info = cp2112.HidSmbus_Open(dev, c_ulong(numDevices.value - 1), cp2112_vid, cp2112_pid)
    cp2112.HidSmbus_IsOpened(dev, is_opened)
    if is_opened.value:
        print("cp2112 is opened")
    else:
        print("cp2112 is not opened")

    # reset smbus??
    # ret, error_info = cp2112.HidSmbus_Reset(dev)
    # print("0 error info:", ret, error_info)

    # 3. set smbus configuration
    time_out = 100  # ms
    set_smbus_config(dev, c_ulong(33000), c_ushort(time_out), c_ushort(3))

    time.sleep(0.1)

    aslan_pack_beacon_thermal_pin(True)
    time.sleep(1)
    aslan_pack_beacon_config()
    time.sleep(1)


#### enable
    aslan_pack_beacon_status(True)
#### disable
    # aslan_pack_beacon_status(False)

    # time.sleep(1)
    # aslan_pack_beacon_thermal_pin(False)
    # time.sleep(6)













    # # 4. write request, need add cancel tansfer ?
    # dev_addr = c_byte(0x16)
    # buffer_array = c_byte * 16
    # buffer = buffer_array(0x80, 0xEE, 0x03, 0x84)
    # n_bytes = c_byte(4)
    # ret, error_info = cp2112.HidSmbus_WriteRequest(dev, dev_addr, pointer(buffer), n_bytes)
    # # print("5 error info:", ret, error_info)
    # time.sleep(time_out * 0.001)
    # # ret, error_info = cp2112.HidSmbus_CancelTransfer(dev)
    #
    # # write other data
    # buffer = buffer_array(0x81, 0xFF, 0x03, 0xAD)
    # n_bytes = c_byte(4)
    # ret, error_info = cp2112.HidSmbus_WriteRequest(dev, dev_addr, pointer(buffer), n_bytes)
    # time.sleep(time_out * 0.001)
    #
    # get_status(dev)
    #
    # # ret, error_info = cp2112.HidSmbus_CancelTransfer(dev)
    # # print("6 error info:", ret, error_info)
    #
    # # 5. reg addr address read
    # # BM191 test ADDR_W:0x16 W:0x80 ADDR_R:0x16 R:0xEE 0x03 0x60(crc8)
    # byte_array = c_byte * 16
    # reg_addr_buf = byte_array(0x81)
    # reg_addr_read(dev, c_byte(0x16), c_byte(1), reg_addr_buf, c_ushort(3))
    #
    # # 6. smbus read function
    # # read(dev, c_byte(0x16), c_ushort(3))
    #
    # # get_status(dev)
    #
    # # 6. test cp2112 gpio
    # cp2112_gpio_test(dev)
    #
    # get_status(dev)
    #
    # # 8. get part number & version
    # cp2112_get_lib_version(dev)
