#pragma once


// GameCreateRoleDlg �Ի���

class GameCreateRoleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GameCreateRoleDlg)

public:
	GameCreateRoleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GameCreateRoleDlg();

// �Ի�������
	enum { IDD = IDD_CREATEROLEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
