#include "stdafx.h"
#include "Public.h"

bool UpdateLayeredWindowT(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE* psize, HDC hdcSrc, POINT* pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags)
{
	typedef BOOL(WINAPI * lpfnUpdateLayeredWindow)(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE* psize, HDC hdcSrc, POINT* pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);
	lpfnUpdateLayeredWindow UpdateLayeredWindow;
	HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
	UpdateLayeredWindow = (lpfnUpdateLayeredWindow)GetProcAddress(hUser32, "UpdateLayeredWindow");
	UpdateLayeredWindow(hWnd, hdcDst, pptDst, psize, hdcSrc, pptSrc, crKey, pblend, dwFlags);
	FreeLibrary(hUser32);
	return true;
}

void GdiImageFromResource(Image** pImg, UINT uImgID, LPCTSTR lpType)
{
	HINSTANCE hInstance = AfxGetResourceHandle();
	HRSRC hResInfo = ::FindResource(hInstance, MAKEINTRESOURCE(uImgID), lpType);
	if (hResInfo == NULL)
		return; //fail
	DWORD dwSize;
	dwSize = SizeofResource(hInstance, hResInfo); //get resource size(bytes) 
	HGLOBAL hResData;
	hResData = ::LoadResource(hInstance, hResInfo);
	if (hResData == NULL)
		return; //fail
	HGLOBAL hMem;
	hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
	if (hMem == NULL){
		::FreeResource(hResData);
		return;
	}
	LPVOID lpResData, lpMem;
	lpResData = ::LockResource(hResData);
	lpMem = ::GlobalLock(hMem);
	::CopyMemory(lpMem, lpResData, dwSize); //copy memory
	::GlobalUnlock(hMem);
	::FreeResource(hResData); //free memory

	IStream *pStream;
	HRESULT hr;
	hr = ::CreateStreamOnHGlobal(hMem, TRUE, &pStream);//create stream object
	//Image *pImage = NULL;
	if (SUCCEEDED(hr)){
		*pImg = Image::FromStream(pStream);//get GDI+ pointer
		pStream->Release();
	}
	::GlobalFree(hMem);
	//return pImage;
}

bool LoadImageFromResourse(CImage* pImg, UINT nImgID, LPCTSTR lpImgType)          
{
	if (pImg == NULL)
	{
		return FALSE;
	}
	pImg->Destroy();

	//查找资源
	HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nImgID), lpImgType);
	if (hRsrc == NULL)
	{
		return false;
	}

	//加载资源
	HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}


	LPVOID lpVoid = ::LockResource(hImgData);							//锁定内存中指定资源
	LPSTREAM pStream = NULL;
	DWORD dwSize = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);
	::GlobalUnlock(hNew);									           //解除资源锁定

	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if(ht != S_OK)
	{
		GlobalFree(hNew);
	}
	else
	{
		//加载图片
		pImg->Load(pStream);
		GlobalFree(hNew);
	}

	//释放资源
	::FreeResource(hImgData);
	return true;
}

bool LoadPicture(CImage& bmp, UINT nImgID, LPCTSTR lpImgType)			//含Alpha通道的图片处理成CImage
{
	LoadImageFromResourse(&bmp, nImgID, lpImgType);					//加载图片资源

	if (bmp.IsNull())
	{
		return false;
	}
	if(bmp.GetBPP() == 32)												//确认该图片包含Alpha通道
	{
		for (int i=0; i < bmp.GetWidth(); i++)
		{
			for(int j=0; j < bmp.GetHeight(); j++)
			{
				byte* pByte = (byte*)bmp.GetPixelAddress(i, j);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}

	return true;
}
