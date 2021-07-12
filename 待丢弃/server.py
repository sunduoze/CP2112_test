from ctypes import *
from flask import Flask, request
from flask_cors import CORS
from SLABHIDtoSMBUS import HidSmbus

from slab_hid_to_smbus import hid_smbus
import time

CP2112_VID = c_ushort(0x10C4)
CP2112_PID = c_ushort(0xEA90)

cp2112 = HidSmbus()

cp2112_smbus = hid_smbus()
cp2112_vid = c_uint16(0x10C4)
cp2112_pid = c_uint16(0xEA90)

# devicePtr = pointer(c_int())
app = Flask(__name__)

CORS(app)

@app.route('/')
def index():
    return "OK"

@app.route('/WriteLatch')
def WriteLatch():
    port = int(request.args.get("port"))
    value = int(request.args.get("value"))

    if (port < 0 or port > 7):
        return "Bad Request", 400

    mask = c_byte(1 << port)
    if (value == 0):
        cp2112.WriteLatch(devicePtr, c_byte(0x00), mask)
    else:
        cp2112.WriteLatch(devicePtr, c_byte(0xFF), mask)
    return "OK"

@app.route('/I2CSend')
def I2CSend():
    address = int(request.args.get("address"), 16)
    data = bytes.fromhex(request.args.get("data"))
    length = len(data)

    BufferArray = c_byte * length
    dataArray = [d for d in data]
    buffer = BufferArray(*dataArray)

    ret = cp2112.WriteRequest(devicePtr, c_byte(address), pointer(buffer), c_ushort(length))
    return "Status: " + str(ret)

@app.route('/I2CReceive')
def I2CReceive():
    address = int(request.args.get("address"), 16)
    length = int(request.args.get("length"))

    BufferArray = c_byte * cp2112.I2C_RECEIVE_BUFFER_SIZE_INT
    buffer = BufferArray()

    status = c_byte()
    numBytesRead = c_byte()

    cp2112.ReadRequest(devicePtr, c_byte(address), c_ushort(length))
    cp2112.ForceReadResponse(devicePtr)
    cp2112.GetReadResponse(devicePtr, pointer(status), pointer(buffer), byref(numBytesRead))

    value = 0
    for i in range(length):
        value = (value << 8) | (buffer[i] & 0xFF)

    return str(value)

def cp2112_gpio_test(refDevice):
    # Set all GPIO to OUTPUT
    ret, error_info = cp2112.SetGpioConfig(refDevice, c_byte(0xFF), c_byte(0x00), c_byte(0x00), c_byte(0x00))
    # print(ret, error_info)

    # Set or Reset GPIO status
    for i in range(10):
        ret = cp2112.WriteLatch(refDevice, c_byte(0xFF), c_byte(0xFF))
        time.sleep(0.05)
        ret = cp2112.WriteLatch(refDevice, c_byte(0x00), c_byte(0xFF))
        time.sleep(0.05)
    return ret, error_info

def cp2112_get_lib_version():
    status1 = c_byte()
    status2 = c_byte()
    status3 = c_byte()
    cp2112.get_library_version(pointer(status1), pointer(status2), pointer(status3))
    print("lib version:", status1, status2, status3)


if __name__ == '__main__':

    # 1.print cp2112 lib version
    cp2112_get_lib_version()

    # 2.judge cp2112 dev status
    refNumDevices = c_uint()
    cp2112.GetNumDevices(pointer(refNumDevices), CP2112_VID, CP2112_PID)
    if refNumDevices.value != 1:
        print("ERROR: No Device Connected or device over 1, dev_num:", refNumDevices)
        exit()

    # 3.Open device
    dev_ptr = pointer(c_int())
    if (cp2112.Open(pointer(dev_ptr), c_uint(0), CP2112_VID, CP2112_PID) == 0):
        print("open device success")
    else:
        print("open device fail ")

    # 4.test cp2112 gpio
    cp2112_gpio_test(dev_ptr)

    # set smbus config ( baudrate addr autoReadRespond... )
    ret, error_info = cp2112.set_smbus_config(dev_ptr, c_int(100000), c_byte(0x16), c_bool(True),
                                              c_int(100), c_int(100), c_bool(False), c_int(3)) # send retry-send times = 3
    if(ret != 0):
        print("config error info:", ret, error_info)

    # 5. read request
    # hexstr = '3C2A55'
    # address = int("16", 16)
    # length = 2
    # ret, error_info = cp2112.ReadRequest(dev_ptr, c_byte(address), c_ushort(length))
    # print("1 error info:", ret, error_info)

    # 6. read response
    # status = c_byte()
    # numBytesRead = c_byte()
    # BufferArray = c_byte * cp2112.I2C_RECEIVE_BUFFER_SIZE_INT
    # buffer = BufferArray()
    # ret, error_info = cp2112.GetReadResponse(dev_ptr, pointer(status), pointer(buffer), byref(numBytesRead))
    # print("3 error info:", ret, error_info)
    # print(BufferArray)

    # force read
    ret, error_info = cp2112.ForceReadResponse(dev_ptr)
    print("2 error info:", ret, error_info)


    # address read
    # ubyte_array = c_ubyte * 16
    # tar_addr_buf = ubyte_array(0x3C, 0x00)

    # ubyte_3array = c_uint8 * 16
    # reg_addr_buf = ubyte_3array(0x3C, 0, 0)
    #
    # ret, error_info = cp2112.AddressReadRequest(dev_ptr, c_uint8(0x16), c_uint16(2),
    #                                             c_uint8(1), reg_addr_buf)
    # print("A error info:", ret, error_info)

    # ret = cp2112.AddressReadRequest(devicePtr, c_byte(address), byref(buffer), c_ushort(length))

    # write request
    # hexstr = '3C' # 2A55'
    # address = int("16", 16)
    # data = bytes.fromhex(hexstr)
    # length = len(data)
    # BufferArray = c_byte * length
    # dataArray = [i for i in data]
    # buffer = BufferArray(*dataArray)
    # ret2, error_info = cp2112.WriteRequest(dev_ptr, c_byte(address), pointer(buffer), c_ushort(length))
    # print("4 error info:", ret2, error_info)

    # write request
    ubyte_array = c_ubyte * 8
    length = 1
    buffer2 = ubyte_array(0x3C)
    ret2, error_info = cp2112.WriteRequest(dev_ptr, c_byte(0x16), pointer(buffer2), c_ushort(length))
    print("5 error info:", ret2, error_info)

    # 5. read request
    address = int("16", 16)
    length = 2
    ret, error_info = cp2112.ReadRequest(dev_ptr, c_byte(address), c_ushort(length))
    print("1 error info:", ret, error_info)

# BM191 test
# ADDR_W:0x0B
# W:0x3C
# ADDR_R:0x0B
# R:0xD7
# R:0x0B
#   app.run(debug=True)