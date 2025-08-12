
// TCPClient3Dlg.h: 헤더 파일
// [Lab08] TCP 기반 양방향 통신 21920141_이민하


#pragma once
#include "afxcoll.h"
#include "afxwin.h"

// 스레드에서 사용할 구조체 정의
struct ThreadArg
{
	CStringList* pList;
	CDialogEx* pDlg;
	int Thread_run;
};
// 데이터 소켓 정의
class CDataSocket;

// CTCPClient3Dlg 대화 상자
class CTCPClient3Dlg : public CDialogEx
{
// 생성입니다.
public:
	CTCPClient3Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPCLIENT3_DIALOG };
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
	CDataSocket* m_pDataSocket; // 데이터 소켓 주소
	CIPAddressCtrl m_ipaddr; // ip 인자
	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	CButton m_connect;
	CButton m_disconnect;
	CButton m_send;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	void ProcessReceive(CDataSocket* pSocket, int nErrorCode);
	void ProcessClose(CDataSocket* pSocket, int nErrorCode);
};
