// [Lab08] TCP 기반 양방향 통신 21920141_이민하
#pragma once

// CDataSocket 명령 대상

class CTCPServer3Dlg;

class CDataSocket : public CSocket
{
public:
	CDataSocket(CTCPServer3Dlg *pDlg);
	virtual ~CDataSocket();
	CTCPServer3Dlg* m_pDlg;
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
};


