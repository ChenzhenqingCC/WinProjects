#pragma once


// GameLoginDlg 对话框

class GameLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GameLoginDlg)

public:
	GameLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GameLoginDlg();

// 对话框数据
	enum { IDD = IDD_LOGINDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedRegist();
};
