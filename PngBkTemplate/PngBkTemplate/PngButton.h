#if !defined(AFX_PNGBUTTON_H__F33C59EA_E493_49CD_96C0_9D37FDD09749__INCLUDED_)
#define AFX_PNGBUTTON_H__F33C59EA_E493_49CD_96C0_9D37FDD09749__INCLUDED_

#include "Public.h"

#if _MSC_VER > 1000
#pragma once
#endif 

class CPngButton : public CButton
{
public:
	CPngButton();
	virtual ~CPngButton();

public:
	void Init(UINT nImg, int nPartNum, UINT nBtnType=BTN_TYPE_NORMAL);
	bool ShowImage(CDC* pDC, Image* pImage, UINT nState);
	Image *ImageFromResource(HINSTANCE hInstance,UINT uImgID,LPCTSTR lpType);
	void PaintParent();

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHOver(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

public:
	bool m_bTracked;

private:
	int m_nImgPart;

	Image* m_pImage;

	UINT m_nState;
	UINT m_nBtnType;
	BOOL m_bMenuOn;		//BTN类型为BTN_TYPE_MENU时，是否处于按下的状态
};

#endif 
