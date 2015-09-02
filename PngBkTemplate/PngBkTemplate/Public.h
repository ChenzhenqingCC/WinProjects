#pragma once

#include "stdafx.h"
#include <atlimage.h>

//��ť��״̬
enum
{
	CTRL_NOFOCUS = 0x01,			//��ͨ
	CTRL_FOCUS,						//mousemove
	CTRL_SELECTED,					//buttondown
	CTRL_DISABLE,					//��Ч
};

//ͼƬ��ʽ
enum
{
	BTN_IMG_1 = 1,					//
	BTN_IMG_3 = 3,					//3��ͼ��1��ͼƬ����3Сͼ����ͬ��
	BTN_IMG_4 = 4,					//4��ͼ
};

//��ť����
enum
{
	BTN_TYPE_NORMAL = 0x10,			//��ͨBTN
	BTN_TYPE_MENU,					//�˵����͵�BTN
	BTN_TYPE_STATIC,				//��̬���͵�BTN
};

#define WM_RESTART		(WM_USER + 109)

using namespace Gdiplus;
//����Դ�������λͼ
void GdiImageFromResource(Image** pImg, UINT uImgID, LPCTSTR lpType);
bool LoadImageFromResourse(CImage* pImg, UINT nImgID, LPCTSTR lpImgType);          
bool LoadPicture(CImage& bmp, UINT nImgID, LPCTSTR lpImgType = _T("PNG"));			//��Alphaͨ����ͼƬ�����CImage
//bool SetLayeredWindowAttributesT(_In_ HWND hwnd, _In_ COLORREF crKey, _In_ BYTE bAlpha, _In_ DWORD dwFlags);
bool UpdateLayeredWindowT(HWND hWnd, HDC hdcDst, POINT *pptDst, SIZE* psize, HDC hdcSrc, POINT* pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);

