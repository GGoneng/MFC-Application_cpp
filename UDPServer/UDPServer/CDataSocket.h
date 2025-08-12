#pragma once

// CDataSocket 명령 대상
// [Lab10] UDP 기반 양방향 통신 21920141_이민하

class CUDPServerDlg;

class CDataSocket : public CSocket
{
public:
	CDataSocket(CUDPServerDlg *pDlg);
	virtual ~CDataSocket();
	CUDPServerDlg* m_pDlg;
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


