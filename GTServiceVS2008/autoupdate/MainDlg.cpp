// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <atlimage.h>
#include "resource.h"
#include "aboutdlg.h"
#include "maindlg.h"
#include "core.h"
#include "charset_helper.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMainDlg::OnIdle()
{
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);


	//UIAddChildWindowContainer(m_hWnd);
	//ModifyStyle(0,WS_CAPTION,SWP_DRAWFRAME);
	//////////////////////////////////////////////////////////////////////////
	m_processbar = GetDlgItem(IDC_WORKPS);
	m_processbar.SetRange(0,100);
	
	
	//load bk img
	CString strPath = GetLocalPath()+_T("\\bk.png");
	CImage image;
	image.LoadFromResource(_Module.GetResourceInstance(),IDB_BITMAP1);
	if (!image.IsNull())
	{
		CRect nwrect;
		GetWindowRect(nwrect);
		nwrect.bottom = nwrect.top + image.GetHeight()+1;
		nwrect.right = nwrect.left + image.GetWidth()+1;
		SetWindowPos(0,nwrect,SWP_NOMOVE);
		m_brBack.CreatePatternBrush(image);
		m_txtreport.SetColors(RGB(255,255,255),RGB(0,0,0));
	}
	else
	{
		COLORREF bkcolor = RGB(111,140,222);
		m_txtreport.SetColors(bkcolor,RGB(255,255,255));
		m_brBack.CreateSolidBrush(bkcolor);
	}
	m_txtreport.SubclassWindow(GetDlgItem(IDC_TXTREPORT));

	CRgn m_rgn;
	CRect rc; 
	GetWindowRect(&rc);
	rc -= rc.TopLeft();
	m_rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,5,5);
	SetWindowRgn(m_rgn,TRUE);

	return TRUE;
}

LRESULT CMainDlg::OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if( m_brBack.IsNull()) 
	{
		return DefWindowProc();
	}
	else
	{
		return (LRESULT) (HBRUSH) m_brBack;
	}
}

LRESULT CMainDlg::OnCopyData(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	//SetWindowText(_T("host msg!!"));
	COPYDATASTRUCT  *pcds=(COPYDATASTRUCT*)lParam;
	if (!pcds)
	{
		return 0;
	}

	if (pcds->dwData == 1)//close 
	{
		CloseDialog(0);
	}
	else if (pcds->dwData ==2)//confirm to update
	{
		m_pupdator->m_confirmed = pcds->cbData;

		WPARAM wParam=MAKEWPARAM(IDC_RUN_UPDATE,0);
		PostMessage(WM_COMMAND,wParam,0);
	}
	else if(pcds->dwData == 3)//need restart
	{
		m_pupdator->NeedRestart(TRUE);
		WPARAM wParam=MAKEWPARAM(IDC_RUN_UPDATE,0);
		PostMessage(WM_COMMAND,wParam,0);
	}
	return 0;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	//m_pupdator->Log(_T("dlg closed"));
	return 0;
}

LRESULT CMainDlg::OnUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	RedrawWindow();
	Running();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	//m_pupdator->Log(_T("close dlg"));
	DestroyWindow();
	::PostQuitMessage(nVal);
}

void CMainDlg::ShowMsg(const int type,const int res)
{
	CString msg;
	int msgid =0;
	switch(type)
	{
		case CUpdator::E_LOAD_VER:
			{
				if (res)
				{
					msgid = IDS_VER_LOAD_OK;
				}
				else
				{
					msgid = IDS_VER_LOAD_FALSE;
				}
			}
			break;
		case CUpdator::E_CONFIG_LOAD:
			{
				if (res)
				{
					msgid = IDS_CONFIG_LOAD_OK;
				}
				else
				{
					msgid = IDS_CONFIG_LOAD_FALSE;
				}
			}
			break;
		case CUpdator::E_DOWNLOAD:
			{
				if (res)
				{
					msgid = IDS_FILELOAD_OK;
				}
				else
				{
					msgid = IDS_FILELOAD_FALSE;
				}
			}
			break;
		case CUpdator::E_UPDATE_OVER:
			{
				if (res ==1)
				{
					msgid = IDS_UPDATE_OK;
					//
					int lower,higher;
					m_processbar.GetRange(lower,higher);
					m_processbar.SetPos(higher);
				}
				else if (res ==2)//no need update
				{
					msgid = IDS_NONEEDUPDATE;
					//
					int lower,higher;
					m_processbar.GetRange(lower,higher);
					m_processbar.SetPos(higher);
				}
				else if(res ==-1)
				{
					msgid = IDS_CONNECT_FAILED;
				}
				else
				{
					msgid = IDS_UPDATE_FALSE;
				}
			}
			break;
		case CUpdator::E_CHECKED:
			{
				if (res)
				{
					msgid = IDS_CHECK_OK;
				}
				else
				{
					msgid = IDS_CHECK_FALSE;
				}
			}
			break;
	}
	msg.FormatMessage(msgid);
	m_txtreport.SetWindowText(msg);
}

int CMainDlg::UIEventHandler(const Event &evt)
{
	switch(evt.type)
	{
	case CUpdator::E_LOAD_VER:
	case CUpdator::E_CONFIG_LOAD:
	case CUpdator::E_DOWNLOAD:
	case CUpdator::E_CHECKED:
		{
			int res= (int)evt.dwData;
			ShowMsg(evt.type,res);
		}
		break;
	case CUpdator::E_UPDATE_OVER:
		{
			int res= (int)evt.dwData;
			ShowMsg(evt.type,res);
		}
		break;
	case CUpdator::E_LOADING:
		{
			struct dfinfo{
				char src[256];
				char tar[256];
				bool result;
			};
			dfinfo* pdf = (dfinfo*)evt.tar_obj;

			if (pdf->result)
			{
				m_txtreport.SetWindowText(pdf->src);
				m_processbar.StepIt();
			}
		}
		break;
	case CUpdator::E_TOTALFILES:
		{
			int pusize= (int)evt.dwData;
			char buf[256]={0};
			sprintf(buf,_T("%d 个文件需要更新."),pusize);
			m_txtreport.SetWindowText(buf);
			m_processbar.SetRange(0,pusize);
			m_processbar.SetStep(1);
		}
		break;
	}
	RedrawWindow();
	return 1;
}

void CMainDlg::Running()
{
	GetDlgItem(IDC_RUN_UPDATE).EnableWindow(FALSE);
	m_processbar.SetPos(0);
	//////////////////////////////////////////////////////////////////////////
	int ret = 0;
	if(m_pupdator)
	{
		if (m_pupdator->IsNeedRestart())
		{
			m_pupdator->UpdateRestart();
			CloseDialog(0);
		}
		else
		{
			ret = m_pupdator->RunUpdate();
		}
	}
	
	GetDlgItem(IDC_RUN_UPDATE).EnableWindow(TRUE);
}

