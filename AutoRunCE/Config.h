#ifndef _CONFIG_H_
#define _CONFIG_H_
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


#define BVL		"$BVL %d*"
#define SIM		"$SIM %d*"
#define BUP		"$BUP %d*"
#define BDW		"$BDW %d*"
#define SEN		"$SEN %d*"
#define UPD		"$UPD *"
#define GET		"$? *"

#define CRNL(x)	x"\n"


enum FileType  {
	LOG,
	INI
};

std::wstring getFileNameFor(FileType type);

std::wstring itows(int  val);

#define CFG(x, y) x.get(_T(#y), y)
#define CFG_LIST_ITEM(x, y, z) x.get(_T(#y), z, y, sizeof(y))
#define CFG_LIST_COUNT(x, y) x.get(_T(#y)##_T("_COUNT"), itows(sizeof(y) / sizeof(std::wstring)))

class Config : private std::map<std::wstring,std::wstring>
{
	std::wofstream &log_;
	void initConfig();
	int sensi_;
	int volume_;
	int barrUp_;
	int barrDw_;
	HANDLE hEventReciveData;
 
public:
	Config(std::wofstream &log);
	~Config();
	HANDLE getHandleEventReciveData() {return hEventReciveData;}
	void parse(const char *data, int len);
	const std::wstring& get(const std::wstring &key, const std::wstring &defValue) const;
	const std::wstring& get(const std::wstring &key_prefix, int pos, const std::wstring defValues[], int defValuesSize) const;
	int getVolume() const { return volume_;}
	int getSensi() const { return sensi_;}
	int getBarrUp() const { return barrUp_;}
	int getBarrDw() const { return barrDw_;}

	void setVolume(int volume) { volume_ = volume;}
	void setSensi(int sensi)   { sensi_  = sensi;}
	void setBarrUp(int barrUp) { barrUp_ = barrUp;}
	void setBarrDw(int barrDw) { barrDw_ = barrDw;}

};
#endif //_CONFIG_H_
