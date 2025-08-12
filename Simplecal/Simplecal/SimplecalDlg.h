
// SimplecalDlg.h: 헤더 파일
//

#pragma once


// CSimplecalDlg 대화 상자
class CSimplecalDlg : public CDialogEx
{
// 생성입니다.
public:
	CSimplecalDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMPLECAL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Combo;
	afx_msg void OnCbnSelchangeCombo1();
	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	Double m_edit2;
	Double m_edit1;
	Double m_edit3;
	double m_edit1;
	double m_eidt2;
	double m_edit2;
	double m_edit3;
	CEdit m_edit1;
	CEdit m_edit;
	afx_msg void OnEnChangeEdit4();
	CEdit m_edit2;
	CEdit m_edit3;
	CEdit m_edit1;
	CEdit m_edit3;
	CEdit m_edit2;
};
