// FrmFolderLocation.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "FrmFolderLocation.h"
#include "BrowseForFolerModule.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "EDIBgbl.h"
/////////////////////////////////////////////////////////////////////////////
// CFrmFolderLocation dialog
//
CString gstrEDIBdir[4];

CFrmFolderLocation::CFrmFolderLocation(CWnd* pParent /*=NULL*/)
	: CDialog(CFrmFolderLocation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFrmFolderLocation)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFrmFolderLocation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrmFolderLocation)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFrmFolderLocation, CDialog)
	//{{AFX_MSG_MAP(CFrmFolderLocation)
	ON_BN_CLICKED(IDC_SF1, OnSf1)
	ON_BN_CLICKED(IDC_SF2, OnSf2)
	ON_BN_CLICKED(IDC_SF3, OnSf3)
	ON_BN_CLICKED(IDC_SF4, OnSf4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrmFolderLocation message handlers

BOOL CFrmFolderLocation::OnInitDialog() 
{
	CDialog::OnInitDialog();
    SetWindowCenter(this->m_hWnd);
	CString strDir;
	LoadCaption();

	InitgstrEDIBdir();
	CString strCmp;
	for (int i=0;i<4;i++)      //���ñ����·����
	{
		strCmp.LoadString(IDS_EDIBDir0+i);
		SetDlgItemText(IDC_DIR1+i, strCmp);

		strDir = GetDir(gstrEDIBdir[i]);
		while(strDir.Right(1)=="\\")     //��������'\'
		{
			strDir.Delete(strDir.GetLength()-1);
		}
		SetDlgItemText(IDC_FL1+i, strDir);
		m_strFormDir[i] = strDir;    //����ԭ����·������
	}

    //iEDIBinstallDir=6
	((CEdit*)GetDlgItem(IDC_FL1))->SetReadOnly();
	GetDlgItem(IDC_SF1)->EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//���öԻ�����⡣
void CFrmFolderLocation::LoadCaption()
{
	CString str;
	str.LoadString(IDS_frmFolderLocation_frmFolderLocation);
	SetWindowText(str);

	str.LoadString(IDS_WarnDontRandomEDITthisDirectoryString);
	SetDlgItemText(IDC_CAUTION,str);

}
//����λ��
void CFrmFolderLocation::SetWindowCenter(HWND hWnd)
{
	long SW,SH;
	if(hWnd==NULL)
		return;
	SW=::GetSystemMetrics(SM_CXSCREEN);
	SH=::GetSystemMetrics(SM_CYSCREEN);
	CRect rc;
	::GetWindowRect(hWnd,&rc);
	long x=(SW-rc.Width()) / 2;
	long y=(SH-rc.Height()) / 2;
	::SetWindowPos(hWnd,NULL,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
}
//��ʼ����������ע����е�·���ģ�
void CFrmFolderLocation::InitgstrEDIBdir()
{
	gstrEDIBdir[0] = "EDInBox_InsDir";				//Ӧ�ó���Ŀ¼��

	gstrEDIBdir[1] = "EDInBox_TemplateDir";			//ģ���ļ�Ŀ¼

	gstrEDIBdir[2] = "EDInBox_prjDbDir";              //��Ŀ���ݿ���Ŀ¼��

    gstrEDIBdir[3] = "EDInBox_prjDir";				//��Ŀ��ʱ���ݿ���Ŀ¼��

}

CString CFrmFolderLocation::GetDir(CString key)
{
	CString sDir("");
	sDir=GetRegKey(_T("directory"),key,CString(_T("")));
	sDir.MakeLower();
	return sDir;

}

CString CFrmFolderLocation::GetRegKey(LPCTSTR pszKey, LPCTSTR pszName, const CString Default)
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
	ret.TrimLeft();ret.TrimRight();
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
	//ret= Default;
	RegCloseKey(hKey);
	return ret;

}

void CFrmFolderLocation::SetRegValue(LPCTSTR pszKey, LPCTSTR pszName, const CString vValue)
{
		HKEY hKey;
	CString subKey=szSoftwareKey;
	subKey+=pszKey;
	if(RegOpenKey(HKEY_LOCAL_MACHINE,subKey,&hKey)!=ERROR_SUCCESS)
	{
		RegCreateKey(HKEY_LOCAL_MACHINE,subKey,&hKey);
	}
	if(RegSetValueEx(hKey,pszName,NULL,REG_SZ,(LPBYTE)(LPCTSTR)vValue,(vValue.GetLength()+1)*sizeof(TCHAR))!=ERROR_SUCCESS)
	{
		if(::RegDeleteValue(hKey,pszName)==ERROR_SUCCESS)
			RegSetValueEx(hKey,pszName,NULL,REG_SZ,(LPBYTE)(LPCTSTR)vValue,(vValue.GetLength()+1)*sizeof(TCHAR));
	}
	RegCloseKey(hKey);

}
//ѡ��Ի���
void CFrmFolderLocation::OnSf(UINT uID)
{
	CString strStart, strSelect;
	CFrmFolderLocation::GetDlgItemText(uID, strStart);
	while(strStart.Right(1)=="\\")
	{
		strStart.Delete(strStart.GetLength()-1);
	}
	BrowseForFolerModule::BrowseForFoldersFromPathStart(this->GetSafeHwnd(),
		           strStart, strSelect);
	if(!strSelect.IsEmpty())
	{
		CFrmFolderLocation::SetDlgItemText(uID, strSelect);
	}

}

void CFrmFolderLocation::OnSf1() 
{
	OnSf(IDC_FL1);	
}

void CFrmFolderLocation::OnSf2() 
{
	OnSf(IDC_FL2);	
	
}

void CFrmFolderLocation::OnSf3() 
{
	OnSf(IDC_FL3);	
	
}

void CFrmFolderLocation::OnSf4() 
{
	OnSf(IDC_FL4);	
	
}


void CFrmFolderLocation::OnOK() 
{
	CString strCap, strMess, str;
	bool PrjDBDirChanged = false;//��Ŀ���ݿ�Ŀ¼/����Ŀ��ʱ���ݿ�Ŀ¼�� ���ı䣬��Ҫ����
	bool PrjXDirChanged = false;
	for(int i=0; i<4; i++)
	{
		CFrmFolderLocation::GetDlgItemText(IDC_FL1+i, strCap);
		strCap.TrimLeft( );
		strCap.TrimRight( );
		while(strCap.Right(1)=="\\" || (!strCap.IsEmpty() && strCap.Right(1)==" "))       
		{
			strCap.Delete(strCap.GetLength()-1);
		}

		if( !IsDirExists(strCap + "\\") )//�ж��ļ����Ƿ���ڡ�
		{
			strMess.Format("' %s '����һ����Ч��·�������������ã�",strCap);
			::MessageBox(NULL, strMess, "AutoIPED", MB_OK|MB_ICONEXCLAMATION|MB_SYSTEMMODAL);
			this->BringWindowToTop();
			return;
		}
		if( strCap.IsEmpty() )
		{
			str.LoadString(IDS_EDIBDir0+i);
			strMess = ("'"+str+"'����Ϊ�գ�");
			AfxMessageBox(strMess);
			return;
		}
		SetRegValue("directory", gstrEDIBdir[i], strCap+"\\");   //д��ע����м�'\'
		
		if( strCap != m_strFormDir[i] )    //ԭ����·���Ƿ�ı䡣
		{
			PrjXDirChanged=true;
			if( i == 2/*prjDBDir*/|| i == 3/*prjDir*/)        //���ݿ�Ŀ¼�ı䡣
			{
				PrjDBDirChanged=true;
			}
		}

	}
	if (PrjXDirChanged)
	{
		if (IDYES==(::MessageBox(this->m_hWnd,_T("��Ҫ��Ŀ¼���ݱ��޸ģ���Ҫ����������"),
			_T("AutoIPED"),MB_YESNO|MB_ICONEXCLAMATION|MB_SYSTEMMODAL)))
		{
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_bIsExitMsg=false;
			//��·���µ����ݿ������Ҫ������
			if (PrjDBDirChanged)
			{
				SetRegValue(_T("Status"), _T("Install"), CString(_T("0")));
			}
			::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);

		}
	}

	CDialog::OnOK();
}

bool CFrmFolderLocation::IsDirExists(CString Directory)
{
	DWORD code;
	code = GetFileAttributes(Directory);
	if(code!=-1 && (code & FILE_ATTRIBUTE_DIRECTORY)!=0)
		return TRUE;
	return FALSE;
}
