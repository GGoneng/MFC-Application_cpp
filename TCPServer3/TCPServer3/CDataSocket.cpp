// CDataSocket.cpp: 구현 파일
// [Lab08] TCP 기반 양방향 통신 21920141_이민하


#include "pch.h"
#include "TCPServer3.h"
#include "CDataSocket.h"
#include "TCPServer3Dlg.h"


// CDataSocket

CDataSocket::CDataSocket(CTCPServer3Dlg *pDlg)
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CSocket::OnReceive(nErrorCode);
	m_pDlg->ProcessReceive(this, nErrorCode);
}

void CDataSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CSocket::OnClose(nErrorCode);
	m_pDlg->ProcessClose(this, nErrorCode);
}

void CDataSocket::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CSocket::OnAccept(nErrorCode);
}
