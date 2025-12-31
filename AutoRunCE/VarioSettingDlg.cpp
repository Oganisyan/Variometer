// VarioSettingDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "AutoRunCE.h"
#include "VarioSettingDlg.h"

// VarioSettingDlg-Dialogfeld

IMPLEMENT_DYNAMIC(VarioSettingDlg, CDialog)

VarioSettingDlg::VarioSettingDlg(CWinReg &pReg, Config &pCfg, SocketServer &pServer, int value, CWnd* pParent /*=NULL*/)
	: CDialog(VarioSettingDlg::IDD, pParent), reg(pReg), cfg_(pCfg), server_(pServer), currentValue_(value)
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

	initSlider(1,      5,  1,  cfg_.getSensi(), mSliderSensiCtrl);
	initSlider(0,    400, 50, cfg_.getBarrUp(), mSliderLiftCtrl);
	initSlider(0,    400, 50, cfg_.getBarrDw(), mSliderSinkCtrl);
	initSlider(-400, 400, 50,                0, mSliderSimCtrl);

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
	server_.sendSerial(CRNL(SIM), 0);
	server_.sendSerial(CRNL(UPD));

	cfg_.setSensi(mSliderSensiCtrl.GetPos());
	cfg_.setBarrUp(mSliderLiftCtrl.GetPos());
	cfg_.setBarrDw(mSliderSinkCtrl.GetPos());
	cfg_.setVolume(currentValue_);

	CDialog::EndDialog(0);
}

void VarioSettingDlg::OnBnClickedButtonCancel()
{
	KillTimer(3);
	server_.sendSerial(CRNL(BUP), cfg_.getBarrUp());
	server_.sendSerial(CRNL(BDW), cfg_.getBarrDw());
	server_.sendSerial(CRNL(SEN), cfg_.getSensi());
	server_.sendSerial(CRNL(SIM), 0);
	server_.sendSerial(CRNL(UPD));
	CDialog::EndDialog(0);
}

void VarioSettingDlg::OnNMCustomdrawSliderSound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	server_.sendSerial(CRNL(SIM), mSliderSimCtrl.GetPos());
	*pResult = 0;
}

void VarioSettingDlg::OnNMCustomdrawSliderDwbarr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	server_.sendSerial(CRNL(BDW), mSliderSinkCtrl.GetPos());
	*pResult = 0;
}

void VarioSettingDlg::OnNMCustomdrawSliderUpbarr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	server_.sendSerial(CRNL(BUP), mSliderLiftCtrl.GetPos());
	*pResult = 0;
}

void VarioSettingDlg::OnBnClickedButtonMm()
{
	int pos = max(mSliderSimCtrl.GetPos() - mSliderSimCtrl.GetPageSize(), mSliderSimCtrl.GetRangeMin());
	mSliderSimCtrl.SetPos(pos);
}

void VarioSettingDlg::OnBnClickedButtonPp()
{
	int pos = min(mSliderSimCtrl.GetPos() + mSliderSimCtrl.GetPageSize(), mSliderSimCtrl.GetRangeMax());
	mSliderSimCtrl.SetPos(pos);
}

void VarioSettingDlg::OnNMCustomdrawSliderSensi(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	server_.sendSerial(CRNL(SEN), mSliderSensiCtrl.GetPos());
	*pResult = 0;
}
