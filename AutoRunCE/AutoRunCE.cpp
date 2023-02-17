// AutoRunCE.cpp : Definiert das Klassenverhalten für die Anwendung.
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
	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}


// Das einzige CAutoRunCEApp-Objekt
CAutoRunCEApp theApp;

// CAutoRunCEApp-Initialisierung

BOOL CAutoRunCEApp::InitInstance()
{

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));


	INT_PTR rc; 
	CAutoRunCEDlg dlg;
	m_pMainWnd = &dlg;
	rc = dlg.DoModal();

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, sodass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}
