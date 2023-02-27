#pragma once
#include <fstream>


class CWinReg
{
	std::wofstream& log;
public:
	CWinReg(std::wofstream& pLog);
	~CWinReg(void);
	DWORD RegReadDword(HKEY key, wchar_t *subKey, wchar_t* name, DWORD defaultValue);
	void RegRead(HKEY key, wchar_t *subKey, wchar_t* name, DWORD &type, LPBYTE data, DWORD &size);
	LSTATUS RegOpenKey(HKEY key, wchar_t *subKey, REGSAM mask, HKEY &hKey);

	void RegWriteString(HKEY key, wchar_t *subKey, wchar_t* name, wchar_t* value);
	void RegWriteDword(HKEY key, wchar_t *subKey, wchar_t* name, DWORD value);
	void RegWrite(HKEY key, wchar_t *subKey, wchar_t* name, DWORD type,  LPBYTE data, DWORD size);
};
