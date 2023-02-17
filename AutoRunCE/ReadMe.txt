================================================================================
    MICROSOFT FOUNDATION CLASS-BIBLIOTHEK: AutoRunCE-Projekt�bersicht
===============================================================================

Der Anwendungs-Assistent hat diese AutoRunCE-Anwendung f�r Sie 
erstellt. Diese Anwendung zeigt nicht nur die Grundlagen der Verwendung von 
Microsoft Foundation Classes, sondern dient auch als Ausgangspunkt f�r das 
Schreiben Ihrer Anwendung.

Diese Datei enth�lt eine Zusammenfassung dessen, was Sie in jeder der Dateien
finden, aus denen Ihre AutoRunCE�Anwendung besteht.

AutoRunCE.vcproj
    Dies ist die Hauptprojektdatei f�r VC++-Projekte, die mithilfe eines 
    Anwendungs-Assistenten erstellt werden. 
    Sie enth�lt Informationen �ber die Version von Visual C++, in der die Datei 
    erzeugt wurde, sowie Informationen �ber die Plattformen, Konfigurationen und 
    Projektfunktionen, die mit dem Anwendungs-Assistenten ausgew�hlt wurden.

AutoRunCE.h
    Dies ist die Hauptheaderdatei f�r die Anwendung. Sie enth�lt weitere
    projektspezifische Header und deklariert die CAutoRunCEApp-
    Anwendungsklasse.

AutoRunCE.cpp
    Dies ist die Hauptquelldatei der Anwendung, die die CAutoRunCEApp-
    Anwendungsklasse enth�lt.

AutoRunCE.rc
    Dies ist die Hauptressourcendatei des Projekts, die alle Microsoft Windows-
    Ressourcen auflistet, die das Projekt verwendet. Sie enth�lt die Symbole, 
    Bitmaps und Cursor, die im Unterverzeichnis "RES" gespeichert werden. Diese 
    Datei kann direkt in Microsoft Visual C++ bearbeitet werden. Ihre 
    Projektressourcen befinden sich in 1031.
    Wenn die RC-Datei beibehalten wird, werden die Definitionen im 
    Datenabschnitt als Hexadezimalversion des numerischen Wertes, f�r den sie 
    definiert sind, gespeichert, und nicht als der angezeigte Name der 
    Definition.

res\AutoRunCE.rc2
    Diese Datei enth�lt Ressourcen, die nicht von Microsoft Visual C++
    bearbeitet werden. Sie sollten alle Ressourcen, die nicht mit dem
    Ressourcen-Editor bearbeitet werden k�nnen, in dieser Datei platzieren.



res\AutoRunCE.ico
    Dies ist eine Symboldatei, die als Symbol der Anwendung verwendet wird. 
    Dieses Symbol ist in der Hauptressourcendatei enthalten.


/////////////////////////////////////////////////////////////////////////////

Der Anwendungs-Assistent erstellt eine Dialogfeldklasse:

AutoRunCEDlg.h, AutoRunCEDlg.cpp � das Dialogfeld
    Diese Dateien enthalten Ihre CAutoRunCEDlg�Klasse. Diese Klasse 
    bestimmt das Verhalten des Hauptdialogfelds Ihrer Anwendung. Die Vorlage des 
    Dialogfelds befindet sich in der Hauptressourcendatei, die in Microsoft 
    Visual C++ bearbeitet werden kann.


/////////////////////////////////////////////////////////////////////////////

Weitere Standarddateien:

StdAfx.h, StdAfx.cpp
    Diese Dateien werden verwendet, um eine vorkompilierte Headerdatei
    (PCH-Datei) mit dem Namen "AutoRunCE.pch2 und eine 
    vorkompilierte Typendatei mit dem Namen "StdAfx.obj" zu erstellen.

Resource.h
    Dies ist die Standardheaderdatei, die neue Ressourcen-IDs definiert.
    Microsoft Visual C++ liest und aktualisiert diese Datei.

/////////////////////////////////////////////////////////////////////////////

Weitere Hinweise:

Der Anwendungs-Assistent verwendet "TODO:, um auf Teile des Quellcodes
hinzuweisen, die Sie hinzuf�gen oder anpassen sollten.

Wenn Ihre Anwendung MFC in einer freigegebenen DLL verwendet und Ihre Anwendung
in einer anderen als der aktuellen Sprache des Betriebssystems vorliegt,
m�ssen Sie die entsprechenden lokalisierten Ressourcen "MFC90XXX.DLL" in Ihr
Anwendungsverzeichnis kopieren ("XXX" steht f�r die Sprachabk�rzung. "MFC90DEU.DLL" enth�lt beispielsweise ins Deutsche �bersetzte Ressourcen.) Wenn Sie dies nicht tun, bleiben einige der Benutzeroberfl�chenelemente Ihrer Anwendung in der Sprache des Betriebssystems.

/////////////////////////////////////////////////////////////////////////////