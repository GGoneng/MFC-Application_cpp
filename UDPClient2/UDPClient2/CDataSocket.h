#pragma once

// CDataSocket 명령 대상
// [Lab11] ARQ on UDP 21920141_이민하

class CUDPClient2Dlg;

class CDataSocket : public CSocket
{
public:
	CDataSocket(CUDPClient2Dlg *pDlg);
	virtual ~CDataSocket();
	CUDPClient2Dlg* m_pDlg;
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


