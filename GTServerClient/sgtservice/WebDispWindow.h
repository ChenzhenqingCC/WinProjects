#pragma once
#include "webui/webwindow.h"
#include "webui/jsobject.h"
#include "webui/webformdispatchimpl.h"

class WebDispWindow
{
public:
	WebDispWindow();
	~WebDispWindow();
	void Create(HINSTANCE hInstance, LPCWSTR className, int x, int y, int width, int height, LPCWSTR url);

private:
	WebWindow* webWindow;
	JSObject *jsobj;
	WebformDispatchImpl *webformDispatchImpl;
};

