#if !defined(AFX_APLICATIOINITDLG_H__F6E86972_6E3C_4895_B6E4_37DA22C82A90__INCLUDED_)
#define AFX_APLICATIOINITDLG_H__F6E86972_6E3C_4895_B6E4_37DA22C82A90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AplicatioInitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// 
// ��������ʱ��ʼ�����õĶԻ���
//
// ��Ҫ������ļ���COPY����ģ��Ŀ¼����������Ŀ¼��
//
// ����������Ŀ¼�����ڻ�û��������ļ�ʱ��Ӧ���ô�
// �Ի���
//
// ����ʼ�������㣬�ٵ��ô˶Ի���ʱ���Ի��򲢲�����ʾ��
//
/////////////////////////////////////////////////////////////////////////////

#define WM_INITIALIZEWORK	(WM_USER+10)

class CAplicatioInitDlg : public CDialog
{
// Construction
public:
	CAplicatioInitDlg(CWnd* pParent = NULL);   // standard constructor

public:
	// CopyFileEx�Ļص�����
	static DWORD CALLBACK CopyProgressRoutine(
												LARGE_INTEGER TotalFileSize,
												LARGE_INTEGER TotalBytesThransferred,
												LARGE_INTEGER StreamSize,
												LARGE_INTEGER StreamBytesTransferred,
												DWORD dwStreamNumber,
												DWORD dwCallbackReason,
												HANDLE hSourceFile,
												HANDLE hDestinatonFile,
												LPVOID lpData);

protected:
	// ��ʽ��ʹ·���Ϻ�����Ҫ��
	void FormatDirPath(CString &DirPath);

	short UpgradeDB(CString strDestDB, CString strSourceDB);

private:
	// �����ļ�������ָ����Ŀ¼
	BOOL CopyFileToEveryDir();

// Dialog Data
	//{{AFX_DATA(CAplicatioInitDlg)
	enum { IDD = IDD_APPINIT_DLG };
	CProgressCtrl	m_InitProgress;
	CString	m_InitializeInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAplicatioInitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation


protected:
	short UpgradeDatabase();
	CStringList* GetFieldNameList(CDaoTableDef *pTDef);
	BOOL StringInList(CString strTableName, CStringList *strList);
	CStringList* GetTableNameList(CDaoDatabase *pDB);
	// Generated message map functions
	//{{AFX_MSG(CAplicatioInitDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnInitializeWorkFunc(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CString ReadCommonMdbPath();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APLICATIOINITDLG_H__F6E86972_6E3C_4895_B6E4_37DA22C82A90__INCLUDED_)
