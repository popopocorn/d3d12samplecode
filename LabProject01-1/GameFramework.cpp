//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"

void CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	BuildFrameBuffer(); 

	BuildObjects(); 

	_tcscpy_s(m_pszFrameRate, _T("LabProject ("));
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
	if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);
}

void CGameFramework::BuildFrameBuffer()
{
	::GetClientRect(m_hWnd, &m_rcClient);

	HDC hDC = ::GetDC(m_hWnd);

    m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
    ::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
    ::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 0, dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(m_hDCFrameBuffer, hPen);
	HBRUSH hBrush = ::CreateSolidBrush(dwColor);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	::Rectangle(m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);
	::SelectObject(m_hDCFrameBuffer, hOldBrush);
	::SelectObject(m_hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{    
    HDC hDC = ::GetDC(m_hWnd);
    ::BitBlt(hDC, m_rcClient.left, m_rcClient.top, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top, m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, SRCCOPY);
    ::ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::BuildObjects()
{
	random_device randomdevice;
	m_pRandomGenerator = new mt19937(randomdevice());

	m_pSizeDistribution = new uniform_int_distribution<int>(20, 100);
	m_pPositionDistribution = new uniform_int_distribution<int>(20, FRAME_BUFFER_WIDTH - 20);
	m_pColorDistribution = new uniform_int_distribution<int>(0, 255);

	m_nGameObjects = 2500;
	m_ppGameObjects = new CGameObject*[m_nGameObjects];
	for (int i = 0; i < m_nGameObjects; i++)
	{
		int x = (*m_pPositionDistribution)(*m_pRandomGenerator);
		int y = (*m_pPositionDistribution)(*m_pRandomGenerator);
		int nHalfWidth = (*m_pSizeDistribution)(*m_pRandomGenerator);
		int nHalfHeight = (*m_pSizeDistribution)(*m_pRandomGenerator);

		int nType = (i % 5);
		switch (nType)
		{
			case 0:
				m_ppGameObjects[i] = new CRectangleObject(x - nHalfWidth, y - nHalfHeight, x + nHalfWidth, y + nHalfHeight);
				break;
			case 1:
				m_ppGameObjects[i] = new CRoundRectangleObject(x - nHalfWidth, y - nHalfHeight, x + nHalfWidth, y + nHalfHeight, int(nHalfWidth * 0.3f), int(nHalfHeight * 0.3f));
				break;
			case 2:
				m_ppGameObjects[i] = new CEllipseObject(x - nHalfWidth, y - nHalfHeight, x + nHalfWidth, y + nHalfHeight);
				break;
			case 3:
				m_ppGameObjects[i] = new CStarObject(x - nHalfWidth, y - nHalfWidth, x + nHalfWidth, y + nHalfWidth);
				break;
			case 4:
				m_ppGameObjects[i] = new CHexagonObject(x - nHalfWidth, y - nHalfWidth, x + nHalfWidth, y + nHalfWidth);
				break;
		}
		m_ppGameObjects[i]->SetType(nType);
	}
}

void CGameFramework::AnimateObjects()
{
	for (int i = 0; i < m_nGameObjects; i++)
	{
		int x = (*m_pPositionDistribution)(*m_pRandomGenerator);
		int y = (*m_pPositionDistribution)(*m_pRandomGenerator);
		m_ppGameObjects[i]->SetPosition(x, y);
		m_ppGameObjects[i]->SetColor(RGB((*m_pColorDistribution)(*m_pRandomGenerator), (*m_pColorDistribution)(*m_pRandomGenerator), (*m_pColorDistribution)(*m_pRandomGenerator)));
	}
}

void CGameFramework::ReleaseObjects()
{
	for (int i = 0; i < m_nGameObjects; i++) delete m_ppGameObjects[i];
	delete[] m_ppGameObjects;

	delete m_pRandomGenerator;
	delete m_pSizeDistribution;
	delete m_pPositionDistribution;
	delete m_pColorDistribution;
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		case VK_CONTROL:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		case WM_ACTIVATE:
			break;
		case WM_SIZE:
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
			OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
			OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
			break;
	}
	return(0);
}

void CGameFramework::ProcessInput()
{
}

void CGameFramework::FrameAdvance()
{    
	m_GameTimer.Tick(30.0f);

	ProcessInput();

	AnimateObjects();

    ClearFrameBuffer(RGB(255, 255, 255));

	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->Render(m_hDCFrameBuffer);

	PresentFrameBuffer();

	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	::SetWindowText(m_hWnd, m_pszFrameRate);
}


