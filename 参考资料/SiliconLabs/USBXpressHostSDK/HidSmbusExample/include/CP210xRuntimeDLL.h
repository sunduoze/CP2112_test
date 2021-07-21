// TODO: Header guards? Or is that what _CP210x_STANDARD_DEF_ is doing? If so, that same token is used in CP210xRuntimeDLL.h, is that by design, or error?
#ifndef	HOST_LIB_RUNTIME_INCLUDE_CP210XRUNTIMEDLL_H_INCLUDED_QU4N7WC20W
#define	HOST_LIB_RUNTIME_INCLUDE_CP210XRUNTIMEDLL_H_INCLUDED_QU4N7WC20W

/// @addtogroup runtime CP201x Runtime Library
///
/// The Silicon Labs CP210x USB-to-UART bridges are devices that communicate over
/// the Universal Serial Bus(USB) to perform Universal Asynchronous Receiver / Transmitter
/// (UART) data transfers. These devices have many programmable options that can
/// be configured via USB. These devices also often include flexible GPIO functions that
/// can be configured and accessed via USB during runtime. Silicon Labs provides libraries
/// that can be used to configure these devices and access their GPIOs.
///
/// Two interface library files can be used to interface with CP210x devices, CP210xManufacturing.DLL and .LIB, which is responsible for reading and writing
/// the device settings, and CP210xRuntime.DLL and .LIB, which is responsible for interfacing with the device's GPIOs. The APIs for CP210xRuntime are
/// described below.
///
/// This document supercedes, obsoletes and takes precedences over AN978.
///
/// The CP210x Runtime Host API provides access to the GPIO port latch, and is meant for distribution with the product containing a CP210x
/// device.
///
/// Typically, the user initiates communication with the target CP210x device by opening a handle to a COM port using CreateFile() (See
/// AN197: Serial Communication Guide for CP210x). The handle returned allows the user to call the API functions listed below.
///
///	@note: Function calls into this API are blocked until completed. This can take several milliseconds depending on USB traffic.
///
/// @{

#include	<stdint.h>	// For C99 fixed sized types
#include	<stdbool.h>	// For C99 _Bool

#include	"silabs_defs.h"
#include	"silabs_sal.h"

#ifdef _WIN32
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the CP210xRUNTIMEDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// CP210xRUNTIMEDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CP210xRUNTIMEDLL_EXPORTS
#define CP210xRUNTIMEDLL_API
#else
#define CP210xRUNTIMEDLL_API __declspec(dllimport)
#pragma comment (lib, "CP210xRuntime.lib")
#endif
#else // !_WIN32
#define CP210xDLL_API
#define WINAPI
#define CP210xRUNTIMEDLL_API
#endif // !_WIN32


// CP210xRT_GetPartNumber() returned PartNums (@deprecated {instead see silabs_defs.h::CP210X_PARTNUM})
#define		CP210x_CP2101_VERSION				CP210x_PARTNUM_CP2101	///< @deprecated use @ref CP210x_PARTNUM_CP2101
#define		CP210x_CP2102_VERSION				CP210x_PARTNUM_CP2102	///< @deprecated use @ref CP210x_PARTNUM_CP2102
#define		CP210x_CP2103_VERSION				CP210x_PARTNUM_CP2103	///< @deprecated use @ref CP210x_PARTNUM_CP2103
#define		CP210x_CP2104_VERSION				CP210x_PARTNUM_CP2104	///< @deprecated use @ref CP210x_PARTNUM_CP2104
#define		CP210x_CP2105_VERSION				CP210x_PARTNUM_CP2105	///< @deprecated use @ref CP210x_PARTNUM_CP2105
#define		CP210x_CP2108_VERSION				CP210x_PARTNUM_CP2108	///< @deprecated use @ref CP210x_PARTNUM_CP2108
#define     CP210x_CP2109_VERSION               CP210x_PARTNUM_CP2109	///< @deprecated use @ref CP210x_PARTNUM_CP2109
#define     CP210x_CP2102N_QFN28_VERSION		CP210x_PARTNUM_CP2102N_QFN28	///< @deprecated use @ref CP210x_PARTNUM_CP2102N_QFN28
#define     CP210x_CP2102N_QFN24_VERSION		CP210x_PARTNUM_CP2102N_QFN24	///< @deprecated use @ref CP210x_PARTNUM_CP2102N_QFN24
#define     CP210x_CP2102N_QFN20_VERSION		CP210x_PARTNUM_CP2102N_QFN20	///< @deprecated use @ref CP210x_PARTNUM_CP2102N_QFN20


/// API Status return codes
#if defined(NOT_YET)
typedef enum _CP210x_STATUS {
	CP210x_SUCCESS = SILABS_STATUS_SUCCESS
	, CP210x_INVALID_HANDLE = 0x01			///< an handle parameter was not valid
	, CP210x_INVALID_PARAMETER = 0x02			///< a parameter was not valid
	, CP210x_DEVICE_IO_FAILED = 0x03	///< device I/O failed
	, CP210x_FUNCTION_NOT_SUPPORTED = 0x04			///< the specified function is not supported
	, CP210x_GLOBAL_DATA_ERROR = 0x05	///< global data error
	, CP210x_FILE_ERROR = 0x06	///< file error
	, CP210x_COMMAND_FAILED = 0x08	///< command failed
	, CP210x_INVALID_ACCESS_TYPE = 0x09	///< invalid access type

	, CP210x_DEVICE_NOT_FOUND = 0xFF			///< the specified device was not found
} CP210x_STATUS, *PCP210x_STATUS;
#else // defined(NOT_YET)
/// @defgroup RTCP210x_STATUS
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
#define     CP210x_MAX_PRODUCT_STRLEN           126
#define     CP210x_MAX_SERIAL_STRLEN            63

// Type definitions
typedef     char    CP210x_PRODUCT_STRING[CP210x_MAX_PRODUCT_STRLEN];
typedef     char    CP210x_SERIAL_STRING[CP210x_MAX_SERIAL_STRLEN];

// Mask and Latch value bit definitions
#define     CP210x_GPIO_0                       0x0001
#define     CP210x_GPIO_1                       0x0002
#define     CP210x_GPIO_2                       0x0004
#define     CP210x_GPIO_3                       0x0008
#define     CP210x_GPIO_4                       0x0010
#define     CP210x_GPIO_5                       0x0020
#define     CP210x_GPIO_6                       0x0040
#define     CP210x_GPIO_7                       0x0080
#define     CP210x_GPIO_8                       0x0100
#define     CP210x_GPIO_9                       0x0200
#define     CP210x_GPIO_10                      0x0400
#define     CP210x_GPIO_11                      0x0800
#define     CP210x_GPIO_12                      0x1000
#define     CP210x_GPIO_13                      0x2000
#define     CP210x_GPIO_14                      0x4000
#define     CP210x_GPIO_15                      0x8000

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
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
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_GetNumDevices(
		_Out_writes_bytes_(sizeof(uint32_t)) _Pre_defensive_ uint32_t *lpdwNumDevices
	);

/// @brief Opens a handle to the device
///
/// @detail Opens and returns a handle to a device using a device index determined by the number returned
/// from @ref CP210x_GetNumDevices().
///
/// @param[in] dwDeviceIndex is the desired device's index into the device list used to determine @ref CP210xRT_GetNumDevices()
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
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_Open(
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
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_Close(
	_In_ _Pre_defensive_ const HANDLE cyHandle
);

/// Reads and returns the current GPIO port latch value from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpwLatch points at a 2-byte buffer into which the port latch value will be written
///
/// @note Supported on CP210x devices @ref IsWriteReadLatchPartNum()
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref RTCP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpwLatch is an unexpected value
///	@retval	#CP210x_DEVICE_NOT_FOUND -- the device could not be determined to be a CP210x device
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support port latch
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_ReadLatch(_In_ _Pre_defensive_ const HANDLE cyHandle,
				   _Out_writes_bytes_(sizeof(WORD)) _Pre_defensive_ LPWORD lpwLatch);

/// @brief Writes a new GPIO port latch value to the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] mask is a bit-mask which determines which pins to change [Change = 1, Do not change = 0].
/// @param[in] latch is the new 2-byte GPIO port latch value to be written to the device
///
/// @note Supported on CP210x devices @ref IsWriteReadLatchPartNum()
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref RTCP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_DEVICE_NOT_FOUND -- the device could not be determined to be a CP210x device
///	@retval	#CP210x_FUNCTION_NOT_SUPPORTED -- the device does not support port latch
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_WriteLatch(_In_ _Pre_defensive_ const HANDLE cyHandle,
					_In_ _Pre_defensive_ const WORD mask,
					_In_ _Pre_defensive_ const WORD latch);

/// @brief Reads and returns the Part Number from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpbPartNum points at a 1-byte buffer into which the @ref CP210X_PARTNUM Part Number value will be written
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref RTCP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpbPartNum is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_GetPartNumber(_In_ _Pre_defensive_ const HANDLE cyHandle,
					   _Out_writes_bytes_(1) _Pre_defensive_ LPBYTE lpbPartNum);

/// @brief Reads and returns the Product String directly from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpProductString points at a buffer into which the Product String will be written
/// @param[out] lpbProductStringLengthInBytes is the total number of characters in the Product String
/// @param[in] bConvertToASCII is a true or false value indicating whether the Product String is an 16-bit UCS-2 character string needing converting to 8-bit ascii characters (true) to not (false).
///
/// @note Length of lpProductString must be <= @ref CP210x_MAX_SETUP_STRING_SIZE
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref RTCP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpProductString or lpbProductStringLengthInBytes are unexpected values
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_GetDeviceProductString(_In_ _Pre_defensive_ const HANDLE cyHandle,
								_Out_writes_bytes_(*lpbProductStringLengthInBytes) _Pre_defensive_ LPVOID lpProductString,
								_Out_writes_bytes_(1) _Pre_defensive_ LPBYTE lpbProductStringLengthInBytes,
								_In_ _Pre_defensive_ const BOOL bConvertToASCII = TRUE
                                );

/// @brief Reads and returns the Serial Number String directly from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpSerialNumberString points at a buffer into which the Serial Number String will be written
/// @param[out] lpbSerialNumberStringLengthInBytes is the total number of characters in the Serial Number String
/// @param[in] bConvertToASCII is a true or false value indicating whether the Serial Number String is an 16-bit UCS-2 character string needing converting to 8-bit ascii characters (true) to not (false).
///
/// @note Length of lpSerialNumberString must be <= @ref CP210x_MAX_SETUP_STRING_SIZE
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref RTCP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpSerialNumberString or lpbSerialNumberStringLengthInBytes are unexpected values
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_GetDeviceSerialNumber(_In_ _Pre_defensive_ const HANDLE cyHandle,
							   _Out_writes_bytes_(*lpbSerialNumberStringLengthInBytes) _Pre_defensive_ LPVOID lpSerialNumberString,
							   _Out_writes_bytes_(1) _Pre_defensive_ LPBYTE lpbSerialNumberStringLengthInBytes,
							   _In_ _Pre_defensive_ const BOOL bConvertToASCII = TRUE
                               );

/// @brief Reads and returns the Interface String directly from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] lpInterfaceString points at a buffer into which the Interface String will be written
/// @param[out] lpbInterfaceStringLengthInBytes is the total number of characters in the Interface String
/// @param[in] bConvertToASCII is a true or false value indicating whether the Interface String is an 16-bit UCS-2 character string needing converting to 8-bit ascii characters (true) to not (false).
///
/// @note Length of lpInterfaceString must be <= @ref CP210x_MAX_SETUP_STRING_SIZE
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref RTCP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- lpInterfaceString or lpbInterfaceStringLengthInBytes are unexpected values
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_GetDeviceInterfaceString(_In_ _Pre_defensive_ const HANDLE	cyHandle,
								  _Out_writes_bytes_(*lpbInterfaceStringLengthInBytes) _Pre_defensive_ LPVOID lpInterfaceString,
								  _Out_writes_bytes_(1) _Pre_defensive_ LPBYTE lpbInterfaceStringLengthInBytes,
								  _In_ _Pre_defensive_ const BOOL bConvertToASCII);


/// @brief Reads and returns the Receiver Max Timeout directly from the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[out] pReceiverMaxTimeoutInMicroseconds points at a buffer into which the current Receiver Max Timeout will be written
///
/// @note xxx
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref RTCP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- pReceiverMaxTimeoutInMicroseconds is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_GetReceiverMaxTimeout(_In_ _Pre_defensive_ const HANDLE cyHandle,
	_Out_writes_bytes_(2) _Pre_defensive_ uint16_t *pReceiverMaxTimeoutInMicroseconds);


/// @brief Sets the Receiver Max Timeout directly to the device
///
/// @param[in] cyHandle is an open handle to the device
/// @param[in] ReceiverMaxTimeoutInMicroseconds is the new Receiver Max Timeout for the device to use
///
/// @note xxx
///
/// @returns Returns #CP210x_SUCCESS on success, or another @ref RTCP210x_STATUS Return value if there is an error.
///	@retval	#CP210x_SUCCESS -- success
///	@retval	#CP210x_INVALID_HANDLE -- cyHandle is invalid
///	@retval	#CP210x_INVALID_PARAMETER -- ReceiverMaxTimeoutInMicroseconds is an unexpected value
///	@retval	#CP210x_DEVICE_IO_FAILED -- the device failed to respond to I/O in any expected manner
_Check_return_
_Ret_range_(CP210x_SUCCESS, CP210x_DEVICE_NOT_FOUND)
_Success_(return == CP210x_SUCCESS)
CP210xRUNTIMEDLL_API CP210x_STATUS WINAPI
CP210xRT_SetReceiverMaxTimeout(_In_ _Pre_defensive_ const HANDLE cyHandle,
	_In_ _Pre_defensive_ const uint16_t ReceiverMaxTimeoutInMicroseconds);

#ifdef __cplusplus
}
#endif // __cplusplus

/// @}

#endif	// !HOST_LIB_RUNTIME_INCLUDE_CP210XRUNTIMEDLL_H_INCLUDED_QU4N7WC20W


