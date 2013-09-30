#if !defined(AFX_DLGSHOWALLTABLE_H__7675EB0E_DE59_4B0F_A54F_FD32C258ECF5__INCLUDED_)
#define AFX_DLGSHOWALLTABLE_H__7675EB0E_DE59_4B0F_A54F_FD32C258ECF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShowAllTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgShowAllTable dialog
#include "DataShowDlg.h"
#include "mybutton.h"
class CDlgShowAllTable : public CDataShowDlg
{
// Construction
public:
	bool UpdateTblNameList();	//��ʼ����ʾ�ı�
	BOOL UpdateControlsPos();  // ���¸����ؼ���λ��

	CDlgShowAllTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgShowAllTable)
//	enum { IDD = IDD_REPORT_CAL_ERROR_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShowAllTable)
	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strDescTblName;	//��ǰѡ������������
	void UpdateDataGrid();		// ������ѡ������ݡ�
	CListBox m_TableNameList;
	CMyButton	 m_ctlDeleteAll;	//ɾ��ѡ��ı��е�ȫ����¼.
	// Generated message map functions
	//{{AFX_MSG(CDlgShowAllTable)
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnTableSelChange(); // ��Ӧ��ͬ�ı��ѡ��ı�
	afx_msg void OnDeleteTblAll();	//ɾ�����е����м�¼

	DECLARE_MESSAGE_MAP()
private:
	BOOL tableExists(_ConnectionPtr pCon, CString tbn);
	long m_nCurTblID;
	CSize			m_ListSize;	//���������б�ĳߴ�

	CRect	m_VDragArea;	// �����϶�������
	CRect	m_HDragArea;	// �����϶�������
	bool	m_IsInVDragArea;	// �Ƿ��������϶�������
	bool	m_IsInHDragArea;	// �Ƿ��ں����϶�������
	CPoint	m_LButtonPt;		// ���ڼ�¼�����λ��

	_RecordsetPtr m_pRsTableList;  //���ڼ�¼��Ķ�Ӧ��ϵ.
//	_RecordsetPtr m_pRsBrowseResult;	//��Ӧ�������ļ�¼��.
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHOWALLTABLE_H__7675EB0E_DE59_4B0F_A54F_FD32C258ECF5__INCLUDED_)
