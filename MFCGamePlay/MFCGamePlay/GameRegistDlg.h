#pragma once


// GameRegistDlg �Ի���

class GameRegistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GameRegistDlg)

public:
	GameRegistDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GameRegistDlg();

// �Ի�������
	enum { IDD = IDD_REGISTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
