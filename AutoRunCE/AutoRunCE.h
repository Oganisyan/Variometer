// AutoRunCE.h : Hauptheaderdatei f�r die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei f�r PCH einschlie�en"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif


// CAutoRunCEApp:
// Siehe AutoRunCE.cpp f�r die Implementierung dieser Klasse
//
class CAutoRunCEApp : public CWinApp
{
public:
	CAutoRunCEApp();
	
// �berschreibungen
public:
	virtual BOOL InitInstance();
// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CAutoRunCEApp theApp;
