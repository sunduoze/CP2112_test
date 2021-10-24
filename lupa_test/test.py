import lupa
from lupa import LuaRuntime

lua = LuaRuntime(unpack_returned_tuples=True)


def py_func(p):
    print(p)
    return 'hello ' + str(p)


def pyTcpInit(Ip: str, Port: int):
    print("python func exec:" + Ip + ":", Port)
    return 0


if __name__ == "__main__":
    fileHandler = open('test.lua')
    content = fileHandler.read()

    # 获取lua脚本中的全局变量值
    lua.execute('a,b,c = python.eval("(111,222)")')
    g = lua.globals()
    print(g.a)
    print(g.b)

    wrapped_type = lua.globals().type
    if wrapped_type(1) == 'number':
        print("true")

    if wrapped_type('abc') == 'string':
        print("true")


    lua.execute('ret=1234;print(ret);')

    # # eval返回的是一个lua call到py call的映射
    # pycall = lua.eval('function(pyfunc, param1, param2) '
    #                   'ret = pyfunc(param1, param2) '
    #                   'end')
    # pycall(pyTcpInit, '192.168.1.10', 8888)

    # execute无返回，打印出来是None
    lua.execute(content)

    lua.execute('print(ret)')
