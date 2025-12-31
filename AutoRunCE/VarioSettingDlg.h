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
    Config &cfg_;
	SocketServer &server_;
	int currentValue_;
	static void initSlider(int min, int max, int step, int pos, CSliderCtrl &ctrl) {
		ctrl.SetRange(min, max, TRUE);
		ctrl.SetPageSize(step);
		ctrl.SetLineSize(step);
		ctrl.SetPos(pos);
	}

public:
	VarioSettingDlg(CWinReg &refReg, Config &pCfg, SocketServer &pServer, int value,CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~VarioSettingDlg();

// Dialogfelddaten
	enum { IDD = IDD_VARIOCONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL OnInitDialog();
	MyButton mButtonClose;
	MyButton mButtonSave;
	MyButton mButtonMinus;
	MyButton mButtonPlus;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnNMCustomdrawSliderSound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderDwbarr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderUpbarr(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl mSliderLiftCtrl;
	CSliderCtrl mSliderSinkCtrl;
	CSliderCtrl mSliderSimCtrl;
	afx_msg void OnBnClickedButtonMm();
	afx_msg void OnBnClickedButtonPp();
	CSliderCtrl mSliderSensiCtrl;
	afx_msg void OnNMCustomdrawSliderSensi(NMHDR *pNMHDR, LRESULT *pResult);
};
