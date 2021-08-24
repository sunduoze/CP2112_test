::pyinstaller -D aslan_ble_beacon.py --add-binary="C:\\Users\\Ez\\PycharmProjects\\CP2112_test\\library\\Windows\\x64\\SLABHIDtoSMBus.dll;library\\Windows\\x64"
pyinstaller -D aslan_ble_beacon.py --add-binary="..\\CP2112_test\\library\\.;library"

pause
# pyinstaller --add-binary '/path/to/some.dll:.' myscript.py