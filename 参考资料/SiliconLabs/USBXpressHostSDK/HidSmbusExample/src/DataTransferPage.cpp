/////////////////////////////////////////////////////////////////////////////
// DataTransferPage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HidSmbusExample.h"
#include "DataTransferPage.h"
#include "HidSmbusExampleDlg.h"
#include "CustomDDX.h"
#include "Utilities.h"

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// Static Global Function Prototypes
/////////////////////////////////////////////////////////////////////////////

static std::vector<BYTE> GetHexArray(CString hexString);

/////////////////////////////////////////////////////////////////////////////
// Static Global Functions
/////////////////////////////////////////////////////////////////////////////

// Given a input string containing hex byte values,
// return a vector of the numeric values
std::vector<BYTE> GetHexArray(CString hexString)
{
	std::vector<BYTE> retVal;

	// Remove whitespace
	hexString.Replace(_T(" "), _T(""));
	hexString.Replace(_T(","), _T(""));
	hexString.Replace(_T("\t"), _T(""));
	hexString.Replace(_T("\r"), _T(""));
	hexString.Replace(_T("\n"), _T(""));

	// Grab valid hexadecimal characters
	hexString = hexString.SpanIncluding(_T("0123456789abcdefABCDEF"));

	// Make sure that there are an even number of hex characters
	if (hexString.GetLength() % 2 == 1)
	{
		// Insert an extra zero before the first character
		hexString = _T("0") + hexString;
	}

	// Iterate through each byte (2 hex characters)
	for (int i = 0; i < hexString.GetLength(); i += 2)
	{
		// Insert the decimal value into the vector
		CString byteString = hexString.Mid(i, 2);
		retVal.push_back((BYTE)_tcstoul(byteString, NULL, 16));
	}

	return retVal;
}

/////////////////////////////////////////////////////////////////////////////
// CDataTransferPage dialog
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDataTransferPage, CDialog)

CDataTransferPage::CDataTransferPage(HID_SMBUS_DEVICE* pHidSmbus, CWnd* pParent /*=NULL*/)
	: CDialog(CDataTransferPage::IDD, pParent)
	, m_pHidSmbus(pHidSmbus)
{
	SetDefaults();
}

CDataTransferPage::~CDataTransferPage()
{
}

void CDataTransferPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_TARGET_ADDRESS, m_targetAddressString);
	DDX_Text(pDX, IDC_EDIT_RECEIVE_DATA, m_receiveData);
	DDX_Text(pDX, IDC_EDIT_TRANSMIT_DATA, m_transmitData);

	// Call custom DDX and DDV functions from the CustomDDX module
	CustomDataExchange(pDX);
}

void CDataTransferPage::CustomDataExchange(CDataExchange* pDX)
{
	// Slave Address is an even 1-byte hex value in the
	// range [HID_SMBUS_MIN_ADDRESS, HID_SMBUS_MAX_ADDRESS]
	DDX_TextHex(pDX, IDC_EDIT_SLAVE_ADDRESS, m_slaveAddress);
	DDV_MaxCharsHex(pDX, m_slaveAddress);
	DDV_MinMaxHex(pDX, m_slaveAddress, HID_SMBUS_MIN_ADDRESS, HID_SMBUS_MAX_ADDRESS);
	DDV_Even(pDX, m_slaveAddress);

	// Bytes to read is a 2-byte decimal value in the
	// range [HID_SMBUS_MIN_READ_REQUEST_SIZE, HID_SMBUS_MAX_READ_REQUEST_SIZE]
	DDX_TextPlus(pDX, IDC_EDIT_BYTES_TO_READ, m_bytesToRead);
	DDV_MinMaxDWord(pDX, m_bytesToRead, HID_SMBUS_MIN_READ_REQUEST_SIZE, HID_SMBUS_MAX_READ_REQUEST_SIZE);

	// Addressed Read Slave Address is an even 1-byte hex value in the
	// range [HID_SMBUS_MIN_ADDRESS, HID_SMBUS_MAX_ADDRESS]
	DDX_TextHex(pDX, IDC_EDIT_ADDRESSED_SLAVE_ADDRESS, m_addressedSlaveAddress);
	DDV_MaxCharsHex(pDX, m_addressedSlaveAddress);
	DDV_MinMaxHex(pDX, m_addressedSlaveAddress, HID_SMBUS_MIN_ADDRESS, HID_SMBUS_MAX_ADDRESS);
	DDV_Even(pDX, m_addressedSlaveAddress);

	// Target Address Size is a 1-byte decimal value in the
	// range [HID_SMBUS_MIN_TARGET_ADDRESS_SIZE, HID_SMBUS_MAX_TARGET_ADDRESS_SIZE]
	DDX_TextPlus(pDX, IDC_EDIT_TARGET_ADDRESS_SIZE, m_targetAddressSize);
	DDV_MinMaxByte(pDX, m_targetAddressSize, HID_SMBUS_MIN_TARGET_ADDRESS_SIZE, HID_SMBUS_MAX_TARGET_ADDRESS_SIZE);

	// Addressed Read Bytes to Read is a 2-byte decimal value in the
	// range [HID_SMBUS_MIN_READ_REQUEST_SIZE, HID_SMBUS_MAX_READ_REQUEST_SIZE]
	DDX_TextPlus(pDX, IDC_EDIT_ADDRESSED_BYTES_TO_READ, m_addressedBytesToRead);
	DDV_MinMaxDWord(pDX, m_addressedBytesToRead, HID_SMBUS_MIN_READ_REQUEST_SIZE, HID_SMBUS_MAX_READ_REQUEST_SIZE);

	// Force Bytes to Read is a 2-byte decimal value in the
	// range [HID_SMBUS_MIN_READ_REQUEST_SIZE, HID_SMBUS_MAX_READ_REQUEST_SIZE]
	DDX_TextPlus(pDX, IDC_EDIT_FORCE_BYTES_TO_READ, m_forceBytesToRead);
	DDV_MinMaxDWord(pDX, m_forceBytesToRead, HID_SMBUS_MIN_READ_REQUEST_SIZE, HID_SMBUS_MAX_READ_REQUEST_SIZE);

	// Write Slave Address is an even 1-byte hex value in the
	// range [HID_SMBUS_MIN_ADDRESS, HID_SMBUS_MAX_ADDRESS]
	DDX_TextHex(pDX, IDC_EDIT_WRITE_SLAVE_ADDRESS, m_writeSlaveAddress);
	DDV_MaxCharsHex(pDX, m_writeSlaveAddress);
	DDV_Even(pDX, m_writeSlaveAddress);
}

/////////////////////////////////////////////////////////////////////////////
// CDataTransferPage - Public Methods
/////////////////////////////////////////////////////////////////////////////

// Set default control values
void CDataTransferPage::SetDefaults()
{
	m_slaveAddress			= 0xF0;
	m_bytesToRead			= 1;
	m_addressedSlaveAddress	= 0xF0;
	m_targetAddressSize		= 2;
	m_targetAddressString	= _T("0000");
	m_addressedBytesToRead	= 1;
	m_forceBytesToRead		= 1;
	m_receiveData			= _T("");
	m_transmitData			= _T("");
	m_writeSlaveAddress		= 0xF0;
}

// Set control values by getting the device configuration
void CDataTransferPage::SetFromDevice()
{
	// Set default control values
	SetDefaults();

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CDataTransferPage - Protected Methods
/////////////////////////////////////////////////////////////////////////////

// Return the target address as a 16-byte array
BOOL CDataTransferPage::GetTargetAddress(BYTE targetAddress[HID_SMBUS_MAX_TARGET_ADDRESS_SIZE])
{
	BOOL valid = FALSE;

	// Return a vector containing the target address bytes
	std::vector<BYTE> targetAddressVector = GetHexArray(m_targetAddressString);

	// Clear target address
	memset(targetAddress, 0x00, HID_SMBUS_MAX_TARGET_ADDRESS_SIZE);

	// Make sure that the user entered the correct number of bytes
	// for the target address
	if (targetAddressVector.size() == m_targetAddressSize)
	{
		// Copy target address to array
		for (size_t i = 0; i < targetAddressVector.size(); i++)
		{
			targetAddress[i] = targetAddressVector[i];
		}

		valid = TRUE;
	}
	else
	{
		// Display a warning if the target address isn't the correct size
		CString msg;
		msg.Format(_T("Please enter a %u-byte hexadecimal address."), m_targetAddressSize);
		MessageBox(msg, 0, MB_ICONWARNING);

		// Set focus to the target address control
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TARGET_ADDRESS);
		pEdit->SetFocus();
		pEdit->SetSel(0, -1);
	}

	return valid;
}

// Return the write data buffer as a 61-byte array
BOOL CDataTransferPage::GetWriteData(BYTE writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE], BYTE& size)
{
	BOOL valid = FALSE;

	// Return a vector containing the write data bytes
	std::vector<BYTE> writeDataVector = GetHexArray(m_transmitData);

	// Clear the write data buffer
	memset(writeData, 0x00, HID_SMBUS_MAX_WRITE_REQUEST_SIZE);

	// Validate the number of bytes to write
	if (writeDataVector.size() >= HID_SMBUS_MIN_WRITE_REQUEST_SIZE &&
		writeDataVector.size() <= HID_SMBUS_MAX_WRITE_REQUEST_SIZE)
	{
		// Copy write data to array
		for (size_t i = 0; i < writeDataVector.size(); i++)
		{
			writeData[i] = writeDataVector[i];
		}

		size	= (BYTE)writeDataVector.size();
		valid	= TRUE;
	}
	else
	{
		// Display a warning if the number of bytes to write is invalid
		CString msg;
		msg.Format(_T("Number of bytes to write must be between %d and %d bytes."), HID_SMBUS_MIN_WRITE_REQUEST_SIZE, HID_SMBUS_MAX_WRITE_REQUEST_SIZE);
		MessageBox(msg, 0, MB_ICONWARNING);

		// Set focus to the target address control
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TRANSMIT_DATA);
		pEdit->SetFocus();
		pEdit->SetSel(0, -1);
	}
	return valid;
}

void CDataTransferPage::ReadRequest()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		// Issue a read request
		HID_SMBUS_STATUS status = HidSmbus_ReadRequest(*m_pHidSmbus, m_slaveAddress, m_bytesToRead);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_ReadRequest"), status);
	}
}

void CDataTransferPage::AddressReadRequest()
{
	BYTE targetAddress[HID_SMBUS_MAX_TARGET_ADDRESS_SIZE];

	// Get target address array
	if (GetTargetAddress(targetAddress))
	{
		BOOL opened;

		// Make sure that the device is opened
		if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
		{
			// Issue an address read request
			HID_SMBUS_STATUS status = HidSmbus_AddressReadRequest(*m_pHidSmbus, m_addressedSlaveAddress, m_addressedBytesToRead, m_targetAddressSize, targetAddress);

			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_AddressReadRequest"), status);
		}
	}
}

void CDataTransferPage::ForceReadResponse()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		// Notify device that it should send a read response back
		HID_SMBUS_STATUS status = HidSmbus_ForceReadResponse(*m_pHidSmbus, m_forceBytesToRead);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_ForceReadResponse"), status);
	}
}

void CDataTransferPage::GetReadResponse()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		HID_SMBUS_S0	status0;
		BYTE			buffer[HID_SMBUS_MAX_READ_RESPONSE_SIZE];
		BYTE			numBytesRead = 0;

		// Wait for a read response
		HID_SMBUS_STATUS status = HidSmbus_GetReadResponse(*m_pHidSmbus, &status0, buffer, HID_SMBUS_MAX_READ_RESPONSE_SIZE, &numBytesRead);

		CString receiveString;

		// Show the received data in the receive data editbox
		if (status == HID_SMBUS_SUCCESS)
		{
			for (int i = 0; i < numBytesRead; i++)
			{
				receiveString.AppendFormat(_T("%02X "), buffer[i]);
			}
		}

		m_receiveData = receiveString;

		// Read response received successfully
		if (status == HID_SMBUS_SUCCESS)
		{
			// Update status bar text
			CString statusText;
			statusText.Format(_T("HidSmbus_GetReadResponse(): %s  Transfer Status: %s  Bytes Read: %u"), HidSmbus_DecodeErrorStatus(status).GetString(), HidSmbus_DecodeTransferStatus(status0).GetString(), numBytesRead);
			((CHidSmbusExampleDlg*)GetParent())->SetStatusText(statusText);
		}
		// Read response failed
		else
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_GetReadResponse"), status);
		}
	}
}

void CDataTransferPage::WriteRequest()
{
	BYTE writeData[HID_SMBUS_MAX_WRITE_REQUEST_SIZE];
	BYTE numBytesToWrite = 0;

	if (GetWriteData(writeData, numBytesToWrite))
	{
		BOOL opened;

		// Make sure that the device is opened
		if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
		{
			// Issue write transfer request
			HID_SMBUS_STATUS status = HidSmbus_WriteRequest(*m_pHidSmbus, m_writeSlaveAddress, writeData, numBytesToWrite);

			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_WriteRequest"), status);
		}
	}
}

void CDataTransferPage::CancelTransfer()

{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		// Cancel pending transfer
		HID_SMBUS_STATUS status = HidSmbus_CancelTransfer(*m_pHidSmbus);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_CancelTransfer"), status);
	}
}

void CDataTransferPage::GetTransferStatus()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		// Issue transfer status request
		HID_SMBUS_STATUS status = HidSmbus_TransferStatusRequest(*m_pHidSmbus);

		// Transfer status request was successful
		if (status == HID_SMBUS_SUCCESS)
		{
			HID_SMBUS_S0	status0;
			HID_SMBUS_S1	status1;
			WORD			numRetries;
			WORD			bytesRead;

			// Wait for transfer status response
			status = HidSmbus_GetTransferStatusResponse(*m_pHidSmbus, &status0, &status1, &numRetries, &bytesRead);

			// Transfer status response received successfully
			if (status == HID_SMBUS_SUCCESS)
			{
				// Update status bar text
				CString statusText;
				statusText.Format(_T("Transfer Status: %s  Retries: %u  Bytes Read: %u"), HidSmbus_DecodeTransferStatus(status0, status1).GetString(), numRetries, bytesRead);
				((CHidSmbusExampleDlg*)GetParent())->SetStatusText(statusText);
			}
			// Transfer status response failed
			else
			{
				// Output status to status bar
				// And play an audible alert if the status is not HID_SMBUS_SUCCESS
				OutputStatus(_T("HidSmbus_GetTransferStatusResponse"), status);
			}
		}
		// Transfer status request failed
		else
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_TransferStatusRequest"), status);
		}
	}
}

// Output status to status bar
// And play an audible alert if the status is not HID_SMBUS_SUCCESS
void CDataTransferPage::OutputStatus(CString functionName, HID_SMBUS_STATUS status)
{
	CString statusText;

	// Update status bar text
	statusText.Format(_T("%s(): %s"), functionName.GetString(), HidSmbus_DecodeErrorStatus(status).GetString());
	((CHidSmbusExampleDlg*)GetParent())->SetStatusText(statusText);

	// Alert user if the function failed
	if (status != HID_SMBUS_SUCCESS)
	{
		MessageBeep(MB_ICONHAND);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDataTransferPage - Message Handlers
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDataTransferPage, CDialog)
	ON_BN_CLICKED(IDOK, &CDataTransferPage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDataTransferPage::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_READ_REQUEST, &CDataTransferPage::OnBnClickedButtonReadRequest)
	ON_BN_CLICKED(IDC_BUTTON_ADDRESS_READ_REQUEST, &CDataTransferPage::OnBnClickedButtonAddressReadRequest)
	ON_BN_CLICKED(IDC_BUTTON_FORCE_READ_RESPONSE, &CDataTransferPage::OnBnClickedButtonForceReadResponse)
	ON_BN_CLICKED(IDC_BUTTON_GET_READ_RESPONSE, &CDataTransferPage::OnBnClickedButtonGetReadResponse)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_REQUEST, &CDataTransferPage::OnBnClickedButtonWriteRequest)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_TRANSFER, &CDataTransferPage::OnBnClickedButtonCancelTransfer)
	ON_BN_CLICKED(IDC_BUTTON_GET_TRANSFER_STATUS, &CDataTransferPage::OnBnClickedButtonGetTransferStatus)
END_MESSAGE_MAP()

// Override enter and cancel keys so that the property
// pages aren't closed
void CDataTransferPage::OnBnClickedOk()
{
}

// Override enter and cancel keys so that the property
// pages aren't closed
void CDataTransferPage::OnBnClickedCancel()
{
}

void CDataTransferPage::OnBnClickedButtonReadRequest()
{
	if (UpdateData(TRUE))
	{
		ReadRequest();
	}
}

void CDataTransferPage::OnBnClickedButtonAddressReadRequest()
{
	if (UpdateData(TRUE))
	{
		AddressReadRequest();
	}
}

void CDataTransferPage::OnBnClickedButtonForceReadResponse()
{
	if (UpdateData(TRUE))
	{
		ForceReadResponse();
	}
}

void CDataTransferPage::OnBnClickedButtonGetReadResponse()
{
	if (UpdateData(TRUE))
	{
		GetReadResponse();
		UpdateData(FALSE);
	}
}

void CDataTransferPage::OnBnClickedButtonWriteRequest()
{
	if (UpdateData(TRUE))
	{
		WriteRequest();
	}
}

void CDataTransferPage::OnBnClickedButtonCancelTransfer()
{
	if (UpdateData(TRUE))
	{
		CancelTransfer();
	}
}

void CDataTransferPage::OnBnClickedButtonGetTransferStatus()
{
	if (UpdateData(TRUE))
	{
		GetTransferStatus();
	}
}
