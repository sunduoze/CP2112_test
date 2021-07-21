/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CustomDDX.h"
#include <AfxPriv.h>
#include <limits>

/////////////////////////////////////////////////////////////////////////////
// Static Global Function Prototypes
/////////////////////////////////////////////////////////////////////////////

template <class T> static void TextPlus(CDataExchange* pDX, int nIDC, T& value);
template <class T> static void UnsignedTextHex(CDataExchange* pDX, int nIDC, T& value);
template <class T> static void UnsignedMaxCharsHex(CDataExchange* pDX, T value);

template <class T> static void Even(CDataExchange* pDX, T value);
template <class T> static void Odd(CDataExchange* pDX, T value);
template <class T> static void UnsignedMinMaxHex(CDataExchange* pDX, T value, T minVal, T maxVal);

/////////////////////////////////////////////////////////////////////////////
// Static Global Functions
/////////////////////////////////////////////////////////////////////////////

// Used for enhanced version of standard DDX_Text() called DDX_TextPlus()
// - Checks for invalid characters (valid characters are 0-9 and +/-)
// - Checks for overflow
template <class T> void TextPlus(CDataExchange* pDX, int nIDC, T& value)
{
	HWND	hWndCtrl	= pDX->PrepareEditCtrl(nIDC);
	BOOL	isSigned	= (std::numeric_limits<T>::is_signed);

	// UpdateData(TRUE)
	if (pDX->m_bSaveAndValidate)
    {
		CString		valueStr;
		CString		validChars;
		TCHAR*		endPtr;

		// Get the text from the edit box
        int nLen = ::GetWindowTextLength(hWndCtrl);
        ::GetWindowText(hWndCtrl, valueStr.GetBufferSetLength(nLen), nLen + 1);
        valueStr.ReleaseBuffer();

		validChars = (isSigned) ? _T("+-0123456789") : _T("+0123456789");

		if (valueStr.SpanIncluding(validChars).GetLength() != valueStr.GetLength())
		{
			if (isSigned)
			{
				AfxMessageBox(_T("Please enter an integer."), MB_ICONEXCLAMATION);
			}
			else
			{
				AfxMessageBox(_T("Please enter a positive integer."), MB_ICONEXCLAMATION);
			}
			pDX->Fail();
		}
		else
		{
			errno = 0;

			if (isSigned)
			{
				// Convert to a numeric value
				LONG lValue		= _tcstol(valueStr, &endPtr, 10);
				LONG maxValue	= (std::numeric_limits<T>::max)();
				LONG minValue	= (std::numeric_limits<T>::min)();

				if (errno == ERANGE || lValue > maxValue || lValue < minValue)
				{
					AfxMessageBox(_T("Value overflow"), MB_ICONEXCLAMATION);
					pDX->Fail();
				}
				else
				{
					value = (T)lValue;
				}
			}
			else
			{
				// Convert to a numeric value
				ULONG ulValue	= _tcstoul(valueStr, &endPtr, 10);
				ULONG maxValue	= (std::numeric_limits<T>::max)();

				if (errno == ERANGE || ulValue > maxValue)
				{
					AfxMessageBox(_T("Value overflow"), MB_ICONEXCLAMATION);
					pDX->Fail();
				}
				else
				{
					value = (T)ulValue;
				}
			}
		}
    }
	// UpdateData(FALSE);
    else
    {
		if (isSigned)
		{
			CString valueStr;
			valueStr.Format(_T("%d"), value);

			// Set the edit box text
			AfxSetWindowText(hWndCtrl, valueStr);
		}
		else
		{
			CString valueStr;
			valueStr.Format(_T("%u"), value);

			// Set the edit box text
			AfxSetWindowText(hWndCtrl, valueStr);
		}
    }
}

// Used for custom DDX_TextHex()
// - Valid characters are hexadecimal characters 0-9, a-f, A-F
// - Checks for overflow
template <class T> void UnsignedTextHex(CDataExchange* pDX, int nIDC, T& value)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
    
	ASSERT((std::numeric_limits<T>::is_signed) == false);

	// UpdateData(TRUE)
	if (pDX->m_bSaveAndValidate)
    {
		CString valueStr;
		TCHAR*	endPtr;

		// Get the text from the edit box
        int nLen = ::GetWindowTextLength(hWndCtrl);
        ::GetWindowText(hWndCtrl, valueStr.GetBufferSetLength(nLen), nLen + 1);
        valueStr.ReleaseBuffer();

		if (valueStr.SpanIncluding(_T("0123456789ABCDEFabcdef")).GetLength() != valueStr.GetLength())
		{
			AfxMessageBox(_T("Please enter a hexadecimal value."), MB_ICONEXCLAMATION);
			pDX->Fail();
		}
		else
		{
			errno = 0;

			// Convert to a numeric value (hexadecimal)
			ULONG ulValue	= _tcstoul(valueStr, &endPtr, 16);
			ULONG maxValue	= (std::numeric_limits<T>::max)();

			if (errno == ERANGE || ulValue == ULONG_MAX || ulValue > maxValue)
			{
				AfxMessageBox(_T("Value overflow"), MB_ICONEXCLAMATION);
				pDX->Fail();
			}
			else
			{
				value = (T)ulValue;
			}
		}
    }
	// UpdateData(FALSE);
    else
    {
		CString formatStr;
		CString valueStr;

		int nChars = sizeof(T) * 2;

		valueStr.Format(_T("%0*X"), nChars, value);

		// Set the edit box text
		AfxSetWindowText(hWndCtrl, valueStr);
    }
}

// Used for custom DDV_MaxCharsHex()
// - Limits the number of characters allowed in an edit box
template <class T> void UnsignedMaxCharsHex(CDataExchange* pDX, T value)
{
	CString valueStr;
	valueStr.Format(_T("%X"), value);

	int nChars = sizeof(T) * 2;

	if (pDX->m_bSaveAndValidate && valueStr.GetLength() > nChars)
	{
		TCHAR szT[32];
		_stprintf_s(szT, _countof(szT), _T("%d"), nChars);
		CString prompt;
		AfxFormatString1(prompt, AFX_IDP_PARSE_STRING_SIZE, szT);
		AfxMessageBox(prompt, MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE);
		prompt.Empty(); // exception prep
		pDX->Fail();
	}
	else if (pDX->m_idLastControl != 0 && pDX->m_bEditLastControl)
	{
		HWND hWndLastControl;
		pDX->m_pDlgWnd->GetDlgItem(pDX->m_idLastControl, &hWndLastControl);
		// limit the control max-chars automatically
		::SendMessage(hWndLastControl, EM_LIMITTEXT, nChars, 0);
	}
}

// Used for custom DDV_Even()
// - Restricts valid entries to even numbers only
template <class T> void Even(CDataExchange* pDX, T value)
{
	if (pDX->m_bSaveAndValidate)
	{
		if (value % 2 != 0)
		{
			CString prompt;
			prompt.Format(_T("Please enter an even integer."));

			AfxMessageBox(prompt, MB_ICONEXCLAMATION);
			prompt.Empty(); // exception prep
			pDX->Fail();
		}
	}
}

// Used for custom DDV_Odd()
// - Restricts valid entries to odd numbers only
template <class T> void Odd(CDataExchange* pDX, T value)
{
	if (pDX->m_bSaveAndValidate)
	{
		if (value % 2 == 0)
		{
			CString prompt;
			prompt.Format(_T("Please enter an odd integer."));

			AfxMessageBox(prompt, MB_ICONEXCLAMATION);
			prompt.Empty(); // exception prep
			pDX->Fail();
		}
	}
}

// Used for custom DDV_MinMaxHex()
// - Restricts valid entries to hexadecimal numbers in the range [minVal, maxVal]
template <class T> void UnsignedMinMaxHex(CDataExchange* pDX, T value, T minVal, T maxVal)
{
	int nChars = sizeof(T) * 2;

	ASSERT(std::numeric_limits<T>::is_signed == false);

	if (pDX->m_bSaveAndValidate)
	{
		if (value < minVal || value > maxVal)
		{
			CString prompt;
			prompt.Format(_T("Please enter an integer between 0x%0*X and 0x%0*X."), nChars, minVal, nChars, maxVal);
			AfxMessageBox(prompt, MB_ICONEXCLAMATION);
			prompt.Empty(); // exception prep
			pDX->Fail();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Dialog Data Exchange Functions
/////////////////////////////////////////////////////////////////////////////

// Enhanced version of standard DDX_Text()
// - Checks for invalid characters (valid characters are 0-9)
// - Checks for overflow

void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, BYTE& value)
{
	TextPlus<BYTE>(pDX, nIDC, value);
}

void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, WORD& value)
{
	TextPlus<WORD>(pDX, nIDC, value);
}

void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, DWORD& value)
{
	TextPlus<DWORD>(pDX, nIDC, value);
}

void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, UINT& value)
{
	TextPlus<UINT>(pDX, nIDC, value);
}

void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, char& value)
{
	TextPlus<char>(pDX, nIDC, value);
}

void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, short& value)
{
	TextPlus<short>(pDX, nIDC, value);
}

void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, long& value)
{
	TextPlus<long>(pDX, nIDC, value);
}

void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, int& value)
{
	TextPlus<int>(pDX, nIDC, value);
}

// Custom hexadecimal text exchange
// - Valid characters are hexadecimal characters 0-9, a-f, A-F
// - Checks for overflow

void AFXAPI DDX_TextHex(CDataExchange* pDX, int nIDC, BYTE& value)
{
	UnsignedTextHex<BYTE>(pDX, nIDC, value);
}

void AFXAPI DDX_TextHex(CDataExchange* pDX, int nIDC, WORD& value)
{
	UnsignedTextHex<WORD>(pDX, nIDC, value);
}

void AFXAPI DDX_TextHex(CDataExchange* pDX, int nIDC, DWORD& value)
{
	UnsignedTextHex<DWORD>(pDX, nIDC, value);
}

void AFXAPI DDX_TextHex(CDataExchange* pDX, int nIDC, UINT& value)
{
	UnsignedTextHex<UINT>(pDX, nIDC, value);
}

/////////////////////////////////////////////////////////////////////////////
// Dialog Data Validation Functions
/////////////////////////////////////////////////////////////////////////////

// Custom hexadecimal text validation
// - Limits the number of characters allowed in an edit box (based on data type)

void AFXAPI DDV_MaxCharsHex(CDataExchange* pDX, BYTE value)
{
	UnsignedMaxCharsHex<BYTE>(pDX, value);
}

void AFXAPI DDV_MaxCharsHex(CDataExchange* pDX, WORD value)
{
	UnsignedMaxCharsHex<WORD>(pDX, value);
}

void AFXAPI DDV_MaxCharsHex(CDataExchange* pDX, DWORD value)
{
	UnsignedMaxCharsHex<DWORD>(pDX, value);
}

void AFXAPI DDV_MaxCharsHex(CDataExchange* pDX, UINT value)
{
	UnsignedMaxCharsHex<UINT>(pDX, value);
}

// Custom hexadecimal range validation
// - Restricts valid entries to hexadecimal numbers in the range [minVal, maxVal]

void AFXAPI DDV_MinMaxHex(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal)
{
	UnsignedMinMaxHex<BYTE>(pDX, value, minVal, maxVal);
}

void AFXAPI DDV_MinMaxHex(CDataExchange* pDX, WORD value, WORD minVal, WORD maxVal)
{
	UnsignedMinMaxHex<WORD>(pDX, value, minVal, maxVal);
}

void AFXAPI DDV_MinMaxHex(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal)
{
	UnsignedMinMaxHex<DWORD>(pDX, value, minVal, maxVal);
}

void AFXAPI DDV_MinMaxHex(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal)
{
	UnsignedMinMaxHex<UINT>(pDX, value, minVal, maxVal);
}


// Custom integer validation
// - Restricts valid entries to even numbers only

void AFXAPI DDV_Even(CDataExchange* pDX, BYTE value)
{
	Even<BYTE>(pDX, value);
}

void AFXAPI DDV_Even(CDataExchange* pDX, WORD value)
{
	Even<WORD>(pDX, value);
}

void AFXAPI DDV_Even(CDataExchange* pDX, DWORD value)
{
	Even<DWORD>(pDX, value);
}

void AFXAPI DDV_Even(CDataExchange* pDX, UINT value)
{
	Even<UINT>(pDX, value);
}

void AFXAPI DDV_Even(CDataExchange* pDX, char value)
{
	Even<char>(pDX, value);
}

void AFXAPI DDV_Even(CDataExchange* pDX, short value)
{
	Even<short>(pDX, value);
}

void AFXAPI DDV_Even(CDataExchange* pDX, long value)
{
	Even<long>(pDX, value);
}

void AFXAPI DDV_Even(CDataExchange* pDX, int value)
{
	Even<int>(pDX, value);
}

// Custom integer validation
// - Restricts valid entries to odd numbers only

void AFXAPI DDV_Odd(CDataExchange* pDX, BYTE value)
{
	Odd<BYTE>(pDX, value);
}

void AFXAPI DDV_Odd(CDataExchange* pDX, WORD value)
{
	Odd<WORD>(pDX, value);
}

void AFXAPI DDV_Odd(CDataExchange* pDX, DWORD value)
{
	Odd<DWORD>(pDX, value);
}

void AFXAPI DDV_Odd(CDataExchange* pDX, UINT value)
{
	Odd<UINT>(pDX, value);
}

void AFXAPI DDV_Odd(CDataExchange* pDX, char value)
{
	Odd<char>(pDX, value);
}

void AFXAPI DDV_Odd(CDataExchange* pDX, short value)
{
	Odd<short>(pDX, value);
}

void AFXAPI DDV_Odd(CDataExchange* pDX, long value)
{
	Odd<long>(pDX, value);
}

void AFXAPI DDV_Odd(CDataExchange* pDX, int value)
{
	Odd<int>(pDX, value);
}
