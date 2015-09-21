////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	HtmlPanel    version:  1.0   ? date: 08/20/2009
//  -------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef HTMLPANEL_H
#define HTMLPANEL_H
#pragma once
#include <vector>
#include <map>
using namespace std;

#include <atlconv.h>
#include <atlcrack.h>
#include <atlfile.h>
#include <exdispid.h>
#include <mshtmcid.h>
//#include <mshtmhst.h>
#include <UrlMon.h>

#ifdef PAGESAVE
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "EndOfFile")
#import "cdoex.dll"  rename("EOF", "EndOfFile")
#endif
//you must define these values before,let these as the resource ids of host dialog and webctrl
#define IDD_WEB_DLG					130
#define IDD_WEB_DLG2					101
#define IDC_WEB_CTRL					1000
#define WM_POSTSCRIPT				WM_APP+3
#define WM_BROWSEREVENT		WM_APP+4

#ifndef IDR_MAINFRAME
#define IDR_MAINFRAME 128
#endif
/////////////////////////////////////////////////////////////////////////////////////////////
//#define DISPID_WINDOWSTATECHANGED  279
#define DISPID_THIRDPARTYURLBLOCKED  280
//#define DISPID_SETPHISHINGFILTERSTATUS  281
#define DISPID_NEWPROCESS  282
#define DISPID_ShowContextMenu	 2283
#define DISPID_GetHostInfo	 2284
#define DISPID_ShowUI	 2285
#define DISPID_HideUI	 2286
#define DISPID_UpdateUI	 2287
#define DISPID_EnableModeless	 2288
#define DISPID_OnDocWindowActivate	 2289
#define DISPID_OnFrameWindowActivate	 2290
#define DISPID_ResizeBorder	 2291
#define DISPID_TranslateAccelerator	 2292
#define DISPID_GetOptionKeyPath	 2293
#define DISPID_GetDropTarget	 2294
#define DISPID_GetExternal	 2295
#define DISPID_TranslateUrl	 2296
#define DISPID_FilterDataObject	 2297
#define DISPID_ExternalCall	2298
/////////////////////////////////////////////////////////////////////////////////////////////
// void OnUserDef(WPARAM,LPARAM)
#define MSG_WM_USERDEF(msg,func) \
	if (uMsg == msg) \
{ \
	SetMsgHandled(TRUE); \
	func(wParam,lParam); \
	lResult = 0; \
	if(IsMsgHandled()) \
	return TRUE; \
}

class CWebCtrlUIHandler :  public CComObjectRootEx<CComSingleThreadModel>
	,public IDispatchImpl<IDocHostUIHandlerDispatch, &IID_IDocHostUIHandlerDispatch, &LIBID_ATLLib>
{
public:
	BEGIN_COM_MAP(CWebCtrlUIHandler)
		COM_INTERFACE_ENTRY(IDocHostUIHandlerDispatch)
	END_COM_MAP()

	CWebCtrlUIHandler():m_pwpanel(0)
	{

	}
public:
	//implements
	struct WB_ShowContextMenu{
		DWORD dwID;
		DWORD x;
		DWORD y;
		HRESULT *dwRetVal;
	};
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( 
		/* [in] */ DWORD dwID,
		/* [in] */ DWORD x,
		/* [in] */ DWORD y,
		/* [in] */ IUnknown *pcmdtReserved,
		/* [in] */ IDispatch *pdispReserved,
		/* [retval][out] */ HRESULT *dwRetVal);
	/**
	*
	*/
	struct  WB_GetHostInfo{
		DWORD *pdwFlags;
		DWORD *pdwDoubleClick;
	};
	virtual HRESULT STDMETHODCALLTYPE GetHostInfo( 
		/* [out][in] */ DWORD *pdwFlags,
		/* [out][in] */ DWORD *pdwDoubleClick);
	/**
	*
	*/
	struct WB_ShowUI{
		DWORD dwID;
		IUnknown *pActiveObject;
		IUnknown *pCommandTarget;
		IUnknown *pFrame;
		IUnknown *pDoc;
		HRESULT *dwRetVal;
	};
	virtual HRESULT STDMETHODCALLTYPE ShowUI( 
		/* [in] */ DWORD dwID,
		/* [in] */ IUnknown *pActiveObject,
		/* [in] */ IUnknown *pCommandTarget,
		/* [in] */ IUnknown *pFrame,
		/* [in] */ IUnknown *pDoc,
		/* [retval][out] */ HRESULT *dwRetVal);
	/**
	*
	*/
	virtual HRESULT STDMETHODCALLTYPE HideUI( void);
	/**
	*
	*/
	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void) ;
	/**
	*
	*/
	virtual HRESULT STDMETHODCALLTYPE EnableModeless( 
		/* [in] */ VARIANT_BOOL fEnable) ;

	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( 
		/* [in] */ VARIANT_BOOL fActivate) ;

	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( 
		/* [in] */ VARIANT_BOOL fActivate) ;
	/**
	*
	*/
	struct WB_ResizeBorder{
		long left;
		long top;
		long right;
		long bottom;
		IUnknown *pUIWindow;
		VARIANT_BOOL fFrameWindow;
	};
	virtual HRESULT STDMETHODCALLTYPE ResizeBorder( 
		/* [in] */ long left,
		/* [in] */ long top,
		/* [in] */ long right,
		/* [in] */ long bottom,
		/* [in] */ IUnknown *pUIWindow,
		/* [in] */ VARIANT_BOOL fFrameWindow) ;
	/**
	*
	*/
	struct WB_TranslateAccelerator{
		DWORD_PTR hWnd;
		DWORD nMessage;
		DWORD_PTR wParam;
		DWORD_PTR lParam;
		BSTR bstrGuidCmdGroup;
		DWORD nCmdID;
		HRESULT *dwRetVal;
	};
	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
		/* [in] */ DWORD_PTR hWnd,
		/* [in] */ DWORD nMessage,
		/* [in] */ DWORD_PTR wParam,
		/* [in] */ DWORD_PTR lParam,
		/* [in] */ BSTR bstrGuidCmdGroup,
		/* [in] */ DWORD nCmdID,
		/* [retval][out] */ HRESULT *dwRetVal);
	/**
	*
	*/
	struct WB_GetOptionKeyPath{
		BSTR *pbstrKey;
		DWORD dw;
	};
	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath( 
		/* [out] */ BSTR *pbstrKey,
		/* [in] */ DWORD dw) ;
	/**
	*
	*/
	struct WB_GetDropTarget{
		IUnknown *pDropTarget;
		IUnknown **ppDropTarget;
	};
	virtual HRESULT STDMETHODCALLTYPE GetDropTarget( 
		/* [in] */ IUnknown *pDropTarget,
		/* [out] */ IUnknown **ppDropTarget) ;
	/**
	*
	*/
	virtual HRESULT STDMETHODCALLTYPE GetExternal( 
		/* [out] */ IDispatch **ppDispatch);
	/**
	*
	*/
	struct WB_TranslateUrl{
		/* [in] */ DWORD dwTranslate;
		/* [in] */ BSTR bstrURLIn;
		/* [out] */ BSTR *pbstrURLOut;
	};
	virtual HRESULT STDMETHODCALLTYPE TranslateUrl( 
		/* [in] */ DWORD dwTranslate,
		/* [in] */ BSTR bstrURLIn,
		/* [out] */ BSTR *pbstrURLOut) ;
	/**
	*
	*/
	struct WB_FilterDataObject{
		/* [in] */ IUnknown *pDO;
		/* [out] */ IUnknown **ppDORet;
	};
	virtual HRESULT STDMETHODCALLTYPE FilterDataObject( 
		/* [in] */ IUnknown *pDO,
		/* [out] */ IUnknown **ppDORet) ;
public:
	//CAxWindow*		m_pwpanel;
	CWindow*				m_pwpanel;
};

/****************************************************************************************************************************/
/* web control host dialog template
//all the web-style dialog are from this class,just different page
//so the IDD and IDC_WEB_CTRL can just use one
//another important thing is,when the call cross thread,you need use 
//main method to override
//CallByScript
//SetOptions
//CloseDialog
//InitControls
//PreTranslateMessage
//OnIdle
*/
/****************************************************************************************************************************/
class CHtmlPanel : public CAxDialogImpl<CHtmlPanel>
	,public IDispEventImpl<IDC_WEB_CTRL,CHtmlPanel>
{
public:
	//enum { IDD = IDD_WEB_DLG};
	int IDD;
	//ui message map
	BEGIN_MSG_MAP_EX(CHtmlPanel)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_USERDEF(WM_POSTSCRIPT,OnPostScript)
		MSG_WM_USERDEF(WM_BROWSEREVENT,OnBrowserEvent)
		CHAIN_MSG_MAP(CAxDialogImpl<CHtmlPanel>)
		END_MSG_MAP()
		//web control event map
		BEGIN_SINK_MAP(CHtmlPanel)
#ifdef DISPID_NEWWINDOW3
			SINK_ENTRY(IDC_WEB_CTRL, 273, NewWindow3WebCtrl)
#endif
			SINK_ENTRY(IDC_WEB_CTRL, 250, BeforeNavigate2WebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 268, ClientToHostWindowWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 105, CommandStateChangeWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 259, DocumentCompleteWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 106, DownloadBeginWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 104, DownloadCompleteWebCtrl)
			//SINK_ENTRY(IDC_WEB_CTRL, 270, FileDownloadWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 252, NavigateComplete2WebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 271, NavigateErrorWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 284, NewProcessWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 251, NewWindow2WebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 273, NewWindow3WebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 258, OnFullScreenWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 256, OnMenuBarWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 253, OnQuitWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 257, OnStatusBarWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 260, OnTheaterModeWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 255, OnToolBarWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 254, OnVisibleWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 225, PrintTemplateInstantiationWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 226, PrintTemplateTeardownWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 272, PrivacyImpactedStateChangeWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 108, ProgressChangeWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 112, PropertyChangeWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 286, RedirectXDomainBlockedWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 282, SetPhishingFilterStatusWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 269, SetSecureLockIconWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 102, StatusTextChangeWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 285, ThirdPartyUrlBlockedWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 113, TitleChangeWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 227, UpdatePageStatusWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 263, WindowClosingWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 267, WindowSetHeightWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 264, WindowSetLeftWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 262, WindowSetResizableWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 265, WindowSetTopWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 266, WindowSetWidthWebCtrl)
			SINK_ENTRY(IDC_WEB_CTRL, 283, WindowStateChangedWebCtrl)
		END_SINK_MAP()
public:
	/**
	*
	*/
	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	/**
	*
	*/
	void OnSize(UINT nType, CSize size);
	/**
	*/
	void OnDestroy();
	/**
	*
	*/
	void OnPostScript(WPARAM wParam,LPARAM lParam);
	/**
	*
	*/
	void OnBrowserEvent(WPARAM wParam,LPARAM lParam);
public:
	/**
	*
	*/
	CHtmlPanel(LPCTSTR pname,const BOOL isheaped=FALSE);
	/**
	*
	*/
	void SetName(LPCTSTR name);
	/**
	*
	*/
	CString GetName();
	/**
	*
	*/
	CString GetOptionKeyPath();
	/**
	*
	*/
	void SetOptionKeyPath(LPCTSTR p);
	/**
	*
	*/
	CComPtr<IWebBrowser2> GetWebCtrl();
	/**
	*
	*/
	CAxWindow& GetWebWnd();
	/**
	*
	*/
	CWindow GetWebIEWnd();
	/**
	*
	*/
	void MaxSized();
	/**
	*load page from resource by id
	*/
	HRESULT Load(const CString& url);
	/**
	*
	*/
	HRESULT Save(LPCTSTR tarpath,LPCTSTR srcpath=0);
	/**
	*
	*/
	CComPtr<IHTMLDocument2> Document() ;
	/**
	*
	*/
	CComPtr<IHTMLBodyElement> Body();
	/**
	*
	*/
	CComPtr<IHTMLWindow2> Window();
	/**
	*
	*/
	CComPtr<IHTMLElement> Head();
	/**
	*get element from document or specified element
	*@pstrName: 
	*document
	*body
	*window
	*[#|.]str  or number
	*
	*/
	map<int,CComPtr<IHTMLElement> > Elements(const CString& pstrName,const CComPtr<IHTMLElement>& spEl=0) ;
	/**
	*@bwhere:
	*	beforeBegin
	*	afterBegin
	*	beforeEnd
	*	afterEnd
	*/
	HRESULT InsertHtml(LPCTSTR pstr,LPCTSTR pwhere=_T("afterBegin"),const CComPtr<IHTMLElement>& spEl=0);
	/**
	*When using the IHTMLElement::insertAdjacentHTML method to insert script,
	*must include the DEFER attribute in the script element
	*/
	HRESULT AddScript(LPCTSTR psrc,const BOOL issrc=FALSE);
	/**
	*
	*/
	HRESULT LoadScript(LPCTSTR path);
	/**
	*
	*/
	HRESULT AddCSS(LPCTSTR psrc,const long index=31);
	/**
	*
	*/
	HRESULT RunScript(LPCTSTR jfun);
	/**
	*run script,params is like [funname,arg1,arg2,..],so params len must > 0
	*/
	CComVariant RunScript(const CString& strfun,const vector<CString>&params);
	/**
	*used when called from another thread
	*this one should convert charset to multi type
	*should use self heap,which means to use HeapAlloc not GlobalAlloc
	*/
	BOOL PostScript(const CString& params);
	static BOOL PostScript(HWND hwnd,const CString& params);
	/**
	*
	*/
	BOOL EnableFeatures(DWORD features,BOOL enable=TRUE,DWORD flags=SET_FEATURE_ON_PROCESS);
public:
	/**
	*
	*/
	BOOL PreTranslateMessage(MSG* pMsg);
	/**
	*
	*/
	virtual void OnFinalMessage(HWND /*hWnd*/);
	/**
	*
	*/
	BOOL isHeapObject();
	/**
	*called by PreTranslateMessage
	*/
	BOOL DisableWebAccel(MSG* pMsg);
protected:
	/*
	*	
	*/
	void SetIcons();
	/*
	*	
	*/
	virtual BOOL InitControls();
	/*
	*	
	*/
	virtual void CloseDialog(int nVal);
	/*
	*	
	*/
	void ShowPropSheet();
	/**
	*
	*/
	void CustomWebUI();
	/**
	*
	*/
	HRESULT InitIESettings();
	/**
	*
	*/
	virtual void SetOptions(CComPtr<IAxWinAmbientDispatch>& spdisp);
	/**
	*called by web page
	*@apiname: the id of function
	*@params:the arguments include apiname as the first item
	*@res:result back to page
	*@return: operation result,if TRUE,means we handled this api,if FALSE means to flow down this api to anywhere
	*/
	virtual BOOL PreTranslateWebInvoke(CString& apiname,vector<CString>& params,CComVariant& res);
	/**
	*handle the web request from the host protocol:[app:verb&param1&param2...]
	*@from:the hwnd which the send request
	*@req:the protocol formatted as a vector
	*@return: if handle the request,return TRUE,or FALSE to let the default handle
	*the common requests:
	*1 panel.close()
	*2 panel.size(cx,cy)
	*3 panel.maxsize()
	*4 panel.minsize()
	*5 panel.create(pname,url)
	*6 panel.slide(pname)
	*7 panel.toggle(pname)
	*8 panel.style(pname,)
	*9 panel.move(pname,)
	*
	*/
	virtual CComVariant CallByScript(const CComVariant& vattr);
	/**
	*
	*/
	BOOL IsInnerCall(CString& callstr);
protected:
	CAxWindow									m_webwnd;
	CComPtr<IWebBrowser2>		m_spweb;
	BOOL												m_isheaped;
	CString											m_name;
	CString											m_optionKeyPath;
public:
	/**
	*
	*/
	struct WB_BeforeNavigate2{
		LPDISPATCH pDisp;
		VARIANT* URL;
		VARIANT* Flags;
		VARIANT* TargetFrameName;
		VARIANT* PostData;
		VARIANT* Headers;
		BOOL* Cancel;
	};
	void __stdcall BeforeNavigate2WebCtrl(LPDISPATCH pDisp, VARIANT* URL, \
		VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, \
		VARIANT* Headers, BOOL* Cancel)
	{
		WB_BeforeNavigate2 wbbn2={pDisp,URL,Flags,TargetFrameName,PostData,Headers,Cancel};
		SendMessage(WM_BROWSEREVENT,DISPID_BEFORENAVIGATE2,(LPARAM)&wbbn2);
	}
	/**
	*
	*/
	void __stdcall ClientToHostWindowWebCtrl(long* CX, long* CY)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_CLIENTTOHOSTWINDOW,MAKELPARAM(CX,CY));
	}
	/**
	*
	*/
	struct WB_CommandStateChange{
		long Command;
		BOOL Enable;
	};
	void __stdcall CommandStateChangeWebCtrl(long Command, BOOL Enable)
	{
		WB_CommandStateChange wbscs={Command,Enable};
		SendMessage(WM_BROWSEREVENT,DISPID_COMMANDSTATECHANGE,(LPARAM)&wbscs);
	}
	/**
	*
	*/
	struct WB_DocumentComplete {
		LPDISPATCH pDisp;
		VARIANT* URL;
	};
	void __stdcall DocumentCompleteWebCtrl(LPDISPATCH pDisp, VARIANT* URL)
	{
		WB_DocumentComplete wbdc={pDisp,URL};
		SendMessage(WM_BROWSEREVENT,DISPID_DOCUMENTCOMPLETE,(LPARAM)&wbdc);
	}
	/**
	*
	*/
	void __stdcall DownloadBeginWebCtrl()
	{
		SendMessage(WM_BROWSEREVENT,DISPID_DOWNLOADBEGIN,0);
	}
	/**
	*
	*/
	void __stdcall DownloadCompleteWebCtrl()
	{

		SendMessage(WM_BROWSEREVENT,DISPID_DOWNLOADCOMPLETE,0);
	}
	/**
	*
	*/
	struct WB_FileDownload{
		BOOL ActiveDocument;
		BOOL* Cancel;
	};
	void __stdcall FileDownloadWebCtrl(BOOL ActiveDocument, BOOL* Cancel)
	{
		WB_FileDownload wbfd={ActiveDocument,Cancel};
		SendMessage(WM_BROWSEREVENT,DISPID_FILEDOWNLOAD,(LPARAM)&wbfd);
	}
	/**
	*
	*/
	struct WB_NavigateComplete2{
		LPDISPATCH pDisp;
		VARIANT* URL;
	};
	void __stdcall NavigateComplete2WebCtrl(LPDISPATCH pDisp, VARIANT* URL)
	{
		WB_NavigateComplete2 wbnc={pDisp,URL};
		SendMessage(WM_BROWSEREVENT,DISPID_NAVIGATECOMPLETE2,(LPARAM)&wbnc);
	}
	/**
	*
	*/
	struct WB_NavigateError{
		LPDISPATCH pDisp;
		VARIANT* URL;
		VARIANT* Frame;
		VARIANT* StatusCode;
		BOOL* Cancel;
	};
	void __stdcall NavigateErrorWebCtrl(LPDISPATCH pDisp, VARIANT* URL, \
		VARIANT* Frame, VARIANT* StatusCode, BOOL* Cancel)
	{
		WB_NavigateError wbne={pDisp,URL,Frame,StatusCode,Cancel};
		SendMessage(WM_BROWSEREVENT,DISPID_NAVIGATEERROR,(LPARAM)&wbne);
	}
	/**
	*
	*/
	void __stdcall NewProcessWebCtrl(long lCauseFlag, LPDISPATCH pWB2, BOOL* Cancel)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_NEWPROCESS,(LPARAM)Cancel);
	}
	/**
	*
	*/
	struct WB_NewWindow3{
		LPDISPATCH* ppDisp;
		BOOL* Cancel;
		unsigned long dwFlags;
		BSTR bstrUrlContext;
		BSTR bstrUrl;
	}; 
	void __stdcall NewWindow2WebCtrl(LPDISPATCH* ppDisp, BOOL* Cancel)
	{
		WB_NewWindow3 wbnw3={ppDisp,Cancel,0};
		SendMessage(WM_BROWSEREVENT,DISPID_NEWWINDOW2,(LPARAM)&wbnw3);
	}
	/**
	*
	*/
	void __stdcall NewWindow3WebCtrl(LPDISPATCH* ppDisp, BOOL* Cancel, \
		unsigned long dwFlags, BSTR bstrUrlContext, BSTR bstrUrl)
	{
		WB_NewWindow3 wbnw3={ppDisp,Cancel,dwFlags,bstrUrlContext,bstrUrl};
		SendMessage(WM_BROWSEREVENT,DISPID_NEWWINDOW3,(LPARAM)&wbnw3);
	}
	/**
	*
	*/
	void __stdcall OnFullScreenWebCtrl(BOOL FullScreen)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_ONFULLSCREEN,FullScreen);
	}
	/**
	*
	*/
	void __stdcall OnMenuBarWebCtrl(BOOL MenuBar)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_ONMENUBAR,MenuBar);
	}
	/**
	*
	*/
	void __stdcall OnQuitWebCtrl()
	{
		SendMessage(WM_BROWSEREVENT,DISPID_ONQUIT,0);
	}
	/**
	*
	*/
	void __stdcall OnStatusBarWebCtrl(BOOL StatusBar)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_ONSTATUSBAR,StatusBar);
	}
	/**
	*
	*/
	void __stdcall OnTheaterModeWebCtrl(BOOL TheaterMode)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_ONTHEATERMODE,TheaterMode);
	}
	/**
	*
	*/
	void __stdcall OnToolBarWebCtrl(BOOL ToolBar)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_ONTOOLBAR,ToolBar);
	}
	/**
	*
	*/
	void __stdcall OnVisibleWebCtrl(BOOL Visible)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_ONVISIBLE,Visible);
	}
	/**
	*
	*/
	void __stdcall PrintTemplateInstantiationWebCtrl(LPDISPATCH pDisp)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_PRINTTEMPLATEINSTANTIATION,(LPARAM)pDisp);
	}
	/**
	*
	*/
	void __stdcall PrintTemplateTeardownWebCtrl(LPDISPATCH pDisp)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_PRINTTEMPLATETEARDOWN,(LPARAM)pDisp);
	}
	/**
	*
	*/
	void __stdcall PrivacyImpactedStateChangeWebCtrl(BOOL bImpacted)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_PRIVACYIMPACTEDSTATECHANGE,bImpacted);
	}
	/**
	*
	*/
	struct WB_ProgressChange{
		long Progress;
		long ProgressMax;
	};
	void __stdcall ProgressChangeWebCtrl(long Progress, long ProgressMax)
	{
		WB_ProgressChange wbpc={Progress,ProgressMax};
		SendMessage(WM_BROWSEREVENT,DISPID_PROGRESSCHANGE,(LPARAM)&wbpc);
	}
	/**
	*
	*/
	void __stdcall PropertyChangeWebCtrl(BSTR szProperty)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_PROPERTYCHANGE,(LPARAM)&szProperty);
	}
	/**
	*
	*/
	struct WB_RedirectXDomainBlocked{
		LPDISPATCH pDisp;
		VARIANT* StartURL;
		VARIANT* RedirectURL;
		VARIANT* Frame;
		VARIANT* StatusCode;
	};
	void __stdcall RedirectXDomainBlockedWebCtrl(LPDISPATCH pDisp, VARIANT* StartURL, \
		VARIANT* RedirectURL, VARIANT* Frame, VARIANT* StatusCode)
	{
		WB_RedirectXDomainBlocked wbrdib={pDisp,StartURL,RedirectURL,Frame,StatusCode};
		SendMessage(WM_BROWSEREVENT,DISPID_SETPHISHINGFILTERSTATUS,(LPARAM)&wbrdib);
	}
	/**
	*
	*/
	void __stdcall SetPhishingFilterStatusWebCtrl(long PhishingFilterStatus)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_SETPHISHINGFILTERSTATUS,PhishingFilterStatus);
	}
	/**
	*
	*/
	void __stdcall SetSecureLockIconWebCtrl(long SecureLockIcon)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_SETSECURELOCKICON,SecureLockIcon);
	}
	/**
	*
	*/
	void __stdcall StatusTextChangeWebCtrl(BSTR Text)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_STATUSTEXTCHANGE,(LPARAM)&Text);
	}
	/**
	*
	*/
	struct WB_ThirdPartyUrlBlocked{
		VARIANT* URL;
		unsigned long dwCount;
	};
	void __stdcall ThirdPartyUrlBlockedWebCtrl(VARIANT* URL, unsigned long dwCount)
	{
		WB_ThirdPartyUrlBlocked wbtpb={URL,dwCount};
		SendMessage(WM_BROWSEREVENT,DISPID_THIRDPARTYURLBLOCKED,(LPARAM)&wbtpb);
	}
	/**
	*
	*/
	void __stdcall TitleChangeWebCtrl(BSTR Text)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_TITLECHANGE,(LPARAM)&Text);
	}
	/**
	*
	*/
	struct WB_UpdatePageStatus {
		LPDISPATCH pDisp;
		VARIANT* nPage;
		VARIANT* fDone;
	};
	void __stdcall UpdatePageStatusWebCtrl(LPDISPATCH pDisp, VARIANT* nPage, VARIANT* fDone)
	{
		WB_UpdatePageStatus wbpt={pDisp,nPage,fDone};
		SendMessage(WM_BROWSEREVENT,DISPID_UPDATEPAGESTATUS,(LPARAM)&wbpt);
	}
	/**
	*
	*/
	struct WB_WindowClosing{
		BOOL IsChildWindow;
		BOOL* Cancel;
	};
	void __stdcall WindowClosingWebCtrl(BOOL IsChildWindow, BOOL* Cancel)
	{
		WB_WindowClosing wbwc={IsChildWindow,Cancel};
		SendMessage(WM_BROWSEREVENT,DISPID_WINDOWCLOSING,(LPARAM)&wbwc);
	}
	/**
	*
	*/
	void __stdcall WindowSetHeightWebCtrl(long Height)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_WINDOWSETHEIGHT,Height);
	}
	/**
	*
	*/
	void __stdcall WindowSetLeftWebCtrl(long Left)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_WINDOWSETLEFT,Left);
	}
	/**
	*
	*/
	void __stdcall WindowSetResizableWebCtrl(BOOL Resizable)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_WINDOWSETRESIZABLE,Resizable);
	}
	/**
	*
	*/
	void __stdcall WindowSetTopWebCtrl(long Top)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_WINDOWSETTOP,Top);
	}
	/**
	*
	*/
	void __stdcall WindowSetWidthWebCtrl(long Width)
	{
		SendMessage(WM_BROWSEREVENT,DISPID_WINDOWSETWIDTH,Width);
	}
	/**
	*
	*/
	struct WB_WindowStateChanged{
		DWORD dwWindowStateFlags;
		DWORD dwValidFlagsMask;
	};
	void __stdcall WindowStateChangedWebCtrl(unsigned long dwWindowStateFlags, unsigned long dwValidFlagsMask)
	{
		//
		WB_WindowStateChanged wbsc={dwWindowStateFlags,dwValidFlagsMask};
		SendMessage(WM_BROWSEREVENT,DISPID_WINDOWSTATECHANGED,(LPARAM)&wbsc);
	}
};
/**
*
*/
template<class T> inline T* CreatePanel(LPCTSTR name,LPCTSTR path,HWND parent=NULL)
{
	T* pp = new T(TRUE);
	if (pp)
	{
		pp->Create(parent);
		pp->SetName(name);
		pp->Load(path);
	}
	return pp;
}
template<class T> inline void DestroyPanel(T* pt)
{
	pt->DestroyWindow();
}
#endif