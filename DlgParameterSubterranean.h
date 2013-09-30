#if !defined(AFX_DLGPARAMETERSUBTERRANEAN_H__91E34FAA_AB90_4032_8926_12260348C5E3__INCLUDED_)
#define AFX_DLGPARAMETERSUBTERRANEAN_H__91E34FAA_AB90_4032_8926_12260348C5E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgParameterSubterranean.h : header file
//
#include "XDialog.h" 

/////////////////////////////////////////////////////////////////////////////
// CDlgParameterSubterranean dialog

class CDlgParameterSubterranean : public CXDialog 
{
// Construction 
public:  
	void PutCurrentNO(long nCurNO);
	short PutDataToSubterraneanDB(long nID=-1);	// ���Ի����е�����д�뵽���ݿ���
	void SetProjectID(LPCTSTR lpstrProjectID);	// ���õ�ǰ�Ĺ���ID��
	short GetDataToSubterraneanControl(long nID);		// �����ݿ��ж�ȡԭʼ���� 
	void SetCurrentProjectConnect(_ConnectionPtr pCon);	//  ���õ�ǰ����ԭʼ������ݿ�����

	void SetCurrentRes(_RecordsetPtr pRsCur);			// �������ݼ�¼��

	CDlgParameterSubterranean(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgParameterSubterranean)
	enum { IDD = IDD_PARAMETER_SUBTERRANEAN };
	CComboBox	m_ctlPipeCount;
	CComboBox	m_comboPipeLay;
	double	m_dPipeSubDepth;
	double	m_dEdaphicTemp;
	double	m_dPipeSpan;
	double	m_dEdaphicLamda;
	int		m_nPipeLay;
	double	m_dSupportK;
	double	m_dPipeDag;
	double	m_dCanalDepth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgParameterSubterranean)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgParameterSubterranean)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLay();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSelchangeComboPipeCount();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CToolTipCtrl m_pctlTip;
private:
	void UpdateControlState();
	void InitPipeLayList();
	void InitToolTip();	// ��ʼ��������ʾ��Ϣ

	long	m_nID;		// ��ǰ��¼�����
	CString m_strProjectID;
	_RecordsetPtr m_pRsSubterranean;
	_ConnectionPtr m_pConSubter;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPARAMETERSUBTERRANEAN_H__91E34FAA_AB90_4032_8926_12260348C5E3__INCLUDED_)
