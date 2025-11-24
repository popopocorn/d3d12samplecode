#pragma once

class CGameObject
{
public:
	CGameObject() { }
	CGameObject(int xPosition, int yPosition) { m_xPosition = xPosition; m_yPosition = yPosition; }
	virtual ~CGameObject() { }

protected:
	bool						m_bActive = true;
	int							m_nType = -1;
	DWORD						m_dwColor = RGB(255, 0, 0);

	int							m_xPosition = 0;
	int							m_yPosition = 0;

public:
	void SetActive(bool bActive) { m_bActive = bActive; }
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	void SetType(int nType) { m_nType = nType; }
	int GetType() { return(m_nType); }
	void SetPosition(int x, int y);

public:
	virtual void Animate(float fElapsedTime) { }
	virtual void Render(HDC hDCFrameBuffer) { }
};

class CRectangleObject : public CGameObject
{
public:
	CRectangleObject() { }
	CRectangleObject(int nLeft, int nTop, int nRight, int nBottom);
	virtual ~CRectangleObject() { }

protected:
	int							m_nWidth = 0;
	int							m_nHeight = 0;

public:
	virtual void Render(HDC hDCFrameBuffer);
};

class CRoundRectangleObject : public CRectangleObject
{
public:
	CRoundRectangleObject() {}
	CRoundRectangleObject(int nLeft, int nTop, int nRight, int nBottom, int nCornerWidth, int nCornerHeight);
	virtual ~CRoundRectangleObject() { }

private:
	int							m_nCornerWidth = 0;
	int							m_nCornerHeight = 0;

public:
	virtual void Render(HDC hDCFrameBuffer);
};

class CEllipseObject : public CGameObject
{
public:
	CEllipseObject() { }
	CEllipseObject(int nLeft, int nTop, int nRight, int nBottom);
	virtual ~CEllipseObject() { }

private:
	int							m_nWidth = 0;
	int							m_nHeight = 0;

public:
	virtual void Render(HDC hDCFrameBuffer);
};

class CPolygonObject : public CGameObject
{
public:
	CPolygonObject() {}
	CPolygonObject(int nLeft, int nTop, int nRight, int nBottom);
	virtual ~CPolygonObject() { }

protected:
	POINT*						m_pVertices = NULL;
	int							m_nVertices = 0;

public:
	void SetVertices(int nVertices, POINT* pVertices);

	virtual void Render(HDC hDCFrameBuffer);
};

class CStarObject : public CPolygonObject
{
public:
	CStarObject() {}
	CStarObject(int nLeft, int nTop, int nRight, int nBottom);
	virtual ~CStarObject() { }
};

class CHexagonObject : public CPolygonObject
{
public:
	CHexagonObject() {}
	CHexagonObject(int nLeft, int nTop, int nRight, int nBottom);
	virtual ~CHexagonObject() { }
};
