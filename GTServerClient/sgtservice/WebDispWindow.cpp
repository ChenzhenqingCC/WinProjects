#include "stdafx.h"
#include "WebDispWindow.h"


void WebDispWindow::Create(HINSTANCE hInstance, LPCWSTR className, int x, int y, int width, int height, LPCWSTR url)
{
	//	OleInitialize(0);
	
	jsobj = new JSObject();
	jsobj->AddRef();
	webformDispatchImpl = new WebformDispatchImpl(jsobj);
	
	webWindow = new WebWindow(webformDispatchImpl);
	webWindow->Create(hInstance, className, x, y, width, height, false);
	//webWindow->webForm->Go(_T("http://zt2.ztgame.com/activity/tips/tips.html"));
	webWindow->webForm->Go(url);
}

WebDispWindow::WebDispWindow()
{
	webWindow = NULL;
	jsobj = NULL;
	webformDispatchImpl = NULL;
}


WebDispWindow::~WebDispWindow()
{
	if (webWindow)
		delete webWindow;
	if (webformDispatchImpl)
		delete webformDispatchImpl;
	jsobj->Release();
}
