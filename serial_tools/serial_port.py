# -*- mode: python ; coding: utf-8 -*-
import time

import serial
import serial.tools.list_ports
import threading
import re

import lupa

debug_status = False


def debug(*kwargs):
    if debug_status:
        print(*kwargs)


class serial_class(object):
    # def __init__(self, com, bps_list, parity, data_bits, stop_bits):
    #     self.conn_status = False
    #     self.com = com
    #     self.bps = bps_list[0]
    #     self.parity = parity[0]
    #     self.data_bits = data_bits
    #     self.stop_bits = stop_bits

    # self.sendString = 'test RS485'
    # self.numSend = 0
    # self.uartState = False
    # # self.serialOpen()
    def open_port(self, com=None, desc=None, bps=115200, timeout=100):
        serial_port = ""
        res = False
        if com is not None:
            res, serial_port = self.new_port(com, bps, timeout=timeout)
        elif desc is not None:
            ret_val, port_list = self.list_port
            if ret_val is True:
                for i in range(0, len(port_list)):
                    desc_temp = ''.join(port_list[i])
                    if desc in desc_temp:
                        com_x = re.search(r'COM\d{1,2}', desc_temp)
                        res, serial_port = self.new_port(com_x.group(), bps, timeout=timeout)
                        if res is True:
                            debug("bind-ed:" + com_x.group())
                            break
                        else:
                            print("bind " + com_x.group() + " fail!")
                    elif i >= (len(port_list) - 1):
                        print("not match,out of list, pls list_port()")
                        res = False
            else:
                print("list_port fail! ")
                res = False
        else:
            res = False
        return res, serial_port

    def write(self, serial_port, text=None, str_val=None, hex_str=None):
        res = False
        if str_val is None and hex_str is None:
            res = serial_port.write(text.encode("gbk"))  # 写
        return res

    @property
    def list_port(self):
        res = False
        ret_list = ""
        try:
            port_list = list(serial.tools.list_ports.comports())
            if len(port_list) == 0:
                debug('无可用串口！\n')
                res = False
            else:
                res = True
                ret_list = port_list
                for i in range(0, len(port_list)):
                    debug(port_list[i])
        except Exception as e:
            print("list_port Exception:", e)
        return res, ret_list

    def close_port(self, ser_port: serial.SerialBase):
        res = False
        try:
            res = ser_port.close()
            # debug("close serial port:" + ser_port)
        except Exception as e:
            print("error! close_port:", e)
        return res

    def read_data(self, ser):
        global DATA, NOEND

        # 循环接收数据（此为死循环，可用线程实现）
        while NOEND:
            if ser.in_waiting:
                # DATA = ser.read(ser.in_waiting).decode("gbk")
                DATA = ser.readline().decode("gbk")
                print(DATA)
                # print("\n>> receive: ", DATA, "\n>>", end="")
                if (DATA == "quit" or DATA == "quit\n"):
                    print("seri has closen.\n>>", end="")

    def new_port(self, com=None, bps=115200, timeout=100):
        serial_port = ""
        ret_val = False
        if com is not None:
            try:
                serial_port = serial.Serial(com, bps, timeout=timeout)  # 打开串口，并得到串口对象
                if serial_port.is_open:  # 判断是否成功打开
                    ret_val = True
                    t = threading.Thread(args=(serial_port,), target=self.read_data)  # 创建一个子线程去等待读数据
                    t.setDaemon(True)  # 守护线程
                    t.start()
                    # t.join(1)
            except Exception as e:
                print("error! can not open serial port", e)
        return ret_val, serial_port


DATA = ""  # 读取的数据
NOEND = True  # 是否读取结束

# 读数据
def read_from_seri():
    global DATA
    data = DATA
    DATA = ""  # 清空当次读取
    return data


if __name__ == "__main__":
    sp = serial_class()
    ret, pt_list = sp.list_port
    print(ret)
    for i in range(0, len(pt_list)):
        print(pt_list[i])
    # ret, pt = sp.open_port(com="COM3", bps=115200)
    ret, pt = sp.open_port(desc="ELTIMA Virtual Serial Port (COM2->COM1)", bps=115200)
    print(ret)
    # print(pt)
    sp.write(pt, "scan_beacon\r\n")
    time.sleep(1)
    print(read_from_seri())
    # sp.close_port(pt)

    while True:
        i = 1
