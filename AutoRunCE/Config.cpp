#include "StdAfx.h"
#include "Config.h"
#include <sstream>
#include <fstream>


std::wstring itows(int  val) {
	std::wostringstream stream;
	stream << val;
	return stream.str();
}

std::wstring getFileNameFor(FileType type) {
	std::wstring rv;
	rv.resize(MAX_PATH);
	DWORD size = GetModuleFileName(NULL, &rv[0], MAX_PATH);
	wchar_t *replace = wcsstr(&rv[0], _T(".exe"));
	if(replace != NULL) {
		if(type == INI)
			wcscpy(replace, _T(".ini"));
		else if(type == LOG)
			wcscpy(replace, _T(".log"));
	}
	if(size > 0) {
		rv.resize(size);
	}
	return rv;
}



Config::Config(std::wofstream &log) : std::map<std::wstring,std::wstring>(), log_(log), 
	sensi_(3), volume_(5), barrUp_(25), barrDw_(100)
{
	initConfig();
	hEventReciveData = CreateEvent(NULL, TRUE, FALSE, _T("HW_VARIO_RECIVE_DATA"));
}

Config::~Config() 
{}

void Config::initConfig() {
	std::wstring fileName = getFileNameFor(INI);
	log_<< _T("FileName: ") << fileName << std::endl;
	std::wifstream iniFile(fileName.c_str());
	std::wstring line;
	if(iniFile.good()) {
		while( std::getline(iniFile, line) )
		{
			log_<< _T("Line: ") << line << std::endl;
			std::wistringstream is_line(line, std::ios_base::in);
			std::wstring key;
			if(std::getline(is_line, key, _T('=')))
			{
				std::wstring value;
				if( std::getline(is_line, value) ) {
					log_<< _T("Set key: ") << key << _T(" value: ") << value << std::endl;
					(*this)[key] = value;
				}
		  }
		}
	}
}

void Config::parse(const char *data, int len) {
	static char state = 0x00;
	if(len > 6) {
		//log_ << data << std::endl;
		if(strncmp(data, BVL, 4)==0) {
			sscanf(data, BVL, &volume_);
			state |= 0x01;
			log_ << _T(BVL) <<_T(" read: ") << volume_ << std::endl;
		} else if(strncmp(data, SEN, 4)==0) {
			sscanf(data, SEN, &sensi_);
			log_ << _T(SEN) << _T(" read: ") << sensi_ << std::endl;
			state |= 0x02;
		} else if(strncmp(data, BUP, 4)==0) {
			sscanf(data, BUP, &barrUp_);
			log_ << _T(BUP) << _T(" read: ")<< barrUp_ << std::endl;
			state |= 0x04;
		} else if(strncmp(data, BDW, 4)==0) {
			sscanf(data, BDW, &barrDw_);
			log_ << _T(BDW) << _T(" read: ") << barrDw_ << std::endl;
			
			state |= 0x08;
		} 
		if((state & 0x0F) == 0x0F) {
			state = 0x00;
			log_ << _T("SetEvent(hEventReciveData)") << std::endl;
			SetEvent(hEventReciveData);
		}
	}
}

const std::wstring& Config::get(const std::wstring &key, const std::wstring &defValue) const {
	log_<< _T("Get key: ") << key << _T(" default value: ") << defValue << std::endl;
	const_iterator it = find(key);
	return (it == end()) ? defValue : it->second;
}

const std::wstring& Config::get(const std::wstring &key, int pos, const std::wstring defValues[], int defValuesSize) const {
	std::wostringstream keyNewStream;
	keyNewStream << key << _T("_") << pos;
	return get(keyNewStream.str(), defValues[min(pos, defValuesSize - 1)]); 
}
