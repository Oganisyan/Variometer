#pragma once
#include <map>
#include <string>
#include <ostream>

#define UDP_PORT			_T("8880")
#define SENSOR_PORT			_T("\\$device\\COM4")
#define SENSOR_PORT_RATE	_T("57600")

#define BIKE_NAV_PATH		_T("\\My Flash Disk\\Navigator\\Navigator.exe")
#define XC_SOAR_PATH		_T("\\Storage Card\\XCSoarData\\XCSoar.exe")
#define WIN_EXPLORER_PATH	_T("\\Windows\\explorer.exe")

#define XC_SOAR_WIN_NAME	_T("XCSoarMain")


const std::wstring	SENSOR_VOLCMD[] = {
	std::wstring(_T("$BVL 0*")), 
	std::wstring(_T("$BVL 1*")), 
	std::wstring(_T("$BVL 2*")), 
	std::wstring(_T("$BVL 3*")), 
	std::wstring(_T("$BVL 4*")), 
	std::wstring(_T("$BVL 5*")), 
	std::wstring(_T("$BVL 6*")),
	std::wstring(_T("$BVL 7*")),
	std::wstring(_T("$BVL 8*")),
	std::wstring(_T("$BVL 9*")), 
	std::wstring(_T("$BVL 10*"))
};


enum FileType  {
	LOG,
	INI
};

std::wstring getFileNameFor(FileType type);

std::wstring itows(int  val);

#define CFG(x, y) x.get(_T(#y), y)
#define CFG_LIST_ITEM(x, y, z) x.get(_T(#y), z, y)
#define CFG_LIST_COUNT(x, y) x.get(_T(#y)##_T("_COUNT"), itows(sizeof(y) / sizeof(std::wstring)))

class Config : private std::map<std::wstring,std::wstring>
{
	std::wofstream &log_;
	void initConfig();
public:
	Config(std::wofstream &log);
	~Config();
	const std::wstring& get(const std::wstring &key, const std::wstring &defValue) const;
	const std::wstring& get(const std::wstring &key, int pos, const std::wstring defValues[]) const;
};

