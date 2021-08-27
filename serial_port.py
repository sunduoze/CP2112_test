import sys
import threading
import datetime
import serial
import serial.tools.list_ports
import time
from serial.tools import list_ports_common
import re

DATA = ""  # 读取的数据
NOEND = True  # 是否读取结束

# def log_init():
file_name = str(sys.argv[0])
log_file = open(file_name + '.log', 'w')
data_file = open(file_name + '.csv', 'w')


# 读数据的本体
def read_data(ser):
    global DATA, NOEND

    # 循环接收数据（此为死循环，可用线程实现）
    while NOEND:
        if ser.in_waiting:
            # DATA = ser.read(ser.in_waiting).decode("gbk")
            DATA = ser.readline().decode("gbk")
            # print(DATA)
            # print("\n>> receive: ", DATA, "\n>>", end="")
            if (DATA == "quit" or DATA == "quit\n"):
                print("oppo seri has closen.\n>>", end="")


# 列出串口清单
def list_seri():
    ret = False
    try:

        port_list = list(serial.tools.list_ports.comports())
        print(port_list)

        if len(port_list) == 0:
            test_str = r'无可用串口！\n'
            print(test_str)
            test_str = '无可用串口！\n'
            print(test_str)
            ret = False
        else:
            ret = True
            for i in range(0, len(port_list)):
                print(port_list[i])
                str_temp = ''.join(port_list[i])
                if 'Prolific USB-to-Serial Comm Port' in str_temp:
                    print(str_temp)
                # print(serial.tools.list_ports.comports())

    except Exception as e:
        print("error!", e)
    return ret


def list_and_bind(string_dercription, baudrate, timeout):
    ret_val = False
    serial_port = ""
    try:
        port_list = list(serial.tools.list_ports.comports())
        # print(port_list)

        if len(port_list) == 0:
            test_str = r'无可用串口！\n'
            print(test_str)
            test_str = '无可用串口！\n'
            print(test_str)
            ret_val = False
        else:
            for i in range(0, len(port_list)):
                print(port_list[i])
                str_temp = ''.join(port_list[i])
                if string_dercription in str_temp:
                    # print("bind:" + str_temp)
                    import re
                    # res = re.search(r'(?P<province>\d{3})(?P<city>\d{3})(?P<born_year>\d{4})', '1102231990xxxxxxxx')
                    # print(res.groupdict())

                    com_x = re.search(r'COM\d{1,2}', str_temp)
                    serial_port, ret_val = open_seri(com_x.group(), baudrate, timeout=timeout)
                    if ret_val is True:
                        print("bind-ed:" + com_x.group())
    except Exception as e:
        print("list_and_bind error!", e)
    return serial_port, ret_val


# 打开串口
def open_seri(port_x, bps, timeout):
    serial_port = ""
    ret_val = False
    try:
        # 打开串口，并得到串口对象
        serial_port = serial.Serial(port_x, bps, timeout=timeout)
        # 判断是否成功打开
        if serial_port.is_open:
            ret_val = True
            th = threading.Thread(args=(serial_port,), target=read_data)  # 创建一个子线程去等待读数据
            th.start()
    except Exception as e:
        print("error! can not open serial port", e)
    return serial_port, ret_val

# 关闭串口
def close_seri(ser):
    global NOEND
    NOEND = False
    ser.close()


# 写数据
def write_to_seri(ser, text):
    res = ser.write(text.encode("gbk"))  # 写
    return res


# 读数据
def read_from_seri():
    global DATA
    data = DATA
    DATA = ""  # 清空当次读取
    return data

def serial_read_data_from_cc2540(ble_mac_addr, rssi_down_limit):
    global DATA
    mac_addr_set_status = False
    scan_status = False
    rssi_status = False
    rssi_val = 0

    ser, ret = list_and_bind('Prolific USB-to-Serial Comm Port', 115200, 100)
    for i in range(2):
        write_to_seri(ser, "set_addr:" + ble_mac_addr + "\r\n")
        time.sleep(0.1)
        mac_addr = "0x" + ble_mac_addr
        if re.match(mac_addr, DATA) is not None:
            mac_addr_set_status = True
            print("mac addr:" + mac_addr)
            break

    if mac_addr_set_status is True:

        for i in range(10):
            write_to_seri(ser, "scan_beacon\r\n")
            time.sleep(0.1)
            if re.match('scaned', DATA) is not None:
                DATA = ""
                print("scaned")
                scan_status = True
                break

        if scan_status is True:
            for i in range(3):
                write_to_seri(ser, "get_rssi\r\n")
                time.sleep(0.1)
                if re.match(r'rssi', DATA) != None:
                    rssi_x = re.search(r'\d{1,2}', DATA)
                    DATA = ""
                    rssi_val = int(rssi_x.group()) - 100
                    if(rssi_val > rssi_down_limit):
                        rssi_status = True
                        print("rssi:%d" % rssi_val + "dB")
                        break
                    else:
                        print("rssi too low,rssi:%d" % rssi_val + "dB")
            if rssi_status is not True:
                print("rssi:%d" % rssi_val + "dB")
        else:
            print("scan_beacon error,data:" + DATA)
    else:
        print("mac addr set error,data:" + DATA)

    close_seri(ser)
    return rssi_status, rssi_val


if __name__ == "__main__":

    rssi_status, rssi_val = serial_read_data_from_cc2540("209148552FC1", -70)
    if rssi_status is True:
        print("----rssi:%d" % rssi_val + "dB")
    else:
        print("test fail!")
