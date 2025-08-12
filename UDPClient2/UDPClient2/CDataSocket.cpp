// CDataSocket.cpp: 구현 파일
// [Lab11] ARQ on UDP 21920141_이민하

#include "pch.h"
#include "UDPClient2.h"
#include "CDataSocket.h"
#include "UDPClient2Dlg.h"

// CDataSocket

CDataSocket::CDataSocket(CUDPClient2Dlg * pDlg)
{
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
