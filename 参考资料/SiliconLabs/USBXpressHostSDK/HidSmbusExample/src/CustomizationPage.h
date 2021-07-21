#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "SLABCP2112.h"

/////////////////////////////////////////////////////////////////////////////
// CCustomizationPage dialog
/////////////////////////////////////////////////////////////////////////////

class CCustomizationPage : public CDialog
{
	DECLARE_DYNAMIC(CCustomizationPage)

public:
	CCustomizationPage(HID_SMBUS_DEVICE* pHidSmbus, CWnd* pParent = NULL);   // standard constructor
	virtual ~CCustomizationPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CUSTOMIZE };

// Control Variables
protected:
	WORD		m_vid;
	WORD		m_pid;
	WORD		m_power;
	int			m_powerMode;
	BYTE		m_releaseVersionMSB;
	BYTE		m_releaseVersionLSB;
	CString		m_manufacturer;
	CString		m_product;
	CString		m_serial;

// Protected Members
protected:
	HID_SMBUS_DEVICE* m_pHidSmbus;

// Public Methods
public:
	void SetDefaults();
	void SetFromDevice();

// Protected Methods
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CustomDDX(CDataExchange* pDX);

	void GetUsbConfig(BOOL silent = FALSE);
	void SetUsbConfig();
	void GetManufacturer(BOOL silent = FALSE);
	void SetManufacturer();
	void GetProduct(BOOL silent = FALSE);
	void SetProduct();
	void GetSerial(BOOL silent = FALSE);
	void SetSerial();
	void GetLock(BOOL silent = FALSE);
	void SetLock();

	void OutputStatus(CString functionName, HID_SMBUS_STATUS status);

// Message Handlers
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonGetUsbConfig();
	afx_msg void OnBnClickedButtonSetUsbConfig();
	afx_msg void OnBnClickedButtonGetManufacturer();
	afx_msg void OnBnClickedButtonSetManufacturer();
	afx_msg void OnBnClickedButtonGetProduct();
	afx_msg void OnBnClickedButtonSetProduct();
	afx_msg void OnBnClickedButtonGetSerial();
	afx_msg void OnBnClickedButtonSetSerial();
	afx_msg void OnBnClickedButtonGetLock();
	afx_msg void OnBnClickedButtonSetLock();
};
