/////////////////////////////////////////////////////////////////////////////
// Utilities.cpp
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Utilities.h"

/////////////////////////////////////////////////////////////////////////////
// Public Functions
/////////////////////////////////////////////////////////////////////////////

CString HidSmbus_DecodeErrorStatus(const HID_SMBUS_STATUS status)
{
	CString str;

	switch (status)
	{
	case HID_SMBUS_SUCCESS:					str = _T("HID_SMBUS_SUCCESS");					break;
	case HID_SMBUS_DEVICE_NOT_FOUND:		str = _T("HID_SMBUS_DEVICE_NOT_FOUND");			break;
	case HID_SMBUS_INVALID_HANDLE:			str = _T("HID_SMBUS_INVALID_HANDLE");			break;
	case HID_SMBUS_INVALID_DEVICE_OBJECT:	str = _T("HID_SMBUS_INVALID_DEVICE_OBJECT");	break;
	case HID_SMBUS_INVALID_PARAMETER:		str = _T("HID_SMBUS_INVALID_PARAMETER");		break;
	case HID_SMBUS_INVALID_REQUEST_LENGTH:	str = _T("HID_SMBUS_INVALID_REQUEST_LENGTH");	break;
	case HID_SMBUS_READ_ERROR:				str = _T("HID_SMBUS_READ_ERROR");				break;
	case HID_SMBUS_WRITE_ERROR:				str = _T("HID_SMBUS_WRITE_ERROR");				break;
	case HID_SMBUS_READ_TIMED_OUT:			str = _T("HID_SMBUS_READ_TIMED_OUT");			break;
	case HID_SMBUS_WRITE_TIMED_OUT:			str = _T("HID_SMBUS_WRITE_TIMED_OUT");			break;
	case HID_SMBUS_DEVICE_IO_FAILED:		str = _T("HID_SMBUS_DEVICE_IO_FAILED");			break;
	case HID_SMBUS_DEVICE_ACCESS_ERROR:		str = _T("HID_SMBUS_DEVICE_ACCESS_ERROR");		break;
	case HID_SMBUS_DEVICE_NOT_SUPPORTED:	str = _T("HID_SMBUS_DEVICE_NOT_SUPPORTED");		break;
	case HID_SMBUS_UNKNOWN_ERROR:			str = _T("HID_SMBUS_UNKNOWN_ERROR");			break;
	default:								str = _T("Unknown Status");						break;
	}

	return str;
}

CString HidSmbus_DecodeTransferStatus(const HID_SMBUS_S0 status0)
{
	CString str;

	switch (status0)
	{
	case HID_SMBUS_S0_IDLE:			str = _T("Idle");				break;
	case HID_SMBUS_S0_BUSY:			str = _T("Busy");				break;
	case HID_SMBUS_S0_COMPLETE:		str = _T("Complete");			break;
	case HID_SMBUS_S0_ERROR:		str = _T("Error");				break;
	default:						str = _T("Unknown Status");		break;
	}

	return str;
}

CString HidSmbus_DecodeTransferStatus(const HID_SMBUS_S0 status0, const HID_SMBUS_S1 status1)
{
	CString str;

	switch (status0)
	{
	case HID_SMBUS_S0_IDLE:			str = _T("Idle");			break;
	case HID_SMBUS_S0_BUSY:			str = _T("Busy - ");		break;
	case HID_SMBUS_S0_COMPLETE:		str = _T("Complete");		break;
	case HID_SMBUS_S0_ERROR:		str = _T("Error - ");		break;
	default:						str = _T("Unknown Status");	break;
	}

	if (status0 == HID_SMBUS_S0_BUSY)
	{
		switch (status1)
		{
		case HID_SMBUS_S1_BUSY_ADDRESS_ACKED:	str += _T("Address Acked");		break;
		case HID_SMBUS_S1_BUSY_ADDRESS_NACKED:	str += _T("Address Nacked");	break;
		case HID_SMBUS_S1_BUSY_READING:			str += _T("Read in Progress");	break;
		case HID_SMBUS_S1_BUSY_WRITING:			str += _T("Write in Progress");	break;
		default:								str += _T("Unknown Status");	break;
		}
	}
	else if (status0 == HID_SMBUS_S0_ERROR)
	{
		switch (status1)
		{
		case HID_SMBUS_S1_ERROR_TIMEOUT_NACK:			str += _T("Timeout (Address Nacked)");	break;
		case HID_SMBUS_S1_ERROR_TIMEOUT_BUS_NOT_FREE:	str += _T("Timeout (Bus Not Free)");	break;
		case HID_SMBUS_S1_ERROR_ARB_LOST:				str += _T("Arbitration Lost");			break;
		case HID_SMBUS_S1_ERROR_READ_INCOMPLETE:		str += _T("Read Incomplete");			break;
		case HID_SMBUS_S1_ERROR_WRITE_INCOMPLETE:		str += _T("Write Incomplete");			break;
		case HID_SMBUS_S1_ERROR_SUCCESS_AFTER_RETRY:	str += _T("Success After Retries");		break;
		default:										str += _T("Unknown Status");			break;
		}
	}

	return str;
}
