# CP2112_test
cp2112 usb to smbus &amp; gpio tools

test @20210713 ref“DSLogic-la-210713-193902.dsl”
```
"D:\Program Files\Anaconda3\python.exe" C:/Users/Ez/PycharmProjects/CP2112_test/main.py
os: Windows , arch: x64
cp2112 is opened
smbus config: bitRate: 100000 address:0x16 transferRetries: 3
-------------get_status--
7 error info: 0 HID_SMBUS_SUCCESS				
2 HID_SMBUS_S0_COMPLETE
num_retrys 0
bytes_read 0
-------end of get_status----------
-----recv completed-----
0xff
0x3
0x34
0x0
0x0
0x64
0 HID_SMBUS_SUCCESS				
-------------get_status--
7 error info: 0 HID_SMBUS_SUCCESS				
2 HID_SMBUS_S0_COMPLETE
num_retrys 0
bytes_read 3
-------end of get_status----------
lib version: 4 . 1 . False
hid lib version: 7 . 1 . False

Process finished with exit code 0
```