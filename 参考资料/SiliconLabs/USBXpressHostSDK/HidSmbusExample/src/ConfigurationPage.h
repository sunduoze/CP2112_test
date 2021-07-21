#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "SLABCP2112.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigurationPage dialog
/////////////////////////////////////////////////////////////////////////////

class CConfigurationPage : public CDialog
{
	DECLARE_DYNAMIC(CConfigurationPage)

public:
	CConfigurationPage(HID_SMBUS_DEVICE* pHidSmbus, CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigurationPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CONFIGURATION };

// Control Variables
protected:
	DWORD	m_bitRate;
	BYTE	m_ackAddress;
	BOOL	m_autoRespond;
	WORD	m_writeTimeout;
	WORD	m_readTimeout;
	WORD	m_transferRetries;
	BOOL	m_sclLowTimeout;
	DWORD	m_responseTimeout;

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
	void CustomDataExchange(CDataExchange* pDX);

	void GetSmbusConfig(BOOL silent = FALSE);
	void SetSmbusConfig();
	void GetTimeout(BOOL silent = FALSE);
	void SetTimeout();

	void OutputStatus(CString functionName, HID_SMBUS_STATUS status);

// Message Handlers
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonGetSmbusConfig();
	afx_msg void OnBnClickedButtonSetSmbusConfig();
	afx_msg void OnBnClickedButtonGetTimeout();
	afx_msg void OnBnClickedButtonSetTimeout();
};
