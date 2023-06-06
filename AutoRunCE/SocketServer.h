#pragma once
#include <fstream>
#include "Config.h"


class SocketServer
{
	static DWORD WINAPI loop(LPVOID lpParam);

	HANDLE hComm;
	int udpPort_;
	std::wstring sensorPort_;
	unsigned int bitRate_;
	int socket_;
	std::wofstream &log;
	OVERLAPPED ov;
	Config &cfg_;

	void broadcast(const char *data, int data_len);
public:
	SocketServer(Config &cfg, std::wofstream &pLog);
	~SocketServer(void);
	void serialSend(const char *data, int data_len);
	void OpenSerialPort();
	void CloseSerialPort();

};
