-- Time:2016/07/15
 
function test1(params)
    return 'test1:'..params
end


local function p(...)
    print(...)
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

local function Sleep(time_ms)
    lua_str = "Sleep(" .. tostring(time_ms) .. ")"
    p(lua_str)
    python.eval(lua_str)
	return true
end


local function TcpInit(Ip,Port)
    lua_str = "TcpInit(\"" .. Ip .. "\"," .. tostring(Port) .. ")"
    p(lua_str)
    python.eval(lua_str)
	return true
end

local function TcpConnect()
    lua_str = "TcpConnect()"
    p(lua_str)
    python.eval(lua_str)
	return true
end

local function TcpDisConnect()
    lua_str = "TcpDisConnect()"
    p(lua_str)
    python.eval(lua_str)
	return true
end

local function TcpReadStringBuff()
    lua_str = "TcpReadStringBuff()"
    p(lua_str)
    python.eval(lua_str)
	return true
end

local function TcpSendString(str)
    lua_str = "TcpInit(\"" .. str .. "\")"
    p(lua_str)
    python.eval(lua_str)
	return true
end

local function GetPackCode()
    lua_str = "GetPackCode()"
    p(lua_str)
    python.eval(lua_str)
	return "GetPackCode"
end

local function GetCellCode()
    lua_str = "GetCellCode()"
    p(lua_str)
    python.eval(lua_str)
	return "GetCellCode"
end

local function TimeStart()
    lua_str = "TimeStart()"
    p(lua_str)
    python.eval(lua_str)
	return 12
end

local function TimeStop_MS()
    lua_str = "TimeStop_MS()"
    p(lua_str)
    python.eval(lua_str)
	return true
end

local function Getindex()
    lua_str = "Getindex()"
    p(lua_str)
    python.eval(lua_str)
	return true
end


TcpInit('192.168.1.10', 8888)
--
--print("local para test:" .. test1("hello world!\r\n"))

------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------

--	link ethernet
--	link ethernet
--#region link ethernet, use this function need init at first
------------------------------------------------------------------------------------------------
--  local gIp,gPort  = "192.168.1.10",8888
-- 	TcpDisConnect()
--	TcpInit(gIp,gPort)
--	TcpConnect()
local gIp,gPort               = "192.168.1.10",8888
local function Tcp_init(Ip,Port)
	TcpInit(Ip,Port)
	return true
end

local function Tcp_Connect()
	local str = TcpConnect()
	if type(str)~= "string" or (type(str) == "string" and str ~= "OK") then
		return false
	else
		return true
	end
end

local function Tcp_DisConnect()
	TcpDisConnect()
	return true
end

--ethernet communication
local function Tcp_cmd(str,timedelay)
	TcpReadStringBuff()
	TcpSendString(str .. "\r\n")
	Sleep(timedelay)
	return  TcpReadStringBuff()
end
--#endregion


local MCB_addr =18       --main control board address





DUT_COM_Table 	= {["DUT1"]  = 0 }	--default DUT serial port, not fill nil
PSU_COM_Table 	= {["PSU1"]  = 0 }	--default PS serial port, not fill nil
LOAD_COM_Table  = {["LOAD1"] = 6 }	--default E_LOAD serial port, not fill nil

--instrument protocol :SCPI, FRAME
PSU_PROT_Table  = {["PSU1"]  = "SCPI"  }
LOAD_PROT_Table = {["LOAD1"] = "FRAME" }

_COMMAND_WAITTIME_LOAD 		= 100 --ms serial receive delay [ms]
_COMMAND_WAITTIME_PSU 		= 100
_COMMAND_WAITTIME_DUT 		= 50


local PSU_value 			= {["CURR"] = 0, ["VOLT"] = 0}		--PS set output value
local Prot_value 			= {["CURR"] = 0, ["VOLT"] = 0}		--PS set protect value

local SerialPortCount 		= 8									--serial count



---------------------------------------------------------
---------------------------------------------------------


_KeepRet 			= true
_FailToContinue 	= true    -- true: NG & do until carry out completed，False：NG & stop test.
DEV_USBCAN  		= "DEV_USBCAN"
DEV_USBCAN2 		= "DEV_USBCAN2"
Getindex()
gPacKBatterCode     = GetPackCode()		      --global battery barcode
gCellCode 		    = GetCellCode()			  --global cell barcode / global temporary barcode

--global, match with function gettime_slot() to use
_gbTimeBegin = TimeStart()
_gbTimeSun 	 = 0
_gbTimeSlot  = 0
--return execution time between two call, default at first start, need add when you want to call
--unit S
--each call will be execution SetUI_Auto()
function gettime_slot()
	_gbTimeSlot = TimeStop_MS()
	_gbTimeSun 	= _gbTimeSun + _gbTimeSlot
	TimeStart()									-- re-calc data sections
	_gbTimeSlot = tonumber(string.format("%6.1f",_gbTimeSlot))
	return _gbTimeSlot
end
function get_test_state()
	return (_KeepRet or _FailToContinue)
end
function set_test_state( newstate )
	_KeepRet = _KeepRet and newstate
end


