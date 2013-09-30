// DlgShowAllTable.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "DlgShowAllTable.h"
#include "DataShowDlg.h"
#include "ExceptionInfohandle.h"
#include "vtot.h"
#include "EDIBgbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CAutoIPEDApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgShowAllTable dialog


CDlgShowAllTable::CDlgShowAllTable(CWnd* pParent /*=NULL*/)
: CDataShowDlg(pParent)
{
	//{{AFX_DATA_INIT(CDlgShowAllTable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_IsInHDragArea = false;
	m_IsInVDragArea = false;
	m_ListSize.cx=200;
	m_ListSize.cy =350;
}

/*
void CDlgShowAllTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShowAllTable)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
*/

BEGIN_MESSAGE_MAP(CDlgShowAllTable, CDataShowDlg)
	//{{AFX_MSG_MAP(CDlgShowAllTable)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_MATERIALNAME_LIST, OnTableSelChange)  //ѡ��ͬ�ı�.
	ON_BN_CLICKED(IDC_DELETE_SEL_MATERIAL,  OnDeleteTblAll)		//ɾ�����е����м�¼
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShowAllTable message handlers

BOOL CDlgShowAllTable::tableExists(_ConnectionPtr pCon, CString tbn)
{
	if(pCon==NULL || tbn=="")
		return false;
	_RecordsetPtr rs;
	if(tbn.Left(1)!="[")
		tbn="["+tbn;
	if(tbn.Right(1)!="]")
		tbn+="]";
	try{
		rs=pCon->Execute(_bstr_t(tbn),NULL,adCmdTable);
		rs->Close();
		return true;
	}
	catch(_com_error e)
	{
		return false;
	}

}


BOOL CDlgShowAllTable::OnInitDialog() 
{
	CDataShowDlg::OnInitDialog();
	CRect ClientRect;
	CFont *pFont;

	m_HDragArea.SetRectEmpty();
	m_VDragArea.SetRectEmpty();

	GetWindowRect(&ClientRect);
	m_ListSize.cx = ClientRect.Width()/4;
	m_ListSize.cy = (long)(ClientRect.Height()/1.5);

/*
	try
	{
		if(m_pRsBrowseResult == NULL)
		{
			m_pRsBrowseResult.CreateInstance(__uuidof(Recordset));
			m_pRsBrowseResult->CursorLocation = adUseClient;
		}
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

*/	pFont = GetFont();
	//
	//�������ֱ������List�ؼ�
	//
	m_TableNameList.Create(LBS_NOTIFY|WS_VISIBLE|WS_BORDER|WS_CHILD|WS_VSCROLL|LBS_NOINTEGRALHEIGHT,
									ClientRect,this,IDC_MATERIALNAME_LIST);
	//	m_MaterialNameList.SetFont(pFont);
	m_TableNameList.SetFont(pFont);
	m_TableNameList.ShowWindow(SW_SHOW);
	//����ɾ��ȫ����¼�İ�ť.
	this->m_ctlDeleteAll.Create("ȫ��ɾ��", BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,
								ClientRect,this,IDC_DELETE_SEL_MATERIAL);
	m_ctlDeleteAll.SetFont( pFont );
	m_ctlDeleteAll.SetTooltipText("ɾ��ѡ��ı��е����м�¼��", TRUE);

	//��ʼ��Ҫ��ʾ�ĸ�������
	if( !UpdateTblNameList() )
	{
		return false;
	}
	// ���¿ؼ�������
	//��ʼΪ��һ����¼��
	m_TableNameList.SetCurSel(0);
	m_nCurTblID = m_TableNameList.GetItemData(0);
	m_TableNameList.GetText(0, this->m_strDescTblName);
	UpdateDataGrid();

	// ���¸����ؼ���λ��
	UpdateControlsPos();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
///////////////////////////////////////////////
//
// ���¸����ؼ���λ��
//
BOOL CDlgShowAllTable::UpdateControlsPos()
{
	CRect ClientRect;
	GetClientRect(&ClientRect);
//	int ctlWidth, left, right;
	GetClientRect(&ClientRect);

	//
	// ����ɺ����϶�������Ϊ�վ�������
	//
	if(this->m_HDragArea.IsRectEmpty())
	{
		this->m_HDragArea=ClientRect;
		this->m_HDragArea.left=ClientRect.left+m_ListSize.cx;
		this->m_HDragArea.right=this->m_HDragArea.left+5;
	}

	//������ʾ�����DataGrid�ؼ�.
	ClientRect.left=m_HDragArea.right;
	this->m_ResultBrowseControl.MoveWindow(&ClientRect);

	//������ʾ�������Ƶ�λ��
	GetClientRect(&ClientRect);
	ClientRect.right = ClientRect.left+m_ListSize.cx;
	ClientRect.bottom = ClientRect.top+m_ListSize.cy;
	this->m_TableNameList.MoveWindow(&ClientRect);

	//��������
	this->m_VDragArea = ClientRect;
	this->m_VDragArea.top = m_VDragArea.bottom;
	this->m_VDragArea.bottom = this->m_VDragArea.top+5;

	//ɾ����ť.
	ClientRect.top = m_VDragArea.bottom + 10;
	ClientRect.bottom = ClientRect.top + 20;
	short width = ClientRect.Width();
	ClientRect.left += width/3;
	ClientRect.right -= width/3;
	this->m_ctlDeleteAll.MoveWindow(&ClientRect);

	//�ػ�����.
	this->Invalidate();
	return true;
}
//
//��ʼ��Ҫ��ʾ�ĸ�������.
//
bool CDlgShowAllTable::UpdateTblNameList()
{

/*	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	pRs->CursorLocation = adUseClient;
*/	CString strSql, strTemp;

	if( m_pRsTableList == NULL )
	{
		m_pRsTableList.CreateInstance(__uuidof(Recordset));
	}
	if( m_pRsTableList->State == adStateOpen )
	{
		m_pRsTableList->Close();
	}
	strSql = "SELECT * FROM [TableINFO] WHERE SEQ IS NOT NULL ORDER BY SEQ";
	try
	{
		this->m_TableNameList.ResetContent();


		m_pRsTableList->Open(_bstr_t(strSql), theApp.m_pConIPEDsort.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		int count = m_pRsTableList->GetRecordCount();
		if( m_pRsTableList->GetRecordCount() <= 0 )
		{
			AfxMessageBox("û������!");
			return false;
		}
		//������������뵽�б����
		int Index;
		long dwItemData;
		for(; !m_pRsTableList->adoEOF; m_pRsTableList->MoveNext() )
		{
			strTemp = vtos(m_pRsTableList->GetCollect(_variant_t("ShortDescribe")));
			if( !strTemp.IsEmpty() )
			{
				Index = m_TableNameList.AddString(strTemp);
				if( Index != -1 )
				{
					//Ωһ��ʾһ��������.
					dwItemData = vtoi(m_pRsTableList->GetCollect(_variant_t("SEQ")));
					m_TableNameList.SetItemData(Index, dwItemData);
				}
			}
		}
//		this->SetRecordset(m_pRsTableList);
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return false;
	}

	return true;
}
//
// ����ڿ��϶�������͸ı������Ԫ�Ĵ�С��λ��
//
void CDlgShowAllTable::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( this->m_IsInHDragArea && this->m_LButtonPt != point )
	{
		//�ں����϶�������.
		m_HDragArea.OffsetRect(point.x - m_LButtonPt.x, 0 );
		m_ListSize.cx += (point.x - m_LButtonPt.x);
		//���¸��ؼ���λ��
		UpdateControlsPos();
		m_LButtonPt = point;
	}
	else
	{
		if( m_IsInVDragArea && m_LButtonPt != point )
		{
			//�������϶�������.
			m_ListSize.cy += (point.y-m_LButtonPt.y);
			//���¸��ؼ���λ��
			UpdateControlsPos();
			m_LButtonPt = point;
		}
	}
	CDataShowDlg::OnMouseMove(nFlags, point);
}
// �ڿ��϶�������ı���ʸ����
BOOL CDlgShowAllTable::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR hCursor;
	CPoint   pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);
	//
	// �ڿ��϶�������ı���ʸ����
	//
	if( this->m_HDragArea.PtInRect(pt) )
	{
		hCursor = LoadCursor(NULL,IDC_SIZEWE);
		SetCursor(hCursor);
		::DeleteObject(hCursor);
		return TRUE;
	}
	else
	{
		if( this->m_VDragArea.PtInRect(pt) )
		{
			hCursor = LoadCursor(NULL, IDC_SIZENS);
			SetCursor(hCursor);
			::DeleteObject(hCursor);
			return TRUE;
		}
	}

	return CDataShowDlg::OnSetCursor(pWnd, nHitTest, message);
}

void CDlgShowAllTable::OnSize(UINT nType, int cx, int cy) 
{
	CDataShowDlg::OnSize(nType, cx, cy);

	if( IsWindow(this->m_TableNameList.GetSafeHwnd()) && IsWindow(this->m_ResultBrowseControl.GetSafeHwnd()) )
	{
		//�����Сʱ���ı䴰�ڴ�С��
		this->m_HDragArea.SetRectEmpty();
		this->m_VDragArea.SetRectEmpty();

		CRect ClientRect;
		GetWindowRect(&ClientRect);
		m_ListSize.cx = ClientRect.Width()/4;
		m_ListSize.cy = (long)(ClientRect.Height()/1.5);
		//

		this->UpdateControlsPos();
	}	
	
}
//Ϊ���϶������������ɫ
void CDlgShowAllTable::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
	CBrush hBrush;
	if( !this->m_HDragArea.IsRectEmpty() && !this->m_VDragArea.IsRectEmpty() )
	{
		hBrush.CreateSolidBrush(RGB(127,127,255));
		dc.FillRect(m_HDragArea,&hBrush);
		dc.FillRect(m_VDragArea,&hBrush);
	}
}
//
void CDlgShowAllTable::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect ClientRect;

	if( this->m_HDragArea.PtInRect(point) )
	{
		this->SetCapture();
		this->m_IsInHDragArea = true;
		GetClientRect(&ClientRect);
		ClientToScreen(ClientRect);

		//�޶�����ƶ��ڿͻ�����
		ClipCursor(ClientRect);
	}
	else
	{
		if( this->m_VDragArea.PtInRect(point) )
		{
			this->SetCapture();
			this->m_IsInVDragArea = true;
			GetClientRect(&ClientRect);
			ClientToScreen(&ClientRect);

			//�޶�����ƶ��ڿͻ�����
			ClipCursor(ClientRect);
		}
	}
	this->m_LButtonPt = point;

	CDataShowDlg::OnLButtonDown(nFlags, point);
}

void CDlgShowAllTable::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_IsInVDragArea || m_IsInHDragArea )
	{
		m_IsInVDragArea = false;
		m_IsInHDragArea = false;
		ReleaseCapture();
		ClipCursor(NULL);
	}
	CDataShowDlg::OnLButtonUp(nFlags, point);
}
//////////////////////////////////////////////////
//
// ��Ӧ��ͬ�ı��ѡ��ı�
//
void CDlgShowAllTable::OnTableSelChange()
{
//	CString strValue;
	int bT = this->m_TableNameList.GetCurSel();
	if( bT != CB_ERR )
	{
		m_nCurTblID = m_TableNameList.GetItemData(bT);
		m_TableNameList.GetText( bT, m_strDescTblName);
		UpdateDataGrid();
	}
}
//////////////////////////////////////////////////
//
// ������ѡ������ݡ�
//
void CDlgShowAllTable::UpdateDataGrid()
{
	//��Ӧ�������ļ�¼��.
	_RecordsetPtr m_pRsBrowseResult;
	try
	{
		if(m_pRsBrowseResult == NULL)
		{
			m_pRsBrowseResult.CreateInstance(__uuidof(Recordset));
			m_pRsBrowseResult->CursorLocation = adUseClient;
		}
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return ;
	}

	try
	{	//�����û�м�¼��
		if( m_pRsTableList->GetRecordCount() <= 0 )
		{
			return ;
		}

		CString strSql,
				strTblName;
		int     FlgDB;
		//�ñ��Ӧ��ID�����ҡ�
		strSql.Format("SEQ=%d", m_nCurTblID);
		try
		{
			m_pRsTableList->MoveFirst();
			m_pRsTableList->Find(_bstr_t(strSql), 0, adSearchForward);
		}
		catch(_com_error)
		{
			AfxMessageBox("û�иñ�");
			return;
		}
		if( m_pRsTableList->adoEOF || m_pRsTableList->BOF )
		{
			return ;
		}
		strTblName = vtos(m_pRsTableList->GetCollect(_variant_t("ManuTBName")) );
		FlgDB = vtoi(m_pRsTableList->GetCollect(_variant_t("FromDBMark")) );

		try
		{
			if( m_pRsBrowseResult->State == adStateOpen )
			{
				m_pRsBrowseResult->Close();
			}
		}
		catch(_com_error)
		{
			m_pRsBrowseResult->Cancel();
			m_pRsBrowseResult->Close();
		}
		
		_ConnectionPtr ptr = theApp.m_pConnection;
		strSql = "SELECT * FROM ["+strTblName+"] ";
		switch( FlgDB )
		{
		case 0:/*"AutoIPED.mdb":*/
			if ( !tableExists(ptr, strTblName) )
			{
				ptr = theApp.m_pConAllPrj;
			}
			strSql += " WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
			m_pRsBrowseResult->Open(_bstr_t(strSql), ptr.GetInterfacePtr(),
							adOpenStatic, adLockOptimistic, adCmdText);
			break;
		case 1://"IPEDCODE.mdb":*/
			m_pRsBrowseResult->Open(_bstr_t(strSql), theApp.m_pConnectionCODE.GetInterfacePtr(),
							adOpenStatic, adLockOptimistic, adCmdText);
			break;
		case 2:/*"Material.MDB":*/
			m_pRsBrowseResult->Open(_bstr_t(strSql), theApp.m_pConMaterial.GetInterfacePtr(),
							adOpenStatic, adLockOptimistic, adCmdText);
			break;
		}
		//������ʾ������;
		this->SetRecordset(m_pRsBrowseResult);

		//����ID��ENGINID��
		this->SetHideColumns(_T("ID|ENGINID"));
		//
		this->SetDataGridCaption(strTblName);
		//�����ֶεı���Ϳ��
		this->SetFieldCaption(strTblName);
		
		//����Ĭ���ֶ�ֵ.
		CDataShowDlg::tag_DefaultFieldValue DefaultFieldValue;

		DefaultFieldValue.strFieldName=_T("EnginID");
		DefaultFieldValue.varFieldValue=EDIBgbl::SelPrjID;
		this->SetDefaultFieldValue(&DefaultFieldValue,1);

	//	pdlg->SetDefaultFieldValue(&DefaultFieldValue,1);

	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return;
	}
}
//
//ɾ�����е����м�¼
void CDlgShowAllTable::OnDeleteTblAll()
{
	try
	{	//�����û�м�¼��
		if( m_pRsTableList->GetRecordCount() <= 0 )
		{
			return ;
		}

		CString strSql,
				strTblName;
		int     FlgDB;
	//	strSql = "ShortDescribe='"+m_strDescTblName+"'";
		//�ñ��Ӧ��ID�����ҡ�
		strSql.Format("SEQ=%d", m_nCurTblID);

		try
		{
			m_pRsTableList->MoveFirst();
			m_pRsTableList->Find(_bstr_t(strSql), 0, adSearchForward);
		}
		catch(_com_error)
		{
			AfxMessageBox("û�иñ�");
			return;
		}
		if( m_pRsTableList->adoEOF || m_pRsTableList->BOF )
		{
			return ;
		}
		strTblName = vtos(m_pRsTableList->GetCollect(_variant_t("ManuTBName")) );
		FlgDB = vtoi(m_pRsTableList->GetCollect(_variant_t("FromDBMark")) );

		if( IDNO == AfxMessageBox("�Ƿ�ɾ�� '"+m_strDescTblName+"' �е����м�¼?", MB_YESNO+MB_DEFBUTTON2) )
		{
			return ;
		}

		_ConnectionPtr ptr = theApp.m_pConnection;
		strSql = "DELETE * FROM ["+strTblName+"] ";
		switch( FlgDB )
		{
		case 0://"AutoIPED.mdb"
			if ( !tableExists(ptr, strTblName) )
			{
				ptr = theApp.m_pConAllPrj;
			}

			strSql += " WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
			ptr->Execute(_bstr_t(strSql), NULL, adCmdText);
			break;
		case 1://"IPEDCODE.mdb"
			theApp.m_pConnectionCODE->Execute(_bstr_t(strSql), NULL, adCmdText);
			break;
		case 2://"Material.MDB"
			theApp.m_pConMaterial->Execute(_bstr_t(strSql), NULL, adCmdText);
			break;
		}
		UpdateDataGrid();
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return ;
	}
}