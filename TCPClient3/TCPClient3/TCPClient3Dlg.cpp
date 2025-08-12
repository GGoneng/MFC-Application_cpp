
// TCPClient3Dlg.cpp: 구현 파일
// [Lab08] TCP 기반 양방향 통신 21920141_이민하

#include "pch.h"
#include "framework.h"
#include "TCPClient3.h"
#include "TCPClient3Dlg.h"
#include "CDataSocket.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CTCPClient3Dlg 대화 상자



CTCPClient3Dlg::CTCPClient3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPCLIENT3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPClient3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipaddr);
	DDX_Control(pDX, IDC_BUTTON1, m_connect);
	DDX_Control(pDX, IDC_BUTTON2, m_disconnect);
	DDX_Control(pDX, IDC_BUTTON3, m_send);
}

BEGIN_MESSAGE_MAP(CTCPClient3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTCPClient3Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTCPClient3Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTCPClient3Dlg::OnBnClickedButton3)
END_MESSAGE_MAP()

UINT RXThread(LPVOID arg)
{
	ThreadArg* pArg = (ThreadArg*)arg;
	CStringList* plist = pArg->pList;
	CTCPClient3Dlg* pDlg = (CTCPClient3Dlg*)pArg->pDlg;
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

UINT TXThread(LPVOID arg)
{
	ThreadArg* pArg = (ThreadArg*)arg;
	CStringList* plist = pArg->pList;
	CTCPClient3Dlg* pDlg = (CTCPClient3Dlg*)pArg->pDlg;
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

int CalculateChecksum(const unsigned char* data, int len)
{
	unsigned int sum = 0;

	// 2바이트 단위로 더하기
	for (int i = 0; i < len; i += 2)
	{
		unsigned short word = data[i];

		if (i + 1 < len)
			word |= (data[i + 1] << 8);

		sum += word;

		if (sum > 0xFFFF)
			sum = (sum & 0xFFFF) + 1;
	}

	unsigned short checksum = ~sum & 0xFFFF;
	return checksum;
}




// CTCPClient3Dlg 메시지 처리기

BOOL CTCPClient3Dlg::OnInitDialog()
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

	// DataSocket, IP 초기화
	m_pDataSocket = NULL;
	m_ipaddr.SetWindowTextW(_T("127.0.0.1"));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTCPClient3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCPClient3Dlg::OnPaint()
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
HCURSOR CTCPClient3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTCPClient3Dlg::OnBnClickedButton1() // Connect Button
{	
	// Connect Button 클릭 시 서버 접속
	if (m_pDataSocket == NULL) {
		m_pDataSocket = new CDataSocket(this);
		m_pDataSocket->Create();
		CString addr;
		m_ipaddr.GetWindowText(addr);
		if (m_pDataSocket->Connect(addr, 8000)) {
			m_edit2.SetWindowText(_T("### 서버에 접속 성공! ###\r\n\r\n"));
			m_edit1.SetFocus();
			// 서버 접속과 동시에 송, 수신 스레드 작동
			pThread1 = AfxBeginThread(TXThread, (LPVOID)&arg1);
			pThread2 = AfxBeginThread(RXThread, (LPVOID)&arg2);
		}
		// 서버 접속 실패 예외 처리
		else {
			MessageBox(_T("서버에 접속 실패!"), _T("알림"), MB_ICONERROR);
			delete m_pDataSocket;
			m_pDataSocket = NULL;
		}
	}
	// 서버 중복 접속 예외 처리
	else {
		MessageBox(_T("서버에 이미 접속됨!"), _T("알림"), MB_ICONINFORMATION);
		m_edit1.SetFocus();
	}
}

void CTCPClient3Dlg::OnBnClickedButton2() // Disconnect Button
{	
	// 연결 해제 실패 예외 처리
	if (m_pDataSocket == NULL) {
		AfxMessageBox(_T("이미 접속 종료"));
	}
	
	// Disconnect Button을 누를 시 서버 연결 해제
	else {
		arg1.Thread_run = 0;
		arg2.Thread_run = 0;
		m_pDataSocket->Close();
		m_edit2.SetWindowText(_T("### 접속 종료 ###\r\n\r\n"));
		delete m_pDataSocket;
		m_pDataSocket = NULL;
	}
}

void CTCPClient3Dlg::OnBnClickedButton3() // Send Button
{
	// 송신하려는 메시지 병합
	CString tx_message;
	m_edit1.GetWindowTextW(tx_message);
	tx_message += _T("\r\n");

	// CStringList Tail에 송신 메세지 추가
	tx_cs.Lock();
	arg1.pList->AddTail(tx_message);
	tx_cs.Unlock();

	// 메시지 입력 편집 컨트롤 초기화
	m_edit1.SetWindowTextW(_T(""));
	m_edit1.SetFocus();

	// 스크롤 자동 내림
	int len = m_edit2.GetWindowTextLengthW();
	m_edit2.SetSel(len, len);
	m_edit2.ReplaceSel(tx_message);
}


void CTCPClient3Dlg::ProcessReceive(CDataSocket* pSocket, int nErrorCode)
{
	// 수신 인자 초기화
	TCHAR buf[1024 + 1];
	CString strData;
	int nbytes;


	// 수신된 buf 내용 CStringList에 Tail로 추가
	nbytes = pSocket->Receive(buf, 1024);
	buf[nbytes] = NULL;
	strData = (LPCTSTR)buf;

	const unsigned char* data = reinterpret_cast<const unsigned char*>(buf);
	int checksum = CalculateChecksum(data, nbytes);
	
	CString msg;
	msg.Format(_T("Checksum: 0x%04X\n"), checksum);
	int len = m_edit3.GetWindowTextLengthW();
	m_edit3.SetSel(len, len);
	m_edit3.ReplaceSel(msg);

	rx_cs.Lock();
	arg2.pList->AddTail((LPCTSTR)strData);
	rx_cs.Unlock();

}


void CTCPClient3Dlg::ProcessClose(CDataSocket* pSocket, int nErrorCode)
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

