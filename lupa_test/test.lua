-- Time:2016/07/15
 
function test1(params)
    return 'test1:'..params
end
 
function test2(params)
    return 'test2:'..params
end

-- 入口函数,实现反射函数调用
function functionCall(func_name,params)
    local is_true, result
    local sandBox = function(func_name, params)
                        local result
                        result = _G[func_name](params)
                        return result
                    end
    is_true, result = pcall(sandBox, func_name, params)
    return result
end

-- call python function

local function Tcp_init(Ip,Port)
	--TcpInit(Ip,Port)
    lua_str = "pyTcpInit(\"" .. Ip .. "\"," .. tostring(Port) .. ")"
    print(lua_str)
    python.eval(lua_str)
	return true
end

--TcpInit(1,2)
print(a)
print(b)

Tcp_init('192.168.1.10', 8888)

print("local para test:" .. test1("hello world!\r\n"))
