// AutoIPED.cpp : Defines the class behaviors for the application.
//

#include "HyperLink.h"	// Added by ClassView
#include "stdafx.h"
#include "AutoIPED.h"

#include "MainFrm.h"
#include "AutoIPEDDoc.h"
#include "AutoIPEDView.h"
#include "SelEngVolDll.h"
#include "EDIBgbl.h"
#include "Newproject.h"
#include "HyperLink.h"

#include "AplicatioInitDlg.h"	//�����ʼ���Ի���

//#include "ImportFromPreCalcXLSDlg"
#include "ImportFromPreCalcXLSDlg.h"
#include "ProjectOperate.h"
#include "mainfrm.h"
#include "ImportAutoPD.h"

#include "CoolControlsManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CAutoIPEDApp

BEGIN_MESSAGE_MAP(CAutoIPEDApp, CWinApp)
	//{{AFX_MSG_MAP(CAutoIPEDApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
//	ON_COMMAND(IDM_REPLACE_CURRENT_OLDNAME_NEWNAME, OnReplaceCurrentOldnameNewname)
//	ON_UPDATE_COMMAND_UI(IDM_REPLACE_CURRENT_OLDNAME_NEWNAME, OnUpdateReplaceCurrentOldnameNewname)
	//}}AFX_MSG_MAP
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
//ȫ�ֱ���

const _TCHAR szSoftwareKey[]=_T("Software\\��ɳ��������������޹�˾\\AutoIPED\\8.0\\");
CString gsIPEDInsDir;	//AutoIPED��װĿ¼
CString gsProjectDBDir;	//AutoIPED���ݿⰲװĿ¼
CString gsProjectDir;	//�����û��������ݿ�Ŀ¼
CString gsTemplateDir;	//�����û��������ݿ�Ŀ¼
CString gsShareMdbDir;  //�������ݿ�Ĵ��Ŀ¼  xl 10.19.2007
CString gsHelpFilePath; //�����ĵ�·��  Luorj   2008.06.18


BOOL bIsCloseCompress;  //�ر�ʱ�Ƿ�ѹ�����ݿ� 
BOOL bIsReplaceOld;		//��ʱ�Ƿ��Զ��滻�ɲ�������
BOOL bIsMoveUpdate;		//�༭ԭʼ�����ƶ���¼ʱ�Ƿ����
BOOL bIsAutoSelectPre;  //�༭ԭʼ�����ƶ���¼ʱ�Զ�ѡ���²���
BOOL bIsHeatLoss;		//����ʱ�ж����ɢ���ܶ�
BOOL gbIsStopCalc;		//�Ƿ����.
BOOL bIsAutoAddValve;	//�༭ԭʼ�����ƶ���¼ʱ�ڹܵ����Զ����ӷ���
BOOL gbIsRunSelectEng;	//����ʱ�Ƿ񵯳�ѡ�񹤳̾��Ի���.
BOOL gbIsSelTblPrice;	//ѡ����е��ȼ۱�������ֵ
int  giInsulStruct;		//����ʱ�����½ṹ����ϵ�ѡ��0���ֹ�ѡ��     1, �����Զ�ѡ��
int  giCalSteanProp;	//����ˮ���������ʵı�׼:		0, IAPWS-IF97��   1, IFC67
//int Logo;				//mark_hubb_��ʶ����������ݿ�
BOOL gbIsReplaceUnit;	//���뱣����������ʱ�滻��λ
BOOL gbAutoPaint120;    //ͳ������ʱ���Զ����ϱ������ݽ����¶�С��120�ȵļ�¼��
BOOL gbWithoutProtectionCost;    //���㾭�ú��ʱ��������������Ϸ��ã�Ĭ��Ϊ0-����
BOOL gbInnerByEconomic;    //˫����ı��¼��㾭�ú��ʱ�ڲ㲻�������¶ȷ����㣬Ĭ��Ϊ0-�������¶ȷ�����

//int	 giSupportOldCode;	//����֧���ܼ��ı�׼.			0,�¹ܹ�(���ҵ�����ҵ��׼,DL/T 5054-1996)  1,�Ϲܹ�(ԭ���ҵ�������׼,DLGJ 23-81).
//int  giSupportRigidity;	//����֧���ܼ��ĸն�����.		0,��ǿ������  1, ���ն�����	
//int  giHCrSteel;    	//�߸���,


/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDApp construction

CAutoIPEDApp::CAutoIPEDApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAutoIPEDApp object

CAutoIPEDApp theApp;

//
// ȫ�ֹ���ı����������һ��������
// ��ֹ������������
//
#pragma data_seg(".IpedSeg")
HWND gShareMainWnd=NULL;		
#pragma data_seg()

#pragma comment(linker,"/SECTION:.IpedSeg,RWS")

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDApp initialization

BOOL CAutoIPEDApp::InitInstance()
{
	BOOL bRet;

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//
	// ����Ѿ��и����������о�ֹͣ���д˳���,
	// ����ʾ��ǰ�ĳ���
	//
/*	//�������ж������������, ���ǵ���AutoPD�������ݵ�IPEDʱ���ܹر��ֶ��򿪵ĳ���. [2005/07/22]
	if(gShareMainWnd && IsWindow(gShareMainWnd))
	{
		if(::IsIconic(gShareMainWnd))
		{
			::ShowWindow(gShareMainWnd,SW_RESTORE);
		}
		::SetWindowPos(gShareMainWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_ASYNCWINDOWPOS);
		::SetWindowPos(gShareMainWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_ASYNCWINDOWPOS);

		return FALSE;
	}
*/
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	free((void*)m_pszRegistryKey);
	CString str;
	str.LoadString(IDS_UESOFTCO);
	SetRegistryKey(str);
	if(m_pszProfileName!=NULL)
		free((void*)m_pszProfileName);
	m_pszProfileName=_tcsdup(_T("AutoIPED\\8.0"));
	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)

	ReadInitKey();//��������ע���ֵ

	//�Ա����̵����ݿ���г�ʼ��,�������������ݿ�.
	CopyFromTemplateDirToPrjDir();

	CString strHelpPath=gsHelpFilePath+_T("AutoIPED.chm");
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	free((void*)this->m_pszHelpFilePath);
	m_pszHelpFilePath=NULL;
	WriteProfileString(_T("Directory"),_T("HelpFilePath"),strHelpPath);
	m_pszHelpFilePath=_tcsdup(strHelpPath);


	strHelpPath=this->GetProfileString(_T("Directory"),_T("HelpFilePath"),strHelpPath);
	if(FileExists(strHelpPath))
	{
		if(m_pszHelpFilePath != NULL)
		{
			free((void*)this->m_pszHelpFilePath);
			m_pszHelpFilePath = NULL;
		}
		this->m_pszHelpFilePath=_tcsdup(strHelpPath);
		this->WriteProfileString(_T("Directory"),_T("HelpFilePath"),strHelpPath);
	}

	::CoInitialize(NULL);
	AfxOleInit();
	m_pConnection.CreateInstance(__uuidof(Connection));
	m_pConnectionCODE.CreateInstance(__uuidof(Connection));
	m_pConAllPrj.CreateInstance( __uuidof(Connection) );

	m_pConMaterial.CreateInstance(__uuidof(Connection));
	m_pConMaterialName.CreateInstance( __uuidof(Connection) );
	m_pConIPEDsort.CreateInstance(__uuidof(Connection));

	m_pConWorkPrj.CreateInstance(__uuidof(Connection));
	m_pConWeather.CreateInstance(__uuidof(Connection));

	m_pConMedium.CreateInstance(__uuidof(Connection));
	m_pConRefInfo.CreateInstance(__uuidof(Connection));

	// ��ADO�����н��������Ҫ����try...catch()�����������Ϣ��
	// ��Ϊ����ʱ�ᾭ������һЩ�벻���Ĵ���
	CString strConn;
//	strConnCODE = CONNECTSTRING + gsProjectDBDir + "IPEDcode.mdb";	
//	strConn.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s%s"),gsProjectDBDir,"AutoIPED.mdb");	
//	strConnCODE.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s%s"),gsProjectDBDir,"IPEDcode.mdb");	
	try                 
	{	
		// �򿪱���Access��bw.mdb
		strConn = CONNECTSTRING + gsProjectDBDir + "AutoIPED.mdb";
		m_pConnection->Open(_bstr_t(strConn),"","",-1);	

		//�򿪱��¹����
		strConn = CONNECTSTRING + gsTemplateDir + "IPEDsort.mdb";
		m_pConIPEDsort->Open(_bstr_t(strConn),"","",-1);	

		//����Ŀ��ʱ��
		strConn = CONNECTSTRING + gsProjectDir + "WorkPrj.mdb";
		m_pConWorkPrj->Open(_bstr_t(strConn),"","",-1);	

		//�򿪽������ݿ�,�����ݿ���ڹ���·����
		//strConn = CONNECTSTRING + EDIBgbl::strCur_MediumDBPath + "Medium.mdb";
		strConn = CONNECTSTRING + gsShareMdbDir + "Medium.mdb";
		m_pConMedium->Open(_bstr_t(strConn),"","",-1);
		
		// ��ҵ�������ݿ�
		strConn = CONNECTSTRING + gsTemplateDir + "RefInfo.mdb";
		m_pConRefInfo->Open( _bstr_t( strConn ), _T(""), _T(""), -1 );

		// �������ݿ�
		strConn = CONNECTSTRING + gsShareMdbDir + "AllPrjDB.mdb";
		m_pConAllPrj->Open(_bstr_t(strConn),"","",-1);
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	//�򿪱�׼��Ͳ��Ͽ�
	try
	{
		strConn = CONNECTSTRING + EDIBgbl::sCritPath + EDIBgbl::sCur_CritDbName;
		m_pConnectionCODE->Open(_bstr_t(strConn),"","",-1);	

		//���Ͽ�
		strConn = CONNECTSTRING + EDIBgbl::sMaterialPath + _T("Material.mdb");
		m_pConMaterial->Open(_bstr_t(strConn), "", "", -1 );

		//���������ݿ�
		strConn = CONNECTSTRING + EDIBgbl::sMaterialPath + "MaterialName.mdb";
		m_pConMaterialName->Open( _bstr_t(strConn), "", "", -1 );

		// "�й������������.mdb";
		strConn = CONNECTSTRING	+ EDIBgbl::sCritPath + "�й������������.mdb";
		m_pConWeather->Open(_bstr_t(strConn), "", "", -1 );
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}

	//�½�һ����ʱ�����ڼ������ϵ����
	EDIBgbl::NewCalcAlfaTable() ;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAutoIPEDDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CAutoIPEDView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//�������д���·�������ֿո�ʱ.�Զ�����ʱ,���ܽ������ļ�����������.
	//һ�ɶ����½��ļ�.
	cmdInfo.m_nShellCommand=CCommandLineInfo::FileNew;	//zsy

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	//���һ������Ҳû�оͲ����öԻ���
	try
	{
		bRet=IsNoOneEngin();
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return FALSE;
	}

	CString strCommandLine;
	strCommandLine = ::GetCommandLine();
	//test 
//	strCommandLine = "C:\\Program Files\\��ɳ��������������޹�˾\\AutoIPED7.0\\AutoIPED.exe  -DATA_FILENAME C:\\Program Files\\��ɳ��������������޹�˾\\AutoIPED7.0\\Template\\����Excelԭʼ��������.xls  -PROJECT_ID 567  -PROJECT_ENGVOL 56  -RECORD_COUNT 10";
///	CImportAutoPD  test;
///	test.ImportAutoPDInsul(strCommandLine);
	
	//����.  ��AutoPD�е��뱣�����ݵ�AutoIPED��. 
	//ʵ��   pd����Excel�м��ļ�, ����AutoIPED��ȡ���м��ļ�.
	if( strCommandLine.Find(" -DATA_FILENAME ", 0 ) != -1 )
	{
 		CDocument *pDocument;
  		pDocument=((CFrameWnd*)AfxGetMainWnd())->GetActiveDocument();
 		if(pDocument!=NULL)
		{
			//����½����ĵ�.
			((CAutoIPEDDoc*)pDocument)->m_Result=_T("");
			((CAutoIPEDDoc*)pDocument)->UpdateAllViews(NULL);
		}
		CImportAutoPD inPDInsul;
		//����ʵ��
		if( inPDInsul.ImportAutoPDInsul(strCommandLine) )
		{
			bRet = true;  //���ٵ��������ѡ�񹤳̶Ի���
		}
	}
	
	if(!bRet)
	{
		if ( gbIsRunSelectEng )	//��ѡ��Ի����е�һ��ѡ��ֵ��������ʱ������ѡ�񹤳̾�ᡱ�Ի���
		{
			//����ʹ�����������Ķ�̬��ѡ�񹤳̾��
			CSelEngVolDll dlg;

			CString AllPrjDBPathName=gsShareMdbDir+"AllPrjDb.mdb";
			CString sortPathName=gsShareMdbDir+"DrawingSize.mdb";
			CString workprjPathName=gsProjectDir+"workprj.mdb";
			LPSTR cAllPrjDBPathName = (LPSTR)(LPCTSTR) AllPrjDBPathName;
			LPSTR csortPathName = (LPSTR)(LPCTSTR) sortPathName;
			LPSTR cworkprjPathName = (LPSTR)(LPCTSTR) workprjPathName;
			
			if(dlg.ShowEngVolDlg( cworkprjPathName, cAllPrjDBPathName, csortPathName )==false)
			{
				// �ڱ���������ʾ��ǰ�Ĺ�������
				((CMainFrame*)m_pMainWnd)->ShowCurrentProjectName();
				return TRUE;
			}
						
			EDIBgbl::SelSpecID=dlg.m_SelSpecID;
			EDIBgbl::SelPrjID=dlg.m_SelPrjID;
			EDIBgbl::SelPrjName=dlg.m_SelPrjName;//��������
			EDIBgbl::SelDsgnID=dlg.m_SelDsgnID;
			EDIBgbl::SelHyID=dlg.m_SelHyID;
			EDIBgbl::SelVlmID=dlg.m_SelVlmID;
			EDIBgbl::SelVlmCODE=dlg.m_SelVlmCODE;
			

			//EDIBgbl::SelPrjID="12";
			//EDIBgbl::SelPrjName="12";

			// 
			InitializeProjectTable( EDIBgbl::SelPrjID, EDIBgbl::SelVlmCODE, EDIBgbl::SelPrjName );
		}
	}
	

	//ʵ�ֿؼ��ĸ���Ч��
//	GetCtrlManager().InstallHook();

	// �ڱ���������ʾ��ǰ�Ĺ�������
	((CMainFrame*)m_pMainWnd)->ShowCurrentProjectName();

	gShareMainWnd=m_pMainWnd->m_hWnd;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CHyperLink myURL;
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_address;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
 
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_address, m_address);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAutoIPEDApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
//�Զ��庯��
CString CAutoIPEDApp::GetRegKey(LPCTSTR pszKey, LPCTSTR pszName,const CString Default)
{
	HKEY hKey;
	_TCHAR sv[256];
	unsigned long vtype;
	unsigned long vlen;
	CString subKey=szSoftwareKey;
	subKey+=pszKey;
	if(RegOpenKey(HKEY_LOCAL_MACHINE,subKey,&hKey)!=ERROR_SUCCESS)
	{
		return Default;
	}

	vlen=256*sizeof(TCHAR);
	if(RegQueryValueEx(hKey,pszName,NULL,&vtype,(LPBYTE)sv,&vlen)!=ERROR_SUCCESS)
	{
		RegSetValueEx(hKey,pszName,NULL,REG_SZ,(LPBYTE)(LPCTSTR)Default,(Default.GetLength()+1)*sizeof(_TCHAR));
		RegCloseKey(hKey);
		return Default;
	}

	CString ret=Default;
	if(vtype==REG_SZ)
		ret=sv;
	ret.TrimLeft();
	ret.TrimRight();
	if(ret==_T(""))
	{
		RegSetValueEx(hKey,pszName,NULL,REG_SZ,(LPBYTE)(LPCTSTR)Default,(Default.GetLength()+1)*sizeof(_TCHAR));
		ret = Default;
	}
	if(vtype==REG_DWORD)
	{
		DWORD dwVal=*((DWORD*)(void*)sv);
		ret.Format(_T("%d"),dwVal);
	}

	RegCloseKey(hKey);
	return ret;
}

void CAutoIPEDApp::SetRegValue(CString vKey, CString vName, CString vValue)
{
	HKEY hKey;

	CString subKey=CString(szSoftwareKey+vKey);
	if(RegOpenKey(HKEY_LOCAL_MACHINE,subKey,&hKey)!=ERROR_SUCCESS)
	{
		RegCreateKey(HKEY_LOCAL_MACHINE,subKey,&hKey);
	}
	RegSetValueEx(hKey,vName,NULL,REG_SZ,(unsigned char*)LPCSTR(vValue),vValue.GetLength()+1);
	RegCloseKey(hKey);
}


BOOL CAutoIPEDApp::ReadInitKey()
{
	//���ܣ���������ע���ֵ���ڳ�������ʱ����
	//default directory is Appdir + "\\Example"
	char szPath[MAX_PATH];
	::GetCurrentDirectory( sizeof(szPath) , szPath );
	CString strDefault = "";

	//�����װ·��
	gsIPEDInsDir = GetRegKey(_T("Directory"),_T("EDinBox_InsDir"),strDefault);
	gsIPEDInsDir.TrimLeft();
	gsIPEDInsDir.TrimRight();
	if (gsIPEDInsDir.IsEmpty())
	{
		gsIPEDInsDir.Format("%s\\" , szPath);
		SetRegValue(_T("Directory"),_T("EDinBox_InsDir"),gsIPEDInsDir);
	}
	if( gsIPEDInsDir.Right(1) != "\\" )
	{
		gsIPEDInsDir += "\\";
	}
	HKEY key;
	DWORD szSize = 255;
	
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\��ɳ��������������޹�˾\\shareMDB"),0, KEY_ALL_ACCESS, &key) != ERROR_SUCCESS )
	{
		AfxMessageBox( "��ע���ʧ��!" );
		return FALSE;
	}
    gsShareMdbDir = "";
	if( RegQueryValueEx(key, "shareMDB", NULL, NULL, (LPBYTE)/*(LPTSTR)(LPCTSTR)*/gsShareMdbDir.GetBufferSetLength(255), &szSize) != ERROR_SUCCESS )
	{
		AfxMessageBox( "��ע���ʧ��!" );
		return FALSE;
	}
	
	RegCloseKey( key );
	gsShareMdbDir.TrimLeft();
	gsShareMdbDir.TrimRight();
	gsShareMdbDir.ReleaseBuffer();
	if( gsShareMdbDir.Right(1) != "\\" )
	{
		gsShareMdbDir += "\\";
	}

	//��ñ�׼���ݿ�·��
	EDIBgbl::sCritPath = GetRegKey(_T("Directory"),_T("EDinBox_CodeDBDir"), "");
	EDIBgbl::sCritPath.TrimLeft();
	EDIBgbl::sCritPath.TrimRight();
	if( EDIBgbl::sCritPath.IsEmpty() )
	{
		EDIBgbl::sCritPath = gsIPEDInsDir + "Code\\";
		SetRegValue(_T("Directory"), _T("EDinBox_CodeDBDir"), EDIBgbl::sCritPath);
	}
	if( EDIBgbl::sCritPath.Right(1) != "\\" )
	{
		EDIBgbl::sCritPath += "\\";
	}
	//��ò������ݿ�·��
	//��Ϊ��AutoPDMS3.0����Ŀ¼һ�� ligb 2010.08.11
	EDIBgbl::sMaterialPath = gsShareMdbDir;
	if( EDIBgbl::sMaterialPath.IsEmpty() )
	{
		AfxMessageBox(IDS_SHAREDIR_ISEMPTY,MB_OK); 
	}
	//��Ŀ���ݿ�·��
	strDefault = "";	
	gsProjectDBDir=GetRegKey(_T("Directory"),_T("EDInBox_prjDbDir"),strDefault);
	gsProjectDBDir.TrimLeft();
	gsProjectDBDir.TrimRight();
	if (gsProjectDBDir.IsEmpty())
	{
		gsProjectDBDir.Format("%s" ,_T("c:\\IPEDprjdb8.0\\"));
		SetRegValue(_T("Directory"),_T("EDInBox_prjDbDir"),gsProjectDBDir);
	}
	if( gsProjectDBDir.Right(1) != "\\" )
	{
		gsProjectDBDir += "\\";
	}
	//��ʱ���ݿ�·��
	strDefault = "";	
	gsProjectDir=GetRegKey(_T("Directory"),_T("EDInBox_prjDir"),strDefault);
	gsProjectDir.TrimLeft();
	gsProjectDir.TrimRight();
	if (gsProjectDir.IsEmpty())
	{
		gsProjectDir.Format("%s" ,_T("c:\\IPEDprj8.0\\"));
		SetRegValue(_T("Directory"),_T("EDInBox_prjDir"),gsProjectDir);
	}
	if( gsProjectDir.Right(1) != "\\" )
	{
		gsProjectDir += "\\";
	}
	//ģ��·��
	strDefault = "";	
	gsTemplateDir=GetRegKey(_T("Directory"),_T("EDinBox_templateDir"),strDefault);
	gsTemplateDir.TrimLeft();
	gsTemplateDir.TrimRight();
	if (gsTemplateDir.IsEmpty())
	{
		gsTemplateDir = gsIPEDInsDir + "Templater\\";
		SetRegValue(_T("Directory"),_T("EDinBox_templateDir"),gsTemplateDir);
	}
	if( gsTemplateDir.Right(1) != "\\" )
	{
		gsTemplateDir += "\\";
	}
	//�����ĵ�·��
	strDefault = "";	
	gsHelpFilePath=GetRegKey(_T("Directory"),_T("HelpFilePath"),strDefault);
	gsHelpFilePath.TrimLeft();
	gsHelpFilePath.TrimRight();
	if (gsHelpFilePath.IsEmpty())
	{
		gsHelpFilePath = gsIPEDInsDir + "Help\\";
		SetRegValue(_T("Directory"),_T("HelpFilePath"),gsHelpFilePath);
	}
	if( gsHelpFilePath.Right(1) != "\\" )
	{
		gsHelpFilePath += "\\";
	}

	//�������ݿ�·��
	// �ŵ���Ŀ���ݿ���  [2005/06/13]
/*	EDIBgbl::strCur_MediumDBPath = GetRegKey(_T("Directory"),_T("EDINBox_MediumDBDir"),"");
	EDIBgbl::strCur_MediumDBPath.TrimLeft();
	EDIBgbl::strCur_MediumDBPath.TrimRight();
	if(EDIBgbl::strCur_MediumDBPath.IsEmpty())
	{
		EDIBgbl::strCur_MediumDBPath = gsProjectDBDir;
		SetRegValue(("Directory"),_T("EDINBox_MediumDBDir"),EDIBgbl::strCur_MediumDBPath);
	}
	if (EDIBgbl::strCur_MediumDBPath.Right(1) != "\\" )
	{
		EDIBgbl::strCur_MediumDBPath += "\\";
	}
*/
	
	/////////////////////////ѡ��Ի����е�״̬���ݡ�
	CString str = GetRegKey(_T("Settings"),_T("CompactDBatClose"),_T(""));
	str.TrimLeft();
	str.TrimRight();
	if(str.IsEmpty())
	{
		SetRegValue(_T("Settings"),_T("CompactDBatClose"),_T("1"));
		str = _T("1");
	}
	if(!str.Compare("1"))
	{
		bIsCloseCompress = TRUE;
	}else 
		bIsCloseCompress = FALSE; 

	str = GetRegKey(_T("Settings"),_T("ReplaceOldMaterialWithNewMaterial"),_T("1"));
	if(!str.Compare("1"))
	{
		bIsReplaceOld = TRUE;
	}else
		bIsReplaceOld = FALSE;

	str = GetRegKey(_T("Settings"),_T("UpdateRecordAfterMove"),_T("1"));
	if(!str.Compare("1"))
	{
		bIsMoveUpdate = TRUE;
	}else
		bIsMoveUpdate = FALSE;

	str = GetRegKey(_T("Settings"),_T("AutoSelectPre"),_T("1"));
	if(!str.Compare("1"))
	{
		bIsAutoSelectPre = TRUE;
	}else
		bIsAutoSelectPre = FALSE;
	
 	//�༭ԭʼ�����ƶ���¼ʱ�ڹܵ����Զ����ӷ���
	str = GetRegKey(_T("Settings"),_T("AutoAddValve"),_T("0"));
	if(!str.Compare("1"))
	{
		bIsAutoAddValve = TRUE;
	}else
		bIsAutoAddValve = FALSE;

	//����ʱ�ж����ɢ���ܶ�
	str = GetRegKey(_T("Settings"),_T("IsHeatLoss"),_T("1"));
	str.TrimLeft();
	str.TrimRight();
	if(!str.Compare("1"))
	{
		bIsHeatLoss = TRUE;
	}else
		bIsHeatLoss = FALSE;
	//����ʱ�Ƿ񵯳�ѡ�񹤳̾��Ի���.
	str = GetRegKey( _T( "Settings" ), _T( "IsRunSelEng" ), _T( "1" ));
	gbIsRunSelectEng = !str.CompareNoCase( "1" ) ? TRUE : FALSE;

 	//�༭ԭʼ���ݣ��Զ�ʱȡ���е��ȼ۱�������
	str = GetRegKey( _T( "Settings" ), _T( "IsSelPrice" ), _T( "0" ));
	gbIsSelTblPrice = !str.CompareNoCase( "1" ) ? TRUE : FALSE;
	
 	//���뱣����������ʱ�滻��λ
	str = GetRegKey( _T( "Settings" ), _T( "IsReplaceUnit" ), _T( "0" ));
	gbIsReplaceUnit = !str.CompareNoCase( "1" ) ? TRUE : FALSE;
	
 	//ͳ������ʱ���Զ����ϱ������ݽ����¶�С��120�ȵļ�¼��
	str = GetRegKey( _T( "Settings" ), _T( "AutoPaint120" ), _T( "1" ));
	gbAutoPaint120 = !str.CompareNoCase( "1" ) ? TRUE : FALSE;

	//���㱣�½ṹ��
	str = GetRegKey(_T("Settings"),_T("CalcInsulStruct"),_T("0"));
	giInsulStruct = _tcstol(str,NULL,10);

	//����ˮ���������ʵı�׼
	str = GetRegKey( _T("Settings"), _T("CalcSteanProp"), _T("0") );
	giCalSteanProp = _tcstol(str, NULL, 10);

 	//���㾭�ú��ʱ��������������Ϸ���
	str = GetRegKey( _T( "Settings" ), _T( "WithoutProtectionCost" ), _T( "0" ));
	gbWithoutProtectionCost = !str.CompareNoCase( "1" ) ? TRUE : FALSE;

 	//˫����ı��¼��㾭�ú��ʱ�ڲ㲻�������¶ȷ�����
	str = GetRegKey( _T( "Settings" ), _T( "InnerByEconomic" ), _T( "0" ));
	gbInnerByEconomic = !str.CompareNoCase( "1" ) ? TRUE : FALSE;

	//��ȡ����IPED.ini�ļ��е����ݿ�����
	EDIBgbl::GetCurDBName();

	return true;
}

BOOL FileExists(LPCTSTR lpszPathName)
{
	DWORD att=::GetFileAttributes(lpszPathName);
	if(att==0xFFFFFFFF || ((att & FILE_ATTRIBUTE_DIRECTORY)!=0) ) return FALSE;
	return TRUE;

}
BOOL DirExists(LPCTSTR lpszDir)
{
	DWORD att=::GetFileAttributes(lpszDir);
	if(att!=0xFFFFFFFF && ((att & FILE_ATTRIBUTE_DIRECTORY)!=0) ) return TRUE;
	return FALSE;
}

//�ж�ָ�����Ƿ����pCon���ݿ��С�
BOOL IsTableExists(_ConnectionPtr pCon, CString tb)
{
	if(pCon==NULL || tb=="")
		return FALSE;
	_RecordsetPtr rs;
	if(tb.Left(1)!="[")
		tb="["+tb;
	if(tb.Right(1)!="]")
		tb+="]";
	try{
		rs=pCon->Execute(_bstr_t(tb), NULL, adCmdTable);
		rs->Close();
		return TRUE;
	}
	catch(_com_error e)
	{
		return FALSE;
	}
}

CString _GetFileName(LPCTSTR lpszPathName)
{
	CString strFile;
	::GetFileTitle(lpszPathName,strFile.GetBuffer(MAX_PATH),MAX_PATH);
	strFile.ReleaseBuffer();
	return strFile;
}

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDApp message handlers


int CAutoIPEDApp::ExitInstance() 
{
	WriteRegedit();
	
	::CoUninitialize();	
	return CWinApp::ExitInstance();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	myURL.SetURL("http://www.uesoft.com");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CAutoIPEDApp::WriteRegedit()
{
	CString strTmp;
	//ѡ��Ի����е�״̬ 
	if(bIsCloseCompress)
	{
		theApp.SetRegValue("Settings", "CompactDBatClose", "1");
	}else
		theApp.SetRegValue("Settings", "CompactDBatClose", "0");

	if(bIsReplaceOld)
	{
		theApp.SetRegValue("Settings", "ReplaceOldMaterialWithNewMaterial", "1");
	}else
		theApp.SetRegValue("Settings", "ReplaceOldMaterialWithNewMaterial", "0");
	
	if(bIsMoveUpdate)
	{
		theApp.SetRegValue("Settings", "UpdateRecordAfterMove", "1");
	}else
		theApp.SetRegValue("Settings", "UpdateRecordAfterMove", "0");

	if(bIsAutoSelectPre)
	{
		theApp.SetRegValue("Settings", "AutoSelectPre", "1");
	}else
		theApp.SetRegValue("Settings", "AutoSelectPre", "0");

 	//�༭ԭʼ�����ƶ���¼ʱ�ڹܵ����Զ����ӷ���
	if (bIsAutoAddValve)
	{
		theApp.SetRegValue("Settings", "AutoAddValve", "1");
	}else
		theApp.SetRegValue("Settings", "AutoAddValve", "0");
 
	//����ʱ�ж����ɢ���ܶ�
	if(bIsHeatLoss)
	{
		theApp.SetRegValue("Settings", "IsHeatLoss", "1");
	}else
		theApp.SetRegValue("Settings", "IsHeatLoss", "0");
	
	//����ʱ�Ƿ񵯳�ѡ�񹤳̾��Ի���
	strTmp = ( gbIsRunSelectEng == 0 ) ? "0" : "1";
	theApp.SetRegValue( "Settings", "IsRunSelEng", strTmp );
	
 	//�༭ԭʼ���ݣ��Զ�ʱȡ���е��ȼ۱�������
	strTmp = ( gbIsSelTblPrice == 0 ) ? "0" : "1";
	theApp.SetRegValue( "Settings", "IsSelPrice", strTmp );

 	// ���뱣����������ʱ�滻��λ
	strTmp = ( gbIsReplaceUnit == 0 ) ? "0" : "1";
	theApp.SetRegValue( "Settings", "IsReplaceUnit", strTmp );

 	// ͳ������ʱ���Զ����ϱ������ݽ����¶�С��120�ȵļ�¼��
	strTmp = ( gbAutoPaint120 == 0 ) ? "0" : "1";
	theApp.SetRegValue( "Settings", "AutoPaint120", strTmp );

 	// ���㾭�ú��ʱ��������������Ϸ��ã�Ĭ��Ϊ0-����
	strTmp = ( gbWithoutProtectionCost == 0 ) ? "0" : "1";
	theApp.SetRegValue( "Settings", "WithoutProtectionCost", strTmp );

 	// ˫����ı��¼��㾭�ú��ʱ�ڲ㲻�������¶ȷ����㣬Ĭ��Ϊ0-�������¶ȷ�����
	strTmp = ( gbInnerByEconomic == 0 ) ? "0" : "1";
	theApp.SetRegValue( "Settings", "InnerByEconomic", strTmp );

	//����ѡ���½ṹ��
	strTmp.Format("%d",giInsulStruct);
	theApp.SetRegValue("Settings", "CalcInsulStruct", strTmp);
	//����ˮ���������ʵı�׼
	strTmp.Format("%d", giCalSteanProp );
	theApp.SetRegValue("Settings", "CalcSteanProp", strTmp);
	
	return true;
}

/////////////////////////////////////////////////////
//
// �������ݿ��ļ���ģ��Ŀ¼������Ŀ¼
//
void CAutoIPEDApp::CopyFromTemplateDirToPrjDir()
{
	CAplicatioInitDlg InitializeDlg;

	InitializeDlg.DoModal();
	
}

////////////////////////////////////////
//
// �ж����ݿ����Ƿ�û��һ������
//
// ���һ������Ҳû�н�����TRUE�����򷵻�FALSE
//
// throw(_com_error)
//
BOOL CAutoIPEDApp::IsNoOneEngin()
{
	_RecordsetPtr IRecordset;
	HRESULT hr;

	if(this->m_pConnection==NULL)
	{
		_com_error e(E_POINTER);
		ReportExceptionErrorLV2(e);
		throw e;
	}

	hr=IRecordset.CreateInstance(__uuidof(Recordset));

	if(FAILED(hr))
	{
		_com_error e(hr);
		ReportExceptionErrorLV2(e);
		throw e;
	}

	try
	{
		IRecordset->Open(_variant_t("Engin"),_variant_t((IDispatch*)m_pConAllPrj),
						 adOpenStatic,adLockOptimistic,adCmdTable);


		if(IRecordset->adoEOF && IRecordset->BOF)
		{
			return TRUE;
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV2(e);
		throw;
	}

	return FALSE;
}

void CAutoIPEDApp::OnFileNew() 
{
	CWinApp::OnFileNew();

	CDocument *pDocument;

	pDocument=((CFrameWnd*)AfxGetMainWnd())->GetActiveDocument();

	if(pDocument==NULL)
		return;

	((CAutoIPEDDoc*)pDocument)->m_Result=_T("");

	((CAutoIPEDDoc*)pDocument)->UpdateAllViews(NULL);

	((CMainFrame*)::AfxGetMainWnd())->ShowCurrentProjectName();
}

/////////////////////////////////////////////////////
//
// ��Ӧ���滻��ǰ���������оɵĲ������ơ�
//
void CAutoIPEDApp::OnReplaceCurrentOldnameNewname() 
{
	CMainFrame *pMainFrame;
	
	pMainFrame=(CMainFrame*)AfxGetMainWnd();

	pMainFrame->m_wndStatusBar.GetProgressCtrl().SetRange(0,100);

//	pMainFrame->m_wndStatusBar.OnProgress(10);
}

void CAutoIPEDApp::OnUpdateReplaceCurrentOldnameNewname(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

BOOL CAutoIPEDApp::InitializeProjectTable( const CString& strPrjID, const CString& strVolCode, 
										  const CString& strPrjName )
{
	newproject dlg;

	dlg.m_engin = EDIBgbl::SelPrjName  = strPrjName;
	dlg.m_eng_code = EDIBgbl::SelPrjID = strPrjID; 
	EDIBgbl::SelVlmCODE = strVolCode;
	dlg.SetAuto( true );

	return dlg.insertitem();
}