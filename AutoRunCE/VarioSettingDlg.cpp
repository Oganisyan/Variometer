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
	DDX_Control(pDX, IDC_BUTTON_CANCEL, mButtonClose);
	DDX_Control(pDX, IDC_BUTTON_SAVE, mButtonSave);
	DDX_Control(pDX, IDC_SLIDER_UPBARR, mSliderLiftCtrl);
	DDX_Control(pDX, IDC_SLIDER_DWBARR, mSliderSinkCtrl);
	DDX_Control(pDX, IDC_SLIDER_SOUND, mSliderSimCtrl);
}

BOOL VarioSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	mButtonClose.SetBitMap(IDB_BITMAP_CLOSE);
	mButtonSave.SetBitMap(IDB_BITMAP_SAVE);
	mSliderLiftCtrl.SetRange(0, 8, TRUE);
	mSliderSinkCtrl.SetRange(0, 8, TRUE);
	mSliderSimCtrl.SetRange(-16, +16, TRUE);
	mSliderSimCtrl.SetPos(0);

	origUpBarr = reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("UpBarr"), 0);
	origDwBarr = reg.RegReadDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  _T("DwBarr"), 100);
	mSliderLiftCtrl.SetPos(origUpBarr/50);
	mSliderSinkCtrl.SetPos(origDwBarr/50);

	return TRUE;
}

BEGIN_MESSAGE_MAP(VarioSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &VarioSettingDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &VarioSettingDlg::OnBnClickedButtonCancel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SOUND, &VarioSettingDlg::OnNMCustomdrawSliderSound)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DWBARR, &VarioSettingDlg::OnNMCustomdrawSliderDwbarr)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_UPBARR, &VarioSettingDlg::OnNMCustomdrawSliderUpbarr)
END_MESSAGE_MAP()


// VarioSettingDlg-Meldungshandler

void VarioSettingDlg::OnBnClickedButtonSave()
{

	if(origUpBarr != mSliderLiftCtrl.GetPos() *50) {
		reg.RegWriteDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  
			_T("UpBarr"), mSliderLiftCtrl.GetPos() *50);
	}
	if(origDwBarr != mSliderSinkCtrl.GetPos() *50) {
		reg.RegWriteDword(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\AutoRunCE"),  
			_T("DwBarr"), mSliderSinkCtrl.GetPos() *50);
	}
	sendCmd("SIM", 0);

	CDialog::EndDialog(0);
}

void VarioSettingDlg::OnBnClickedButtonCancel()
{
	sendCmd("BUP", origUpBarr);
	sendCmd("BDW", origDwBarr);
	sendCmd("SIM", 0);
	CDialog::EndDialog(0);
}

void VarioSettingDlg::OnNMCustomdrawSliderSound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int value = mSliderSimCtrl.GetPos() * 50;
	sendCmd("SIM", value);
	*pResult = 0;
}

void VarioSettingDlg::OnNMCustomdrawSliderDwbarr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int value = mSliderSinkCtrl.GetPos() * 50;
	sendCmd("BDW", value);
	*pResult = 0;
}

void VarioSettingDlg::OnNMCustomdrawSliderUpbarr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int value = mSliderLiftCtrl.GetPos() * 50;
	sendCmd("BUP", value);
	*pResult = 0;
}

void VarioSettingDlg::sendCmd(const char* cmdId, int value) {
	char buffer[1024] ={0};
	sprintf(buffer, "$%s %d*\r\n", cmdId, value);
	server.serialSend(buffer, strlen(buffer));
}