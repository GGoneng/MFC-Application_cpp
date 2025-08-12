#pragma once
// [Lab08] TCP 기반 양방향 통신 21920141_이민하


// CListenSocket 명령 대상
class CTCPServer3Dlg;

class CListenSocket : public CSocket
{
public:
	CListenSocket(CTCPServer3Dlg *pDlg);
	virtual ~CListenSocket();
	CTCPServer3Dlg* m_pDlg;
	virtual void OnAccept(int nErrorCode);
};
