// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "AutoRunCE.h"
#include "MyButton.h"


// MyButton

IMPLEMENT_DYNAMIC(MyButton, CButton)

MyButton::MyButton() : clr(RGB(200, 200, 200))
{
}

MyButton::~MyButton()
{
}


BEGIN_MESSAGE_MAP(MyButton, CButton)
END_MESSAGE_MAP()



// MyButton message handlers

void MyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{

  CDC dc;
  dc.Attach(lpDrawItemStruct->hDC);     //Get device context object
  CRect rt;
  rt = lpDrawItemStruct->rcItem;        //Get button rect
 
  dc.FillSolidRect(rt, clr); //Fill button with blue color
 
  UINT state = lpDrawItemStruct->itemState; //Get state of the button
  if(!IsWindowEnabled()){
    dc.DrawEdge(rt,EDGE_ETCHED,BF_RECT);    // Draw a sunken face
  } 
  else if ( (state & ODS_SELECTED) )            // If it is pressed
  {
    dc.DrawEdge(rt,EDGE_SUNKEN,BF_RECT);    // Draw a sunken face
  }
  else
  {
    dc.DrawEdge(rt,EDGE_RAISED,BF_RECT);    // Draw a raised face
  }


   // draw the bitmap
   if(NULL != m_bitmap.GetSafeHandle())
   {
      // create a memory DC
      CDC memDC;
      memDC.CreateCompatibleDC(&dc);
      // select the bitmap into the memory DC
      CBitmap* pbmpOld = memDC.SelectObject(&m_bitmap);

      // get BITMAP structure to find bitmap dimensions
      BITMAP bmp;
      m_bitmap.GetBitmap(&bmp);

      // (stretch) blit from memory DC to button DC
      dc.StretchBlt(2, 2,                      // destination coordinates
                      rt.Height()-4, rt.Height()-4,   // destination dimensions  
                      &memDC,                    // source DC
                      0, 0,                      // source coordinates
                      bmp.bmWidth, bmp.bmHeight, // source dimensions
                      SRCCOPY);                  // raster operation

      // restore the inital attributes of memory DC
      memDC.SelectObject(pbmpOld); 
  }

  CString strTemp;
  GetWindowText(strTemp); 
  if(IsWindowEnabled()) {
	  dc.SetTextColor(RGB(0,0,0));
  } else {
	  dc.SetTextColor(RGB(125,125,125));
  }
  dc.DrawText(strTemp,rt,DT_CENTER|DT_VCENTER|DT_SINGLELINE); 

  if ( (state & ODS_FOCUS ) )       // If the button is focused
  {
    // Draw a focus rect which indicates the user 
    // that the button is focused
    int iChange = 3;
    rt.top += iChange;
    rt.left += iChange;
    rt.right -= iChange;
    rt.bottom -= iChange;
    dc.DrawFocusRect(rt);
  }
  dc.Detach();
}


void MyButton::SetBitMap(UINT bitmapId)
{
	m_bitmap.LoadBitmap(bitmapId);
}
