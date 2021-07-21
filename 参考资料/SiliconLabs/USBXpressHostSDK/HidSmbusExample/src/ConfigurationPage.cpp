/////////////////////////////////////////////////////////////////////////////
// ConfigurationPage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HidSmbusExample.h"
#include "ConfigurationPage.h"
#include "HidSmbusExampleDlg.h"
#include "CustomDDX.h"
#include "Utilities.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigurationPage dialog
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CConfigurationPage, CDialog)

CConfigurationPage::CConfigurationPage(HID_SMBUS_DEVICE* pHidSmbus, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigurationPage::IDD, pParent)
	, m_pHidSmbus(pHidSmbus)
{
	SetDefaults();
}

CConfigurationPage::~CConfigurationPage()
{
}

void CConfigurationPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_AUTO_RESPOND, m_autoRespond);
	DDX_Check(pDX, IDC_CHECK_SCL_LOW_TIMEOUT, m_sclLowTimeout);

	// Call custom DDX and DDV functions from the CustomDDX module
	CustomDataExchange(pDX);
}

void CConfigurationPage::CustomDataExchange(CDataExchange* pDX)
{
	// Bit Rate is a 4-byte decimal value in the
	// range [HID_SMBUS_MIN_BIT_RATE, 0xFFFFFFFF]
	DDX_TextPlus(pDX, IDC_EDIT_BIT_RATE, m_bitRate);
	DDV_MinMaxDWord(pDX, m_bitRate, HID_SMBUS_MIN_BIT_RATE, 0xFFFFFFFF);

	// Master's Slave Address is an even 1-byte hex value in the
	// range [HID_SMBUS_MIN_ADDRESS, HID_SMBUS_MAX_ADDRESS]
	DDX_TextHex(pDX, IDC_EDIT_ACK_ADDRESS, m_ackAddress);
	DDV_MaxCharsHex(pDX, m_ackAddress);
	DDV_MinMaxHex(pDX, m_ackAddress, HID_SMBUS_MIN_ADDRESS, HID_SMBUS_MAX_ADDRESS);
	DDV_Even(pDX, m_ackAddress);

	// Write Timeout is a 2-byte decimal value in the
	// range [HID_SMBUS_MIN_TIMEOUT, HID_SMBUS_MAX_TIMEOUT]
	DDX_TextPlus(pDX, IDC_EDIT_WRITE_TIMEOUT, m_writeTimeout);
	DDV_MinMaxDWord(pDX, m_writeTimeout, HID_SMBUS_MIN_TIMEOUT, HID_SMBUS_MAX_TIMEOUT);

	// Read Timeout is a 2-byte decimal value in the
	// range [HID_SMBUS_MIN_TIMEOUT, HID_SMBUS_MAX_TIMEOUT]
	DDX_TextPlus(pDX, IDC_EDIT_READ_TIMEOUT, m_readTimeout);
	DDV_MinMaxDWord(pDX, m_readTimeout, HID_SMBUS_MIN_TIMEOUT, HID_SMBUS_MAX_TIMEOUT);

	// Transfer Retries is a 2-byte decimal value in the
	// range [0, HID_SMBUS_MAX_RETRIES]
	DDX_TextPlus(pDX, IDC_EDIT_TRANSFER_RETRIES, m_transferRetries);
	DDV_MinMaxDWord(pDX, m_transferRetries, 0, HID_SMBUS_MAX_RETRIES);

	// Response timeout is a 4-byte decimal value
	DDX_TextPlus(pDX, IDC_EDIT_RESPONSE_TIMEOUT, m_responseTimeout);
}

/////////////////////////////////////////////////////////////////////////////
// CConfigurationPage - Public Methods
/////////////////////////////////////////////////////////////////////////////

// Set default control values
void CConfigurationPage::SetDefaults()
{
	m_bitRate			= 100000;
	m_ackAddress		= 0x02;
	m_autoRespond		= FALSE;
	m_writeTimeout		= 0;
	m_readTimeout		= 0;
	m_transferRetries	= 0;
	m_sclLowTimeout		= 0;
	m_responseTimeout	= 1000;
}

// Set control values by getting the device configuration
void CConfigurationPage::SetFromDevice()
{
	// Set default control values
	SetDefaults();

	// Update control values
	GetSmbusConfig(TRUE);
	GetTimeout(TRUE);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CConfigurationPage - Protected Methods
/////////////////////////////////////////////////////////////////////////////

void CConfigurationPage::GetSmbusConfig(BOOL silent)
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		// Get the SMBus configuration
		HID_SMBUS_STATUS status = HidSmbus_GetSmbusConfig(*m_pHidSmbus, &m_bitRate, &m_ackAddress, &m_autoRespond, &m_writeTimeout, &m_readTimeout, &m_sclLowTimeout, &m_transferRetries);

		if (!silent)
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_GetSmbusConfig"), status);
		}
	}
}

void CConfigurationPage::SetSmbusConfig()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		// Set the SMBus configuration
		HID_SMBUS_STATUS status = HidSmbus_SetSmbusConfig(*m_pHidSmbus, m_bitRate, m_ackAddress, m_autoRespond, m_writeTimeout, m_readTimeout, m_sclLowTimeout, m_transferRetries);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_SetSmbusConfig"), status);
	}
}

void CConfigurationPage::GetTimeout(BOOL silent)
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		// Get response timeouts
		HID_SMBUS_STATUS status = HidSmbus_GetTimeouts(*m_pHidSmbus, &m_responseTimeout);

		if (!silent)
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_GetTimeouts"), status);
		}
	}
}

void CConfigurationPage::SetTimeout()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		// Set response timeouts
		HID_SMBUS_STATUS status = HidSmbus_SetTimeouts(*m_pHidSmbus, m_responseTimeout);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_SetTimeouts"), status);
	}
}

// Output status to status bar
// And play an audible alert if the status is not HID_SMBUS_SUCCESS
void CConfigurationPage::OutputStatus(CString functionName, HID_SMBUS_STATUS status)
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
// CConfigurationPage - Message Handlers
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CConfigurationPage, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigurationPage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConfigurationPage::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_GET_SMBUS_CONFIG, &CConfigurationPage::OnBnClickedButtonGetSmbusConfig)
	ON_BN_CLICKED(IDC_BUTTON_SET_SMBUS_CONFIG, &CConfigurationPage::OnBnClickedButtonSetSmbusConfig)
	ON_BN_CLICKED(IDC_BUTTON_GET_TIMEOUT, &CConfigurationPage::OnBnClickedButtonGetTimeout)
	ON_BN_CLICKED(IDC_BUTTON_SET_TIMEOUT, &CConfigurationPage::OnBnClickedButtonSetTimeout)
END_MESSAGE_MAP()

// Override enter and cancel keys so that the property
// pages aren't closed
void CConfigurationPage::OnBnClickedOk()
{
}

// Override enter and cancel keys so that the property
// pages aren't closed
void CConfigurationPage::OnBnClickedCancel()
{
}

void CConfigurationPage::OnBnClickedButtonGetSmbusConfig()
{
	if (UpdateData(TRUE))
	{
		GetSmbusConfig();
		UpdateData(FALSE);
	}
}

void CConfigurationPage::OnBnClickedButtonSetSmbusConfig()
{
	if (UpdateData(TRUE))
	{
		SetSmbusConfig();
	}
}

void CConfigurationPage::OnBnClickedButtonGetTimeout()
{
	if (UpdateData(TRUE))
	{
		GetTimeout();
		UpdateData(FALSE);
	}
}

void CConfigurationPage::OnBnClickedButtonSetTimeout()
{
	if (UpdateData(TRUE))
	{
		SetTimeout();
	}
}
