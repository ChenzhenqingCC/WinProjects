
// PngBkTemplateDlg.h : 头文件
//

#pragma once


// CPngBkTemplateDlg 对话框
class CPngBkTemplateDlg : public CDialogEx
{
// 构造
public:
	CPngBkTemplateDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PNGBKTEMPLATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	

private:
	CBitmap* m_pBitmap;
	CDC* m_pMemDC;
	int m_nScreenX;
	int m_nScreenY;

	CBrush m_brush;
	CWnd* parentDlg;
	void DrawSomething();

	


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void SetParentDlg(CWnd * dlg){ parentDlg = dlg; }
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMove(int x, int y);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};
