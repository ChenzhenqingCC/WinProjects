#pragma once
#include "GdiImageRender.h"

// ChildDlg 对话框

class ChildDlg : public CDialog
{
	DECLARE_DYNAMIC(ChildDlg)

public:
	ChildDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ChildDlg();

// 对话框数据
	enum { IDD = IDD_CHILD_DIALOG };
private:
	CBitmap* m_pBitmap;
	CDC* m_pMemDC;
	int m_nScreenX;
	int m_nScreenY;

	CBrush m_brush;
	CWnd* parentDlg;
	void DrawSomething();
	HICON m_hIcon;
	CRect m_DrawRect;

	GdiImageRender m_BK;

	GdiImageRender m_Img1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetParentDlg(CWnd * dlg){ parentDlg = dlg; }
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void SetDrawRect(CRect & crt){ m_DrawRect = crt; }
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
