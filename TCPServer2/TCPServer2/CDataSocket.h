#pragma once

// CDataSocket 명령 대상

class CTCPServer2Doc;

class CDataSocket : public CSocket
{
public:
	CDataSocket(CTCPServer2Doc *pDoc);
	virtual ~CDataSocket();
	CTCPServer2Doc* m_pDoc;
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


