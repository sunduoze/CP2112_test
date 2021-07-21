/////////////////////////////////////////////////////////////////////////////
// DlgTabCtrl.cpp
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DlgTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgTabCtrl Class - Constructor/Destructor
/////////////////////////////////////////////////////////////////////////////

CDlgTabCtrl::CDlgTabCtrl(void)
{
}

CDlgTabCtrl::~CDlgTabCtrl(void)
{
	// Remove all CDialog pointers from the vector
	m_tabDlgs.clear();
}

/////////////////////////////////////////////////////////////////////////////
// CDlgTabCtrl Class - Public Overrides
/////////////////////////////////////////////////////////////////////////////

LONG CDlgTabCtrl::InsertItem(int nItem, LPCTSTR lpszItem, int nID, CDialog* pDlg)
{
	// Create a new tab with the specified caption
	LONG result = CTabCtrl::InsertItem(nItem, lpszItem);

	// Store dialog ID and pointer for later use
	m_tabDlgs.insert(m_tabDlgs.begin() + nItem, pDlg);

	// Call CDialog::Create()
	// Reposition the dialog to fit in the tab control client area
	InitDialog(nID, pDlg);

	// Show only the dialog for the current tab
	SetActiveDialog();

	return result;
}

int CDlgTabCtrl::GetCurSel() const
{
	return CTabCtrl::GetCurSel();
}

int CDlgTabCtrl::SetCurSel(int nItem)
{
	// Activate specified tab
	int result = CTabCtrl::SetCurSel(nItem);

	// Show only the dialog for the current tab
	SetActiveDialog();

	return result;
}

BOOL CDlgTabCtrl::DeleteAllItems()
{
	BOOL result = CTabCtrl::DeleteAllItems();

	if (result)
	{
		// Remove all CDialog pointers from the vector
		m_tabDlgs.clear();
	}

	return result;
}

BOOL CDlgTabCtrl::DeleteItem(int nItem)
{
	BOOL result = CTabCtrl::DeleteItem(nItem);

	if (result)
	{
		if (nItem >= 0 && nItem < (int)m_tabDlgs.size())
		{
			// Remove the CDialog pointer from the vector
			m_tabDlgs.erase(m_tabDlgs.begin() + nItem);
		}

		// Show only the dialog for the current tab
		SetActiveDialog();
	}

	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgTabCtrl Class - Public Methods
/////////////////////////////////////////////////////////////////////////////

CDialog* CDlgTabCtrl::GetDialog(int nItem)
{
	CDialog* pDlg = NULL;

	if (nItem >= 0 && nItem < (int)m_tabDlgs.size())
	{
		pDlg = m_tabDlgs[nItem];
	}

	return pDlg;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgTabCtrl Class - Protected Methods
/////////////////////////////////////////////////////////////////////////////

// Create and position a CDialog in the tab client area
void CDlgTabCtrl::InitDialog(int nID, CDialog* pDlg)
{
	// Create the dialog
	// Make sure that the dialog style is set to child
	// Set the dialog's parent to the tab control's parent
	pDlg->Create(nID, GetParent());

	// Position a CDialog in the tab client area
	RepositionDialog(pDlg);
}

// Position a CDialog in the tab client area
void CDlgTabCtrl::RepositionDialog(CDialog* pDlg)
{
	CRect client;
	CRect window;

	// Calculate tab client area location
	GetClientRect(client);
	AdjustRect(FALSE, client);
	GetWindowRect(window);
	GetParent()->ScreenToClient(window);
	client.OffsetRect(window.left, window.top);
	
	// Move dialog to the tab client area
	pDlg->SetWindowPos(&wndTop, client.left, client.top, client.Width(), client.Height(), SWP_HIDEWINDOW);
}

// Show only the window for the corresponding tab
// selection
void CDlgTabCtrl::SetActiveDialog()
{
	const size_t sel = GetCurSel();

	for (size_t i = 0; i < m_tabDlgs.size(); i++)
	{
		if (m_tabDlgs[i]->GetSafeHwnd())
		{
			// Position a CDialog in the tab client area
			RepositionDialog(m_tabDlgs[i]);

			if (i == sel)
			{
				m_tabDlgs[i]->ShowWindow(SW_SHOW);
			}
			else
			{
				m_tabDlgs[i]->ShowWindow(SW_HIDE);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDlgTabCtrl Class - Message Handlers
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CDlgTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()

// Automatically activate the dialog for the current tab selection
// when a tab is selected
void CDlgTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	// Show only the dialog for the current tab
	SetActiveDialog();

	*pResult = 0;
}
