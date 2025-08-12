// CDataSocket.cpp: 구현 파일
// [Lab10] UDP 기반 양방향 통신 21920141_이민하

#include "pch.h"
#include "UDPServer.h"
#include "CDataSocket.h"
#include "UDPServerDlg.h"


// CDataSocket

CDataSocket::CDataSocket(CUDPServerDlg *pDlg)
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
