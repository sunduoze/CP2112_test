////////////////////////////////////////////////////////////////////////////////
// SLABHIDDevice.h
////////////////////////////////////////////////////////////////////////////////

#ifndef HOST_LIB_SLABHIDDEVICE_INCLUDE_SLABHIDDEVICE_H_INCLUDED_MJWYLEYUPA
#define HOST_LIB_SLABHIDDEVICE_INCLUDE_SLABHIDDEVICE_H_INCLUDED_MJWYLEYUPA

/// @addtogroup slabhiddevice CP211x HID-based interface Library
///
/// @{
/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////


#include	"silabs_defs.h"
#include	"silabs_sal.h"
#ifdef _WIN32
#include <windows.h>
#else // !_WIN32
#endif // !_WIN32
#include "Types.h"

#ifdef _WIN32
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CP210xDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CP210xDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SLAB_HID_DEVICE_EXPORTS
#define SLAB_HID_DEVICE_API
#elif defined(SLAB_HID_DEVICE_BUILD_SOURCE)
#define SLAB_HID_DEVICE_API
#else
#define SLAB_HID_DEVICE_API __declspec(dllimport)
#pragma comment (lib, "SLABHIDDevice.lib")
#endif
#else // !_WIN32
#define SLAB_HID_DEVICE_API
#define WINAPI
#endif // !_WIN32


/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

/// API Status return codes
#define	USE_LEGACY_SLAB_HID_DEVICE_STATUS
#if !defined(USE_LEGACY_SLAB_HID_DEVICE_STATUS)	// TODO: Enstone: These defines are a cut'n'paste dup of the ones in HIDDevice.h.
typedef enum _SLAB_HID_DEVICE_STATUS {
	HID_DEVICE_SUCCESS = SILABS_STATUS_SUCCESS		///< @ref SILABS_STATUS no error
	, HID_DEVICE_NOT_FOUND = 0x01					///< the specified device was not found
	, HID_DEVICE_NOT_OPENED = 0x02
	, HID_DEVICE_ALREADY_OPENED = 0x03
	, HID_DEVICE_TRANSFER_TIMEOUT = 0x04
	, HID_DEVICE_TRANSFER_FAILED = 0x05
	, HID_DEVICE_CANNOT_GET_HID_INFO = 0x06
	, HID_DEVICE_HANDLE_ERROR = 0x07
	, HID_DEVICE_INVALID_BUFFER_SIZE = 0x08
	, HID_DEVICE_SYSTEM_CODE = 0x09
	, HID_DEVICE_UNSUPPORTED_FUNCTION = 0x0A
	, HID_DEVICE_UNKNOWN_ERROR = SILABS_STATUS_UNKNOWN_ERROR
} SLAB_HID_DEVICE_STATUS, *PSLAB_HID_DEVICE_STATUS;
#else // defined(NOT_YET)
typedef BYTE SLAB_HID_DEVICE_STATUS;
#define HID_DEVICE_SUCCESS					SILABS_STATUS_SUCCESS	///< @ref SILABS_STATUS no error
#define HID_DEVICE_NOT_FOUND				0x01					///< the specified device was not found
#define HID_DEVICE_NOT_OPENED				0x02
#define HID_DEVICE_ALREADY_OPENED			0x03
#define	HID_DEVICE_TRANSFER_TIMEOUT			0x04
#define HID_DEVICE_TRANSFER_FAILED			0x05
#define HID_DEVICE_CANNOT_GET_HID_INFO		0x06
#define HID_DEVICE_HANDLE_ERROR				0x07
#define HID_DEVICE_INVALID_BUFFER_SIZE		0x08
#define HID_DEVICE_SYSTEM_CODE				0x09
#define HID_DEVICE_UNSUPPORTED_FUNCTION		0x0A
#define HID_DEVICE_UNKNOWN_ERROR			SILABS_STATUS_UNKNOWN_ERROR
#endif // defined(NOT_YET)


// Max number of USB Devices allowed
#define MAX_USB_DEVICES					64

// Max number of reports that can be requested at time
#define MAX_REPORT_REQUEST_XP			512
#define MAX_REPORT_REQUEST_2K			200

#define DEFAULT_REPORT_INPUT_BUFFERS	0

// String Types
typedef enum _SLAB_HID_DEVICE_STRING_ID {
	SLAB_HID_DEVICE_STRING_ID_VID_STRING	= 0x01
	, SLAB_HID_DEVICE_STRING_ID_PID_STRING = 0x02
	, SLAB_HID_DEVICE_STRING_ID_PATH_STRING = 0x03
	, SLAB_HID_DEVICE_STRING_ID_SERIAL_STRING = 0x04
	, SLAB_HID_DEVICE_STRING_ID_MANUFACTURER_STRING = 0x05
	, SLAB_HID_DEVICE_STRING_ID_PRODUCT_STRING = 0x06
} SLAB_HID_DEVICE_STRING_ID, *PSLAB_HID_DEVICE_STRING_ID;
#define HID_VID_STRING					0x01
#define HID_PID_STRING					0x02
#define HID_PATH_STRING					0x03
#define HID_SERIAL_STRING				0x04
#define HID_MANUFACTURER_STRING			0x05
#define HID_PRODUCT_STRING				0x06

// String Lengths	// TODO: Length in bytes, or length in characters? NOTE: These are device-side string descriptor lengths, see host_common/include/silabs_usb.h
typedef enum _SLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES {
	SLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES_VID = 5
	, SLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES_PID = 5
	, SLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES_PATH = 260
	, SLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES_SERIAL = 256
	, SLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES_MANUFACTURER = 256
	, SLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES_PRODUCT = 256

	, SLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES_MAX = 260
	, SLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES_MAX_INDEXED = 256
} SLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES, *PSLAB_HID_DEVICE_STRING_MAX_LENGTH_IN_BYTES;
#define MAX_VID_LENGTH					5
#define MAX_PID_LENGTH					5
#define MAX_PATH_LENGTH                         260
#define MAX_SERIAL_STRING_LENGTH		256
#define MAX_MANUFACTURER_STRING_LENGTH	256
#define MAX_PRODUCT_STRING_LENGTH		256
#define MAX_INDEXED_STRING_LENGTH		256
#define MAX_STRING_LENGTH				260

/////////////////////////////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////////////////////////////

typedef void* HID_DEVICE;

/////////////////////////////////////////////////////////////////////////////
// Exported Functions
/////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/// @brief tbd
///
/// @param[in] Vid is the 2-byte Vendor ID value.
/// @param[in] Pid is the 2-byte Product ID value.
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS)
SLAB_HID_DEVICE_API	DWORD WINAPI
HidDevice_GetNumHidDevices(_In_ _Pre_defensive_ const WORD Vid, _In_ _Pre_defensive_ const WORD Pid);

/// @brief tbd
///
/// @param[in]  TBD
/// @param[in]  hidStringType - an SLAB_HID_DEVICE_STRING_ID 
/// @param[in] Vid is the 2-byte Vendor ID value.
/// @param[in] Pid is the 2-byte Product ID value.
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetHidString(_In_ _Pre_defensive_ const DWORD deviceIndex, _In_ _Pre_defensive_ const WORD Vid, _In_ _Pre_defensive_ const WORD Pid, _In_ _Pre_defensive_ const BYTE hidStringType, char* deviceString, _In_ _Pre_defensive_ const DWORD deviceStringLength);

/// @brief tbd
///
/// @param[in]  TBD
/// @param[in] Vid is the 2-byte Vendor ID value.
/// @param[in] Pid is the 2-byte Product ID value.
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetHidIndexedString(_In_ _Pre_defensive_ const DWORD deviceIndex, _In_ _Pre_defensive_ const WORD Vid, _In_ _Pre_defensive_ const WORD Pid, _In_ _Pre_defensive_ const DWORD stringIndex, char* deviceString, _In_ _Pre_defensive_ const DWORD deviceStringLength);

/// @brief tbd
///
/// @param[in]  TBD
/// @param[in] Vid is the 2-byte Vendor ID value.
/// @param[in] Pid is the 2-byte Product ID value.
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetHidAttributes(_In_ _Pre_defensive_ const DWORD deviceIndex, _In_ _Pre_defensive_ const WORD Vid, _In_ _Pre_defensive_ const WORD Pid, _Out_writes_bytes_(2) _Pre_defensive_ WORD* deviceVid, _Out_writes_bytes_(2) _Pre_defensive_ WORD* devicePid, _Out_writes_bytes_(2) _Pre_defensive_ WORD* deviceReleaseNumber);

SLAB_HID_DEVICE_API	void	WINAPI
HidDevice_GetHidGuid(void* hidGuid);

/// @brief tbd
///
/// @param[in]  TBD
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetHidLibraryVersion(_Out_writes_bytes_(1) _Pre_defensive_ BYTE* major, _Out_writes_bytes_(1) _Pre_defensive_ BYTE* minor, _Out_writes_bytes_(4) BOOL* release);

/// @brief tbd
///
/// @param[in]  TBD
/// @param[in] Vid is the 2-byte Vendor ID value.
/// @param[in] Pid is the 2-byte Product ID value.
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_Open(HID_DEVICE* device, _In_ _Pre_defensive_ const DWORD deviceIndex, _In_ _Pre_defensive_ const WORD Vid, _In_ _Pre_defensive_ const WORD Pid, _In_ _Pre_defensive_ const DWORD numInputBuffers);

_Check_return_
_Ret_range_(FALSE, TRUE)
_Success_(return)
SLAB_HID_DEVICE_API	BOOL WINAPI
HidDevice_IsOpened(_In_ _Pre_defensive_ const HID_DEVICE device);

SLAB_HID_DEVICE_API	HANDLE WINAPI
HidDevice_GetHandle(_In_ _Pre_defensive_ const HID_DEVICE device);

/// @brief tbd
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in]  TBD
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetString(_In_ _Pre_defensive_ const HID_DEVICE device, _In_ _Pre_defensive_ const BYTE hidStringType, char* deviceString, _In_ _Pre_defensive_ const DWORD deviceStringLength);

/// @brief tbd
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in]  TBD
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetIndexedString(_In_ _Pre_defensive_ const HID_DEVICE device, _In_ _Pre_defensive_ const DWORD stringIndex, char* deviceString, _In_ _Pre_defensive_ const DWORD deviceStringLength);

/// @brief tbd
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in]  TBD
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetAttributes(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(2) _Pre_defensive_ WORD* deviceVid, _Out_writes_bytes_(2) _Pre_defensive_ WORD* devicePid, _Out_writes_bytes_(2) _Pre_defensive_ WORD* deviceReleaseNumber);

/// @brief tbd
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in]  ReportBuffer - The buffer into which the tbd report should be placed.The first byte of the buffer should be set to the report ID of the desired report
/// @param[in] ReportBufferLengthInBytes - The size of ReportBuffer in bytes
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_SetFeatureReport_Control(_In_ _Pre_defensive_ const HID_DEVICE device, _In_reads_bytes_(ReportBufferLengthInBytes) _Pre_defensive_ BYTE* ReportBuffer, _In_ _Pre_defensive_ const DWORD ReportBufferLengthInBytes);

/// @brief tbd
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in]  ReportBuffer - The buffer into which the tbd report should be placed. The first byte of the buffer should be set to the report ID of the desired report.
/// @param[in] ReportBufferLengthInBytes - The size of ReportBuffer in bytes
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetFeatureReport_Control(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(ReportBufferLengthInBytes) _Pre_defensive_ BYTE* ReportBuffer, _In_ _Pre_defensive_ const DWORD ReportBufferLengthInBytes);

/// @brief tbd
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in]  ReportBuffer - The buffer into which the tbd report should be placed.The first byte of the buffer should be set to the report ID of the desired report
/// @param[in] ReportBufferLengthInBytes - The size of ReportBuffer in bytes
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_SetOutputReport_Interrupt(_In_ _Pre_defensive_ const HID_DEVICE device, _In_reads_bytes_(ReportBufferLengthInBytes) _Pre_defensive_ BYTE* ReportBuffer, _In_ _Pre_defensive_ const DWORD ReportBufferLengthInBytes);

/// @brief tbd
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in]  ReportBuffer - The buffer into which the tbd report should be placed.The first byte of the buffer should be set to the report ID of the desired report
/// @param[in] ReportBufferLengthInBytes - The size of ReportBuffer in bytes
/// @param[in] numReports - The maximum number of input reports to return
/// @param[in] bytesReturned - The number of bytes returned in ReportBuffer
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetInputReport_Interrupt(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(ReportBufferLengthInBytes) _Pre_defensive_ BYTE* ReportBuffer, _In_ _Pre_defensive_ const DWORD ReportBufferLengthInBytes, _In_ _Pre_defensive_ const DWORD numReports, _Out_writes_bytes_(sizeof(DWORD)) _Pre_defensive_ DWORD* bytesReturned);

/// @brief tbd
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in]  ReportBuffer - The buffer into which the tbd report should be placed.The first byte of the buffer should be set to the report ID of the desired report
/// @param[in] ReportBufferLengthInBytes - The size of ReportBuffer in bytes
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetInputReport_Interrupt_WithTimeout(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(ReportBufferLengthInBytes) _Pre_defensive_ BYTE* ReportBuffer, _In_ _Pre_defensive_ const DWORD ReportBufferLengthInBytes, _In_ _Pre_defensive_ const DWORD numReports, _Out_writes_bytes_(sizeof(BYTE)) _Pre_defensive_ DWORD* bytesReturned, _In_ _Pre_defensive_ const DWORD TimeoutMSec);

/// @brief Sends an HID output report from the host to the device over the control endpoint
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in]  TBD
/// @param[in] ReportBufferLengthInBytes - The size of ReportBuffer in bytes
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_SetOutputReport_Control(_In_ _Pre_defensive_ const HID_DEVICE device, _In_reads_bytes_(ReportBufferLengthInBytes) _Pre_defensive_ BYTE* ReportBuffer, _In_ _Pre_defensive_ const DWORD ReportBufferLengthInBytes);

/// @brief Receives an HID input report from the device to the host over the control endpoint
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in]  ReportBuffer - The buffer into which the tbd report should be placed.The first byte of the buffer should be set to the report ID of the desired report
/// @param[in] ReportBufferLengthInBytes - The size of ReportBuffer in bytes
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Check_return_
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_GetInputReport_Control(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(ReportBufferLengthInBytes) _Pre_defensive_ BYTE* ReportBuffer, _In_ _Pre_defensive_ const DWORD ReportBufferLengthInBytes);

/// @brief Returns the maximum input report size including the report ID
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
///
/// @returns Returns the tbd
SLAB_HID_DEVICE_API	WORD WINAPI
HidDevice_GetInputReportBufferLength(_In_ _Pre_defensive_ const HID_DEVICE device);

/// @brief Returns the maximum output report size including the report ID
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
///
/// @returns Returns the tbd
SLAB_HID_DEVICE_API	WORD WINAPI
HidDevice_GetOutputReportBufferLength(_In_ _Pre_defensive_ const HID_DEVICE device);

/// @brief Returns the maximum feature report size, including the report ID
///
/// @detail This function should be called to determine the appropriate buffer size for HidDevice_GetFeatureReport_Control().
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
///
/// @returns A return value of zero indicates that the specified device cannot be found. A non-zero value
/// specifies the maximum feature report size, including the report ID.
SLAB_HID_DEVICE_API	WORD WINAPI
HidDevice_GetFeatureReportBufferLength(_In_ _Pre_defensive_ const HID_DEVICE device);

/// @brief Returns the maximum number of input reports that can be queued in the HID driver
///
/// @detail This function should be called to determine the appropriate buffer size for @ref HidDevice_GetInputReport_Interrupt().
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
///
/// @returns A return value of zero indicates that the specified device cannot be found. A non-zero return value
/// specifies the maximum number of reports in the HID driver input report queue.
SLAB_HID_DEVICE_API	DWORD WINAPI
HidDevice_GetMaxReportRequest(_In_ _Pre_defensive_ const HID_DEVICE device);

/// @brief Deletes all pending input reports in the HID driver queue and the HID library
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
///
/// @returns Returns TRUE if the function completed successfully.
/// Returns FALSE if the function failed or if the device does not exist.
///	@retval	TRUE -- success
///	@retval	FALSE -- the function failed or the device does not exist.
_Check_return_
_Ret_range_(FALSE, TRUE)
_Success_(return)
SLAB_HID_DEVICE_API	BOOL WINAPI
HidDevice_FlushBuffers(_In_ _Pre_defensive_ const HID_DEVICE device);

/// @brief Cancels all pending input and output operations issued by the calling thread
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
///
/// @returns Returns TRUE if the function completed successfully.
/// Returns FALSE if the function failed or if the device does not exist.
///	@retval	TRUE -- success
///	@retval	FALSE -- the function failed or the device does not exist.
_Check_return_
_Ret_range_(FALSE, TRUE)
_Success_(return)
SLAB_HID_DEVICE_API	BOOL WINAPI
HidDevice_CancelIo(_In_ _Pre_defensive_ const HID_DEVICE device);

/// @brief Returns the input and output report timeouts over the interrupt endpoint
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[out] getReportTimeoutInMilliSeconds
/// @param[out] setReportTimeoutInMilliSeconds
///
/// @note 
///
/// @returns Returns no return value.
SLAB_HID_DEVICE_API	void WINAPI
HidDevice_GetTimeouts(_In_ _Pre_defensive_ const HID_DEVICE device, _Out_writes_bytes_(4) _Pre_defensive_ DWORD* getReportTimeoutInMilliSeconds, _Out_writes_bytes_(4) _Pre_defensive_ DWORD* setReportTimeoutInMilliSeconds);

/// @brief Sets the input and output report timeouts over the interrupt endpoint
///
/// @detail Timeouts are used for @ref HidDevice_GetInputReport_Interrupt() and @ref HidDevice_SetOutputReport_Interrupt().
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
/// @param[in] getReportTimeoutInMilliSeconds - Sets the timeout used in @ref  HidDevice_GetInputReport_Interrupt() in
/// milliseconds. This timeout specifies the number of milliseconds that must elapse before
/// @ref HidDevice_GetInputReport_Interrupt() will return fewer bytes than requested with an
/// #HID_DEVICE_TRANSFER_TIMEOUT error. A timeout of 0 ms will return immediately with
/// any available data.
/// @param[in] setReportTimeoutInMilliSeconds - Sets the timeout used in @ref HidDevice_SetOutputReport_Interrupt() in
/// milliseconds.This timeout specifies the number of milliseconds that must elapse during an
/// output report before @ref HidDevice_SetOutputReport_Interrupt() will return with an
/// #HID_DEVICE_TRANSFER_TIMEOUT error.
///
/// @note If get report timeouts are set to a large value and no data is received, the application may appear
/// unresponsive.Therefore, it is recommended to set timeouts to an appropriate value before reading
/// from the device.
/// The default value for get report and set report timeouts is 1000 ms and can be set to any number
/// of milliseconds from 0 to 0xFFFFFFFF.
///
/// @returns Returns no return value.
SLAB_HID_DEVICE_API	void WINAPI
HidDevice_SetTimeouts(_In_ _Pre_defensive_ const HID_DEVICE device, _In_ _Pre_defensive_ const DWORD getReportTimeoutInMilliSeconds, _In_ _Pre_defensive_ const DWORD setReportTimeoutInMilliSeconds);

/// @brief Closes an open handle to the device
///
/// @param[in] device is an open handle to the device as returned by @ref HidDevice_Open()
///
/// @returns Returns HID_DEVICE_SUCCESS on success, or another @ref SLAB_HID_DEVICE_STATUS Return value if there is an error.
///	\retval	HID_DEVICE_SUCCESS -- success
_Ret_range_(HID_DEVICE_SUCCESS, HID_DEVICE_UNKNOWN_ERROR)
_Success_(HID_DEVICE_SUCCESS == return)
SLAB_HID_DEVICE_API	SLAB_HID_DEVICE_STATUS WINAPI
HidDevice_Close(_In_ _Pre_defensive_ const HID_DEVICE device);

#ifdef __cplusplus
}
#endif // __cplusplus

/// @}

#endif // HOST_LIB_SLABHIDDEVICE_INCLUDE_SLABHIDDEVICE_H_INCLUDED_MJWYLEYUPA
