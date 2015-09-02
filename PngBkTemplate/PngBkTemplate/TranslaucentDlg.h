#pragma once
#include "gdiplus.h"
//#include "PngBkTemplateDlg.h"
#include "ChildDlg.h"

using namespace Gdiplus;


// CTranslaucentDlg 对话框

class CTranslaucentDlg : public CDialog
{
	DECLARE_DYNAMIC(CTranslaucentDlg)

public:
	CTranslaucentDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTranslaucentDlg();

// 对话框数据
	enum { IDD = IDD_PARENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ReSetChildDlg();
	afx_msg void OnMove(int x, int y);
	void setImage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	void DrawTXBar();
	CRect GetCenterArea(){ return m_CenterArea; }
private:
	void UpdateView();
	Image* m_pImage;
	BLENDFUNCTION m_blend;
	ChildDlg* m_pMainDlg;
	CRect m_CenterArea;


};
