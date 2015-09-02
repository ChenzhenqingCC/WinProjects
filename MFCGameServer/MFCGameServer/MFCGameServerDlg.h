
// MFCGameServerDlg.h : 头文件
//

#pragma once

#include "Zoic/logger/logmonitorsender.h"


// CMFCGameServerDlg 对话框
class CMFCGameServerDlg : public CDialogEx
{
// 构造
public:
	CMFCGameServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCGAMESERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
