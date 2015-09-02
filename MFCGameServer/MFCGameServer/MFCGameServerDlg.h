
// MFCGameServerDlg.h : ͷ�ļ�
//

#pragma once

#include "Zoic/logger/logmonitorsender.h"


// CMFCGameServerDlg �Ի���
class CMFCGameServerDlg : public CDialogEx
{
// ����
public:
	CMFCGameServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCGAMESERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	bool static SendLogToMe(Zoic::LogContent* pLogContent);
	afx_msg void OnBnClickedButton1();
};
