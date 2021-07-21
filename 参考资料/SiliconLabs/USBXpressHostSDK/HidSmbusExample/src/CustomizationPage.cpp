/////////////////////////////////////////////////////////////////////////////
// CustomizationPage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HidSmbusExample.h"
#include "CustomizationPage.h"
#include "HidSmbusExampleDlg.h"
#include "CustomDDX.h"
#include "Utilities.h"

/////////////////////////////////////////////////////////////////////////////
// CCustomizationPage dialog
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CCustomizationPage, CDialog)

CCustomizationPage::CCustomizationPage(HID_SMBUS_DEVICE* pHidSmbus, CWnd* pParent /*=NULL*/)
	: CDialog(CCustomizationPage::IDD, pParent)
	, m_pHidSmbus(pHidSmbus)
{
	SetDefaults();
}

CCustomizationPage::~CCustomizationPage()
{
}

void CCustomizationPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO_BUS_POWER, m_powerMode);
	DDX_Text(pDX, IDC_EDIT_MANUFACTURER, m_manufacturer);
	DDV_MaxChars(pDX, m_manufacturer, HID_SMBUS_CP2112_MFG_STRLEN);
	DDX_Text(pDX, IDC_EDIT_PRODUCT, m_product);
	DDV_MaxChars(pDX, m_product, HID_SMBUS_CP2112_PRODUCT_STRLEN);
	DDX_Text(pDX, IDC_EDIT_SERIAL, m_serial);
	DDV_MaxChars(pDX, m_serial, HID_SMBUS_CP2112_SERIAL_STRLEN);

	// Call custom DDX and DDV functions from the CustomDDX module
	CustomDDX(pDX);
}

void CCustomizationPage::CustomDDX(CDataExchange* pDX)
{
	// VID is a 2-byte hex value
	DDX_TextHex(pDX, IDC_EDIT_VID, m_vid);
	DDV_MaxCharsHex(pDX, m_vid);

	// PID is a 2-byte hex value
	DDX_TextHex(pDX, IDC_EDIT_PID, m_pid);
	DDV_MaxCharsHex(pDX, m_pid);

	// Power is an even 2-byte decimal value in the
	// range [0, HID_SMBUS_BUS_POWER_MAX*2]
	DDX_TextPlus(pDX, IDC_EDIT_POWER, m_power);
	DDV_MinMaxDWord(pDX, m_power, 0, HID_SMBUS_BUS_POWER_MAX*2);
	DDV_Even(pDX, m_power);

	// Release Version MSB is a 1-byte decimal value
	DDX_TextPlus(pDX, IDC_EDIT_RELEASE_VERSION_MSB, m_releaseVersionMSB);

	// Release Version LSB is a 1-byte decimal value
	DDX_TextPlus(pDX, IDC_EDIT_RELEASE_VERSION_LSB, m_releaseVersionLSB);
}

/////////////////////////////////////////////////////////////////////////////
// CCustomizationPage - Public Methods
/////////////////////////////////////////////////////////////////////////////

// Set default control values
void CCustomizationPage::SetDefaults()
{
	m_vid				= 0x10C4;
	m_pid				= 0xea90;
	m_power				= 100;
	m_powerMode			= 0;
	m_releaseVersionMSB	= 1;
	m_releaseVersionLSB	= 0;
	m_manufacturer		= _T("Silicon Laboratories");
	m_product			= _T("CP2112 HID USB-to-SMBus Bridge");
	m_serial			= _T("0001");
}

// Set control values by getting the device configuration
void CCustomizationPage::SetFromDevice()
{
	// Set default control values
	SetDefaults();

	// Update control values
	GetUsbConfig(TRUE);
	GetManufacturer(TRUE);
	GetProduct(TRUE);
	GetSerial(TRUE);
	GetLock(TRUE);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CCustomizationPage - Protected Methods
/////////////////////////////////////////////////////////////////////////////

void CCustomizationPage::GetUsbConfig(BOOL silent)
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		BYTE power;
		BYTE powerMode;
		WORD releaseVersion;

		// Get USB Configuration
		HID_SMBUS_STATUS status = HidSmbus_GetUsbConfig(*m_pHidSmbus, &m_vid, &m_pid, &power, &powerMode, &releaseVersion);

		if (status == HID_SMBUS_SUCCESS)
		{
			m_power					= power * 2;
			m_powerMode				= powerMode;
			m_releaseVersionMSB		= HIBYTE(releaseVersion);
			m_releaseVersionLSB		= LOBYTE(releaseVersion);
		}

		if (!silent)
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_GetUsbConfig"), status);
		}
	}
}

void CCustomizationPage::SetUsbConfig()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		BYTE mask = 0x00;

		// Calculate USB configuration variable values
		const BYTE power = (BYTE) (m_power / 2);
		const BYTE powerMode = (BYTE)m_powerMode;
		const WORD releaseVersion = MAKEWORD(m_releaseVersionLSB, m_releaseVersionMSB);

		// Initialize the mask to decide which values will be programmed
		if (IsDlgButtonChecked(IDC_CHECK_VID))				mask |= HID_SMBUS_SET_VID;
		if (IsDlgButtonChecked(IDC_CHECK_PID))				mask |= HID_SMBUS_SET_PID;
		if (IsDlgButtonChecked(IDC_CHECK_POWER))			mask |= HID_SMBUS_SET_POWER;
		if (IsDlgButtonChecked(IDC_CHECK_POWER_MODE))		mask |= HID_SMBUS_SET_POWER_MODE;
		if (IsDlgButtonChecked(IDC_CHECK_RELEASE_VERSION))	mask |= HID_SMBUS_SET_RELEASE_VERSION;

		// Set USB Configuration
		HID_SMBUS_STATUS status = HidSmbus_SetUsbConfig(*m_pHidSmbus, m_vid, m_pid, power, powerMode, releaseVersion, mask);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_SetUsbConfig"), status);
	}
}

void CCustomizationPage::GetManufacturer(BOOL silent)
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		HID_SMBUS_CP2112_MFG_STR	manufacturingString;
		BYTE						strlen					= 0;

		// Get manufacturer string
		HID_SMBUS_STATUS status = HidSmbus_GetManufacturingString(*m_pHidSmbus, manufacturingString, &strlen);

		if (status == HID_SMBUS_SUCCESS)
		{
			m_manufacturer = CString(manufacturingString).Left(strlen);
		}

		if (!silent)
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_GetManufacturingString"), status);
		}
	}
}

void CCustomizationPage::SetManufacturer()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		HID_SMBUS_CP2112_MFG_STR manufacturingString;
		const int strlen = m_manufacturer.GetLength();	// count of bytes in m_manufacturer not including null terminator

		for (int i = 0; i < strlen; i++)	// TODO: Buffer overrun: What if m_manufacturer.GetLength() > sizeof manufacturingString?
		{
			// Convert Unicode CString to ASCII character array
			manufacturingString[i] = (char)m_manufacturer[i];
		}

		// Set manufacturer string
		HID_SMBUS_STATUS status = HidSmbus_SetManufacturingString(*m_pHidSmbus, manufacturingString, (BYTE) strlen);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_SetManufacturingString"), status);
	}
}

void CCustomizationPage::GetProduct(BOOL silent)
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		HID_SMBUS_CP2112_PRODUCT_STR	productString;
		BYTE							strlen			= 0;

		// Get product string
		HID_SMBUS_STATUS status = HidSmbus_GetProductString(*m_pHidSmbus, productString, &strlen);

		if (status == HID_SMBUS_SUCCESS)
		{
			m_product = CString(productString).Left(strlen);
		}

		if (!silent)
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_GetProductString"), status);
		}
	}
}

void CCustomizationPage::SetProduct()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		HID_SMBUS_CP2112_PRODUCT_STR productString;
		const int strlen = m_product.GetLength();

		for (int i = 0; i < strlen; i++)
		{
			// Convert Unicode CString to ASCII character array
			productString[i] = (char)m_product[i];
		}

		// Set product string
		HID_SMBUS_STATUS status = HidSmbus_SetProductString(*m_pHidSmbus, productString, (BYTE) strlen);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_SetProductString"), status);
	}
}

void CCustomizationPage::GetSerial(BOOL silent)
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		HID_SMBUS_CP2112_SERIAL_STR		serialString;
		BYTE							strlen			= 0;

		// Get serial string
		HID_SMBUS_STATUS status = HidSmbus_GetSerialString(*m_pHidSmbus, serialString, &strlen);

		if (status == HID_SMBUS_SUCCESS)
		{
			m_serial = CString(serialString).Left(strlen);
		}

		if (!silent)
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_GetSerialString"), status);
		}
	}
}

void CCustomizationPage::SetSerial()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		HID_SMBUS_CP2112_SERIAL_STR serialString;
		const int strlen = m_serial.GetLength();

		for (int i = 0; i < strlen; i++)
		{
			// Convert Unicode CString to ASCII character array
			serialString[i] = (char)m_serial[i];
		}

		// Set serial string
		HID_SMBUS_STATUS status = HidSmbus_SetSerialString(*m_pHidSmbus, serialString, (BYTE) strlen);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_SetSerialString"), status);
	}
}

void CCustomizationPage::GetLock(BOOL silent)
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		BYTE lock = 0x00;

		// Get lock byte
		HID_SMBUS_STATUS status = HidSmbus_GetLock(*m_pHidSmbus, &lock);

		if (status == HID_SMBUS_SUCCESS)
		{
			// Check the field lock checkbox if the field is unlocked
			// Once a field is locked, it cannot be unlocked
			if (lock & HID_SMBUS_LOCK_VID)				CheckDlgButton(IDC_CHECK_LOCK_VID, TRUE);
			else										CheckDlgButton(IDC_CHECK_LOCK_VID, FALSE);
			if (lock & HID_SMBUS_LOCK_PID)				CheckDlgButton(IDC_CHECK_LOCK_PID, TRUE);
			else										CheckDlgButton(IDC_CHECK_LOCK_PID, FALSE);
			if (lock & HID_SMBUS_LOCK_POWER)			CheckDlgButton(IDC_CHECK_LOCK_POWER, TRUE);
			else										CheckDlgButton(IDC_CHECK_LOCK_POWER, FALSE);
			if (lock & HID_SMBUS_LOCK_POWER_MODE)		CheckDlgButton(IDC_CHECK_LOCK_POWER_MODE, TRUE);
			else										CheckDlgButton(IDC_CHECK_LOCK_POWER_MODE, FALSE);
			if (lock & HID_SMBUS_LOCK_RELEASE_VERSION)	CheckDlgButton(IDC_CHECK_LOCK_RELEASE_VERSION, TRUE);
			else										CheckDlgButton(IDC_CHECK_LOCK_RELEASE_VERSION, FALSE);
			if (lock & HID_SMBUS_LOCK_MFG_STR)			CheckDlgButton(IDC_CHECK_LOCK_MANUFACTURER, TRUE);
			else										CheckDlgButton(IDC_CHECK_LOCK_MANUFACTURER, FALSE);
			if (lock & HID_SMBUS_LOCK_PRODUCT_STR)		CheckDlgButton(IDC_CHECK_LOCK_PRODUCT, TRUE);
			else										CheckDlgButton(IDC_CHECK_LOCK_PRODUCT, FALSE);
			if (lock & HID_SMBUS_LOCK_SERIAL_STR)		CheckDlgButton(IDC_CHECK_LOCK_SERIAL, TRUE);
			else										CheckDlgButton(IDC_CHECK_LOCK_SERIAL, FALSE);
		}

		if (!silent)
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_GetLock"), status);
		}
	}
}

void CCustomizationPage::SetLock()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		BYTE lock = 0x00;

		// Uncheck the field lock checkbox to prevent further programming
		// of that field
		//
		// Once a field is locked, it cannot be unlocked
		if (IsDlgButtonChecked(IDC_CHECK_LOCK_VID))					lock |= HID_SMBUS_LOCK_VID;
		if (IsDlgButtonChecked(IDC_CHECK_LOCK_PID))					lock |= HID_SMBUS_LOCK_PID;
		if (IsDlgButtonChecked(IDC_CHECK_LOCK_POWER))				lock |= HID_SMBUS_LOCK_POWER;
		if (IsDlgButtonChecked(IDC_CHECK_LOCK_POWER_MODE))			lock |= HID_SMBUS_LOCK_POWER_MODE;
		if (IsDlgButtonChecked(IDC_CHECK_LOCK_RELEASE_VERSION))		lock |= HID_SMBUS_LOCK_RELEASE_VERSION;
		if (IsDlgButtonChecked(IDC_CHECK_LOCK_MANUFACTURER))		lock |= HID_SMBUS_LOCK_MFG_STR;
		if (IsDlgButtonChecked(IDC_CHECK_LOCK_PRODUCT))				lock |= HID_SMBUS_LOCK_PRODUCT_STR;
		if (IsDlgButtonChecked(IDC_CHECK_LOCK_SERIAL))				lock |= HID_SMBUS_LOCK_SERIAL_STR;

		// Set lock byte
		HID_SMBUS_STATUS status = HidSmbus_SetLock(*m_pHidSmbus, lock);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_SetLock"), status);
	}
}

// Output status to status bar
// And play an audible alert if the status is not HID_SMBUS_SUCCESS
void CCustomizationPage::OutputStatus(CString functionName, HID_SMBUS_STATUS status)
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
// CCustomizationPage - Message Handlers
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCustomizationPage, CDialog)
	ON_BN_CLICKED(IDOK, &CCustomizationPage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCustomizationPage::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_GET_USB_CONFIG, &CCustomizationPage::OnBnClickedButtonGetUsbConfig)
	ON_BN_CLICKED(IDC_BUTTON_SET_USB_CONFIG, &CCustomizationPage::OnBnClickedButtonSetUsbConfig)
	ON_BN_CLICKED(IDC_BUTTON_GET_MANUFACTURER, &CCustomizationPage::OnBnClickedButtonGetManufacturer)
	ON_BN_CLICKED(IDC_BUTTON_SET_MANUFACTURER, &CCustomizationPage::OnBnClickedButtonSetManufacturer)
	ON_BN_CLICKED(IDC_BUTTON_GET_PRODUCT, &CCustomizationPage::OnBnClickedButtonGetProduct)
	ON_BN_CLICKED(IDC_BUTTON_SET_PRODUCT, &CCustomizationPage::OnBnClickedButtonSetProduct)
	ON_BN_CLICKED(IDC_BUTTON_GET_SERIAL, &CCustomizationPage::OnBnClickedButtonGetSerial)
	ON_BN_CLICKED(IDC_BUTTON_SET_SERIAL, &CCustomizationPage::OnBnClickedButtonSetSerial)
	ON_BN_CLICKED(IDC_BUTTON_GET_LOCK, &CCustomizationPage::OnBnClickedButtonGetLock)
	ON_BN_CLICKED(IDC_BUTTON_SET_LOCK, &CCustomizationPage::OnBnClickedButtonSetLock)
END_MESSAGE_MAP()

// Override enter and cancel keys so that the property
// pages aren't closed
void CCustomizationPage::OnBnClickedOk()
{
}

// Override enter and cancel keys so that the property
// pages aren't closed
void CCustomizationPage::OnBnClickedCancel()
{
}

void CCustomizationPage::OnBnClickedButtonGetUsbConfig()
{
	if (UpdateData(TRUE))
	{
		GetUsbConfig();
		UpdateData(FALSE);
	}
}

void CCustomizationPage::OnBnClickedButtonSetUsbConfig()
{
	if (UpdateData(TRUE))
	{
		SetUsbConfig();
	}
}

void CCustomizationPage::OnBnClickedButtonGetManufacturer()
{
	if (UpdateData(TRUE))
	{
		GetManufacturer();
		UpdateData(FALSE);
	}
}

void CCustomizationPage::OnBnClickedButtonSetManufacturer()
{
	if (UpdateData(TRUE))
	{
		SetManufacturer();
	}
}

void CCustomizationPage::OnBnClickedButtonGetProduct()
{
	if (UpdateData(TRUE))
	{
		GetProduct();
		UpdateData(FALSE);
	}
}

void CCustomizationPage::OnBnClickedButtonSetProduct()
{
	if (UpdateData(TRUE))
	{
		SetProduct();
	}
}

void CCustomizationPage::OnBnClickedButtonGetSerial()
{
	if (UpdateData(TRUE))
	{
		GetSerial();
		UpdateData(FALSE);
	}
}

void CCustomizationPage::OnBnClickedButtonSetSerial()
{
	if (UpdateData(TRUE))
	{
		SetSerial();
	}
}

void CCustomizationPage::OnBnClickedButtonGetLock()
{
	if (UpdateData(TRUE))
	{
		GetLock();
	}
}

void CCustomizationPage::OnBnClickedButtonSetLock()
{
	if (UpdateData(TRUE))
	{
		SetLock();
	}
}
