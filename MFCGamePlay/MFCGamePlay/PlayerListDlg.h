#pragma once


// PlayerListDlg �Ի���

class PlayerListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PlayerListDlg)

public:
	PlayerListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PlayerListDlg();

// �Ի�������
	enum { IDD = IDD_PLAYERLISTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
