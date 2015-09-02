#pragma once


// GameCreateRoleDlg 对话框

class GameCreateRoleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GameCreateRoleDlg)

public:
	GameCreateRoleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GameCreateRoleDlg();

// 对话框数据
	enum { IDD = IDD_CREATEROLEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
