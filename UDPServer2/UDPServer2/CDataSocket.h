#pragma once

// CDataSocket 명령 대상
// [Lab11] ARQ on UDP 21920141_이민하

class CUDPServer2Dlg;

class CDataSocket : public CSocket
{
public:
	CDataSocket(CUDPServer2Dlg *pDlg);
	virtual ~CDataSocket();
	CUDPServer2Dlg* m_pDlg;
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


