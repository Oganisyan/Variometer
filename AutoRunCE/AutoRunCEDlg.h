// AutoRunCEDlg.h : Headerdatei
//

#pragma once
#include "afxwin.h"
#include "WinReg.h"
#include "MyButton.h"
#include "SocketServer.h"
#include <fstream>
#include "afxcmn.h"

#define LOG_FILE			_T("\\Storage Card\\AutoRunCE.log")


#define LOCK_STR			_T("Lock")
#define UNLOCK_STR			_T("Unlock")


// CAutoRunCEDlg-Dialogfeld
class CAutoRunCEDlg : public CDialog
{

	static std::wstring getFileName(const std::wstring &path) {
		std::wstring::size_type pos = path.find_last_of(_T('\\'));
		return (pos != std::string::npos) ? path.substr(pos+1) : _T("");
	}

// Konstruktion
public:
	CAutoRunCEDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_AUTORUNCE_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung

// Implementierung
protected:
	HICON m_hIcon;
	MyButton mButtonClose;
	MyButton mButtonLock;
	MyButton mButtonBikeNavi;
	MyButton mButtonXCSoar;
	MyButton mButtonOff;
	MyButton mButtonWindows;
	MyButton mButtonRotate;
	MyButton mButtonSetup;
	std::wofstream log;
	Config cfg_;
	SocketServer server;
	std::wstring xcSoarWinName;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonOff();
	afx_msg void OnBnClickedButtonLock();
	afx_msg void OnBnClickedButtonXcsoar();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonBikeNavi();
	afx_msg void OnBnClickedButtonWindows();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnNMCustomdrawSliderSound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRotate();

private:
	HANDLE hEvent;
	CWnd *xcSoarWnd;
	bool bScreanLocked;
	HANDLE hChildProcess;
	void InitChildProcess();
	HANDLE CreateProcess(LPCWSTR pProcessName);
	void KillProcess(LPCWSTR pProcessName);
	HANDLE  GetProcessHandle(LPCWSTR szProcessName);
	CSliderCtrl mSliderCtrl;
	DWORD displayOrientation;
	CWinReg reg;
public:
	afx_msg void OnBnClickedButtonVario();
};
