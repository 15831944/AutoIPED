#if !defined(AFX_EDITOILPAINTDATADLG_H__75996F8B_4AE5_467D_8844_56F429CE879C__INCLUDED_)
#define AFX_EDITOILPAINTDATADLG_H__75996F8B_4AE5_467D_8844_56F429CE879C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditOilPaintDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// 
// �༭����ԭʼ���ݶԻ���
//
//////////////////////////////////////////////////
#include "MyButton.h"
#include "FoxBase.h"

class CEditOilPaintDataDlg : public CDialog,public CFoxBase
{
// Construction
public: 
	CEditOilPaintDataDlg(CWnd* pParent = NULL);   // standard constructor

public:

	//������ѡ���̵����ݿ������
	void SetCurrentProjectConnect(_ConnectionPtr &IConnection);
	//�������ݿ����ӵ�����ָ�������
	_ConnectionPtr& GetCurrentProjectConnect();

	//���ù��������ݿ������
	void SetPublicConnect(_ConnectionPtr& IConnection);
	// ���ع������ݿ����ӵ�����ָ�������
	_ConnectionPtr& GetPublicConnect();

	//������ѡ���̵�ID��
	void SetProjectID(LPCTSTR ProjectID);
	// ������ѡ���̵�ID��
	CString& GetProjectID();
	//���ô���λ��
	BOOL SetWindowCenter(HWND hWnd);

// Dialog Data
	//{{AFX_DATA(CEditOilPaintDataDlg)
	enum { IDD = IDD_EDIT_OILPAINT_DATA };
	CMyButton	m_DeleteAllPaint;
	CMyButton	m_SaveCurrent;
	CMyButton	m_DelCurrent;
	CMyButton	m_AddNew;
	CMyButton	m_MoveLast;
	CMyButton	m_MoveSubsequent;
	CMyButton	m_MovePrevious;
	CMyButton	m_MoveForefront;
	CComboBox	m_OilPaintType;
	CComboBox	m_OilPaintColor;
	CComboBox	m_PipeDeviceName;
	int		m_ID;
	CString	m_VolNo;
	double	m_DeviceOutsizeArea;
	double	m_PipeSize;
	double	m_PipeLengthDevice;
	CString	m_ReMark;
	BOOL	m_IsUpdateByRoll;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditOilPaintDataDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL InitToolTip();
	BOOL WritePassTbl();
	virtual	void OnCancel(); 
	// Generated message map functions
	//{{AFX_MSG(CEditOilPaintDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMoveForefront();
	afx_msg void OnMovePrevious();
	afx_msg void OnMoveSubsequent();
	afx_msg void OnMoveLast();
	afx_msg void OnAddNew();
	afx_msg void OnDelCurrent();
	afx_msg void OnSaveCurrent();
	afx_msg void OnDropdownPipeDeviceName();
	afx_msg void OnExit();
	afx_msg void OnChangeId();
	afx_msg void OnDelAllPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL InitCurrentProjectRecordset();

private:
	CToolTipCtrl m_pctlTip;
	void ProcessInExit();
	BOOL UpdateVolTable();
	BOOL UpdatePipeDeviceNameCombox();
	void UpdateControlsState();
	BOOL PutDataToDatabaseFromControl();
	BOOL PutDataToEveryControl();
	BOOL InitOilPaintTypeCombox();
	BOOL InitOilPaintColorCombox();
	BOOL InitBitmapButton();
	CString m_ProjectID;							//���̵�ID��
	_RecordsetPtr m_ICurrentProjectRecordset;	
	_ConnectionPtr m_IPublicConnection;				//�������ݿ������
	_ConnectionPtr m_ICurrentProjectConnection;		//��ѡ���̵����ݿ������
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITOILPAINTDATADLG_H__75996F8B_4AE5_467D_8844_56F429CE879C__INCLUDED_)
