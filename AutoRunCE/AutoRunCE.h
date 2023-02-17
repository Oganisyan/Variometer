// AutoRunCE.h : Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif


// CAutoRunCEApp:
// Siehe AutoRunCE.cpp für die Implementierung dieser Klasse
//
class CAutoRunCEApp : public CWinApp
{
public:
	CAutoRunCEApp();
	
// Überschreibungen
public:
	virtual BOOL InitInstance();
// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CAutoRunCEApp theApp;
