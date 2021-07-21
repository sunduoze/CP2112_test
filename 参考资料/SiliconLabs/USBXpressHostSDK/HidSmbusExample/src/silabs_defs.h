 /*++

	VER_LEGALCOPYRIGHT_STR

Module Name:

    silabs_defs.h

Abstract:

    A top-level, across-all-repos, collection of some "useful" defines/enums

Environment:

    User or kernel mode

--*/
//
#if (_MSC_VER > 1000)
#pragma once
#endif

#ifndef	HOST_COMMON_INCLUDE_SILABS_DEFS_H_INCLUDED_BVHHTNCO7E
#define	HOST_COMMON_INCLUDE_SILABS_DEFS_H_INCLUDED_BVHHTNCO7E

/// @addtogroup silabs_defs
/// @{

#include	"silabs_sal.h"

#ifdef _WIN32
#if	! defined(__cplusplus)
#define bool BOOLEAN
#endif // ! defined(__cplusplus)
#else
typedef unsigned char BYTE;
#endif


#ifndef _WIN32
#define UNREFERENCED_PARAMETER(p) (void)(p)
#endif

#if defined(DEFINE_GUID)
DEFINE_GUID(GUID_DEVINTERFACE_SILABS_CP210x,
	0xa2a39220, 0x39f4, 0x4b88, 0xae, 0xcb, 0x3d, 0x86, 0xa3, 0x5d, 0xc7, 0x48);

// CP213x Device GUID (must match WinUSB Driver INF file's interface GUID)
// {F81D8403-3C27-45FF-98B7-3D00F19169CE}
DEFINE_GUID(GUID_DEVINTERFACE_SILABS_CP213x,
	0xf81d8403, 0x3c27, 0x45ff, 0x98, 0xb7, 0x3d, 0x00, 0xf1, 0x91, 0x69, 0xce);

// CP2615 Device GUID
// {26C6CE68-9013-4B40-B72A-298EB2BDD269}
DEFINE_GUID(GUID_DEVINTERFACE_SILABS_CP2615,
	0x26C6CE68, 0x9013, 0x4B40, 0xB7, 0x2A, 0x29, 0x8E, 0xB2, 0xBD, 0xD2, 0x69);

// USBXpress
// {3C5E1462-5695-4e18-876B-F3F3D08AAF18}
DEFINE_GUID(GUID_DEVINTERFACE_SILABS_USBXPRESS_BRIDGE, 
0x3c5e1462, 0x5695, 0x4e18, 0x87, 0x6b, 0xf3, 0xf3, 0xd0, 0x8a, 0xaf, 0x18);
#endif // DEFINE_GUID

#define SILABS_TEST_FILL	0xa5	// 10100101


/// Company Vendor ID (VID)
typedef enum _SILABS_VID {
	SILABS_VID_10C4 = ((unsigned short)(0xFFFF & 0x10C4))	///< Decimal 4292; VID aquired via Cygnal.
	, SILABS_VID_1BA4 = ((unsigned short)(0xFFFF & 0x1BA4))	///< Decimal 7076; VID aquired via Ember.
	, SILABS_VID_2544 = ((unsigned short)(0xFFFF & 0x2544))	///< Decimal 9540; VID aquired via Energy Micro.
} SILABS_VID, *PSILABS_VID;
_Check_return_
_Success_(return == TRUE)
__inline static bool IsValidSILABS_VID ( _In_ const SILABS_VID _v ) { return (SILABS_VID_10C4 == _v); }

/// Device Product IDs (PIDs)
typedef enum _SILABS_PID {
	SILABS_PID_UNKNOWN		=  ((unsigned short)(0xFFFF & 0x0000))
	, SILABS_PID_CP210SINGLEPORT		= ((unsigned short)(0xFFFF & 0xEA60))	///< Single port CP210x device/s
	, SILABS_PID_CP210SINGLEPORTII		= ((unsigned short)(0xFFFF & 0xEA63))	///< Single port CP210x device/s (Windows Update PID)
	, SILABS_PID_CP2101		= SILABS_PID_CP210SINGLEPORT						///< CP2101
	, SILABS_PID_CP2102		= SILABS_PID_CP210SINGLEPORT						///< CP2102
	, SILABS_PID_CP2102N	= SILABS_PID_CP210SINGLEPORT						///< CP2102N
	, SILABS_PID_CP2103		= SILABS_PID_CP210SINGLEPORT						///< CP2103
	, SILABS_PID_CP2104		= SILABS_PID_CP210SINGLEPORT						///< CP2104
	, SILABS_PID_CP210DUALPORT		= ((unsigned short)(0xFFFF & 0xEA70))		///< Dual port CP210x device/s
	, SILABS_PID_CP210DUALPORTII	= ((unsigned short)(0xFFFF & 0xEA7A))		///< Dual port CP210x device/s (Windows Update PID)
	, SILABS_PID_CP2105		= SILABS_PID_CP210DUALPORT							///< CP2105
	, SILABS_PID_CP2105II	= SILABS_PID_CP210DUALPORTII						///< CP2105 (Windows Update PID)
	, SILABS_PID_CP210QUADPORT		= ((unsigned short)(0xFFFF & 0xEA71))		///< Quad port CP210x device/s
	, SILABS_PID_CP210QUADPORTII	= ((unsigned short)(0xFFFF & 0xEA7B))		///< Quad port CP210x device/s (Windows Update PID)
	, SILABS_PID_CP2108		= SILABS_PID_CP210QUADPORT							///< CP2108
	, SILABS_PID_CP2108II	= SILABS_PID_CP210QUADPORTII						///< CP2108 (Windows Update PID)
	, SILABS_PID_CP2109		= SILABS_PID_CP210SINGLEPORT						///< CP2109
	, SILABS_PID_CP2110		= ((unsigned short)(0xFFFF & 0xEA80))				///< CP2110
	, SILABS_PID_CP2111		= SILABS_PID_CP2110									///< CP2111
	, SILABS_PID_CP2112		= ((unsigned short)(0xFFFF & 0xEA90))				///< CP2112
	, SILABS_PID_CP2114		= ((unsigned short)(0xFFFF & 0xEAB0))				///< CP2114
	, SILABS_PID_CP2130		= ((unsigned short)(0xFFFF & 0x87A0))				///< CP2130
	, SILABS_PID_CP2615     = ((unsigned short)(0xFFFF & 0xEAC1))				///< CP2615
	, SILABS_PID_DIRECTACCESS = ((unsigned short)(0xFFFF & 0xEA61))				///< Direct Access
	, SILABS_PID_USBXPress = SILABS_PID_DIRECTACCESS				///< Direct Access (nee USBXPress) @deprecate SILABS_PID_USBXPress
} SILABS_PID, *PSILABS_PID;
_Check_return_
_Success_(return == TRUE)
__inline static bool IsValidSILABS_PID(_In_ const SILABS_PID _p) {
#pragma warning ( push )
#pragma warning ( disable : 6287 ) // warning C6287: redundant code: the left and right sub-expressions are identical
	return ((SILABS_PID_CP210SINGLEPORT == _p) || (SILABS_PID_CP210SINGLEPORTII == _p)
			|| (SILABS_PID_CP210DUALPORT == _p) || (SILABS_PID_CP210DUALPORTII == _p)
			|| (SILABS_PID_CP210QUADPORT == _p) || (SILABS_PID_CP210QUADPORTII == _p)
			|| (SILABS_PID_CP2101 == _p) || (SILABS_PID_CP2102 == _p) || (SILABS_PID_CP2102N == _p) || (SILABS_PID_CP2103 == _p)
			|| (SILABS_PID_CP2104 == _p)
			|| (SILABS_PID_CP2105 == _p) || (SILABS_PID_CP2105II == _p)
			|| (SILABS_PID_CP2108 == _p) || (SILABS_PID_CP2108II == _p)
			|| (SILABS_PID_CP2109 == _p)
			|| (SILABS_PID_CP2110 == _p) || (SILABS_PID_CP2111 == _p) || (SILABS_PID_CP2112 == _p) || (SILABS_PID_CP2114 == _p)
			|| (SILABS_PID_CP2130 == _p)
			|| (SILABS_PID_CP2615 == _p)
			|| (SILABS_PID_DIRECTACCESS == _p));
#pragma warning ( pop )
}
_Check_return_
_Success_(return == TRUE)
__inline static bool IsValidCP210X_PID(_In_ const SILABS_PID _p) {
#pragma warning ( push )
#pragma warning ( disable : 6287 ) // warning C6287: redundant code: the left and right sub-expressions are identical
	return ((SILABS_PID_CP210SINGLEPORT == _p) || (SILABS_PID_CP210SINGLEPORTII == _p)
		 || (SILABS_PID_CP210DUALPORT == _p) || (SILABS_PID_CP210DUALPORTII == _p)
		 || (SILABS_PID_CP210QUADPORT == _p) || (SILABS_PID_CP210QUADPORTII == _p)
		 || (SILABS_PID_CP2101 == _p) || (SILABS_PID_CP2102 == _p) || (SILABS_PID_CP2102N == _p) || (SILABS_PID_CP2103 == _p)
		 || (SILABS_PID_CP2104 == _p)
		 || (SILABS_PID_CP2105 == _p) || (SILABS_PID_CP2105II == _p)
		 || (SILABS_PID_CP2108 == _p) || (SILABS_PID_CP2108II == _p)
		 || (SILABS_PID_CP2109 == _p));
#pragma warning ( pop )
}

/// Device Part Numbers
typedef enum _SILABS_PARTNUM_CPXXXX {
	CP210x_PARTNUM_UNKNOWN =  ((BYTE)(0xFF & 0x00))				///< 
	, CP210x_PARTNUM_CP2101 = ((BYTE)(0xFF & 0x01))				///< CP2101
	, CP210x_PARTNUM_CP2102 = ((BYTE)(0xFF & 0x02))				///< CP2102
	, CP210x_PARTNUM_CP2103 = ((BYTE)(0xFF & 0x03))				///< CP2103
	, CP210x_PARTNUM_CP2104 = ((BYTE)(0xFF & 0x04))				///< CP2104
	, CP210x_PARTNUM_CP2105 = ((BYTE)(0xFF & 0x05))				///< CP2105
	, CP210x_PARTNUM_CP2108 = ((BYTE)(0xFF & 0x08))				///< CP2108
	, CP210x_PARTNUM_CP2109 = ((BYTE)(0xFF & 0x09))				///< CP2109

	, SILABS_PARTNUM_CP2110 = ((BYTE)(0xFF & 0x0A))				///< CP2110 HID_UART_PART_CP2110
	, HID_UART_PART_CP2110 = SILABS_PARTNUM_CP2110

	, CP210x_PARTNUM_CP2112 = ((BYTE)(0xFF & 0x0C))				///< CP2112 HID_SMBUS_PART_CP2112
	, HID_SMBUS_PART_CP2112 = CP210x_PARTNUM_CP2112

	, SILABS_PARTNUM_CP2114 = ((BYTE)(0xFF & 0x0E))				///< CP2114 HID_UART_PART_CP2114
	, HID_UART_PART_CP2114 = SILABS_PARTNUM_CP2114

	, CP210x_PARTNUM_CP2102N_QFN28 = ((BYTE)(0xFF & 0x20))		///< CP2102N QFN28 
	, CP210x_PARTNUM_CP2102N_QFN24 = ((BYTE)(0xFF & 0x21))		///< CP2102N QFN24
	, CP210x_PARTNUM_CP2102N_QFN20 = ((BYTE)(0xFF & 0x22))		///< CP2102N QFN20

	, CP210x_PARTNUM_USBXPRESS_F3XX = ((BYTE)(0xFF & 0x80))		///< USBXPRESS_F3XX
	, CP210x_PARTNUM_USBXPRESS_EFM8 = ((BYTE)(0xFF & 0x80))		///< USBXPRESS_EFM8
	, CP210x_PARTNUM_USBXPRESS_EFM32 = ((BYTE)(0xFF & 0x81))	///< USBXPRESS_EFM32
} SILABS_PARTNUM_CPXXXX, *PSILABS_PARTNUM_CPXXXX;
#define CP210X_PARTNUM	SILABS_PARTNUM_CPXXXX	// TODO: Enstone: For now, preserve the old, CP210X-centric name for this enum
_Check_return_
_Success_(return == TRUE)
__inline static bool IsValidCP210X_PARTNUM(_In_ const SILABS_PARTNUM_CPXXXX _v) {
	return (((CP210x_PARTNUM_CP2101 <= _v) && (_v <= CP210x_PARTNUM_CP2105)) || (CP210x_PARTNUM_CP2108 == _v) || (CP210x_PARTNUM_CP2109 == _v) || (CP210x_PARTNUM_CP2112 == _v) || ((CP210x_PARTNUM_CP2102N_QFN28 <= _v) && (_v <= CP210x_PARTNUM_CP2102N_QFN20)) || (CP210x_PARTNUM_USBXPRESS_F3XX == _v));
}
_Check_return_
_Success_(return == TRUE)
__inline static bool IsCP2102N_PARTNUM(_In_ const SILABS_PARTNUM_CPXXXX _v) {
	return ((CP210x_PARTNUM_CP2102N_QFN28 == _v) || (CP210x_PARTNUM_CP2102N_QFN24 == _v) || (CP210x_PARTNUM_CP2102N_QFN20 == _v));
}
_Check_return_
_Success_(return == TRUE)
__inline static bool IsOTPCP210X_PARTNUM(_In_ const SILABS_PARTNUM_CPXXXX _v) {
	return ((CP210x_PARTNUM_CP2104 == _v) || (CP210x_PARTNUM_CP2105 == _v) || (CP210x_PARTNUM_CP2109 == _v));
}
_Check_return_
_Success_(return == TRUE)
__inline static bool IsOTP(_In_ const SILABS_PID _p, _In_ const SILABS_PARTNUM_CPXXXX _v) {
	return IsValidCP210X_PID(_p) ? IsOTPCP210X_PARTNUM(_v) : ((SILABS_PID_CP2110 == _v) || (SILABS_PID_CP2112 == _v) || (SILABS_PID_CP2114 == _v) || (SILABS_PID_CP2130 == _v));
}
_Check_return_
_Success_(return == TRUE)
__inline static bool IsWriteReadLatchPartNum(_In_ const SILABS_PARTNUM_CPXXXX _v) {
	return (IsCP2102N_PARTNUM(_v) || (CP210x_PARTNUM_CP2103 == _v) || (CP210x_PARTNUM_CP2104 == _v) || (CP210x_PARTNUM_CP2105 == _v) || (CP210x_PARTNUM_CP2108 == _v));
}
_Check_return_
_Success_(return == TRUE)
__inline static bool IsMultiInterfacePartNum(_In_ const SILABS_PARTNUM_CPXXXX _v) {
	return ((CP210x_PARTNUM_CP2105 == _v) || (CP210x_PARTNUM_CP2108 == _v));
}

/// API Status return codes
/// @enum SILABS_STATUS
typedef enum _SILABS_STATUS {
	SILABS_STATUS_SUCCESS = ((BYTE)(0xFF & 0x00))					///< no error

	, SILABS_STATUS_TBD = ((BYTE)(0xFF & 0x01))						///< tbd
	, SILABS_STATUS_INVALID_PARAMETER = ((BYTE)(0xFF & 0x02))		///< a parameter was not valid
	, SILABS_STATUS_INVALID_HANDLE = ((BYTE)(0xFF & 0x03))			///< an handle parameter was not valid

	, SILABS_STATUS_DEVICE_NOT_FOUND = ((BYTE)(0xFF & 0x04))		///< the specified device was not found
	, SILABS_STATUS_DEVICE_TBD = ((BYTE)(0xFF & 0x05))				///< the specified device is tbd
	, SILABS_STATUS_DEVICE_ALREADY_OPENED = ((BYTE)(0xFF & 0x06))	///< the specified device is already (exclusively) opened

	, SILABS_STATUS_DEVICE_ERROR = ((BYTE)(0xFF & 0x07))			///< general/underetmined device-side error
	, SILABS_STATUS_DEVICE_ERROR_STATE = ((BYTE)(0xFF & 0x08))		///< device-side in wrong state to perform operation
	, SILABS_STATUS_DEVICE_ERROR_TIMEOUT = ((BYTE)(0xFF & 0x09))	///< device-side timed out the operation

	, SILABS_STATUS_FUNCTION_NOT_SUPPORTED = ((BYTE)(0xFF & 0x0F))	///< the specified function is not supported

	, SILABS_STATUS_MEMORY_ALLOC_ERROR = ((BYTE)(0xFF & 0x44))		///< an attempt to dynamically allocate memory failed
	, SILABS_STATUS_SYSTEM_ERROR = ((BYTE)(0xFF & 0x74))			///< an unexpected, unrecoverable error happened while interacting with the operating system
	, SILABS_STATUS_UNKNOWN_ERROR = ((BYTE)(0xFF & 0xFF))			///< an otherwise unclassified error occurred
} SILABS_STATUS, *PSILABS_STATUS;
_Check_return_
_Success_(return == TRUE)
__inline static bool IsValidSILABS_STATUS(_In_ const SILABS_STATUS _s) {
	return (((SILABS_STATUS_SUCCESS <= _s) && (_s <= SILABS_STATUS_DEVICE_ERROR_TIMEOUT))
			|| (SILABS_STATUS_FUNCTION_NOT_SUPPORTED == _s)
			|| (SILABS_STATUS_MEMORY_ALLOC_ERROR == _s)
			|| (SILABS_STATUS_SYSTEM_ERROR == _s)
			|| (SILABS_STATUS_UNKNOWN_ERROR == _s));
}
_Check_return_
_Success_(return == TRUE)
__inline static bool IsSuccessSILABS_STATUS(_In_ const SILABS_STATUS _s) { return SILABS_STATUS_SUCCESS == _s; }

/// @}

#endif // !defined(HOST_COMMON_INCLUDE_SILABS_DEFS_H_INCLUDED_BVHHTNCO7E)

