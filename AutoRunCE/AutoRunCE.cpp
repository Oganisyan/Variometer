// AutoRunCE.cpp : Definiert das Klassenverhalten f�r die Anwendung.
//

#include "stdafx.h"
#include "AutoRunCE.h"
#include "AutoRunCEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoRunCEApp

BEGIN_MESSAGE_MAP(CAutoRunCEApp, CWinApp)
END_MESSAGE_MAP()


// CAutoRunCEApp-Erstellung
CAutoRunCEApp::CAutoRunCEApp()
	: CWinApp()
{
	// TODO: Hier Code zur Konstruktion einf�gen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}


// Das einzige CAutoRunCEApp-Objekt
CAutoRunCEApp theApp;

// CAutoRunCEApp-Initialisierung

BOOL CAutoRunCEApp::InitInstance()
{

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Gr��e
	// der ausf�hrbaren Datei verringern m�chten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// �ndern Sie den Registrierungsschl�ssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: �ndern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));


	INT_PTR rc; 
	CAutoRunCEDlg dlg;
	m_pMainWnd = &dlg;
	rc = dlg.DoModal();

	// Da das Dialogfeld geschlossen wurde, FALSE zur�ckliefern, sodass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}
