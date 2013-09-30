// AutoIPED.h : main header file for the AUTOIPED application
//

#if !defined(AFX_AUTOIPED_H__8F862085_B060_11D7_9BCC_000AE616B8F1__INCLUDED_)
#define AFX_AUTOIPED_H__8F862085_B060_11D7_9BCC_000AE616B8F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
//ȫ�ֱ���

extern  const _TCHAR szSoftwareKey[];//����ע���λ�ã�

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDApp:
// See AutoIPED.cpp for the implementation of this class
//

class CAutoIPEDApp : public CWinApp
{
public:
	CAutoIPEDApp();

public:
	// �ж����ݿ����Ƿ�û��һ������
	BOOL IsNoOneEngin();

	bool WriteRegedit();

	CString GetRegKey(LPCTSTR pszKey, LPCTSTR pszName,const CString Default);

    void SetRegValue(CString vKey, CString vName, CString vValue);

	BOOL ReadInitKey();

	BOOL InitializeProjectTable( const CString& strPrjID, const CString& strVolCode, 
										  const CString& strPrjName = _T("AutoIPED") );

protected:
	void CopyFromTemplateDirToPrjDir();

public:
    _ConnectionPtr m_pConnection;		// ��Ŀ���ݿ�(AutoIPED.mdb)
	_ConnectionPtr m_pConnectionCODE;	// ��׼���ݿ�
	_ConnectionPtr m_pConAllPrj;        // ����Ĺ������ݿ�,���������ѡ�񹤳̾����Ҫ�õ��ı� 

	_ConnectionPtr m_pConMaterial;		// ���Ͽ�ָ��.
	_ConnectionPtr m_pConMaterialName;  // ���������ݿ�ָ��
	_ConnectionPtr m_pConIPEDsort;		// �������ݿ�

	_ConnectionPtr m_pConWorkPrj;		// ��Ŀ��ʱ�⡣
	_ConnectionPtr m_pConWeather;		// �������������

	_ConnectionPtr m_pConMedium;		// �������ݿ�.
	_ConnectionPtr m_pConRefInfo;		// ��ҵ�������ݿ�
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoIPEDApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAutoIPEDApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnReplaceCurrentOldnameNewname();
	afx_msg void OnUpdateReplaceCurrentOldnameNewname(CCmdUI* pCmdUI);
	//}}AFX_MSG

public:
//	bool ImportAutoPD( CString strCommandLine );
	DECLARE_MESSAGE_MAP()

//����ָ�����̵ļ�¼, ����֪���ֶ�,��������ֶε�ֵ.
//	bool RefreshData(CString EnginID);
};

extern CAutoIPEDApp theApp;

/////////////////////////////////////////////////////////////////////////////
extern BOOL FileExists(LPCTSTR lpszPathName);
extern BOOL DirExists(LPCTSTR lpszDir);
extern CString _GetFileName(LPCTSTR lpszPathName);
extern BOOL IsTableExists(_ConnectionPtr pCon, CString tb);

extern CString gsIPEDInsDir;    //AutoIPED��װĿ¼
extern CString gsProjectDBDir;  //AutoIPED���ݿⰲװĿ¼
extern CString gsProjectDir;	//�����û��������ݿ�Ŀ¼
extern CString gsTemplateDir;	//�����û��������ݿ�Ŀ¼
extern CString gsShareMdbDir;  //�������ݿ�Ĵ��Ŀ¼  xl 10.19.2007

extern BOOL bIsCloseCompress;   //�ر�ʱ�Ƿ�ѹ�����ݿ� 
extern BOOL bIsReplaceOld;		//��ʱ�Ƿ��Զ��滻�ɲ�������
extern BOOL bIsMoveUpdate;		//�༭ԭʼ�����ƶ���¼ʱ�Ƿ����
extern BOOL bIsAutoSelectPre;   //�༭ԭʼ�����ƶ���¼ʱ�Զ�ѡ���²���
extern BOOL bIsHeatLoss;		//����ʱ�ж����ɢ���ܶ�
extern BOOL bIsAutoAddValve;	//�༭ԭʼ�����ƶ���¼ʱ�ڹܵ����Զ����ӷ���
extern BOOL gbIsStopCalc;	 	//�Ƿ����.
extern BOOL gbIsRunSelectEng;	//����ʱ�Ƿ񵯳�ѡ�񹤳̾��Ի���.
extern BOOL gbIsSelTblPrice;	//ѡ����е��ȼ۱�������ֵ
extern BOOL gbIsReplaceUnit;	//���뱣����������ʱ�滻��λ

extern int  giInsulStruct;		//����ʱ�����½ṹ����ϵ�ѡ��0���ֹ�ѡ��     1�������Զ�ѡ��
extern int  giCalSteanProp;		//����ˮ���������ʵı�׼:		0, IAPWS-IF97��   1, IFC67
extern BOOL gbAutoPaint120; 

extern BOOL gbWithoutProtectionCost;    //���㾭�ú��ʱ��������������Ϸ��ã�Ĭ��Ϊ0-����
extern BOOL gbInnerByEconomic;    //˫����ı��¼��㾭�ú��ʱ�ڲ㲻�������¶ȷ����㣬Ĭ��Ϊ0-�������¶ȷ�����

//extern int	giSupportOldCode;	//����֧���ܼ��ı�׼.			0,�¹ܹ�(���ҵ�����ҵ��׼,DL/T 5054-1996)  1,�Ϲܹ�(ԭ���ҵ�������׼,DLGJ 23-81).
//extern int  giSupportRigidity;	//����֧���ܼ��ĸն�����.		0,��ǿ������  1, ���ն�����	
//extern int  giHCrSteel;			//�߸���,

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOIPED_H__8F862085_B060_11D7_9BCC_000AE616B8F1__INCLUDED_)
