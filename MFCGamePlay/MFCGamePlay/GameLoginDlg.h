#pragma once


// GameLoginDlg �Ի���

class GameLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GameLoginDlg)

public:
	GameLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GameLoginDlg();

// �Ի�������
	enum { IDD = IDD_LOGINDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedRegist();
};
