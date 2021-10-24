--打开sublime的“package control“，输入inp,可以看到install package选项，搜索 ConvertToUTF8 安装， File 菜单栏增加转码选项，转换为 GBK格式即可
--uartscan_it6863a()

--	链接 以太网，
--	链接 以太网，
--#region 链接 以太网，使用此函数前，先初始化。
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

--以太网通讯
local function Tcp_cmd(str,timedelay)
	TcpReadStringBuff()
	TcpSendString(str .. "\r\n")
	Sleep(timedelay)
	return  TcpReadStringBuff()
end
--#endregion


local MCB_addr =18       --主控板地址





DUT_COM_Table 	= {["DUT1"]  = 0 }	--默认DUT串口	不能填 nil
PSU_COM_Table 	= {["PSU1"]  = 0 }	--默认电源串口	不能填 nil
LOAD_COM_Table  = {["LOAD1"] = 6 }	--默认负载串口	不能填 nil
--仪表协议 SCPI, FRAME
PSU_PROT_Table  = {["PSU1"]  = "SCPI"  }
LOAD_PROT_Table = {["LOAD1"] = "FRAME" }

_COMMAND_WAITTIME_LOAD 		= 100 --ms 串口发送接受延时时间，
_COMMAND_WAITTIME_PSU 		= 100
_COMMAND_WAITTIME_DUT 		= 50


local PSU_value 			= {["CURR"] = 0, ["VOLT"] = 0}		--电源表设置输出值
local Prot_value 			= {["CURR"] = 0, ["VOLT"] = 0}		--电源表设置保护值

local SerialPortCount 		= 8									--串口数量



---------------------------------------------------------
---------------------------------------------------------


_KeepRet 			= true
_FailToContinue 	= true    -- true: NG 继续测试，False：NG 停止测试
DEV_USBCAN  		= "DEV_USBCAN"
DEV_USBCAN2 		= "DEV_USBCAN2"
Getindex()
gPacKBatterCode     = GetPackCode()		      --全局电池条码
gCellCode 		    = GetCellCode()			  --全局电芯条码/临时条码

--全局变量，配合function gettime_slot()使用
_gbTimeBegin = TimeStart()	
_gbTimeSun 	 = 0
_gbTimeSlot  = 0
--返回两次调用之间的时间，默认开始测试调用一次，或手动在必要位置调用一次
--单位 S
--每次调用 SetUI_Auto() 将会调用此函数
--每次
function gettime_slot() 	
	_gbTimeSlot = TimeStop_MS()
	_gbTimeSun 	= _gbTimeSun + _gbTimeSlot  
	TimeStart()									--重新计算数据段
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
	--数字类型
	if type(ActMeas) =="number" and type(LimitMin) =="number" and type(LimitMax) == "number" then 						
		if ActMeas >=LimitMin and ActMeas <= LimitMax then
			TestRest =true
		end
	--字符串类型
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

--判断 value 是否 table1 中,返回 true ，fales
function  is_intable( value, table1 )
	for k,port in pairs(table1) do		
		if port == value then 
			return true
		end
	end
	return false
end
--数转16进制字符串 转出长度默认2位 最长8位，可变参指定转换长度
function num2hexstr_s( Num, ...)
	local len 			= ...
	if len == nil then len = 2 end 
	if len >= 8   then len = 8 end 
	local NumQt 		= Num 		--商
	local NumStr 		= ""
	local i = 1
	repeat
		NumL 	= NumQt % 16		--余数
		NumQt 	= NumQt / 16		--商
		sNumL =string.format("%X", NumL)
		NumStr = sNumL .. NumStr
		i = i + 1
	until i > len  

	return NumStr
end
--16进制的字符串转换成对应的数值，可选择转换的起止位
  -- {--eg: hexstr2num_n("123",1,2)从第一位开始转换，到第二位结束
   	 -- hexstr2num_n("123",3) 	转换前三位数据
   	 -- hexstr2num_n("1234",-3)转换后三位数据
   	 -- hexstr2num_n("1234")	全部转换
  -- }
function hexstr2num_n(str,... )
	local arry 		= {...}
	local len_b 	= arry[1] --一参数
	local len_e 	= arry[2] --二参数
	local len_str 	= string.len(str)  
	--只有一参数,
	if len_e == nil then  										
		if len_b == nil or len_b == 0 then 			--没变参数，默认整个字符串转换									
			len_b = len_str									
		end
		if math.abs(len_b)>len_str then --参数设置不合理会全部转换
			len_b = 1		
		end 
		if len_b <-8 then len_b =-8 end --限制长度
		if len_b > 8 then len_b = 8 end 

		if len_b > 0 then 				--正参
			len_e = len_b
			len_b = 1 
		elseif len_b < 0 then			--负参	
			len_b = len_str +len_b + 1
			len_e = len_str 
		end
	--两个参数 ,--len_b代表开始地址， len_e表示结束地址
	else 																
		if len_b < 0 then len_b = len_str + len_b + 1 	end 	--负参数转正
		if len_e < 0 then len_e = len_str + len_e + 1 	end		--负参数转正
		if len_e - len_b > 8 then len_e = len_b + 8 	end 	--参数超长 
		if len_e - len_b < 0 then len_e = len_b 		end 	--参数颠倒
		if len_b <= 0 then len_b =1						end 	--参数超字符串长
		if len_e >= len_str then len_e =len_str			end		--参数超字符串长
	end
	--len_b代表开始地址， len_e表示结束地址
	local num_str = string.upper(str)							--转大写	
	local num_arr = { ["0"] = 0, ["1"] = 1, ["2"] = 2, ["3"] = 3, ["4"] = 4, ["5"] = 5, ["6"] = 6, ["7"] = 7, 
					  ["8"] = 8, ["9"] = 9, ["A"] = 10,["B"] = 11,["C"] = 12,["D"] = 13,["E"] = 14,["F"] = 15 }
	local num = 0
	for i = len_b, len_e do
		local num_b = num_arr[string.sub(num_str,i,i)]
		if  num_b ~= nil then 	--防止输入非 0~9 和 A~F 的字符，可以考虑在前面拦截
			num = num*16 + num_b
		end
	end
	return num
end
--crc8 校验 
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
--校验和计算，输入字符串和校验和长度，输出字符串校验和
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
--在字符串后面叫上校验和
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
--求Table 中数据的最大差值, ~~电芯压差
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
		--增加限制
		--SerialPortInit(PortName, Baud_rate, checkbit, databit, stopbit)
	end
	PortSerialOpen(PortName)
	PortClearBuff(PortName)	
end
--发送接收HexStr格式的字符串
function serial_cmd_hexstr( PortName, SendStr, waittime)	--串口号, 发送字符串，接收的延时
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
--发送接收Str字符串
function serial_cmd_str( PortName, SendStr, waittime)	--串口号, 发送字符串, 接收的延时
    PortClearBuff(PortName)	
    PortSendStr(SendStr, PortName)
	Sleep(waittime)
	local rx_str = PortGetStr(PortName)
	return rx_str
end


-----------------------电源表it6863a-----------------------
--#region--------------电源表it6863a-----------------------
--一些 SCPI 命令
local SCPI_END 								= "\r\n"
local SCPI_SYST_MESSAGE						= "*IDN?" 				..SCPI_END	--
local SCPI_SYST_REM							= "SYST:REM" 			..SCPI_END	--程控
local SCPI_SYST_LOC							= "SYST:LOC" 			..SCPI_END	--手动控制
local SCPI_CLS								= "*CLS"				..SCPI_END	--清除错误
local SCPI_RST								= "*RST"				..SCPI_END	--初始化
---------------------------	电源表	--------------------------
local SCPI_OUT_ON							= "OUTP ON" 			..SCPI_END	--使能输出
local SCPI_OUT_OFF							= "OUTP OFF" 			..SCPI_END	--关闭输出
local SCPI_MEAS_VOLT						= "MEAS:VOLT?"			..SCPI_END	--回读输出口电压
local SCPI_MEAS_CURR						= "MEAS:CURR?"			..SCPI_END	--回读输出电流
local SCPI_VOLT_RANG_LOW 					= "VOLT:RANG low"		..SCPI_END	--电源输出档输出低档位切换
local SCPI_PROT_VOLT						= "VOLT:PORT?"			..SCPI_END	--保护电压
local SCPI_VOLT_RANG_HIGH 					= "VOLT:RANG high"		..SCPI_END	--电源输出档输出高档位切换

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
			PortName=string.gsub(SerialPortName[i],"COM","") 	--必须通过Portname_R 在转成 PortName 
	    	PortName = tonumber(PortName)	 
			if is_intable(PortName,PSU_COM_Table) ==false and is_intable(PortName,LOAD_COM_Table) ==false and is_intable(PortName,DUT_COM_Table) ==false then  
				local find_b = nil
				local scpi_bit = false				

				SerialPortInit(PortName,9600,0,8,1)	
				PortSerialOpen(PortName)
				PortClearBuff(PortName)		
				PortSendStr("*CLS\r\n",PortName)
				Sleep(waittime)	
				PortClearBuff(PortName)								--指令，串口号(不可省略)
				PortSendStr("*IDN?\r\n",PortName)					--指令，串口号(不可省略)
				Sleep(waittime)
				local read  = PortGetStr(PortName)
				if type(read)=="string" then
					scpi_bit = true 		
					-----------------------------------------------
					-----------------------------------------------
					--ShowMsg(tostring(PortName)..tostring(read))

				  	find_b=string.match(read,"IT6")				--IT6 开头的电源表
				  	if(find_b ~= nil) then
				  		PSU_COM_Table["PSU1"] = PortName		
				  		PSU_PROT_Table["PSU1"] = "SCPI"
				  	end
				-----------------------------------------------
				-----------------------------------------------
				  	find_b=string.match(read,"IT8")				--IT8 开头的电子负载
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
						is_load=string.match(read_str,"3332313032")				--IT6 开头的电源表
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

function scpi_command( PortName, order, waittime )	--串口号, 发送字符串, 接收的延时实际
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
	serial_init(PortName_R)												--打开端口
	scpi_command(PortName_R,SCPI_SYST_REM,waittime)									--程控
	scpi_command(PortName_R,SCPI_CLS,waittime)										--清除错误
	--scpi_command(PortName_R,SCPI_RST,waittime)									--不初始化
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
	--切换量程
	---------------------------------------------------------
	if(set_curr >= 1500 ) then
		if(PSU_value["VOLT"] >=32000) then
			set_curr = 1500
		else
			--切换量程
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
			--切换量程
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
--  代码区	代码区	    电源表it6863a	 电子负载  代码区	代码区		 

-----------------------IT8500 / IT32102 电子负载------------------
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
-- 命令补长到25 字节
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
	order = it32102_command_package(order) 		--	补全长度
	order = addchecksum_s(order)				--	加checksum
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
	serial_init(PortName_R)	--打开串口

	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_REM,waittime) 			--程控
	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_OFF,waittime) 			--关闭输出	LOAD_LOCAL_ENABLE
	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_LOCAL_ENABLE,waittime) --允许面板控制
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
	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_OFF,waittime) 			--关闭输出	LOAD_LOCAL_ENABLE
	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_LOCAL_ENABLE,waittime) --允许面板控制
	it32102_cmd(PortName_R,LOAD_SYNC..LOAD_ADDR..LOAD_LOC,waittime) 			--程控

	PortSerialClose(PortName_R)  --关闭串口
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
	local curr_set = math.floor(curr*10.0)			 	--	最小分辨率为 0.1A
	curr_set = num2hexstr_s(curr_set,4)					-- 	转换为 8*2长度的 hexstr
	curr_set = msb2lsb_hexstr(curr_set)					--	低位在前
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
	--接受的前 len 个数据和 发送的命令 相同，此处为了校验，防止接受错误	
	local len = string.len(cmd_str)					--发送命令长度							
	local rx_check = string.sub(rx_str,1,len)		--截取接受命令 len 个长度
	if rx_check ~= cmd_str then                     --比对
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
--  代码区	代码区	   -IT8500 / IT32102 电子负载  代码区	代码区		 
-----------------------IT8500 / IT32102 电子负载------------------

------------------------------------------------------------------
--#region --------------ITM3432/双向电源---------------------------
local LEO_ITM3432_COMMAND_TIMEDELAY = 50 --默认命令指令接受延时。
local LEO_ITM3432_OUTPUT_MODE = 999 	--标志位 1 为当前为 输出模式(输出电流)，2 为输入模式(吸收电流)
local LEO_ITM3432_OUTPUT_CHG_FLAG = 1	--标志位
local LEO_ITM3432_OUTPUT_DSG_FLAG = 2	--标志位

--leo 板通讯命令，发送 str 命令，并在 指定时间返回 接受的str
-- ：参数1 要发送的str, 
-- ：参数2 可选 命令接受延时时间
-- ：return 接受到的命令
local  function leo_comunication(str,...)
	local timedelay = ...
	if type(timedelay) ~="number" then
		timedelay = LEO_ITM3432_COMMAND_TIMEDELAY
	end
	local rec_str = Tcp_cmd( str,timedelay)
	return rec_str
end

--解析leopard 返回的 指令 boolean,str,str,str
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
-- 充电设置，输入 电压，电流，延时时间, 返回 true or false
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
	if LEO_ITM3432_OUTPUT_MODE == LEO_ITM3432_OUTPUT_CHG_FLAG then -- 一样就代表上次也为充电模式，则 命令为 
		mode = 2 --持续充(不会断电)
	else
		mode = 0 -- 会断电
	end
	local time_str = tostring(timedelay)
	local send_str = "psu_set_vi("..send_volt ..",".. send_curr..",".. mode ..",".. time_str .. ",36.0,3.6,20.0,0.05)"
	local rec_str = leo_comunication(send_str,timedelay)
	local state = lep_rec_cmd_decode(rec_str)
	if(state == "pass") then 
		--成功标记为充电状态
		LEO_ITM3432_OUTPUT_MODE = LEO_ITM3432_OUTPUT_CHG_FLAG
		return true 
	else
		--失败则标为非正常状态(待确定)
		-- LEO_ITM3432_OUTPUT_MODE = 999 
		return false
	end
end
-- 放电设置，输入 电压，电流，延时时间, 返回 true or false
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
	if LEO_ITM3432_OUTPUT_MODE == LEO_ITM3432_OUTPUT_DSG_FLAG then -- 一样就代表上次也为充电模式，则 命令为 
		mode = 3 --持续放(不会断电)
	else
		mode = 1 --会断电
	end
	local time_str = tostring(timedelay)
	--拼接命令
	local send_str = "psu_set_vi("..send_volt ..",".. send_curr..",".. mode ..",".. time_str .. ",36.0,3.6,20.0,0.05)"
	-- ShowMsg(send_str)
	local rec_str = leo_comunication(send_str,timedelay)
	local state = lep_rec_cmd_decode(rec_str)
	if(state == "pass") then 
		--成功标记为充电状态
		LEO_ITM3432_OUTPUT_MODE = LEO_ITM3432_OUTPUT_DSG_FLAG
		return true 
	else
		--失败则标为非正常状态(待确定)
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
--  代码区	代码区		 ITM3432/双向电源  代码区	代码区		 
------------------------ITM3432/双向电源--------------------------






-------------------------以下是电源表控制函数-----------------------------------------------
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
			ShowMsg("电源表通信异常，键盘按回车继续")
			_KeepRet = false 
		end
	end
end

function psu_read_curr_n()
	-- body
	if PSU_PROT_Table["PSU1"] == "SCPI" then 
		return it6863a_read_curr_n()

	elseif PSU_PROT_Table["PSU1"] == "FRAME" then 

	end
end

function psu_read_volt_n()
	-- body
	if PSU_PROT_Table["PSU1"] == "SCPI" then 
		return it6863a_read_volt_n()

	elseif PSU_PROT_Table["PSU1"] == "FRAME" then 
	end
end

function psu_set_volt_curr(volt,curr)
	-- body
	if PSU_PROT_Table["PSU1"] == "SCPI" then 
		it6863a_set_volt_curr(volt,curr)

	elseif PSU_PROT_Table["PSU1"] == "FRAME" then 
	end
end

function psu_output_on()
	-- body
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

-------------------------以下是电子负载控制函数------------------------------------------
--
--#region
function load_init()
	-- body
	if get_test_state() then 
		if LOAD_COM_Table["LOAD1"] ~=0 then 
			if LOAD_PROT_Table["LOAD1"] == "SCPI" then 
			elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then 
				it32102_init()
			end
		else
			ShowMsg("电子负载通信异常，键盘按回车继续")
			_KeepRet = false 
		end
	end   --电子负载初始化 
end
function load_close( ... )
	-- body
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then 
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then 
		it32102_close()
	end
end
function load_set_curr( curr )
	-- body
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then 
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then 
		it32102_set_curr(curr)
	end
end
function load_read_volt_curr_2n()
	-- body
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
	-- body
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then 
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then 
		it32102_enable()
	end
end	
function load_output_off()
	-- body
	if LOAD_PROT_Table["LOAD1"] == "SCPI" then 
	elseif LOAD_PROT_Table["LOAD1"] == "FRAME" then 
		it32102_disable()
	end
end
--#endregion

-------------------------以下是电池模拟器控制函数----------------------------------
--
--#region
function battray_init()

end

function battray_cell_voltage(volt)   --设置电压
end

function battray_charge_set(volt,curr)  --设置充电
	return leo_m3432_chg_set(volt,curr,2000)
end

function battray_discharg_set(volt,curr)  --设置放电
	leo_m3432_dsg_set(volt,curr,2000)
end

function battray_read_v_i()  --读电压
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

local gCat9555_CFG = {				--  16位数字代表CAT9555 16个IO口的输入输出状态，0代表输出, 1为输出。
	["BCS"]			= 0x0000,		--	均为输出模式，控制继电器
	["PSU"] 		= 0x0000,		--	均为输出模式，控制继电器
	["LOAD"] 		= 0x0000,		--	均为输出模式，控制继电器
	["IO_MUX"]		= 0x00FF,		--	低8位IO位输入模式，检测输入电平逻辑，高8位IO输出模式，控制固态继电器
	["DMM"]			= 0x0000,		--  均为输出模式，控制继电器，万用表测试电压或者交流内阻 	
}
--硬件接线保持一致
local gCat9555_Board = {			-- 	A02 A01 A00 跳线帽
	["BCS"] 		= 0x00,			--	L	L 	L
	["PSU"] 		= 0x01,			--	L 	L 	H
	["LOAD"] 		= 0x02,			--	L 	H 	L
	["IO_MUX"]		= 0x04,			--	H 	L 	L 
	["DMM"]			= 0x05,			--	H 	H 	L 	
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
	--小写
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
-- 默认下位机控制拓展IO时间
local Leo_ExtIO_Delaytime = 50
function cat9555_io_search(number1,number2)
	-- body
	if(type(number1) ~= "number") or (type(number2) ~= "number") then 
		return nil
	end 
	return number1 * 16 + number2
end
--EXIO 输出IO控制 命令格式拼接
-- 参数1: IO号， 
-- 参数2： 0（低）或 1 （高）
-- 参数3： 可选，通讯接收延时时间，不设置默认值为 Leo_ExtIO_Delaytime
-- 返回 设置状态 ， true or  false 
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
-- psu1 电源设置 通道
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
	DMM切换板初始化IO_DMM_MUX 板 的DMM切换模块 有两个选择:
		1.外部DMM(接标准仪器测量) 
		2.内部DMM(接自己开发DMM)
	这两选择会导致IO_DMM_MUX 板 的DMM切换模块的一个继电器状态不一样(0.7)。
	... 可变参数为0 选择内部DMM，为空或者大于1 则选择外部DMM
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
--控制光耦MUX，cat_ch 为通道口， state 为 0，1（输出高），... 为 延时。
function opto_io_set(cat_ch,state,...)
	-- body
	local io_num 	= cat9555_io_search(gCat9555_Board.IO_MUX,cat_ch)
	local state 	= leo_exio_set(io_num,state,...)
	return state
end
--输入 IO，延时时间
--返回 对应时间
function opto_io_get(cat_ch,...)
	if(cat_cj >=9 ) then
	 	return false,0
	end
	local io_num 	= cat9555_io_search(gCat9555_Board.IO_MUX,cat_ch)
	local rec_state, io_state = leo_exio_get(io_num,...)
	return rec_state, io_state
end
--#endregion
--  代码区	代码区		 	 Leo_ExtIO Mode 代码区	代码区		 
----------------------------Leo_ExtIO Mode-------------------------

---------------------------------------------------------

function dut_communication(addr,comm)  		--通用的DUT通信函数
	local comm1	= fc2012_send_command(addr,comm)
	return comm1
end
function fc2012_send_command(addr,comm) 	--fc2012用，输入命令码和命令内容，返回补充帧头帧尾以及校验和的完整命令,并发送出去
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

function fc2012_analysis_command(comm)  	--收到命令先切开，然后校验，最后再返回命令内容
	local temp=comm
	local length = string.len(temp)
	if length<16 then
		return "",false
	end
	local sumcheck=string.sub(temp,length-7,length-4)    --截取校验和
	local temp2=string.sub(temp,3,length-8)  --截取被校验内容
	temp = string.sub(temp,9,length-8)       --截取命令内容
	local sumcheck2=string.sub(checksum_s(temp2,4),3,4)..string.sub(checksum_s(temp2,4),1,2)  --根据命令内容算出校验和
	if sumcheck~=sumcheck2 then   --对比校验和，确保接收的命令无误
		return "",false
	else
		return temp,true  --校验后返回命令内容
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


function state_main(state_cmd)   --读取主要状态
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


function IO_MUX_Ctrl(relay,state)    --IO DMM MUX 控制
end

function relay_mux_ctrl()   --RelayMux板控制
end

function dmm_read()  --内部DMM读取
end

function code_judge( code_len,... )   
end




local _CONFIG_NAME = "dev_config_FC2012.txt"
local _WRITE 	   = "w"
local _READ	 	   = "r"
local _FILE_DEV    = nil				-- 写文件句柄，需要在读入 _CONFIG_NAME 文件后 指向该文件
function dut_scan(...)
	-- body
	-----------------------
	--刚刚接上可以通信，时间长了要充电才可以激活通信
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
			--如果没有该文件，不需要关闭，新建默认设备表
			--dev_def_config_load()
			psu_load_scan(150)
			-- dut_scan(150)	--根据各产品不一样选择编写合适的扫描函数
		else
			dev_load()   --文件导入设备
			_FILE_DEV:close()
		end
	
		--清除数据
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
		if line < 3 then  		--仪器少于3个
			psu_load_scan(150)
			dut_scan(150)		--根据各产品不一样选择编写合适的扫描函数
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

function enter_test_mode()    --进入测试模式，失败回00，成功回01

	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_TEST_MODE"],"46544D")

	SetUI_Auto("enter_test_mode",temp,"01","01")
	return temp
end

function reset()  --重启，实测没回复，但是LED会一直闪动
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

function check_hw_version(hw)   --硬件版本
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["STATE_BMS_FW_APP"],"")
	SetUI_Auto("check_hw_version",temp,hw,hw)
    return temp
end

function check_fw_version(fw)   --软件版本
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
function fc2012_main_motor_on()    --只开主电机
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_MOTOR"],"050A01")
	return temp 
end
function fc2012_floor_brush_motor_on()   --只开地刷电机
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_MOTOR"],"050A02")
	return temp 
end
function fc2012_pump_motor_on()    --只开水泵电机
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_MOTOR"],"050A04")
	return temp 
end
function fc2012_all_motor_off()    --关闭全部电机
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["CTRL_MOTOR"],"050A00")
	return temp 
end
function fc2012_check_key_state()  --检查按键状态，哪个按键被按下了？
	local temp=fc2012_send_command(WET_SCRUBBER_COMMAND["STATE_KEY"],"")
	return temp	
end


function normal_curr_consumption(section,min,max)
	--OVP板还没画
end
function shutdown_curr_consumption(section,min,max)
	--OVP板还没画
end
function c_p_to_led_p_function()
end

function main_motor_mos(volt)
	fc2012_send_command()--关闭mos
	relay_mux_ctrl()--电子负载切到主机回路
	load_set_curr()
	load_output_on()
	load_read_volt_n()
	--判断
	fc2012_send_command()--使能mos
	load_read_curr_n()
	--判断
	load_close()--关闭电子负载
	relay_mux_ctrl() --断开电子负载
end
function floor_brush_motor_mos(volt)
	fc2012_send_command()--关闭mos
	relay_mux_ctrl()--电子负载切到地刷回路
	load_set_curr()
	load_output_on()
	load_read_volt_n()
	--判断
	fc2012_send_command()--使能mos
	load_read_curr_n()
	--判断
	load_close()--关闭电子负载
	relay_mux_ctrl() --断开电子负载
end
function pump_motor_mos(volt)
	fc2012_send_command()--关闭mos
	relay_mux_ctrl()--电子负载切到水泵回路
	load_set_curr()
	load_output_on()
	load_read_curr_n()
	--判断
	fc2012_send_command()--使能mos
	load_read_curr_n()
	--判断
	load_close()--关闭电子负载
	relay_mux_ctrl() --断开电子负载
end
function charge_curr(diff_value)
	relay_mux_ctrl()--电源切到充电回路
	psu_set_volt_curr() 
	psu_output_on()
	local psu_curr=psu_read_curr_n()
	local dut_charge_curr=fc2012_send_command(add,com)--读电流
	--比较电流

	if math.abs(psu_curr - dut_charge_curr)<diff_value then
	return true
	else
	return false
	end
end
function main_motor_curr(curr,diff_value)
	relay_mux_ctrl()--电子负载切到主机回路
	load_set_curr(curr)
	load_output_on()--启动电子负载
	local load_curr = load_read_curr_n()
	local main_motor_curr = fc2012_send_command(add,com)--读主机电流
	--判断
	if math.abs(load_curr - main_motor_curr)<diff_value then
	return true
	else
	return false
	end
	load_close()--关闭电子负载
	relay_mux_ctrl() --断开电子负载
end
function floor_brush_motor_curr(curr,diff_value)
	relay_mux_ctrl()--电子负载切到地刷回路
	load_set_curr(curr)
	load_output_on()--启动电子负载
	local load_curr = load_read_curr_n()
	local floor_brush_motor_curr = fc2012_send_command(add,com)--读地刷电流
	--判断
	if math.abs(load_curr - floor_brush_motor_curr)<diff_value then
	return true
	else
	return false
	end
	load_close()--关闭电子负载
	relay_mux_ctrl() --断开电子负载
end
function pump_motor_curr()
	relay_mux_ctrl()--负载切到水泵回路
	load_set_curr(curr)
	load_output_on()--启动电子负载
	local load_curr = load_read_curr_n()
	local pump_motor_curr = fc2012_send_command(add,com)--读水泵电流
	--判断
	if math.abs(load_curr - pump_motor_curr)<diff_value then
	return true
	else
	return false
	end
	load_close()--关闭电子负载
	relay_mux_ctrl() --断开电子负载
end
function main_motor_voltage(volt_min,volt_max)
	relay_mux_ctrl()--把电子负载接到主机回路
	load_set_curr()
	load_output_on()
	local load_volt = load_read_volt_n()--电子负载读电压
	local main_motor_voltage = fc2012_send_command(add,com)  --回读主机电压
	--判断

end    

function floor_brush_motor_volt()
	fc2012_send_command(add,com)--关闭mos
	relay_mux_ctrl()--充电器接到F-
	ChargerCtrl()
	fc2012_send_command()--读AD_F_LOAD
	--计算与比较
end
function pump_motor_volt()
	fc2012_send_command(add,com)--关闭mos
	relay_mux_ctrl()--充电器接到F-
	ChargerCtrl()
	fc2012_send_command()--读AD_F_LOAD
	--计算与比较
end
function main_motor_short_circuit()
	IO_MUX_Ctrl()--三极管拉个高电平
	fc2012_send_command()
	IO_MUX_Ctrl()--还原
	--也许要放电恢复?
end
function brush_motor_short_circuit()
	IO_MUX_Ctrl()--三极管拉个高电平
	fc2012_send_command()
	IO_MUX_Ctrl()--还原
	--也许要放电恢复?
end
function pump_motor_short_circuit()
	IO_MUX_Ctrl()--三极管拉个高电平
	fc2012_send_command()
	IO_MUX_Ctrl()--还原
	--也许要放电恢复?
end
function read_charge_voltage()
	relay_mux_ctrl()--充电器接到C+C-
	ChargerCtrl()
	fc2012_send_command()
	ChargerRead()
	--计算与比较
end
function read_discharge_voltage()
	relay_mux_ctrl()--接电子负载
	EloadCtrl()--放电
	fc2012_send_command()
	EloadRead()
	--计算与比较
end
function charge_mos_ctrl()
	relay_mux_ctrl()--切到充电回路
	fc2012_send_command(add,com)--使能mos
	ChargerCtrl() 
	ChargerRead()
	fc2012_send_command(add,com)--关闭mos
	ChargerRead()
	--判断
end
function charger_detect()
	relay_mux_ctrl()--接入充电器
	ChargerCtrl()
	fc2012_send_command()--检测接入
	relay_mux_ctrl()--断开充电器
	ChargerCtrl()--关
	fc2012_send_command()--检测断开
end
--function main_motor_pwm_get
--end	
function main_motor_en_get()
end
--function main_motor_encorder_set
--end
function angle_switch()
  
	fc2012_send_command()--通信验证
	IO_MUX_Ctrl()--断开
	fc2012_send_command()--通信验证
end
function key_power_test()   --主电源按钮检测
	if(get_test_state) then
		--控制光耦MUX，cat_ch 为通道口， state 为 0，1（输出高），... 为 延时。
    	opto_io_set(gOpto_CH.out_ch1,1) 			 -- 按下主电源按钮
    	Sleep(200)
		local temp = fc2012_check_key_state()--返回02，代表主电源按钮按下
		SetUI_Auto("key_power_on",temp,"02","02")
	end
	if (get_test_state) then 
		opto_io_set(gOpto_CH.out_ch1,0)    -- 放开主电源按钮
   		Sleep(200)
		local temp = fc2012_check_key_state()
		SetUI_Auto("key_power_off",temp,"00","00")
	end
end
function boost_switch()
	if(get_test_state) then
		--控制光耦MUX，cat_ch 为通道口， state 为 0，1（输出高），... 为 延时。
    	opto_io_set(gOpto_CH.out_ch2,1) 			 -- 按下主电源按钮
    	Sleep(200)
		local temp = fc2012_check_key_state()--返回02，代表主电源按钮按下
		SetUI_Auto("boost_switch",temp,"04","04")
	end
	if (get_test_state) then 
		opto_io_set(gOpto_CH.out_ch2,0)    -- 放开主电源按钮
   		Sleep(200)
		local temp = fc2012_check_key_state()
		SetUI_Auto("boost_switch",temp,"00","00")
	end
end
function clean_switch()
	if(get_test_state) then
		--控制光耦MUX，cat_ch 为通道口， state 为 0，1（输出高），... 为 延时。
    	opto_io_set(gOpto_CH.out_ch3,1) 			 -- 按下主电源按钮
    	Sleep(200)
		local temp = fc2012_check_key_state()--返回02，代表主电源按钮按下
		SetUI_Auto("clean_switch",temp,"08","08")
	end
	if (get_test_state) then 
		opto_io_set(gOpto_CH.out_ch3,0)    -- 放开主电源按钮
   		Sleep(200)
		local temp = fc2012_check_key_state()
		SetUI_Auto("clean_switch",temp,"00","00")
	end
end


--ShowMsg(state_main(FC2012_STATE_CMD["BRUSH_TEMP"]))
--ShowMsg(state_main(FC2012_STATE_CMD["PUMP_TEMP"]))  
function main_motor_mos_temp(tempmin,tempmax)  --测主mos温度
	local temp=state_main(FC2012_STATE_CMD["MOTOR_TEMP"])
	temp=hexstr2num_n(temp)-40
	SetUI_Auto("main_motor_mos_temp",temp,tempmin,tempmax)
	return temp
end
function brush_motor_mos_temp(tempmin,tempmax)  --测mos温度
	local temp=state_main(FC2012_STATE_CMD["BRUSH_TEMP"])
	temp=hexstr2num_n(temp)-40
	SetUI_Auto("brush_motor_mos_temp",temp,tempmin,tempmax)
	return temp
end
function pump_motor_mos_temp(tempmin,tempmax)
	local temp=state_main(FC2012_STATE_CMD["PUMP_TEMP"])
	temp=hexstr2num_n(temp)-40
	SetUI_Auto("pump_motor_mos_temp",temp,tempmin,tempmax)
	return temp
end
--function led_ps_volt
--end
function LED_test()  --暂时方案：控制LED全亮一次，并弹框提示
	ShowMsg("LED deng shanshuo")
	for i=0,7 do
	all_led_on(i)
	Sleep(500)
	end
	all_led_off()
	Sleep(2000)
	SetUI_Auto("LED_test",0,0,0)
end
function exti_test_mode()
	fc2012_send_command()
end

function leo_check()
	--临时的
	local str = leo_comunication("get_version()",2000)
	local len = #str
	if(len <=1) then
		SetUI_Auto("Leo_Check","false","true","true")
	else
		SetUI_Auto("Leo_Check","true","true","true")
	end
	
end

---------------------------------------------------------
-----------------------生产测试项-------------------------
---------------------------------------------------------
---------------------------------------------------------
_FailToContinue 	= false    -- true: NG 继续测试，False：NG 停止测试
-- code_judge(12)
--Tcp_init(gIp,gPort)
--Tcp_Connect()
--leo_check() -- 检测 下位机
-- ShowMsg(leo_comunication("get_version()",1000))
-- key_power_test()
--serial_init(20)
--load_init()
-- leo_m3432_chg_set(21,0.1,2000)
-- leo_m3432_enable(500)
-- Sleep(1000)
-- opto_io_set(gOpto_CH.out_ch1,1) 			 -- 按下主电源按钮
-- Sleep(2000)
-- opto_io_set(gOpto_CH.out_ch1,0) 			 --
-- -- 
-- Sleep(1000)
-- ------------
-- ShowMsg("anjian ")
--enter_test_mode()
--check_hw_version("0D002075")
--check_fw_version("75920000")
-- key_power_test(1000) -- 激活
-- leo_m3432_disable(500)
-- load_close()



-- serial_init(20)
-- dmm1_ch_init()
-- enter_test_mode()
-- check_hw_version("0D002075")
-- check_fw_version("75920000")
-- main_motor_mos_temp(20,30)
-- brush_motor_mos_temp(20,30)
-- pump_motor_mos_temp(20,30)
-- all_led_off()
-- LED_test()
-- key_power_test()
-- boost_switch()
-- clean_switch()
-- PortSerialClose(20)
--
--SetLuaAllTestResult(true)

--

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