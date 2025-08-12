
// [Lab05] 편집 컨트롤 기반 텍스트 출력 21920141_이민하

// EditCtrlView.cpp: CEditCtrlView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "EditCtrl.h"
#endif

#include "EditCtrlDoc.h"
#include "EditCtrlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEditCtrlView

IMPLEMENT_DYNCREATE(CEditCtrlView, CFormView)

BEGIN_MESSAGE_MAP(CEditCtrlView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BUTTON1, &CEditCtrlView::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CEditCtrlView::OnEnChangeEdit1)
	ON_EN_MAXTEXT(IDC_EDIT1, &CEditCtrlView::OnEnMaxtextEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CEditCtrlView::OnEnChangeEdit2)
END_MESSAGE_MAP()

// CEditCtrlView 생성/소멸

CEditCtrlView::CEditCtrlView() noexcept
	: CFormView(IDD_EDITCTRL_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CEditCtrlView::~CEditCtrlView()
{
}

void CEditCtrlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_BUTTON1, m_enter);
}

BOOL CEditCtrlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CEditCtrlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	// 윗쪽 편집 컨트롤에 글자 수 제한
	m_edit1.SetLimitText(30);
}


// CEditCtrlView 인쇄

BOOL CEditCtrlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEditCtrlView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEditCtrlView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CEditCtrlView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CEditCtrlView 진단

#ifdef _DEBUG
void CEditCtrlView::AssertValid() const
{
	CFormView::AssertValid();
}

void CEditCtrlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEditCtrlDoc* CEditCtrlView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEditCtrlDoc)));
	return (CEditCtrlDoc*)m_pDocument;
}
#endif //_DEBUG
	

// CEditCtrlView 메시지 처리기

// Enter 버튼 클릭 시 동작
void CEditCtrlView::OnBnClickedButton1()
{	// 입력된 텍스트에 엔터 공백 추가
	outputstr += (str + _T("\r\n"));
	
	// 아래쪽 편집 컨트롤에 텍스트 표시
	m_edit2.SetWindowText(outputstr);

	// 윗쪽 편집 컨트롤 초기화
	m_edit1.SetWindowText(_T(""));

	// 아래쪽 편집 컨트롤 스크롤을 항상 마지막으로 이동
	m_edit2.SetSel(-1);
}

void CEditCtrlView::OnEnChangeEdit1()
{	// 윗쪽 편집 컨트롤에 입력되는 텍스트 저장
	m_edit1.GetWindowText(str);
}

void CEditCtrlView::OnEnMaxtextEdit1()
{	// 오류 메세지 발생
	MessageBox(_T("최대 길이 도달!"), _T("오류"), MB_ICONERROR);
}

void CEditCtrlView::OnEnChangeEdit2()
{

}
