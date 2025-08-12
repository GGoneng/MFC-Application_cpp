// CDataSocket.cpp: 구현 파일
//

#include "pch.h"
#include "UDPServer2.h"
#include "CDataSocket.h"
#include "UDPServer2Dlg.h"


// CDataSocket

CDataSocket::CDataSocket(CUDPServer2Dlg *pDlg)
{
	// 멤버 변수 생성
	m_pDlg = pDlg;
}

CDataSocket::~CDataSocket()
{
}


// CDataSocket 멤버 함수

void CDataSocket::OnReceive(int nErrorCode)
{
	// OnReceive 가상 함수 기능 정의
	CSocket::OnReceive(nErrorCode);
	m_pDlg->ProcessReceive(this, nErrorCode);
}

void CDataSocket::OnClose(int nErrorCode)
{
	// OnClose 가상 함수 기능 정의
	CSocket::OnClose(nErrorCode);
	m_pDlg->ProcessClose(this, nErrorCode);
}
