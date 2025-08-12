
// TCPServer3Dlg.cpp: 구현 파일
// [Lab08] TCP 기반 양방향 통신 21920141_이민하

//#include "stdafx.h"
#include "pch.h"
#include "framework.h"
#include "TCPServer3.h"
#include "TCPServer3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CListenSocket.h"
#include "CDataSocket.h"

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

CCriticalSection tx_cs;
CCriticalSection rx_cs;


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTCPServer3Dlg 대화 상자



CTCPServer3Dlg::CTCPServer3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPSERVER3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPServer3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, m_edit1);
	DDX_Control(pDX, IDC_EDIT4, m_edit2);
	DDX_Control(pDX, IDC_EDIT2, m_edit3);
}

BEGIN_MESSAGE_MAP(CTCPServer3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTCPServer3Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTCPServer3Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()

UINT RXThread(LPVOID arg)
{
	ThreadArg* pArg = (ThreadArg*)arg;
	CStringList* plist = pArg->pList;
	CTCPServer3Dlg* pDlg = (CTCPServer3Dlg*)pArg->pDlg;
	while (pArg->Thread_run) {
		POSITION pos = plist->GetHeadPosition();
		POSITION current_pos;
		while (pos != NULL) {
			current_pos = pos;
			//	rx_event.Lock();
			rx_cs.Lock();
			CString str = plist->GetNext(pos);
			rx_cs.Unlock();

			int len = pDlg->m_edit3.GetWindowTextLengthW();
			pDlg->m_edit3.SetSel(len, len);
			pDlg->m_edit3.ReplaceSel(str);

			plist->RemoveAt(current_pos);
		}
		Sleep(10);
	}
	return 0;
}

UINT TXThread(LPVOID arg)
{
	ThreadArg* pArg = (ThreadArg*)arg;
	CStringList* plist = pArg->pList;
	CTCPServer3Dlg* pDlg = (CTCPServer3Dlg*)pArg->pDlg;
	BOOL isEmpty = plist->IsEmpty();

	while (pArg->Thread_run) {
		POSITION pos = plist->GetHeadPosition();
		POSITION current_pos;
		while (pos != NULL) {
			current_pos = pos;
			tx_cs.Lock();
			CString str = plist->GetNext(pos);
			tx_cs.Unlock();
			pDlg->m_pDataSocket->Send((LPCTSTR)str, (str.GetLength() + 1) * sizeof(TCHAR));

			plist->RemoveAt(current_pos);
		}
		Sleep(10);
	}
	return 0;
}


// CTCPServer3Dlg 메시지 처리기

BOOL CTCPServer3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	// 송신 구조체 인자 초기화
	CStringList* newlist = new CStringList;
	arg1.pList = newlist;
	arg1.Thread_run = 1;
	arg1.pDlg = this;

	// 수신 구조체 인자 초기화
	CStringList* newlist2 = new CStringList;
	arg2.pList = newlist2;
	arg2.Thread_run = 1;
	arg2.pDlg = this;

	// 윈도우 소켓 초기화
	WSADATA wsa;
	int error_code;
	if ((error_code = WSAStartup(MAKEWORD(2, 2), &wsa)) != 0) {
		TCHAR buffer[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 256, NULL);
		AfxMessageBox(buffer, MB_ICONERROR);
	}

	// DataSocket, ListenSocket 초기화
	m_pDataSocket = NULL;
	m_pListenSocket = NULL;

	// 서버 생성과 동시에 송, 수신 스레드 작동
	ASSERT(m_pListenSocket == NULL);
	m_pListenSocket = new CListenSocket(this);
	if (m_pListenSocket->Create(8000)) {
		if (m_pListenSocket->Listen()) {
			AfxMessageBox(_T("서버를 시작합니다."), MB_ICONINFORMATION);
			pThread1 = AfxBeginThread(TXThread, (LPVOID)&arg1);
			pThread2 = AfxBeginThread(RXThread, (LPVOID)&arg2);
			return TRUE;
		}
	}
	// 서버 생성 예외 처리
	else {
		int err = m_pListenSocket->GetLastError();
		TCHAR buffer[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 256, NULL);
		AfxMessageBox(buffer, MB_ICONERROR);
	}

	AfxMessageBox(_T("이미 실행 중인 서버가 있습니다.")
		_T("\n프로그램을 종료합니다."), MB_ICONERROR);
	return FALSE;
	
}

void CTCPServer3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTCPServer3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTCPServer3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CTCPServer3Dlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTCPServer3Dlg::OnBnClickedButton1() // Send Button
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// if (UpdataData(TRUE) == 0)
		// return;
	
	// 송신하려는 메세지 병합
	CString tx_message;
	m_edit1.GetWindowTextW(tx_message);
	tx_message += _T("\r\n");

	// CStringList의 Tail에 송신 메세지 추가
	tx_cs.Lock();
	arg1.pList->AddTail(tx_message);
	tx_cs.Unlock();

	// 메세지 입력 편집 컨트롤 초기화
	m_edit1.SetWindowTextW(_T(""));
	m_edit1.SetFocus();

	// 스크롤 자동 내림
	int len = m_edit1.GetWindowTextLengthW();
	m_edit2.SetSel(len, len);
	m_edit2.ReplaceSel(tx_message);
}

void CTCPServer3Dlg::OnBnClickedButton2() // Close Button
{
	// 연결 해제 실패 예외 처리
	if (m_pDataSocket == NULL) {
		AfxMessageBox(_T("이미 접속 종료"));
	}
	// Close Button 누를 시 서버 연결 해제
	else {
		arg1.Thread_run = 0;
		arg2.Thread_run = 0;
		m_pDataSocket->Close();
		delete m_pDataSocket;
		m_pDataSocket = NULL;
		m_pListenSocket->Close();
		delete m_pListenSocket;
		m_pListenSocket = NULL;
	}
}



void CTCPServer3Dlg::ProcessAccept(int nErrorCode)
{
	CString PeerAddr;
	UINT PeerPort;
	CString str;

	// 통신 연결 요청 승인 및 IP주소, Port번호 출력
	ASSERT(nErrorCode == 0);
	if (m_pDataSocket == NULL) {
		m_pDataSocket = new CDataSocket(this);
		if (m_pListenSocket->Accept(*m_pDataSocket)) {
			m_pDataSocket->GetPeerName(PeerAddr, PeerPort);
			str.Format(_T("### IP 주소: %s, 포트 번호: %d ###\r\n\r\n"),
				PeerAddr, PeerPort);
			m_edit3.SetWindowTextW(str);
		}
		else {
			delete m_pDataSocket;
			m_pDataSocket = NULL;
		}
	}
}

void CTCPServer3Dlg::ProcessReceive(CDataSocket* pSocket, int nErrorCode)
{
	// 수신 인자 초기화
	TCHAR buf[1024 + 1];
	CString strData;
	int nbytes;

	// 수신된 buf 내용 CStringList에 Tail로 추가
	nbytes = pSocket->Receive(buf, 1024);
	buf[nbytes] = NULL;
	strData = (LPCTSTR)buf;
//	AfxMessageBox(strData);

	rx_cs.Lock();
	arg2.pList->AddTail((LPCTSTR)strData);
	rx_cs.Unlock();

//	rx_event.SetEvent();
}

void CTCPServer3Dlg::ProcessClose(CDataSocket* pSocket, int nErrorCode)
{
	// DataSocket 연결 해제 하면서 통신 종료
	pSocket->Close();
	delete m_pDataSocket;
	m_pDataSocket = NULL;

	int len = m_edit3.GetWindowTextLengthW();
	CString message = _T("### 접속 종료 ###\r\n\n");
	m_edit3.SetSel(len, len);
	m_edit3.ReplaceSel(message);
}
