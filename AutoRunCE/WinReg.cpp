#include "StdAfx.h"
#include "WinReg.h"

CWinReg::CWinReg(std::wofstream& pLog) : log(pLog)
{
}

CWinReg::~CWinReg(void)
{
}

void CWinReg::RegWriteString(HKEY key, wchar_t *subKey, wchar_t* name, wchar_t* value) {
	RegWrite(key, subKey, name, REG_SZ, (LPBYTE) value, (wcslen(value)+1)*sizeof(wchar_t));
}

void CWinReg::RegWriteDword(HKEY key, wchar_t *subKey, wchar_t* name, DWORD value) {
	RegWrite(key, subKey, name, REG_DWORD, (LPBYTE) &value, sizeof(DWORD));
}

void CWinReg::RegWrite(HKEY key, wchar_t *subKey, wchar_t* name, DWORD type, LPBYTE data, DWORD size) {
		HKEY hKey;			
		if(ERROR_SUCCESS == RegOpenKey(key,  subKey, KEY_ALL_ACCESS, hKey)) 
		{ 
			if(ERROR_SUCCESS != RegSetValueEx(hKey, name, 0, type, data, size))
			{
				log << _T("RegSetValueEx: ") << GetLastError() << std::endl;
			} 
		 
			RegCloseKey (hKey); 
		}
}

DWORD CWinReg::RegReadDword(HKEY key, wchar_t *subKey, wchar_t* name, DWORD defaultValue) {
	DWORD type=0;
	DWORD rv=0;
	DWORD size=sizeof(DWORD);
	RegRead(key, subKey, name, type, (LPBYTE) &rv, size);
	return (type == REG_DWORD) && size == sizeof(DWORD) ? rv : defaultValue;
}

void CWinReg::RegRead(HKEY key, wchar_t *subKey, wchar_t* name, DWORD &type, LPBYTE data, DWORD &size) {
	HKEY hKey;			
	if(ERROR_SUCCESS == RegOpenKey(key,  subKey, KEY_ALL_ACCESS, hKey)) 
	{ 
		if(ERROR_SUCCESS != RegQueryValueEx(hKey, name, 0, &type, data, &size))
		{
			log << _T("RegQueryValueEx: ") << key << _T(" : ") << name << _T(" : ")<< GetLastError() << std::endl;
		} 
	 
		RegCloseKey (hKey); 
	} 
}

LSTATUS CWinReg::RegOpenKey(HKEY key, wchar_t *subKey, REGSAM mask, HKEY &hKey) {
	LSTATUS status = RegOpenKeyEx(key,  subKey, 0, mask, &hKey);
	DWORD error = GetLastError();
	wchar_t *funcName = _T("RegOpenKeyEx: ");
	if(status != ERROR_SUCCESS && error == ERROR_RESOURCE_NAME_NOT_FOUND) {
		DWORD desc;
		status = RegCreateKeyEx(key, subKey, 0, NULL, REG_OPTION_NON_VOLATILE, mask, NULL, &hKey , &desc);
		error = GetLastError();
		funcName = _T("RegCreateKeyEx: ");
	}
	if(status != ERROR_SUCCESS) {
			log << funcName << error << std::endl;
	}
	return status;
}
