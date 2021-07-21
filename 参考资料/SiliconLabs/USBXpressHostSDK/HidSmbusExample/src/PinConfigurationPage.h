#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "SLABCP2112.h"

/////////////////////////////////////////////////////////////////////////////
// CPinConfigurationPage dialog
/////////////////////////////////////////////////////////////////////////////

class CPinConfigurationPage : public CDialog
{
	DECLARE_DYNAMIC(CPinConfigurationPage)

public:
	CPinConfigurationPage(HID_SMBUS_DEVICE* pHidSmbus, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPinConfigurationPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_PIN_CONFIGURATION };

// Control Variables
protected:
	BYTE m_clkDiv;

// Protected Members
protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;

// Public Methods
public:
	void UpdateControlStates();

	void SetDefaults();
	void SetFromDevice();

// Protected Methods
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CustomDataExchange(CDataExchange* pDX);

	void SetDirection(BYTE direction);
	BYTE GetDirection();
	void SetMode(BYTE mode);
	BYTE GetMode();
	void SetFunction(BYTE function);
	BYTE GetFunction();
	void SetLatchValue(BYTE latchValue);
	BYTE GetLatchValue(BYTE& mask);

	void GetGpioConfig(BOOL silent = FALSE);
	void SetGpioConfig();
	void ReadLatch(BOOL silent = FALSE);
	void WriteLatch();

	void OutputStatus(CString functionName, HID_SMBUS_STATUS status);

// Message Handlers
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonGetGpioConfig();
	afx_msg void OnBnClickedButtonSetGpioConfig();
	afx_msg void OnBnClickedButtonReadLatch();
	afx_msg void OnBnClickedButtonWriteLatch();
	afx_msg void OnBnClickedCheckDirection();
	afx_msg void OnBnClickedCheckMode();
	afx_msg void OnBnClickedCheckFunction();
	afx_msg void OnBnClickedCheckLatch();
	afx_msg void OnEnChangeEditClkDiv();
};
