/////////////////////////////////////////////////////////////////////////////
// DlgTabCtrl.h
/////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "afxcmn.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CDlgTabCtrl Class
/////////////////////////////////////////////////////////////////////////////

class CDlgTabCtrl : public CTabCtrl
{
// Constructor/Destructor
public:
	CDlgTabCtrl(void);
	~CDlgTabCtrl(void);

// Public Overrides
public:
	LONG InsertItem(int nItem, LPCTSTR lpszItem, int nID, CDialog* pDlg);
	int GetCurSel() const;
	int SetCurSel(int nItem);
	BOOL DeleteAllItems();
	BOOL DeleteItem(int nItem);

// Public Methods
public:
	CDialog* GetDialog(int nItem);

// Protected Methods
protected:
	void InitDialog(int nID, CDialog* pDlg);
	void RepositionDialog(CDialog* pDlg);
	void SetActiveDialog();

// Protected Members
protected:
	std::vector<CDialog*>	m_tabDlgs;

// Message Handlers
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};
