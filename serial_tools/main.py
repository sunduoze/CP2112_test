# -*- mode: python ; coding: utf-8 -*-
# -*- encoding: gb2312 -*-
import logging
from ctypes import *
import time
from tkinter import ttk
import tkinter.font as tkFont
from tkinter.scrolledtext import ScrolledText

from serial_port import serial_read_data_from_cc2540

from tkinter import *
import tkinter.messagebox  # 弹窗库

LOG_LINE_NUM = 0


class GUI():
    def __init__(self, init_win_name):
        self.init_win_name = init_win_name
        self.ft1 = tkFont.Font(family='宋体', size=10, weight='bold')
        self.ft2 = tkFont.Font(family='宋体', size=10, slant='italic')
        self.ft3 = tkFont.Font(family='宋体', size=10)  # , overstrike=1, underline=1,

        self.start_row = 30
        self.start_column = 0
        self.bt_bg = "DarkGray"

    def init_menu(self):
        # 菜单
        self.menu0 = Menu(self.init_win_name, font=self.ft1, tearoff=0)  # 1的话多了一个虚线，如果点击的话就会发现，这个菜单框可以独立出来显示
        self.menu0.add_command(label="通讯端口")
        self.menu0.add_separator()
        self.menu0.add_command(label="串口设置")
        self.menu0.add_separator()
        self.menu0.add_command(label="显示")
        self.menu0.add_separator()
        self.menu0.add_command(label="发送")
        self.menu0.add_separator()
        self.menu0.add_command(label="多字符串")
        self.menu0.add_separator()
        self.menu0.add_command(label="小工具")
        self.menu0.add_separator()
        self.menu0.add_command(label="帮助")

        self.menu_setting = Menu(self.init_win_name, font=self.ft2, tearoff=0)  # 1的话多了一个虚线，如果点击的话就会发现，这个菜单框可以独立出来显示
        self.menu_setting.add_cascade(label="通讯端口", menu=self.menu0)
        self.menu_setting.add_command(label="第二个")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="串口设置")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="显示")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="发送")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="多字符串")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="小工具")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="帮助")

        self.menu_setting = Menu(self.init_win_name, font=self.ft3, tearoff=0)  # 1的话多了一个虚线，如果点击的话就会发现，这个菜单框可以独立出来显示
        self.menu_setting.add_cascade(label="通讯端口", menu=self.menu0)
        self.menu_setting.add_command(label="第二个")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="串口设置")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="显示")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="发送")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="多字符串")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="小工具")
        self.menu_setting.add_separator()
        self.menu_setting.add_command(label="帮助")

        self.menu_bar = Menu(self.init_win_name)
        self.menu_bar.add_cascade(label="通讯端口", menu=self.menu_setting)
        self.menu_bar.add_cascade(label="串口设置", menu=self.menu_setting)  # 原理：先在主菜单中添加一个菜单，与之前创建的菜单进行绑定。
        self.menu_bar.add_cascade(label="显示", menu=self.menu_setting)
        self.menu_bar.add_cascade(label="发送", menu=self.menu_setting)
        self.menu_bar.add_cascade(label="多字符串", menu=self.menu_setting)
        self.menu_bar.add_cascade(label="小工具", menu=self.menu_setting)
        # self.menu_bar.add_cascade(label="帮助", menu=self.menu_help)
        self.menu_bar.add_command(label="退出", command=self.init_win_name.quit)
        self.init_win_name.config(menu=self.menu_bar)

    # 设置窗口
    def set_init_window(self):

        self.init_win_name.title("蓝牙Beacon测试程序")  # 窗口名
        self.init_win_name.geometry('790x566+10+10')
        # self.init_window_name["bg"] = "pink"                                    #窗口背景色，
        # 其他背景色见：blog.csdn.net/chl0000/article/details/7657887
        # self.init_window_name.attributes("-alpha",0.9)                          #虚化，值越小虚化程度越高

        # 下拉菜单
        # comvalue = tkinter.StringVar()  # 窗体自带的文本，新建一个值
        self.comboxlist = ttk.Combobox(self.init_win_name, font=self.ft1, width=24)
        self.comboxlist["values"] = ("COM1", "COM2")
        self.comboxlist.current(0)  # 选择第一个
        self.comboxlist.bind("<<ComboboxSelected>>", self.scan_beacon())  # 绑定事件,(下拉列表框被选中时，绑定go()函数)
        self.comboxlist.grid(row=self.start_row+1, column=0, columnspan=3)

        self.baud_cbx = ttk.Combobox(self.init_win_name, font=self.ft1, width=8)
        self.baud_cbx["values"] = ("9600", "38400", "57600", "115200", "230400", "500000", "1000000")
        self.baud_cbx.current(3)  # 选择第一个
        self.baud_cbx.bind("<<ComboboxSelected>>", self.scan_beacon())  # 绑定事件,(下拉列表框被选中时，绑定go()函数)
        self.baud_cbx.grid(row=self.start_row+4, column=2, columnspan=3, sticky='w')
        # get()获取选中的值


        # 标签
        self.timer_send_freq_label = Label(self.init_win_name, text="ms/次")
        self.timer_send_freq_label\
            .grid(row=self.start_row+1, column=9, sticky='e')

        self.timeout_label = Label(self.init_win_name, text="ms 超时")
        self.timeout_label.grid(row=self.start_row + 2, column=5, sticky='w')

        self.check4_label = Label(self.init_win_name, text="校验方式:")
        self.check4_label.grid(row=self.start_row + 2, column=6, sticky='w')

        self.send_cnt_label = Label(self.init_win_name, text="发送个数:")
        self.send_cnt_label.grid(row=self.start_row + 5, column=0)
        self.recv_cnt_label = Label(self.init_win_name, text="接收个数:")
        self.recv_cnt_label.grid(row=self.start_row + 6, column=0)
        # 按钮
        # #################### row 0 ######################
        self.clear_get_bt = Button(self.init_win_name, font=self.ft1, text="清除窗口",
                                   bg=self.bt_bg, command=self.enable_beacon)\
            .grid(row=self.start_row+0, column=0, sticky='w')
        self.open_file_bt = Button(self.init_win_name, font=self.ft1, text="打开文件",
                                   bg=self.bt_bg, command=self.enable_beacon).grid(row=self.start_row+0, column=1)
        self.send_file_bt = Button(self.init_win_name, font=self.ft1, text=" 发送文件 ",
                                   bg=self.bt_bg, command=self.enable_beacon)\
            .grid(row=self.start_row+0, column=2, sticky='w')
        self.stop_file_bt = Button(self.init_win_name, font=self.ft1, text="停止",
                                   bg=self.bt_bg, command=self.enable_beacon)\
            .grid(row=self.start_row + 0, column=6)
        self.clear_sent_bt = Button(self.init_win_name, font=self.ft1, text="清发送区",
                                    bg=self.bt_bg, command=self.enable_beacon).grid(row=self.start_row + 0, column=7)
        self.save_para_bt = Button(self.init_win_name, font=self.ft1, text="保存参数",
                                   bg=self.bt_bg, command=self.enable_beacon)\
            .grid(row=self.start_row + 0, column=10, sticky='w')
        self.extend_sent_bt = Button(self.init_win_name, font=self.ft1, text="扩展",
                                     bg=self.bt_bg, command=self.enable_beacon)\
            .grid(row=self.start_row + 0, column=11, sticky='w')
        self.folding_bt = Button(self.init_win_name, font=self.ft1, text="-",
                                 bg=self.bt_bg, command=self.enable_beacon)\
            .grid(row=self.start_row + 0, column=12, sticky='w')

        # #################### row 1 ######################
        self.save_data_bt = Button(self.init_win_name, font=self.ft1, text="保存数据",
                                   bg=self.bt_bg, command=self.enable_beacon)\
            .grid(row=self.start_row + 1, column=4, sticky='w')
        self.help_bt = Button(self.init_win_name, font=self.ft1, text="?",
                              bg=self.bt_bg, command=self.enable_beacon)\
            .grid(row=self.start_row + 1, column=12, sticky='w')
        # #################### row 2 ######################
        self.scan_bt = Button(self.init_win_name, font=self.ft1, text="打开串口",
                              bg=self.bt_bg, command=self.enable_beacon).grid(row=self.start_row+2, column=0)
        self.scan_bt = Button(self.init_win_name, font=self.ft1, text=" 扫描 ",
                              bg=self.bt_bg, command=self.enable_beacon).grid(row=self.start_row+2, column=1)
        self.port_set_bt = Button(self.init_win_name, font=self.ft1, text="更多串口设置", width=10,
                                  bg=self.bt_bg, command=self.enable_beacon)\
            .grid(row=self.start_row+2, column=2, sticky='w')

        # #################### row 5 ######################
        self.send_bt = Button(self.init_win_name, font=self.ft1, text="发送",
                              bg=self.bt_bg, command=self.enable_beacon, height=2, width=9)\
            .grid(row=self.start_row+5, column=2, columnspan=1, rowspan=2)

        # 文本框
        self.send_recv_text = ScrolledText(self.init_win_name, height=30, width=110) \
            .grid(row=0, column=0, columnspan=15, rowspan=30, sticky='w')

        self.open_file_text = Text(self.init_win_name, height=1, width=30)\
            .grid(row=self.start_row+0, column=3, columnspan=3, sticky='w')

        self.timer_send_freq_text = Text(self.init_win_name, height=1, width=5)\
            .grid(row=self.start_row+1, column=8, sticky='w')

        self.time_out_ms_text = Text(self.init_win_name, height=1, width=10)
        self.time_out_ms_text.grid(row=self.start_row+2, column=4, sticky='w')
        self.time_out_ms_text.insert('0.0', '1234')
        # self.timer_send_freq_label = Label(self.time_out_ms_text, text="ms", height=1)
        # self.time_out_ms_text.window_create(INSERT, window=self.timer_send_freq_label)

        self.check_text = Text(self.init_win_name, height=1, width=38)\
            .grid(row=self.start_row+2, column=7, columnspan=6, sticky='w')

        self.send_area_text = Text(self.init_win_name, height=4) \
            .grid(row=self.start_row + 4, column=3, columnspan=10, rowspan=4, sticky='w')

        self.tx_count_text = Text(self.init_win_name, width=9, height=1)
        self.tx_count_text.grid(row=self.start_row + 5, column=1, columnspan=1, sticky='w')
        self.tx_count_text.insert('0.0', '0')

        self.rx_count_text = Text(self.init_win_name, width=9, height=1)
        self.rx_count_text.grid(row=self.start_row + 6, column=1, columnspan=1, sticky='w')
        self.rx_count_text.insert('0.0', '0')

        self.dev_connect_info_text = Text(self.init_win_name, width=50, height=1)
        self.dev_connect_info_text.grid(row=self.start_row + 9, column=0, columnspan=5, sticky='w')
        self.dev_connect_info_text.insert('0.0', 'COM12 已关闭 115200bps,8,1,None,None')
        # self.other_info_text = Text(self.init_win_name, width=30, height=1)
        # self.other_info_text.grid(row=self.start_row + 9, column=8, columnspan=3, sticky='w')
        # self.other_info_text.insert('0.0', '0')

        # 复选框
        # #################### row 0 ######################
        top_val = IntVar()
        self.top_bt = Checkbutton(self.init_win_name, text="顶", variable=top_val,  # anchor='w',
                                  onvalue=1, offvalue=0)\
            .grid(row=self.start_row+0, column=8, sticky='w')
        eng_val = IntVar()
        self.eng_bt = Checkbutton(self.init_win_name, text="英", variable=eng_val,  # anchor='w',
                                  onvalue=1, offvalue=0)\
            .grid(row=self.start_row+0, column=9, sticky='w')
        # #################### row 1 ######################
        hex_disp_val = IntVar()
        self.hex_disp_bt = Checkbutton(self.init_win_name, text="HEX显", variable=hex_disp_val,
                                       onvalue=1, offvalue=0)\
            .grid(row=self.start_row+1, column=3, sticky='w')
        save_data_to_file_val = IntVar()
        self.save_data_bt = Checkbutton(self.init_win_name, text="存文件", variable=save_data_to_file_val,
                                        onvalue=1, offvalue=0)\
            .grid(row=self.start_row+1, column=5, sticky='w')
        hex_send_val = IntVar()
        self.hex_send_bt = Checkbutton(self.init_win_name, text="HEX发", variable=hex_send_val,
                                       onvalue=1, offvalue=0)\
            .grid(row=self.start_row + 1, column=6, sticky='w')
        timer_send_val = IntVar()
        self.timer_send_bt = Checkbutton(self.init_win_name, text="定时发送", variable=timer_send_val,
                                         onvalue=1, offvalue=0)\
            .grid(row=self.start_row + 1, column=7, sticky='w', columnspan=2)
        add_enter_wrap_val = IntVar()
        self.add_enter_wrap_bt = Checkbutton(self.init_win_name, text="加回车换行", variable=add_enter_wrap_val,
                                             onvalue=1, offvalue=0)\
            .grid(row=self.start_row+1, column=10, columnspan=3, sticky='w')

        # #################### row 2 ######################
        add_time_stamp_diff_pack_disp_val = IntVar()
        self.add_time_stamp_diff_pack_disp_bt = Checkbutton(self.init_win_name, text="时间戳",
                                                            variable=add_time_stamp_diff_pack_disp_val,
                                                            onvalue=1, offvalue=0)\
            .grid(row=self.start_row+2, column=3, sticky='w')

        # #################### row 4 ######################
        rts_val = IntVar()
        self.rts_bt = Checkbutton(self.init_win_name, text="RTS", variable=rts_val, anchor='w',
                                  onvalue=1, offvalue=0)\
            .grid(row=self.start_row+4, column=0, sticky='w')
        dtr_val = IntVar()
        self.dtr_bt = Checkbutton(self.init_win_name, text="DTR", variable=dtr_val, anchor='w',
                                  onvalue=1, offvalue=0)\
            .grid(row=self.start_row+4, column=1, sticky='w')
    # 功能函数
    def enable_beacon(self):
        print("test fail!")

    def disable_beacon(self):
        print("[disable]retry times:")

    def scan_beacon(self):
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
    ui.init_menu()
    init_window.mainloop()  # 父窗口进入事件循环，可以理解为保持窗口运行，否则界面不展示


def log_init():
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
                        datefmt='%a, %d %b %Y %H:%M:%S',
                        filename='myapp.log',
                        filemode='w')

    from logging.handlers import RotatingFileHandler

    #################################################################################################
    # 定义一个RotatingFileHandler，最多备份5个日志文件，每个日志文件最大1M
    Rthandler = RotatingFileHandler('app.log', maxBytes=1 * 1024 * 1024, backupCount=5)
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


if __name__ == '__main__':
    log_init()
    gui_start()
