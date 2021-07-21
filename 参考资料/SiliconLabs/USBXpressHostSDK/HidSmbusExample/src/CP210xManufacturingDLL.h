#ifndef	HOST_LIB_MANUFACTURING_INCLUDE_CP210XMANUFACTURINGDLL_H_INCLUDED_XF3N9QM3BK
#define	HOST_LIB_MANUFACTURING_INCLUDE_CP210XMANUFACTURINGDLL_H_INCLUDED_XF3N9QM3BK

/// @addtogroup manufacturing CP201x Manufacturing Library
///
/// The Silicon Labs CP210x USB-to-UART bridges are devices that communicate over
/// the Universal Serial Bus(USB) to perform Universal Asynchronous Receiver / Transmitter
/// (UART) data transfers. These devices have many programmable options that can
/// be configured via USB. These devices also often include flexible GPIO functions that
/// can be configured and accessed via USB during runtime. Silicon Labs provides libraries
/// that can be used to configure these devices and access their GPIOs.
///
/// Two interface library files can be used to interface with CP210x devices, CP210xManufacturing.DLL and .LIB, which is responsible for reading and writing
/// the device settings, and CP210xRuntime.DLL and .LIB, which is responsible for interfacing with the device's GPIOs. The APIs for CP210xManufacturing are
/// described below.
///
/// The CP210x Manufacturing Host API is provided as a means to facilitate production of customized CP210x devices. This API allows access to the
/// CP210x device for retrieving and setting the VID, PID, product string, serial number, self - power attribute, maximum power consumption,
/// and device version.
///
/// The CP210x Manufacturing Host API is provided in the form of a Windows Dynamic Link Library(DLL), CP210xManufacturing.DLL and as a Windows
/// static link library (.static.LIB).The host interface
/// DLL communicates with the bridge controller device via the provided device driver and the operating system's USB stack.
///
/// In general, the user initiates communication with the target CP210x device by making a call to @ref CP210x_GetNumDevices(). This call
/// returns the number of CP210x target devices. This number is used as a range when calling @ref CP210x_GetProductString() to build a list
/// of devices connected to the host machine.
/// A handle to the device must first be opened by a call to @ref CP210x_Open() using an index determined from the call to @ref CP210x_GetNumDevices().
/// The handle will be used for all subsequent accesses. When I/O operations are complete, the device handle is closed by a call to
/// @ref CP210x_Close().
///
/// @{

#include	<stddef.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdio.h>
#include	<stdint.h>	// For C99 fixed sized types
#include	<stdbool.h>	// For C99 _Bool

#include	"silabs_defs.h"
#include	"silabs_sal.h"

#ifdef _WIN32
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CP210xDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CP210xDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CP210xDLL_EXPORTS
#define CP210xDLL_API
#else
#define CP210xDLL_API __declspec(dllimport)
#pragma comment (lib, "CP210xManufacturing.lib")
#endif
#else // !_WIN32
#define CP210xDLL_API
#define WINAPI
#endif // !_WIN32


/// @ref CP210x_GetProductString() function flags
typedef enum _CP210X_RETURN_GETPRODUCTSTRING {
	CP210x_RETURN_SERIAL_NUMBER = 0x00	///< Serial Number string
	, CP210x_RETURN_DESCRIPTION	= 0x01	///< Description string a la "Friendly Name"
	, CP210x_RETURN_FULL_PATH	= 0x02	///< Full Path string a la "Device Path" "A NULL-terminated string that contains the device interface path. This path can be passed to Win32 functions such as CreateFile(). "
} CP210X_RETURN_GETPRODUCTSTRING, *PCP210X_RETURN_GETPRODUCTSTRING;
_Check_return_
_Success_(return == TRUE)
__inline static bool IsValidSILABS_GETPRODUCTSTRING(_In_ const CP210X_RETURN_GETPRODUCTSTRING _s) { return (CP210x_RETURN_SERIAL_NUMBER == _s) || (CP210x_RETURN_DESCRIPTION == _s) || (CP210x_RETURN_FULL_PATH == _s); }

// CP210x_GetPartNumber() returned PartNums (@deprecated {instead see @ref CP210X_PARTNUM})
#define		CP210x_CP2101_VERSION				CP210x_PARTNUM_CP2101	///< @deprecated use @ref CP210x_PARTNUM_CP2101
#define		CP210x_CP2102_VERSION				CP210x_PARTNUM_CP2102	///< @deprecated use @ref CP210X_PARTNUM
#define		CP210x_CP2103_VERSION				CP210x_PARTNUM_CP2103	///< @deprecated use @ref CP210x_PARTNUM_CP2103
#define		CP210x_CP2104_VERSION				CP210x_PARTNUM_CP2104	///< @deprecated use @ref CP210x_PARTNUM_CP2104
#define		CP210x_CP2105_VERSION				CP210x_PARTNUM_CP2105	///< @deprecated use @ref CP210x_PARTNUM_CP2105
#define		CP210x_CP2108_VERSION				CP210x_PARTNUM_CP2108	///< @deprecated use @ref CP210x_PARTNUM_CP2108
#define     CP210x_CP2109_VERSION               CP210x_PARTNUM_CP2109	///< @deprecated use @ref CP210x_PARTNUM_CP2109
#define     CP210x_CP2102N_QFN28_VERSION		CP210x_PARTNUM_CP2102N_QFN28	///< @deprecated use @ref CP210x_PARTNUM_CP2102N_QFN28
#define     CP210x_CP2102N_QFN24_VERSION		CP210x_PARTNUM_CP2102N_QFN24	///< @deprecated use @ref CP210x_PARTNUM_CP2102N_QFN24
#define     CP210x_CP2102N_QFN20_VERSION		CP210x_PARTNUM_CP2102N_QFN20	///< @deprecated use @ref CP210x_PARTNUM_CP2102N_QFN20

/// API Status return codes
#if defined(NOT_YET)	// TODO: Enstone: These defines are a cut'n'paste dup of the ones in CP210xRuntimeDLL.h.
typedef enum _CP210x_STATUS {
	CP210x_SUCCESS					= SILABS_STATUS_SUCCESS
	, CP210x_INVALID_HANDLE			= 0x01			///< an handle parameter was not valid
	, CP210x_INVALID_PARAMETER		= 0x02			///< a parameter was not valid
	, CP210x_DEVICE_IO_FAILED		= 0x03	///< device I/O failed
	, CP210x_FUNCTION_NOT_SUPPORTED	= 0x04			///< the specified function is not supported
	, CP210x_GLOBAL_DATA_ERROR		= 0x05	///< global data error
	, CP210x_FILE_ERROR				= 0x06	///< file error
	, CP210x_COMMAND_FAILED			= 0x08	///< command failed
	, CP210x_INVALID_ACCESS_TYPE	= 0x09	///< invalid access type

	, CP210x_DEVICE_NOT_FOUND		= 0xFF			///< the specified device was not found
} CP210x_STATUS, *PCP210x_STATUS;
#else // defined(NOT_YET)
/// @defgroup CP210x_STATUS
/// @{
#define	CP210x_SUCCESS SILABS_STATUS_SUCCESS	///< Success
#define	CP210x_INVALID_HANDLE 0x01				///< an handle parameter was not valid
#define	CP210x_INVALID_PARAMETER 0x02			///< a parameter was not valid
#define	CP210x_DEVICE_IO_FAILED 0x03	///< device I/O failed
#define	CP210x_FUNCTION_NOT_SUPPORTED 0x04	///< the specified function is not supported
#define	CP210x_GLOBAL_DATA_ERROR 0x05	///< global data error
#define	CP210x_FILE_ERROR 0x06	///< file error
#define	CP210x_COMMAND_FAILED 0x08	///< command failed
#define	CP210x_INVALID_ACCESS_TYPE 0x09	///< invalid access type
#define	CP210x_DEVICE_NOT_FOUND 0xFF	///< the specified device was not found
typedef     int     CP210x_STATUS;
/// @}
#endif // defined(NOT_YET)

// Buffer size limits
//
//
// CP2101/2/3/4/9
#define		CP210x_MAX_DEVICE_STRLEN			256	///< CP210x Maximum Device String length
#define     CP210x_MAX_MANUFACTURER_STRLEN      45

#define		CP210x_MAX_PRODUCT_STRLEN			126
#define		CP210x_MAX_SERIAL_STRLEN			63
//
// CP2105
#define     CP2105_MAX_MANUFACTURER_STRLEN      12
#define		CP2105_MAX_PRODUCT_STRLEN			47
#define		CP2105_MAX_SERIAL_STRLEN			16
#define		CP2105_MAX_INTERFACE_STRLEN			32
//
// CP2108
#define     CP2108_MAX_MANUFACTURER_STRLEN      126
#define		CP2108_MAX_PRODUCT_STRLEN			126
#define		CP2108_MAX_SERIAL_STRLEN			126
#define		CP2108_MAX_INTERFACE_STRLEN			126
//
// CP2102N
#define		CP2102N_MAX_
// Type definitions
typedef		char	CP210x_DEVICE_STRING[CP210x_MAX_DEVICE_STRLEN];
typedef		char	CP210x_MANUFACTURER_STRING[CP210x_MAX_MANUFACTURER_STRLEN];
typedef		char	CP210x_PRODUCT_STRING[CP210x_MAX_PRODUCT_STRLEN];
typedef		char	CP210x_SERIAL_STRING[CP210x_MAX_SERIAL_STRLEN];
typedef		char	CP2105_INTERFACE_STRING[CP2105_MAX_INTERFACE_STRLEN];
typedef		char	CP2108_INTERFACE_STRING[CP2108_MAX_INTERFACE_STRLEN];

#define		CP210x_MAX_MAXPOWER					250


/// Baud Rate Aliasing definitions
/// @defgroup BaudRateAliasingDefines
/// @{
#define		NUM_BAUD_CONFIGS	32
//
/// Baud Config Structure
typedef struct _BAUD_CONFIG
{
	uint16_t	BaudGen;
	uint16_t	Timer0Reload;
	uint8_t	Prescaler;
	uint32_t	BaudRate;
} BAUD_CONFIG, *PBAUD_CONFIG;
//
/// Size of the Baud Config Structure
#define		BAUD_CONFIG_SIZE	10
//
/// Array of all Baud Rate Configurations
typedef		BAUD_CONFIG		BAUD_CONFIG_DATA[NUM_BAUD_CONFIGS];
/// @}


/// Flush Buffer definitions see @ref CP210x_SetFlushBufferConfig()
/// @defgroup FlushConfigDefines
/// When these bits are set, dictate when the device will flush that buffer (TX and/or RX) and upon which event (Open and/or Close)
/// @{
/// CP2104
#define FC_OPEN_TX		0x01	///< on Open, Tx
#define FC_OPEN_RX		0x02	///< on Open, Rx
#define FC_CLOSE_TX		0x04	///< on Close, Tx
#define FC_CLOSE_RX		0x08	///< on Close, Tx
//
/// CP2105 - Standard Port
#define FC_OPEN_TX_SCI  FC_OPEN_TX	///< Standard port, on Open, Tx
#define FC_OPEN_RX_SCI  FC_OPEN_RX	///< Standard port, on Open, Rx
#define FC_CLOSE_TX_SCI FC_CLOSE_TX	///< Standard port, on Close, Tx
#define FC_CLOSE_RX_SCI FC_CLOSE_RX	///< Standard port, on Close, Rx
//
/// CP2105 - Enhanced Port
#define FC_OPEN_TX_ECI  0x10	///< Enhanced port, on Open, Tx
#define FC_OPEN_RX_ECI  0x20	///< Enhanced port, on Open, Tx
#define FC_CLOSE_TX_ECI 0x40	///< Enhanced port, on Close, Tx
#define FC_CLOSE_RX_ECI 0x80	///< Enhanced port, on Close, Rx
//
/// CP2108
#define FC_OPEN_TX_IFC0  0x0001	///< CP2108 port 0, on Open, Tx
#define FC_OPEN_RX_IFC0  0x0002	///< CP2108 port 0, on Open, Rx
#define FC_CLOSE_TX_IFC0 0x0004	///< CP2108 port 0, on Close, Tx
#define FC_CLOSE_RX_IFC0 0x0008	///< CP2108 port 0, on Close, Rx
#define FC_OPEN_TX_IFC1  0x0010	///< CP2108 port 1, on Open, Tx
#define FC_OPEN_RX_IFC1  0x0020	///< CP2108 port 1, on Open, Rx
#define FC_CLOSE_TX_IFC1 0x0040	///< CP2108 port 1, on Close, Tx
#define FC_CLOSE_RX_IFC1 0x0080	///< CP2108 port 1, on Close, Rx
#define FC_OPEN_TX_IFC2  0x0100	///< CP2108 port 2, on Open, Tx
#define FC_OPEN_RX_IFC2  0x0200	///< CP2108 port 2, on Open, Rx
#define FC_CLOSE_TX_IFC2 0x0400	///< CP2108 port 2, on Close, Tx
#define FC_CLOSE_RX_IFC2 0x0800	///< CP2108 port 2, on Close, Rx
#define FC_OPEN_TX_IFC3  0x1000	///< CP2108 port 3, on Open, Tx
#define FC_OPEN_RX_IFC3  0x2000	///< CP2108 port 3, on Open, Rx
#define FC_CLOSE_TX_IFC3 0x4000	///< CP2108 port 3, on Close, Tx
#define FC_CLOSE_RX_IFC3 0x8000	///< CP2108 port 3, on Close, Rx
/// @}

/// Port Config definitions see @ref tbd()
/// @defgroup PortConfigDefs
/// CP2103/4 Port Config Structure
/// @{
typedef struct _PORT_CONFIG
{
	uint16_t Mode;			// Push-Pull = 1, Open-Drain = 0
	uint16_t Reset_Latch;	// Logic High = 1, Logic Low = =0
	uint16_t Suspend_Latch;	// Logic High = 1, Logic Low = =0
	unsigned char EnhancedFxn;
} PORT_CONFIG, *PPORT_CONFIG;
//
// Define bit locations for Mode/Latch for Reset and Suspend structures
#define PORT_RI_ON				0x0001
#define PORT_DCD_ON				0x0002
#define PORT_DTR_ON				0x0004
#define PORT_DSR_ON				0x0008
#define PORT_TXD_ON				0x0010
#define PORT_RXD_ON				0x0020
#define PORT_RTS_ON				0x0040
#define PORT_CTS_ON				0x0080
//
#define PORT_GPIO_0_ON			0x0100
#define PORT_GPIO_1_ON			0x0200
#define PORT_GPIO_2_ON			0x0400
#define PORT_GPIO_3_ON			0x0800
//
#define PORT_SUSPEND_ON			0x4000	//  Can't configure latch value
#define PORT_SUSPEND_BAR_ON		0x8000	//  Can't configure latch value
//
// Define bit locations for EnhancedFxn
#define EF_GPIO_0_TXLED				0x01	//  Under device control
#define EF_GPIO_1_RXLED				0x02	//  Under device control
#define EF_GPIO_2_RS485				0x04	//  Under device control
#define EF_RS485_INVERT				0x08	//  RS485 Invert bit
#define EF_WEAKPULLUP				0x10	//  Weak Pull-up on
#define EF_RESERVED_1				0x20	//	Reserved, leave bit 5 cleared
#define EF_SERIAL_DYNAMIC_SUSPEND	0x40	//  For 8 UART/Modem signals
#define EF_GPIO_DYNAMIC_SUSPEND		0x80	//  For 4 GPIO signals
/// @}


/// Dual Port Config definitions see @ref tbd()
/// @defgroup DualPortConfigDefs
/// CP2105 Dual Port Config Structure
/// @{
typedef struct _DUAL_PORT_CONFIG
{
	uint16_t Mode;			// Push-Pull = 1, Open-Drain = 0
	uint16_t Reset_Latch;	// Logic High = 1, Logic Low = =0
	uint16_t Suspend_Latch;	// Logic High = 1, Logic Low = =0
	unsigned char EnhancedFxn_ECI;
	unsigned char EnhancedFxn_SCI;
	unsigned char EnhancedFxn_Device;
} DUAL_PORT_CONFIG, *PDUAL_PORT_CONFIG;
//
// CP2105 Define bit locations for Mode/Latch for Reset and Suspend structures
#define PORT_RI_SCI_ON			0x0001
#define PORT_DCD_SCI_ON			0x0002
#define PORT_DTR_SCI_ON			0x0004
#define PORT_DSR_SCI_ON			0x0008
#define PORT_TXD_SCI_ON			0x0010
#define PORT_RXD_SCI_ON			0x0020
#define PORT_RTS_SCI_ON			0x0040
#define PORT_CTS_SCI_ON			0x0080
#define PORT_GPIO_0_SCI_ON		0x0002
#define PORT_GPIO_1_SCI_ON		0x0004
#define PORT_GPIO_2_SCI_ON		0x0008
#define PORT_SUSPEND_SCI_ON		0x0001	//  Can't configure latch value
//
#define PORT_RI_ECI_ON			0x0100
#define PORT_DCD_ECI_ON			0x0200
#define PORT_DTR_ECI_ON			0x0400
#define PORT_DSR_ECI_ON			0x0800
#define PORT_TXD_ECI_ON			0x1000
#define PORT_RXD_ECI_ON			0x2000
#define PORT_RTS_ECI_ON			0x4000
#define PORT_CTS_ECI_ON			0x8000
#define PORT_GPIO_0_ECI_ON		0x0400
#define PORT_GPIO_1_ECI_ON		0x0800
#define PORT_SUSPEND_ECI_ON		0x0100	//  Can't configure latch value
//
// CP2105 Define bit locations for EnhancedFxn_ECI
#define EF_GPIO_0_TXLED_ECI			0x01	//  Under device control
#define EF_GPIO_1_RXLED_ECI			0x02	//  Under device control
#define EF_GPIO_1_RS485_ECI			0x04	//  Under device control
#define EF_RS485_INVERT				0x08	//  Under device control
#define EF_INVERT_SUSPEND_ECI		0x10	//  RS485 Invert bit
#define EF_DYNAMIC_SUSPEND_ECI		0x40	//  For GPIO signals
//
// CP2105 Define bit locations for EnhancedFxn_SCI
#define EF_GPIO_0_TXLED_SCI			0x01	//  Under device control
#define EF_GPIO_1_RXLED_SCI			0x02	//  Under device control
#define EF_INVERT_SUSPEND_SCI		0x10	//  RS485 Invert bit
#define EF_DYNAMIC_SUSPEND_SCI		0x40	//  For GPIO signals
//
// CP2105 Define bit locations for EnhancedFxn_Device
#define EF_WEAKPULLUP				0x10	//  Weak Pull-up on
/// @}


/// Quad Port Config definitions see @ref tbd()
/// @defgroup QuadPortConfigDefs
/// CP2108 Quad Port State structure (used in Quad Port Config structure)
/// @{
typedef struct _QUAD_PORT_STATE
{
    uint16_t Mode_PB0;
    uint16_t Mode_PB1;
    uint16_t Mode_PB2;
    uint16_t Mode_PB3;
    uint16_t Mode_PB4;

    uint16_t LowPower_PB0;
    uint16_t LowPower_PB1;
    uint16_t LowPower_PB2;
    uint16_t LowPower_PB3;
    uint16_t LowPower_PB4;

    uint16_t Latch_PB0;
    uint16_t Latch_PB1;
    uint16_t Latch_PB2;
    uint16_t Latch_PB3;
    uint16_t Latch_PB4;
} QUAD_PORT_STATE, *PQUAD_PORT_STATE;

/// CP2108 Quad Port Config structure
typedef struct _QUAD_PORT_CONFIG
{
	QUAD_PORT_STATE Reset_Latch;
	QUAD_PORT_STATE Suspend_Latch;
    uint8_t IPDelay_IFC0;
    uint8_t IPDelay_IFC1;
    uint8_t IPDelay_IFC2;
    uint8_t IPDelay_IFC3;
    uint8_t EnhancedFxn_IFC0;
    uint8_t EnhancedFxn_IFC1;
    uint8_t EnhancedFxn_IFC2;
    uint8_t EnhancedFxn_IFC3;
    uint8_t EnhancedFxn_Device;
    uint8_t ExtClk0Freq;
    uint8_t ExtClk1Freq;
    uint8_t ExtClk2Freq;
    uint8_t ExtClk3Freq;
} QUAD_PORT_CONFIG, *PQUAD_PORT_CONFIG;

//
// CP2108 Define bit locations for Mode/Latch for Reset and Suspend structures
// PB0
#define  PORT_TX0       0x0001
#define  PORT_RX0       0x0002
#define  PORT_RTS0      0x0004
#define  PORT_CTS0      0x0008
#define  PORT_DTR0      0x0010
#define  PORT_DSR0      0x0020
#define  PORT_DCD0      0x0040
#define  PORT_RI0       0x0080
#define  PORT_TX1       0x0100
#define  PORT_RX1       0x0200
#define  PORT_RTS1      0x0400
#define  PORT_CTS1      0x0800
#define  PORT_DTR1      0x1000
#define  PORT_DSR1      0x2000
#define  PORT_DCD1      0x4000
#define  PORT_RI1       0x8000

// PB1
#define  PORT_GPIO_0    0x0001	// (1<<0)
#define  PORT_GPIO_1    0x0002	// (1<<1)
#define  PORT_GPIO_2    0x0004	// (1<<2)
#define  PORT_GPIO_3    0x0008	// etc.
#define  PORT_GPIO_4    0x0010
#define  PORT_GPIO_5    0x0020
#define  PORT_GPIO_6    0x0040
#define  PORT_GPIO_7    0x0080
#define  PORT_GPIO_8    0x0100
#define  PORT_GPIO_9    0x0200
#define  PORT_GPIO_10   0x0400
#define  PORT_GPIO_11   0x0800
#define  PORT_GPIO_12   0x1000
#define  PORT_GPIO_13   0x2000
#define  PORT_GPIO_14   0x4000
#define  PORT_GPIO_15   0x8000

// PB2
#define  PORT_SUSPEND   0x0001
#define  PORT_SUSPEND_BAR   0x0002
#define  PORT_DTR2      0x0004
#define  PORT_DSR2      0x0008

// PB3
#define  PORT_TX2       0x0001
#define  PORT_RX2       0x0002
#define  PORT_RTS2      0x0004
#define  PORT_CTS2      0x0008
#define  PORT_DCD2      0x0010
#define  PORT_RI2       0x0020
#define  PORT_DTR3      0x0040
#define  PORT_DSR3      0x0080
#define  PORT_DCD3      0x0100
#define  PORT_RI3       0x0200

// PB4
#define  PORT_RTS3      0x0001
#define  PORT_CTS3      0x0002
#define  PORT_TX3       0x0004
#define  PORT_RX3       0x0008

//
// CP2108 Define bit locations for EnhancedFxn_IFCx
#define EF_IFC_GPIO_TXLED   0x01
#define EF_IFC_GPIO_RXLED   0x02
#define EF_IFC_GPIO_RS485   0x04
// If the next bit is clear, GPIO1 is low while sending UART data.
// If it is set, GPIO1 is high while sending UART data, and low otherwise
#define EF_IFC_GPIO_RS485_LOGIC 0x08
#define EF_IFC_GPIO_CLOCK       0x10
#define EF_IFC_DYNAMIC_SUSPEND  0x40

//
// CP2108 Define bit locations for EnhancedFxn_Device
#define EF_DEVICE_WEAKPULLUP_RESET      0x10
#define EF_DEVICE_WEAKPULLUP_SUSPEND    0x20
#define EF_DEVICE_DYNAMIC_SUSPEND       0x40
/// @}


typedef unsigned char uint8_t;
/// Firmware version structure.
/// Not writeable by OEMs or end-users.
/// Automatically populated by build infrastructure
typedef struct _firmware_t  {
	uint8_t major; ///< major version number
	uint8_t minor; ///< minor version number
	uint8_t build; ///< build number
} firmware_t, *pFirmware_t;

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Determines the number of CP210x devices connected to the system
///
/// @param[out] lpdwNumDevices a pointer to a uint32_t location to hold the returned device count
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_PARAMETER -- lpdwNumDevices is an unexpected value
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetNumDevices(
	_Out_writes_bytes_(sizeof(uint32_t)) _Pre_defensive_ uint32_t *lpdwNumDevices
	);

/// @brief Gets a "Product String"
///
/// @detail This function returns a NULL-terminated serial number string, product description string, or full
/// path string for the device specified by an index passed in the DeviceNum parameter. The index of the
/// first device is 0, and the index of the last device is the value(NumDevices) returned by @ref CP210x_GetNumDevices() - 1.
///
/// The @ref CP210x_RETURN_FULL_PATH "full path" string is the "device interface path" or "Device Path". This path can be passed to Win32 functions such as CreateFile().
///
/// The @ref CP210x_RETURN_SERIAL_NUMBER "Serial number" string is derived from the "full path" string.
///
/// The @ref CP210x_RETURN_DESCRIPTION "description" string is either the "description of a device" Plug and Play device property from the Registry, if available, else the
/// "friendly name of a device" Plug and Play device property from the Registry, if available,
/// else the same "device interface path" returned for the "full path" string.
///
/// Returned strings are NULL-terminated.
///
/// @note: This function may return cached data, or data from the device driver. To access the data from
/// the device directly, please use @ref CP210x_GetDeviceProductString().
///
/// @note: This is an string-UNSAFE function. lpvDeviceString must point to a buffer at least @ref CP210x_MAX_DEVICE_STRLEN bytes
///
/// @param[in] dwDeviceIndex is the desired device's index into the device list used to determine @ref CP210x_GetNumDevices()
/// @param[out] lpvDeviceString points at a buffer into which the "Product String" will be copied and returned
/// @param[in] dwFlags is the indication of which "Product String" to return @ref CP210X_RETURN_GETPRODUCTSTRING
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_PARAMETER -- lpvDeviceString or dwFlags is an unexpected value
///	@retval	#CP210x_DEVICE_NOT_FOUND -- the device specified by the dwDeviceIndex was not found
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetProductString(
	_In_ _Pre_defensive_ const uint32_t dwDeviceIndex,
	void * lpvDeviceString,
	_In_ _Pre_defensive_ const uint32_t dwFlags
	);

/// @brief Opens a handle to the device
///
/// @detail Opens and returns a handle to a device using a device index determined by the number returned
/// from @ref CP210x_GetNumDevices().
///
/// @param[in] dwDeviceIndex is the desired device's index into the device list used to determine @ref CP210x_GetNumDevices()
/// @param[out] pcyHandle is a pointer to a HANDLE/4-byte location to hold the returned open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_PARAMETER -- pcyHandle is an unexpected value
///	@retval	#CP210x_DEVICE_NOT_FOUND -- the device specified by the dwDeviceIndex was not found
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_Open(
	_In_ _Pre_defensive_ const uint32_t DeviceIndex,
	_Out_writes_bytes_(sizeof(HANDLE)) _Pre_defensive_ PHANDLE pcyHandle
	); 

/// @brief Closes an open handle to the device
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_Close(
	_In_ _Pre_defensive_ const HANDLE cyHandle
	);

/// @brief Reads and returns the Part Number from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpbPartNum points at a 1-byte buffer into which the @ref CP210X_PARTNUM Part Number value will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpbPartNum is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetPartNumber(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(uint8_t)) _Pre_defensive_ uint8_t *lpbPartNum
	);

/// @brief Sets the Vendor ID field of the device's USB Device Descriptor.
///
/// @detail
///
/// @param[in] cyHandle is an open handle to the device.
/// @param[in] Vid is the 2-byte Vendor ID value.
///
/// @note @ref IsOTP() OTP parts can not often/indefinitely have configuration data (re-)written (I.e. Set) to them
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetVid(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ const uint16_t	Vid
	);

/// @brief Sets the Product ID field of the device's USB Device Descriptor.
///
/// @detail
///
/// @param[in] cyHandle is an open handle to the device.
/// @param[in] Pid is the 2-byte Product ID value.
///
/// @note @ref IsOTP() OTP parts can not often/indefinitely have configuration data (re-)written (I.e. Set) to them
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetPid(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ const uint16_t	Pid
	);

/// @brief Sets the Manufacturer Description String of the device's USB Device Descriptor's iManufacturer USB String Descriptor
///
/// @detail If the string is not already in Unicode format, the function will convert the string to Unicode before committing it to
/// programmable memory. The character size limit (in characters, not bytes), NOT including a NULL terminator,
/// is @ref SILABS_USB_STRING_DESCRIPTOR_MANUFACTURER_STRING_LENGTH_IN_CHARS_MAX for the device type.
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] lpvManufacturerString is a pointer to a buffer containing the Manufacturer String value
/// @param[in] bManufacturerStringLength is the length of the string in characters NOT including a NULL terminator.
/// @param[in] bConvertToUnicode is a flag indicating whether the string is ASCII and needs converting to UNICODE (TRUE), else FALSE.
///
/// @note @ref IsOTP() OTP parts can not often/indefinitely have configuration data (re-)written (I.e. Set) to them
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpvManufacturer or bManufacturerStringLength is an unexpected value
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetManufacturerString(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ void *lpvManufacturerString,
	_In_ _Pre_defensive_ const uint8_t bManufacturerStringLength,
	_In_ _Pre_defensive_ const BOOL bConvertToUnicode = TRUE
	);

/// @brief Sets the Product Description String of the device's USB Device Descriptor's iProduct USB String Descriptor
///
/// @detail If the string is not already in Unicode format, the function will convert the string to Unicode before committing it to
/// programmable memory. The character size limit(in characters, not bytes), NOT including a NULL terminator,
/// is @ref CP210x_MAX_PRODUCT_STRLEN or @ref CP2105_MAX_PRODUCT_STRLEN.
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] lpvProductString is a pointer to a buffer containing the Product String value
/// @param[in] bProductStringLength is the length of the string in characters NOT including a NULL terminator.
/// @param[in] bConvertToUnicode is a flag indicating whether the string is ASCII and needs converting to UNICODE (TRUE), else FALSE.
///
/// @note @ref IsOTP() OTP parts can not often/indefinitely have configuration data (re-)written (I.e. Set) to them
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpvProductString or bProductStringLength is an unexpected value
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetProductString(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ void * lpvProductString,
	_In_ _Pre_defensive_ const uint8_t bProductStringLength,
	_In_ _Pre_defensive_ const BOOL bConvertToUnicode = TRUE
	);

/// @brief Sets the Interface Description String of the device's specified interface's iInterface USB String Descriptor
///
/// @detail If the string is not already in Unicode format, the function will convert the string to Unicode before committing it to
/// programmable memory. The character size limit(in characters, not bytes), NOT including a NULL terminator,
/// is @ref CP210x_MAX_PRODUCT_STRLEN or @ref CP2105_MAX_PRODUCT_STRLEN.
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] bInterfaceNumber is Set to 0 for Enhanced Interface String, or 1 for Standard Interface String on the CP2105. 0 - 3 for the CP2108 which has 4 interfaces.
/// @param[in] lpvInterfaceString is a pointer to a buffer containing the Interface String value
/// @param[in] bInterfaceStringLength is the length of the string in characters NOT including a NULL terminator.
/// @param[in] bConvertToUnicode is a flag indicating whether the string is ASCII and needs converting to UNICODE (TRUE), else FALSE.
///
/// @note supported on the @ref IsMultiInterfacePartNum() CP2105 and CP2108 multi-interface CP210x devices.
///
/// @note @ref IsOTP() OTP parts can not often/indefinitely have configuration data (re-)written (I.e. Set) to them
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpvProductString or bInterfaceStringLength is an unexpected value
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- Only the CP2105 and CP2108 have (multiple) interfaces and thus interface strings
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetInterfaceString(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ const uint8_t bInterfaceNumber,
	_In_ _Pre_defensive_ void * lpvInterfaceString,
	_In_ _Pre_defensive_ const uint8_t bInterfaceStringLength,
	_In_ _Pre_defensive_ const BOOL bConvertToUnicode
	);

/// @brief Sets the Serial Number Description String of the device's USB Device Descriptor's iSerialNumber USB String Descriptor
///
/// @detail If the string is not already in Unicode format, the function will convert the string to Unicode before committing it to
/// programmable memory. The character size limit(in characters, not bytes), NOT including a NULL terminator,
/// is @ref SILABS_USB_STRING_DESCRIPTOR_SERIAL_STRING_LENGTH_IN_CHARS_MAX for the device type.
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] lpvSerialNumberString is a pointer to a buffer containing the Serial Number String value
/// @param[in] bSerialNumberStringLength is the length of the string in characters NOT including a NULL terminator.
/// @param[in] bConvertToUnicode is a flag indicating whether the string is ASCII and needs converting to UNICODE (TRUE), else FALSE.
///
/// @note @ref IsOTP() OTP parts can not often/indefinitely have configuration data (re-)written (I.e. Set) to them
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpvSerialNumberString or bSerialNumberStringLength is an unexpected value
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support setting of the Serial Number string
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetSerialNumber(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ void *lpvSerialNumberString,
	_In_ _Pre_defensive_ const uint8_t bSerialNumberStringLength,
	_In_ _Pre_defensive_ const BOOL bConvertToUnicode = TRUE
	);

/// @brief Sets or clears the Self-Powered bit of the Power Attributes field of the USB Configuration Descriptor on the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] bSelfPower is a Boolean flag where TRUE means set the Self-Powered bit, and FALSE means clear the Self - Powered bit.
///
/// @note @ref IsOTP() OTP parts can not often/indefinitely have configuration data (re-)written (I.e. Set) to them
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetSelfPower(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ const BOOL bSelfPower
	);

/// @brief Sets the Max Power field of the USB Configuration Descriptor of the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] bMaxPower is a 1-byte value representing the maximum power consumption of the CP210x USB device, expressed in 2 mA units.
///
/// @note @ref IsOTP() OTP parts can not often/indefinitely have configuration data (re-)written (I.e. Set) to them
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetMaxPower(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ const uint8_t bMaxPower
	);

/// @brief Sets the Flush Buffer configuration of the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] wFlushBufferConfig is a 2-byte bitmask Set to determine which buffer/s to flush (TX and/or RX) and upon which event (Open and/or Close).
///
/// @note wFlushBufferConfig is a "pass through" value directly to the device, see @ref FlushConfigDefines
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetFlushBufferConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ const uint16_t wFlushBufferConfig
	);

/// @brief Sets the operating mode (GPIO or Modem) of each Interface of the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] bDeviceModeECI Set to 0 for modem mode for Enhanced interface
/// @param[in] bDeviceModeSCI Set to 0 for modem mode for Standard interface
///
/// @note supported on the CP2105 CP210x devices.
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support setting of the Device Mode
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetDeviceMode(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ const uint8_t bDeviceModeECI,
	_In_ _Pre_defensive_ const uint8_t bDeviceModeSCI
	);

/// @brief Sets the Device Release Version field of the USB Device Descriptor of the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] wVersion is the 2-byte Device Release Version number in Binary-Coded Decimal (BCD) format with
/// the upper two nibbles containing the two decimal digits of the major version and the lower two
/// nibbles containing the two decimal digits of the minor version.
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetDeviceVersion(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ const uint16_t wVersion
	);

/// @brief Sets the baud rate configuration data of the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] baudConfigData is a pointer to a @ref BAUD_CONFIG structure containing the Baud Config data to be set on the device.
///
/// @note supported on the CP2102, CP2103 and CP2109 CP210x devices.
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support setting of the Baud Rate Configuration
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetBaudRateConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_reads_bytes_(sizeof(BAUD_CONFIG)) _Pre_defensive_ PBAUD_CONFIG baudConfigData
	);

/// @brief Sets the current port pin configuration from the device.
///
/// @note supported on the CP2103 and CP2104 CP210x devices.
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] PortConfig is a pointer to a @ref PORT_CONFIG structure
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support setting of the Baud Rate Configuration
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetPortConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ PORT_CONFIG *PortConfig
	);

/// @brief Sets the current port pin configuration from the device.
///
/// @note @ref CP210x_SetDeviceMode() must be called before calling this function.
///
/// @note supported on the CP2105 multi-interface CP210x devices.
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] DualPortConfig is a pointer to a @ref DUAL_PORT_CONFIG structure
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support setting of the Baud Rate Configuration
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetDualPortConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ DUAL_PORT_CONFIG *DualPortConfig
	);

/// @brief Sets the current port pin configuration from the device.
///
/// @note supported on the CP2108 multi-interface CP210x devices.
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] QuadPortConfig is a pointer to a @ref QUAD_PORT_CONFIG structure
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support setting of the Baud Rate Configuration
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetQuadPortConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ QUAD_PORT_CONFIG *QuadPortConfig
	);

/// @brief Sets the 1-byte Lock Value of a device.
///
/// @Note: Setting the lock value locks ALL customizable data and cannot be reset; only use this function
/// to keep all customizable data on the part permanently.
///
/// @note supported on the CP2102, CP2103, CP2104, CP2105, CP2108 and CP2109 CP210x devices.
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support setting of the Baud Rate Configuration
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetLockValue(
	_In_ _Pre_defensive_ const HANDLE cyHandle
	);

/// @brief Reads and returns the VID from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpwVid points at a buffer into which the VID will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpwVid is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetDeviceVid(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(uint16_t)) _Pre_defensive_ uint16_t *lpwVid
	);

/// @brief Reads and returns the PID from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpwPid points at a buffer into which the PID will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpwPid is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetDevicePid(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(uint16_t)) _Pre_defensive_ uint16_t *lpwPid
	);

/// @brief TBD
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetDeviceManufacturerString(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	void *	lpManufacturer,
	uint8_t	*lpbLength,
	_In_ const BOOL	bConvertToASCII = TRUE
	);

/// @brief TBD
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetDeviceProductString(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	void *	lpProduct,
	uint8_t *lpbLength,
	_In_ _Pre_defensive_ const BOOL	bConvertToASCII = TRUE
	);

/// @brief TBD
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetDeviceInterfaceString(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ const uint8_t bInterfaceNumber,
	void * lpInterface,
	uint8_t *lpbLength,
	_In_ _Pre_defensive_ const BOOL bConvertToASCII
	);

/// @brief TBD
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetDeviceSerialNumber(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	void *	lpSerialNumber,
	uint8_t *lpbLength,
	_In_ _Pre_defensive_ const BOOL	bConvertToASCII = TRUE
	);


/// @brief Returns the USB Address of the device.
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpbAddress points to a byte where the address will be written.
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetDeviceAddress(
  _In_ _Pre_defensive_ const HANDLE cyHandle,
	uint8_t *lpbAddress);



/// @brief Reads and returns the Power attributes from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpbSelfPower points to a buffer into which the Power attributes will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpbSelfPower is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetSelfPower(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(BOOL)) _Pre_defensive_ LPBOOL lpbSelfPower
	);

/// @brief Reads and returns the Maximum power from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpbPower points to a buffer into which the Maximum power will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpbPower is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetMaxPower(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(uint8_t)) _Pre_defensive_ uint8_t *lpbPower
	);

/// @brief Reads and returns the flush buffer behavior from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] lpwFlushBufferConfig points to a buffer into which the flush buffer behavior will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpwFlushBufferConfig is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetFlushBufferConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(uint16_t)) _Pre_defensive_ uint16_t *lpwFlushBufferConfig
	);

/// @brief Reads and returns the device mode from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpbDeviceModeECI points to a buffer into which the ECI will be written
/// @param[out] lpbDeviceModeSCI points to a buffer into which the SCI will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpbDeviceModeECI or lpbDeviceModeSCI are unexpected values
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support device mode
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetDeviceMode(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(uint8_t)) _Pre_defensive_ uint8_t *lpbDeviceModeECI,
	_Out_writes_bytes_(sizeof(uint8_t)) _Pre_defensive_ uint8_t *lpbDeviceModeSCI
	);

/// @brief Reads and returns the Version (Device from the USB Standard Device Descriptor) from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpwVersion points to a buffer into which the Version will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpwVersion is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetDeviceVersion(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(uint16_t)) _Pre_defensive_ uint16_t *lpwVersion
	);

/// @brief Reads and returns the Baud Rate Configuration from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] pBaudConfig points to a buffer into which the Baud Rate Configuration will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- pBaudConfig is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetBaudRateConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(BAUD_CONFIG)) _Pre_defensive_ PBAUD_CONFIG pBaudConfig
	);

/// @brief Reads and returns the Port Configuration from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] pPortConfig points to a buffer into which the Port Configuration will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- pPortConfig is an unexpected value
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support Port Configuration
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetPortConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(PORT_CONFIG)) _Pre_defensive_ PPORT_CONFIG pPortConfig
	);

/// @brief Reads and returns the Dual Port Configuration from the Dual Port device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] pDualPortConfig points to a buffer into which the Dual Port Configuration will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- pDualPortConfig is an unexpected value
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support Dual Port Configuration
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetDualPortConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(DUAL_PORT_CONFIG)) _Pre_defensive_ DUAL_PORT_CONFIG* pDualPortConfig
	);

/// @brief Reads and returns the Quad Port Configuration from the Quad Port device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] pQuadPortConfig points to a buffer into which the Quad Port Configuration will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- pQuadPortConfig is an unexpected value
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support Quad Port Configuration
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetQuadPortConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(QUAD_PORT_CONFIG)) _Pre_defensive_ QUAD_PORT_CONFIG* pQuadPortConfig
	);

/// @brief Reads and returns the Lock Value from the  device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpbLockValue points to a buffer into which the Quad Port Configuration will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpbLockValue is an unexpected value
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support Lock Value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetLockValue(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(uint8_t)) _Pre_defensive_ uint8_t *lpbLockValue
	);

/// @brief tbd
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_Reset(
	_In_ _Pre_defensive_ const HANDLE cyHandle
	);

/// @brief tbd
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_CreateHexFile(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ LPCSTR lpvFileName
	);

/// @brief Reads and returns the Firmware Version from the  device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpVersion points to a buffer into which the Firmware Version will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpVersion is an unexpected value
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support reporting its Firmware Version
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetFirmwareVersion(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(sizeof(firmware_t)) _Pre_defensive_ pFirmware_t lpVersion
	);

/// @brief tbd
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_SUCCESS -- success
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(bLength) _Pre_defensive_ uint8_t *lpbConfig,
	_In_ _Pre_defensive_ const uint16_t	bLength
);

/// @brief tbd
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetConfig(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_reads_bytes_(bLength) _Pre_defensive_ uint8_t *lpbConfig,
	_In_ _Pre_defensive_ const uint16_t	bLength
	);

/// @brief tbd
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_UpdateFirmware(
	_In_ _Pre_defensive_ const HANDLE cyHandle);

/// @brief tbd
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#TBD -- tbd
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_GetGeneric(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(bLength) _Pre_defensive_ uint8_t *lpbGeneric,
	_In_ _Pre_defensive_ const uint16_t	bLength
	);

/// @brief tbd
///
/// @param[in] cyHandle is an open handle to the device
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref CP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_SUCCESS -- success
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xDLL_API CP210x_STATUS WINAPI
CP210x_SetGeneric(
	_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_reads_bytes_(bLength) _Pre_defensive_ uint8_t *lpbGeneric,
	_In_ _Pre_defensive_ const uint16_t bLength
	);

#ifdef __cplusplus
}
#endif

/// @}

#endif	// !HOST_LIB_MANUFACTURING_INCLUDE_CP210XMANUFACTURINGDLL_H_INCLUDED_XF3N9QM3BK
