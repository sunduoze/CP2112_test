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

function serial_init(PortName, ...)
	local serial_arg = {...}
	local Baud_rate  = serial_arg[1]
	local checkbit	= serial_arg[2]
	local databit 	= serial_arg[3]
	local stopbit 	= serial_arg[4]
	SetSerialPortCount(SerialPortCount)
	if Baud_rate == nil then
		SerialPortInit(PortName,9600,0,8,1)
	else
		--add limit
		--SerialPortInit(PortName, Baud_rate, checkbit, databit, stopbit)
	end
	PortSerialOpen(PortName)
	PortClearBuff(PortName)
end
--send receive hex string format string
function serial_cmd_hexstr( PortName, SendStr, waittime)	--serial num, send string, receive timeout
    local rx_str 		= ""
	local Rx_Table = nil
    PortClearBuff(PortName)
    PortSendStrArray2Byte(SendStr, PortName)
    Sleep(waittime)
	Rx_Table = PortGetByte2StrArray(PortName)
	local rx_len = 0
	if Rx_Table ~= nil then
		rx_len = ArrayLens(Rx_Table)
		for i=0, rx_len-1 do
			rx_str = rx_str .. Rx_Table[i]
		end
	end
	return rx_str
end
--send receive string
function serial_cmd_str( PortName, SendStr, waittime)	--serial num, send string, receive timeout
    PortClearBuff(PortName)
    PortSendStr(SendStr, PortName)
	Sleep(waittime)
	local rx_str = PortGetStr(PortName)
	return rx_str
end


-----------------------PSU it6863a-----------------------
--#region--------------PSU it6863a-----------------------
local SCPI_END 								= "\r\n"
local SCPI_SYST_MESSAGE						= "*IDN?" 				..SCPI_END	--
local SCPI_SYST_REM							= "SYST:REM" 			..SCPI_END	--remote
local SCPI_SYST_LOC							= "SYST:LOC" 			..SCPI_END	--manual
local SCPI_CLS								= "*CLS"				..SCPI_END	--clear
local SCPI_RST								= "*RST"				..SCPI_END	--reset
---------------------------	PSU	--------------------------
local SCPI_OUT_ON							= "OUTP ON" 			..SCPI_END	--enable
local SCPI_OUT_OFF							= "OUTP OFF" 			..SCPI_END	--disable
local SCPI_MEAS_VOLT						= "MEAS:VOLT?"			..SCPI_END	--
local SCPI_MEAS_CURR						= "MEAS:CURR?"			..SCPI_END	--
local SCPI_VOLT_RANG_LOW 					= "VOLT:RANG low"		..SCPI_END	--set output range:low
local SCPI_PROT_VOLT						= "VOLT:PORT?"			..SCPI_END	--ovp volt
local SCPI_VOLT_RANG_HIGH 					= "VOLT:RANG high"		..SCPI_END	--set output range:high

--order_numb
local SCPI_CURR	 							= "CURR "
local SCPI_VOLT								= "VOLT "
local SCPI_PRO_CURR							= "VOLT:PROT "
local LOAD_CURR								= "SOUR:CURR "
--------------------------------------------------------------
local LOAD_DISABLE  						= "SOUR:INP OFF"		..SCPI_END
local LOAD_ENABLE  							= "SOUR:INP ON" 		..SCPI_END
--------------------------------------------------------------
function psu_load_scan(...)
	local waittime = ...
	if waittime == nil then waittime = 100 end
	SerialPortName = {}
	SerialPortName = SerialPort_Sacn()
	Portnumb =ArrayLens(SerialPortName)
	SetSerialPortCount(Portnumb)

	local PortName = nil
	local i =1
	for i=0,Portnumb-1 do
			PortName=string.gsub(SerialPortName[i],"COM","") 	-- must use Portname_R convert PortName
	    	PortName = tonumber(PortName)
			if is_intable(PortName,PSU_COM_Table) ==false and
					is_intable(PortName,LOAD_COM_Table) ==false and
					is_intable(PortName,DUT_COM_Table) ==false then
				local find_b = nil
				local scpi_bit = false

				SerialPortInit(PortName,9600,0,8,1)
				PortSerialOpen(PortName)
				PortClearBuff(PortName)
				PortSendStr("*CLS\r\n",PortName)
				Sleep(waittime)
				PortClearBuff(PortName)
				PortSendStr("*IDN?\r\n",PortName)
				Sleep(waittime)
				local read  = PortGetStr(PortName)
				if type(read)=="string" then
					scpi_bit = true
					-----------------------------------------------
					-----------------------------------------------
					--ShowMsg(tostring(PortName)..tostring(read))

				  	find_b=string.match(read,"IT6")				--IT6xxx PSU
				  	if(find_b ~= nil) then
				  		PSU_COM_Table["PSU1"] = PortName
				  		PSU_PROT_Table["PSU1"] = "SCPI"
				  	end
				-----------------------------------------------
				-----------------------------------------------
				  	find_b=string.match(read,"IT8")				--IT8xxx E-LOAD
				  	if(find_b ~= nil) then
				  		LOAD_COM_Table ["LOAD1"] = PortName
				  		LOAD_PROT_Table["LOAD1"] = "SCPI"
				  	end
				-----------------------------------------------
				end

				if scpi_bit == false then 							--
				-- ShowMsg("LOAD  "..tostring(PortName))
				    --it32102_cmd(PortName,"AA006A",100)
					local read_str = ""
					--it32102_cmd(PortName,"AA006A",300)
					PortClearBuff(PortName)
					read_str = read_str .. it32102_cmd(PortName,"AA006A",waittime)
					local is_load = nil
					if type(read_str) ~=nil then
						is_load=string.match(read_str,"3332313032")				--IT6xxx PSU
						if is_load ~= "" and is_load ~=nil then
					  		LOAD_COM_Table ["LOAD1"] = PortName
					  		LOAD_PROT_Table["LOAD1"] = "FRAME"
						end
					end
				end

				PortSerialClose(PortName)
				Sleep(10)
			end 	-- if is_intable(i,PSU_COM_Table) =false and ....
	end -- for i=1,Portnumb do
	--PSU_COM_Table["PSU1"] =Portname_R
end

function scpi_command( PortName, order, waittime )	--serial num, send string, receive timeout
	-- body
  local rx_str 	= nil
	rx_str = serial_cmd_str(PortName,order,waittime)
	return rx_str
end
function scpi_command_num( PortName, order_numb, number, waittime )
    local rx_str 		= nil
    local num = number/1000
    local num_str 		= tostring(num)
    num_str			= order_numb .. num_str .."\r\n"
    rx_str = serial_cmd_str(PortName,num_str,waittime)
	return rx_str
end
function it6863a_init(...)
	local PortName_R = 0
	local waittime 	 = 0
	local value_table = {...}

	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= PSU_COM_Table["PSU1"]
		waittime 	= _COMMAND_WAITTIME_PSU
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_PSU
	else
		PortName_R  = value1
		waittime 	= value2
	end
	serial_init(PortName_R)
	scpi_command(PortName_R,SCPI_SYST_REM,waittime)
	scpi_command(PortName_R,SCPI_CLS,waittime)
	--scpi_command(PortName_R,SCPI_RST,waittime)
	it6863a_set_curr(0,PortName_R,waittime)
	it6863a_set_volt(0,PortName_R,waittime)
end

function it6863a_read_curr_n( ... )
	local PortName_R = 0
	local waittime 	 = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= PSU_COM_Table["PSU1"]
		waittime 	= _COMMAND_WAITTIME_PSU
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_PSU
	else
		PortName_R  = value1
		waittime 	= value2
	end

	local num_str 	= scpi_command(PortName_R,SCPI_MEAS_CURR, waittime)
	-- ShowMsg(num_str)
	local num 		= tonumber(num_str) *1000
	return num
end
function it6863a_read_volt_n( ... )
	local PortName_R = 0
	local waittime 	 = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= PSU_COM_Table["PSU1"]
		waittime 	= _COMMAND_WAITTIME_PSU
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_PSU
	else
		PortName_R  = value1
		waittime 	= value2
	end

	local num_str 	= scpi_command(PortName_R,SCPI_MEAS_VOLT, waittime)
	local num 		= tonumber(num_str) *1000
	return num
end
function it6863a_set_curr( curr,... )	-- (current) / (PortName ,current)
	-- body
	local set_curr		= curr
	local PortName_R    = nil
	local waittime	    = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= PSU_COM_Table["PSU1"]
		waittime 	= _COMMAND_WAITTIME_PSU
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_PSU
	else
		PortName_R  = value1
		waittime 	= value2
	end
	--switch range
	---------------------------------------------------------
	if(set_curr >= 1500 ) then
		if(PSU_value["VOLT"] >=32000) then
			set_curr = 1500
		else
			--switch range
			it6863a_volt_rang_low()
		end
	end
	PSU_value["CURR"] = set_curr
	scpi_command_num(PortName_R, SCPI_CURR, set_curr, waittime)
end
function it6863a_set_volt( volt,... )	--(PortName, waittime)
	-- body
	local set_volt		= volt

	local PortName_R    = nil
	local waittime	    = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= PSU_COM_Table["PSU1"]
		waittime 	= _COMMAND_WAITTIME_PSU
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_PSU
	else
		PortName_R  = value1
		waittime 	= value2
	end
	---------------------------------------------------------
	if(set_volt >= 32000 ) then
		if(PSU_value["CURR"] >1500) then
			set_volt = 32000
		else
			--switch range
			it6863a_volt_rang_high()
		end
	end
	---------------------------------------------------------
	PSU_value["VOLT"] = set_volt
	scpi_command_num( PortName_R, SCPI_VOLT, set_volt, waittime)
end
function it6863a_set_volt_curr(volt, curr, ... ) -- (PortName, timeout)
	-- body
	local PortName_R    = nil
	local waittime	    = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= PSU_COM_Table["PSU1"]
		waittime 	= _COMMAND_WAITTIME_PSU
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_PSU
	else
		PortName_R  = value1
		waittime 	= value2
	end
	it6863a_set_volt(volt,PortName_R,waittime)
	it6863a_set_curr(curr,PortName_R,waittime)
end
function it6863a_volt_rang_high( ... )
	-- body
	local PortName_R    = nil
	local waittime	    = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= PSU_COM_Table["PSU1"]
		waittime 	= _COMMAND_WAITTIME_PSU
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_PSU
	else
		PortName_R  = value1
		waittime 	= value2
	end
	scpi_command(PortName_R, SCPI_VOLT_RANG_HIGH,waittime)
end
function it6863a_volt_rang_low( ... )
	-- body
	local PortName_R    = nil
	local waittime	    = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= PSU_COM_Table["PSU1"]
		waittime 	= _COMMAND_WAITTIME_PSU
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_PSU
	else
		PortName_R  = value1
		waittime 	= value2
	end
	scpi_command(PortName_R, SCPI_VOLT_RANG_LOW,waittime)
end
function it6863a_enable( ... ) -- PortName
	local PortName_R    = nil
	local waittime	    = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= PSU_COM_Table["PSU1"]
		waittime 	= _COMMAND_WAITTIME_PSU
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_PSU
	else
		PortName_R  = value1
		waittime 	= value2
	end
	scpi_command(PortName_R, SCPI_OUT_ON,waittime)
end
function it6863a_disable( ... )	-- PortName
	local PortName_R    = nil
	local waittime	    = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= PSU_COM_Table["PSU1"]
		waittime 	= _COMMAND_WAITTIME_PSU
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_PSU
	else
		PortName_R  = value1
		waittime 	= value2
	end
	scpi_command(PortName_R, SCPI_OUT_OFF,waittime)
end

--#endregion

-----------------------IT8500 / it6863a E_LAOD------------------
-----------------------IT8500 / IT32102 E_LAOD------------------
--#region----------------------------------------------
local LOAD_
local LOAD_SYNC 					= "AA"
local LOAD_ADDR						= "00"
local LOAD_REM						= "2001"
local LOAD_LOC						= "2000"
local LOAD_ON						= "2101"
local LOAD_OFF						= "2100"

local LOAD_LOCAL_ENABLE				= "5501"
local LOAD_LOCAL_DISABLE			= "5500"

local LOAD_SET_MAXVOLT				= "22"
local LOAD_READ_MAXVOLT				= "23"
local LOAD_SET_MAXCURR				= "24"
local LOAD_READ_MAXCURR				= "25"
---
---
local LOAD_SET_MODE	  				= "28"
local LOAD_READ_MODE				= "29"
local LOAD_set_MODE_CURR 			= "2800"
local LOAD_set_MODE_VOLT 			= "2801"
local LOAD_set_MODE_RET 			= "2802"
local LOAD_SET_CURR 				= "2A"
local LOAD_READ_CURR 				= "2B"
local LOAD_SET_VOLT 				= "2C"
local LOAD_READ_VOLT 				= "2D"

local LOAD_INPUT_STATE				= "5F"
local LOAD_INFO_READ				= "6A"

LOAD_ADDR = "00"
-- cmd comp to 25 bytes
function it32102_command_package(Hexstr)
	-- body
	local len = string.len(Hexstr)
	if len <50 then
		repeat
			Hexstr = Hexstr.."0"
			len = len + 1
		until(len==50)
	end
	return Hexstr
end

function it32102_cmd(PortName,order,waittime)
	order = it32102_command_package(order) 		--	command package
	order = addchecksum_s(order)				--	add checksum
	return  serial_cmd_hexstr(PortName,order,waittime)

end

function it32102_init(...)
	local PortName_R = 0
	local waittime = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= LOAD_COM_Table ["LOAD1"]
		waittime 	= _COMMAND_WAITTIME_LOAD
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_LOAD
	else
		PortName_R  = value1
		waittime 	= value2
	end
	serial_init(PortName_R)

	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_REM,waittime)
	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_OFF,waittime) 		 --	LOAD_LOCAL_ENABLE
	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_LOCAL_ENABLE,waittime) --enable panel ctrl
end
function it32102_close( ... )
	local PortName_R = 0
	local waittime = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= LOAD_COM_Table ["LOAD1"]
		waittime 	= _COMMAND_WAITTIME_LOAD
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_LOAD
	else
		PortName_R  = value1
		waittime 	= value2
	end
	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_OFF,waittime) 			--	LOAD_LOCAL_ENABLE
	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_LOCAL_ENABLE,waittime)    --- enable panel ctrl
	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_LOC,waittime)

	PortSerialClose(PortName_R)
end


function it32102_set_curr(curr,...)
	local PortName_R = 0
	local waittime = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= LOAD_COM_Table ["LOAD1"]
		waittime 	= _COMMAND_WAITTIME_LOAD
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_LOAD
	else
		PortName_R  = value1
		waittime 	= value2
	end
	local curr_set = math.floor(curr*10.0)			--	min accuracy 0.1A
	curr_set = num2hexstr_s(curr_set,4)					-- 	convert 8*2 length hex string
	curr_set = msb2lsb_hexstr(curr_set)					--	LSB
	curr_set = it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_SET_CURR..curr_set,waittime)
end
-- value = PortName or value1 = PortName, value2 = waittime
function it32102_input_read( ... )
	local PortName_R = 0
	local waittime = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= LOAD_COM_Table ["LOAD1"]
		waittime 	= _COMMAND_WAITTIME_LOAD
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_LOAD
	else
		PortName_R  = value1
		waittime 	= value2
	end

	local volt_read = nil
	local curr_read = nil
	local power_read = nil
	local cmd_str 	= ""
	cmd_str 	 	= cmd_str .. LOAD_SYNC..LOAD_ADDR..LOAD_INPUT_STATE
	local rx_str = it32102_cmd(PortName_R,cmd_str,waittime)
	--check first len data with send cmd, for data check
	local len = string.len(cmd_str)					--send cmd length
	local rx_check = string.sub(rx_str,1,len)	--intercept len receive cmd
	if rx_check ~= cmd_str then                     --comp
		-- SetUI(" ","IT32102 Read State Error,try to send command slower",tostring(error))
		volt_read = nil
		curr_read = nil
		power_read = nil
	else
		rx_str 		= string.sub(rx_str,len+1)
		volt_read 	= string.sub(rx_str,1,8)
		curr_read 	= string.sub(rx_str,9,16)
		power_read 	= string.sub(rx_str,17,24)

		volt_read 	= msb2lsb_hexstr(volt_read)
		curr_read	= msb2lsb_hexstr(curr_read)
		power_read	= msb2lsb_hexstr(power_read)

		volt_read 	= hexstr2num_n(volt_read)
		curr_read	= hexstr2num_n(curr_read)
		power_read	= hexstr2num_n(power_read)

		curr_read 	= curr_read * 0.1
	end
	return volt_read,curr_read,power_read
end


function it32102_read_volt_curr_2n( ... )
	local volt_read = 0
	local curr_read = 0
	volt_read,curr_read = it32102_input_read(...)
	return volt_read,curr_read
end
function it32102_enable(...)
	local PortName_R = 0
	local waittime = 0

	local value_table = {...}
	value1 = value_table[1]
	value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= LOAD_COM_Table ["LOAD1"]
		waittime 	= _COMMAND_WAITTIME_LOAD
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_LOAD
	else
		PortName_R  = value1
		waittime 	= value2
	end


	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_ON,waittime)
end
function it32102_disable(...)
	local PortName_R = 0
	local waittime = 0
	local value_table = {...}
	local value1 = value_table[1]
	local value2 = value_table[2]
	if value1 == nil then
		PortName_R 	= LOAD_COM_Table ["LOAD1"]
		waittime 	= _COMMAND_WAITTIME_LOAD
	elseif value2 == nil then
		PortName_R  = value1
		waittime 	= _COMMAND_WAITTIME_LOAD
	else
		PortName_R  = value1
		waittime 	= value2
	end

	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_OFF,waittime)
end
--#endregion
----------------------- IT8500 / IT32102 E_LOAD ------------------
----------------------- IT8500 / IT32102 E_LOAD ------------------

------------------------------------------------------------------
--#region --------------ITM3432/bi-dir PUS------------------------
local LEO_ITM3432_COMMAND_TIMEDELAY = 50 --default receive cmd delay
local LEO_ITM3432_OUTPUT_MODE = 999 	--flag 1:source 2:sink
local LEO_ITM3432_OUTPUT_CHG_FLAG = 1	--chg flag
local LEO_ITM3432_OUTPUT_DSG_FLAG = 2	--dsg flag

--leopard comm cmd，send str cmd & delay timeout to receive
-- : par1 send str
-- : par2 timeout
-- : receive data
local  function leo_comunication(str,...)
	local timedelay = ...
	if type(timedelay) ~="number" then
		timedelay = LEO_ITM3432_COMMAND_TIMEDELAY
	end
	local rec_str = Tcp_cmd( str,timedelay)
	return rec_str
end

--parsing leopard cmd, boolean,str,str,str
local function lep_rec_cmd_decode(str)
	if(type(str) ~= "string") then
		return false,"","",""
	end
	local a,b,c,d = string.match(str,"(%w*),(%w*),(%w*),(%w*)")
	if a == "pass" then
		return true,b,c,d
	else
		return false,b,c,d
	end
end

-- par:chg volt, chg curr, delay return: boolean
local function leo_m3432_chg_set(volt,curr,...)
	if(type(volt) ~= "number") or (type(curr) ~= "number") then
		return false
	end
	local timedelay = ...
	if(timedelay ~= "number") then
		timedelay = LEO_ITM3432_COMMAND_TIMEDELAY
	end

	local send_volt = string.format( "%.5f",volt )
	local send_curr = string.format( "%.5f",curr)
	local mode = nil
	if LEO_ITM3432_OUTPUT_MODE == LEO_ITM3432_OUTPUT_CHG_FLAG then
		mode = 2 -- continues charge
	else
		mode = 0 -- restart charge
	end
	local time_str = tostring(timedelay)
	local send_str = "psu_set_vi("..send_volt ..",".. send_curr..",".. mode ..",".. time_str .. ",36.0,3.6,20.0,0.05)"
	local rec_str = leo_comunication(send_str,timedelay)
	local state = lep_rec_cmd_decode(rec_str)
	if(state == "pass") then
		--charge status
		LEO_ITM3432_OUTPUT_MODE = LEO_ITM3432_OUTPUT_CHG_FLAG
		return true
	else
		--fail TBF
		-- LEO_ITM3432_OUTPUT_MODE = 999
		return false
	end
end

-- par:dsg volt, dsg curr, delay return: boolean
local function leo_m3432_dsg_set(volt,curr,...)
	if(type(volt) ~= "number") or (type(curr) ~= "number") then
		return false
	end
	local timedelay = ...
	if(timedelay ~= "number") then
		timedelay = LEO_ITM3432_COMMAND_TIMEDELAY
	end
	local send_volt = string.format( "%.5f",volt )
	local send_curr = string.format( "%.5f",curr)
	local mode = nil
	if LEO_ITM3432_OUTPUT_MODE == LEO_ITM3432_OUTPUT_DSG_FLAG then
		mode = 3 -- continues discharge
	else
		mode = 1 -- restart discharge
	end
	local time_str = tostring(timedelay)
	local send_str = "psu_set_vi("..send_volt ..",".. send_curr..",".. mode ..",".. time_str .. ",36.0,3.6,20.0,0.05)"
	-- ShowMsg(send_str)
	local rec_str = leo_comunication(send_str,timedelay)
	local state = lep_rec_cmd_decode(rec_str)
	if(state == "pass") then
		--mark chg status
		LEO_ITM3432_OUTPUT_MODE = LEO_ITM3432_OUTPUT_DSG_FLAG
		return true
	else
		--TBD
		-- LEO_ITM3432_OUTPUT_MODE = 999
		return false
	end
end

local function leo_m3432_enable(...)
	local timedelay = ...
	if(timedelay ~= "number") then
		timedelay = LEO_ITM3432_COMMAND_TIMEDELAY
	end
	local send_str = "psu_out(1)"
	local rec_str = leo_comunication(send_str,timedelay)
	---
	local state = lep_rec_cmd_decode(rec_str)
	if(state == "pass") then
		return true
	else
		return false
	end
end

local function leo_m3432_disable(...)
	local timedelay = ...
	if(timedelay ~= "number") then
		timedelay = LEO_ITM3432_COMMAND_TIMEDELAY
	end
	local send_str = "psu_out(0)"
	local rec_str = leo_comunication(send_str,timedelay)
	---
	local state = lep_rec_cmd_decode(rec_str)
	if(state == "pass") then
		return true
	else
		return false
	end
end

local function leo_m3432_read_volt_curr(...)
	local timedelay = ...
	if(timedelay ~= "number") then
		timedelay = LEO_ITM3432_COMMAND_TIMEDELAY
	end

	local send_str = "psu_get_vi(0)"

	local rec_str = leo_comunication(send_str,timedelay)
	---rec_str,volt_str,curr_str
	local state = true

	local str_state,volt,curr = lep_rec_cmd_decode(rec_str)
	if(str_state ~= "pass") then
		return false,0,0
	end
	return true,tonumber(volt),tonumber(curr)
end
--#endregion
----------------------- ITM3432 / bi-dir PSU   ------------------
----------------------- ITM3432 / bi-dir PSU   ------------------




-------------------------PSU ctrl function-----------------------------------------------
--
--#region
function psu_init()
	-- body
	if get_test_state() then
		if PSU_COM_Table["PSU1"] ~=0 and PSU_COM_Table["PSU1"] ~= nil then
			if PSU_PROT_Table["PSU1"] == "SCPI" then
				it6863a_init()
			elseif PSU_PROT_Table["PSU1"] == "FRAME" then
			end
		else
			ShowMsg("PSU comm error, pls press enter key")
			_KeepRet = false
		end
	end
end

function psu_read_curr_n()
	if PSU_PROT_Table["PSU1"] == "SCPI" then
		return it6863a_read_curr_n()

	elseif PSU_PROT_Table["PSU1"] == "FRAME" then

	end
end

function psu_read_volt_n()
	if PSU_PROT_Table["PSU1"] == "SCPI" then
		return it6863a_read_volt_n()

	elseif PSU_PROT_Table["PSU1"] == "FRAME" then
	end
end

function psu_set_volt_curr(volt,curr)
	if PSU_PROT_Table["PSU1"] == "SCPI" then
		it6863a_set_volt_curr(volt,curr)

	elseif PSU_PROT_Table["PSU1"] == "FRAME" then
	end
end

function psu_output_on()
	if PSU_PROT_Table["PSU1"] == "SCPI" then
		it6863a_enable()

	elseif PSU_PROT_Table["PSU1"] == "FRAME" then
	end
end

function psu_output_off()
	-- body
	if PSU_PROT_Table["PSU1"] == "SCPI" then
		it6863a_disable()

	elseif PSU_PROT_Table["PSU1"] == "FRAME" then
	end
end
--#endregion

-------------------------E_LOAD ctrl function-----------------------------------------------
--
--#region
function load_init()
	if get_test_state() then
		if LOAD_COM_Table["LOAD1"] ~=0 then
			if LOAD_PROT_Table["LOAD1"] == "SCPI" then
			elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then
				it32102_init()
			end
		else
			ShowMsg("E_LOAD comm error, pls press enter key")
			_KeepRet = false
		end
	end
end
function load_close( ... )
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then
		it32102_close()
	end
end
function load_set_curr( curr )
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then
		it32102_set_curr(curr)
	end
end
function load_read_volt_curr_2n()
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then
		local volt = 0
		local curr = 0
		volt, curr = it32102_read_volt_curr_2n()
		return volt, curr
	end
end
function load_read_curr_n()
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then
		local volt = 0
		local curr = 0
		volt, curr = it32102_read_volt_curr_2n()
		return curr
	end
end

function load_read_volt_n()
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then
		local volt = 0
		local curr = 0
		volt, curr = it32102_read_volt_curr_2n()
		return volt
	end
end

function load_output_on()
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then
		it32102_enable()
	end
end

function load_output_off()
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then
		it32102_disable()
	end
end
--#endregion

-------------------------BCS battery ctrl function-----------------------------------------
--
--#region
function battray_init()

end

function battray_cell_voltage(volt)
end

function battray_charge_set(volt,curr)
	return leo_m3432_chg_set(volt,curr,2000)
end

function battray_discharg_set(volt,curr)
	leo_m3432_dsg_set(volt,curr,2000)
end

function battray_read_v_i()
	local state,volt,curr =  leo_m3432_read_volt_curr(1000);
	return state,volt,curr
end

function battray_on()
	local  state = leo_m3432_enable(500)
	return state
end
function battray_off()
	local  state = leo_m3432_disable(500)
	return state
end
--#endregion




----------------------------Leo_ExtIO Mode-------------------------
--#region ------------------Leo_ExtIO Mode-------------------------

local gCat9555_CFG = {				--  16bits is CAT9555 io status 0:output 1: intput
	["BCS"]			= 0x0000,		--
	["PSU"] 		= 0x0000,		--
	["LOAD"] 		= 0x0000,		--
	["IO_MUX"]		= 0x00FF,		--	low 8 bits is intput
	["DMM"]			= 0x0000,		--
}
--match with hardware
local gCat9555_Board = {			-- 	A02 A01 A00 Jumper
	["BCS"] 		= 0x00,			--	L L L
	["PSU"] 		= 0x01,			--	L L H
	["LOAD"] 		= 0x02,			--	L H L
	["IO_MUX"]		= 0x04,			--	H L L
	["DMM"]			= 0x05,			--	H H L
}
local gCH = {
	["CH1"] 		= 1,
	["CH2"] 		= 2,
	["CH3"] 		= 3,
	["CH4"] 		= 4,
	["CH5"] 		= 5,
	["CH6"] 		= 6,
	["CH7"] 		= 7,
	["CH8"] 		= 8,
	["CH9"] 		= 9,
	["CH10"] 		= 10,
	["CH11"] 		= 11,
	["CH12"] 		= 12,
	["CH13"] 		= 13,
	["CH14"] 		= 14,
	["CH15"] 		= 15,
	["CH16"] 		= 16,

	["ch1"] 		= 1,
	["ch2"] 		= 2,
	["ch3"] 		= 3,
	["ch4"] 		= 4,
	["ch5"] 		= 5,
	["ch6"] 		= 6,
	["ch7"] 		= 7,
	["ch8"] 		= 8,
	["ch9"] 		= 9,
	["ch10"] 		= 10,
	["ch11"] 		= 11,
	["ch12"] 		= 12,
	["ch13"] 		= 13,
	["ch14"] 		= 14,
	["ch15"] 		= 15,
	["ch16"] 		= 16,
}
local gOpto_CH = {
	["out_ch1"] 		= 9,
	["out_ch2"] 		= 10,
	["out_ch3"] 		= 11,
	["out_ch4"] 		= 12,
	["out_ch5"] 		= 13,
	["out_ch6"] 		= 14,
	["out_ch7"] 		= 15,
	["out_ch8"] 		= 16,
	["in_ch1"]			= 1,
	["in_ch2"]			= 2,
	["in_ch3"]			= 3,
	["in_ch4"]			= 4,
	["in_ch5"]			= 5,
	["in_ch6"]			= 6,
	["in_ch7"]			= 7,
	["in_ch8"] 			= 8,
}
local gDMM_INIT = {["IntDMM"] = 0, ["ExtDMM"] = 1}
-- default leopard ctrl time
local Leo_ExtIO_Delaytime = 50
function cat9555_io_search(number1,number2)
	-- body
	if(type(number1) ~= "number") or (type(number2) ~= "number") then
		return nil
	end
	return number1 * 16 + number2
end
-- EXIO output io cmd
-- par1: IO number
-- par2: 0 or 1(high)
-- par3: comm receive timeout
-- return status: bool
function leo_exio_set(io_n,state,...)
	-- body
	local timedelay = ...
	if type(timedelay) ~= "number" then
		timedelay = Leo_ExtIO_Delaytime
	end
	local command_b 	= "exio_set(\""
	local command_e 	= ",\")\r\n"
	local command_io 	= string.format( "%d",io_n)
	local command_state = string.format( "%d",state)
	local command 		= command_b .. command_io .. "=" .. command_state .. command_e
	-- ShowMsg(command)
	local rec_str = leo_comunication(command,timedelay)

	local state = lep_rec_cmd_decode(rec_str)
	return state
end
function leo_exio_get(io_n,...)
	-- body
	local timedelay = ...
	if type(timedelay) ~= "number" then
		timedelay = Leo_ExtIO_Delaytime
	end
	local command_b 	= "exio_get("
	local command_e 	= ")\r\n"
	local command_io 	= string.format( "%d",io_n)
	local command 		= command_b .. command_io .. command_e
	ShowMsg(command)
	local rec_str = leo_comunication(command,timedelay)
	local state, io_temp = lep_rec_cmd_decode(rec_str)
	local io_return = tonumber(io_temp)
	return state, io_return
end
-- psu1 PSU channel
function psu1_ch_set(cat_ch,state,...)
	local io_num 	= cat9555_io_search(gCat9555_Board.PSU,cat_ch)
	local state 	= leo_exio_set(io_num,state,...)
	return state
end
function bcs1_ch_set(cat_ch,state,... )
	-- body
	local io_num 	= cat9555_io_search(gCat9555_Board.BCS,cat_ch)
	local state 	= leo_exio_set(io_num,state,...)
	return state
end
function load1_ch_set(cat_ch,state,...)
	-- body
	local io_num 	= cat9555_io_search(gCat9555_Board.LOAD,cat_ch)
	local state 	= leo_exio_set(io_num,state,...)
	return state
end


--[[
	DMM switch baord init IO_DMM_MUX board:
		1.external standard DMM
		2.internal DMM
	two select will cause IO_DMM_MUX board switch different relay(0.7)
	... is 0, select internal DMM, nil or > 1 is external
--]]
function dmm1_ch_init( ... )
	-- body
	local DMM_S = ...
	if(type(DMM_S) ~= "number") then
		DMM_S = gDMM_INIT.ExtDMM
	end
	if DMM_S ~= gDMM_INIT.IntDMM then
		DMM_S = gDMM_INIT.ExtDMM
	end
	local io_num 	= cat9555_io_search(gCat9555_Board.DMM,8)
	local state 	= leo_exio_set(io_num,DMM_S,100)
	return state
end
function dmm1_ch_set(cat_ch,state,...)
	-- body
	local io_num 	= cat9555_io_search(gCat9555_Board.DMM,cat_ch)
	local state 	= leo_exio_set(io_num,state,...)
	return state
end
--ctrl opto MUX, cat_ch:channel,state:0 or 1, ... delay
function opto_io_set(cat_ch,state,...)
	-- body
	local io_num 	= cat9555_io_search(gCat9555_Board.IO_MUX,cat_ch)
	local state 	= leo_exio_set(io_num,state,...)
	return state
end
--cat io, delay
--retrun status
function opto_io_get(cat_io,...)
	if(cat_cj >=9 ) then
	 	return false,0
	end
	local io_num 	= cat9555_io_search(gCat9555_Board.IO_MUX,cat_io)
	local rec_state, io_state = leo_exio_get(io_num,...)
	return rec_state, io_state
end
--#endregion
----------------------------Leo_ExtIO Mode-------------------------
----------------------------Leo_ExtIO Mode-------------------------

---------------------------------------------------------

function dut_communication(addr,comm)
	local comm1	= fc2012_send_command(addr,comm)
	return comm1
end
function fc2012_send_command(addr,comm) 	--fc2012 intput: cmd + cmd data, carry out completed cmd
	if type(addr) ~= "string" then
	addr = ""
	end
	if type(comm) ~= "string" then
	comm = ""
	end
	local comm1      = MCB_addr..addr..string.format("%02x",string.len(comm)/2)..comm
	local sum1       = string.sub(checksum_s(comm1,4),3,4)
	local sum2       = string.sub(checksum_s(comm1,4),1,2)
	local comm1      = "3A"..comm1..sum1..sum2.."0D".."0A"

	local temp2      = serial_cmd_hexstr(20,comm1,100)
	local comm3,state=fc2012_analysis_command(temp2)
	--ShowMsg(comm1)
    return comm3,state
end

function fc2012_analysis_command(comm)  	--handle receive cmd, div them & check, then return cmd
	local temp=comm
	local length = string.len(temp)
	if length<16 then
		return "",false
	end
	local sumcheck=string.sub(temp,length-7,length-4)    --intercept checksum
	local temp2=string.sub(temp,3,length-8)  --intercept data to be check
	temp = string.sub(temp,9,length-8)       --intercept cmd
	local sumcheck2=string.sub(checksum_s(temp2,4),3,4)..string.sub(checksum_s(temp2,4),1,2) --calc check
	if sumcheck~=sumcheck2 then   --compare checksum
		return "",false
	else
		return temp,true  --return cmd
	end
end
local WET_SCRUBBER_COMMAND = {
--STATE COMMAND
["STATE_MAIN"] 							     	=	"30",
["STATE_KEY"]									= 	"31",
["STATE_BATT"]									=	"50",
["STATE_CHG"]									=	"51",
["STATE_MOT0E"]								    =	"52",
["STATE_BRUSH"]								    =	"53",
["STATE_PUMP"]									= 	"54",
["STATE_PRECT"]								    =	"5A",
["XXX"]											=	"5B", --??
["STATE_BMS_FW_APP"]							=	"80",
["STATE_BMS_SW"]								=	"81",
["STATE_BMS_FW_EEP"]							= 	"85",
["STATE_BMS_ID"]								= 	"A2",
--["STATW_BMS_SN"]								=	"A3",
--CTRL COMMAND
["CTRL_RTC_SET"]								=	"B0",
["CTRL_RTC_INIT"]								=	"B6",
["CTRL_MFG"]									=	"B1",
["CTRT_BMS_ID"] 								= 	"B2",
["CTRL_BMS_SOC"]								=	"B4",
["CTRL_LOG_PRINT"]							    = 	"C0",
["CTRL_FAC_MODE"]								=   "C1",
["CTRL_TEST_MODE"]							    =   "C1",
["CTRL_MOTOR"]									= 	"C2",
["CTRL_LED"]									=	"C3",
["CTRL_CHG"]									=	"C4",
["CTRL_CLEE_OTP"]								=	"E0",
["CTRL_RESET"]							    	=	"E1",
["CTRL_SLEEP"]									=	"E2",
["CTRL_SHUTDOWN"]								=	"E3",
["CTRL_KEY"]									= 	"E4",
}

local FC2012_STATE_CMD = {
	["BATT_VOLT"] 		= 					0,
	["CHG_VOLT"]		=					1,
	["BRUSH_VOLT"] 		= 					2,
	["BRUSH_N_VOLT"]	= 					3,
	["PUMP_VOLT"]		=					4,
	["PUMP_N_VOLT"]		=					5,
	["BATT_DSG_CURR"]	=					6,
	["BATT_CHG_CURR"]	=					7,
	["MOTOR_CURR"]		=					8,
	["BRUSH_CURR"]		=					9,
	["PUMP_CURR"]		=					10,
	["MOTOR_TEMP"]		=					11,
	["BRUSH_TEMP"]		= 					12,
	["PUMP_TEMP"]		=					13,
	["MOTOR_FG_FREQ"] 	=					14,
	["MOTOR_PWM_DUTY"]  = 					15,
	["BRUSH_PWM_DUTY"]	=					16,
	["PUMP_PWM_DUTY"]	=					17,
	["PUMP_PWM_FREQ"]	=					18,
}


function state_main(state_cmd)   --read main status
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND.STATE_MAIN,"")
	local len = string.len(temp)
	--ShowMsg(len)
	if( len~=76) then
		return ""
	end
	local str 		= string.sub(temp,state_cmd*4+1,state_cmd*4+4)
	local temp2     = string.sub(str,3,4)..string.sub(str,1,2)
	return temp2
end


function IO_MUX_Ctrl(relay,state)    --IO DMM MUX ctrl
end

function relay_mux_ctrl()   --RelayMux board ctrl
end

function dmm_read()  --internal DMM read
end

function code_judge( code_len,... )
end




local _CONFIG_NAME = "dev_config_FC2012.txt"
local _WRITE 	   = "w"
local _READ	 	   = "r"
local _FILE_DEV    = nil				-- write file handles, after read _CONFIG_NAME file then to point the file
function dut_scan(...)
	-- body
	-----------------------
	--
	-- psu_init()
	-- psu_set_volt_curr(28000,500)
	-- psu_output_on()
	-- Sleep(1500)
	-- psu_output_off()

	--DUT_COM_Table["DUT1"] = sh36730_find(...)
end
function dev_init()
	if get_test_state() then
		_FILE_DEV = io.open(_CONFIG_NAME,_READ)
		if _FILE_DEV ==nil then
			--if can not find file, then creat default device table
			--dev_def_config_load()
			psu_load_scan(150)
			-- dut_scan(150)	--by different DUT write different scan function
		else
			dev_load()   --file load to lua
			_FILE_DEV:close()
		end

		--clear data
		_FILE_DEV = io.open(_CONFIG_NAME,_WRITE)
		_FILE_DEV:close()
	end
end

function dev_load()
	if get_test_state() then
		local dev_table = {}
		local str_line = nil
		local line = 1

		str_line = _FILE_DEV:read()
		while str_line ~=nil
		do
			dev_table[line] = str_line
			line = line + 1
			str_line = _FILE_DEV:read()
		end
		if line < 3 then  		--instrument less 3
			psu_load_scan(150)
			dut_scan(150)		--by different DUT write different scan function
		else
			DUT_COM_Table["DUT1"] 	 = tonumber(dev_table[1])
			-- PSU_COM_Table["PSU1"] 	 = tonumber(dev_table[2])
			-- LOAD_COM_Table["LOAD1"]  = tonumber(dev_table[3])
		end
	end
end

function dut_init(...)
	-- body
	if get_test_state() then
		local PortName = ...
		if PortName == nil then
			PortName = DUT_COM_Table["DUT1"]
		end
		if PortName == 0 then
		else
			--serial_sh36730x_init(PortName)
		end
	end
end

function enter_test_mode()    -- get into test mode, return 0:fail 1:pass

	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_TEST_MODE"],"46544D")

	SetUI_Auto("enter_test_mode",temp,"01","01")
	return temp
end

function reset()  -- restart, led const blink
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_RESET"],"5253")
    return temp
end

function enter_sleep()
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_SLEEP"],"534C")
    return temp
end

function shutdown()
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_SHUTDOWN"],"5344")
    return temp
end

function check_hw_version(hw)
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["STATE_BMS_FW_APP"],"")
	SetUI_Auto("check_hw_version",temp,hw,hw)
    return temp
end

function check_fw_version(fw)
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND.STATE_BMS_SW,"")
	SetUI_Auto("check_fw_version",temp,fw,fw)
    return temp
end

function all_led_on(...)
	local led = ...
	local led_cmd = ""
	if(type(led)~="number") or (type(led) == "number" and led >8) then
		led_cmd = led_cmd .."050AFF"
	else
		led = BitLeft(0x01,led)
		led_cmd = "050A" .. string.format("%02X",led)
	end
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_LED"],led_cmd)
    return temp
end

function all_led_off()
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_LED"],"050A00")
    return temp
end
function read_BMS_id()
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["STATE_BMS_ID"],"")
    return temp
end
function fc2012_main_motor_on()    --only open main motor
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_MOTOR"],"050A01")
	return temp
end
function fc2012_floor_brush_motor_on()   --only open floor brush motor
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_MOTOR"],"050A02")
	return temp
end
function fc2012_pump_motor_on()    --only open pump motor
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_MOTOR"],"050A04")
	return temp
end
function fc2012_all_motor_off()    --close all motor
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_MOTOR"],"050A00")
	return temp
end
function fc2012_check_key_state()  --check button status, which button pressed?
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["STATE_KEY"],"")
	return temp
end


function normal_curr_consumption(section,min,max)
	--TODO: OVP board
end
function shutdown_curr_consumption(section,min,max)
	--TODO: OVP board
end
function c_p_to_led_p_function()
end

function main_motor_mos(volt)
	fc2012_send_command()--close mos
	relay_mux_ctrl()--switch e-load to main circuit
	load_set_curr()
	load_output_on()
	load_read_volt_n()
	--judge
	fc2012_send_command()--enable mos
	load_read_curr_n()
	--judge
	load_close()--close e-load
	relay_mux_ctrl() --close e-load circuit
end
