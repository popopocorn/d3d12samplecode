#include "stdafx.h"
#include "GameObject.h"

void CGameObject::SetPosition(int x, int y)
{
	m_xPosition = x;
	m_yPosition = y;
}

CRectangleObject::CRectangleObject(int nLeft, int nTop, int nRight, int nBottom)
{
	m_nWidth = nRight - nLeft;
	m_nHeight = nBottom - nTop;

	m_xPosition = int(m_nWidth * 0.5f);
	m_yPosition = int(m_nHeight * 0.5f);
}

void CRectangleObject::Render(HDC hDCFrameBuffer)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HBRUSH hBrush = ::CreateSolidBrush(m_dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
	HPEN hOldBrush = (HPEN)::SelectObject(hDCFrameBuffer, hBrush);
	int nHalfWidth = int(m_nWidth * 0.5f), nHalfHeight = int(m_nHeight * 0.5f);
	::Rectangle(hDCFrameBuffer, m_xPosition - nHalfWidth, m_yPosition - nHalfHeight, m_xPosition + nHalfWidth, m_yPosition + nHalfHeight);
	::SelectObject(hDCFrameBuffer, hOldPen);
	::SelectObject(hDCFrameBuffer, hOldBrush);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

CEllipseObject::CEllipseObject(int nLeft, int nTop, int nRight, int nBottom)
{
	m_nWidth = nRight - nLeft;
	m_nHeight = nBottom - nTop;

	m_xPosition = int(m_nWidth * 0.5f);
	m_yPosition = int(m_nHeight * 0.5f);
}

void CEllipseObject::Render(HDC hDCFrameBuffer)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HBRUSH hBrush = ::CreateSolidBrush(m_dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
	HPEN hOldBrush = (HPEN)::SelectObject(hDCFrameBuffer, hBrush);
	int nHalfWidth = int(m_nWidth * 0.5f), nHalfHeight = int(m_nHeight * 0.5f);
	::Ellipse(hDCFrameBuffer, m_xPosition - nHalfWidth, m_yPosition - nHalfHeight, m_xPosition + nHalfWidth, m_yPosition + nHalfHeight);
	::SelectObject(hDCFrameBuffer, hOldPen);
	::SelectObject(hDCFrameBuffer, hOldBrush);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

CRoundRectangleObject::CRoundRectangleObject(int nLeft, int nTop, int nRight, int nBottom, int nCornerWidth, int nCornerHeight) : CRectangleObject(nLeft, nTop, nRight, nBottom)
{ 
	m_nCornerWidth = nCornerWidth;
	m_nCornerHeight = nCornerHeight;
}

void CRoundRectangleObject::Render(HDC hDCFrameBuffer)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HBRUSH hBrush = ::CreateSolidBrush(m_dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
	HPEN hOldBrush = (HPEN)::SelectObject(hDCFrameBuffer, hBrush);
	int nHalfWidth = int(m_nWidth * 0.5f), nHalfHeight = int(m_nHeight * 0.5f);
	::RoundRect(hDCFrameBuffer, m_xPosition - nHalfWidth, m_yPosition - nHalfHeight, m_xPosition + nHalfWidth, m_yPosition + nHalfHeight, m_nCornerWidth, m_nCornerHeight);
	::SelectObject(hDCFrameBuffer, hOldPen);
	::SelectObject(hDCFrameBuffer, hOldBrush);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

CPolygonObject::CPolygonObject(int nLeft, int nTop, int nRight, int nBottom)
{
	m_xPosition = int((nRight - nLeft) * 0.5f);
	m_yPosition = int((nBottom - nTop) * 0.5f);
}

void CPolygonObject::SetVertices(int nVertices, POINT* pVertices)
{
	m_nVertices = nVertices;
	m_pVertices = new POINT[m_nVertices];
	::memcpy(m_pVertices, pVertices, sizeof(POINT) * m_nVertices);
}

void CPolygonObject::Render(HDC hDCFrameBuffer)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HBRUSH hBrush = ::CreateSolidBrush(m_dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
	HPEN hOldBrush = (HPEN)::SelectObject(hDCFrameBuffer, hBrush);
	POINT *pVertices = new POINT[m_nVertices];
	for (int i = 0; i < m_nVertices; i++) { pVertices[i].x = m_pVertices[i].x + m_xPosition; pVertices[i].y = m_pVertices[i].y + m_yPosition; }
	::Polygon(hDCFrameBuffer, pVertices, m_nVertices);
	::SelectObject(hDCFrameBuffer, hOldPen);
	::SelectObject(hDCFrameBuffer, hOldBrush);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
	delete[] pVertices;
}

CStarObject::CStarObject(int nLeft, int nTop, int nRight, int nBottom) : CPolygonObject(nLeft, nTop, nRight, nBottom)
{
	int nHalfWidth = int((nRight - nLeft) * 0.5f);
	int nHalfHeight = int((nBottom - nTop) * 0.5f);
	int x0 = -nHalfWidth, x1 = -int(nHalfWidth * (2.0f / 3.0f)), x2 = -int(nHalfWidth * (1.0f / 3.0f)), x3 = 0, x4 = int(nHalfWidth * (1.0f / 3.0f)), x5 = int(nHalfWidth * (2.0f / 3.0f)), x6 = nHalfWidth;
	int y0 = -nHalfHeight, y1 = -int(nHalfHeight * 0.5f), y2 = 0, y3 = int(nHalfWidth * 0.5f), y4 = nHalfWidth;
	POINT pVertices[12] = { {x3,y0}, {x4,y1}, {x6,y1}, {x5,y2}, {x6,y3}, {x4,y3}, {x3,y4}, {x2,y3}, {x0,y3}, {x1,y2}, {x0,y1}, {x2,y1} };
	SetVertices(12, pVertices);
}

CHexagonObject::CHexagonObject(int nLeft, int nTop, int nRight, int nBottom) : CPolygonObject(nLeft, nTop, nRight, nBottom)
{
	int nHalfWidth = int((nRight - nLeft) * 0.5f);
	int nHalfHeight = int((nBottom - nTop) * 0.5f);
	int x0 = -nHalfWidth, x1 = -int(nHalfWidth * 0.5f), x2 = int(nHalfWidth * 0.5f), x3 = nHalfWidth;
	int y0 = -nHalfHeight, y1 = 0, y2 = nHalfHeight;
	POINT pVertices[6] = { {x0,y1}, {x1,y0}, {x2,y0}, {x3,y1}, {x2,y2}, {x1,y2} };
	SetVertices(6, pVertices);
}
