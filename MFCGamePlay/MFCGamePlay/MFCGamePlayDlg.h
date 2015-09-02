
// MFCGamePlayDlg.h : 头文件
//

#pragma once
#include "Zoic/logger/logmonitorsender.h"
#include "GameLoginDlg.h"
#include "GameCreateRoleDlg.h"
#include "GameRegistDlg.h"
#include "PlayerListDlg.h"
#include "ServerDataDef.h"
#include <map>

using namespace GameDataDef;

enum MyStage
{
	LOGINSTAGE = 0,
	REGSITSTAGE = 1,
	CREATESTAGE = 2,
	CHOOSESTAGE = 3,
	FIGHTSTAGE = 4,
};

static const int myDlgList[] = { IDD_LOGINDIALOG, IDD_REGISTDLG, IDD_CREATEROLEDIALOG, IDD_PLAYERLISTDLG };

// CMFCGamePlayDlg 对话框
class CMFCGamePlayDlg : public CDialogEx
{
// 构造
public:
	CMFCGamePlayDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCGAMEPLAY_DIALOG };

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
	afx_msg void PostNcDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool static SendLogToMe(Zoic::LogContent* pLogContent);
	void SwitchStage(MyStage mystage);
	void SetPlayerList(std::vector<PlayerDBBase> & plist);
private:
	std::map<MyStage, CDialogEx *> myStageMap;
	CDialogEx * nowShowDlg;
};


#define ADDDLG_DEFINE(CLASS, NAME)	\
	CLASS* _DLG##CLASS = new CLASS(); \
	_DLG##CLASS->Create(CLASS::IDD, this); \
	_DLG##CLASS->ShowWindow(SW_HIDE); \
	myStageMap[NAME] = _DLG##CLASS
