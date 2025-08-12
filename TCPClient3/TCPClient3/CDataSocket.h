// [Lab08] TCP 기반 양방향 통신 21920141_이민하

#pragma once
#include <afxsock.h>

// CDataSocket 명령 대상

class CTCPClient3Dlg;

class CDataSocket : public CSocket
{
public:
	CDataSocket(CTCPClient3Dlg *pDlg);
	virtual ~CDataSocket();
	CTCPClient3Dlg* m_pDlg;
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


