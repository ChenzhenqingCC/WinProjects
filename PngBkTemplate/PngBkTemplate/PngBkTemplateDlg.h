
// PngBkTemplateDlg.h : ͷ�ļ�
//

#pragma once


// CPngBkTemplateDlg �Ի���
class CPngBkTemplateDlg : public CDialogEx
{
// ����
public:
	CPngBkTemplateDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PNGBKTEMPLATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	

private:
	CBitmap* m_pBitmap;
	CDC* m_pMemDC;
	int m_nScreenX;
	int m_nScreenY;

	CBrush m_brush;
	CWnd* parentDlg;
	void DrawSomething();

	


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
