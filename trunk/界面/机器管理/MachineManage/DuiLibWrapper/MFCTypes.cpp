#include "StdAfx.h"
#include "MFCTypes.h"



#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

// CSize
 CSize::CSize() throw()
{ /* random filled */ }
 CSize::CSize(int initCX, int initCY) throw()
{ cx = initCX; cy = initCY; }
 CSize::CSize(SIZE initSize) throw()
{ *(SIZE*)this = initSize; }
 CSize::CSize(POINT initPt) throw()
{ *(POINT*)this = initPt; }
 CSize::CSize(DWORD dwSize) throw()
{
	cx = (short)LOWORD(dwSize);
	cy = (short)HIWORD(dwSize);
}
 BOOL CSize::operator==(SIZE size) const throw()
{ return (cx == size.cx && cy == size.cy); }
 BOOL CSize::operator!=(SIZE size) const throw()
{ return (cx != size.cx || cy != size.cy); }
 void CSize::operator+=(SIZE size) throw()
{ cx += size.cx; cy += size.cy; }
 void CSize::operator-=(SIZE size) throw()
{ cx -= size.cx; cy -= size.cy; }
 void CSize::SetSize(int CX, int CY) throw()
{ cx = CX; cy = CY; }	
 CSize CSize::operator+(SIZE size) const throw()
{ return CSize(cx + size.cx, cy + size.cy); }
 CSize CSize::operator-(SIZE size) const throw()
{ return CSize(cx - size.cx, cy - size.cy); }
 CSize CSize::operator-() const throw()
{ return CSize(-cx, -cy); }
 CPoint CSize::operator+(POINT point) const throw()
{ return CPoint(cx + point.x, cy + point.y); }
 CPoint CSize::operator-(POINT point) const throw()
{ return CPoint(cx - point.x, cy - point.y); }
 CRect CSize::operator+(const RECT* lpRect) const throw()
{ return CRect(lpRect) + *this; }
 CRect CSize::operator-(const RECT* lpRect) const throw()
{ return CRect(lpRect) - *this; }

// CPoint
 CPoint::CPoint() throw()
{ /* random filled */ }
 CPoint::CPoint(int initX, int initY) throw()
{ x = initX; y = initY; }
 CPoint::CPoint(POINT initPt) throw()
{ *(POINT*)this = initPt; }
 CPoint::CPoint(SIZE initSize) throw()
{ *(SIZE*)this = initSize; }
 CPoint::CPoint(LPARAM dwPoint) throw()
{
	x = (short)GET_X_LPARAM(dwPoint);
	y = (short)GET_Y_LPARAM(dwPoint);
}
 void CPoint::Offset(int xOffset, int yOffset) throw()
{ x += xOffset; y += yOffset; }
void CPoint::Offset(POINT point) throw()
{ x += point.x; y += point.y; }
 void CPoint::Offset(SIZE size) throw()
{ x += size.cx; y += size.cy; }
 void CPoint::SetPoint(int X, int Y) throw()
{ x = X; y = Y; }
 BOOL CPoint::operator==(POINT point) const throw()
{ return (x == point.x && y == point.y); }
 BOOL CPoint::operator!=(POINT point) const throw()
{ return (x != point.x || y != point.y); }
 void CPoint::operator+=(SIZE size) throw()
{ x += size.cx; y += size.cy; }
 void CPoint::operator-=(SIZE size) throw()
{ x -= size.cx; y -= size.cy; }
 void CPoint::operator+=(POINT point) throw()
{ x += point.x; y += point.y; }
 void CPoint::operator-=(POINT point) throw()
{ x -= point.x; y -= point.y; }
 CPoint CPoint::operator+(SIZE size) const throw()
{ return CPoint(x + size.cx, y + size.cy); }
 CPoint CPoint::operator-(SIZE size) const throw()
{ return CPoint(x - size.cx, y - size.cy); }
 CPoint CPoint::operator-() const throw()
{ return CPoint(-x, -y); }
 CPoint CPoint::operator+(POINT point) const throw()
{ return CPoint(x + point.x, y + point.y); }
 CSize CPoint::operator-(POINT point) const throw()
{ return CSize(x - point.x, y - point.y); }
 CRect CPoint::operator+(const RECT* lpRect) const throw()
{ return CRect(lpRect) + *this; }
 CRect CPoint::operator-(const RECT* lpRect) const throw()
{ return CRect(lpRect) - *this; }

// CRect
 CRect::CRect() throw()
{ /* random filled */ }
 CRect::CRect(int l, int t, int r, int b) throw()
{ left = l; top = t; right = r; bottom = b; }
 CRect::CRect(const RECT& srcRect) throw()
{ ::CopyRect(this, &srcRect); }
 CRect::CRect(LPCRECT lpSrcRect) throw()
{ ::CopyRect(this, lpSrcRect); }
 CRect::CRect(POINT point, SIZE size) throw()
{ right = (left = point.x) + size.cx; bottom = (top = point.y) + size.cy; }
 CRect::CRect(POINT topLeft, POINT bottomRight) throw()
{ left = topLeft.x; top = topLeft.y;
right = bottomRight.x; bottom = bottomRight.y; }
 int CRect::Width() const throw()
{ return right - left; }
 int CRect::Height() const throw()
{ return bottom - top; }
 CSize CRect::Size() const throw()
{ return CSize(right - left, bottom - top); }
 CPoint& CRect::TopLeft() throw()
{ return *((CPoint*)this); }
 CPoint& CRect::BottomRight() throw()
{ return *((CPoint*)this+1); }
 const CPoint& CRect::TopLeft() const throw()
{ return *((CPoint*)this); }
 const CPoint& CRect::BottomRight() const throw()
{ return *((CPoint*)this+1); }
 CPoint CRect::CenterPoint() const throw()
{ return CPoint((left+right)/2, (top+bottom)/2); }
 void CRect::SwapLeftRight() throw()
{ SwapLeftRight(LPRECT(this)); }
 void WINAPI CRect::SwapLeftRight(LPRECT lpRect) throw()
{ LONG temp = lpRect->left; lpRect->left = lpRect->right; lpRect->right = temp; }
 CRect::operator LPRECT() throw()
{ return this; }
 CRect::operator LPCRECT() const throw()
{ return this; }
 BOOL CRect::IsRectEmpty() const throw()
{ return ::IsRectEmpty(this); }
 BOOL CRect::IsRectNull() const throw()
{ return (left == 0 && right == 0 && top == 0 && bottom == 0); }
 BOOL CRect::PtInRect(POINT point) const throw()
{ return ::PtInRect(this, point); }
 void CRect::SetRect(int x1, int y1, int x2, int y2) throw()
{ ::SetRect(this, x1, y1, x2, y2); }
 void CRect::SetRect(POINT topLeft, POINT bottomRight) throw()
{ ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y); }
 void CRect::SetRectEmpty() throw()
{ ::SetRectEmpty(this); }
 void CRect::CopyRect(LPCRECT lpSrcRect) throw()
{ ::CopyRect(this, lpSrcRect); }
 BOOL CRect::EqualRect(LPCRECT lpRect) const throw()
{ return ::EqualRect(this, lpRect); }
 void CRect::InflateRect(int x, int y) throw()
{ ::InflateRect(this, x, y); }
 void CRect::InflateRect(SIZE size) throw()
{ ::InflateRect(this, size.cx, size.cy); }
 void CRect::DeflateRect(int x, int y) throw()
{ ::InflateRect(this, -x, -y); }
 void CRect::DeflateRect(SIZE size) throw()
{ ::InflateRect(this, -size.cx, -size.cy); }
 void CRect::OffsetRect(int x, int y) throw()
{ ::OffsetRect(this, x, y); }
 void CRect::OffsetRect(POINT point) throw()
{ ::OffsetRect(this, point.x, point.y); }
 void CRect::OffsetRect(SIZE size) throw()
{ ::OffsetRect(this, size.cx, size.cy); }
 void CRect::MoveToY(int y) throw()
{ bottom = Height() + y; top = y; }
 void CRect::MoveToX(int x) throw()
{ right = Width() + x; left = x; }
 void CRect::MoveToXY(int x, int y) throw()
{ MoveToX(x); MoveToY(y); }
 void CRect::MoveToXY(POINT pt) throw()
{ MoveToX(pt.x); MoveToY(pt.y); }
 BOOL CRect::IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2) throw()
{ return ::IntersectRect(this, lpRect1, lpRect2);}
 BOOL CRect::UnionRect(LPCRECT lpRect1, LPCRECT lpRect2) throw()
{ return ::UnionRect(this, lpRect1, lpRect2); }
 void CRect::operator=(const RECT& srcRect) throw()
{ ::CopyRect(this, &srcRect); }
 BOOL CRect::operator==(const RECT& rect) const throw()
{ return ::EqualRect(this, &rect); }
 BOOL CRect::operator!=(const RECT& rect) const throw()
{ return !::EqualRect(this, &rect); }
 void CRect::operator+=(POINT point) throw()
{ ::OffsetRect(this, point.x, point.y); }
 void CRect::operator+=(SIZE size) throw()
{ ::OffsetRect(this, size.cx, size.cy); }
 void CRect::operator+=(LPCRECT lpRect) throw()
{ InflateRect(lpRect); }
 void CRect::operator-=(POINT point) throw()
{ ::OffsetRect(this, -point.x, -point.y); }
 void CRect::operator-=(SIZE size) throw()
{ ::OffsetRect(this, -size.cx, -size.cy); }
 void CRect::operator-=(LPCRECT lpRect) throw()
{ DeflateRect(lpRect); }
 void CRect::operator&=(const RECT& rect) throw()
{ ::IntersectRect(this, this, &rect); }
 void CRect::operator|=(const RECT& rect) throw()
{ ::UnionRect(this, this, &rect); }
 CRect CRect::operator+(POINT pt) const throw()
{ CRect rect(*this); ::OffsetRect(&rect, pt.x, pt.y); return rect; }
 CRect CRect::operator-(POINT pt) const throw()
{ CRect rect(*this); ::OffsetRect(&rect, -pt.x, -pt.y); return rect; }
 CRect CRect::operator+(SIZE size) const throw()
{ CRect rect(*this); ::OffsetRect(&rect, size.cx, size.cy); return rect; }
 CRect CRect::operator-(SIZE size) const throw()
{ CRect rect(*this); ::OffsetRect(&rect, -size.cx, -size.cy); return rect; }
 CRect CRect::operator+(LPCRECT lpRect) const throw()
{ CRect rect(this); rect.InflateRect(lpRect); return rect; }
 CRect CRect::operator-(LPCRECT lpRect) const throw()
{ CRect rect(this); rect.DeflateRect(lpRect); return rect; }
 CRect CRect::operator&(const RECT& rect2) const throw()
{ CRect rect; ::IntersectRect(&rect, this, &rect2);
return rect; }
 CRect CRect::operator|(const RECT& rect2) const throw()
{ CRect rect; ::UnionRect(&rect, this, &rect2);
return rect; }
 BOOL CRect::SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2) throw()
{ return ::SubtractRect(this, lpRectSrc1, lpRectSrc2); }

 void CRect::NormalizeRect() throw()
{
	int nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

 void CRect::InflateRect(LPCRECT lpRect) throw()
{
	left -= lpRect->left;		top -= lpRect->top;
	right += lpRect->right;		bottom += lpRect->bottom;
}

 void CRect::InflateRect(int l, int t, int r, int b) throw()
{
	left -= l;			top -= t;
	right += r;			bottom += b;
}

 void CRect::DeflateRect(LPCRECT lpRect) throw()
{
	left += lpRect->left;	top += lpRect->top;
	right -= lpRect->right;	bottom -= lpRect->bottom;
}

 void CRect::DeflateRect(int l, int t, int r, int b) throw()
{
	left += l;		top += t;
	right -= r;		bottom -= b;
}

 CRect CRect::MulDiv(int nMultiplier, int nDivisor) const throw()
{
	return CRect(
		::MulDiv(left, nMultiplier, nDivisor),
		::MulDiv(top, nMultiplier, nDivisor),
		::MulDiv(right, nMultiplier, nDivisor),
		::MulDiv(bottom, nMultiplier, nDivisor));
}

