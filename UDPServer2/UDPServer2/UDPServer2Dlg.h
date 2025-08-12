
// UDPServer2Dlg.h: 헤더 파일
// [Lab11] ARQ on UDP 21920141_이민하

#pragma once

// 스레드에서 사용할 구조체 정의
struct ThreadArg
{
	CStringList* pList;
	CDialogEx* pDlg;
	int Thread_run;
};

// 프레임 구현을 위한 구조체 정의
struct Frame
{
	int seq_num;
	int ack_num;
	int checksum;
	TCHAR p_buffer[256];
	int is_ack;
	Frame() {}
};


// 데이터 소켓 정의
class CDataSocket;

// CUDPServer2Dlg 대화 상자
class CUDPServer2Dlg : public CDialogEx
{
// 생성입니다.
public:
	CUDPServer2Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UDPSERVER2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CWinThread* pThread1, * pThread2; // 스레드 객체 주소
	ThreadArg arg1, arg2; // 스레드 전달 인자
	CDataSocket* m_pDataSocket; // DataSocket 주소
	CString clientAddress; // 클라이언트 주소
	UINT clientPort; // 클라이언트 포트
	int prev_ack_num;
	Frame LastFrame;
	CButton m_send;
	CButton m_close;
	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void ProcessReceive(CDataSocket* pSocket, int nErrorCode);
	void ProcessClose(CDataSocket* pSocket, int nErrorCode);
};
