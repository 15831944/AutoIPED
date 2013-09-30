#if !defined(AFX_IMPORTFROMXLSDLG_H__B50DF418_09BE_4479_9AD3_1EB20C186C24__INCLUDED_)
#define AFX_IMPORTFROMXLSDLG_H__B50DF418_09BE_4479_9AD3_1EB20C186C24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportFromXLSDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// 
// ���ⲿ����ԭʼ���ݶԻ���Ļ���
//
// ���ôζԻ���ǰ��Ҫ����SetProjectID��SetProjectDbConnect
// ���ù��̵ı��������
//
/////////////////////////////////////////////////////////////////////////
class _Application;

#include "MyButton.h"
#include "PropertyWnd.h"	// ���Դ�����
#include "excel9.h"
#include "edibgbl.h"


class CImportFromXLSDlg : public CDialog
{
// Construction
public:
	CImportFromXLSDlg(CWnd* pParent = NULL);   // standard constructor
	~CImportFromXLSDlg();

public:
	// �����ݿ���ȡ��Ĭ�ϵ�Excel�к�
	void SetDefaultExcelNO(LPCTSTR lpItemTblName);
	
	void SetDataTblName(LPCTSTR lpDataTblName);
	// �����ӿؼ��ı���� �����ӿؼ��ı���
	void SetItemTblName(LPCTSTR lpTitleName, LPCTSTR lpTblName);
	_ConnectionPtr GetIPEDSortDBConnect();
	void SetIPEDSortDBConnect(_ConnectionPtr pConSort);
	CPropertyWnd* GetPropertyWnd();
	CString GetHintInformation();
	void SetHintInformation(LPCTSTR szHint);

	CString GetRegSubKey();
	void SetRegSubKey(LPCTSTR szRegSubkey);

	_ConnectionPtr GetProjectDbConnect();
	void SetProjectDbConnect(_ConnectionPtr IConnection);

	CString GetProjectID();
	void SetProjectID(LPCTSTR szProjectID);


// Dialog Data
	//{{AFX_DATA(CImportFromXLSDlg)
	enum { IDD = IDD_IMPORT_FROM_XLS };
	CMyButton	m_OpenFileDlgButton;
	CString	m_XLSFilePath;
	CString	m_HintInformation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportFromXLSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// ����Ϣ���浽ע�����
	virtual void SaveSetToReg();
	// ��ע��������ݳ�ʼ��
	virtual void InitFromReg();
	
	// ��ʼ�����Դ���
	virtual BOOL InitPropertyWnd();

	// �ڵ���ʱ��֤�������ݵ���Ч��
	virtual BOOL ValidateData();

	// ִ�е������ݵĲ���
	virtual void BeginImport();
		
	// ����ǰ��Ӧ��Excel�к�,д�뵽���ݿ���
	BOOL WriteExcelNoToTbl(const CString strTblName);

	// �����ݱ���ȡ������������̬���ӵ�������
	BOOL AddPropertyWndFromTbl(LPCTSTR lpTitleName, const CString strTblName);

protected:
	CPropertyWnd m_PropertyWnd;		// ���Դ��ڿؼ�

private:
	// ��IPEDsort.mdb��PD2IPED����ȡ��EXCEL�е��ֶ�����ACCESS�е��ֶεĶ�Ӧֵ
	int GetField2Caption(EDIBgbl::CAPTION2FIELD *&pFieldStruct, const CString strTblName, const CString strDefField = "ExcelColNO");

	CString m_strProjectID;				// ���̵ı��
	_ConnectionPtr m_ProjectDbConnect;	// ������ص����ݿ�
	_ConnectionPtr m_pConIpedSort;		// ���Կ������ݿ�

	CString	m_RegSubKey;	// ���ڴ����Ϣ���Ӽ�����
	CString m_strTitleName;	// �ӿؼ��ı���
	CString m_strItemTblName;	// �����ӿؼ��ı���
	CString m_strDataTblName;	// �����Ŀ�����ݱ�ı���
	CString m_strPrecFilePath;

protected:

	// Generated message map functions
	//{{AFX_MSG(CImportFromXLSDlg)
	virtual BOOL OnInitDialog();
	virtual afx_msg void OnOpenFiledlg();
	afx_msg void OnDestroy();
	virtual afx_msg void OnBeginImport();
	afx_msg void OnSetDefaultNo();
	//}}AFX_MSG
	virtual afx_msg	void OnSelectChange(NMHDR *pNMHDR,LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTFROMXLSDLG_H__B50DF418_09BE_4479_9AD3_1EB20C186C24__INCLUDED_)
