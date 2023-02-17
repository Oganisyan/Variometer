#pragma once
#include "afxwin.h"


// MyButton

class MyButton : public CButton
{
	DECLARE_DYNAMIC(MyButton)

public:
	MyButton();
	virtual ~MyButton();
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetColor(COLORREF pClr) {
		clr = pClr;
	}

protected:
	DECLARE_MESSAGE_MAP()
	COLORREF clr;
private:
	CBitmap m_bitmap;
public:
	void SetBitMap(UINT bitmapId);
};


