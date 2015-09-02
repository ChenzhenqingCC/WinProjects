#include "stdafx.h"
#include "GdiImageRender.h"
#include "Public.h"


GdiImageRender::GdiImageRender()
{
	m_DrawRect = Rect(0, 0, 0, 0);
}


GdiImageRender::~GdiImageRender()
{
}


void GdiImageRender::DrawImg(Image* pImg, HDC hdc, PointF pt, const ImageAttributes * attr)
{
	Graphics graph(hdc);
	if (pImg)
	{
		Rect crt(pt.X, pt.Y, m_DrawRect.Width, m_DrawRect.Height);
		graph.DrawImage(pImg, crt, m_DrawRect.GetLeft(), m_DrawRect.GetTop(), m_DrawRect.Width, m_DrawRect.Height, UnitPixel);
		/*if (attr)
		{
		graph.DrawImage(pImg, crt, m_DrawRect.GetLeft(), m_DrawRect.GetTop(), m_DrawRect.Width, m_DrawRect.Height, UnitPixel, attr);
		}
		else
		{
		graph.DrawImage(pImg, crt, m_DrawRect.GetLeft(), m_DrawRect.GetTop(), m_DrawRect.Width, m_DrawRect.Height, UnitPixel);
		}*/
	}
}

BOOL GdiImageRender::SetImage(UINT nID, LPCTSTR sTR)
{
	GdiImageFromResource(&m_pImage, nID, sTR);
	if (m_pImage)
		m_DrawRect = Rect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
	return TRUE;
}

void GdiImageRender::Render(HDC hdc, PointF pt, const ImageAttributes * attr)
{
	DrawImg(m_pImage, hdc, pt, attr);
}
UINT GdiImageRender::GetWidth()
{
	return m_pImage ? m_pImage->GetWidth() : 0;
}
UINT GdiImageRender::GetHeight()
{
	return m_pImage ? m_pImage->GetHeight() : 0;
}