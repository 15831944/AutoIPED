// DlgSelCritDB.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "DlgSelCritDB.h"
#include "MainFrm.h"

#include "EDIBgbl.h"
#include "vtot.h"
#include "GetPropertyofMaterial2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelCritDB dialog
extern CAutoIPEDApp theApp;

CDlgSelCritDB::CDlgSelCritDB(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelCritDB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelCritDB)
	//}}AFX_DATA_INIT
}


void CDlgSelCritDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelCritDB)
	DDX_Control(pDX, IDC_LIST1, m_ctrlShowCODE);
	DDX_Control(pDX, IDC_CBO_STUFF_DB, m_ctlMaterCodeName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelCritDB, CDialog)
	//{{AFX_MSG_MAP(CDlgSelCritDB)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelCritDB message handlers
BOOL CDlgSelCritDB::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nFCount, pos;
	CString strCritDBName[20], strMaterialDBName[20];
	struct KeyToCalling sutKeyCalling[20];	// ��ҵ���ƺͱ�ʾ��

	int nKeyCount = GetCallingFromDB( sutKeyCalling );
	if ( nKeyCount <= 0 )
	{
		return FALSE;
	}
	//��ʼ����׼���ݿ��б�
//	nFCount = this->GetCriterionDBName( strCritDBName );
	//��׼�淶
	CMapStringToString mapCode_Mdb;
	CString strValue;
	short	nIndex = 0;	//��ʼ��ѡ��淶��������

	CRect rc;
	m_ctrlShowCODE.GetWindowRect(&rc);
	m_ctrlShowCODE.InsertColumn(0, _T("��ҵ����"), LVCFMT_LEFT, rc.Width()/3);
	m_ctrlShowCODE.InsertColumn(1, _T("���ݿ�"), LVCFMT_LEFT, rc.Width()/3);
	m_ctrlShowCODE.InsertColumn(2, _T("��׼����"), LVCFMT_LEFT, rc.Width()/3);
	
	m_ctrlShowCODE.SetExtendedStyle( m_ctrlShowCODE.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );

	try
	{
		_RecordsetPtr pRsDBRef;
		pRsDBRef.CreateInstance( __uuidof( Recordset ) );
		CString strSQL;
		CString strTmp;
		strSQL = "SELECT * FROM [DataBaseRef]";
		pRsDBRef->Open( _variant_t(strSQL), theApp.m_pConRefInfo.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText );
		for( int j=0, row=0; j < nKeyCount; j++ )
		{
			m_ctrlShowCODE.InsertItem( row, sutKeyCalling[j].strCallingName );
			if ( !pRsDBRef->adoEOF || !pRsDBRef->BOF)
			{
				strSQL.Format( "CallingKey='%d'",sutKeyCalling[j].strKey);
				pRsDBRef->MoveFirst();
				pRsDBRef->Find( _bstr_t(strSQL), 0, adSearchForward);
				if ( !pRsDBRef->adoEOF )
				{
					// ��ҵ�ı�׼���ݿ���
					strTmp = vtos( pRsDBRef->GetCollect( _variant_t("CodeDBName") ) );
					m_ctrlShowCODE.SetItemText( row, 1, strTmp );
					// ��ҵ�в��ϵĹ淶��
					m_ctrlShowCODE.SetItemText( row, 2, sutKeyCalling[j].strCodeName );
					// 
				}

				if( sutKeyCalling[j].strKey==EDIBgbl::iCur_CodeKey)
				{
					//��ס��һ��ѡ��Ĺ淶��������
					nIndex = row;
				}
			}
			row++;
		}
		if ( m_ctrlShowCODE.GetItemCount() > 0 )
		{
			m_ctrlShowCODE.SetHotItem( nIndex );
			m_ctrlShowCODE.SetSelectionMark( nIndex );
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox( e.Description() );
		return FALSE;
	}
	
	//��ʼ���淶�б�
	CArray<CString,CString> mCodeNameArray;
	GetPropertyofMaterial mGetPropertyofMaterial;
	mGetPropertyofMaterial.GetAllCodeName( theApp.m_pConMaterial,mCodeNameArray );
	int iCount = mCodeNameArray.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		m_ctlMaterCodeName.AddString( mCodeNameArray.GetAt(i) );
	}
	
	int iCurSel = m_ctlMaterCodeName.FindString(-1,EDIBgbl::sCur_MaterialCodeName );
	if ( iCurSel == -1 )
		iCurSel = 0;
	m_ctlMaterCodeName.SetCurSel( iCurSel );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgSelCritDB::OnOK() 
{
	CString	strCurCodeName;
	CString	strCurDBName;
	CString strTmp;
	UpdateData();
	m_ctlMaterCodeName.GetWindowText( EDIBgbl::sCur_MaterialCodeName );

	int row = m_ctrlShowCODE.GetSelectionMark();
	if( row == -1 )
	{
		AfxMessageBox("��ѡ��һ����ҵ�Ĺ淶���ݿ�!");
		return;
	}
	char c[256];
	//�淶���ݿ�
	m_ctrlShowCODE.GetItemText(row, 1, c, 256);
	strCurDBName = c;

	//��ǰ�淶
	memset(c,'\0',256);
	m_ctrlShowCODE.GetItemText(row, 0, c, 256);
	strCurCodeName = c;

	//��ǰ�淶����
	memset(c,'\0',256);
	m_ctrlShowCODE.GetItemText(row, 2, c, 256);
	EDIBgbl::sCur_CodeNO = c;

	try
	{
		if ( !FileExists( EDIBgbl::sCritPath + strCurDBName ) )
		{
			strTmp.Format(IDS_NOT_EXISTS_FILE, EDIBgbl::sCritPath + strCurDBName);
			AfxMessageBox( strTmp );
			return;
		}
		CString strCon;
		//�������ӱ�׼��
		strCon = CONNECTSTRING + EDIBgbl::sCritPath + strCurDBName;
		if( theApp.m_pConnectionCODE->State == adStateOpen )
		{
			theApp.m_pConnectionCODE->Close();
		}
		theApp.m_pConnectionCODE->Open(_bstr_t(strCon), "", "", -1);
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description()+"\n\nѡ��Ĺ淶���ݿ⣨"+strCurCodeName+"�����ƻ�, �����°�װ AutoIPED !");
		CDialog::OnCancel();
		return;
	}
	EDIBgbl::sCur_CritDbName = strCurDBName;
	EDIBgbl::sCur_CodeName = strCurCodeName;
	EDIBgbl::iCur_CodeKey=row+1;

	EDIBgbl::SetCurDBName();
	//��ʾ��ǰ�Ĺ�����(������ҵ��׼)
	((CMainFrame*)theApp.m_pMainWnd)->ShowCurrentProjectName();
	
	CDialog::OnOK();
}

//���ܣ���ñ�׼���ݿ����ơ�
//�����������ݿ����Ƶĸ���������strCritDBName[]��
short CDlgSelCritDB::GetCriterionDBName(CString strCritDBName[])
{
	CFileFind  fileFind;
	CString    strFN;
	short      nFCount;
	short i=0;
	nFCount = fileFind.FindFile( EDIBgbl::sCritPath+"*.mdb" );
	if( nFCount == 0 )
	{
		return 0;
	}
	while( fileFind.FindNextFile() )
	{
		strFN = fileFind.GetFileName();
		if( strFN != "" )
		{
			strCritDBName[i++] = strFN;
		}
	}
	strFN = fileFind.GetFileName();
	if( strFN != "" )
	{
		strCritDBName[i++] = strFN;
	}

	return i;
}

//���ܣ�������в������ݿ�����ơ�
//�������ݿ����Ƶĸ���������������strMaterialDBName��
short CDlgSelCritDB::GetMaterialDBName(CString strMaterialDBName[])
{
	short nFCount, i=0;
	CString strFN;
	CFileFind fileFind;
	nFCount = fileFind.FindFile(EDIBgbl::sMaterialPath+"*.mdb");
	if( nFCount == 0 )
	{
		return 0;
	}
	while( fileFind.FindNextFile() )
	{
		strFN = fileFind.GetFileName();
		if( strFN != "" )
		{
			strMaterialDBName[i++] = strFN;
		}
	}
	strFN = fileFind.GetFileName();
	if( strFN != "" )
	{
		strMaterialDBName[i++] = strFN;
	}
	return i;
}

void CDlgSelCritDB::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}
//------------------------------------------------------------------
// DATE         :[2006/01/17]
// Parameter(s) :
// Return       :
// Remark       :�����ݿ���ȡ����ͬ����ҵ����
//------------------------------------------------------------------
int CDlgSelCritDB::GetCallingFromDB(struct KeyToCalling sutKeyCalling[])
{
	// ��ͬ����ҵ���������ݿ�ͳһ���й��� [2006/01/17]
	_RecordsetPtr pRsCalling;	// ��ҵ�����
	pRsCalling.CreateInstance( __uuidof( Recordset ) );
	CString strSQL;	// SQL���
	int nCount = 0;	// ��ͬ��ҵ�ĸ���
	try
	{
		strSQL = "SELECT * FROM [CallingManage]";
		pRsCalling->Open( _variant_t(strSQL), theApp.m_pConRefInfo.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText );
		if ( pRsCalling->adoEOF && pRsCalling->BOF )
		{
			return -1;
		}
		for (pRsCalling->MoveFirst(); !pRsCalling->adoEOF; pRsCalling->MoveNext())
		{
			sutKeyCalling[nCount].strKey = vtoi( pRsCalling->GetCollect( _variant_t("CallingKey") ) );
			sutKeyCalling[nCount].strCallingName = vtos( pRsCalling->GetCollect( _variant_t("CallingName") ) );
			sutKeyCalling[nCount].strCodeName = vtos( pRsCalling->GetCollect( _variant_t("CODE") ) );

			nCount++;
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox( e.Description() );
		return -1;
	}
	return nCount;
}

void CDlgSelCritDB::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
