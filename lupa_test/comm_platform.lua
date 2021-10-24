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

local function gettime_slot()
    lua_str = "gettime_slot()"
    p(lua_str)
    --python.eval(lua_str)
	return true
end


local function Setbinding(str)
    lua_str = "Setbinding(\"" .. str .. "\")"
    p(lua_str)
    python.eval(lua_str)
	return true
end

local function set_test_state(status)
    lua_str = "Setbinding(" .. tostring(status) .. ")"
    p(lua_str)
    python.eval(lua_str)
	return true
end

local function LogPush(str)
    lua_str = "LogPush(\"" .. str .. "\")"
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


function SetUI_Auto(TestItemName,ActMeas,LimitMin,LimitMax)
	-- body
	local Time 		= ""
	local ErrInfo 	= ""
	local TestRest  = false
	--num type
	if type(ActMeas) =="number" and type(LimitMin) =="number" and type(LimitMax) == "number" then
		if ActMeas >=LimitMin and ActMeas <= LimitMax then
			TestRest =true
		end
	--string type
	elseif type(ActMeas) == "string" and type(LimitMin) == "string"  and type(LimitMax) == "string" then
		if ActMeas == LimitMin and ActMeas == LimitMax then
			TestRest = true
		end
	end
	local Time = gettime_slot()
	Setbinding(tostring(TestItemName),tostring(TestRest),tostring(ActMeas),tostring(Time),tostring(ErrInfo),tostring(LimitMin),tostring(LimitMax))

	set_test_state(TestRest)

	LogPush(TestItemName .. ","..tostring(ActMeas).. ","..tostring(TestRest)..",")
	return TestRest
end

function SetUI_Error()
	-- body
end

function SetUI(TestItemName,ActMeas,TestRest,Time,ErrInfo,LimitMin,LimitMax)
	if LimitMin == nil  then
		LimitMin = ""
	end
	if LimitMax == nil then
		LimitMax = ""
	end
	Setbinding(tostring(TestItemName),tostring(TestRest),tostring(ActMeas),tostring(Time),tostring(ErrInfo),tostring(LimitMin),tostring(LimitMax))
	_KeepRet = _KeepRet .. tostring(TestRest)
end

--judge value is in table
function  is_intable( value, table1 )
	for k,port in pairs(table1) do
		if port == value then
			return true
		end
	end
	return false
end
--num 2 hex string, default is 2, max is 8
function num2hexstr_s( Num, ...)
	local len 			= ...
	if len == nil then len = 2 end
	if len >= 8   then len = 8 end
	local NumQt 		= Num 		--quotient /
	local NumStr 		= ""
	local i = 1
	repeat
		NumL 	= NumQt % 16		--remainder %
		NumQt 	= NumQt / 16		--quotient /
		sNumL =string.format("%X", NumL)
		NumStr = sNumL .. NumStr
		i = i + 1
	until i > len

	return NumStr
end
--hex string to n bit num
  -- {--eg: hexstr2num_n("123",1,2)convert from 1 to 2
   	 -- hexstr2num_n("123",3) 	convert first 3
   	 -- hexstr2num_n("1234",-3) convert last 3
   	 -- hexstr2num_n("1234")	convert all
  -- }
function hexstr2num_n(str,... )
	local arry 		= {...}
	local len_b 	= arry[1] --par1
	local len_e 	= arry[2] --par2
	local len_str 	= string.len(str)
	--only one para
	if len_e == nil then
		if len_b == nil or len_b == 0 then 			--default:whole string convert
			len_b = len_str
		end
		if math.abs(len_b)>len_str then --para setting abnormal,do not convert
			len_b = 1
		end
		if len_b <-8 then len_b =-8 end --limit length
		if len_b > 8 then len_b = 8 end

		if len_b > 0 then 				--pos para
			len_e = len_b
			len_b = 1
		elseif len_b < 0 then			--neg para
			len_b = len_str +len_b + 1
			len_e = len_str
		end
	--len_b:start address, len_e:stop address
	else
		if len_b < 0 then len_b = len_str + len_b + 1 	end 	--neg to pos
		if len_e < 0 then len_e = len_str + len_e + 1 	end		--neg to pos
		if len_e - len_b > 8 then len_e = len_b + 8 	end 	--para too long
		if len_e - len_b < 0 then len_e = len_b 		end 	--para converted
		if len_b <= 0 then len_b =1						end 	--para too long
		if len_e >= len_str then len_e =len_str			end		--para too long
	end
	--len_b:start address, len_e:stop address
	local num_str = string.upper(str)							--convert to capital
	local num_arr = { ["0"] = 0, ["1"] = 1, ["2"] = 2, ["3"] = 3, ["4"] = 4, ["5"] = 5, ["6"] = 6, ["7"] = 7,
					  ["8"] = 8, ["9"] = 9, ["A"] = 10,["B"] = 11,["C"] = 12,["D"] = 13,["E"] = 14,["F"] = 15 }
	local num = 0
	for i = len_b, len_e do
		local num_b = num_arr[string.sub(num_str,i,i)]
		if  num_b ~= nil then 	--prevent input char out of 0~9 & A~F
			num = num*16 + num_b
		end
	end
	return num
end

--crc8
function crc8_msb_s( str )
	local poly 	= 0x07
	local init 	= 0x00
	local i 	= 1
	local crc 	= init
	local len_check = (#str)%2
	if len_check ~= 0 then
		return ""
	else
		local len 	= (#str) /2   --2.5
		local bittemp = 0
		while(len > 0)
		do
			num = hexstr2num_n(string.sub(str,2*i-1,2*i))
			crc = Xor(num, crc)
			i = i + 1
    	    for j = 0, 7 do
    	        bittemp = BitAnd(crc, 0x80)
    	        if(bittemp ~= 0)
    	        then
    	            crc = Xor(BitLeft(crc, 1), poly)
    	            crc = BitAnd(crc, 0xFF)
    	        else
    	            crc = BitLeft(crc, 1)
    	            crc = BitAnd(crc, 0xFF)
    	        end
    	    end
    	    len = len - 1
		end
		crc = num2hexstr_s(crc, 2)
		return crc
	end
end

--calc checksum
--[input]:string
--[input]:checksum len
function checksum_s( hexstr,...)
	local len = ...
	if(type(len) ~= "number") then
		len = 2
	end
	if type(hexstr) ~= "string" then
		hexstr = ""
	end
	local str_len 	= string.len(hexstr)
	if str_len % 2 ==1 then
		hexstr = ""
		str_len 	= 0
	end
	local sum  		= 0
	local num  		= 0
	local sun_str   = ""

	str_len = str_len / 2

	for i = 1 ,str_len do
		num = hexstr2num_n(string.sub(hexstr,2*i-1,2*i))
		sum = sum + num
	--	sum = sum % 256
	end
	return num2hexstr_s(sum,len)
end

--add checksum to the end of string
function addchecksum_s( hexstr,len )
	-- body
	local sum = checksum_s( hexstr,len )
	return hexstr .. sum
end
--msb to lsb
function msb2lsb_hexstr(hexstr)
	if type(hexstr) ~= "string" then
		return hexstr
	else
		local str_len 	= string.len(hexstr)
		local str_r		= ""
		if (str_len % 2) == 1 then
			str_len = str_len + 1
			hexstr = string.sub(hexstr,1,-2) .. "0"..string.sub(hexstr,-1,-1)
		end
		str_len = str_len /2

		for i = 1 , str_len do
			str_r = str_r .. string.sub(hexstr,-2*i,-2*i+1)
		end
		return str_r
	end
end
--get table data max diff value, Eg. for cell voltage difference
function diff_value_n( table_value )
	local value_max	= nil
	local value_min = nil
	for i ,value in pairs(table_value) do
		if (value_max == nil or value_max < value) then
			value_max = value
		end
		if (value_min == nil or value_min > value) then
			value_min = value
		end
	end
	if value_min ~= nil then
		return value_max - value_min
	else
		return 0
	end
end
