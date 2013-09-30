// ImportFromXLSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "ImportFromXLSDlg.h"
#include "vtot.h"
#include "projectoperate.h"
#include "importautopd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportFromXLSDlg dialog

extern CAutoIPEDApp theApp;

CImportFromXLSDlg::CImportFromXLSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportFromXLSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportFromXLSDlg)
	m_XLSFilePath = _T("");
	m_HintInformation = _T("");
	//}}AFX_DATA_INIT

	m_strItemTblName = _T("");
	m_strTitleName = _T("");
	//����Ĭ�ϵ��Ӽ�������
	m_RegSubKey=_T("ImportFromXLSDlg");
}

CImportFromXLSDlg::~CImportFromXLSDlg()
{
	CPropertyWnd::ElementDef Element;

	//
	// �ͷ����ڱ�����ʾ��Ϣ���ڴ�
	//

	for(int i=0;i<this->m_PropertyWnd.GetElementCount();i++)
	{
		this->m_PropertyWnd.GetElementAt(&Element,i);

		if(Element.pVoid)
		{
			delete ((CString*)Element.pVoid);
			Element.pVoid=NULL;
		}
	}
}

void CImportFromXLSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportFromXLSDlg)
	DDX_Control(pDX, IDC_OPEN_FILEDLG, m_OpenFileDlgButton);
	DDX_Text(pDX, IDC_FILE_FULLPATH, m_XLSFilePath);
	DDX_Text(pDX, IDC_HINT_INFORMATION, m_HintInformation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportFromXLSDlg, CDialog)
	//{{AFX_MSG_MAP(CImportFromXLSDlg)
	ON_BN_CLICKED(IDC_OPEN_FILEDLG, OnOpenFiledlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BEGIN_IMPORT, OnBeginImport)
	ON_BN_CLICKED(IDC_SET_DEFAULT_NO, OnSetDefaultNo)
	//}}AFX_MSG_MAP
	ON_NOTIFY(PWN_SELECTCHANGE,IDC_PROPERTY_WND, OnSelectChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportFromXLSDlg message handlers

BOOL CImportFromXLSDlg::OnInitDialog() 
{
	if ( m_pConIpedSort == NULL )
	{
		AfxMessageBox(" û���������ݿ����� ��");
		return FALSE;
	}
	CBitmap Bitmap;

	CDialog::OnInitDialog();

	Bitmap.LoadBitmap(IDB_OPENFILE);
	this->m_OpenFileDlgButton.SetBitmap((HBITMAP)Bitmap.Detach());


	CRect Rect;

	//
	// �����ص�Group�ؼ�����λ���Կؼ�
	//
	GetDlgItem(IDC_PROP_WND_RECT)->GetWindowRect(&Rect);

	this->ScreenToClient(&Rect);

	m_PropertyWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP,Rect,this,IDC_PROPERTY_WND);

	//��ʼ�����Կؼ�
	InitPropertyWnd();
	
	//��ע����ó�ʼ��Ϣ
	InitFromReg();

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////
//
// ��Ӧ��ѡ���ļ���ť��
//
void CImportFromXLSDlg::OnOpenFiledlg() 
{
	BOOL bRet;
	LPDISPATCH pDispatch;
	_Application Application;
	Workbooks workbooks;
	_Workbook workbook;
	Worksheets worksheets;
	_Worksheet worksheet;

	if( m_strPrecFilePath.IsEmpty() )
	{
		m_strPrecFilePath = theApp.GetRegKey(_T("Directory"),
			_T("EDinBox_templateDir"), CString(_T("")));		//Ĭ��Ϊģ���ļ��е�·��.
	}

	CFileDialog  FileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						 _T("Microsoft Excel �ļ� (*.xls)|*.xls|All Files (*.*)|*.*||"));

	FileDlg.m_ofn.lpstrInitialDir = m_strPrecFilePath;			 //05/1/4
	// ���ѡ����XLS�ļ��ʹ���������ù�������
	//
	if(FileDlg.DoModal()==IDOK)
	{
		CWaitCursor WaitCursor;

		this->m_XLSFilePath=FileDlg.GetPathName();
		this->UpdateData(FALSE);

		bRet=Application.CreateDispatch(_T("Excel.Application"));

		if(!bRet)
		{
			ReportExceptionErrorLV1(_T("���Ȱ�װExcel!"));
			return;
		}

		//��ù������ļ���
		pDispatch=Application.GetWorkbooks();
		workbooks.AttachDispatch(pDispatch);

		try
		{
			//��ָ����EXCEL�ļ�
			pDispatch=workbooks.Open(this->m_XLSFilePath,vtMissing,vtMissing,vtMissing,vtMissing,
									 vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,
									 vtMissing);
			workbook.AttachDispatch(pDispatch);

			//���ָ��������
			pDispatch=workbook.GetWorksheets();
			worksheets.AttachDispatch(pDispatch);

			long iCount;
			int iRet;
			CString strTemp, strSheetName;
			int iPos;
			CPropertyWnd::ElementDef ElementInfo;	//���Է������Կؼ���Ԫ����Ϣ

			iCount=worksheets.GetCount();


			//����"Excel��������"��Ԫ����Ϣ
			iPos=this->m_PropertyWnd.FindElement(0,_T("Excel��������"));
			this->m_PropertyWnd.CreateElementControl(iPos);
			this->m_PropertyWnd.GetElementAt(&ElementInfo,iPos);
			//��õ�ǰ״̬�Ĺ�������   1/11     
			strSheetName = ElementInfo.RightElementText;
			//

			do
			{
				iRet=((CComboBox*)ElementInfo.pControlWnd)->DeleteString(0);
			}while(iRet!=CB_ERR);

			for(long i=1;i<=iCount;i++)
			{
				pDispatch=worksheets.GetItem(_variant_t(i));
				worksheet.AttachDispatch(pDispatch);

				strTemp=worksheet.GetName();

				worksheet.ReleaseDispatch();

				((CComboBox*)ElementInfo.pControlWnd)->AddString(strTemp);
			}
			///////���ָ���Ĺ�������      05/1/4	

			int pos = ((CComboBox*)ElementInfo.pControlWnd)->FindString(0, strSheetName);
			pos = (pos>0)?pos:0;
			((CComboBox*)ElementInfo.pControlWnd)->SetCurSel(pos);
			//////05/1/4

			//�ػ����Կؼ��Ĵ���
			this->m_PropertyWnd.Invalidate();
		}
		catch(COleDispatchException *e)
		{
			Application.Quit();

			ReportExceptionErrorLV2(e);
			throw;
		}
		catch(COleException *e)
		{
			Application.Quit();

			_com_error e2(e->m_sc);
			e->Delete();
			ReportExceptionErrorLV2(e2);
			throw e2;
		}

		Application.Quit();
		///��ǰ��05/1/4
		m_strPrecFilePath = m_XLSFilePath;
	}
}

void CImportFromXLSDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	//�رմ���ʱ�������ݵ�ע���
	SaveSetToReg();
}

//////////////////////////////////////////////////////////////

///////////////////////////////////////////
//
// ��ʼ�����Դ���
//
// �����ɹ�����TRUE�����򷵻�FALSE
//
// �˺���Ӧ����WM_INITDIALOG��Ϣ�е���
//
BOOL CImportFromXLSDlg::InitPropertyWnd()
{
	//
	// ��ʼ�����Կؼ�
	//
	CPropertyWnd::ElementDef ElementDef;

	struct
	{
		LPCTSTR ElementName;	// ��Ԫ������
		UINT	Style;			// ��ʾ��ʽ
		LPCTSTR HintInfo;		// �뵥Ԫ��ص���ʾ��Ϣ
	}ElementSet[]=
	{
		{_T("Excel"),			CPropertyWnd::TitleElement,NULL},								//0

		{_T("Excel��������"),	CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ�����ű������")},	//1

		{_T("�������ݿ�ʼ�к�"),CPropertyWnd::ChildElement|CPropertyWnd::EditElement,NULL},		//2
		{_T("�������������"),	CPropertyWnd::ChildElement|CPropertyWnd::EditElement,NULL},		//3
	};


	for(int i=0;i<sizeof(ElementSet)/sizeof(ElementSet[0]);i++)
	{
		ElementDef.szElementName=ElementSet[i].ElementName;
		ElementDef.Style=ElementSet[i].Style;

		ElementDef.pVoid=NULL;

		if(ElementSet[i].HintInfo!=NULL)
		{			
			ElementDef.pVoid=new CString(ElementSet[i].HintInfo);
		}

		m_PropertyWnd.InsertElement(&ElementDef);

	}

	m_PropertyWnd.RefreshData();

	if ( !m_strItemTblName.IsEmpty() )
	{	
		// �����ݱ���ȡ������������̬���ӵ�������
		AddPropertyWndFromTbl( m_strTitleName, m_strItemTblName );
	}
	return TRUE;
}

///////////////////////////////////////////
//
// ��ע��������ݳ�ʼ��
//
void CImportFromXLSDlg::InitFromReg()
{
	LONG lRet;
	DWORD dwDisposition;
	HKEY hKey;
	CString SubKey;
	CPropertyWnd::ElementDef ElementDef;
	TCHAR szTemp[256];
	DWORD dwData;

	SubKey=szSoftwareKey;
	SubKey+=m_RegSubKey;

	//��ע���
	lRet=::RegCreateKeyEx(HKEY_LOCAL_MACHINE,SubKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_READ,NULL,&hKey,&dwDisposition);
	
	if(lRet!=ERROR_SUCCESS)
	{
		ReportExceptionErrorLV1(_T("��ע���ʧ��"));
		return;
	}
	// ��ע��������ݷ������Կؼ�
	//
//	for(int i=0;i<this->m_PropertyWnd.GetElementCount();i++)
	int nPos = this->m_PropertyWnd.FindElement(0, "�������������");
	if ( nPos < 0 )
		nPos = 3;
	
	for( int i = 0; i <= nPos; i++ )
	{
		this->m_PropertyWnd.CreateElementControl(i);
		this->m_PropertyWnd.GetElementAt(&ElementDef,i);

		dwData=256;

		lRet=RegQueryValueEx(hKey,ElementDef.szElementName,NULL,NULL,(BYTE*)szTemp,&dwData);

		if(lRet==ERROR_SUCCESS)
		{
			if(ElementDef.pControlWnd && IsWindow(ElementDef.pControlWnd->GetSafeHwnd()))
			{
				if(dwData>0)
					ElementDef.pControlWnd->SetWindowText(szTemp);
			}
		}
	}
	/////////  ��ʼExcel �ļ�·��
	dwData=256;
	lRet=RegQueryValueEx(hKey,"ExcelFilePath",NULL,NULL,(BYTE*)szTemp,&dwData);  

	if(lRet==ERROR_SUCCESS)
	{
		m_strPrecFilePath = szTemp;
		m_XLSFilePath = m_strPrecFilePath;
	}
	//////
	::RegCloseKey(hKey);
}

////////////////////////////////////////////
//
// ����Ϣ���浽ע�����
//
void CImportFromXLSDlg::SaveSetToReg()
{
	UpdateData();
	
	LONG lRet;
	DWORD dwDisposition;
	HKEY hKey;
	CString SubKey;
	CPropertyWnd::ElementDef ElementDef;

	SubKey=szSoftwareKey;
	SubKey+=m_RegSubKey;

	// ��ע���
	lRet=::RegCreateKeyEx(HKEY_LOCAL_MACHINE,SubKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hKey,&dwDisposition);
	
	if(lRet!=ERROR_SUCCESS)
	{
		ReportExceptionErrorLV1(_T("��ע���ʧ��"));
		return;
	}

	//
	// �����Կؼ������ݴ���ע���
	//
//	for(int i=0;i<this->m_PropertyWnd.GetElementCount();i++)
	int nPos = this->m_PropertyWnd.FindElement(0, "�������������");
	if ( nPos < 0 )
		nPos = 3;

	for( int i = 0; i <= nPos; i++ )
	{
		this->m_PropertyWnd.GetElementAt(&ElementDef,i);

		if(!ElementDef.RightElementText.IsEmpty())
		{
			lRet=RegSetValueEx(hKey,ElementDef.szElementName,NULL,REG_SZ,(BYTE*)(LPCTSTR)ElementDef.RightElementText,
						  strlen(ElementDef.RightElementText)+sizeof(TCHAR));
		}
		else
		{
			lRet=RegSetValueEx(hKey,ElementDef.szElementName,NULL,REG_SZ,(BYTE*)NULL,
						  0);
		}
	}
	///////  ����·��
	if ( !m_XLSFilePath.IsEmpty() )
	{
		lRet=RegSetValueEx(hKey,"ExcelFilePath",NULL,REG_SZ,(BYTE*)(LPCTSTR)m_strPrecFilePath,
				  strlen(m_XLSFilePath)+sizeof(TCHAR));
	}
	//////
	::RegCloseKey(hKey);
	
	if ( !m_strItemTblName.IsEmpty() )
	{
		// ���ӿؼ���Ӧ��EXCEL�к�д�뵽���ݿ���
		WriteExcelNoToTbl( m_strItemTblName );
	}
}


/////////////////////////////////////////////////////////////////////
//
// ��Ӧ���Կؼ�ѡ�е�֪ͨ
//
void CImportFromXLSDlg::OnSelectChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPropertyWnd::PWNSelectChangeStruct *pSelect;

	pSelect=(CPropertyWnd::PWNSelectChangeStruct*)pNMHDR;

	this->m_HintInformation=(pSelect->szElementName);

	if(pSelect->pVoid)
	{
		this->m_HintInformation+=_T(":\r\n");

		this->m_HintInformation+=*((CString*)pSelect->pVoid);
	}

	this->UpdateData(FALSE);
	*pResult=0;
}

///////////////////////////////////////////
//
// ��Ӧ��ʼ�����롰
//
void CImportFromXLSDlg::OnBeginImport() 
{
	BOOL bRet;

	this->UpdateData();

	// ��֤�������� �Ƿ���Ч
	bRet=ValidateData();

	if(!bRet)
	{
		return;
	}

	BeginImport();

	// �Ե���ļ�¼���д���
	CImportAutoPD::RefreshData( m_strProjectID );
}

/////////////////////////////////////////////
//
// ��֤�������ݵ���Ч��
//
BOOL CImportFromXLSDlg::ValidateData()
{
	CPropertyWnd::ElementDef ElementDef;
	CString strTemp;

	this->UpdateData();

	if(GetProjectID().IsEmpty())
	{
		ReportExceptionErrorLV1(_T("δѡ�񹤳�"));

		return FALSE;
	}

	//�ж��ļ�·���Ƿ�Ϊ��
	if(this->m_XLSFilePath.IsEmpty())
	{
		ReportExceptionErrorLV1(_T("�ļ�·������Ϊ��"));
		return FALSE;
	}

	//
	// ������Կؼ���ָ���ĵ�Ԫ�Ƿ�Ϊ��
	//
	for(int i=0;i<this->m_PropertyWnd.GetElementCount();i++)
	{
		LPCTSTR szTableName[]=
		{
			_T("Excel��������"),
			_T("�������ݿ�ʼ�к�"),
			_T("�������������"),
		};

		this->m_PropertyWnd.GetElementAt(&ElementDef,i);

		for(int j=0;j<sizeof(szTableName)/sizeof(szTableName[0]);j++)
		{
			if(ElementDef.szElementName==szTableName[j] && ElementDef.RightElementText.IsEmpty())
			{
				strTemp.Format(_T("%s����Ϊ��"),szTableName[j]); 
				ReportExceptionErrorLV1(strTemp); 
				return FALSE; 
			}
		}

	}

	return TRUE;
}

//////////////////////////////////////////////////////////
//
// ���ù��̱��
//
// szProjectID[in]	���̱��
//
void CImportFromXLSDlg::SetProjectID(LPCTSTR szProjectID)
{
	if(szProjectID==NULL)
	{
		m_strProjectID=_T("");
	}
	else
	{
		m_strProjectID=szProjectID;
	}
}

//////////////////////////////////////////////////////
//
// ���ع��̵ı��
//
CString CImportFromXLSDlg::GetProjectID()
{
	return m_strProjectID;
}

//------------------------------------------------------------------
// DATE         :[2005/12/27]
// Parameter(s) :���ݿ������
// Return       :
// Remark       :���ÿ������Ե����ݿ�
//------------------------------------------------------------------
void CImportFromXLSDlg::SetIPEDSortDBConnect(_ConnectionPtr pConSort)
{
	this->m_pConIpedSort = pConSort;
}

//------------------------------------------------------------------
// DATE         :[2005/12/27]
// Parameter(s) :
// Return       :
// Remark       :��ÿ������Ե����ݿ�
//------------------------------------------------------------------
_ConnectionPtr CImportFromXLSDlg::GetIPEDSortDBConnect()
{
	return this->m_pConIpedSort;
}

//////////////////////////////////////////////////////////////////////
//
// �����빤��������ݿ������
//
// IConnection[in]	���ݿ������
//
void CImportFromXLSDlg::SetProjectDbConnect(_ConnectionPtr IConnection)
{
	this->m_ProjectDbConnect=IConnection;
}

////////////////////////////////////////////////////////////////////
//
// ���������ݿ������
//
_ConnectionPtr CImportFromXLSDlg::GetProjectDbConnect()
{
	return this->m_ProjectDbConnect;
}

//------------------------------------------------------------------
// DATE         :[2005/12/28]
// Parameter(s) :lpTitleName[in] �ӿؼ��ı���, lpTblName[in] �����ӿؼ��ı���
// Return       :
// Remark       :�����ӿؼ��ı���� �����ӿؼ��ı���
//------------------------------------------------------------------
void CImportFromXLSDlg::SetItemTblName(LPCTSTR lpTitleName, LPCTSTR lpTblName)
{
	m_strTitleName = lpTitleName;
	m_strItemTblName = lpTblName;
}

//------------------------------------------------------------------
// DATE         :[2005/12/29]
// Parameter(s) :
// Return       :
// Remark       :�����Ŀ�����ݱ���
//------------------------------------------------------------------
void CImportFromXLSDlg::SetDataTblName(LPCTSTR lpDataTblName)
{
	m_strDataTblName = lpDataTblName;
}

///////////////////////////////////////////////////////////
//
// �������ڴ����Ϣ���Ӽ���
//
// szRegSubKey[in]	���ڴ����Ϣ���Ӽ���
//
void CImportFromXLSDlg::SetRegSubKey(LPCTSTR szRegSubKey)
{
	if(szRegSubKey)
		m_RegSubKey = szRegSubKey;
}

////////////////////////////////////////////////////////
//
// �������ڴ����Ϣ���Ӽ���
//
CString CImportFromXLSDlg::GetRegSubKey()
{
	return m_RegSubKey;
}

///////////////////////////////////////////////////////////
//
// �������ڶԻ����²���ʾ��Ϣ����ʾ������
//
// szHint[in]	�ڶԻ����²���ʾ��Ϣ����ʾ������
//
void CImportFromXLSDlg::SetHintInformation(LPCTSTR szHint)
{
	if(szHint)
		m_HintInformation=szHint;
	else
		m_HintInformation=_T("");

	if(IsWindow(GetSafeHwnd()))
	{
		UpdateData(FALSE);
	}
}

////////////////////////////////////////////////////////
//
// �������ڶԻ����²���ʾ��Ϣ����ʾ������
//
CString CImportFromXLSDlg::GetHintInformation()
{
	return m_HintInformation;
}

////////////////////////////////////////////////////////
//
// ������Կؼ��Ĵ���
//
CPropertyWnd* CImportFromXLSDlg::GetPropertyWnd()
{
	return &m_PropertyWnd;
}

//////////////////////////////////////////////////
//
// ִ�е������ݵĲ���
//
void CImportFromXLSDlg::BeginImport()
{
	int iPos;
	CPropertyWnd::ElementDef ElementDef;
	CProjectOperate::ImportFromXLSStruct ImportTable;
	CProjectOperate::ImportFromXLSElement ImportTableItem[60];
	int TableItemCount;
	CProjectOperate Import;

	CWaitCursor WaitCursor;
	BOOL	bIsAutoNO = FALSE;		// �Լ�¼�Զ����

	this->m_HintInformation=_T("��ʼ��������");
	this->UpdateData(FALSE);
	this->UpdateWindow();

	Import.SetProjectDbConnect(this->GetProjectDbConnect());

	//
	// ��дImportFromXLSStruct�ṹ��Ϣ
	//
	if ( m_strDataTblName.IsEmpty() )
	{
		AfxMessageBox("��ָ�����ݱ�����");
		return ;
	}
	ImportTable.ProjectDBTableName = m_strDataTblName;

	ImportTable.ProjectID=this->GetProjectID();

	ImportTable.XLSFilePath=this->m_XLSFilePath;

	iPos=this->m_PropertyWnd.FindElement(0,_T("�������ݿ�ʼ�к�"));
	this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
	ImportTable.BeginRow=_ttoi(ElementDef.RightElementText);

	iPos=this->m_PropertyWnd.FindElement(0,_T("Excel��������"));
	this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
	ImportTable.XLSTableName=ElementDef.RightElementText;

	iPos=this->m_PropertyWnd.FindElement(0,_T("�������������"));
	this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
	ImportTable.RowCount=_ttoi(ElementDef.RightElementText);
	//
	// �����Կؼ��е���������ImportTableItem�ṹ
	//
	iPos++;
	TableItemCount = 0;
	while(TRUE)
	{
		if(iPos>=this->m_PropertyWnd.GetElementCount())
			break;

		this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
		iPos++;

		if ( ElementDef.szElementName == _T("���") && ElementDef.RightElementText == _T("�Զ�") )
		{
			bIsAutoNO = TRUE;
			continue;
		}
		
		// ���ҵ�Ԫ��Ϊ��ʱ������
		if( !ElementDef.RightElementText.IsEmpty() )
		{
			ImportTableItem[TableItemCount].SourceFieldName = ElementDef.RightElementText;
			ImportTableItem[TableItemCount].DestinationName = ElementDef.strFieldName;
			TableItemCount++;
		}
	}

	ImportTable.pElement=ImportTableItem;
	ImportTable.ElementNum=TableItemCount;
	try
	{
		//
		// ����ڡ���š������ˡ��Զ�����ID���Զ���д
		// ����һ����û��ID�ֶ�,����������ĺ���
		//Import.ImportPre_CalcFromXLS( &ImportTable, bIsAutoNO );
		Import.ImportTblEnginIDXLS( &ImportTable, bIsAutoNO );
	}
	catch(_com_error &e)
	{
		this->m_HintInformation=_T("���ݵ���ʧ��");
		this->UpdateData(FALSE);
		ReportExceptionErrorLV1(e);		
		return;
	}
	catch(COleDispatchException *e)
	{
		this->m_HintInformation=_T("���ݵ���ʧ��");
		this->UpdateData(FALSE);
		ReportExceptionErrorLV1(e);
		e->Delete();
		return;
	}

	this->m_HintInformation=_T("���ݵ���ɹ�");
	this->UpdateData(FALSE);
}

//------------------------------------------------------------------
// DATE         :[2005/12/27]
// Parameter(s) :const CString strTblName, ��IPEDsort.mdb���з�Ų�ͬ���Եı���
// Return       :���óɹ�����TRUE�����򷵻�FALSE
// Remark       :�����ݱ���ȡ������������̬���ӵ�������
//------------------------------------------------------------------
BOOL CImportFromXLSDlg::AddPropertyWndFromTbl(LPCTSTR lpTitleName, const CString strTblName)
{
	if ( strTblName.IsEmpty() )
		return	FALSE;

	CString strSQL;		// SQL���	
	CString strTmp;
	_RecordsetPtr pRsInfo;		// �������Եļ�¼��
	CPropertyWnd::ElementDef ElementDef;	// �ӿؼ�
	CHAR ColumeName[10];
	int  iPos;
	pRsInfo.CreateInstance( __uuidof( Recordset ) );
	EDIBgbl::CAPTION2FIELD * pAllFieldStruct = NULL;	// �Ӵ��ڵ���������
	int nWndCount = GetField2Caption( pAllFieldStruct, strTblName );	// �Ӵ��ڵ����Ը���
	if ( nWndCount <= 0 )
	{
		return FALSE;
	}
	// ���ȴ����ӿؼ��ı���
	if ( NULL != lpTitleName )
	{
		ElementDef.szElementName = lpTitleName;
		ElementDef.Style = CPropertyWnd::TitleElement;
		this->m_PropertyWnd.InsertElement( &ElementDef );
	}
	//�����ӿؼ�
	for ( int i = 0; i < nWndCount; i++ )
	{
		//ԭʼ���ݱ��ж�Ӧ���ֶ�����		
		if ( pAllFieldStruct[i].strField.IsEmpty() )
			continue;
		ElementDef.strFieldName = pAllFieldStruct[i].strField;

		//�ֶ�����
		if ( pAllFieldStruct[i].strCaption.IsEmpty() )
			continue;
		ElementDef.szElementName = pAllFieldStruct[i].strCaption;
		//��ʾ��Ϣ
		if ( pAllFieldStruct[i].strRemark.IsEmpty() )
			strTmp.Format(IDS_IMPORT_ITEM_REMARK, pAllFieldStruct[i].strCaption);
		else
			strTmp = pAllFieldStruct[i].strRemark;
		ElementDef.pVoid = new CString(strTmp);
		//���
		ElementDef.Style = CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement;			
		
		//Ĭ�ϵ�EXCEL�к�
		ElementDef.RightElementText = pAllFieldStruct[i].strExcelNO;
		
		//����һ��
		iPos = m_PropertyWnd.InsertElement(&ElementDef);
		//�����ӿؼ�
		if ( m_PropertyWnd.CreateElementControl(iPos) )
		{
			this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);

			if ( !ElementDef.szElementName.Compare("���") )
			{
				((CComboBox*)ElementDef.pControlWnd)->AddString("�Զ�");
			}
			
			//�ұ��ṩѡ��EXCEL�к�
			for(int k = 0; k < 100; k++)
			{
				int j,m,m2;
				for(j=k,m=1;j/26!=0;m++,j/=26);
				ColumeName[m]=_T('\0');
				m--;
				j=k;
				m2=m;
				while(m>=0)
				{
					ColumeName[m]=j%26+_T('A');					
					j=j/26;					
					if(m2!=m)
						ColumeName[m]--;					
					m--;
				}
				((CComboBox*)ElementDef.pControlWnd)->AddString(ColumeName);
			}	
		}
	}
	// ���±��
	this->m_PropertyWnd.RefreshData();
	
	if ( NULL != pAllFieldStruct )
	{
		delete [] pAllFieldStruct;
	}

	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/12/27]
// Parameter(s) :EDIBgbl::CAPTION2FIELD* &pFieldStruct[out]	�����ݿ���ȡ�����ֶζ�Ӧ��ϵ���ڽṹ�����С�
//				:const CString strTblName[in]	�����ӿؼ��Ľṹ����
//				:const CString strDefField[in]	����ExcelĬ���кŵ��ֶ�����
// Return       :�����ֶθ���
// Remark       :��IPEDsort.mdb��PD2IPED����ȡ��EXCEL�е��ֶ�����ACCESS�е��ֶεĶ�Ӧֵ
//------------------------------------------------------------------
int CImportFromXLSDlg::GetField2Caption(EDIBgbl::CAPTION2FIELD *&pFieldStruct, const CString strTblName, const CString strDefField)
{
	_RecordsetPtr pRs;
	CString strSQL;
	int		nFieldCount;	//�ֶεĸ���    
	pRs.CreateInstance(__uuidof(Recordset));
	try
	{        
		strSQL = " SELECT * FROM ["+strTblName+"] WHERE SEQ IS NOT NULL ORDER BY SEQ";
		
		pRs->Open(_variant_t(strSQL), m_pConIpedSort.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		nFieldCount = pRs->GetRecordCount();
		if (nFieldCount <= 0)
		{
			return FALSE;
		} 
		pFieldStruct = new EDIBgbl::CAPTION2FIELD[nFieldCount];
		for (int i=0; !pRs->adoEOF && i < nFieldCount; pRs->MoveNext(),i++)
		{
			pFieldStruct[i].strCaption = vtos(pRs->GetCollect("LocalCaption"));		//EXCEL�е��ֶ�����
			pFieldStruct[i].strField   = vtos(pRs->GetCollect("FieldName"));		//ACCESS�е��ֶ�����
			pFieldStruct[i].strExcelNO = vtos(pRs->GetCollect(_variant_t(strDefField)));		// ��ӦExcel���к�
			pFieldStruct[i].strRemark  = vtos(pRs->GetCollect("Remark"));			// ��ע��Ϣ
			pFieldStruct[i].nFieldType = vtoi(pRs->GetCollect("FieldType"));		//�ֶ�����
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return nFieldCount;
}
//------------------------------------------------------------------
// DATE         :[2005/12/28]
// Parameter(s) :���ݿ�ı���
// Return       :
// Remark       :����ǰ��Ӧ��Excel�к�,д�뵽���ݿ���
//------------------------------------------------------------------
BOOL CImportFromXLSDlg::WriteExcelNoToTbl(const CString strTblName)
{
	if ( strTblName.IsEmpty() )
		return FALSE;
	CString strSQL;			// SQL���
	CString strTmp;			// ��ʱ�ַ���
	int iPos;
	CPropertyWnd::ElementDef ElementDef;
	_RecordsetPtr pRsInfo;	// �������Եļ�¼��
	pRsInfo.CreateInstance( __uuidof( Recordset ) );

	try
	{
		strSQL = "SELECT * FROM ["+strTblName+"] ";
		pRsInfo->Open( _variant_t( strSQL ), m_pConIpedSort.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText );
		if ( pRsInfo->adoEOF && pRsInfo->BOF )
		{
			return FALSE;
		}
		for ( pRsInfo->MoveFirst(); !pRsInfo->adoEOF; pRsInfo->MoveNext() )
		{
			strTmp = vtos( pRsInfo->GetCollect( _variant_t("LocalCaption") ) );
			if ( strTmp.IsEmpty() )
				continue;

			iPos = this->m_PropertyWnd.FindElement( 0, strTmp );
			if ( iPos == -1 )
				continue;
			
			this->m_PropertyWnd.GetElementAt( &ElementDef, iPos );

			//д�뵽���ݿ�
			pRsInfo->PutCollect( _variant_t("ExcelColNO"), _variant_t(ElementDef.RightElementText) );
			pRsInfo->Update();
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox( e.Description() );
		return FALSE;
	}
	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/12/30]
// Parameter(s) :
// Return       :
// Remark       :
//------------------------------------------------------------------
void CImportFromXLSDlg::SetDefaultExcelNO(LPCTSTR lpItemTblName)
{
	if ( NULL == lpItemTblName )
		return;
	
	CPropertyWnd::ElementDef ElementDef;
	EDIBgbl::CAPTION2FIELD* pFieldStruct;	// �ӿؼ��Ľṹ
	int nFieldCount = GetField2Caption( pFieldStruct, lpItemTblName, "DefExcelColNO" );
	int iPos;

	for ( int i = 0; i < nFieldCount ; i++ )
	{
		iPos = this->m_PropertyWnd.FindElement( 0, pFieldStruct[i].strCaption );
		if ( -1 != iPos )
		{	
			m_PropertyWnd.SetRightElementContent( iPos, pFieldStruct[i].strExcelNO );
		}
	}

	if ( NULL != pFieldStruct )
	{
		delete [] pFieldStruct;
	}
	UpdateData( FALSE );
}

void CImportFromXLSDlg::OnSetDefaultNo() 
{
	SetDefaultExcelNO( m_strItemTblName );
}
