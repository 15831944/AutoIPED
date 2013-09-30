// AplicatioInitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "AplicatioInitDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "autoiped.h"
extern CAutoIPEDApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CAplicatioInitDlg dialog


CAplicatioInitDlg::CAplicatioInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAplicatioInitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAplicatioInitDlg)
	m_InitializeInfo = _T("");
	//}}AFX_DATA_INIT
}


void CAplicatioInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAplicatioInitDlg)
	DDX_Control(pDX, IDC_INIT_PROGRESS, m_InitProgress);
	DDX_Text(pDX, IDC_STATIC_INFO, m_InitializeInfo);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CAplicatioInitDlg, CDialog)
	//{{AFX_MSG_MAP(CAplicatioInitDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITIALIZEWORK,OnInitializeWorkFunc)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////////
//
// ��Ӧ�Զ������Ϣ����Ӧ����
//
// ��ɶԱ����̵ĳ�ʼ��
//
LRESULT CAplicatioInitDlg::OnInitializeWorkFunc(WPARAM wParam, LPARAM lParam)
{
	//
	if(!CopyFileToEveryDir())
	{
		OnCancel();
		return 0;
	}
	//�������ݿ�
	if ( !UpgradeDatabase() )
	{
		OnCancel();
		return 0;
	}

	OnOK();
	return 0;
}
//////////////////////////////////////
//��ȡ�������ݿ���Ŀ¼
CString CAplicatioInitDlg::ReadCommonMdbPath()
{
	HKEY hKey;
	DWORD szSize = 255;
	CString strCommon = "";

	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\UESoft Shared\\AutoPDS3.0"),0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS )
	{
		AfxMessageBox( "��ע���ʧ��!" );
		return "";
	}

	if( RegQueryValueEx(hKey, "ArxIniPath", NULL, NULL, (LPBYTE)strCommon.GetBufferSetLength(255), &szSize) != ERROR_SUCCESS )
	{
		AfxMessageBox( "��ע���ʧ��!" );
		return "";
	}
	
	strCommon.ReleaseBuffer();
	strCommon.TrimRight("\\");
	strCommon = strCommon + _T("\\Common Mdb");
	return strCommon;

}
/////////////////////////////////////////////////////////////
//
// �����ļ�������ָ����Ŀ¼
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
BOOL CAplicatioInitDlg::CopyFileToEveryDir()
{
	CString strFrom,strTo;
	BOOL bRet;
	DWORD dwRet;
	UINT uRet;

	if( gsProjectDBDir.IsEmpty()	||
		gsProjectDir.IsEmpty()		||
		gsTemplateDir.IsEmpty()		||
		gsShareMdbDir.IsEmpty() )
		
	{
		ReportExceptionErrorLV1(_T("ע�����Ϣ������"));

		return FALSE;
	}


	bRet=::CreateDirectory(gsProjectDBDir,NULL);

	if(!bRet)
	{
		dwRet=GetLastError();

		if(dwRet!=ERROR_ALREADY_EXISTS)
		{
			uRet=GetDriveType(gsProjectDBDir);

			if(uRet==DRIVE_UNKNOWN || uRet==DRIVE_NO_ROOT_DIR)
			{
				ReportExceptionErrorLV1(_T("�����ļ���ʧ��"));
				return FALSE;
			}
		}
	}

	bRet=::CreateDirectory(gsProjectDir,NULL);

	if(!bRet)
	{
		dwRet=GetLastError();

		if(dwRet!=ERROR_ALREADY_EXISTS)
		{
			uRet=GetDriveType(gsProjectDBDir);

			if(uRet==DRIVE_UNKNOWN || uRet==DRIVE_NO_ROOT_DIR)
			{
				ReportExceptionErrorLV1(_T("�����ļ���ʧ��"));
				return FALSE;
			}
		}
	}

	//
	// ��gsTemplateDir��gsProjectDBDir
	//

	LPCTSTR szCopyFileInfo[]=
	{
		_T("AUTOIPED.MDB"),
	};

	for(int i=0;i<sizeof(szCopyFileInfo)/sizeof(szCopyFileInfo[0]);i++)
	{
		strFrom=gsTemplateDir;
		strTo=gsProjectDBDir;

		FormatDirPath(strFrom);
		FormatDirPath(strTo);

		strFrom+=szCopyFileInfo[i];
		strTo+=szCopyFileInfo[i];

		if(!FileExists(strTo))
		{
			ShowWindow(SW_NORMAL);
			UpdateWindow();
		}
		else
		{
			continue;
		}

		m_InitializeInfo.Format(_T("�����ļ���%s��%s"),strFrom,strTo);

		UpdateData(FALSE);
		UpdateWindow();
		
		bRet=::CopyFileEx(strFrom,strTo,CopyProgressRoutine,this,NULL,COPY_FILE_FAIL_IF_EXISTS);

		if(!bRet)
		{
			dwRet=GetLastError();

			if(dwRet!=ERROR_FILE_EXISTS)
			{
				//
				// �������CopyFileExʧ���п�������Ϊ����ϵͳ��98������
				// ����CopyFile����
				//
				bRet=CopyFile(strFrom,strTo,TRUE);

				if(!bRet)
				{
					dwRet=GetLastError();

					if(dwRet!=ERROR_FILE_EXISTS)
					{
						ReportExceptionErrorLV1(_T("�����ļ�ʧ��:\n��ȷ���ļ���"+strFrom+"���Ƿ���ڡ�"));
						return FALSE;
					}
				}

			}
		}

	}

	//
	// ��gsTemplateDir��gsProjectDir
	//

	LPCTSTR szCopyFileInfo2[]=
	{
		_T("WORKPRJ.MDB")
	};


	for(i=0;i<sizeof(szCopyFileInfo2)/sizeof(szCopyFileInfo2[0]);i++)
	{
		strFrom=gsTemplateDir;
		strTo=gsProjectDir;

		FormatDirPath(strFrom);
		FormatDirPath(strTo);

		strFrom+=szCopyFileInfo2[i];
		strTo+=szCopyFileInfo2[i];

		if(!FileExists(strTo))
		{
			ShowWindow(SW_NORMAL);
			UpdateWindow();
		}
		else
		{
			continue;
		}

		m_InitializeInfo.Format(_T("�����ļ���%s��%s"),strFrom,strTo);

		UpdateData(FALSE);
		UpdateWindow();

		bRet=::CopyFileEx(strFrom,strTo,CopyProgressRoutine,this,NULL,COPY_FILE_FAIL_IF_EXISTS);

		if(!bRet)
		{
			dwRet=GetLastError();

			if(dwRet!=ERROR_FILE_EXISTS)
			{
				//
				// �������CopyFileExʧ���п�������Ϊ����ϵͳ��95������
				// ����CopyFile����
				//
				bRet=CopyFile(strFrom,strTo,TRUE);

				if(!bRet)
				{
					dwRet=GetLastError();

					if(dwRet!=ERROR_FILE_EXISTS)
					{
						ReportExceptionErrorLV1(_T("�����ļ�ʧ��"));
						return FALSE;
					}
				}
			}
		}

	}
//�������ݿ⵽�����ļ�������

	LPCTSTR szCopyFileInfo3[]=
	{
		_T("TableFormat.MDB"),
		_T("DrawingSize.MDB"),
		_T("Material.MDB"),
		_T("Medium.mdb"),
		_T("MaterialName.mdb"),
		_T("ALLPRJDB.MDB")
	};


	for(i=0;i<sizeof(szCopyFileInfo3)/sizeof(szCopyFileInfo3[0]);i++)
	{
		strFrom=ReadCommonMdbPath();
		if ( "" == strFrom )
		{
			return FALSE;
		}

		strTo=gsShareMdbDir;

		FormatDirPath(strFrom);
		FormatDirPath(strTo);

		strFrom += szCopyFileInfo3[i];
		strTo += szCopyFileInfo3[i];

		if(!FileExists(strTo))
		{
			ShowWindow(SW_NORMAL);
			UpdateWindow();
		}
		else
		{
			continue;
		}

		m_InitializeInfo.Format(_T("�����ļ���%s��%s"),strFrom,strTo);

		UpdateData(FALSE);
		UpdateWindow();

		bRet=::CopyFileEx(strFrom,strTo,CopyProgressRoutine,this,NULL,COPY_FILE_FAIL_IF_EXISTS);

		if(!bRet)
		{
			dwRet=GetLastError();

			if(dwRet!=ERROR_FILE_EXISTS)
			{
				//
				// �������CopyFileExʧ���п�������Ϊ����ϵͳ��95������
				// ����CopyFile����
				//
				bRet=CopyFile(strFrom,strTo,TRUE);

				if(!bRet)
				{
					dwRet=GetLastError();

					if(dwRet!=ERROR_FILE_EXISTS)
					{
						ReportExceptionErrorLV1(_T("�����ļ�ʧ��"));
						return FALSE;
					}
				}
			}
		}

	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
//
// CopyFileEx�Ļص�����
//
DWORD CALLBACK CAplicatioInitDlg::CopyProgressRoutine(
  													   LARGE_INTEGER TotalFileSize,
													   LARGE_INTEGER TotalBytesThransferred,
													   LARGE_INTEGER StreamSize,
													   LARGE_INTEGER StreamBytesTransferred,
													   DWORD dwStreamNumber,
													   DWORD dwCallbackReason,
													   HANDLE hSourceFile,
													   HANDLE hDestinatonFile,
													   LPVOID lpData)
{
	CAplicatioInitDlg *pDlg;
	double fPercent;

	pDlg=(CAplicatioInitDlg*)lpData;

	fPercent=(double)TotalBytesThransferred.LowPart/(double)TotalFileSize.LowPart*100.0;
 
  
	pDlg->m_InitProgress.SetPos((int)fPercent);

	pDlg->UpdateWindow();
	
	return PROGRESS_CONTINUE;
}

//////////////////////////////////////////////////////////////////
//
// ��ʽ��ʹ·���Ϻ�����Ҫ��
//
// DirPath[in/out]	��Ҫ��ʽ�����ַ����������Ѿ���ʽ�����ַ���
//
// �κ�����·�����Ҷ���Ŀո�ȥ���������ĩβû�С�\��������
//
void CAplicatioInitDlg::FormatDirPath(CString &DirPath)
{
	DirPath.TrimLeft();
	DirPath.TrimRight();

	if(DirPath.IsEmpty())
	{
		return;
	}

	if(DirPath.Right(1)!=_T("\\"))
	{
		DirPath+=_T("\\");
	}
}


/////////////////////////////////////////////////////////////////////////////
// CAplicatioInitDlg message handlers

BOOL CAplicatioInitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_InitProgress.SetRange(1,100);
	m_InitProgress.SetPos(0);
	
	this->PostMessage(WM_INITIALIZEWORK,0,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



//------------------------------------------------------------------                
// DATE         :[2005/06/03]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :��ָ�������ݿ��������.
//------------------------------------------------------------------
short CAplicatioInitDlg::UpgradeDB(CString strDestDB, CString strSourceDB)
{

	 _ConnectionPtr pConAlter;
	 pConAlter.CreateInstance(__uuidof(Connection));

	CDaoDatabase	sDB,dDB;
	CDaoTableDef	sTDef(&sDB);		//���ݱ�
	CDaoTableDef	dTDef(&dDB);

	CDaoFieldInfo	fieldInfo;			//�ֶ���Ϣ
	CDaoFieldInfo	fieldInfoS;
	CDaoFieldInfo	fieldInfoD;
	CString			strSQL;				//SQL���
	try
	{
		//Microsoft.Jet.OLEDB.4.0;	������. �����޸��ֶεĴ�С.
		pConAlter->Open(_bstr_t(CONNECTSTRING+strDestDB),"","",-1);

		sDB.Open(strSourceDB);			//�����ݿ�
		dDB.Open(strDestDB);
		
		CStringList *sListTableName;	//���б������б�.
		CStringList *dListTableName;
		CStringList *sListFieldName;	//ָ�����������ֶ������б�
		CStringList *dListFieldName;
		
		CString		strTableName;		//��ǰ����ı���
		CString		strFieldName;		//��ǰ������ֶ���

		sListTableName = GetTableNameList(&sDB);		//�����Դ���ݿ��еĸ��������
		dListTableName = GetTableNameList(&dDB);		//Ŀ�����ݿ����.
		
		for (POSITION pos=sListTableName->GetHeadPosition(); pos != NULL ; )
		{
			strTableName = sListTableName->GetNext(pos);

			if ( StringInList(strTableName, dListTableName) )
			{
				//�������ݿ��ж�������ͬ�ı�, 
				sTDef.Open(strTableName);		//��
				dTDef.Open(strTableName);
				
				sListFieldName = GetFieldNameList(&sTDef);		//�����Դ���ݿ��е�ָ����������ֶ���.
				dListFieldName = GetFieldNameList(&dTDef);		//Ŀ�Ŀ��б���ֶ�.

				for ( POSITION pos=sListFieldName->GetHeadPosition(); pos != NULL; )
				{
					strFieldName = sListFieldName->GetNext(pos);
					if ( StringInList(strFieldName, dListFieldName) )
					{
						//������������ͬ���ֶ�
						sTDef.GetFieldInfo(strFieldName, fieldInfoS);
						dTDef.GetFieldInfo(strFieldName, fieldInfoD);

						if (fieldInfoD.m_lSize != fieldInfoS.m_lSize)
						{
							//�ֶδ�С��һ��.�ı�Ŀ�ı��е��ֶεĴ�С,Ϊԭ���ݿ���е��ֶ�
							strSQL.Format("ALTER TABLE ["+strTableName+"] ALTER COLUMN "+strFieldName+" VARCHAR (%d) ", fieldInfoS.m_lSize);
							pConAlter->Execute(_bstr_t(strSQL),NULL,adCmdText);
						}

					}
					else
					{
						//��������û����ͬ���ֶ�
						sTDef.GetFieldInfo(strFieldName, fieldInfo);
						fieldInfo.m_bAllowZeroLength = FALSE;
						fieldInfo.m_bRequired		 = FALSE;

						dTDef.CreateField(fieldInfo);			//��Ŀ�����ݱ�������һ���ֶ�.

						//�����µ��ֶ�ʱ,��ͬ����һ�����.
				//		strSQL.Format("UPDATE %s SET %s.%s=T2.%s FROM (SELECT * FROM [%s] IN \'%s\') AS T2 ",
				//				strTableName, strTableName,strFieldName,strFieldName,strTableName,sDB.GetName());
				//		dDB.Execute(strSQL);
				//		pConAlter->Execute(_bstr_t(strSQL),NULL,adCmdText);
					}
				}
				sListFieldName->RemoveAll();	//��������ֶ������б�. ���ͷ��ڴ�
				dListFieldName->RemoveAll();
				delete sListFieldName;
				delete dListFieldName;
				sTDef.Close();
				dTDef.Close();
			}
			else
			{	//��Ŀ�����ݿ���û��Դ���ݿ�ָ���ı�	������.
				strSQL.Format("SELECT * INTO %s FROM [%s] IN \"%s\" ",
							strTableName, strTableName, sDB.GetName());
				dDB.Execute(strSQL);
			}
		}

		sListTableName->RemoveAll();		//������б������б�. ���ͷ��ڴ�
		dListTableName->RemoveAll();
		delete sListTableName;
		delete dListTableName;
		sDB.Close();
		dDB.Close();

		pConAlter->Close();		
	}catch (CException* e)
	{
		e->ReportError();
		return 0 ;
	}	
	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/06/03]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :����ָ�����ݿ��е����б������. ���ص��б�ָ�붼Ҫ�����ͷ��ڴ�ռ�
//------------------------------------------------------------------
CStringList* CAplicatioInitDlg::GetTableNameList(CDaoDatabase *pDB)
{
	ASSERT_VALID(pDB);
	CStringList *tableNameList;
	CDaoTableDefInfo tDefInfo;
	tableNameList = new CStringList();
	int iCount = pDB->GetTableDefCount();
	for(int i=0; i<iCount; i++)
	{
		pDB->GetTableDefInfo(i, tDefInfo);
		CString strTemp = tDefInfo.m_strName;
		if((tDefInfo.m_lAttributes & dbSystemObject) == 0)
		{
			tableNameList->AddTail(tDefInfo.m_strName);
		}
	}
	return tableNameList;
}

//------------------------------------------------------------------                
// DATE         :[2005/06/03]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :���ָ����������ֶ�����. ���ص��б�ָ�붼Ҫ�����ͷ��ڴ�ռ�
//------------------------------------------------------------------
CStringList* CAplicatioInitDlg::GetFieldNameList(CDaoTableDef *pTDef)
{
	ASSERT_VALID(pTDef);
	CStringList	*	fieldNameList;
	CDaoFieldInfo	tDefInfo;
	
	fieldNameList = new CStringList();
	int nCount = pTDef->GetFieldCount();
	for (int i=0; i<nCount; i++)
	{
		pTDef->GetFieldInfo(i, tDefInfo);

		fieldNameList->AddTail(tDefInfo.m_strName);
	}
	return fieldNameList;
}

//------------------------------------------------------------------                
// DATE         :[2005/06/03]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�Ѷ�һ��ָ�����ַ����Ƿ����ַ��б��У����ڷ���TRUE�����򷵻�FALSEL��
//------------------------------------------------------------------
BOOL CAplicatioInitDlg::StringInList(CString strTableName, CStringList *strList)
{
	CString		strTmp;

	for (POSITION pos=strList->GetHeadPosition(); pos != NULL; )
	{
		strTmp = strList->GetNext(pos);
		if ( !strTmp.CompareNoCase(strTableName) )
		{
			return TRUE;
		}
	}
	return FALSE;
}


//------------------------------------------------------------------                
// DATE         :[2005/06/03]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :���Ӧ�ó����ǵ�һ������ʱ,�����ݿ��������
//------------------------------------------------------------------
short CAplicatioInitDlg::UpgradeDatabase()
{
	if (_T("0") == theApp.GetRegKey(_T("Status"), _T("Install"), CString(_T("0"))) ) 
	{
		try
		{		
			CString strTo;
			CString strFrom;
			LPCTSTR szCopyFileInfo[]=
			{
				_T("AUTOIPED.MDB"),		//problem      xl
//				_T("IPEDsort.MDB"),
			};
			
			for(int i=0;i<sizeof(szCopyFileInfo)/sizeof(szCopyFileInfo[0]);i++)
			{
				strFrom=gsTemplateDir;
				strTo=gsProjectDBDir;
				
				FormatDirPath(strFrom);
				FormatDirPath(strTo);
				
				strFrom+=szCopyFileInfo[i];
				strTo+=szCopyFileInfo[i];
				
				UpgradeDB(strTo,strFrom);		//������
			}
			//
			// ��gsTemplateDir��gsProjectDir
			//
			
			LPCTSTR szCopyFileInfo2[]=
			{
				_T("WORKPRJ.MDB")
			};		
			for(i=0;i<sizeof(szCopyFileInfo2)/sizeof(szCopyFileInfo2[0]);i++)
			{
				strFrom=gsTemplateDir;
				strTo=gsProjectDir;
				
				FormatDirPath(strFrom);
				FormatDirPath(strTo);
				
				strFrom+=szCopyFileInfo2[i];
				strTo+=szCopyFileInfo2[i];

				UpgradeDB(strTo,strFrom);		//������				
			}
			//

			LPCTSTR szCopyFileInfo3[]=
			{
				_T("TableFormat.MDB"),
				_T("DrawingSize.MDB"),
				_T("Material.MDB"),
				_T("Medium.mdb"),
				_T("MaterialName.mdb"),
				_T("ALLPRJDB.MDB")
			};		
			for(i=0;i<sizeof(szCopyFileInfo3)/sizeof(szCopyFileInfo3[0]);i++)
			{
				strFrom=ReadCommonMdbPath();
				if ( "" == strFrom )
				{
					return FALSE;
				}
				strTo=gsShareMdbDir;

				FormatDirPath(strFrom);
				FormatDirPath(strTo);
				
				strFrom+=szCopyFileInfo3[i];
				strTo+=szCopyFileInfo3[i];

				UpgradeDB(strTo,strFrom);		//������				
			}
			//
			theApp.SetRegValue(_T("Status"), _T("Install"), CString(_T("1")));
		}
		catch (_com_error& e) 
		{
			AfxMessageBox(e.Description());
			return 0;
		}
	}
	return 1;
}
