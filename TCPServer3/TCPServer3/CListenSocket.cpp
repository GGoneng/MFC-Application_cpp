// CListenSocket.cpp: 구현 파일
// [Lab08] TCP 기반 양방향 통신 21920141_이민하


#include "pch.h"
#include "TCPServer3.h"
#include "CListenSocket.h"
#include "TCPServer3Dlg.h"

// CListenSocket

CListenSocket::CListenSocket(CTCPServer3Dlg *pDlg)
{
	// 멤버 변수 생성
	m_pDlg = pDlg;
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket 멤버 함수

void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CSocket::OnAccept(nErrorCode);
	m_pDlg->ProcessAccept(nErrorCode);
}
