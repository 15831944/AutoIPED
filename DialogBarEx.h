#if !defined(AFX_DIALOGBAREX_H__A885A54C_9195_48D4_AEA5_71CF5BFB7E24__INCLUDED_)
#define AFX_DIALOGBAREX_H__A885A54C_9195_48D4_AEA5_71CF5BFB7E24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogBarEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// 
// ��CDialogBar������
//
// ������ͣ���͸���ʱ�ı��С
//
////////////////////////////////////////////////////////////////////////////
#include "DialogBarExContainer.h"

class CDialogBarEx : public CDialogBar
{
	friend class CDialogBarExContainer;
// Construction
public:
	CDialogBarEx();
	virtual ~CDialogBarEx();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBarEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetClientRect(LPRECT lpRect);

	bool IsWindowVisible();	// �жϴ����Ƿ�ɼ�
	void HideDlgBar();		// ���ش���
	void ShowDlgBar();		// ��ʾ����

	void GetFloatingSize(CSize &size);// ��ø���ʱ�Ĵ��ڵĳߴ�

	CSize GetWndMinSize();			// ���ش��ڵ���С�ߴ�
	void SetWndMinSize(CSize size);	// ���ô��ڵ���С�ߴ�

	int GetDockingSide();			// ����ͣ����λ��

protected:
	// ���Ʊ�����
	virtual void PaintTitle(CDC *pDC);

	virtual CSize CalcFixedLayout(BOOL bStretch,BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength,DWORD dwMode);


protected:
	CSize		m_InitSize;		//��ʼ���Ĵ��ڵĴ�С
	CSize		m_FloatingSize;	//����ʱ���ڵĴ�С
	CRect		m_TitleRect;	//�������Ĵ�С
	COLORREF	m_TitleBarColor;	// ����������ɫ
	CBrush		m_TitleBarBrush;	// ��������ɫ�Ļ�ˢ


private:
	CPoint	m_CapturePt;	// ��׽ʱ�����λ��
	BOOL	m_IsCapture;	// �Ƿ�������׽
	BOOL	m_bHorz;		// �Ƿ���ˮƽ��

	// �Ƿ��ڿɸı��С������ֻ��ͣ��ʱ����,
	// 0,���ɸı��С��1���ϣ�2�£�3��4����
	int		m_DragSizeArea;	
	CSize	m_MinSize;		// ���ڵ���С�ߴ磨����Ϊ��λ��

	CDialogBarExContainer	*m_pContainWnd;		//���ڰ�������ʱ�Ĵ���

	CRect	m_CloseButtonRect;	//�رմ��ڰ�ť��λ��
	bool	m_IsInCloseButton;	// �Ƿ��ڹرմ�����
	bool	m_IsCursorInWnd;	// ����Ƿ��ڴ�����
	HCURSOR	m_hCursorH;
	HCURSOR	m_hCursorV;
protected:
	//{{AFX_MSG(CDialogBarEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg	LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBAREX_H__A885A54C_9195_48D4_AEA5_71CF5BFB7E24__INCLUDED_)
