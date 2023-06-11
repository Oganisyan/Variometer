// VarioSettingDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "AutoRunCE.h"
#include "VarioSettingDlg.h"

// VarioSettingDlg-Dialogfeld

IMPLEMENT_DYNAMIC(VarioSettingDlg, CDialog)

VarioSettingDlg::VarioSettingDlg(CWinReg &pReg, SocketServer &pServer, CWnd* pParent /*=NULL*/)
	: CDialog(VarioSettingDlg::IDD, pParent), reg(pReg), server(pServer)
{

}

VarioSettingDlg::~VarioSettingDlg()
{
}

void VarioSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, mButtonClose);
	DDX_Control(pDX, IDC_BUTTON_SAVE, mButtonSave);
	DDX_Control(pDX, IDC_SLIDER_UPBARR, mSliderLiftCtrl);
	DDX_Control(pDX, IDC_SLIDER_DWBARR, mSliderSinkCtrl);
	DDX_Control(pDX, IDC_SLIDER_SOUND, mSliderSimCtrl);
	DDX_Control(pDX, IDC_BUTTON_MM, mButtonMinus);
	DDX_Control(pDX, IDC_BUTTON_PP, mButtonPlus);
	DDX_Control(pDX, IDC_SLIDER_SENSI, mSliderSensiCtrl);
}

BOOL VarioSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	mButtonClose.SetBitMap(IDB_BITMAP_CLOSE);
	mButtonSave.SetBitMap(IDB_BITMAP_SAVE);
	mButtonMinus.SetBitMap(IDB_BITMAP_MINUS);
	mButtonPlus.SetBitMap(IDB_BITMAP_PLUS);

	mSliderLiftCtrl.SetRange(0, 8, TRUE);
	mSliderSinkCtrl.SetRange(0, 8, TRUE);
	mSliderSimCtrl.SetRange(-16, +16, TRUE);
	mSliderSensiCtrl.SetRange(0, 5, TRUE);
	mSliderSimCtrl.SetPos(0);


	origSensi = reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("Sensi"), 40);
	origUpBarr = reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("UpBarr"), 0);
	origDwBarr = reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("DwBarr"), 100);
	mSliderSensiCtrl.SetPos(origSensi/20);
	mSliderLiftCtrl.SetPos(origUpBarr/50);
	mSliderSinkCtrl.SetPos(origDwBarr/50);

	SetTimer(3, 100, NULL);
	return TRUE;
}

BEGIN_MESSAGE_MAP(VarioSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &VarioSettingDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &VarioSettingDlg::OnBnClickedButtonCancel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SOUND, &VarioSettingDlg::OnNMCustomdrawSliderSound)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DWBARR, &VarioSettingDlg::OnNMCustomdrawSliderDwbarr)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_UPBARR, &VarioSettingDlg::OnNMCustomdrawSliderUpbarr)
	ON_BN_CLICKED(IDC_BUTTON_MM, &VarioSettingDlg::OnBnClickedButtonMm)
	ON_BN_CLICKED(IDC_BUTTON_PP, &VarioSettingDlg::OnBnClickedButtonPp)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SENSI, &VarioSettingDlg::OnNMCustomdrawSliderSensi)
END_MESSAGE_MAP()


// VarioSettingDlg-Meldungshandler

void VarioSettingDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 3) {
		VarioSettingDlg::SetFocus();
	}
}


void VarioSettingDlg::OnBnClickedButtonSave()
{
	KillTimer(3);
	if(origUpBarr != mSliderLiftCtrl.GetPos() *50) {
		reg.RegWriteDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  
			_T("UpBarr"), mSliderLiftCtrl.GetPos() *50);
	}
	if(origDwBarr != mSliderSinkCtrl.GetPos() *50) {
		reg.RegWriteDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  
			_T("DwBarr"), mSliderSinkCtrl.GetPos() *50);
	}
	if(origSensi != mSliderSensiCtrl.GetPos() *20) {
		reg.RegWriteDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  
			_T("Sensi"), mSliderSensiCtrl.GetPos() *20);
	}

	sendCmd(server, "SIM", 0);

	CDialog::EndDialog(0);
}

void VarioSettingDlg::OnBnClickedButtonCancel()
{
	KillTimer(3);
	sendCmd(server, "BUP", origUpBarr/50);
	sendCmd(server, "BDW", origDwBarr/50);
	sendCmd(server, "SEN", origSensi/20);
	sendCmd(server, "SIM", 0);
	CDialog::EndDialog(0);
}

void VarioSettingDlg::OnNMCustomdrawSliderSound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int value = mSliderSimCtrl.GetPos() * 50;
	sendCmd(server, "SIM", value);
	*pResult = 0;
}

void VarioSettingDlg::OnNMCustomdrawSliderDwbarr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int value = mSliderSinkCtrl.GetPos() * 50;
	sendCmd(server, "BDW", value);
	*pResult = 0;
}

void VarioSettingDlg::OnNMCustomdrawSliderUpbarr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int value = mSliderLiftCtrl.GetPos() * 50;
	sendCmd(server, "BUP", value);
	*pResult = 0;
}

void VarioSettingDlg::sendCmd(SocketServer &server, const char* cmdId, int value) {
	char buffer[1024] ={0};
	sprintf(buffer, "$%s %d*\r\n", cmdId, value);
	server.serialSend(buffer, strlen(buffer));
}
void VarioSettingDlg::OnBnClickedButtonMm()
{
	int value = mSliderSimCtrl.GetPos() -1;
	mSliderSimCtrl.SetPos(value);
}

void VarioSettingDlg::OnBnClickedButtonPp()
{
	int value = mSliderSimCtrl.GetPos() +1;
	mSliderSimCtrl.SetPos(value);
}

void VarioSettingDlg::OnNMCustomdrawSliderSensi(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int value = mSliderSensiCtrl.GetPos() * 20;
	sendCmd(server, "SEN", value);
	*pResult = 0;
}
