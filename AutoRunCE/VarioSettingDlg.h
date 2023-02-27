#pragma once
#include "afxwin.h"
#include "SocketServer.h"
#include "WinReg.h"
#include "MyButton.h"
#include "afxcmn.h"

// VarioSettingDlg-Dialogfeld

class VarioSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(VarioSettingDlg)
	CWinReg &reg;
	SocketServer &server;

	DWORD origUpBarr;
	DWORD origDwBarr;

public:
	VarioSettingDlg(CWinReg &refReg, SocketServer &pServer, CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~VarioSettingDlg();
	void sendCmd(const char *cmdId, int value);

// Dialogfelddaten
	enum { IDD = IDD_VARIOCONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL OnInitDialog();
	MyButton mButtonClose;
	MyButton mButtonSave;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnNMCustomdrawSliderSound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderDwbarr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderUpbarr(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl mSliderLiftCtrl;
	CSliderCtrl mSliderSinkCtrl;
	CSliderCtrl mSliderSimCtrl;
};
