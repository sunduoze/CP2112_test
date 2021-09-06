# -*- mode: python ; coding: utf-8 -*-
# ASLAN
# dev_addr:21
# reg_addr:0A
#
# beacon enable W:03 84 01 4B 0D
# Tx rate set   W:04 83 00 01 65 0D
# Tx power set  W:03 82 01 0B 0D
# Beacon data   W:1F 86 02 1B FF 01 F1 BE AC 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF 1A EB EB EC DD C3 00 1A 0D

import logging
from ctypes import *
from slab_hid_to_smbus import hid_smbus
import time
from serial_port import serial_read_data_from_cc2540
from config_json import config_file

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
        logging.warning("set_smbus_config fail")
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
    logging.info("smbus config: bitRate:" + str(bitRate.value) + " address:" + str(hex(address.value)) +
                 " transferRetries:" + str(transferRetries.value))
    if (ret != 0):
        logging.info("HidSmbus_GetSmbusConfig ret:" + str(ret) + " error_info:" + str(error_info))
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
    return_status = False
    logging.debug("-------------get_status--")
    ret, error_info = cp2112.HidSmbus_TransferStatusRequest(dev)

    if ret == 0:
        status = c_byte()
        detail_status = c_byte()
        buffer_array = c_byte * 16
        buffer = buffer_array()
        num_retrys = c_ushort()
        bytes_read = c_ushort()
        ret, error_info = cp2112.HidSmbus_GetTransferStatusResponse(dev, status, detail_status, num_retrys, bytes_read)
        # print("A error info:", ret, error_info)

        if status.value == 0x01 or status.value == 0x03:  # display error info
            logging.error(str(status.value) + " " + str(cp2112.status_return_Code[status.value]))
            if status.value == 0x01:  # HID_SMBUS_S0_BUSY
                logging.warning(str(detail_status.value) + " " + str(cp2112.HID_SMBUS_S0_BUSY[detail_status.value]))
            elif status.value == 0x03:  # HID_SMBUS_S0_ERROR
                logging.warning(str(detail_status.value) + " " + str(cp2112.HID_SMBUS_S0_ERROR[detail_status.value]))
            logging.debug("num_retrys" + str(num_retrys.value))
            logging.debug("bytes_read" + str(bytes_read.value))
            # print("num_retrys", num_retrys.value)
            # print("bytes_read", bytes_read.value)
        elif status.value == 0x00:  # HID_SMBUS_S0_IDLE
            logging.info("HID_SMBUS_S0_IDLE" + str(detail_status.value))
            return_status = True
        else:  # HID_SMBUS_S0_COMPLETE
            return_status = True
            # print(detail_status.value)
        logging.debug("-------end of get_status----------")
        logging.debug('This is debug message')
    else:
        logging.error("HidSmbus_TransferStatusRequest FAIL:" + "ret:" + str(ret, ) + "error info:" + str(error_info))
    # print("7 get_status:error info:", return_status)
    return return_status


def addr_write(dev_addr, reg_addr, reg_addr_len, data, data_len):
    time_out = 100
    # write request, need add cancel tansfer ?
    dev_addr = c_byte(dev_addr)
    buffer_array = c_byte * 64
    if reg_addr_len != 0:
        buffer = buffer_array(reg_addr)
        n_bytes = c_byte(reg_addr_len)
        ret, error_info = cp2112.HidSmbus_WriteRequest(dev, dev_addr, pointer(buffer), n_bytes)
        # print("5 error info:", ret, error_info)
        time.sleep(time_out * 0.001)
        # ret, error_info = cp2112.HidSmbus_CancelTransfer(dev)

    buffer = data  # buffer_array(0x03, 0x84, 0x01, 0x4B, 0x0D)
    n_bytes = c_byte(data_len)  # c_byte(5)
    ret, error_info = cp2112.HidSmbus_WriteRequest(dev, dev_addr, pointer(buffer), n_bytes)
    # print("5 error info:", ret, error_info)
    time.sleep(time_out * 0.001)

    return get_status(dev)


def aslan_pack_beacon_config():
    ret = 0
    buffer_array = c_byte * 64

    # Beacon data
    buf = buffer_array(0x0A,
                       0x1F, 0x86, 0x02, 0x1B, 0xFF, 0x01, 0xF1, 0xBE,
                       0xAC, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                       0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
                       0x1A, 0xEB, 0xEB, 0xEC, 0xDD, 0xC3, 0x00, 0x1A,
                       0x0D)
    if addr_write(0x42, 0x00, 0, buf, 34) is False:
        ret = ret + 1
    time.sleep(0.1)

    # Tx power set
    buf = buffer_array(0x0A,
                       0x03, 0x82, 0x01, 0x4B, 0x0D)
    if addr_write(0x42, 0x00, 0, buf, 6) is False:
        ret = ret + 1
    time.sleep(0.1)

    # Tx rate set
    buf = buffer_array(0x0A,
                       0x04, 0x83, 0x00, 0x01, 0x65, 0x0D)
    if addr_write(0x42, 0x00, 0, buf, 7) is False:
        ret = ret + 1
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
    return ret


def aslan_pack_beacon_status(status):
    ret = 0
    if (status is True):
        buffer_array = c_byte * 64
        # beacon enable
        buf = buffer_array(0x0A,
                           0x03, 0x84, 0x01, 0x4B, 0x0D)
        if addr_write(0x42, 0x00, 0, buf, 6) is False:
            ret = ret + 1
        time.sleep(0.1)
        # print("beacon is opened")
        logging.info("beacon is opened")
    else:
        buffer_array = c_byte * 64
        # beacon disable
        buf = buffer_array(0x0A,
                           0x03, 0x84, 0x00, 0x02, 0x0D)
        if addr_write(0x42, 0x00, 0, buf, 6) is False:
            ret = ret + 1
        # print("beacon is closed")
        logging.info("beacon is closed")
    return ret


def aslan_pack_beacon_thermal_pin(status):
    # Set all GPIO to OUTPUT
    # 0xF0: GPIO 0:3 ->input 4:7 ->output
    ret, error_info = cp2112.HidSmbus_SetGpioConfig(dev, c_byte(0xF0), c_byte(0x00), c_byte(0x00), c_byte(0x00))
    # print(ret, error_info)
    if (status is True):
        # 0xF0: GPIO 0:3 ->low 4:4 -> high 5:7 ->low
        ret = cp2112.HidSmbus_WriteLatch(dev, c_byte(0x10), c_byte(0xFF))
        # print("pin pull high, i2c cmd mode")
        logging.info("pin pull high, i2c cmd mode")
    else:
        ret = cp2112.HidSmbus_WriteLatch(dev, c_byte(0x00), c_byte(0xFF))
        # print("default(internel pull down 100k), beacon boardcast mode")
        logging.info("default(internel pull down 100k), beacon boardcast mode")
    return ret, error_info


def enable_beacon_mode():
    ret = 0
    aslan_pack_beacon_thermal_pin(True)
    time.sleep(6)
    if aslan_pack_beacon_config() == 0:
        time.sleep(1)
        #### enable
        if aslan_pack_beacon_status(True) != 0:
            ret = ret + 1
            print("\r\nbreak\r\n")
            aslan_pack_beacon_thermal_pin(False)
            logging.error("enable fail\r\n\r\n")
            return ret
        #### disable
        # aslan_pack_beacon_status(False)

        time.sleep(0.1)
        aslan_pack_beacon_thermal_pin(False)
        time.sleep(6)
        logging.info("-------end of ctrl----------enable\r\n\r\n")
    else:
        ret = ret + 1
        aslan_pack_beacon_thermal_pin(False)
        logging.error("enable fail\r\n\r\n")
    # print("-------end of ctrl----------enable\r\n\r\n")
    return ret


def disable_beacon_mode():
    ret = 0
    aslan_pack_beacon_thermal_pin(True)
    time.sleep(6)
    if aslan_pack_beacon_config() == 0:
        time.sleep(0.1)

        #### enable
        # aslan_pack_beacon_status(True)
        #### disable
        if aslan_pack_beacon_status(False) != 0:
            ret = ret + 1
            print("\r\nbreak\r\n")
            aslan_pack_beacon_thermal_pin(False)
            logging.error("disable fail\r\n\r\n")
            return ret
        time.sleep(1)
        aslan_pack_beacon_thermal_pin(False)
        time.sleep(1)
        logging.info("-------end of ctrl----------disable\r\n\r\n")
    else:
        ret = ret + 1
        aslan_pack_beacon_thermal_pin(False)
        logging.error("disable fail\r\n\r\n")
    return ret


# dev_addr:21
# reg_addr:0A
#
# beacon enable W:03 84 01 4B 0D
# Tx rate set   W:04 83 00 01 65 0D
# Tx power set  W:03 82 01 0B 0D
# Beacon data   W:1F 86 02 1B FF 01 F1 BE AC 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF 1A EB EB EC DD C3 00 1A 0D


from tkinter import *
import tkinter.messagebox  # 弹窗库

LOG_LINE_NUM = 0


class GUI():
    def __init__(self, init_window_name):
        self.init_window_name = init_window_name

    # 设置窗口
    def set_init_window(self):
        self.init_window_name.title("蓝牙Beacon测试程序")  # 窗口名
        self.init_window_name.geometry('700x581+10+10')
        # self.init_window_name["bg"] = "pink"                                    #窗口背景色，其他背景色见：blog.csdn.net/chl0000/article/details/7657887
        # self.init_window_name.attributes("-alpha",0.9)                          #虚化，值越小虚化程度越高
        # 标签
        # self.init_data_label = Label(self.init_window_name, text="按钮按下时，如果软件闪退，说明硬件电路连接存在问题！")
        # self.init_data_label.grid(row=0, column=0)

        # self.result_data_label = Label(self.init_window_name, text="输出结果")
        # self.result_data_label.grid(row=0, column=12)
        # self.log_label = Label(self.init_window_name, text="日志")
        # self.log_label.grid(row=12, column=0)
        # 按钮
        self.enable_beacon_mode_button = Button(self.init_window_name, font=('Arial', 16), text="使能Beacon",
                                                bg="DarkGray", width=40, height=10, command=self.enable_beacon)
        self.enable_beacon_mode_button.grid(row=1, column=1)

        self.enable_beacon_mode_button = Button(self.init_window_name, font=('Arial', 16), text="关闭Beacon",
                                                bg="MediumSeaGreen", width=40, height=10, command=self.disable_beacon)
        self.enable_beacon_mode_button.grid(row=2, column=1)

        if config['aslan']['UI_display']['scan_rssi'] == 'True':
            self.scan_rssi_button = Button(self.init_window_name, font=('Arial', 16), text="scan rssi", bg="DarkGray",
                                           width=10, height=10, command=self.scan_beacon)
            self.scan_rssi_button.grid(row=1, column=2)

    # 功能函数
    def enable_beacon(self):
        for i in range(config['aslan']['enable_beacon']['retry_times']):
            if enable_beacon_mode() == 0:

                rssi_status, rssi_val = serial_read_data_from_cc2540(config['aslan']['ble_scan']['rssi_down_limit'],
                                                                     config['aslan']['ble_scan']['scan_times'],
                                                                     config['aslan']['ble_scan']['scan_cycle'])
                if rssi_status is True:
                    tkinter.messagebox.showinfo('PASS', '良品' + str(rssi_val) + "dB")
                    print('PASS', '良品' + str(rssi_val) + "dB")
                else:
                    tkinter.messagebox.showinfo('失败', '产品不良')
                    print("test fail!")

                # self.init_data_label = Label(self.init_window_name, text="[enable]重试次数:" + str(0))
                # self.init_data_label.grid(row=30, column=0)
                break
            else:
                if config['aslan']['UI_display']['retry_times'] == 'True':
                    self.init_data_label = Label(self.init_window_name, text="[enable]重试次数:" + str(i + 1))
                    self.init_data_label.grid(row=30, column=0)
                    print("[enable]retry times:" + str(i + 1))

    def disable_beacon(self):
        for i in range(config['aslan']['disable_beacon']['retry_times']):
            if disable_beacon_mode() == 0:
                # self.init_data_label = Label(self.init_window_name, text="[disable]重试次数:" + str(0))
                # self.init_data_label.grid(row=60, column=0)
                break
            else:
                if config['aslan']['UI_display']['retry_times'] == 'True':
                    self.init_data_label = Label(self.init_window_name, text="[disable]重试次数:" + str(i + 1))
                    self.init_data_label.grid(row=60, column=0)
                    print("[disable]retry times:" + str(i + 1))

    def scan_beacon(self):
        rssi_status, rssi_val = serial_read_data_from_cc2540(-40, 10, 0.1)
        if rssi_status is True:
            tkinter.messagebox.showinfo('PASS', '良品' + str(rssi_val) + "dB")
            print('PASS', '良品' + str(rssi_val) + "dB")
        else:
            tkinter.messagebox.showinfo('失败', '产品不良')
            print("test fail!")

    # 获取当前时间
    def get_current_time(self):
        current_time = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time()))
        return current_time


def gui_start():
    init_window = Tk()  # 实例化出一个父窗口
    ui = GUI(init_window)
    # 设置根窗口默认属性
    ui.set_init_window()
    init_window.mainloop()  # 父窗口进入事件循环，可以理解为保持窗口运行，否则界面不展示


def log_init():
    log_level = config['log']['level']

    if config['log']['level'] == 'DEBUG':
        log_level = logging.DEBUG
    elif config['log']['level'] == 'INFO':
        log_level = logging.INFO
    elif config['log']['level'] == 'ERROR':
        log_level = logging.ERROR
    elif config['log']['level'] == 'WARNING':
        log_level = logging.WARNING
    else:
        print("out of range, need change code!")

    logging.basicConfig(level=log_level,
                        format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
                        datefmt='%a, %d %b %Y %H:%M:%S',
                        filename='myapp.log',
                        filemode='w')

    from logging.handlers import RotatingFileHandler

    #################################################################################################
    # 定义一个RotatingFileHandler，最多备份5个日志文件，每个日志文件最大10M
    Rthandler = RotatingFileHandler('myapp.log', maxBytes=10 * 1024 * 1024, backupCount=5)
    Rthandler.setLevel(logging.INFO)
    formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
    Rthandler.setFormatter(formatter)
    logging.getLogger('').addHandler(Rthandler)
    ################################################################################################

    #################################################################################################
    # 定义一个StreamHandler，将INFO级别或更高的日志信息打印到标准错误，并将其添加到当前的日志处理对象#
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
    console.setFormatter(formatter)
    logging.getLogger('').addHandler(console)
    #################################################################################################

    # logging.debug('This is debug message')
    # logging.info('This is info message')
    # logging.warning('This is warning message')


def get_config_file():
    res = False
    dict_temp = ''
    try:
        cfg_dict = config_file()
        status = cfg_dict.file_is_null()
        if status is False:
            print("file is full")
            dict_temp = cfg_dict.read()
        else:
            print("file is null")
            cfg_dict.add(cfg_dict.str_to_dict(cfg_dict.init_dict))
            dict_temp = cfg_dict.read()
        res = True
    except Exception as e:
        print("list_port Exception:", e)

    return res, dict_temp


if __name__ == '__main__':

    ret_temp, config = get_config_file()
    if ret_temp is False:
        logging.error("ERROR: get_config_file fail")
        exit()
    # print('--------------------------')
    # print(config['aslan']['enable_beacon']['retry_times'])

    log_init()

    # 1. judge cp2112 dev status & get num Devices
    numDevices = c_ulong()
    cp2112.HidSmbus_GetNumDevices(numDevices, cp2112_vid, cp2112_pid)
    if numDevices.value != 1:
        logging.error("ERROR: No Device Connected or device over 1")
        print("ERROR: No Device Connected or device over 1, dev_num:", numDevices.value)
        exit()

    # 2. Open cp2112 device
    is_opened = c_bool(False)
    ret, error_info = cp2112.HidSmbus_Open(dev, c_ulong(numDevices.value - 1), cp2112_vid, cp2112_pid)
    cp2112.HidSmbus_IsOpened(dev, is_opened)
    if is_opened.value:
        # print("cp2112 is opened")
        logging.debug("cp2112 is opened")
    else:
        # print("cp2112 is not opened")
        logging.error("cp2112 is not opened")
    # 3. set smbus configuration
    time_out = 100  # ms
    set_smbus_config(dev, c_ulong(33000), c_ushort(time_out), c_ushort(3))

    time.sleep(0.1)

    #  UI start loop
    gui_start()
