#pragma once
#include "gdiplus.h"

using namespace Gdiplus;
class GdiImageRender
{
public:
	GdiImageRender();
	~GdiImageRender();

	BOOL SetImage(UINT nID, LPCTSTR sTR);
	void DrawImg(Image* pImg, HDC hdc, PointF pt, const ImageAttributes * attr = NULL);
	void Render(HDC hdc, PointF pt, const ImageAttributes * attr = NULL);
	UINT GetWidth();
	UINT GetHeight();
	Image* GetImage(){ return m_pImage; }
	void SetDrawRect(int x, int y, int width, int length){ m_DrawRect = Rect(x, y, width, length); }
private:
	Image* m_pImage;
	Rect m_DrawRect;
};

