# -*- mode: python ; coding: utf-8 -*-
import logging
import time
import threading

import tkinter as tk
import tkinter.font as tkFont
from tkinter import ttk
from tkinter.scrolledtext import ScrolledText

from serial_port import *
from serial_tools.serial_port import serial_class


class GUI(tk.Tk):
    def __init__(self, *args, **kw):
        super().__init__()
        self.ft1 = tkFont.Font(family='宋体', size=10, weight='bold')
        self.ft2 = tkFont.Font(family='宋体', size=10, slant='italic')
        self.ft3 = tkFont.Font(family='宋体', size=10)  # , overstrike=1, underline=1,
        self.ft4 = tkFont.Font(family='宋体', size=8)

        self.port = serial_class()

        self.start_row = 30
        self.start_column = 0
        self.bt_bg = "DarkGray"
        # 设置根窗口默认属性
        self.set_init_window()
        self.init_menu()
        self.refresh_data()
        self.mainloop()  # 父窗口进入事件循环，可以理解为保持窗口运行，否则界面不展示

    def init_menu(self):
        # 菜单
        self.com_port_val = tk.IntVar()
        self.com_port_val.set(2)
        self.com_port_mn = tk.Menu(self, font=self.ft3, tearoff=0)
        self.com_port_mn.add_command(label="刷新串口列表")
        self.com_port_mn.add_separator()
        self.com_port_mn.add_command(label="TCP Cilent")
        self.com_port_mn.add_command(label="TCP Server")
        self.com_port_mn.add_command(label="UDP")
        self.com_port_mn.add_separator()
        self.com_port_mn.add_radiobutton(label="COM1", variable=self.com_port_val, value=1)
        self.com_port_mn.add_radiobutton(label="COM2", variable=self.com_port_val, value=2)

        self.com_setting_mn = tk.Menu(self, font=self.ft3, tearoff=0)
        self.com_setting_mn.add_command(label="打开串口设置")
        self.com_setting_mn.add_separator()
        self.com_setting_mn.add_checkbutton(label="启动立即打开串口")
        self.com_setting_mn.add_separator()
        self.com_setting_mn.add_command(label="帮助")

        self.disp_buff_size_val = tk.IntVar()
        self.disp_buff_size_val.set(1000000)
        self.disp_buff_size = tk.Menu(self, font=self.ft3, tearoff=0)
        self.disp_buff_size.add_radiobutton(label="200k", variable=self.disp_buff_size_val, value=200000)
        self.disp_buff_size.add_radiobutton(label="500k", variable=self.disp_buff_size_val, value=500000)
        self.disp_buff_size.add_radiobutton(label="1M", variable=self.disp_buff_size_val, value=1000000)
        self.disp_buff_size.add_radiobutton(label="2M", variable=self.disp_buff_size_val, value=2000000)
        self.disp_buff_size.add_radiobutton(label="5M", variable=self.disp_buff_size_val, value=5000000)

        self.disp_mn = tk.Menu(self, font=self.ft3, tearoff=0)
        self.disp_mn.add_command(label="字体")
        self.disp_mn.add_command(label="背景")
        self.disp_mn.add_separator()
        self.disp_mn.add_cascade(label="显示缓冲上限", menu=self.disp_buff_size)
        self.disp_mn.add_separator()
        self.disp_mn.add_command(label="帮助")

        self.terminal_sim_set_val = tk.IntVar()
        self.terminal_sim_set_val.set(2)
        self.terminal_sim_set_mn = tk.Menu(self, font=self.ft3, tearoff=0)
        self.terminal_sim_set_mn.add_radiobutton(label="回车后发送输入行(带回显)",
                                                 variable=self.terminal_sim_set_val, value=1)
        self.terminal_sim_set_mn.add_separator()
        self.terminal_sim_set_mn.add_radiobutton(label="按键立即发送该键值", variable=self.terminal_sim_set_val, value=2)
        self.terminal_sim_set_mn.add_checkbutton(label="本地回显按键值")
        self.terminal_sim_set_mn.add_separator()
        self.terminal_sim_set_mn.add_checkbutton(label="发送回车0x0D后，自动加上换行符0x0A")

        self.send_file_delay_val = tk.IntVar()
        self.send_file_delay_val.set(0)
        self.send_file_delay_mn = tk.Menu(self, font=self.ft3, tearoff=0)
        self.send_file_delay_mn.add_radiobutton(label="连续发送",
                                                variable=self.send_file_delay_val, value=0)
        self.send_file_delay_mn.add_radiobutton(label="每发送256字节延时1ms",
                                                variable=self.send_file_delay_val, value=1)
        self.send_file_delay_mn.add_radiobutton(label="每发送256字节延时10ms",
                                                variable=self.send_file_delay_val, value=10)
        self.send_file_delay_mn.add_radiobutton(label="每发送256字节延时50ms",
                                                variable=self.send_file_delay_val, value=50)
        self.send_file_delay_mn.add_radiobutton(label="每发送256字节延时100ms",
                                                variable=self.send_file_delay_val, value=100)

        self.send_config_mn = tk.Menu(self, font=self.ft3, tearoff=0)
        # 1的话多了一个虚线，如果点击的话就会发现，这个菜单框可以独立出来显示
        self.send_config_mn.add_cascade(label="终端仿真设置", menu=self.terminal_sim_set_mn)
        self.send_config_mn.add_separator()
        self.send_config_mn.add_cascade(label="发送文件延时设置", menu=self.send_file_delay_mn)
        self.send_config_mn.add_separator()
        self.send_config_mn.add_command(label="帮助")

        self.tools_mn = tk.Menu(self, font=self.ft3, tearoff=0)
        self.tools_mn.add_command(label="数据波形(示波器)")
        self.tools_mn.add_command(label="STM/GD32 ISP 烧写器")
        self.tools_mn.add_command(label="STC/IAP15 ISP 烧写器")
        self.tools_mn.add_command(label="转换计算器")
        self.tools_mn.add_command(label="lua脚本")

        self.menu_bar = tk.Menu(self)
        self.menu_bar.add_cascade(label="通讯端口", menu=self.com_port_mn)
        self.menu_bar.add_cascade(label="串口设置", menu=self.com_setting_mn)
        self.menu_bar.add_cascade(label="显示", menu=self.disp_mn)
        self.menu_bar.add_cascade(label="发送", menu=self.send_config_mn)
        self.menu_bar.add_cascade(label="多字符串")
        self.menu_bar.add_cascade(label="小工具", menu=self.tools_mn)
        self.menu_bar.add_command(label="帮助", command=self.quit)
        self.config(menu=self.menu_bar)

    # 设置窗口
    def set_init_window(self):
        self.title("蓝牙Beacon测试程序")  # 窗口名
        self.geometry('790x566+10+10')
        # self.init_window_name["bg"] = "pink"                                    #窗口背景色，
        # 其他背景色见：blog.csdn.net/chl0000/article/details/7657887
        # self.init_window_name.attributes("-alpha",0.9)                          #虚化，值越小虚化程度越高

        # 下拉菜单
        # comvalue = tkinter.StringVar()  # 窗体自带的文本，新建一个值
        self.comboxlist = ttk.Combobox(self, font=self.ft1, width=24)
        self.comboxlist["values"] = ("COM1", "COM2", "COM3-----")
        self.comboxlist.current(0)  # 选择第一个
        self.comboxlist.bind("<<ComboboxSelected>>", self.scan_beacon())  # 绑定事件,(下拉列表框被选中时，绑定go()函数)
        self.comboxlist.grid(row=self.start_row + 1, column=0, columnspan=3)

        self.baud_cbx = ttk.Combobox(self, font=self.ft1, width=8)
        self.baud_cbx["values"] = ("9600", "38400", "57600", "115200", "230400", "500000", "1000000")
        self.baud_cbx.current(3)  # 选择第一个
        self.baud_cbx.bind("<<ComboboxSelected>>", self.scan_beacon())  # 绑定事件,(下拉列表框被选中时，绑定go()函数)
        self.baud_cbx.grid(row=self.start_row + 4, column=2, columnspan=3, sticky='w')
        # get()获取选中的值

        # 标签
        self.timer_send_freq_label = tk.Label(self, text="ms/次")
        self.timer_send_freq_label \
            .grid(row=self.start_row + 1, column=9, sticky='e')

        self.timeout_label = tk.Label(self, text="ms 超时时间")
        self.timeout_label.grid(row=self.start_row + 2, column=5, sticky='w')

        self.check4_label = tk.Label(self, text="校验方式:")
        self.check4_label.grid(row=self.start_row + 2, column=6, sticky='w')

        self.send_cnt_label = tk.Label(self, text="发送个数:")
        self.send_cnt_label.grid(row=self.start_row + 5, column=0)
        self.recv_cnt_label = tk.Label(self, text="接收个数:")
        self.recv_cnt_label.grid(row=self.start_row + 6, column=0)
        # 按钮
        # #################### row 0 ######################
        self.clear_get_bt = tk.Button(self, font=self.ft1, text="清除窗口",
                                      bg=self.bt_bg, command=self.enable_beacon) \
            .grid(row=self.start_row + 0, column=0, sticky='w')
        self.open_file_bt = tk.Button(self, font=self.ft1, text="打开文件",
                                      bg=self.bt_bg, command=self.enable_beacon).grid(row=self.start_row + 0, column=1)
        self.send_file_bt = tk.Button(self, font=self.ft1, text=" 发送文件 ",
                                      bg=self.bt_bg, command=self.enable_beacon) \
            .grid(row=self.start_row + 0, column=2, sticky='w')
        self.stop_file_bt = tk.Button(self, font=self.ft1, text="停止",
                                      bg=self.bt_bg, command=self.enable_beacon) \
            .grid(row=self.start_row + 0, column=6)
        self.clear_sent_bt = tk.Button(self, font=self.ft1, text="清发送区",
                                       bg=self.bt_bg, command=self.enable_beacon).grid(row=self.start_row + 0, column=7)
        self.save_para_bt = tk.Button(self, font=self.ft1, text="保存参数",
                                      bg=self.bt_bg, command=self.enable_beacon) \
            .grid(row=self.start_row + 0, column=10, sticky='w')
        self.extend_sent_bt = tk.Button(self, font=self.ft1, text="扩展",
                                        bg=self.bt_bg, command=self.enable_beacon) \
            .grid(row=self.start_row + 0, column=11, sticky='w')
        self.folding_bt = tk.Button(self, font=self.ft1, text="-",
                                    bg=self.bt_bg, command=self.enable_beacon) \
            .grid(row=self.start_row + 0, column=12, sticky='w')

        # #################### row 1 ######################
        self.save_data_bt = tk.Button(self, font=self.ft1, text="保存数据",
                                      bg=self.bt_bg, command=self.enable_beacon) \
            .grid(row=self.start_row + 1, column=4, sticky='w')
        self.help_bt = tk.Button(self, font=self.ft1, text="?",
                                 bg=self.bt_bg, command=self.enable_beacon) \
            .grid(row=self.start_row + 1, column=12, sticky='w')
        # #################### row 2 ######################
        self.open_close_bt_val = tk.StringVar()
        self.open_close_bt_val.set("打开串口")
        self.open_bt = tk.Button(self, font=self.ft1, textvariable=self.open_close_bt_val,
                                 bg=self.bt_bg, command=self.open_port).grid(row=self.start_row + 2, column=0)
        self.open_bt = tk.Button(self, )
        self.scan_bt = tk.Button(self, font=self.ft1, text=" 扫描 ",
                                 bg=self.bt_bg, command=self.scan_serial).grid(row=self.start_row + 2, column=1)
        self.port_set_bt = tk.Button(self, font=self.ft1, text="更多串口设置", width=10,
                                     bg=self.bt_bg, command=self.enable_beacon) \
            .grid(row=self.start_row + 2, column=2, sticky='w')

        # #################### row 5 ######################
        self.send_bt = tk.Button(self, font=self.ft1, text="发送",
                                 bg=self.bt_bg, command=self.enable_beacon, height=2, width=9) \
            .grid(row=self.start_row + 5, column=2, columnspan=1, rowspan=2)

        # 文本框
        self.send_recv_text = ScrolledText(self, height=30, width=110) \
            .grid(row=0, column=0, columnspan=15, rowspan=30, sticky='w')

        self.open_file_text = tk.Text(self, height=1, width=30) \
            .grid(row=self.start_row + 0, column=3, columnspan=3, sticky='w')

        self.timer_send_freq_text = tk.Text(self, height=1, width=5) \
            .grid(row=self.start_row + 1, column=8, sticky='w')

        self.time_out_ms_text = tk.Text(self, height=1, width=10)
        self.time_out_ms_text.grid(row=self.start_row + 2, column=4, sticky='w')
        self.time_out_ms_text.insert('0.0', '1234')
        # self.timer_send_freq_label = Label(self.time_out_ms_text, text="ms", height=1)
        # self.time_out_ms_text.window_create(INSERT, window=self.timer_send_freq_label)

        self.check_text = tk.Text(self, height=1, width=38) \
            .grid(row=self.start_row + 2, column=7, columnspan=6, sticky='w')

        self.send_area_text = tk.Text(self, height=4) \
            .grid(row=self.start_row + 4, column=3, columnspan=10, rowspan=4, sticky='w')

        self.tx_count_text = tk.Text(self, width=9, height=1)
        self.tx_count_text.grid(row=self.start_row + 5, column=1, columnspan=1, sticky='w')
        self.tx_count_text.insert('0.0', '0')

        self.rx_count_text = tk.Text(self, width=9, height=1)
        self.rx_count_text.grid(row=self.start_row + 6, column=1, columnspan=1, sticky='w')
        self.rx_count_text.insert('0.0', '0')

        self.dev_connect_info_text = tk.Text(self, width=50, height=1)
        self.dev_connect_info_text.grid(row=self.start_row + 9, column=0, columnspan=5, sticky='w')
        self.dev_connect_info_text.insert('0.0', 'COM12 已关闭 115200bps,8,1,None,None')
        # self.other_info_text = Text(self, width=30, height=1)
        # self.other_info_text.grid(row=self.start_row + 9, column=8, columnspan=3, sticky='w')
        # self.other_info_text.insert('0.0', '0')

        # 复选框
        # #################### row 0 ######################
        top_val = tk.IntVar()
        self.top_bt = tk.Checkbutton(self, text="顶", variable=top_val,  # anchor='w',
                                     onvalue=1, offvalue=0) \
            .grid(row=self.start_row + 0, column=8, sticky='w')
        eng_val = tk.IntVar()
        self.eng_bt = tk.Checkbutton(self, text="英", variable=eng_val,  # anchor='w',
                                     onvalue=1, offvalue=0) \
            .grid(row=self.start_row + 0, column=9, sticky='w')
        # #################### row 1 ######################
        hex_disp_val = tk.IntVar()
        self.hex_disp_bt = tk.Checkbutton(self, text="HEX显", variable=hex_disp_val,
                                          onvalue=1, offvalue=0) \
            .grid(row=self.start_row + 1, column=3, sticky='w')
        save_data_to_file_val = tk.IntVar()
        self.save_data_bt = tk.Checkbutton(self, text="存文件", variable=save_data_to_file_val,
                                           onvalue=1, offvalue=0) \
            .grid(row=self.start_row + 1, column=5, sticky='w')
        hex_send_val = tk.IntVar()
        self.hex_send_bt = tk.Checkbutton(self, text="HEX发", variable=hex_send_val,
                                          onvalue=1, offvalue=0) \
            .grid(row=self.start_row + 1, column=6, sticky='w')
        timer_send_val = tk.IntVar()
        self.timer_send_bt = tk.Checkbutton(self, text="定时发送", variable=timer_send_val,
                                            onvalue=1, offvalue=0) \
            .grid(row=self.start_row + 1, column=7, sticky='w', columnspan=2)
        add_enter_wrap_val = tk.IntVar()
        self.add_enter_wrap_bt = tk.Checkbutton(self, text="加回车换行", variable=add_enter_wrap_val,
                                                onvalue=1, offvalue=0) \
            .grid(row=self.start_row + 1, column=10, columnspan=3, sticky='w')

        # #################### row 2 ######################
        add_time_stamp_diff_pack_disp_val = tk.IntVar()
        self.add_time_stamp_diff_pack_disp_bt = tk.Checkbutton(self, text="时间戳",
                                                               variable=add_time_stamp_diff_pack_disp_val,
                                                               onvalue=1, offvalue=0) \
            .grid(row=self.start_row + 2, column=3, sticky='w')

        # #################### row 4 ######################
        rts_val = tk.IntVar()
        self.rts_bt = tk.Checkbutton(self, text="RTS", variable=rts_val, anchor='w',
                                     onvalue=1, offvalue=0) \
            .grid(row=self.start_row + 4, column=0, sticky='w')
        dtr_val = tk.IntVar()
        self.dtr_bt = tk.Checkbutton(self, text="DTR", variable=dtr_val, anchor='w',
                                     onvalue=1, offvalue=0) \
            .grid(row=self.start_row + 4, column=1, sticky='w')

    # 功能函数
    def enable_beacon(self):
        print("enable_beacon")

    def disable_beacon(self):
        print("[disable]retry times:")

    def scan_beacon(self):
        print("scan_beacon")

    def open_port(self):
        global serial_pt
        if self.open_close_bt_val.get() == "打开串口":
            com_x = re.search(r'COM\d{1,2}', self.comboxlist.get())
            ret, serial_pt = self.port.open_port(com=com_x.group(), bps=self.baud_cbx.get())
            if ret is True:
                self.open_close_bt_val.set("关闭串口")
                self.port.write(serial_pt, "open port\r\n" + com_x.group() + " " + self.baud_cbx.get())
            else:
                print("open port fail!")
        else:
            ret = self.port.close_port(serial_pt)
            if ret is True:
                self.open_close_bt_val.set("打开串口")
            else:
                print("close port fail!")


    def scan_serial(self):
        ret, pt_list = self.port.list_port
        self.comboxlist["values"] = tuple(pt_list)

    def refresh_data(self):
        ret, pt_list = self.port.list_port
        self.comboxlist["values"] = tuple(pt_list)

        self.after(10000, self.refresh_data)  # 1000ms

    # # 获取当前时间
    # def get_current_time(self):
    #     current_time = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time()))
    #     return current_time


# def gui_start():
#     # init_window = tk.Tk()  # 实例化出一个父窗口
#     ui = GUI()
#     # # 设置根窗口默认属性
#     # ui.set_init_window()
#     # ui.init_menu()
#     # init_window.mainloop()  # 父窗口进入事件循环，可以理解为保持窗口运行，否则界面不展示


def log_init():
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
                        datefmt='%a, %d %b %Y %H:%M:%S',
                        filename='main.py.log',
                        filemode='w')

    from logging.handlers import RotatingFileHandler

    #################################################################################################
    # 定义一个RotatingFileHandler，最多备份5个日志文件，每个日志文件最大1M
    Rthandler = RotatingFileHandler('main.py.log', maxBytes=1 * 1024 * 1024, backupCount=5)
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

    logging.debug('This is debug message')
    logging.info('This is info message')
    logging.warning('This is warning message')


def serial_task_create():
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


if __name__ == '__main__':
    # serial_task_create()
    log_init()
    ui = GUI()
    # t = threading.Thread(args=(serial_port,), target=self.read_data)  # 创建一个子线程去等待读数据
    # t.setDaemon(True)  # 守护线程
    # t.start()
