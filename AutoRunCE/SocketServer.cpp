#include "StdAfx.h"
#include "SocketServer.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>


SocketServer::SocketServer(Config &cfg, std::wofstream &pLog) : cfg_(cfg), log_(pLog), hComm(INVALID_HANDLE_VALUE) {
	udpPort_ = _wtoi(CFG(cfg_, UDP_PORT).c_str());
	sensorPort_ = CFG(cfg_, SENSOR_PORT);
	bitRate_ = _wtoi(CFG(cfg_, SENSOR_PORT_RATE).c_str());
	log_ << _T("Start UDP Server -  port: ") << udpPort_ 
		<< _T(" SensorPort: ") << sensorPort_ << _T(" SensorSpeed: ") << bitRate_ << std::endl;

	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2 ,2), &wsaData) != 0) {
		log_ << "error at WSASturtup" << GetLastError() << std::endl;
	}

	socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if( socket_ < 0) {
		log_ << "Can't create socket: " << GetLastError() << std::endl;
	}
	int broadcast=1;
	std::string recv;
	setsockopt(socket_, SOL_SOCKET, SO_BROADCAST, (const char *) &broadcast, sizeof(broadcast));
	CreateThread( NULL, 0, SocketServer::loop, this, 0, NULL);  


}
SocketServer::~SocketServer(void)
{
}

void SocketServer::OpenSerialPort() {
	CloseSerialPort();
	hComm = CreateFile(sensorPort_.c_str(),
				GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

	if( hComm  ==  INVALID_HANDLE_VALUE) {
		log_ << "CreateFile COM_PORT error code: " << GetLastError()  << std::endl;
		return;
	}
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hComm, &dcbSerialParams) == FALSE ) {
		log_ << "GetCommState  error code: " << GetLastError()  << std::endl;
		CloseSerialPort();
		return;
	}
	dcbSerialParams.BaudRate = bitRate_;      //BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             //ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    //StopBits = 1
    dcbSerialParams.Parity = NOPARITY;      //Parity = None
	dcbSerialParams.fOutxCtsFlow = FALSE;
	dcbSerialParams.fOutxDsrFlow = FALSE;


	if (SetCommState(hComm, &dcbSerialParams) == FALSE ) {
		log_ << "SetCommState  error code: " << GetLastError()  << std::endl;
		CloseSerialPort();
		return;
	}

	COMMTIMEOUTS timeouts = {
		5,  // ReadIntervalTimeout;         (Maximum time between read chars.)
		0,  // ReadTotalTimeoutMultiplier;  (Multiplier of characters.)
		20, // ReadTotalTimeoutConstant;    (Constant in milliseconds.)
		0,  // WriteTotalTimeoutMultiplier; (Multiplier of characters.)
		0   // WriteTotalTimeoutConstant;   (Constant in milliseconds.)
	};

	if(SetCommTimeouts(hComm, &timeouts) == FALSE ) {
		log_ << "SetCommTimeouts  error code: " << GetLastError()  << std::endl;
		CloseSerialPort();
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
		log_ << "SetCommTimeouts error code: " << GetLastError()  << std::endl;
		CloseSerialPort();
		return;
	}
	memset(&ov, 0, sizeof(ov));

	sendSerial(CRNL(GET));

	log_ << _T("OpenSerialPort OK") << std::endl;

}

void SocketServer::CloseSerialPort() {
	if(hComm != INVALID_HANDLE_VALUE) {
		CloseHandle(hComm);
		hComm = INVALID_HANDLE_VALUE;
	}
}

void SocketServer::sendSerial(const char* format, ...) {
	char buffer[1024] ={0};
	va_list arg;
	va_start (arg, format);
	vsprintf(buffer, format, arg);
	va_end (arg);
	int len = strlen(buffer);
	log_ << "Send: " << buffer;
	for(int i = 0; i < len; i++) {
		TransmitCommChar(hComm, buffer[i]);
	}
}


void SocketServer::broadcast(const char *data, int len) {
	sockaddr_in addr;
	addr.sin_family = AF_INET;
    addr.sin_port = htons(udpPort_);
    addr.sin_addr.s_addr = inet_addr("127.255.255.255");
	if(sendto(socket_, data, len, 0, (const sockaddr *)&addr, sizeof(addr)) < 0) {
		log_ << "Can't sendto socket: " << GetLastError() << std::endl;
	}
}




DWORD WINAPI SocketServer::loop( LPVOID lpParam ) 
{
	SocketServer *myThis = (SocketServer *) lpParam;
	DWORD pos=0;
	char cBuffer[129]; 
	cBuffer[129]= '\0';
	for(;;) {
		if(myThis->hComm != INVALID_HANDLE_VALUE) {
			DWORD dwSize = sizeof(cBuffer) - pos - 1;
			DWORD dwRead = 0;
			if(!ReadFile(myThis->hComm, &cBuffer[0] + pos, dwSize, &dwRead, NULL)) {
				myThis->log_ << 	_T("Error ReadFile: ") << GetLastError() << std::endl;
			} else {
				if(dwRead == 0) {
					Sleep(1);
					continue;
				}
				dwRead+=pos;
				cBuffer[dwRead] = 0;
				char *start=cBuffer;
				char *end;
				while((end=strchr(start, '\n')) != NULL ) {
					myThis->cfg_.parse(start, end-start+1);
					myThis->broadcast(start, end-start+1);
					start = ++end;
				}
				pos = dwRead - (start-cBuffer);
				if(start != cBuffer) {
					cBuffer[dwRead] = '\0';
					strcpy(cBuffer, start);
					cBuffer[pos] = '\0';
				}
			}
		} else {
			Sleep(100);
		}
	}
    return 0; 
} 
