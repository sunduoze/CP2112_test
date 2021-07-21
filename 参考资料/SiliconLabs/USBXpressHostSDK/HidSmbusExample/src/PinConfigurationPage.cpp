/////////////////////////////////////////////////////////////////////////////
// PinConfigurationPage.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HidSmbusExample.h"
#include "PinConfigurationPage.h"
#include "HidSmbusExampleDlg.h"
#include "CustomDDX.h"
#include "Utilities.h"

/////////////////////////////////////////////////////////////////////////////
// CPinConfigurationPage dialog
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPinConfigurationPage, CDialog)

CPinConfigurationPage::CPinConfigurationPage(HID_SMBUS_DEVICE* pHidSmbus, CWnd* pParent /*=NULL*/)
	: CDialog(CPinConfigurationPage::IDD, pParent)
	, m_pHidSmbus(pHidSmbus)
{
	SetDefaults();
}

CPinConfigurationPage::~CPinConfigurationPage()
{
}

void CPinConfigurationPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	// Call custom DDX and DDV functions from CustomDDX module
	CustomDataExchange(pDX);
}

void CPinConfigurationPage::CustomDataExchange(CDataExchange* pDX)
{
	// Clock Divider is a 1-byte decimal value
	DDX_TextPlus(pDX, IDC_EDIT_CLK_DIV, m_clkDiv);

	if (!pDX->m_bSaveAndValidate)
	{
		// Update clock output frequency in Hz
		OnEnChangeEditClkDiv();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPinConfigurationPage - Public Methods
/////////////////////////////////////////////////////////////////////////////

// Enable/disable the GPIO direction and mode checkboxes
// based on the selected settings
void CPinConfigurationPage::UpdateControlStates()
{
	CString caption;

	BOOL directionEnable[8]		= {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
	BOOL modeEnable[8]			= {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};

	// Disable the GPIO direction and mode checkboxes if the pin is configured
	// in special function mode
	if (IsDlgButtonChecked(IDC_CHECK_CLK_OUTPUT))
	{
		directionEnable[7]	= FALSE;
		modeEnable[7]		= FALSE;
		SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_7,	_T("Output"));
		SetDlgItemText(IDC_CHECK_MODE_GPIO_7,		_T("Push-Pull"));
	}
	if (IsDlgButtonChecked(IDC_CHECK_TX_TOGGLE))
	{
		directionEnable[0]	= FALSE;
		modeEnable[0]		= FALSE;
		SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_0,	_T("Output"));
		SetDlgItemText(IDC_CHECK_MODE_GPIO_0,		_T("Push-Pull"));
	}
	if (IsDlgButtonChecked(IDC_CHECK_RX_TOGGLE))
	{
		directionEnable[1]	= FALSE;
		modeEnable[1]		= FALSE;
		SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_1,	_T("Output"));
		SetDlgItemText(IDC_CHECK_MODE_GPIO_1,		_T("Push-Pull"));
	}

	// Disable the GPIO mode checkbox if the direction is configured as input
	// (We will force the mode to be Open-Drain)
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_0, caption);
	if (caption == _T("Input"))
	{
		modeEnable[0] = FALSE;
		SetDlgItemText(IDC_CHECK_MODE_GPIO_0, _T("Open-Drain"));
	}
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_1, caption);
	if (caption == _T("Input"))
	{
		modeEnable[1] = FALSE;
		SetDlgItemText(IDC_CHECK_MODE_GPIO_1, _T("Open-Drain"));
	}
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_2, caption);
	if (caption == _T("Input"))
	{
		modeEnable[2] = FALSE;
		SetDlgItemText(IDC_CHECK_MODE_GPIO_2, _T("Open-Drain"));
	}
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_3, caption);
	if (caption == _T("Input"))
	{
		modeEnable[3] = FALSE;
		SetDlgItemText(IDC_CHECK_MODE_GPIO_3, _T("Open-Drain"));
	}
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_4, caption);
	if (caption == _T("Input"))
	{
		modeEnable[4] = FALSE;
		SetDlgItemText(IDC_CHECK_MODE_GPIO_4, _T("Open-Drain"));
	}
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_5, caption);
	if (caption == _T("Input"))
	{
		modeEnable[5] = FALSE;
		SetDlgItemText(IDC_CHECK_MODE_GPIO_5, _T("Open-Drain"));
	}
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_6, caption);
	if (caption == _T("Input"))
	{
		modeEnable[6] = FALSE;
		SetDlgItemText(IDC_CHECK_MODE_GPIO_6, _T("Open-Drain"));
	}
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_7, caption);
	if (caption == _T("Input"))
	{
		modeEnable[7] = FALSE;
		SetDlgItemText(IDC_CHECK_MODE_GPIO_7, _T("Open-Drain"));
	}

	for (int i = 0; i < 8; i++)
	{
		GetDlgItem(IDC_CHECK_DIRECTION_GPIO_0 + i)->EnableWindow(directionEnable[i]);
		GetDlgItem(IDC_CHECK_MODE_GPIO_0 + i)->EnableWindow(modeEnable[i]);
	}
}

// Set default control values
void CPinConfigurationPage::SetDefaults()
{
	m_clkDiv = 0;
}

// Set control values by getting the device configuration
void CPinConfigurationPage::SetFromDevice()
{
	// Set default control values
	SetDefaults();

	// Update control values
	GetGpioConfig(TRUE);
	ReadLatch(TRUE);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CPinConfigurationPage - Protected Methods
/////////////////////////////////////////////////////////////////////////////

// Update control captions using direction bitmask
void CPinConfigurationPage::SetDirection(BYTE direction)
{
	if (direction & HID_SMBUS_MASK_GPIO_0)	SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_0, _T("Output"));
	else									SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_0, _T("Input"));
	if (direction & HID_SMBUS_MASK_GPIO_1)	SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_1, _T("Output"));
	else									SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_1, _T("Input"));
	if (direction & HID_SMBUS_MASK_GPIO_2)	SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_2, _T("Output"));
	else									SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_2, _T("Input"));
	if (direction & HID_SMBUS_MASK_GPIO_3)	SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_3, _T("Output"));
	else									SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_3, _T("Input"));
	if (direction & HID_SMBUS_MASK_GPIO_4)	SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_4, _T("Output"));
	else									SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_4, _T("Input"));
	if (direction & HID_SMBUS_MASK_GPIO_5)	SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_5, _T("Output"));
	else									SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_5, _T("Input"));
	if (direction & HID_SMBUS_MASK_GPIO_6)	SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_6, _T("Output"));
	else									SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_6, _T("Input"));
	if (direction & HID_SMBUS_MASK_GPIO_7)	SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_7, _T("Output"));
	else									SetDlgItemText(IDC_CHECK_DIRECTION_GPIO_7, _T("Input"));
}

// Get direction bitmask from control state
BYTE CPinConfigurationPage::GetDirection()
{
	BYTE		direction = 0x00;
	CString		caption;

	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_0, caption);
	if (caption == _T("Output"))	direction |= HID_SMBUS_MASK_GPIO_0;
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_1, caption);
	if (caption == _T("Output"))	direction |= HID_SMBUS_MASK_GPIO_1;
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_2, caption);
	if (caption == _T("Output"))	direction |= HID_SMBUS_MASK_GPIO_2;
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_3, caption);
	if (caption == _T("Output"))	direction |= HID_SMBUS_MASK_GPIO_3;
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_4, caption);
	if (caption == _T("Output"))	direction |= HID_SMBUS_MASK_GPIO_4;
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_5, caption);
	if (caption == _T("Output"))	direction |= HID_SMBUS_MASK_GPIO_5;
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_6, caption);
	if (caption == _T("Output"))	direction |= HID_SMBUS_MASK_GPIO_6;
	GetDlgItemText(IDC_CHECK_DIRECTION_GPIO_7, caption);
	if (caption == _T("Output"))	direction |= HID_SMBUS_MASK_GPIO_7;

	return direction;
}

// Update control captions using mode bitmask
void CPinConfigurationPage::SetMode(BYTE mode)
{
	if (mode & HID_SMBUS_MASK_GPIO_0)	SetDlgItemText(IDC_CHECK_MODE_GPIO_0, _T("Push-Pull"));
	else								SetDlgItemText(IDC_CHECK_MODE_GPIO_0, _T("Open-Drain"));
	if (mode & HID_SMBUS_MASK_GPIO_1)	SetDlgItemText(IDC_CHECK_MODE_GPIO_1, _T("Push-Pull"));
	else								SetDlgItemText(IDC_CHECK_MODE_GPIO_1, _T("Open-Drain"));
	if (mode & HID_SMBUS_MASK_GPIO_2)	SetDlgItemText(IDC_CHECK_MODE_GPIO_2, _T("Push-Pull"));
	else								SetDlgItemText(IDC_CHECK_MODE_GPIO_2, _T("Open-Drain"));
	if (mode & HID_SMBUS_MASK_GPIO_3)	SetDlgItemText(IDC_CHECK_MODE_GPIO_3, _T("Push-Pull"));
	else								SetDlgItemText(IDC_CHECK_MODE_GPIO_3, _T("Open-Drain"));
	if (mode & HID_SMBUS_MASK_GPIO_4)	SetDlgItemText(IDC_CHECK_MODE_GPIO_4, _T("Push-Pull"));
	else								SetDlgItemText(IDC_CHECK_MODE_GPIO_4, _T("Open-Drain"));
	if (mode & HID_SMBUS_MASK_GPIO_5)	SetDlgItemText(IDC_CHECK_MODE_GPIO_5, _T("Push-Pull"));
	else								SetDlgItemText(IDC_CHECK_MODE_GPIO_5, _T("Open-Drain"));
	if (mode & HID_SMBUS_MASK_GPIO_6)	SetDlgItemText(IDC_CHECK_MODE_GPIO_6, _T("Push-Pull"));
	else								SetDlgItemText(IDC_CHECK_MODE_GPIO_6, _T("Open-Drain"));
	if (mode & HID_SMBUS_MASK_GPIO_7)	SetDlgItemText(IDC_CHECK_MODE_GPIO_7, _T("Push-Pull"));
	else								SetDlgItemText(IDC_CHECK_MODE_GPIO_7, _T("Open-Drain"));
}

// Get mode bitmask from control state
BYTE CPinConfigurationPage::GetMode()
{
	BYTE		mode		= 0x00;
	CString		caption;

	GetDlgItemText(IDC_CHECK_MODE_GPIO_0, caption);
	if (caption == _T("Push-Pull"))		mode |= HID_SMBUS_MASK_GPIO_0;
	GetDlgItemText(IDC_CHECK_MODE_GPIO_1, caption);
	if (caption == _T("Push-Pull"))		mode |= HID_SMBUS_MASK_GPIO_1;
	GetDlgItemText(IDC_CHECK_MODE_GPIO_2, caption);
	if (caption == _T("Push-Pull"))		mode |= HID_SMBUS_MASK_GPIO_2;
	GetDlgItemText(IDC_CHECK_MODE_GPIO_3, caption);
	if (caption == _T("Push-Pull"))		mode |= HID_SMBUS_MASK_GPIO_3;
	GetDlgItemText(IDC_CHECK_MODE_GPIO_4, caption);
	if (caption == _T("Push-Pull"))		mode |= HID_SMBUS_MASK_GPIO_4;
	GetDlgItemText(IDC_CHECK_MODE_GPIO_5, caption);
	if (caption == _T("Push-Pull"))		mode |= HID_SMBUS_MASK_GPIO_5;
	GetDlgItemText(IDC_CHECK_MODE_GPIO_6, caption);
	if (caption == _T("Push-Pull"))		mode |= HID_SMBUS_MASK_GPIO_6;
	GetDlgItemText(IDC_CHECK_MODE_GPIO_7, caption);
	if (caption == _T("Push-Pull"))		mode |= HID_SMBUS_MASK_GPIO_7;

	return mode;
}

// Update special function control checkboxes using function bitmask
void CPinConfigurationPage::SetFunction(BYTE function)
{
	if (function & HID_SMBUS_MASK_FUNCTION_GPIO_0_TXT)	CheckDlgButton(IDC_CHECK_TX_TOGGLE, TRUE);
	else												CheckDlgButton(IDC_CHECK_TX_TOGGLE, FALSE);
	if (function & HID_SMBUS_MASK_FUNCTION_GPIO_1_RXT)	CheckDlgButton(IDC_CHECK_RX_TOGGLE, TRUE);
	else												CheckDlgButton(IDC_CHECK_RX_TOGGLE, FALSE);
	if (function & HID_SMBUS_MASK_FUNCTION_GPIO_7_CLK)	CheckDlgButton(IDC_CHECK_CLK_OUTPUT, TRUE);
	else												CheckDlgButton(IDC_CHECK_CLK_OUTPUT, FALSE);
}

// Get special function bitmask from checkboxes
BYTE CPinConfigurationPage::GetFunction()
{
	BYTE function = 0x00;

	if (IsDlgButtonChecked(IDC_CHECK_TX_TOGGLE))	function |= HID_SMBUS_MASK_FUNCTION_GPIO_0_TXT;
	if (IsDlgButtonChecked(IDC_CHECK_RX_TOGGLE))	function |= HID_SMBUS_MASK_FUNCTION_GPIO_1_RXT;
	if (IsDlgButtonChecked(IDC_CHECK_CLK_OUTPUT))	function |= HID_SMBUS_MASK_FUNCTION_GPIO_7_CLK;

	return function;
}

// Update control captions using latch bitmask
void CPinConfigurationPage::SetLatchValue(BYTE latchValue)
{
	if (latchValue & HID_SMBUS_MASK_GPIO_0)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_0, _T("1"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_0, _T("0"));
	if (latchValue & HID_SMBUS_MASK_GPIO_1)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_1, _T("1"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_1, _T("0"));
	if (latchValue & HID_SMBUS_MASK_GPIO_2)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_2, _T("1"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_2, _T("0"));
	if (latchValue & HID_SMBUS_MASK_GPIO_3)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_3, _T("1"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_3, _T("0"));
	if (latchValue & HID_SMBUS_MASK_GPIO_4)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_4, _T("1"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_4, _T("0"));
	if (latchValue & HID_SMBUS_MASK_GPIO_5)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_5, _T("1"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_5, _T("0"));
	if (latchValue & HID_SMBUS_MASK_GPIO_6)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_6, _T("1"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_6, _T("0"));
	if (latchValue & HID_SMBUS_MASK_GPIO_7)		SetDlgItemText(IDC_CHECK_LATCH_GPIO_7, _T("1"));
	else										SetDlgItemText(IDC_CHECK_LATCH_GPIO_7, _T("0"));
}

// Get latch and mask bitmasks from control state
BYTE CPinConfigurationPage::GetLatchValue(BYTE& mask)
{
	BYTE		latchValue	= 0x00;	
	CString		caption;

	mask = 0xFF;

	GetDlgItemText(IDC_CHECK_LATCH_GPIO_0, caption);
	if (caption == _T("1"))			latchValue |= HID_SMBUS_MASK_GPIO_0;
	else if (caption == _T("X"))	mask &= ~HID_SMBUS_MASK_GPIO_0;
	GetDlgItemText(IDC_CHECK_LATCH_GPIO_1, caption);
	if (caption == _T("1"))			latchValue |= HID_SMBUS_MASK_GPIO_1;
	else if (caption == _T("X"))	mask &= ~HID_SMBUS_MASK_GPIO_1;
	GetDlgItemText(IDC_CHECK_LATCH_GPIO_2, caption);
	if (caption == _T("1"))			latchValue |= HID_SMBUS_MASK_GPIO_2;
	else if (caption == _T("X"))	mask &= ~HID_SMBUS_MASK_GPIO_2;
	GetDlgItemText(IDC_CHECK_LATCH_GPIO_3, caption);
	if (caption == _T("1"))			latchValue |= HID_SMBUS_MASK_GPIO_3;
	else if (caption == _T("X"))	mask &= ~HID_SMBUS_MASK_GPIO_3;
	GetDlgItemText(IDC_CHECK_LATCH_GPIO_4, caption);
	if (caption == _T("1"))			latchValue |= HID_SMBUS_MASK_GPIO_4;
	else if (caption == _T("X"))	mask &= ~HID_SMBUS_MASK_GPIO_4;
	GetDlgItemText(IDC_CHECK_LATCH_GPIO_5, caption);
	if (caption == _T("1"))			latchValue |= HID_SMBUS_MASK_GPIO_5;
	else if (caption == _T("X"))	mask &= ~HID_SMBUS_MASK_GPIO_5;
	GetDlgItemText(IDC_CHECK_LATCH_GPIO_6, caption);
	if (caption == _T("1"))			latchValue |= HID_SMBUS_MASK_GPIO_6;
	else if (caption == _T("X"))	mask &= ~HID_SMBUS_MASK_GPIO_6;
	GetDlgItemText(IDC_CHECK_LATCH_GPIO_7, caption);
	if (caption == _T("1"))			latchValue |= HID_SMBUS_MASK_GPIO_7;
	else if (caption == _T("X"))	mask &= ~HID_SMBUS_MASK_GPIO_7;

	return latchValue;
}

void CPinConfigurationPage::GetGpioConfig(BOOL silent)
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		BYTE direction;
		BYTE mode;
		BYTE function;

		// Get GPIO direction and mode bitmasks
		HID_SMBUS_STATUS status = HidSmbus_GetGpioConfig(*m_pHidSmbus, &direction, &mode, &function, &m_clkDiv);

		if (status == HID_SMBUS_SUCCESS)
		{
			// Update controls to reflect direction and mode bitmasks
			SetDirection(direction);
			SetMode(mode);
			SetFunction(function);

			UpdateControlStates();
		}

		if (!silent)
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_GetGpioConfig"), status);
		}
	}
}

void CPinConfigurationPage::SetGpioConfig()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		BYTE direction;
		BYTE mode;
		BYTE function;

		// Get the direction and mode bitmasks from the control states
		direction	= GetDirection();
		mode		= GetMode();
		function	= GetFunction();

		// Set GPIO direction and mode bitmasks
		HID_SMBUS_STATUS status = HidSmbus_SetGpioConfig(*m_pHidSmbus, direction, mode, function, m_clkDiv);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_SetGpioConfig"), status);
	}
}

void CPinConfigurationPage::ReadLatch(BOOL silent)
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		BYTE latchValue;

		// Read GPIO latch value
		HID_SMBUS_STATUS status = HidSmbus_ReadLatch(*m_pHidSmbus, &latchValue);

		if (status == HID_SMBUS_SUCCESS)
		{
			// Update controls to reflect latch value
			SetLatchValue(latchValue);
		}

		if (!silent)
		{
			// Output status to status bar
			// And play an audible alert if the status is not HID_SMBUS_SUCCESS
			OutputStatus(_T("HidSmbus_ReadLatch"), status);
		}
	}
}

void CPinConfigurationPage::WriteLatch()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(*m_pHidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		BYTE latchValue;
		BYTE mask;

		// Get the latchValue and mask bitmasks from the control states
		latchValue = GetLatchValue(mask);

		// Write GPIO latch value
		// "X" - means that the GPIO pin will not be modified
		HID_SMBUS_STATUS status = HidSmbus_WriteLatch(*m_pHidSmbus, latchValue, mask);

		// Output status to status bar
		// And play an audible alert if the status is not HID_SMBUS_SUCCESS
		OutputStatus(_T("HidSmbus_WriteLatch"), status);
	}
}

// Output status to status bar
// And play an audible alert if the status is not HID_SMBUS_SUCCESS
void CPinConfigurationPage::OutputStatus(CString functionName, HID_SMBUS_STATUS status)
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
// CPinConfigurationPage - Message Handlers
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPinConfigurationPage, CDialog)
	ON_BN_CLICKED(IDOK, &CPinConfigurationPage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPinConfigurationPage::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_GET_GPIO_CONFIG, &CPinConfigurationPage::OnBnClickedButtonGetGpioConfig)
	ON_BN_CLICKED(IDC_BUTTON_SET_GPIO_CONFIG, &CPinConfigurationPage::OnBnClickedButtonSetGpioConfig)
	ON_BN_CLICKED(IDC_BUTTON_READ_LATCH, &CPinConfigurationPage::OnBnClickedButtonReadLatch)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_LATCH, &CPinConfigurationPage::OnBnClickedButtonWriteLatch)
	ON_BN_CLICKED(IDC_CHECK_DIRECTION_GPIO_0, &CPinConfigurationPage::OnBnClickedCheckDirection)
	ON_BN_CLICKED(IDC_CHECK_DIRECTION_GPIO_1, &CPinConfigurationPage::OnBnClickedCheckDirection)
	ON_BN_CLICKED(IDC_CHECK_DIRECTION_GPIO_2, &CPinConfigurationPage::OnBnClickedCheckDirection)
	ON_BN_CLICKED(IDC_CHECK_DIRECTION_GPIO_3, &CPinConfigurationPage::OnBnClickedCheckDirection)
	ON_BN_CLICKED(IDC_CHECK_DIRECTION_GPIO_4, &CPinConfigurationPage::OnBnClickedCheckDirection)
	ON_BN_CLICKED(IDC_CHECK_DIRECTION_GPIO_5, &CPinConfigurationPage::OnBnClickedCheckDirection)
	ON_BN_CLICKED(IDC_CHECK_DIRECTION_GPIO_6, &CPinConfigurationPage::OnBnClickedCheckDirection)
	ON_BN_CLICKED(IDC_CHECK_DIRECTION_GPIO_7, &CPinConfigurationPage::OnBnClickedCheckDirection)
	ON_BN_CLICKED(IDC_CHECK_MODE_GPIO_0, &CPinConfigurationPage::OnBnClickedCheckMode)
	ON_BN_CLICKED(IDC_CHECK_MODE_GPIO_1, &CPinConfigurationPage::OnBnClickedCheckMode)
	ON_BN_CLICKED(IDC_CHECK_MODE_GPIO_2, &CPinConfigurationPage::OnBnClickedCheckMode)
	ON_BN_CLICKED(IDC_CHECK_MODE_GPIO_3, &CPinConfigurationPage::OnBnClickedCheckMode)
	ON_BN_CLICKED(IDC_CHECK_MODE_GPIO_4, &CPinConfigurationPage::OnBnClickedCheckMode)
	ON_BN_CLICKED(IDC_CHECK_MODE_GPIO_5, &CPinConfigurationPage::OnBnClickedCheckMode)
	ON_BN_CLICKED(IDC_CHECK_MODE_GPIO_6, &CPinConfigurationPage::OnBnClickedCheckMode)
	ON_BN_CLICKED(IDC_CHECK_MODE_GPIO_7, &CPinConfigurationPage::OnBnClickedCheckMode)
	ON_BN_CLICKED(IDC_CHECK_CLK_OUTPUT, &CPinConfigurationPage::OnBnClickedCheckFunction)
	ON_BN_CLICKED(IDC_CHECK_TX_TOGGLE, &CPinConfigurationPage::OnBnClickedCheckFunction)
	ON_BN_CLICKED(IDC_CHECK_RX_TOGGLE, &CPinConfigurationPage::OnBnClickedCheckFunction)
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_0, &CPinConfigurationPage::OnBnClickedCheckLatch)
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_1, &CPinConfigurationPage::OnBnClickedCheckLatch)
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_2, &CPinConfigurationPage::OnBnClickedCheckLatch)
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_3, &CPinConfigurationPage::OnBnClickedCheckLatch)
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_4, &CPinConfigurationPage::OnBnClickedCheckLatch)
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_5, &CPinConfigurationPage::OnBnClickedCheckLatch)
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_6, &CPinConfigurationPage::OnBnClickedCheckLatch)
	ON_BN_CLICKED(IDC_CHECK_LATCH_GPIO_7, &CPinConfigurationPage::OnBnClickedCheckLatch)
	ON_EN_CHANGE(IDC_EDIT_CLK_DIV, &CPinConfigurationPage::OnEnChangeEditClkDiv)
END_MESSAGE_MAP()

// Override enter and cancel keys so that the property
// pages aren't closed
void CPinConfigurationPage::OnBnClickedOk()
{
}

// Override enter and cancel keys so that the property
// pages aren't closed
void CPinConfigurationPage::OnBnClickedCancel()
{
}

void CPinConfigurationPage::OnBnClickedButtonGetGpioConfig()
{
	if (UpdateData(TRUE))
	{
		GetGpioConfig();
		UpdateData(FALSE);
	}
}

void CPinConfigurationPage::OnBnClickedButtonSetGpioConfig()
{
	if (UpdateData(TRUE))
	{
		SetGpioConfig();
	}
}

void CPinConfigurationPage::OnBnClickedButtonReadLatch()
{
	ReadLatch();
}

void CPinConfigurationPage::OnBnClickedButtonWriteLatch()
{
	WriteLatch();
}

// Handle the BN_CLICKED message for:
// IDC_CHECK_DIRECTION_GPIO_0
// IDC_CHECK_DIRECTION_GPIO_1
// IDC_CHECK_DIRECTION_GPIO_2
// IDC_CHECK_DIRECTION_GPIO_3
// IDC_CHECK_DIRECTION_GPIO_4
// IDC_CHECK_DIRECTION_GPIO_5
// IDC_CHECK_DIRECTION_GPIO_6
// IDC_CHECK_DIRECTION_GPIO_7
//
// Toggle direction between "Input" and "Output"
void CPinConfigurationPage::OnBnClickedCheckDirection()
{
	const MSG* pMsg = GetCurrentMessage();

	if (pMsg->message == WM_COMMAND)
	{
		// Get control ID
		int nID = (int)pMsg->wParam;

		// Get current button caption
		CString caption;
		GetDlgItemText(nID, caption);

		// Toggle caption
		if (caption == _T("Output"))
		{
			SetDlgItemText(nID, _T("Input"));
		}
		else
		{
			SetDlgItemText(nID, _T("Output"));
		}

		UpdateControlStates();
	}
}

// Handle the BN_CLICKED message for:
// IDC_CHECK_MODE_GPIO_0
// IDC_CHECK_MODE_GPIO_1
// IDC_CHECK_MODE_GPIO_2
// IDC_CHECK_MODE_GPIO_3
// IDC_CHECK_MODE_GPIO_4
// IDC_CHECK_MODE_GPIO_5
// IDC_CHECK_MODE_GPIO_6
// IDC_CHECK_MODE_GPIO_7
//
// Toggle direction between "Open-Drain" and "Push-Pull"
void CPinConfigurationPage::OnBnClickedCheckMode()
{
	const MSG* pMsg = GetCurrentMessage();

	if (pMsg->message == WM_COMMAND)
	{
		// Get control ID
		int nID = (int)pMsg->wParam;

		// Get current button caption
		CString caption;
		GetDlgItemText(nID, caption);

		// Toggle caption
		if (caption == _T("Push-Pull"))
		{
			SetDlgItemText(nID, _T("Open-Drain"));
		}
		else
		{
			SetDlgItemText(nID, _T("Push-Pull"));
		}
	}
}

// Handle the BN_CLICKED message for:
// IDC_CHECK_CLK_OUTPUT
// IDC_CHECK_TX_TOGGLE
// IDC_CHECK_RX_TOGGLE
void CPinConfigurationPage::OnBnClickedCheckFunction()
{
	UpdateControlStates();
}

// Handle BN_CLICKED message for:
// IDC_CHECK_LATCH_GPIO_0
// IDC_CHECK_LATCH_GPIO_1
// IDC_CHECK_LATCH_GPIO_2
// IDC_CHECK_LATCH_GPIO_3
// IDC_CHECK_LATCH_GPIO_4
// IDC_CHECK_LATCH_GPIO_5
// IDC_CHECK_LATCH_GPIO_6
// IDC_CHECK_LATCH_GPIO_7
//
// Toggle latch value between "0", "1", and "X"
void CPinConfigurationPage::OnBnClickedCheckLatch()
{
	const MSG* pMsg = GetCurrentMessage();

	if (pMsg->message == WM_COMMAND)
	{
		// Get control ID
		int nID = (int)pMsg->wParam;

		// Get current button caption
		CString caption;
		GetDlgItemText(nID, caption);

		// Toggle caption
		if (caption == _T("0"))
		{
			SetDlgItemText(nID, _T("1"));
		}
		else if (caption == _T("1"))
		{
			SetDlgItemText(nID, _T("X"));
		}
		else
		{
			SetDlgItemText(nID, _T("0"));
		}
	}
}

// Convert the CLK Output Speed using
// the CLK Output Divider
void CPinConfigurationPage::OnEnChangeEditClkDiv()
{
	CString clkDiv;
	GetDlgItemText(IDC_EDIT_CLK_DIV, clkDiv);

	UINT clkDivValue = _tcstoul(clkDiv, NULL, 10);

	CString clk_Hz;

	if (clkDivValue == 0)
	{
		clk_Hz = _T("48000000");
	}
	else if (clkDivValue >= 0 && clkDivValue <= 255)
	{
		clk_Hz.Format(_T("%u"), 48000000/(clkDivValue * 2));
	}
	else
	{
		clk_Hz = _T("Invalid");
	}

	SetDlgItemText(IDC_EDIT_CLK_FREQ, clk_Hz);
}
