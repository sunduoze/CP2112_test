import lupa
from lupa import LuaRuntime
import time

lua = LuaRuntime(unpack_returned_tuples=True)

def Sleep(time_ms):
    print("[P]:Sleep:" + time_ms)
    time.sleep(time_ms)
    return 0

def TcpInit(Ip: str, Port: int):
    print("[P]:" + Ip + ":", Port)
    return 0

def TcpConnect():
    print("[P]:TcpConnect")
    return 0

def TcpDisConnect():
    print("[P]:TcpDisConnect")
    return 0

def TcpReadStringBuff():
    print("[P]:TcpReadStringBuff")
    return 0

def TcpSendString(string: str):
    print("[P]:" + string)
    return 0

def GetPackCode():
    print("[P]:GetPackCode")
    return 0

def GetCellCode():
    print("[P]:GetCellCode")
    return 0

def TimeStart():
    print("[P]:TimeStart")
    return 0

def TimeStop_MS():
    print("[P]:TimeStop_MS")
    return 0

def Getindex():
    print("[P]:Getindex")
    return 0



if __name__ == "__main__":
    fileHandler = open('comm_platform.lua')
    content = fileHandler.read()
    # execute无返回，打印出来是None
    lua.execute(content)
