#include "StdAfx.h"
#include "SocketServer.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>


SocketServer::SocketServer(Config &cfg, std::wofstream &pLog) : cfg_(cfg), log(pLog) {
	udpPort_ = _wtoi(CFG(cfg_, UDP_PORT).c_str());
	sensorPort_ = CFG(cfg_, SENSOR_PORT);
	bitRate_ = _wtoi(CFG(cfg_, SENSOR_PORT_RATE).c_str());
	log << _T("Start UDP Server -  port: ") << udpPort_ 
		<< _T(" SensorPort: ") << sensorPort_ << _T(" SensorSpeed: ") << bitRate_ << std::endl;

	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2 ,2), &wsaData) != 0) {
		log << "error at WSASturtup" << GetLastError() << std::endl;
	}

	socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if( socket_ < 0) {
		log << "Can't create socket: " << GetLastError() << std::endl;
	}
	int broadcast=1;
	std::string recv;
	setsockopt(socket_, SOL_SOCKET, SO_BROADCAST, (const char *) &broadcast, sizeof(broadcast));
	initSerialPort();
	CreateThread( NULL, 0, SocketServer::loop, this, 0, NULL);  


}
SocketServer::~SocketServer(void)
{
}

void SocketServer::send2Client(const char *data, int len) {
	sockaddr_in addr;
	addr.sin_family = AF_INET;
    addr.sin_port = htons(udpPort_);
    addr.sin_addr.s_addr = inet_addr("127.255.255.255");
	if(sendto(socket_, data, len, 0, (const sockaddr *)&addr, sizeof(addr)) < 0) {
		log << "Can't sendto socket: " << GetLastError() << std::endl;
	}
}

void SocketServer::initSerialPort() {
	hComm = CreateFile(sensorPort_.c_str(),
				GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

	if( hComm  ==  INVALID_HANDLE_VALUE) {
		log << "CreateFile COM_PORT error code: " << GetLastError()  << std::endl;
		return;
	}
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hComm, &dcbSerialParams) == FALSE ) {
		log << "GetCommState  error code: " << GetLastError()  << std::endl;
		CloseHandle(hComm);
		hComm = INVALID_HANDLE_VALUE;
		return;
	}
	dcbSerialParams.BaudRate = bitRate_;      //BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             //ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    //StopBits = 1
    dcbSerialParams.Parity = NOPARITY;      //Parity = None
	dcbSerialParams.fOutxCtsFlow = FALSE;
	dcbSerialParams.fOutxDsrFlow = FALSE;

	if (SetCommState(hComm, &dcbSerialParams) == FALSE ) {
		log << "SetCommState  error code: " << GetLastError()  << std::endl;
		CloseHandle(hComm);
		hComm = INVALID_HANDLE_VALUE;
		return;
	}

	// instance an object of COMMTIMEOUTS.
	COMMTIMEOUTS comTimeOut;                   
	// Specify time-out between charactor for receiving.
	comTimeOut.ReadIntervalTimeout = 3;
	// Specify value that is multiplied 
	// by the requested number of bytes to be read. 
	comTimeOut.ReadTotalTimeoutMultiplier = 3;
	// Specify value is added to the product of the 
	// ReadTotalTimeoutMultiplier member
	comTimeOut.ReadTotalTimeoutConstant = 2;
	// Specify value that is multiplied 
	// by the requested number of bytes to be sent. 
	comTimeOut.WriteTotalTimeoutMultiplier = 3;
	// Specify value is added to the product of the 
	// WriteTotalTimeoutMultiplier member
	comTimeOut.WriteTotalTimeoutConstant = 2;
	// set the time-out parameter into device control.
	if(!SetCommTimeouts(hComm,&comTimeOut)){
		log << "SetCommTimeouts error code: " << GetLastError()  << std::endl;
		CloseHandle(hComm);
		hComm = INVALID_HANDLE_VALUE;
		return;
	}
	memset(&ov, 0, sizeof(ov));
}


void SocketServer::serialSend(const char *data, int data_len)
{
	for(int i = 0; i < data_len; i++) {
		char c = data[i];
		TransmitCommChar(hComm, c);
	}
};


DWORD WINAPI SocketServer::loop( LPVOID lpParam ) 
{
	SocketServer *myThis = (SocketServer *) lpParam;
	for(;;) {
		char cBuffer[128] = "\0";
		DWORD dwSize = 128, dwRead = 0;
		if(!ReadFile(myThis->hComm, &cBuffer, dwSize, &dwRead, NULL)) {
			myThis->log << 	_T("Error ReadFile: ") << GetLastError() << std::endl;
		} else {
			myThis->send2Client(cBuffer, dwRead);
		}
	}
    return 0; 
} 
