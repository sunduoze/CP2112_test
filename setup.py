# import os
# os.system("..\\CP2112_test\\1.bat ")

import subprocess

filepath = "../CP2112_test/aslan_ble_beacon pack exe.bat"

p = subprocess.Popen(filepath, shell=True, stdout=subprocess.PIPE)
stdout, stderr = p.communicate()

print(p.returncode)  # is 0 if success
