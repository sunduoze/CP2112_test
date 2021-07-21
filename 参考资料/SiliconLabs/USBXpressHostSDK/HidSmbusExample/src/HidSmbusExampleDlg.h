/////////////////////////////////////////////////////////////////////////////
// HidSmbusExampleDlg.h : header file
/////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "ConfigurationPage.h"
#include "DataTransferPage.h"
#include "PinConfigurationPage.h"
#include "CustomizationPage.h"
#include "DlgTabCtrl.h"
#include "SLABCP2112.h"

#include <dbt.h>

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

#define VID				0
#define PID				0

/////////////////////////////////////////////////////////////////////////////
// CHidSmbusExampleDlg dialog
/////////////////////////////////////////////////////////////////////////////

class CHidSmbusExampleDlg : public CDialog
{
// Construction
public:
	CHidSmbusExampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HIDSMBUSEXAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Control Variables
protected:
	CDlgTabCtrl				m_tabCtrl;
	CStatusBar				m_statusBar;
	CComboBox				m_comboDeviceList;

// Implementation
protected:
	HICON					m_hIcon;
	HICON					m_hSmallIcon;
	HID_SMBUS_DEVICE		m_hidSmbus;

	CConfigurationPage		m_pageConfiguration;
	CDataTransferPage		m_pageDataTransfer;
	CPinConfigurationPage	m_pagePinConfiguration;
	CCustomizationPage		m_pageCustomization;

	void InitializeDialog();
	void InitStatusBar();
	void InitDlgTabs();

	void SetConnectionText(const CString& text);

	void UpdateDeviceList();
	void UpdateDeviceInformation(BOOL connected);

	void EnableDeviceCtrls(BOOL enable);

	BOOL GetSelectedDevice(CString& serial);
	BOOL FindDevice(CString serial, DWORD& deviceNum);
	BOOL Connect();
	BOOL Disconnect();

// Public Methods
public:
	void SetStatusText(const CString& text);

// Surprise Removal
protected:
	HDEVNOTIFY	m_hNotifyDevNode;

	void RegisterDeviceChange();
	void UnregisterDeviceChange();

// Generated message map functions
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	afx_msg void OnCbnDropdownComboDeviceList();
	afx_msg void OnCbnCloseupComboDeviceList();
	afx_msg void OnCbnSelchangeComboDeviceList();
	afx_msg void OnBnClickedCheckConnect();
	afx_msg void OnBnClickedButtonReset();
};
