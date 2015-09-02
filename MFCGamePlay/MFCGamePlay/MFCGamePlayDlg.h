
// MFCGamePlayDlg.h : ͷ�ļ�
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

// CMFCGamePlayDlg �Ի���
class CMFCGamePlayDlg : public CDialogEx
{
// ����
public:
	CMFCGamePlayDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCGAMEPLAY_DIALOG };

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
