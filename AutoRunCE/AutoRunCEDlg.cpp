// AutoRunCEDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "AutoRunCE.h"
#include "AutoRunCEDlg.h"
#include "VarioSettingDlg.h"
#include <Pm.h>
#include <TlHelp32.h>

#include <winsock2.h>
#include <ws2bth.h>
#include <Bt_api.h.>
#include <Commctrl.h>
#include <vector>




extern "C" DWORD GetEventData(HANDLE hEvent );
extern "C" BOOL	SetEventData(HANDLE hEvent, DWORD dwData);
extern "C" BOOL KernelIoControl(DWORD dwIoControlCode,  LPVOID lpInBuf,  DWORD nInBufSize,
  LPVOID lpOutBuf,  DWORD nOutBufSize,  LPDWORD lpBytesReturned);


std::vector<std::wstring> volCtrlCmds;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAutoRunCEDlg-Dialogfeld

CAutoRunCEDlg::CAutoRunCEDlg(CWnd* pParent)
	: CDialog(CAutoRunCEDlg::IDD, pParent), 
	log(getFileNameFor(LOG).c_str()),
	reg(log),
	cfg_(log),
	bScreanLocked(true), 
	displayOrientation(DMDO_0),
	server(cfg_, log)

{
	xcSoarWinName = CFG(cfg_, XC_SOAR_WIN_NAME);
	int volCtrlCmdCount = _wtoi(CFG_LIST_COUNT(cfg_,SENSOR_VOLCMD).c_str());
	log << _T("VolCtrl Cmd Count: ") << volCtrlCmdCount  << std::endl;

	volCtrlCmds.resize(volCtrlCmdCount);
	for(int i=0; i < volCtrlCmdCount; i++) {
		volCtrlCmds[i] = CFG_LIST_ITEM(cfg_, SENSOR_VOLCMD, i);
		log << _T("\t Cmd ") << i << _T(": ")   << volCtrlCmds[i]  << std::endl;
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	hEvent = CreateEvent(NULL, TRUE, FALSE, _T("HW_BT_POWER_UP"));	

	InitChildProcess();
}

void CAutoRunCEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, mButtonClose);
	DDX_Control(pDX, IDC_BUTTON_LOCK, mButtonLock);
	DDX_Control(pDX, IDC_BUTTON_BIKE_NAVI, mButtonBikeNavi);
	DDX_Control(pDX, IDC_BUTTON_XCSOAR, mButtonXCSoar);
	DDX_Control(pDX, IDC_BUTTON_OFF, mButtonOff);
	DDX_Control(pDX, IDC_BUTTON_WINDOWS, mButtonWindows);
	DDX_Control(pDX, IDC_SLIDER_SOUND, mSliderCtrl);
	DDX_Control(pDX, IDC_BUTTON_ROTATE, mButtonRotate);
	DDX_Control(pDX, IDC_BUTTON_VARIO, mButtonSetup);
}

BEGIN_MESSAGE_MAP(CAutoRunCEDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
ON_BN_CLICKED(IDC_BUTTON_LOCK, &CAutoRunCEDlg::OnBnClickedButtonLock)
ON_BN_CLICKED(IDC_BUTTON_BIKE_NAVI, &CAutoRunCEDlg::OnBnClickedButtonBikeNavi)
ON_BN_CLICKED(IDC_BUTTON_XCSOAR, &CAutoRunCEDlg::OnBnClickedButtonXcsoar)
ON_BN_CLICKED(IDC_BUTTON_WINDOWS, &CAutoRunCEDlg::OnBnClickedButtonWindows)
ON_BN_CLICKED(IDC_BUTTON_OFF, &CAutoRunCEDlg::OnBnClickedButtonOff)
ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CAutoRunCEDlg::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BUTTON_ROTATE, &CAutoRunCEDlg::OnBnClickedButtonRotate)
ON_WM_ACTIVATE()
ON_WM_TIMER()
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SOUND, &CAutoRunCEDlg::OnNMCustomdrawSliderSound)
ON_BN_CLICKED(IDC_BUTTON_VARIO, &CAutoRunCEDlg::OnBnClickedButtonVario)
END_MESSAGE_MAP()


// CAutoRunCEDlg-Meldungshandler

BOOL CAutoRunCEDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	int sound =(int) reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("Volume"), 0);

	mButtonLock.EnableWindow(xcSoarWnd != NULL); 
	mButtonLock.SetBitMap(IDB_BITMAP_UNLOCK);
	mButtonRotate.SetBitMap(IDB_BITMAP_ROTATE);
	mButtonWindows.SetBitMap(IDB_BITMAP_WINDOWS);
	mButtonClose.SetBitMap(IDB_BITMAP_CLOSE);
	mButtonBikeNavi.SetBitMap(IDB_BITMAP_NAVI);
	mButtonXCSoar.SetBitMap(IDB_BITMAP_XCSOAR);
	mButtonOff.SetBitMap(IDB_BITMAP_OFF);
	mButtonSetup.SetBitMap(IDB_BITMAP_SETUP);
	mSliderCtrl.SetRange(0, 9, TRUE);
	mSliderCtrl.SetPos(sound);

	OnBnClickedButtonXcsoar();

	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}


void CAutoRunCEDlg::OnBnClickedButtonOff()
{
	mSliderCtrl.UpdateData(TRUE);
	int pos = (mSliderCtrl.GetPos() >= sizeof(volCtrlCmds)) ? sizeof(volCtrlCmds)-1 
		: (mSliderCtrl.GetPos() < 0) ? 0 : mSliderCtrl.GetPos();

	int posReg=(int) reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("Volume"), 0);

	if(posReg != pos) {
		reg.RegWriteDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("Volume"), pos);
	}
	SendSerial(volCtrlCmds[0]);

	SetSystemPowerState(NULL, POWER_STATE_SUSPEND, POWER_FORCE);
}

void CAutoRunCEDlg::OnBnClickedButtonLock()
{
	bScreanLocked = !(bScreanLocked);
	xcSoarWnd->EnableWindow(bScreanLocked);
	xcSoarWnd->SetForegroundWindow();
	mButtonLock.SetWindowText(bScreanLocked? LOCK_STR : UNLOCK_STR);
	mButtonLock.SetBitMap(bScreanLocked? 
		IDB_BITMAP_UNLOCK : IDB_BITMAP_LOCK);	

	ShowWindow(SW_HIDE); 
}


typedef void   (WINAPI *AnWBT_Init_t)();
typedef DWORD  (WINAPI *AnWBT_ScanAllService_t)();
typedef DWORD  (WINAPI *AnWBT_Discovery_t)(char *p);



void CAutoRunCEDlg::OnBnClickedCancel()
{
	if(xcSoarWnd != NULL) {
		xcSoarWnd->SetForegroundWindow();
	}
	ShowWindow(SW_HIDE); 
}

void CAutoRunCEDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if(WA_ACTIVE & nState) {
		SetTimer(2, 100, NULL);
	}

	return CDialog::OnActivate(nState, pWndOther, bMinimized); 
}

void CAutoRunCEDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(WaitForSingleObject(hChildProcess,20)==WAIT_OBJECT_0) {
		mButtonBikeNavi.EnableWindow(TRUE);
		mButtonXCSoar.EnableWindow(TRUE);
		mButtonWindows.EnableWindow(TRUE);
		ShowWindow(SW_SHOW);
	} else if(nIDEvent == 1) {
		if(WaitForSingleObject(hEvent,20)==WAIT_OBJECT_0) {
			DWORD rc=GetEventData(hEvent);
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			SetTimer(2, 10000, NULL);
		}
	}else if(nIDEvent == 2) {
		KillTimer(nIDEvent);
		SetTimer(1, 5, NULL);
		ShowWindow(SW_HIDE);
	} 
	xcSoarWnd = FindWindow(xcSoarWinName.c_str(), NULL);
	mButtonLock.EnableWindow(xcSoarWnd != NULL);

	CDialog::OnTimer(nIDEvent); 
}


HANDLE CAutoRunCEDlg::CreateProcess(LPCWSTR pProcessName)
{
	PROCESS_INFORMATION pInfo;
	if(::CreateProcess( pProcessName, 
		NULL, NULL, NULL, FALSE, 0,  NULL, NULL,  NULL,  &pInfo))
	{
		return pInfo.hProcess;
	} else {
		return INVALID_HANDLE_VALUE;
	}
}

void CAutoRunCEDlg::InitChildProcess()
{
	KillProcess(getFileName(CFG(cfg_, BIKE_NAV_PATH)).c_str());
	if(WaitForSingleObject(hEvent, 3000) == WAIT_TIMEOUT) {
		hChildProcess = CreateProcess(CFG(cfg_, XC_SOAR_PATH).c_str());
		Sleep(1000);
	} else {
		ResetEvent(hEvent);
		hChildProcess = CreateProcess(CFG(cfg_, WIN_EXPLORER_PATH).c_str());
	}
}

void CAutoRunCEDlg::KillProcess(LPCWSTR pProcessName)
{
	HANDLE proc;
	while((proc=GetProcessHandle(pProcessName)) != INVALID_HANDLE_VALUE)
		TerminateProcess(proc,0);
}

HANDLE  CAutoRunCEDlg::GetProcessHandle(LPCWSTR szProcessName)
{
    //Get the snapshot of all processes in the system
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    //Get the information of the first process
    if (!Process32First(hSnap, &pe32))
    {
        CloseHandle(hSnap);
        return INVALID_HANDLE_VALUE;
    }

    //Loop through all processes
    do
    {
        if (_tcscmp(szProcessName, pe32.szExeFile) == 0)
        {
            //Got the process ID
            CloseHandle(hSnap);
            printf("sz = %s; exe = %s; pid = %d\n", szProcessName, pe32.szExeFile, pe32.th32ProcessID);
                            //Error here, correct PID was found in both case
            return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        }
    } 
    while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);
    return INVALID_HANDLE_VALUE;
}



void CAutoRunCEDlg::OnBnClickedButtonBikeNavi()
{
	TerminateProcess(hChildProcess, 0);

	server.CloseSerialPort();
	hChildProcess = CreateProcess(CFG(cfg_, BIKE_NAV_PATH).c_str());
	mButtonBikeNavi.EnableWindow(FALSE);
	mButtonXCSoar.EnableWindow(TRUE);
	mButtonWindows.EnableWindow(TRUE);
}



void CAutoRunCEDlg::OnBnClickedButtonXcsoar()
{
	TerminateProcess(hChildProcess, 0);

	server.OpenSerialPort();
	hChildProcess = CreateProcess(CFG(cfg_, XC_SOAR_PATH).c_str());
	mButtonBikeNavi.EnableWindow(TRUE);
	mButtonXCSoar.EnableWindow(FALSE);
	mButtonWindows.EnableWindow(TRUE);

	int sound =(int) reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("Volume"), 0);
	SendSerial(volCtrlCmds[sound]);
	log << "Sound: " << sound << std::endl;

	int upBarr = (int) (reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("UpBarr"), 0)/50);
	VarioSettingDlg::sendCmd(server, "UpBarr", upBarr);
	log << "UpBarr: " << upBarr << std::endl;
	int dwBarr = (int) (reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("DwBarr"), 100) / 50);
	VarioSettingDlg::sendCmd(server, "DwBarr", dwBarr);
	log << "DwBarr: " << dwBarr << std::endl;
	int sensi = (int) (reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("Sensi"), 40)/20);
	VarioSettingDlg::sendCmd(server, "Sensi", sensi);
	log << "Sensi: " << sensi << std::endl;

}

void CAutoRunCEDlg::OnBnClickedButtonWindows()
{
	TerminateProcess(hChildProcess, 0);

	server.CloseSerialPort();
	hChildProcess = CreateProcess(CFG(cfg_, WIN_EXPLORER_PATH).c_str());
	mButtonBikeNavi.EnableWindow(TRUE);
	mButtonXCSoar.EnableWindow(TRUE);
	mButtonWindows.EnableWindow(FALSE);

}

void CAutoRunCEDlg::SendSerial(const std::wstring &wsCmd)
{
	std::string cmd;
	cmd.resize(wsCmd.size() * sizeof(wchar_t) / sizeof(char) + 2);
	int len = wcstombs(&cmd[0], wsCmd.c_str(), wsCmd.size() * sizeof(wchar_t) / sizeof(char));
	cmd[len++] = '\r';
	cmd[len++] = '\n';
	server.serialSend(cmd.c_str(), len);
}

void CAutoRunCEDlg::OnNMCustomdrawSliderSound(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int pos = (mSliderCtrl.GetPos() >= sizeof(volCtrlCmds)) ? sizeof(volCtrlCmds)-1 
		: (mSliderCtrl.GetPos() < 0) ? 0 : mSliderCtrl.GetPos();
	SendSerial(volCtrlCmds[pos]);
	*pResult = 0;
}


void CAutoRunCEDlg::OnBnClickedButtonRotate()
{
#if 1
	CRect rc;
	GetWindowRect(&rc); // getting dialog coordinates

	displayOrientation =	(displayOrientation == DMDO_0)   ? DMDO_90  :
							(displayOrientation == DMDO_90)  ? DMDO_180 :
							(displayOrientation == DMDO_180) ? DMDO_270 :
							DMDO_0;

	DEVMODE devMode;
	memset(&devMode, 0, sizeof(DEVMODE));
	devMode.dmSize = sizeof(DEVMODE);
	devMode.dmFields = DM_DISPLAYORIENTATION;
	devMode.dmDisplayOrientation = displayOrientation;


	if (DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettingsEx(NULL, 
		&devMode, NULL, CDS_RESET, NULL)) {
			log << _T("Can't Rotate display: ") << GetLastError() << std::endl;
	} else {
		log << _T("Display Rotation OK, Orientation: ") << devMode.dmDisplayOrientation << std::endl;
	}

	MoveWindow(rc.top, rc.left, rc.Width(), rc.Height());
#else 
	HKEY hKey;
	if(ERROR_SUCCESS==RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Drivers\\BuiltIn\\BackLight"),0,0,&hKey))
    {
		DWORD dwBklLevel = 100;
       	RegSetValueEx(hKey, _T("Brightness"),0,REG_DWORD,(BYTE*)&dwBklLevel,sizeof(DWORD));
		dwBklLevel = 50;
       	RegSetValueEx(hKey, _T("CurrentDx"),0,REG_DWORD,(BYTE*)&dwBklLevel,sizeof(DWORD));
		RegCloseKey(hKey);
		HANDLE hBL=CreateEvent(NULL,FALSE,FALSE, _T("EventModify"));
		if(hBL)
		{
			SetEvent(hBL);
			CloseHandle(hBL);
		}
	}
#endif
}


void CAutoRunCEDlg::OnBnClickedButtonVario()
{
	INT_PTR rc; 
	ShowWindow(SW_HIDE); 
	VarioSettingDlg dlg(reg, server);
	rc = dlg.DoModal();
}
