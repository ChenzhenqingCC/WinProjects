#pragma once


// PlayerListDlg 对话框

class PlayerListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PlayerListDlg)

public:
	PlayerListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PlayerListDlg();

// 对话框数据
	enum { IDD = IDD_PLAYERLISTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
