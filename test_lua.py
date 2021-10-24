import traceback
from lupa import LuaRuntime
import threading



def TcpInit(Ip, Port):
    print(Ip, Port)



class Executor(threading.Thread):
    """
        执行lua的线程类
    """
    lock = threading.RLock()
    luaScriptContent = None
    luaRuntime = None

    def __init__(self, api, params):
        threading.Thread.__init__(self)
        self.params = params
        self.api = api

    def run(self):
        try:
            # 执行具体的函数,返回结果打印在屏幕上
            luaRuntime = self.getLuaRuntime()
            rel = luaRuntime(self.api, self.params)
            print(rel)
        except Exception as e:
            print(e.message)
            traceback.extract_stack()

    def getLuaRuntime(self):
        """
            从文件中加载要执行的lua脚本,初始化lua执行环境
        """

        # 上锁,保证多个线程加载不会冲突
        if Executor.lock.acquire():
            if Executor.luaRuntime is None:
                fileHandler = open('./test.lua')
                content = ''
                try:
                    content = fileHandler.read()
                except Exception as e:
                    print(e.message)
                    traceback.extract_stack()

                # 创建lua执行环境
                Executor.luaScriptContent = content
                luaRuntime = LuaRuntime()
                luaRuntime.execute(content)

                # 从lua执行环境中取出全局函数functionCall作为入口函数调用,实现lua的反射调用
                g = luaRuntime.globals()
                function_call = g.functionCall
                Executor.luaRuntime = function_call
            Executor.lock.release()

        return Executor.luaRuntime


if __name__ == "__main__":
    # 在两个线程中分别执行lua中test1,test2函数
    executor1 = Executor('test1', 'hello world')
    executor2 = Executor('test2', 'rudy')
    executor1.start()
    executor2.start()
    executor1.join()
    executor2.join()
