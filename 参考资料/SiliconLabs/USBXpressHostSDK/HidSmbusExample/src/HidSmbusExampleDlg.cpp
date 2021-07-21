/////////////////////////////////////////////////////////////////////////////
// HidSmbusExampleDlg.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HidSmbusExample.h"
#include "HidSmbusExampleDlg.h"
#include "Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Library Dependencies
/////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "SLABHIDtoSMBus.lib")

/////////////////////////////////////////////////////////////////////////////
// Static Global Variables
/////////////////////////////////////////////////////////////////////////////

// Set status bar pane 0 and 1 indicators
static UINT BASED_CODE StatusBarIndicators[] =
{
	ID_INDICATOR_DEVICE,
    ID_INDICATOR_STATUS
};

/////////////////////////////////////////////////////////////////////////////
// Static Global Function Prototypes
/////////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK EnableControl(HWND hWnd, LPARAM enable);
static void EnableDlgControls(HWND hParentDlg, BOOL enable);

/////////////////////////////////////////////////////////////////////////////
// Static Global Functions
/////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK EnableControl(HWND hWnd, LPARAM enable)
{
	EnableWindow(hWnd, (BOOL)enable);

	return TRUE;
}

void EnableDlgControls(HWND hParentDlg, BOOL enable)
{
	if (hParentDlg != NULL)
	{
		// Call the EnableControl() callback
		// function for each child control on the dialog
		EnumChildWindows(hParentDlg, EnableControl, enable);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
/////////////////////////////////////////////////////////////////////////////

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	BYTE major;
	BYTE minor;
	BOOL release;

	if (HidSmbus_GetLibraryVersion(&major, &minor, &release) == HID_SMBUS_SUCCESS)
	{
		CString version;
		version.Format(_T("%d.%d%s"), major, minor, (release) ? _T("") : _T(" (Debug)"));
		SetDlgItemText(IDC_STATIC_LIBRARY_VERSION, version);
	}

	if (HidSmbus_GetHidLibraryVersion(&major, &minor, &release) == HID_SMBUS_SUCCESS)
	{
		CString version;
		version.Format(_T("%d.%d%s"), major, minor, (release) ? _T("") : _T(" (Debug)"));
		SetDlgItemText(IDC_STATIC_HID_LIBRARY_VERSION, version);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHidSmbusExampleDlg dialog
/////////////////////////////////////////////////////////////////////////////

CHidSmbusExampleDlg::CHidSmbusExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHidSmbusExampleDlg::IDD, pParent)
	, m_pageConfiguration(&m_hidSmbus)
	, m_pageDataTransfer(&m_hidSmbus)
	, m_pagePinConfiguration(&m_hidSmbus)
	, m_pageCustomization(&m_hidSmbus)
{
	m_hIcon			= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hSmallIcon	= (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0);
	m_hidSmbus		= NULL;
}

void CHidSmbusExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CTRL, m_tabCtrl);
	DDX_Control(pDX, IDC_COMBO_DEVICE_LIST, m_comboDeviceList);
}

BEGIN_MESSAGE_MAP(CHidSmbusExampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CHidSmbusExampleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHidSmbusExampleDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_HELPINFO()
	ON_WM_DEVICECHANGE()
	ON_CBN_DROPDOWN(IDC_COMBO_DEVICE_LIST, &CHidSmbusExampleDlg::OnCbnDropdownComboDeviceList)
	ON_CBN_CLOSEUP(IDC_COMBO_DEVICE_LIST, &CHidSmbusExampleDlg::OnCbnCloseupComboDeviceList)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_LIST, &CHidSmbusExampleDlg::OnCbnSelchangeComboDeviceList)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CHidSmbusExampleDlg::OnBnClickedCheckConnect)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CHidSmbusExampleDlg::OnBnClickedButtonReset)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHidSmbusExampleDlg message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CHidSmbusExampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		if (strAboutMenu.LoadString(IDS_ABOUTBOX) && !strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hSmallIcon, FALSE);	// Set small icon

	InitializeDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHidSmbusExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHidSmbusExampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHidSmbusExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHidSmbusExampleDlg::OnBnClickedOk()
{
}

void CHidSmbusExampleDlg::OnBnClickedCancel()
{
}

void CHidSmbusExampleDlg::OnClose()
{
	CDialog::OnCancel();
}

void CHidSmbusExampleDlg::OnDestroy()
{
	CDialog::OnDestroy();

	BOOL opened;

	// Close the device if it is opened
	if (HidSmbus_IsOpened(m_hidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		HidSmbus_Close(m_hidSmbus);
		m_hidSmbus = NULL;
	}

	// Stop receiving WM_DEVICECHANGE messages
	UnregisterDeviceChange();
}

// Disable F1 help
BOOL CHidSmbusExampleDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	UNREFERENCED_PARAMETER(pHelpInfo);

	return TRUE;
}

// Handle device change messages (ie a device is added or removed)
// - If an HID device is connected, then add the device to the device list
// - If the device we were connected to was removed, then disconnect from the device
BOOL CHidSmbusExampleDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
	// Device has been removed
	if (nEventType == DBT_DEVICEREMOVECOMPLETE ||
		nEventType == DBT_DEVICEARRIVAL)
	{
		if (dwData)
		{
			PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)dwData;

			if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
			{
				PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
				BOOL opened			= FALSE;
				BOOL disconnected	= FALSE;

				// "\\?\hid#vid_10c4&pid_81ba..."
				CString deviceStr = pDevInf->dbcc_name;

				// Check to see if the device is opened
				HidSmbus_IsOpened(m_hidSmbus, &opened);

				if (nEventType == DBT_DEVICEREMOVECOMPLETE)
				{
					HID_SMBUS_DEVICE_STR devPath;

					// If a device is currently open
					if (opened)
					{
						// Get our device's path
						if (HidSmbus_GetOpenedString(m_hidSmbus, devPath, HID_SMBUS_GET_PATH_STR) == HID_SMBUS_SUCCESS)
						{
							// Check to see if our device was removed by comparing device paths
							if (deviceStr.CompareNoCase(CString(devPath)) == 0)
							{
								// Disconnect from it
								Disconnect();

								disconnected = TRUE;
							}
						}
					}
				}

				// Only update the device list if we aren't connected to a device
				if (disconnected || !opened)
				{
					UpdateDeviceList();
					UpdateDeviceInformation(FALSE);
				}
			}
		}
	}

	return TRUE;
}

void CHidSmbusExampleDlg::OnCbnDropdownComboDeviceList()
{
	// Automatically update the device list when the
	// list is opened/dropped down
	UpdateDeviceList();

	// Update device information for selected device (not connected)
	UpdateDeviceInformation(FALSE);
}

void CHidSmbusExampleDlg::OnCbnCloseupComboDeviceList()
{
	CString		serial;
	DWORD		deviceNum;

	if (GetSelectedDevice(serial))
	{
		// If the selected device has been removed
		if (!FindDevice(serial, deviceNum))
		{
			// Then update the device list
			UpdateDeviceList();
			UpdateDeviceInformation(FALSE);			
		}
	}
}

void CHidSmbusExampleDlg::OnCbnSelchangeComboDeviceList()
{
	// Update device information for newly selected device
	// (not connected)
	UpdateDeviceInformation(FALSE);
}

void CHidSmbusExampleDlg::OnBnClickedCheckConnect()
{
	// Disconnecting
	if (IsDlgButtonChecked(IDC_CHECK_CONNECT))
	{
		Disconnect();
	}
	// Connecting
	else
	{
		Connect();
	}
}

void CHidSmbusExampleDlg::OnBnClickedButtonReset()
{
	BOOL opened;

	// Make sure that the device is opened
	if (HidSmbus_IsOpened(m_hidSmbus, &opened) == HID_SMBUS_SUCCESS && opened)
	{
		// Reset the device
		HID_SMBUS_STATUS status = HidSmbus_Reset(m_hidSmbus);

		// The device will re-enumerate and the device pointer will become invalid
		Disconnect();

		// Update status bar text
		CString statusText;
		statusText.Format(_T("HidSmbus_Reset(): %s"), HidSmbus_DecodeErrorStatus(status).GetString());
		SetStatusText(statusText);

		// Alert user if the function failed
		if (status != HID_SMBUS_SUCCESS)
		{
			MessageBeep(MB_ICONHAND);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHidSmbusExampleDlg - Protected Methods
/////////////////////////////////////////////////////////////////////////////

void CHidSmbusExampleDlg::InitializeDialog()
{
	InitStatusBar();
	InitDlgTabs();

	// Register to receive WM_DEVICECHANGE messages
	RegisterDeviceChange();

	// Initially disable device controls
	EnableDeviceCtrls(FALSE);

	UpdateDeviceList();
	UpdateDeviceInformation(FALSE);
}

// Initialize the status bar
// - Create the status bar object
// - Set number of panes to 2
// - Make pane 0 a fixed width
// - Make pane 1 fit the rest of the dialog
// - Draw the status bar
// - Set default status bar text to "Not Connected" and ""
void CHidSmbusExampleDlg::InitStatusBar()
{
	// Create the status bar
	m_statusBar.Create(this);

	// Set number of panes
	m_statusBar.SetIndicators(StatusBarIndicators, sizeof(StatusBarIndicators) / sizeof(StatusBarIndicators[0]));

	// Make the device pane a fixed width
	m_statusBar.SetPaneInfo(0, ID_INDICATOR_DEVICE, SBPS_NORMAL, 100);

	// Make the status pane fit the rest of the status bar
	m_statusBar.SetPaneInfo(1, ID_INDICATOR_STATUS, SBPS_STRETCH, 0);

	// Draw the status bar
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_STATUS);

	// Set default status bar text
	SetConnectionText(_T("Not Connected"));
	SetStatusText(_T(""));
}

// Create each of the dialog tabs and add
// tabs to the tab control
void CHidSmbusExampleDlg::InitDlgTabs()
{
	m_tabCtrl.InsertItem(0, _T("Configuration"),		IDD_PROPPAGE_CONFIGURATION,			&m_pageConfiguration);
	m_tabCtrl.InsertItem(1, _T("Data Transfer"),		IDD_PROPPAGE_DATA_TRANSFER,			&m_pageDataTransfer);
	m_tabCtrl.InsertItem(2, _T("Pin Configuration"),	IDD_PROPPAGE_PIN_CONFIGURATION,		&m_pagePinConfiguration);
	m_tabCtrl.InsertItem(3, _T("Customization"),		IDD_PROPPAGE_CUSTOMIZE,				&m_pageCustomization);
}

// Set status bar text for specified pane 0
void CHidSmbusExampleDlg::SetConnectionText(const CString& text)
{
	// Set the status bar text
	m_statusBar.SetPaneText(0, text);
}

// Set status bar text for specified pane 1
void CHidSmbusExampleDlg::SetStatusText(const CString& text)
{
	// Set the status bar text
	m_statusBar.SetPaneText(1, text);
}

// Populate the device list combo box with connected device serial strings
// - Save previous device serial string selection
// - Fill the device list with connected device serial strings
// - Restore previous device selection
void CHidSmbusExampleDlg::UpdateDeviceList()
{
	// Only update the combo list when the drop down list is closed
	if (!m_comboDeviceList.GetDroppedState())
	{
		int						sel;
		CString					serial;
		DWORD					numDevices;
		HID_SMBUS_DEVICE_STR	deviceString;

		// Get previous combobox string selection
		GetSelectedDevice(serial);

		// Remove all strings from the combobox
		m_comboDeviceList.ResetContent();

		// Get number of HID devices with matching VID/PID (0/0 means not filtered)
		if (HidSmbus_GetNumDevices(&numDevices, VID, PID) == HID_SMBUS_SUCCESS)
		{
			// Iterate through each HID device with matching VID/PID
			for (DWORD i = 0; i < numDevices; i++)
			{
				// Add serial strings to the combobox
				if (HidSmbus_GetString(i, VID, PID, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
				{
					m_comboDeviceList.AddString(CString(deviceString));
				}
			}
		}

		sel = m_comboDeviceList.FindStringExact(-1, serial);

		// Select first combobox string
		if (sel == CB_ERR)
		{
			m_comboDeviceList.SetCurSel(0);
		}
		// Restore previous combobox selection
		else
		{
			m_comboDeviceList.SetCurSel(sel);
		}
	}
}

// Retrieve device information strings and display on the dialog
void CHidSmbusExampleDlg::UpdateDeviceInformation(BOOL connected)
{
	BOOL					opened;
	HID_SMBUS_DEVICE_STR	deviceString;
	WORD					vid;
	WORD					pid;
	WORD					releaseNumber;
	BYTE					partNumber;
	BYTE					version;

	CString					vidString;
	CString					pidString;
	CString					releaseNumberString;
	CString					partNumberString;
	CString					versionString;
	CString					serialString;
	CString					pathString;
	CString					manufacturerString;
	CString					productString;

	// If we're already connected to the device, then we can call the
	// opened version of the device information functions
	if (connected == TRUE &&
		HidSmbus_IsOpened(m_hidSmbus, &opened) == HID_SMBUS_SUCCESS &&
		opened == TRUE)
	{
		// Update device information (opened)

		if (HidSmbus_GetOpenedAttributes(m_hidSmbus, &vid, &pid, &releaseNumber) == HID_SMBUS_SUCCESS)
		{
			vidString.Format(_T("%04X"), vid);
			pidString.Format(_T("%04X"), pid);
			releaseNumberString.Format(_T("%X.%02X"), HIBYTE(releaseNumber), LOBYTE(releaseNumber));
		}
		if (HidSmbus_GetPartNumber(m_hidSmbus, &partNumber, &version) == HID_SMBUS_SUCCESS)
		{
			partNumberString.Format(_T("%d"), partNumber);
			versionString.Format(_T("%d"), version);
		}
		if (HidSmbus_GetOpenedString(m_hidSmbus, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
		{
			serialString = deviceString;
		}
		if (HidSmbus_GetOpenedString(m_hidSmbus, deviceString, HID_SMBUS_GET_PATH_STR) == HID_SMBUS_SUCCESS)
		{
			pathString = deviceString;
		}
		if (HidSmbus_GetOpenedString(m_hidSmbus, deviceString, HID_SMBUS_GET_MANUFACTURER_STR) == HID_SMBUS_SUCCESS)
		{
			manufacturerString = deviceString;
		}
		if (HidSmbus_GetOpenedString(m_hidSmbus, deviceString, HID_SMBUS_GET_PRODUCT_STR) == HID_SMBUS_SUCCESS)
		{
			productString = deviceString;
		}
	}
	else
	{
		CString		serial;
		DWORD		deviceNum;

		// Get selected device serial string
		if (GetSelectedDevice(serial))
		{
			// Find the selected device number
			if (FindDevice(serial, deviceNum))
			{
				// Update device information

				if (HidSmbus_GetAttributes(deviceNum, VID, PID, &vid, &pid, &releaseNumber) == HID_SMBUS_SUCCESS)
				{
					vidString.Format(_T("%04X"), vid);
					pidString.Format(_T("%04X"), pid);
					releaseNumberString.Format(_T("%X.%02X"), HIBYTE(releaseNumber), LOBYTE(releaseNumber));
				}
				if (HidSmbus_GetString(deviceNum, VID, PID, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
				{
					serialString = deviceString;
				}
				if (HidSmbus_GetString(deviceNum, VID, PID, deviceString, HID_SMBUS_GET_PATH_STR) == HID_SMBUS_SUCCESS)
				{
					pathString = deviceString;
				}
				if (HidSmbus_GetString(deviceNum, VID, PID, deviceString, HID_SMBUS_GET_MANUFACTURER_STR) == HID_SMBUS_SUCCESS)
				{
					manufacturerString = deviceString;
				}
				if (HidSmbus_GetString(deviceNum, VID, PID, deviceString, HID_SMBUS_GET_PRODUCT_STR) == HID_SMBUS_SUCCESS)
				{
					productString = deviceString;
				}
			}
		}
	}

	// Update device information on the dialog
	SetDlgItemText(IDC_EDIT_VID,			vidString);
	SetDlgItemText(IDC_EDIT_PID,			pidString);
	SetDlgItemText(IDC_EDIT_RELEASE_NUMBER, releaseNumberString);
	SetDlgItemText(IDC_EDIT_PART_NUMBER,	partNumberString);
	SetDlgItemText(IDC_EDIT_VERSION,		versionString);
	SetDlgItemText(IDC_EDIT_PATH,			pathString);
	SetDlgItemText(IDC_EDIT_SERIAL,			serialString);
	SetDlgItemText(IDC_EDIT_MANUFACTURER,	manufacturerString);
	SetDlgItemText(IDC_EDIT_PRODUCT,		productString);
}

// Enable/disable all of the controls on each of the property
// page dialogs and the reset button
void CHidSmbusExampleDlg::EnableDeviceCtrls(BOOL enable)
{
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(enable);

	EnableDlgControls(m_pageConfiguration.GetSafeHwnd(), enable);
	EnableDlgControls(m_pageDataTransfer.GetSafeHwnd(), enable);
	EnableDlgControls(m_pagePinConfiguration.GetSafeHwnd(), enable);
	EnableDlgControls(m_pageCustomization.GetSafeHwnd(), enable);
	
	if (enable)
	{
		m_pagePinConfiguration.UpdateControlStates();
	}
}

// Get the combobox device selection
// If a device is selected, return TRUE and return the serial string
// Otherwise, return FALSE
BOOL CHidSmbusExampleDlg::GetSelectedDevice(CString& serial)
{
	BOOL selected = FALSE;

	int			sel;
	CString		selText;

	// Get current selection index or CB_ERR(-1)
	// if no device is selected
	sel = m_comboDeviceList.GetCurSel();

	if (sel != CB_ERR)
	{
		// Get the selected device string
		m_comboDeviceList.GetLBText(sel, selText);
		selected	= TRUE;
		serial		= selText;
	}

	return selected;
}

// Search for an HID device with a matching device serial string
// If the device was found return TRUE and return the device number
// in deviceNumber
// Otherwise return FALSE
BOOL CHidSmbusExampleDlg::FindDevice(CString serial, DWORD& deviceNum)
{
	BOOL					found			= FALSE;
	DWORD					numDevices;
	HID_SMBUS_DEVICE_STR	deviceString;

	if (HidSmbus_GetNumDevices(&numDevices, VID, PID) == HID_SMBUS_SUCCESS)
	{
		for (DWORD i = 0; i < numDevices; i++)
		{
			if (HidSmbus_GetString(i, VID, PID, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
			{
				if (serial.CompareNoCase(CString(deviceString)) == 0)
				{
					found		= TRUE;
					deviceNum	= i;
					break;
				}
			}
		}
	}

	return found;
}

// Connect to the device with the serial string selected
// in the device list
// - Connect to the device specified in the device list
// - Update the device information text boxes
// - Set Connect checkbox/button caption and pressed state
// - Enable/disable device combobox
BOOL CHidSmbusExampleDlg::Connect()
{
	BOOL		connected = FALSE;
	CString		serial;
	DWORD		deviceNum;

	// Get selected device serial string
	if (GetSelectedDevice(serial))
	{
		// Find the selected device number
		if (FindDevice(serial, deviceNum))
		{
			HID_SMBUS_STATUS status = HidSmbus_Open(&m_hidSmbus, deviceNum, VID, PID);

			CString statusText;
			statusText.Format(_T("HidSmbus_Open(): %s"), HidSmbus_DecodeErrorStatus(status).GetString());
			SetStatusText(statusText);

			// Attempt to open the device
			if (status == HID_SMBUS_SUCCESS)
			{
				connected = TRUE;
			}
			else
			{
				CString msg;
				msg.Format(_T("Failed to connect to %s: %s"), serial.GetString(), HidSmbus_DecodeErrorStatus(status).GetString());
				MessageBox(msg, 0, MB_ICONWARNING);
			}
		}
	}

	// Connected
	if (connected)
	{
		// Check/press the connect button
		CheckDlgButton(IDC_CHECK_CONNECT, TRUE);

		// Update Connect/Disconnect button caption
		SetDlgItemText(IDC_CHECK_CONNECT, _T("&Disconnect"));

		// Update connection status text
		SetConnectionText(serial);

		// Disable the device combobox
		m_comboDeviceList.EnableWindow(FALSE);

		// Enable all device controls when connected
		EnableDeviceCtrls(TRUE);
	}
	// Disconnected
	else
	{
		// Uncheck/unpress the connect button
		CheckDlgButton(IDC_CHECK_CONNECT, FALSE);

		// Update Connect/Disconnect button caption
		SetDlgItemText(IDC_CHECK_CONNECT, _T("&Connect"));

		// Update connection status text
		SetConnectionText(_T("Not Connected"));

		// Enable the device combobox
		m_comboDeviceList.EnableWindow(TRUE);

		// Disable all device controls when disconnected
		EnableDeviceCtrls(FALSE);
	}

	// Update the device information now that we are connected to it
	// (this will give us the part number and version if connected)
	UpdateDeviceInformation(TRUE);

	// Update all device settings for all property pages
	m_pageConfiguration.SetFromDevice();
	m_pageDataTransfer.SetFromDevice();
	m_pagePinConfiguration.SetFromDevice();
	m_pageCustomization.SetFromDevice();

	return connected;
}

// Disconnect from the currently connected device
// - Disconnect from the current device
// - Output any error messages
// - Display "Not Connected" in the status bar
// - Update the device information text boxes (clear text)
// - Set Connect checkbox/button caption and pressed state
// - Enable/disable device combobox
BOOL CHidSmbusExampleDlg::Disconnect()
{
	BOOL disconnected = FALSE;

	// Disconnect from the current device
	HID_SMBUS_STATUS status = HidSmbus_Close(m_hidSmbus);
	m_hidSmbus = NULL;

	CString statusText;
	statusText.Format(_T("HidSmbus_Close(): %s"), HidSmbus_DecodeErrorStatus(status).GetString());
	SetStatusText(statusText);

	// Output an error message if the close failed
	if (status != HID_SMBUS_SUCCESS)
	{
		CString msg;
		msg.Format(_T("Failed to disconnect: %s"), HidSmbus_DecodeErrorStatus(status).GetString());
		MessageBox(msg, 0, MB_ICONWARNING);
	}
	else
	{
		disconnected = TRUE;
	}

	// Uncheck/unpress the connect button
	CheckDlgButton(IDC_CHECK_CONNECT, FALSE);

	// Update Connect/Disconnect button caption
	SetDlgItemText(IDC_CHECK_CONNECT, _T("&Connect"));

	// Update connection status text
	SetConnectionText(_T("Not Connected"));

	// Enable the device combobox
	m_comboDeviceList.EnableWindow(TRUE);

	// Disable all device controls when disconnected
	EnableDeviceCtrls(FALSE);

	// Update the device information (clear)
	UpdateDeviceInformation(TRUE);

	return disconnected;
}

/////////////////////////////////////////////////////////////////////////////
// CHidSmbusExampleDlg - Surprise Removal Protected Methods
/////////////////////////////////////////////////////////////////////////////

// Register for device change notification for USB HID devices
// OnDeviceChange() will handle device arrival and removal
void CHidSmbusExampleDlg::RegisterDeviceChange()
{
	DEV_BROADCAST_DEVICEINTERFACE devIF = {0};

	devIF.dbcc_size			= sizeof(devIF);    
	devIF.dbcc_devicetype	= DBT_DEVTYP_DEVICEINTERFACE;    
	
	HidSmbus_GetHidGuid(&devIF.dbcc_classguid);
	
	m_hNotifyDevNode = RegisterDeviceNotification(GetSafeHwnd(), &devIF, DEVICE_NOTIFY_WINDOW_HANDLE);
}

// Unregister for device change notification for USB HID devices
void CHidSmbusExampleDlg::UnregisterDeviceChange()
{
	if (m_hNotifyDevNode)
	{
		UnregisterDeviceNotification(m_hNotifyDevNode);
		m_hNotifyDevNode = NULL;
	}
}
