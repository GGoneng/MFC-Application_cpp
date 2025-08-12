
// ChildView.cpp: CChildView 클래스의 구현
// Lab04 
// 21920141 이민하

#include "pch.h"
#include "framework.h"
#include "CDCLab.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() // 무효 영역을 다시 그려주는 역할
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	// 리스트 HEAD 주소 지정
	POSITION Lpos = Lptlist.GetHeadPosition();

	// 리스트 처음부터 끝까지 반복
	while (Lpos != NULL) 
	{	
		// 리스트 내의 각 요소 좌표를 추출해 사각형 그리기
		CPoint p = Lptlist.GetNext(Lpos);

		dc.Rectangle(p.x - 20, p.y - 20, p.x + 20, p.y + 20);
	}
	

}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// CWnd::OnLButtonDown(nFlags, point);

	CClientDC dc(this);

	// 좌클릭을 했을 때, 리스트에 좌표값을 추가하고 사각형 그리기
	Lptlist.AddTail(point);

	dc.Rectangle(point.x - 20, point.y - 20, point.x + 20, point.y + 20);
	

}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// CWnd::OnRButtonDown(nFlags, point);

	CClientDC dc(this);
	
	// 사각형을 지우기 위해 굵기 2인 흰색 펜 지정
	CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
	dc.SelectObject(&pen);

	// 사각형 그리기
	dc.Rectangle(point.x - 20, point.y - 20, point.x + 20, point.y + 20);
}
