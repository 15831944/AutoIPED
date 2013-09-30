// DataShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "DataShowDlg.h"

#include "Column.h"
#include "Columns.h"
#include "vtot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataShowDlg dialog
extern CAutoIPEDApp theApp;

CDataShowDlg::CDataShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataShowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataShowDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCursorCol = -1;

	m_strCol_Pass = _T("");
	m_DlgCaption=_T("");

	m_IsAllowDel=FALSE;
	m_IsAllowAddNew=FALSE;
	m_IsAllowUpdate=FALSE;

	m_pIndexArrayToHide=NULL;

	m_pDefaultFieldValueStruct=NULL;

	m_IsAutoDelete=FALSE;

	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDataShowDlg::~CDataShowDlg()
{
	if(m_pIndexArrayToHide)
		delete[] m_pIndexArrayToHide;

	if(m_pDefaultFieldValueStruct)
		delete[] m_pDefaultFieldValueStruct;
}

void CDataShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataShowDlg)
	DDX_Control(pDX, IDC_RESULT_BROWSE, m_ResultBrowseControl);
	DDX_Control(pDX, IDC_RESULT_BROWSE2, m_BrowseGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataShowDlg, CDialog)
	//{{AFX_MSG_MAP(CDataShowDlg)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////
//
// ������Ҫ��ʾ�ļ�¼��
//
// IRecordset[in]�Ѿ��򿪵ļ�¼��
//
void CDataShowDlg::SetRecordset(_RecordsetPtr &IRecordset, CDataGridEx* pCtlDataGrid)
{
	m_IRecordset=IRecordset;
	//ZSYTEST
	if( pCtlDataGrid == NULL )
	{
		//Ĭ�ϵ�һ��Ϊ��ǰҪ�����DataGrid�ؼ�
		pCurDataGrid = &m_ResultBrowseControl;
	}
	else
	{
		//����Ϊָ����DataGrid�ؼ�
		pCurDataGrid = pCtlDataGrid;
	}
	if( IsWindow( pCurDataGrid->GetSafeHwnd()) )
	{
		pCurDataGrid->SetRefDataSource((LPUNKNOWN)IRecordset);
	}

}

///////////////////////////////////////////////////////////
//
// ������Ҫ��ʾ�ļ�¼��
//
_RecordsetPtr& CDataShowDlg::GetRecordset()
{
	return m_IRecordset;
}

///////////////////////////////////////////////////////
//
// ���öԻ���ı���
//
void CDataShowDlg::SetDlgCaption(LPCTSTR pCaption)
{
	if(pCaption==NULL)
		m_DlgCaption=_T("");
	else
		m_DlgCaption=pCaption;

	if(IsWindow(this->m_hWnd))
	{
		SetWindowText(m_DlgCaption);
	}
}

//////////////////////////////////////////////////////
//
// ���ضԻ���ı���
//
CString CDataShowDlg::GetDlgCaption()
{
	return m_DlgCaption; 
}

//////////////////////////////////////////////////////////////
//
// ����DataGrid�ؼ��ı���
//
void CDataShowDlg::SetDataGridCaption(LPCTSTR pCaption)
{
	if(pCaption==NULL)
		m_DataGridCaption=_T("");
	else
		m_DataGridCaption=pCaption;

	if(IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
	{
		//ȡ����DataGria�ؼ�����ʾ������
	//	m_ResultBrowseControl.SetCaption(m_DataGridCaption);
	}
}

/////////////////////////////////////////////////////////////
//
// ��������DataGrid�ؼ��ı���
//
CString CDataShowDlg::GetDataGridCaption()
{ 
	return m_DataGridCaption; 
}


///////////////////////////////////////////////////////////
//
// �����Ƿ��������
//
// IsAllow[in]	��ΪTRUEʱ������£���ΪFALSEʱ���ܸ���
//
void CDataShowDlg::SetAllowUpdate(BOOL IsAllow)
{
	m_IsAllowUpdate=IsAllow;

	if(IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
	{
		m_ResultBrowseControl.SetAllowUpdate(IsAllow);
	}
}

//////////////////////////////////////////////////////
//
// �����Ƿ��������
//
BOOL CDataShowDlg::GetAllowUpdate()
{
	if(IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
	{
		m_IsAllowUpdate=m_ResultBrowseControl.GetAllowUpdate();
	}

	return m_IsAllowUpdate; 
}



///////////////////////////////////////////////////////////
//
// �����Ƿ��������Ӽ�¼
//
// IsAllow[in]	��ΪTRUEʱ������ΪFALSEʱ�������Ӽ�¼
//
void CDataShowDlg::SetAllowAddNew(BOOL IsAllow)
{
	m_IsAllowAddNew=IsAllow;

	if(IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
	{
		m_ResultBrowseControl.SetAllowAddNew(IsAllow);
	}
}

//////////////////////////////////////////////////////
//
// �����Ƿ����������Ӽ�¼
//
BOOL CDataShowDlg::GetAllowAddNew()
{
	if(IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
	{
		m_IsAllowAddNew=m_ResultBrowseControl.GetAllowAddNew();
	}

	return m_IsAllowAddNew;
}



///////////////////////////////////////////////////////////
//
// �����Ƿ�����ɾ����¼
//
// IsAllow[in]	��ΪTRUEʱ������ΪFALSEʱ������
//
void CDataShowDlg::SetAllowDelete(BOOL IsAllow)
{
	m_IsAllowDel=IsAllow;

	if(IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
	{
		m_ResultBrowseControl.SetAllowDelete(IsAllow);
	}
}

//////////////////////////////////////////////////////
//
// �����Ƿ�����ɾ����¼
//
BOOL CDataShowDlg::GetAllowDelete()
{
	if(IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
	{
		m_IsAllowDel=m_ResultBrowseControl.GetAllowDelete();
	}

	return m_IsAllowDel;
}

//////////////////////////////////////////////////////////////////
//
// ������DataGrid�ؼ���ʾ����ʱ����Ҫ���ص���
//
// pIndex[in]		��Ҫ�����е��������飬��0��ʼ
// IndexSize[in]	pIndex�Ĵ�С
//
// throw(COleDispatchException)
//
void CDataShowDlg::SetHideColumns(int *pIndex, DWORD IndexSize)
{
	if(pIndex==NULL || IndexSize<=0)
		return;

	if(m_pIndexArrayToHide)
		delete[] m_pIndexArrayToHide;

	m_strFieldsNameToHide=_T("");

	m_pIndexArrayToHide=new int[IndexSize+1];

	for(DWORD i=0;i<IndexSize;i++)
	{
		m_pIndexArrayToHide[i]=pIndex[i];
	}

	m_pIndexArrayToHide[IndexSize]=EndHideArray;

	if(IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
	{
		try
		{
			HideColumns();
		}
		catch(COleDispatchException *e)
		{
			ReportExceptionErrorLV2(e);
			throw;
		}

	}
}

//////////////////////////////////////////////////////////////////
//
// ������DataGrid�ؼ���ʾ����ʱ����Ҫ���ص���
//
// szFields[in]		��Ҫ�������ֶ������ж���ֶ���Ҫ����ʱ������
//					�ֶ��á�|������
//
// throw(COleDispatchException)
//
void CDataShowDlg::SetHideColumns(LPCTSTR szFields)
{
	if(szFields==NULL)
		return;

	m_strFieldsNameToHide=szFields;
	
	if(m_pIndexArrayToHide)
	{
		delete[] m_pIndexArrayToHide;
		m_pIndexArrayToHide=NULL;
	}

	if(IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
	{
		try
		{
			HideColumns();
		}
		catch(COleDispatchException *e)
		{
			ReportExceptionErrorLV2(e);
			throw;
		}

	}
}

//////////////////////////////////////////////
//
// ����ָ������
//
// throw(COleDispatchException)
//
void CDataShowDlg::HideColumns()
{
	int *pIndex;
	long iIndex;
	CColumn Col;

	if(!::IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
		return;

	if(!m_strFieldsNameToHide.IsEmpty())
	{
		try
		{
			HideColumnsByFieldsName(m_strFieldsNameToHide);
		}
		catch(COleDispatchException *e)
		{
			ReportExceptionErrorLV2(e);
			throw;
		}

		return;
	}

	if(m_pIndexArrayToHide==NULL)
		return;

	for(pIndex=m_pIndexArrayToHide; *pIndex!=EndHideArray; pIndex++)
	{
		if(*pIndex == HideFirstColumn)
		{
			iIndex=0;
		}
		else if(*pIndex == HideLastColumn)
		{
			iIndex=m_ResultBrowseControl.GetColumns().GetCount()-1;
		}
		else
		{
			iIndex=*pIndex;
		}
	}

	try
	{
		Col=m_ResultBrowseControl.GetColumns().GetItem(_variant_t(iIndex));
		Col.SetVisible(FALSE);
	}
	catch(COleDispatchException *e)
	{
		ReportExceptionErrorLV2(e);
		throw;
	}

}

/////////////////////////////////////////////////////////////
//
// ����ָ���ֶ����Ƶ���
//
// szFields[in]	��Ҫ�����е��ֶ���
//
// throw(COleDispatchException);
//
void CDataShowDlg::HideColumnsByFieldsName(LPCTSTR szFields)
{
	CColumn Col;
	CString strFields,strField;
	int nPrev,nCur=0;

	if(!IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
		return;

	if(szFields==NULL)
		return;

	strFields=szFields;

	nCur=0;
	nPrev=-1;

	while(TRUE)
	{
		nPrev=nCur;

		nCur=strFields.Find(_T('|'),nCur);

		if(nCur==-1)
		{
			strField=strFields.Mid(nPrev);
		}
		else
		{
			strField=strFields.Mid(nPrev,nCur-nPrev);
		}

		try
		{
			Col=m_ResultBrowseControl.GetColumns().GetItem(_variant_t(strField));

			Col.SetVisible(FALSE);
		}
		catch(COleDispatchException *e)
		{
		//	ReportExceptionErrorLV2(e); //zsy 2005.3.16
		//	throw;
			e->Delete();
		}

		if(nCur==-1)
			break;
		
		nCur++;
		
	}

}


//////////////////////////////////////////////////////////////////////////////////////////
//
// ��������Ӽ�¼ʱ���ֶε�Ĭ��ֵ
//
// pDefault[in]		tag_DefaultFieldValue�ṹ��ָ��
// dwLength			pDefault������
//
void CDataShowDlg::SetDefaultFieldValue(tag_DefaultFieldValue *pDefault, DWORD dwLength)
{
	if(pDefault==NULL || dwLength<=0)
		return;

	if(m_pDefaultFieldValueStruct)
	{
		delete []m_pDefaultFieldValueStruct;
	}

	m_pDefaultFieldValueStruct=new tag_DefaultFieldValue[dwLength+1];

	for(DWORD i=0;i<dwLength;i++)
	{
		m_pDefaultFieldValueStruct[i].strFieldName=pDefault[i].strFieldName;
		m_pDefaultFieldValueStruct[i].varFieldValue=pDefault[i].varFieldValue;
	}

	m_pDefaultFieldValueStruct[dwLength].strFieldName=_T("");
}

////////////////////////////////////////////////////
//
// �����ڷ�ģ̬���Ƿ��ڴ�������ʱ�Ƿ��Զ�ɾ������
//
// bAutoDelete[in]	��ΪTRUEʱ�ڴ�������ʱ�Զ�ɾ������
//					��ΪFALSEʱ��Ҫ�Լ����ٶ���
//
void CDataShowDlg::SetAutoDelete(BOOL bAutoDelete)
{
	m_IsAutoDelete=bAutoDelete;
}

/////////////////////////////////////////////////////////////////////////////
// CDataShowDlg message handlers
//

BOOL CDataShowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	if(m_IRecordset==NULL)
//	{
//		ReportExceptionError(_T("δָ������"));
//		OnCancel();
//		return TRUE;
//	}
	pCurDataGrid = &m_ResultBrowseControl;		//Ĭ�ϵ�һ���ؼ�Ϊ��ǰҪ�����.

	m_ResultBrowseControl.SetRefDataSource((LPUNKNOWN)m_IRecordset);	

	if(!GetDlgCaption().IsEmpty())
		SetWindowText(GetDlgCaption());

	//ȡ�� DataGrid�ؼ�����ʾ�ı���,�ñ�������ԭ�������ݿ��б������. [2005/06/20]

//	if(!GetDataGridCaption().IsEmpty())
//		m_ResultBrowseControl.SetCaption(GetDataGridCaption());
	
	this->ShowWindow(SW_MAXIMIZE);
	RECT rect;
	GetClientRect(&rect);
	m_ResultBrowseControl.MoveWindow(0,0,rect.right-rect.left,rect.bottom-rect.top);

	m_ResultBrowseControl.SetAllowAddNew(m_IsAllowAddNew);
	m_ResultBrowseControl.SetAllowUpdate(m_IsAllowUpdate);
	m_ResultBrowseControl.SetAllowDelete(m_IsAllowDel);

	//�����ֶεĿ�Ⱥͱ���
	this->SetFieldCaption( GetDataGridCaption() );

	try
	{
		HideColumns();
	}
	catch(COleDispatchException *e)
	{
		ReportExceptionErrorLV1(e);
		e->Delete();
	}

	m_ResultBrowseControl.InitMouseWheel();
	m_BrowseGrid.InitMouseWheel();

	//��ʼ��궨λ��ָ������.
	if( m_nCursorCol >= 0 )
	{
		this->FindDataGridCol(m_nCursorCol);
	}

	SetIcon(m_hIcon,TRUE);
	SetIcon(m_hIcon,FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDataShowDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(IsWindow(m_ResultBrowseControl.GetSafeHwnd()))
		m_ResultBrowseControl.MoveWindow(0,0,cx,cy);

}



BEGIN_EVENTSINK_MAP(CDataShowDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataShowDlg)
	ON_EVENT(CDataShowDlg, IDC_RESULT_BROWSE, 217 /* OnAddNew */, OnOnAddNewResultBrowse, VTS_NONE)
	ON_EVENT(CDataShowDlg, IDC_RESULT_BROWSE, 218 /* RowColChange */, OnRowColChangeResultBrowse, VTS_PVARIANT VTS_I2)
	ON_EVENT(CDataShowDlg, IDC_RESULT_BROWSE, -600 /* Click */, OnClickResultBrowse, VTS_NONE)
	ON_EVENT(CDataShowDlg, IDC_RESULT_BROWSE, 213 /* ColResize */, OnColResizeResultBrowse, VTS_I2 VTS_PI2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

///////////////////////////////////////////////
//
// ��ӦDataGrid��AddNew�¼�
//
// �ڴ˼���Ĭ�ϵ��ֶ�ֵ
//
void CDataShowDlg::OnOnAddNewResultBrowse() 
{
	tag_DefaultFieldValue *pTemp;
	_RecordsetPtr IRecordset;
	LPUNKNOWN pUnknown;
	HRESULT hr;

	if(m_pDefaultFieldValueStruct==NULL)
		return;

	try
	{
		pUnknown=m_ResultBrowseControl.GetDataSource();

		hr=pUnknown->QueryInterface(__uuidof(_RecordsetPtr),(void**)&IRecordset);

		if(FAILED(hr))
		{
			pUnknown->Release();
			ReportExceptionError(_com_error(hr));
			return;
		}

		pUnknown->Release();
	}
	catch(COleDispatchException *e)
	{
		ReportExceptionErrorLV1(e);
		e->Delete();
	}
	FieldPtr col;
	for(pTemp=m_pDefaultFieldValueStruct ; !pTemp->strFieldName.IsEmpty() ; pTemp++)
	{
		try
		{
			try
			{
				col = IRecordset->GetFields()->GetItem(_variant_t(pTemp->strFieldName));
			}
			catch(_com_error)
			{
				continue;	//û�и���
			}

			IRecordset->PutCollect(_variant_t(pTemp->strFieldName),pTemp->varFieldValue);
		}
		catch(_com_error &e)
		{
			ReportExceptionErrorLV1(e);
		}
	}
	
}


void CDataShowDlg::OnOK()
{
	m_ResultBrowseControl.UnInitMouseWheel();
	m_BrowseGrid.UnInitMouseWheel();

	EndDialog(0);

	if(IsWindow(this->GetSafeHwnd()))
		DestroyWindow();

	if(m_IsAutoDelete)
		delete this;
}

void CDataShowDlg::OnCancel()
{
	m_ResultBrowseControl.UnInitMouseWheel();
	m_BrowseGrid.UnInitMouseWheel();

	EndDialog(0);

	if(IsWindow(this->GetSafeHwnd()))
		DestroyWindow();

	if(m_IsAutoDelete)
		delete this;
}

void CDataShowDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CRect rect;
	POINT pt;
	CWnd *pParent;
	CWnd *pActiveWnd;

	pParent=GetParent();

	if(pParent==NULL)
	{
		CDialog::OnGetMinMaxInfo(lpMMI);
		return;
	}

	if(pParent->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		pActiveWnd=((CFrameWnd*)pParent)->GetActiveView();
	}
	else
	{
		pActiveWnd=pParent;
	}

	if(pActiveWnd && IsWindow(pActiveWnd->GetSafeHwnd()))
	{
		pActiveWnd->GetClientRect(&rect);

		pt.x=rect.left;
		pt.y=rect.top;

		pActiveWnd->ClientToScreen(&pt);

		lpMMI->ptMaxPosition.x=pt.x;
		lpMMI->ptMaxPosition.y=pt.y;

		lpMMI->ptMaxSize.x=rect.Width();
		lpMMI->ptMaxSize.y=rect.Height();
	}

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CDataShowDlg::OnDestroy() 
{
	m_ResultBrowseControl.UnInitMouseWheel();
	m_BrowseGrid.UnInitMouseWheel();

	CDialog::OnDestroy();	
}

void CDataShowDlg::OnRowColChangeResultBrowse(VARIANT FAR* LastRow, short LastCol) 
{
	if( !m_strCol_Pass.IsEmpty() )  //ֻ��ָ�����ֶν��д���.()
	{
		CString strText;
		short Col = 0 ; 
		Col = m_ResultBrowseControl.GetCol();
		if( Col != -1 )
		{
			//strText = m_ResultBrowseControl.GetColumns().GetItem(_variant_t((short)Col)).GetCaption();
			strText = (LPCTSTR)m_IRecordset->GetFields()->GetItem(Col)->GetName();

			if( !strText.CompareNoCase(m_strCol_Pass) )
			{
				strText = m_ResultBrowseControl.GetText();
				if( !strText.CompareNoCase("Y") )
				{
				//	m_ResultBrowseControl.SetText("");
					m_ResultBrowseControl.GetColumns().GetItem(_variant_t((short)Col)).SetText("");
				}
				else
				{
					m_ResultBrowseControl.GetColumns().GetItem(_variant_t((short)Col)).SetText("Y");
				}
			}
		}
	}

}
//����ָ�����ֶθ�һ���ض���ֵ
void CDataShowDlg::SetPassField(CString strField)
{
	m_strCol_Pass = strField;
}

//����.
void CDataShowDlg::OnClickResultBrowse() 
{
/*	int col = m_ResultBrowseControl.GetCol();
	CString strTmp;
	if( col != -1 )
	{
		strTmp = m_ResultBrowseControl.GetColumns().GetItem(_variant_t((long)col)).GetCaption();
		if( !strTmp.CompareNoCase(m_strCol_Pass) )
		{
			strTmp = m_ResultBrowseControl.GetColumns().GetItem(_variant_t((long)col)).GetText();
			if( !strTmp.CompareNoCase("Y") )
			{
				m_ResultBrowseControl.GetColumns().GetItem(_variant_t((long)col)).SetText("");
			}
			else
			{
				m_ResultBrowseControl.GetColumns().GetItem(_variant_t((long)col)).SetText("Y");
			}
		}
	}
*/
}
////////////////////////////////////////////////
//��λ��굽ָ������.
//
bool CDataShowDlg::FindDataGridCol(short colIndex)
{
	try
	{
		if( IsWindow(pCurDataGrid->GetSafeHwnd()) )
		{
			pCurDataGrid->SetCol(colIndex);
		}
	}
	catch(_com_error)
	{
		return false;
	}
	return true;
}
//���ù�굽ָ������.
void CDataShowDlg::SetCursorCol(short startCol)
{
	m_nCursorCol = startCol;
}

//------------------------------------------------------------------                
// DATE         :[2005/06/16]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�����ֶεı���Ϳ��.
//------------------------------------------------------------------
BOOL CDataShowDlg::SetFieldCaption(CString strTblName)
{
	if( !SetFieldCaptionAndWidth(pCurDataGrid,theApp.m_pConIPEDsort, strTblName) )
	{
		return FALSE;
	}
	return TRUE;
}

//------------------------------------------------------------------                
// DATE         :[2005/06/15]
// Author       :ZSY
// Parameter(s) :
// Return       :ȫ�ֵ�
// Remark       :�����ֶεı���Ϳ��.��IPEDsort.MDB���ݿ��п����ҵ���Ӧ��ϵ
//------------------------------------------------------------------
BOOL SetFieldCaptionAndWidth(CDataGridEx* pDataGrid, _ConnectionPtr& pConSort, CString strTblName)
{
	if (pConSort == NULL || strTblName.IsEmpty())
	{
		return FALSE;
	}
	if ( !IsWindow(pDataGrid->GetSafeHwnd()) )
	{
		return FALSE;
	}

	try
	{
		_RecordsetPtr pRsStruct;		//��ͬ���ݱ���֮��Ӧ�Ľṹ��.

		CString		  strSQL;			//SQL���
		CString		  strStructTblName;	//���ݱ��Ӧ�Ľṹ����.
		CString		  strFieldName;		//�ֶ���
		CString		  strChineseCaption;//��Ӧ����������

		long		nFieldCount;		//��ʾ���ֶθ���
		float		fFieldWidth;		//���ֵĿ��
		long		fWindowWidth;		//DataGrid�ؼ��Ŀ��
		
		RECT		rect;
		BOOL		bIsVisible;			//�ֶ��Ƿ�ɼ�.

		float	xRatio = 1;		//��Ļ�ֱ��ʵı���.����
		float	yRatio = 1;
		GetScreenRatio(xRatio,yRatio);

		pRsStruct.CreateInstance(__uuidof(Recordset));
		
		if (!GetStructTblName(pConSort,strTblName,strStructTblName))
		{
			return FALSE;
		}
		
		pDataGrid->GetWindowRect(&rect);
		fWindowWidth = rect.right-rect.left;

		//�򿪽ṹ��
		strSQL = "SELECT * FROM ["+strStructTblName+"]";
		pRsStruct->Open(_bstr_t(strSQL), pConSort.GetInterfacePtr(), 
					adOpenStatic, adLockOptimistic, adCmdText);
		if (pRsStruct->GetRecordCount() <= 0)
		{
			return FALSE;
		}
		nFieldCount = pDataGrid->GetColumns().GetCount();
		for (short i=0; i < nFieldCount; i++)
		{
			strFieldName = pDataGrid->GetColumns().GetItem(_variant_t(i)).GetCaption();
			strSQL = "FieldName='"+strFieldName+"'";
			//��λ����ǰ�ֶ�����Ӧ�ļ�¼�ϡ�
			pRsStruct->MoveFirst();
			pRsStruct->Find(_bstr_t(strSQL), 0, adSearchForward);
			if (pRsStruct->adoEOF)
			{
				
				//��ǰ�ֶ�û�ж�Ӧ��������¼�������д��� ??��Ҫ������??
				pDataGrid->GetColumns().GetItem(_variant_t(i)).SetVisible(FALSE);
				continue;
			}
			//�ֶ����ı��������ַ�.
			strChineseCaption = vtos(pRsStruct->GetCollect(_variant_t("LocalCaption")));
			if (!strChineseCaption.IsEmpty())
			{
				//�����ֶ���Ϊ�����������ַ�
				pDataGrid->GetColumns().GetItem(_variant_t(i)).SetCaption(strChineseCaption);
			}
			fFieldWidth = (float)vtof(pRsStruct->GetCollect(_variant_t("width")));
			if (fWindowWidth > 0 && fFieldWidth > fWindowWidth)
			{
				fFieldWidth = (float)fWindowWidth*2/30;		//ȡ�ܿ�ȵ�2/30
			}
			fFieldWidth = fFieldWidth > 0 ? fFieldWidth:60;

			fFieldWidth *= xRatio;
			//�����ֶεĿ��
			pDataGrid->GetColumns().GetItem(_variant_t(i)).SetWidth(fFieldWidth);
			//�ֶ��Ƿ�ɼ�
			bIsVisible = vtob(pRsStruct->GetCollect(_variant_t("Visible")));
			pDataGrid->GetColumns().GetItem(_variant_t(i)).SetVisible(bIsVisible);				
		}

	}
	catch (_com_error)
	{
		return FALSE;
	}
	return TRUE;
}


//------------------------------------------------------------------                
// DATE         :[2005/06/15]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       : �������ݱ�,������Ӧ�Ľṹ����.
//------------------------------------------------------------------
BOOL GetStructTblName(_ConnectionPtr &pCon, CString strDataTblName, CString& strStructTblName)
{ 
	try
	{
		_RecordsetPtr pRsTblINFO;	//�����.
		pRsTblINFO.CreateInstance(__uuidof(Recordset));
		CString strSQL;

		strSQL = "SELECT * FROM [TableINFO] WHERE ManuTBName='"+strDataTblName+"' ";
		pRsTblINFO->Open(_bstr_t(strSQL), pCon.GetInterfacePtr(),
						adOpenStatic, adLockOptimistic, adCmdText);
		if (pRsTblINFO->GetRecordCount() <= 0)
		{
			return FALSE;
		}
		strStructTblName = vtos( pRsTblINFO->GetCollect(_variant_t("StructTblName")) );
		if (strStructTblName.IsEmpty())		//û�нṹ��
		{
			return FALSE;
		}
	}
	catch (_com_error)
	{
		return FALSE;
	}

	return TRUE;
}


//------------------------------------------------------------------                
// DATE         :[2005/06/17]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :����DataGrid�ؼ����ֶεĿ��
//------------------------------------------------------------------
BOOL SaveDataGridFieldWidth(CDataGridEx* pDataGrid, _ConnectionPtr& pConSort, CString strTblName)
{
	if (pConSort == NULL || strTblName.IsEmpty())
	{
		return FALSE;
	}
	if ( !IsWindow(pDataGrid->GetSafeHwnd()) )
	{
		return FALSE;
	}

	try
	{
		_RecordsetPtr pRsStruct;		//��ͬ���ݱ���֮��Ӧ�Ľṹ��.

		CString		  strSQL;			//SQL���
		CString		  strStructTblName;	//���ݱ��Ӧ�Ľṹ����.
		CString		  strFieldName;		//�ֶ���
		CString		  strChineseCaption;//��Ӧ����������

		long		nFieldCount;		//��ʾ���ֶθ���
		float		fFieldWidth;		//���ֵĿ��

		float	xRatio = 1;		//��Ļ�ֱ��ʵı���.����
		float	yRatio = 1;
		GetScreenRatio(xRatio,yRatio);

		
		pRsStruct.CreateInstance(__uuidof(Recordset));
		
		if (!GetStructTblName(pConSort,strTblName,strStructTblName))
		{
			return FALSE;
		}
		//�򿪽ṹ��
		strSQL = "SELECT * FROM ["+strStructTblName+"]";
		pRsStruct->Open(_bstr_t(strSQL), pConSort.GetInterfacePtr(), 
					adOpenStatic, adLockOptimistic, adCmdText);
		if (pRsStruct->GetRecordCount() <= 0)
		{
			return FALSE;
		}
		nFieldCount = pDataGrid->GetColumns().GetCount();
		for (short i=0; i < nFieldCount; i++)
		{
			strFieldName = pDataGrid->GetColumns().GetItem(_variant_t(i)).GetCaption();
			strSQL = "LocalCaption='"+strFieldName+"'";
			//��λ����ǰ�ֶ�����Ӧ�ļ�¼�ϡ�
			pRsStruct->MoveFirst();
			pRsStruct->Find(_bstr_t(strSQL), 0, adSearchForward);
			if (!pRsStruct->adoEOF)
			{
				
				//��õ�ǰ�ֶεĿ�ȡ�
				fFieldWidth = pDataGrid->GetColumns().GetItem(_variant_t(i)).GetWidth();

				//������Ļ�ֱ���.
				fFieldWidth /= xRatio;
				//����ǰ�ֶεĿ�ȱ��浽���ݿ��С�
				pRsStruct->PutCollect(_variant_t("width"), _variant_t(fFieldWidth));
				pRsStruct->Update();
			}
		}

	}
	catch (_com_error)
	{
		return FALSE;
	}
	return TRUE;
}

//------------------------------------------------------------------                
// DATE         :[2005/06/17]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�ı��еĴ�С
//------------------------------------------------------------------
void CDataShowDlg::OnColResizeResultBrowse(short ColIndex, short FAR* Cancel) 
{
	SaveDataGridFieldWidth(pCurDataGrid, theApp.m_pConIPEDsort, GetDataGridCaption());
}

//------------------------------------------------------------------                
// DATE         :[2005/06/20]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�����Ļ�ķֱ��ʱȡ���800 * 600Ϊ1
//------------------------------------------------------------------
BOOL GetScreenRatio(float& xRatio, float& yRatio)
{
	CSize sizeScreen;		//��Ļ��С
	sizeScreen.cx = GetSystemMetrics(SM_CXSCREEN);
	sizeScreen.cy = GetSystemMetrics(SM_CYSCREEN);

	double ratio;		//��
	double ratio1;
	//���ݿ����Էֱ���800*600Ϊ��׼.
	ratio=8.0/5.0;
	ratio1=89.5/59.0;
	
	switch(sizeScreen.cx*sizeScreen.cy)
	{
 	case 640*480:ratio=2.0;
 			     ratio1=112.0/59.0;
 		break;
 	case 720*480:ratio=24.0/14.0;
 		         ratio1=98.0/58.0;
 				 break;
 	case 720*576:ratio=24.0/14.0;
 		          ratio1=98.0/58.0;
 				 break;
 	case 848*480:
 		 ratio=24.0/16.0;
 		 ratio1=98.0/68.5;
 		 break;
  	case 800*600:
 			ratio=8.0/5.0;
			ratio1=89.5/59.0;
 		break;
	case 1024*768:ratio=6.0/5.0;
 		          ratio1=69.7/59.0;
 		break;
 	case 1152*864:ratio=24.0/22.0;
 		          ratio1=98.0/93.0;
 		break;
 	case 1280*1024:ratio=1.0;
 		           ratio1=98.0/103.5;
 		break;
	}
	//���ݿ����Էֱ���800*600Ϊ��׼.

	xRatio = (float)((8.0/5.0) / ratio);
	yRatio = (float)((89.5/59.0) / ratio1);
	
	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/11/23]
// Parameter(s) :
// Return       :
// Remark       :ˢ���������ĶԻ���
//------------------------------------------------------------------
BOOL CDataShowDlg::UpdateBrowWindow()
{
	if (!IsWindow(pCurDataGrid->GetSafeHwnd()))
	{
		return FALSE;
	}
	pCurDataGrid->SetRefDataSource((LPUNKNOWN)m_IRecordset);	

	if(!GetDlgCaption().IsEmpty())
		SetWindowText(GetDlgCaption());

	//ȡ�� DataGrid�ؼ�����ʾ�ı���,�ñ�������ԭ�������ݿ��б������. [2005/06/20]

//	if(!GetDataGridCaption().IsEmpty())
//		m_ResultBrowseControl.SetCaption(GetDataGridCaption());
	
	this->ShowWindow(SW_MAXIMIZE);
//	RECT rect;
//	GetClientRect(&rect);
//	pCurDataGrid->MoveWindow(0,0,rect.right-rect.left,rect.bottom-rect.top);

	pCurDataGrid->SetAllowAddNew(m_IsAllowAddNew);
	pCurDataGrid->SetAllowUpdate(m_IsAllowUpdate);
	pCurDataGrid->SetAllowDelete(m_IsAllowDel);

	//�����ֶεĿ�Ⱥͱ���
	this->SetFieldCaption( GetDataGridCaption() );

	try
	{
		HideColumns();
	}
	catch(COleDispatchException *e)
	{
		ReportExceptionErrorLV1(e);
		e->Delete();
	}

	pCurDataGrid->InitMouseWheel();

	//��ʼ��궨λ��ָ������.
	if( m_nCursorCol >= 0 )
	{
		this->FindDataGridCol(m_nCursorCol);
	}
	return TRUE;
}
