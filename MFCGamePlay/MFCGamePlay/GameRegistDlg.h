#pragma once


// GameRegistDlg 对话框

class GameRegistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GameRegistDlg)

public:
	GameRegistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GameRegistDlg();

// 对话框数据
	enum { IDD = IDD_REGISTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
