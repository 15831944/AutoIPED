//{{AFX_INCLUDES()
#include "DataGridEx.h"
#include "datagrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATASHOWDLG_H__D0159595_D370_4E3C_8092_F04831CD8155__INCLUDED_)
#define AFX_DATASHOWDLG_H__D0159595_D370_4E3C_8092_F04831CD8155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataShowDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
//
// ������ʾ�Ի���
//
// �ڶԻ��򴴽�֮ǰ����Ҫ����SetRecordset���ü�¼��������
// �򿪼�¼��֮ǰ��  ����CursorLocation = adUseClient;
//
///////////////////////////////////////////////////////////////////////////// 


class CDataShowDlg : public CDialog
{
// Construction
public:
	CDataShowDlg(CWnd* pParent = NULL);   // standard constructor
	~CDataShowDlg();

	enum
	{
		HideFirstColumn=-1,
		HideLastColumn=-2,
		EndHideArray=-4
	};

	struct tag_DefaultFieldValue	//����DataGrid�ؼ�����Ӽ�¼ʱ���ֶε�Ĭ��ֵ 
	{
		CString strFieldName;		// Ĭ���ֶ���
		_variant_t varFieldValue;	// �ֶε�ֵ
	};

public:
	//������DataGrid�ؼ���ʾ����ʱ����Ҫ���ص���throw(COleDispatchException)	
	void SetHideColumns(int *pIndex,DWORD IndexSize);	
	//������DataGrid�ؼ���ʾ����ʱ����Ҫ���ص���throw(COleDispatchException)
	void SetHideColumns(LPCTSTR szFields);		
	
	BOOL UpdateBrowWindow();	//��������������˼�¼����������´��ڵ��ֶ��������⣬Ĭ��ֵ��

	void SetAllowUpdate(BOOL IsAllow);				// �����Ƿ��������
	BOOL GetAllowUpdate();							// �����Ƿ��������

	void SetAllowAddNew(BOOL IsAllow);				// �����Ƿ��������Ӽ�¼
	BOOL GetAllowAddNew();							// �����Ƿ����������Ӽ�¼

	void SetAllowDelete(BOOL IsAllow);				// �����Ƿ�����ɾ����¼
	BOOL GetAllowDelete();							// �����Ƿ�����ɾ����¼

	void SetDataGridCaption(LPCTSTR pCaption);		// ����DataGrid�ؼ��ı���
	CString GetDataGridCaption();					// ����DataGrid�ؼ��ı���	

	void SetDlgCaption(LPCTSTR pCaption);			// ���öԻ���ı���
	CString GetDlgCaption();						// ���ضԻ���ı���

	void SetRecordset(_RecordsetPtr &IRecordset, CDataGridEx* pCtlDataGrid=NULL );	// ������Ҫ��ʾ�ļ�¼��
	_RecordsetPtr& GetRecordset();					// ������Ҫ��ʾ�ļ�¼��

	// ��������Ӽ�¼ʱ���ֶε�Ĭ��ֵ
	void SetDefaultFieldValue(tag_DefaultFieldValue *pDefault,DWORD dwLength);

	// �����ڷ�ģ̬���Ƿ��ڴ�������ʱ�Ƿ��Զ�ɾ������
	void SetAutoDelete(BOOL bAutoDelete);

	//����ָ�����ֶθ�һ���ض���ֵ.
	void SetPassField(CString strField);

protected:
	//����ָ������
	void HideColumns();		//throw(COleDispatchException)

	// ����ָ���ֶ����Ƶ���
	void HideColumnsByFieldsName(LPCTSTR szFields); // throw(COleDispatchException);

	//��λ��굽ָ������.
	bool FindDataGridCol(short colIndex);

public:
	BOOL SetFieldCaption(CString strTblName);//�����ֶεı���Ϳ��
	void SetCursorCol(short startCol);	//���ù�굽ָ������.
	

// Dialog Data
	//{{AFX_DATA(CDataShowDlg)
	enum { IDD = IDD_DATA_SHOW_DLG };
	CDataGridEx	m_ResultBrowseControl;
	CDataGridEx	m_BrowseGrid;

	CDataGridEx* pCurDataGrid;		//��ǰ�����DataGrid�ؼ�.
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataShowDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	short m_nCursorCol;
	CString m_strCol_Pass;
	HICON m_hIcon;
	virtual void OnCancel();
	virtual void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CDataShowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOnAddNewResultBrowse();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDestroy();
	afx_msg void OnRowColChangeResultBrowse(VARIANT FAR* LastRow, short LastCol);
	afx_msg void OnClickResultBrowse();
	afx_msg void OnColResizeResultBrowse(short ColIndex, short FAR* Cancel);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	

	BOOL m_IsAutoDelete;			// �ڷ�ģ̬���Ƿ��ڴ�������ʱ�Զ�ɾ������
	tag_DefaultFieldValue* m_pDefaultFieldValueStruct;

	CString m_strFieldsNameToHide;	//��Ҫ�����е��ֶ����ַ���
	int* m_pIndexArrayToHide;		//��Ҫ�����е�������

	BOOL m_IsAllowDel;				// �Ƿ�����ɾ��
	BOOL m_IsAllowAddNew;			// �Ƿ���������
	BOOL m_IsAllowUpdate;			// �Ƿ��������
	CString m_DataGridCaption;		// DataGrid�ؼ��ı���
	CString m_DlgCaption;			// �Ի���ı���
	_RecordsetPtr m_IRecordset;		// ����ʾ�ļ�¼��������ָ��

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASHOWDLG_H__D0159595_D370_4E3C_8092_F04831CD8155__INCLUDED_)


extern	BOOL GetStructTblName(_ConnectionPtr& pCon, CString strDataTblName,CString& strStructTblName);
extern	BOOL SetFieldCaptionAndWidth(CDataGridEx* pDataGrid, _ConnectionPtr& pConSort, CString strTblName);

extern	BOOL SaveDataGridFieldWidth(CDataGridEx* pDataGrid, _ConnectionPtr& pConSort, CString strTblName);
extern	BOOL GetScreenRatio(float& xRatio, float& yRatio);