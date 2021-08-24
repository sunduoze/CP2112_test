#!/usr/bin/env python
# --*--coding=utf-8--*--
# pip install pybluez

import time
from bluetooth import *

#列表，用于存放已搜索过的蓝牙名称
alreadyFound = []

#搜索蓝牙
def findDevs():

    foundDevs = discover_devices(lookup_names=True)
    # 循环遍历,如果在列表中存在的就不打印
    for (addr,name) in foundDevs:
        if addr not in alreadyFound:
            print("[*]蓝牙设备:" + str(name))
            print("[+]蓝牙MAC:" + str(addr))
            # 新增的设备mac地址定到列表中,用于循环搜索时过滤已打印的设备
            alreadyFound.append(addr)

print("本机蓝牙MAC地址:", read_local_bdaddr())
# 循环执行,每5秒执行一次
while True:
    findDevs()
    time.sleep(5)
