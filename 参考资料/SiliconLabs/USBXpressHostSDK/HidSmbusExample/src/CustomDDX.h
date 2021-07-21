#pragma once

/////////////////////////////////////////////////////////////////////////////
// Dialog Data Exchange Functions
/////////////////////////////////////////////////////////////////////////////

// Enhanced version of standard DDX_Text() exchange for integers
// - Checks for invalid characters (valid characters are 0-9, +/-)
// - Checks for overflow

void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, BYTE& value);
void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, WORD& value);
void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, DWORD& value);
void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, UINT& value);
void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, char& value);
void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, short& value);
void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, long& value);
void AFXAPI DDX_TextPlus(CDataExchange* pDX, int nIDC, int& value);

// Custom hexadecimal text exchange
// - Valid characters are hexadecimal characters 0-9, a-f, A-F
// - Checks for overflow

void AFXAPI DDX_TextHex(CDataExchange* pDX, int nIDC, BYTE& value);
void AFXAPI DDX_TextHex(CDataExchange* pDX, int nIDC, WORD& value);
void AFXAPI DDX_TextHex(CDataExchange* pDX, int nIDC, DWORD& value);
void AFXAPI DDX_TextHex(CDataExchange* pDX, int nIDC, UINT& value);

/////////////////////////////////////////////////////////////////////////////
// Dialog Data Validation Functions
/////////////////////////////////////////////////////////////////////////////

// Custom hexadecimal text validation
// - Limits the number of characters allowed in an edit box (based on data type)

void AFXAPI DDV_MaxCharsHex(CDataExchange* pDX, BYTE value);
void AFXAPI DDV_MaxCharsHex(CDataExchange* pDX, WORD value);
void AFXAPI DDV_MaxCharsHex(CDataExchange* pDX, DWORD value);
void AFXAPI DDV_MaxCharsHex(CDataExchange* pDX, UINT value);

// Custom hexadecimal range validation
// - Restricts valid entries to hexadecimal numbers in the range [minVal, maxVal]

void AFXAPI DDV_MinMaxHex(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
void AFXAPI DDV_MinMaxHex(CDataExchange* pDX, WORD value, WORD minVal, WORD maxVal);
void AFXAPI DDV_MinMaxHex(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
void AFXAPI DDV_MinMaxHex(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);

// Custom integer validation
// - Restricts valid entries to even numbers only

void AFXAPI DDV_Even(CDataExchange* pDX, BYTE value);
void AFXAPI DDV_Even(CDataExchange* pDX, WORD value);
void AFXAPI DDV_Even(CDataExchange* pDX, DWORD value);
void AFXAPI DDV_Even(CDataExchange* pDX, UINT value);
void AFXAPI DDV_Even(CDataExchange* pDX, char value);
void AFXAPI DDV_Even(CDataExchange* pDX, short value);
void AFXAPI DDV_Even(CDataExchange* pDX, long value);
void AFXAPI DDV_Even(CDataExchange* pDX, int value);

// Custom integer validation
// - Restricts valid entries to odd numbers only

void AFXAPI DDV_Odd(CDataExchange* pDX, BYTE value);
void AFXAPI DDV_Odd(CDataExchange* pDX, WORD value);
void AFXAPI DDV_Odd(CDataExchange* pDX, DWORD value);
void AFXAPI DDV_Odd(CDataExchange* pDX, UINT value);
void AFXAPI DDV_Odd(CDataExchange* pDX, char value);
void AFXAPI DDV_Odd(CDataExchange* pDX, short value);
void AFXAPI DDV_Odd(CDataExchange* pDX, long value);
void AFXAPI DDV_Odd(CDataExchange* pDX, int value);
