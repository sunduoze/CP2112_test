/////////////////////////////////////////////////////////////////////////////
// SLABCP2112.h
// For SLABHIDtoSMBus.dll
// and Silicon Labs CP2112 HID to SMBus
/////////////////////////////////////////////////////////////////////////////
#ifndef HOST_LIB_SLABHIDSMBUS_INCLUDE_SLABCP2112_H_INCLUDED_QU4N7WC20W
#define HOST_LIB_SLABHIDSMBUS_INCLUDE_SLABCP2112_H_INCLUDED_QU4N7WC20W

/// @addtogroup slabhidsmbus CP2112 SMBus over HID interface Library
///
/// @{
/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Types.h"
#include	"silabs_defs.h"
#include	"silabs_sal.h"

/////////////////////////////////////////////////////////////////////////////
// Tool-chain-dependent hacks
/////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HID_TO_SMBUS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HID_TO_SMBUS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#if defined(HID_TO_SMBUS_EXPORTS)
#define HID_TO_SMBUS_API
#else
#define HID_TO_SMBUS_API __declspec(dllimport)
#pragma comment(lib, "SLABHIDtoSMBus.lib")
#endif // defined(HID_TO_SMBUS_EXPORTS)
#else // !_WIN32
#define HID_TO_SMBUS_API
#define WINAPI
#endif // !_WIN32


/////////////////////////////////////////////////////////////////////////////
// Return Code Definitions
/////////////////////////////////////////////////////////////////////////////

// HID_SMBUS_STATUS
#if defined(NOT_YET)
typedef enum _HID_SMBUS_STATUS {
	HID_SMBUS_SUCCESS = SILABS_STATUS_SUCCESS			///< @ref SILABS_STATUS no error
	, HID_SMBUS_DEVICE_NOT_FOUND = 0x01					///< the specified device was not found
	, HID_SMBUS_INVALID_HANDLE = 0x02					///< an handle parameter was not valid
	, HID_SMBUS_INVALID_DEVICE_OBJECT = 0x03
	, HID_SMBUS_INVALID_PARAMETER = 0x04				///< a parameter was not valid
	, HID_SMBUS_INVALID_REQUEST_LENGTH = 0x05

	, HID_SMBUS_DEVICE_ALREADY_OPENED = SILABS_STATUS_DEVICE_ALREADY_OPENED	///< the specified device is already (exclusively) opened

	, HID_SMBUS_READ_ERROR = 0x10
	, HID_SMBUS_WRITE_ERROR = 0x11
	, HID_SMBUS_READ_TIMED_OUT = 0x12
	, HID_SMBUS_WRITE_TIMED_OUT = 0x13
	, HID_SMBUS_DEVICE_IO_FAILED = 0x14
	, HID_SMBUS_DEVICE_ACCESS_ERROR = 0x15
	, HID_SMBUS_DEVICE_NOT_SUPPORTED = 0x16

	, HID_SMBUS_UNKNOWN_ERROR = SILABS_STATUS_UNKNOWN_ERROR	///< an otherwise unclassified error occurred
} HID_SMBUS_STATUS, *PHID_SMBUS_STATUS;
#else // defined(NOT_YET)
typedef int HID_SMBUS_STATUS;

// HID_SMBUS_STATUS Return Codes
#define HID_SMBUS_SUCCESS							SILABS_STATUS_SUCCESS
#define	HID_SMBUS_DEVICE_NOT_FOUND					0x01
#define HID_SMBUS_INVALID_HANDLE					0x02
#define	HID_SMBUS_INVALID_DEVICE_OBJECT				0x03
#define	HID_SMBUS_INVALID_PARAMETER					0x04
#define	HID_SMBUS_INVALID_REQUEST_LENGTH			0x05

#define	HID_SMBUS_DEVICE_ALREADY_OPENED				SILABS_STATUS_DEVICE_ALREADY_OPENED	///< the specified device is already (exclusively) opened

#define	HID_SMBUS_READ_ERROR						0x10
#define	HID_SMBUS_WRITE_ERROR						0x11
#define	HID_SMBUS_READ_TIMED_OUT					0x12
#define	HID_SMBUS_WRITE_TIMED_OUT					0x13
#define	HID_SMBUS_DEVICE_IO_FAILED					0x14
#define HID_SMBUS_DEVICE_ACCESS_ERROR				0x15
#define HID_SMBUS_DEVICE_NOT_SUPPORTED				0x16

#define HID_SMBUS_UNKNOWN_ERROR						SILABS_STATUS_UNKNOWN_ERROR
#endif // defined(NOT_YET)

// HID_SMBUS_TRANSFER_S0
typedef BYTE HID_SMBUS_S0;

#define HID_SMBUS_S0_IDLE							0x00
#define HID_SMBUS_S0_BUSY							0x01
#define HID_SMBUS_S0_COMPLETE						0x02
#define HID_SMBUS_S0_ERROR							0x03

// HID_SMBUS_TRANSFER_S1
typedef BYTE HID_SMBUS_S1;

// HID_SMBUS_TRANSFER_S0 = HID_SMBUS_S0_BUSY
#define HID_SMBUS_S1_BUSY_ADDRESS_ACKED				0x00
#define HID_SMBUS_S1_BUSY_ADDRESS_NACKED			0x01
#define HID_SMBUS_S1_BUSY_READING					0x02
#define HID_SMBUS_S1_BUSY_WRITING					0x03

// HID_SMBUS_TRANSFER_S0 = HID_SMBUS_S0_ERROR
#define HID_SMBUS_S1_ERROR_TIMEOUT_NACK				0x00
#define HID_SMBUS_S1_ERROR_TIMEOUT_BUS_NOT_FREE		0x01
#define HID_SMBUS_S1_ERROR_ARB_LOST					0x02
#define HID_SMBUS_S1_ERROR_READ_INCOMPLETE			0x03
#define HID_SMBUS_S1_ERROR_WRITE_INCOMPLETE			0x04
#define HID_SMBUS_S1_ERROR_SUCCESS_AFTER_RETRY		0x05

/////////////////////////////////////////////////////////////////////////////
// String Definitions
/////////////////////////////////////////////////////////////////////////////

// Product String Types
/// @ref HidSmbus_GetString() and @ref HidSmbus_GetOpenedString() options values
typedef enum _HID_SMBUS_GETSTRING {
	HID_SMBUS_GET_VID_STR = 0x01	///< Vendor ID string
	, HID_SMBUS_GET_PID_STR = 0x02	///< Product ID string
	, HID_SMBUS_GET_PATH_STR = 0x03	///< Path string
	, HID_SMBUS_GET_SERIAL_STR = 0x04	///< Serial Number string
	, HID_SMBUS_GET_MANUFACTURER_STR = 0x05	///< Manufacturer string
	, HID_SMBUS_GET_PRODUCT_STR = 0x06	///< Product string
} HID_SMBUS_GETSTRING, *PHID_SMBUS_GETSTRING;
_Check_return_
_Success_(return == TRUE)
__inline static bool IsValidHID_SMBUS_GETSTRING(_In_ const HID_SMBUS_GETSTRING _s) { return (HID_SMBUS_GET_VID_STR == _s) || (HID_SMBUS_GET_PID_STR == _s) || (HID_SMBUS_GET_PATH_STR == _s) || (HID_SMBUS_GET_SERIAL_STR == _s) || (HID_SMBUS_GET_MANUFACTURER_STR == _s) || (HID_SMBUS_GET_PRODUCT_STR == _s); }

// String Lengths
#define HID_SMBUS_DEVICE_STRLEN						260

// HID_SMBUS_DEVICE_STR
typedef char HID_SMBUS_DEVICE_STR[HID_SMBUS_DEVICE_STRLEN];

/////////////////////////////////////////////////////////////////////////////
// SMBUS Definitions
/////////////////////////////////////////////////////////////////////////////

// SMbus Configuration Limits
#define HID_SMBUS_MIN_BIT_RATE						1
#define HID_SMBUS_MIN_TIMEOUT						0
#define HID_SMBUS_MAX_TIMEOUT						1000
#define HID_SMBUS_MAX_RETRIES						1000
#define HID_SMBUS_MIN_ADDRESS						0x02
#define HID_SMBUS_MAX_ADDRESS						0xFE

// Read/Write Limits
#define HID_SMBUS_MIN_READ_REQUEST_SIZE				1
#define HID_SMBUS_MAX_READ_REQUEST_SIZE				512
#define HID_SMBUS_MIN_TARGET_ADDRESS_SIZE			1
#define HID_SMBUS_MAX_TARGET_ADDRESS_SIZE			16
#define HID_SMBUS_MAX_READ_RESPONSE_SIZE			61
#define HID_SMBUS_MIN_WRITE_REQUEST_SIZE			1
#define HID_SMBUS_MAX_WRITE_REQUEST_SIZE			61

/////////////////////////////////////////////////////////////////////////////
// GPIO Definitions
/////////////////////////////////////////////////////////////////////////////

// GPIO Pin Direction Bit Value
#define HID_SMBUS_DIRECTION_INPUT					0
#define HID_SMBUS_DIRECTION_OUTPUT					1

// GPIO Pin Mode Bit Value
#define HID_SMBUS_MODE_OPEN_DRAIN					0
#define HID_SMBUS_MODE_PUSH_PULL					1

// GPIO Function Bitmask
#define HID_SMBUS_MASK_FUNCTION_GPIO_7_CLK			0x01
#define HID_SMBUS_MASK_FUNCTION_GPIO_0_TXT			0x02
#define HID_SMBUS_MASK_FUNCTION_GPIO_1_RXT			0x04

// GPIO Function Bit Value
#define HID_SMBUS_GPIO_FUNCTION						0
#define HID_SMBUS_SPECIAL_FUNCTION					1

// GPIO Pin Bitmask
#define HID_SMBUS_MASK_GPIO_0						0x01
#define HID_SMBUS_MASK_GPIO_1						0x02
#define HID_SMBUS_MASK_GPIO_2						0x04
#define HID_SMBUS_MASK_GPIO_3						0x08
#define HID_SMBUS_MASK_GPIO_4						0x10
#define HID_SMBUS_MASK_GPIO_5						0x20
#define HID_SMBUS_MASK_GPIO_6						0x40
#define HID_SMBUS_MASK_GPIO_7						0x80

/////////////////////////////////////////////////////////////////////////////
// Part Number Definitions
/////////////////////////////////////////////////////////////////////////////

// Part Numbers
#define HID_SMBUS_PART_CP2112						CP210x_PARTNUM_CP2112

/////////////////////////////////////////////////////////////////////////////
// User Customization Definitions
/////////////////////////////////////////////////////////////////////////////

// User-Customizable Field Lock Bitmasks
/// @ref HidSmbus_SetLock() and @ref HidSmbus_GetLock() options values
typedef enum _HID_SMBUS_LOCKBITS {
	HID_SMBUS_LOCK_VID = ((BYTE)(0xFF & 0x01))					///< VID
	, HID_SMBUS_LOCK_PID = ((BYTE)(0xFF & 0x02))				///< PID
	, HID_SMBUS_LOCK_POWER = ((BYTE)(0xFF & 0x04))				///< Power
	, HID_SMBUS_LOCK_POWER_MODE = ((BYTE)(0xFF & 0x08))			///< Power Mode
	, HID_SMBUS_LOCK_RELEASE_VERSION = ((BYTE)(0xFF & 0x10))	///< Release Version
	, HID_SMBUS_LOCK_MFG_STR = ((BYTE)(0xFF & 0x20))			///< Manufacturing String
	, HID_SMBUS_LOCK_PRODUCT_STR = ((BYTE)(0xFF & 0x40))		///< Product String
	, HID_SMBUS_LOCK_SERIAL_STR = ((BYTE)(0xFF & 0x80))			///< Serial String
} HID_SMBUS_LOCKBITS, *PHID_SMBUS_LOCKBITS;
_Check_return_
_Success_(return == TRUE)
__inline static bool IsValidHID_SMBUS_HID_SMBUS_LOCKBITS(_In_ const HID_SMBUS_LOCKBITS _s)
{
	return (HID_SMBUS_LOCK_VID == _s) || (HID_SMBUS_LOCK_PID == _s)
		|| (HID_SMBUS_LOCK_POWER == _s) || (HID_SMBUS_LOCK_POWER_MODE == _s)
		|| (HID_SMBUS_LOCK_RELEASE_VERSION == _s)
		|| (HID_SMBUS_LOCK_MFG_STR == _s) || (HID_SMBUS_LOCK_PRODUCT_STR == _s) || (HID_SMBUS_LOCK_SERIAL_STR == _s);
}

// Field Lock Bit Values
#define HID_SMBUS_LOCK_UNLOCKED						1	///< Field can be customized. If the bit is set to 1, the corresponding field HAS NOT been customized
#define HID_SMBUS_LOCK_LOCKED						0	///< Field has already been customized, or has been locked. If the bit is set to 0, the corresponding field HAS been customized or locked, and can no longer be changed for this device

// Power Max Value (500 mA)
#define HID_SMBUS_BUS_POWER_MAX						0xFA

// Power Modes
#define HID_SMBUS_BUS_POWER							0x00
#define HID_SMBUS_SELF_POWER_VREG_DIS				0x01
#define HID_SMBUS_SELF_POWER_VREG_EN				0x02

// USB Config Bitmasks
#define HID_SMBUS_SET_VID							0x01
#define HID_SMBUS_SET_PID							0x02
#define HID_SMBUS_SET_POWER							0x04
#define HID_SMBUS_SET_POWER_MODE					0x08
#define HID_SMBUS_SET_RELEASE_VERSION				0x10

// USB Config Bit Values
#define HID_SMBUS_SET_IGNORE						0
#define HID_SMBUS_SET_PROGRAM						1

// String Lengths
#define HID_SMBUS_CP2112_MFG_STRLEN					30
#define HID_SMBUS_CP2112_PRODUCT_STRLEN				30
#define HID_SMBUS_CP2112_SERIAL_STRLEN				30

// HID_SMBUS_MFG_STR
typedef char HID_SMBUS_CP2112_MFG_STR[HID_SMBUS_CP2112_MFG_STRLEN];

// HID_SMBUS_PRODUCT_STR
typedef char HID_SMBUS_CP2112_PRODUCT_STR[HID_SMBUS_CP2112_PRODUCT_STRLEN];

// HID_SMBUS_SERIAL_STR
typedef char HID_SMBUS_CP2112_SERIAL_STR[HID_SMBUS_CP2112_SERIAL_STRLEN];

/////////////////////////////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////////////////////////////

typedef void* HID_SMBUS_DEVICE;

/////////////////////////////////////////////////////////////////////////////
// Exported Library Functions
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// HidSmbus_GetNumDevices
/// @brief Determines the number of devices connected to the system with matching VID/PID
///
/// @param[out] lpdwNumDevices a pointer to a DWORD/4-byte location to hold the returned device count
/// @param[in]  vid is the 2-byte Vendor ID value.
/// @param[in]  pid is the 2-byte is the 2-byte Product ID value.
///
/// @returns Returns #HID_SMBUS_SUCCESS on success, or another @ref HID_SMBUS_STATUS Return value if there is an error.
///	@retval	#HID_SMBUS_SUCCESS -- success
///	@retval	#HID_SMBUS_INVALID_PARAMETER -- lpdwNumDevices was not specified, or is an unexpected value
	HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetNumDevices(_Out_writes_bytes_(sizeof(DWORD)) _Pre_defensive_ DWORD* lpdwNumDevices, _In_ _Pre_defensive_ const WORD vid, _In_ _Pre_defensive_ const WORD pid);

// HidSmbus_GetString
/// @brief Get and return the specified string from a non-Opened device
///
/// @param[in]  deviceNum is the 4-byte
/// @param[in]  vid is the 2-byte Vendor ID value.
/// @param[in]  pid is the 2-byte is the 2-byte Product ID value.
/// @param[out] deviceString points at a buffer into which the specified string will be copied and returned
/// @param[in]  options is the 4-byte tbd
///
/// @note Intended for use pre- @ref HidSmbus_Open() or post- @ref HidSmbus_Close(). Fails with #HID_SMBUS_DEVICE_ALREADY_OPENED otherwise.
///
/// @returns Returns #HID_SMBUS_SUCCESS on success, or another @ref HID_SMBUS_STATUS Return value if there is an error.
///	@retval	#HID_SMBUS_SUCCESS -- success
///	@retval	#HID_SMBUS_INVALID_PARAMETER -- deviceString was not specified, and/or options is an unexpected value
///	@retval	#HID_SMBUS_DEVICE_NOT_FOUND -- the specified device was not found
///	@retval	#HID_SMBUS_DEVICE_ALREADY_OPENED -- the specified device is already (exclusively) opened
///	@retval	#HID_SMBUS_DEVICE_ACCESS_ERROR -- tbd
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetString(_In_ _Pre_defensive_ const DWORD deviceNum, _In_ _Pre_defensive_ const WORD vid, WORD pid, char* deviceString, _In_ _Pre_defensive_ const HID_SMBUS_GETSTRING options);

// HidSmbus_GetOpenedString
/// @brief Get and return the specified string from an Opened device
///
/// @param[in]  device is an open handle to the device
/// @param[out] deviceString points at a buffer into which the specified string will be copied and returned
/// @param[in]  options is the 4-byte tbd
///
/// @returns Returns #HID_SMBUS_SUCCESS on success, or another @ref HID_SMBUS_STATUS Return value if there is an error.
///	@retval	#HID_SMBUS_SUCCESS -- success
///	@retval	#HID_SMBUS_INVALID_PARAMETER -- deviceString was not specified, and/or options is an unexpected value
///	@retval	#HID_SMBUS_INVALID_DEVICE_OBJECT -- the specified device is invalid
///	@retval	#HID_SMBUS_DEVICE_ACCESS_ERROR -- tbd
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetOpenedString(HID_SMBUS_DEVICE device, char* deviceString, _In_ _Pre_defensive_ const HID_SMBUS_GETSTRING options);

// HidSmbus_GetIndexedString
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetIndexedString(DWORD deviceNum, _In_ _Pre_defensive_ const WORD vid, _In_ _Pre_defensive_ const WORD pid, _In_ _Pre_defensive_ const DWORD stringIndex, char* deviceString);

// HidSmbus_GetOpenedIndexedString
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetOpenedIndexedString(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const DWORD stringIndex, char* deviceString);

// HidSmbus_GetAttributes
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetAttributes(_In_ _Pre_defensive_ const DWORD deviceNum, _In_ _Pre_defensive_ const WORD vid, _In_ _Pre_defensive_ const WORD pid, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* deviceVid, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* devicePid, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* deviceReleaseNumber);

// HidSmbus_GetOpenedAttributes
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetOpenedAttributes(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* deviceVid, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* devicePid, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* deviceReleaseNumber);

// HidSmbus_Open
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_Open(_Out_writes_bytes_(sizeof(HID_SMBUS_DEVICE*)) _Pre_defensive_ HID_SMBUS_DEVICE* device, _In_ _Pre_defensive_ const DWORD deviceNum, _In_ _Pre_defensive_ const WORD vid, _In_ _Pre_defensive_ const WORD pid);

// HidSmbus_Close
/// @brief Closes an open handle to the device
/// @param device is TBD
/// @returns Returns HID_SMBUS_SUCCESS on success, another HID_SMBUS_STATUS if there is an error:
///			HID_SMBUS_INVALID_HANDLE -- device is invalid
///			HID_SMBUS_DEVICE_ACCESS_ERROR -- device is invalid
///			HID_SMBUS_INVALID_DEVICE_OBJECT -- device is invalid
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_Close(HID_SMBUS_DEVICE device);

// HidSmbus_IsOpened
/// @brief Returns a yes/no indication of whether the device is opened or not
/// @param device is TBD
/// @param bIsOpened is a pointer to a BOOL location to hold the returned yes/no indication
/// @returns Returns HID_SMBUS_SUCCESS on success, another HID_SMBUS_STATUS if there is an error:
///			HID_SMBUS_INVALID_DEVICE_OBJECT -- device is invalid
///			HID_SMBUS_INVALID_PARAMETER -- bIsOpened is an unexpected value
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_IsOpened(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(BOOL)) _Pre_defensive_ BOOL* bIsOpened);

// HidSmbus_ReadRequest
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_ReadRequest(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const BYTE slaveAddress, _In_ _Pre_defensive_ const WORD numBytesToRead);

// HidSmbus_AddressReadRequest
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_AddressReadRequest(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const BYTE slaveAddress, _In_ _Pre_defensive_ const WORD numBytesToRead, _In_ _Pre_defensive_ const BYTE targetAddressSize, BYTE targetAddress[16]);

// HidSmbus_ForceReadResponse
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_ForceReadResponse(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const WORD numBytesToRead);

// HidSmbus_ForceReadResponse
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetReadResponse(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(HID_SMBUS_S0)) _Pre_defensive_ HID_SMBUS_S0* status, _Out_writes_bytes_(sizeof(bufferSize)) _Pre_defensive_ BYTE* buffer, _In_ _Pre_defensive_ const BYTE bufferSize, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* numBytesRead);

// HidSmbus_WriteRequest
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_WriteRequest(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const BYTE slaveAddress, BYTE* buffer, _In_ _Pre_defensive_ const BYTE numBytesToWrite);

// HidSmbus_TransferStatusRequest
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_TransferStatusRequest(HID_SMBUS_DEVICE device);

// HidSmbus_GetTransferStatusResponse
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetTransferStatusResponse(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(HID_SMBUS_S0)) _Pre_defensive_ HID_SMBUS_S0* status, HID_SMBUS_S1* detailedStatus, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* numRetries, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* bytesRead);

// HidSmbus_CancelTransfer
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_CancelTransfer(HID_SMBUS_DEVICE device);

// HidSmbus_CancelIo
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_CancelIo(HID_SMBUS_DEVICE device);

// HidSmbus_SetTimeouts
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_SetTimeouts(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const DWORD responseTimeout);

// HidSmbus_GetTimeouts
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetTimeouts(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(DWORD)) _Pre_defensive_ DWORD* responseTimeout);

// HidSmbus_SetSmbusConfig
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_SetSmbusConfig(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const DWORD bitRate, _In_ _Pre_defensive_ const BYTE address, _In_ _Pre_defensive_ const BOOL autoReadRespond, _In_ _Pre_defensive_ const WORD writeTimeout, _In_ _Pre_defensive_ const WORD readTimeout, _In_ _Pre_defensive_ const BOOL sclLowTimeout, _In_ _Pre_defensive_ const WORD transferRetries);

// HidSmbus_GetSmbusConfig
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetSmbusConfig(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(DWORD)) _Pre_defensive_ DWORD* bitRate, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* address, _Out_writes_bytes_(sizeof(BOOL)) _Pre_defensive_ BOOL* autoReadRespond, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* writeTimeout, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* readTimeout, _Out_writes_bytes_(sizeof(BOOL)) _Pre_defensive_ BOOL* sclLowtimeout, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* transferRetries);

// HidSmbus_Reset
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_Reset(HID_SMBUS_DEVICE device);

// HidSmbus_SetGpioConfig
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_SetGpioConfig(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const BYTE direction, _In_ _Pre_defensive_ const BYTE mode, _In_ _Pre_defensive_ const BYTE function, _In_ _Pre_defensive_ const BYTE clkDiv);

// HidSmbus_GetGpioConfig
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetGpioConfig(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* direction, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* mode, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* function, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* clkDiv);

// HidSmbus_ReadLatch
/// @brief Reads and returns the current port latch value from the device
/// @param device is TBD
/// @param latchValue points at a byte-sized buffer into which the port latch value will be written
/// @returns Returns HID_SMBUS_SUCCESS on success, another HID_SMBUS_STATUS if there is an error:
///			HID_SMBUS_INVALID_DEVICE_OBJECT -- device is invalid
///			HID_SMBUS_INVALID_PARAMETER -- bIsOpened is an unexpected value
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_ReadLatch(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* latchValue);

// HidSmbus_WriteLatch
/// @brief Writes a new port latch value to the device
/// @param device is TBD
/// @param latchValue is the new port latch value to be written to the device
/// @param latchMask is the new port latch value to be written to the device
/// @returns Returns HID_SMBUS_SUCCESS on success, another HID_SMBUS_STATUS if there is an error:
///			HID_SMBUS_INVALID_DEVICE_OBJECT -- device is invalid
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_WriteLatch(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const BYTE latchValue, _In_ _Pre_defensive_ const BYTE latchMask);

// HidSmbus_GetPartNumber
/// @brief Reads and returns the Part Number and Version from the device
///
/// @param[in] device is TBD
/// @param[out] partNumber points at a byte-sized buffer into which the Part Number value will be written
/// @param[out] version points at a byte-sized buffer into which the part's Version value will be written
///
/// @returns Returns #HID_SMBUS_SUCCESS on success, another @ref HID_SMBUS_STATUS if there is an error:
///	@retval	#HID_SMBUS_INVALID_DEVICE_OBJECT -- device is invalid
///	@retval	#HID_SMBUS_INVALID_PARAMETER -- partNumber or version is an unexpected value
///	@retval	#HID_SMBUS_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetPartNumber(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* partNumber, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* version);

// HidSmbus_GetLibraryVersion
/// @brief Reads and returns the DLL Version from the DLL
/// @param major points at a byte-sized buffer into which the major number part of the DLL Version will be written
/// @param minor points at a byte-sized buffer into which the minor number part of the DLL Version will be written
/// @param bIsRelease points at a BOOL-sized buffer into which a True/False indication of whether the DLL is a Release-build or not (Debug-build) will be written
/// @returns Returns HID_SMBUS_SUCCESS on success, another HID_SMBUS_STATUS if there is an error:
///			HID_SMBUS_INVALID_PARAMETER -- major or minor or bIsRelease is an unexpected value
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetLibraryVersion(_Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* major, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* minor, _Out_writes_bytes_(sizeof(BOOL)) _Pre_defensive_ BOOL* bIsRelease);

// HidSmbus_GetHidLibraryVersion
/// @brief Reads and returns the Firmware Version from the device
/// @param major points at a byte-sized buffer into which the major number part of the Firmware Version will be written
/// @param minor points at a byte-sized buffer into which the minor number part of the Firmware Version will be written
/// @param bIsRelease points at a BOOL-sized buffer into which a True/False indication of whether the Firmware is a Release-build or not (Debug-build) will be written
/// @returns Returns HID_SMBUS_SUCCESS on success, another HID_SMBUS_STATUS if there is an error:
///			HID_SMBUS_INVALID_PARAMETER -- major or minor or bIsRelease is an unexpected value
///			HID_SMBUS_DEVICE_ACCESS_ERROR -- the device failed to respond to I/O in any expected manner
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetHidLibraryVersion(_Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* major, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* minor, _Out_writes_bytes_(sizeof(BOOL)) _Pre_defensive_ BOOL* bIsRelease);

// HidSmbus_GetHidGuid
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetHidGuid(void* guid);

/////////////////////////////////////////////////////////////////////////////
// Exported Library Functions - Device Customization
/////////////////////////////////////////////////////////////////////////////

// HidSmbus_SetLock
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_SetLock(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const BYTE lock);

// HidSmbus_GetLock
/// @brief Get and return the device customization lock status
///
/// @param[in]  device is an open handle to the device as returned from @ref HidSmbus_Open()
/// @param[out] lock points at a 1-byte buffer into which the @ref #HID_SMBUS_LOCKBITS bitmask specifying which fields are locked will be copied and returned
///
/// @note 
///
/// @returns Returns #HID_SMBUS_SUCCESS on success, or another @ref HID_SMBUS_STATUS Return value if there is an error.
///	@retval	#HID_SMBUS_SUCCESS -- success
///	@retval	#HID_SMBUS_INVALID_PARAMETER -- lock was not specified, and/or is an unexpected value
///	@retval	#HID_SMBUS_INVALID_DEVICE_OBJECT -- the specified device is invalid
///	@retval	#HID_SMBUS_DEVICE_ACCESS_ERROR -- tbd
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetLock(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* lock);

// HidSmbus_SetUsbConfig
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_SetUsbConfig(HID_SMBUS_DEVICE device, _In_ _Pre_defensive_ const WORD vid, _In_ _Pre_defensive_ const WORD pid, _In_ _Pre_defensive_ const BYTE power, _In_ _Pre_defensive_ const BYTE powerMode, _In_ _Pre_defensive_ const WORD releaseVersion, _In_ _Pre_defensive_ const BYTE mask);

// HidSmbus_GetUsbConfig
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetUsbConfig(HID_SMBUS_DEVICE device, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* vid, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* pid, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* power, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* powerMode, _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ WORD* releaseVersion);

// HidSmbus_SetManufacturingString
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_SetManufacturingString(HID_SMBUS_DEVICE device, char* manufacturingString, _In_ _Pre_defensive_ const BYTE strlen);

// HidSmbus_GetManufacturingString
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetManufacturingString(HID_SMBUS_DEVICE device, char* manufacturingString, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* strlen);

// HidSmbus_SetProductString
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_SetProductString(HID_SMBUS_DEVICE device, char* productString, _In_ _Pre_defensive_ const BYTE strlen);

// HidSmbus_GetProductString
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetProductString(HID_SMBUS_DEVICE device, char* productString, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* strlen);

// HidSmbus_SetSerialString
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_SetSerialString(HID_SMBUS_DEVICE device, char* serialString, _In_ _Pre_defensive_ const BYTE strlen);

// HidSmbus_GetSerialString
_Check_return_
_Ret_range_(HID_SMBUS_SUCCESS, HID_SMBUS_UNKNOWN_ERROR)
_Success_(HID_SMBUS_SUCCESS)
HID_TO_SMBUS_API HID_SMBUS_STATUS WINAPI
HidSmbus_GetSerialString(HID_SMBUS_DEVICE device, char* serialString, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ BYTE* strlen);

#ifdef __cplusplus
}
#endif // __cplusplus

/// @}

#endif // HOST_LIB_SLABHIDSMBUS_INCLUDE_SLABCP2112_H_INCLUDED_QU4N7WC20W
