
// UDPServerDlg.cpp: 구현 파일
// [Lab10] UDP 기반 양방향 통신 21920141_이민하

#include "pch.h"
#include "framework.h"
#include "UDPServer.h"
#include "UDPServerDlg.h"
#include "afxdialogex.h"
#include "afxsock.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

# include "CDataSocket.h"

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


// CUDPServerDlg 대화 상자



CUDPServerDlg::CUDPServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UDPSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_BUTTON1, m_send);
	DDX_Control(pDX, IDC_BUTTON2, m_close);
}

BEGIN_MESSAGE_MAP(CUDPServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CUDPServerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CUDPServerDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

// 수신 스레드 생성
UINT RXThread(LPVOID arg)
{
	ThreadArg* pArg = (ThreadArg*)arg;
	CStringList* plist = pArg->pList;
	CUDPServerDlg* pDlg = (CUDPServerDlg*)pArg->pDlg;
	// RX buffer의 내용을 화면에 출력
	while (pArg->Thread_run) {
		POSITION pos = plist->GetHeadPosition();
		POSITION current_pos;
		while (pos != NULL) {
			current_pos = pos;
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

// 송신 스레드 생성
UINT TXThread(LPVOID arg)
{
	ThreadArg* pArg = (ThreadArg*)arg;
	CStringList* plist = pArg->pList;
	CUDPServerDlg* pDlg = (CUDPServerDlg*)pArg->pDlg;
	BOOL isEmpty = plist->IsEmpty();

	// 입력을 TX buffer를 통해 송신
	while (pArg->Thread_run) {
		POSITION pos = plist->GetHeadPosition();
		POSITION current_pos;
		while (pos != NULL) {
			current_pos = pos;
			tx_cs.Lock();
			CString str = plist->GetNext(pos);
			CString addr = pDlg->clientAddress;
			UINT port = pDlg->clientPort;
			tx_cs.Unlock();

			pDlg->m_pDataSocket->SendToEx((LPCTSTR)str, (str.GetLength() + 1) * sizeof(TCHAR), port, addr);
			
			plist->RemoveAt(current_pos);
		}
		Sleep(10);
	}
	return 0;
}

// CUDPServerDlg 메시지 처리기

BOOL CUDPServerDlg::OnInitDialog()
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

	// 서버 소켓 생성
	m_pDataSocket = NULL;

	ASSERT(m_pDataSocket == NULL);
	m_pDataSocket = new CDataSocket(this);
	if (m_pDataSocket->Create(8000, SOCK_DGRAM)) {
		AfxMessageBox(_T("서버를 시작합니다."), MB_ICONINFORMATION);
		// 멀티스레드 시작
		pThread1 = AfxBeginThread(TXThread, (LPVOID)&arg1);
		pThread2 = AfxBeginThread(RXThread, (LPVOID)&arg2);
		return TRUE;
	}
	// 생성 실패 시 예외 처리
	else {
		int err = m_pDataSocket->GetLastError();
		TCHAR buffer[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 256, NULL);
		AfxMessageBox(buffer, MB_ICONERROR);
	}
	return FALSE; 
}

void CUDPServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUDPServerDlg::OnPaint()
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
HCURSOR CUDPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUDPServerDlg::OnBnClickedButton1() // Send Button
{
	// 송신하려는 메시지 병합
	CString tx_message;
	m_edit1.GetWindowTextW(tx_message);
	tx_message += _T("\r\n");

	// CStringList의 Tail에 송신 메시지 추가
	tx_cs.Lock();
	arg1.pList->AddTail(tx_message);
	tx_cs.Unlock();

	// 메시지 입력 편집 컨트롤 초기화
	m_edit1.SetWindowTextW(_T(""));
	m_edit1.SetFocus();

	// 스크롤 자동 내림
	int len = m_edit1.GetWindowTextLengthW();
	m_edit2.SetSel(len, len);
	m_edit2.ReplaceSel(tx_message);
}

void CUDPServerDlg::OnBnClickedButton2() // Close Button
{
	// thread 종료 알림 및 소켓 초기화
	arg1.Thread_run = 0;
	arg2.Thread_run = 0;

	m_pDataSocket->Close();
	delete m_pDataSocket;
	m_pDataSocket = NULL;

	EndDialog(IDOK);
}

void CUDPServerDlg::ProcessReceive(CDataSocket* pSocket, int nErrorCode)
{
	// 수신 인자 초기화
	TCHAR buf[1024 + 1];
	CString strData;
	int nbytes;
	CString addr;
	UINT port;

	// 수신된 buf 내용 CStringList에 Tail로 추가
	nbytes = pSocket->ReceiveFromEx(buf, 1024, addr, port);
	buf[nbytes] = NULL;
	strData = (LPCTSTR)buf;

	rx_cs.Lock();
	arg2.pList->AddTail((LPCTSTR)strData);
	clientAddress = addr;
	clientPort = port;
	rx_cs.Unlock();
}

void CUDPServerDlg::ProcessClose(CDataSocket* pSocket, int nErrorCode)
{
	// 소켓 초기화
	pSocket->Close();
	delete m_pDataSocket;
	m_pDataSocket = NULL;

	int len = m_edit3.GetWindowTextLengthW();
	CString message = _T("### 접속 종료 ###\r\n");
	m_edit3.SetSel(len, len);
	m_edit3.ReplaceSel(message);
}
