// AutoIPEDView.cpp : implementation of the CAutoIPEDView class
//

#include "stdafx.h"
#include "AutoIPED.h"

#include "AutoIPEDDoc.h"
#include "AutoIPEDView.h"

#include "edibgbl.h"
#include "calc.h"
#include "SelEngVolDll.h"
#include "newproject.h"
#include "FrmFolderLocation.h"
#include "ToExcel.h"
#include "ExplainTable.h"
#include "DlgOption.h"
#include "InputOtherData.h"
#include "TotalTableIPED.h"
#include "MainFrm.h"

#include "DlgAutoTotal.h"

#include "excel9.h"
#include <comutil.h>

#include "EditOriginalData.h"
#include "EditOilPaintDataDlg.h"

#include "DlgSelCritDB.h"
#include "DataShowDlg.h"		//��ʾ���ݵĶԻ���
#include "ImportFromPreCalcXLSDlg.h"	// ���ⲿ����ԭʼ���ݶԻ���
#include "ImportDirFromXlsDlg.h"		// ���ⲿ������Ŀ¼�Ի���
#include "ImportPaintXLSDlg.h"			// ���ⲿ��������ԭʼ���ݶԻ���
#include "DlgPaintRuleModi.h"

#include "DlgShowAllTable.h"
#include "DlgSelCityWeather.h"

#include "CalcSupportSpanDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAutoIPEDApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDView

IMPLEMENT_DYNCREATE(CAutoIPEDView, CEditView)

BEGIN_MESSAGE_MAP(CAutoIPEDView, CEditView)
	//{{AFX_MSG_MAP(CAutoIPEDView)
	ON_COMMAND(ID_MENUITEM61, OnMenuitem61)
	ON_COMMAND(ID_MENUITEM31, OnEditOriginalDataMenu)
	ON_COMMAND(ID_MENUITEM33, OnMenuitem33)
	ON_COMMAND(ID_MENUITEM34, OnMenuitem34)
	ON_COMMAND(ID_MENUITEM36, OnMenuitem36)
	ON_COMMAND(ID_MENUITEM41, OnMenuitem41)
	ON_COMMAND(ID_MENUITEM42, OnMenuitem42)
	ON_COMMAND(ID_MENUITEM43, OnMenuitem43)
	ON_COMMAND(ID_MENUITEM44, OnMenuitem44)
	ON_COMMAND(ID_MENUITEM45, OnMenuitem45)
	ON_COMMAND(ID_MENUITEM46, OnMenuitem46)
	ON_COMMAND(ID_MENUITEM51, OnMenuitem51)
	ON_COMMAND(ID_MENUITEM510, OnMenuitem510)
	ON_COMMAND(ID_MENUITEM511, OnMenuitem511)
	ON_COMMAND(ID_MENUITEM52, OnMenuitem52)
	ON_COMMAND(ID_MENUITEM53, OnMenuitem53)
	ON_COMMAND(ID_MENUITEM54, OnMenuitem54)
	ON_COMMAND(ID_MENUITEM55, OnMenuitem55)
	ON_COMMAND(ID_MENUITEM56, OnMenuitem56)
	ON_COMMAND(ID_MENUITEM57, OnMenuitem57)
	ON_COMMAND(ID_MENUITEM58, OnMenuitem58)
	ON_COMMAND(ID_MENUITEM59, OnMenuitem59)
	ON_COMMAND(ID_MENUITEM610, OnMenuitem610)
	ON_COMMAND(ID_MENUITEM611, OnMenuitem611)
	ON_COMMAND(ID_MENUITEM612, OnMenuitem612)
	ON_COMMAND(ID_MENUITEM62, OnMenuitem62)
	ON_COMMAND(ID_MENUITEM63, OnMenuitem63)
	ON_COMMAND(ID_MENUITEM64, OnMenuitem64)
	ON_COMMAND(ID_MENUITEM65, OnMenuitem65)
	ON_COMMAND(ID_MENUITEM66, OnMenuitem66)
	ON_COMMAND(ID_MENUITEM67, OnMenuitem67)
	ON_COMMAND(ID_MENUITEM68, OnMenuitem68)
	ON_COMMAND(ID_MENUITEM69, OnMenuitem69)
	ON_COMMAND(ID_MENUITEM71, OnMenuitem71)
	ON_COMMAND(ID_MENUITEM72, OnMenuitem72)
	ON_COMMAND(ID_MENUITEM73, OnMenuitem73)
	ON_COMMAND(ID_MENUITEM74, OnMenuitem74)
	ON_COMMAND(ID_MENUITEM613, OnMenuitem613)
	ON_WM_SIZE()
	ON_COMMAND(IDM_SEL_PRJ_VOLUME, OnSelPrjVolume)
	ON_COMMAND(IDM_COPY_FROM_EXIST_PRJ, OnCopyFromExistPrj)
	ON_COMMAND(IDM_FILE_SELDIR, OnFileSeldir)
	ON_COMMAND(IDM_OPTION, OnOption)
	ON_COMMAND(IDM_SELECT_DATABASE, OnSelectCriterionDatabase)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM51, OnUpdateMenuitem51)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM55, OnUpdateMenuitem55)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM61, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM59, OnUpdateMenuitem59)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM510, OnUpdateMenuitem510)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM511, OnUpdateMenuitem511)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM56, OnUpdateMenuitem56)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM57, OnUpdateMenuitem57)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM58, OnUpdateMenuitem58)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM52, OnUpdateMenuitem52)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM53, OnUpdateMenuitem53)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM54, OnUpdateMenuitem54)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM33, OnUpdateMenuitem33)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM41, OnUpdateExplainTblPass)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_AUTO_TOTAL, OnButtonAutoTotal)
	ON_COMMAND(IDM_IMPORT_ORIGINAL_DATA, OnImportOriginalData)
	ON_COMMAND(IDM_DISPLAY_CALERROR_INFO, OnDisplayCalErrorInfo)
	ON_COMMAND(IDM_IMPORT_DIR_DATA, OnImportDirData)
	ON_COMMAND(ID_MODI_RULE1, OnModiRule1)
	ON_COMMAND(IDM_MATERIAL_DATABASE, OnMaterialDatabase)
	ON_COMMAND(ID_CLOSE_ALL_WINDOW, OnCloseAllWindow)
	ON_COMMAND(IDM_IMPORT_PAINT_ORIGINAL_DATA, OnImportPaintData)
	ON_UPDATE_COMMAND_UI(IDM_IMPORT_PAINT_ORIGINAL_DATA, OnUpdateImportPaintData)
	ON_COMMAND(ID_NEW_OLD_MATERIAL, OnNewOldMaterial)
	ON_COMMAND(IDM_OUT_VIEW, OnOutView)
	ON_COMMAND(IDM_CHECK_COHERENCE, OnCheckCoherence)
	ON_COMMAND(IDM_THICKNESS_REGULAR, OnThicknessRegular)
	ON_COMMAND(IDM_SHOW_ALL_TABLE, OnShowAllTable)
	ON_COMMAND(ID_CITY_WEATHER, OnCityWeather)
	ON_COMMAND(IDM_CALC_SUPPORT_SPAN, OnCalcSupportSpan)
	ON_COMMAND(ID_MENUI_RING_NAME, OnMenuiRingName)
	ON_UPDATE_COMMAND_UI(IDM_OUT_VIEW, OnUpdateOutView)
	ON_COMMAND(ID_ALL_MINIMIZE, OnAllMinimize)
	ON_UPDATE_COMMAND_UI(ID_ALL_MINIMIZE, OnUpdateAllMinimize)
	ON_COMMAND(ID_MENU_REPLACE_UNIT, OnMenuReplaceUnit)
	ON_COMMAND(IDM_NEW_OLD_UNIT, OnNewOldUnit)
	ON_COMMAND(IDM_IMPORT_OTHER_IPED_UESOFT, OnImportOtherIpedUE)
	ON_COMMAND(IDM_IMPORT_OTHER_IPED_SW, OnImportOtherIpedSw)
	ON_COMMAND(IDM_IMPORT_OTHER_IPED_JS, OnImportOtherIpedJs)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM62, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM63, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM64, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM65, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM66, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM68, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM69, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM610, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM611, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM613, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM73, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM31, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM36, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM71, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(IDM_IMPORT_DIR_DATA, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(IDM_IMPORT_ORIGINAL_DATA, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM34, OnUpdateMenuitem33)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM35, OnUpdateMenuitem33)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM42, OnUpdateExplainTblPass)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM43, OnUpdateExplainTblPass)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM44, OnUpdateExplainTblPass)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM45, OnUpdateExplainTblPass)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM46, OnUpdateExplainTblPass)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM67, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MODI_RULE1, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM612, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(IDM_THICKNESS_REGULAR, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(IDM_SHOW_ALL_TABLE, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_AUTO_TOTAL, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENUI_RING_NAME, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(IDM_CHECK_COHERENCE, OnUpdateProjectConnectMenu)
	ON_UPDATE_COMMAND_UI(ID_MENU_REPLACE_UNIT, OnUpdateProjectConnectMenu)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDView construction/destruction

CAutoIPEDView::CAutoIPEDView()
{
	m_passState51=false;
	m_stateAutoTotal=true;
	m_pMaterialDlg = NULL;
}

CAutoIPEDView::~CAutoIPEDView()
{
	CString strKey;
	CObject *pObject;
	POSITION pos;
	
	pos=m_ModelessDlgMap.GetStartPosition();

	while(pos!=NULL)
	{
		m_ModelessDlgMap.GetNextAssoc(pos,strKey,pObject);

		delete pObject;
	}

	m_ModelessDlgMap.RemoveAll();

	// �ö�̬��ʵ�ֵ���ʾ���ϵĶԻ���
	if ( m_pMaterialDlg != NULL )
	{
		delete m_pMaterialDlg;
		m_pMaterialDlg = NULL;
	}

}

BOOL CAutoIPEDView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    BOOL bRet;
	bRet=CEditView::PreCreateWindow(cs);

	cs.style|=ES_READONLY;
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDView drawing

void CAutoIPEDView::OnDraw(CDC* pDC)
{
	CAutoIPEDDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDView printing

BOOL CAutoIPEDView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAutoIPEDView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAutoIPEDView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDView diagnostics

#ifdef _DEBUG
void CAutoIPEDView::AssertValid() const
{
	CEditView::AssertValid();
}

void CAutoIPEDView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CAutoIPEDDoc* CAutoIPEDView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAutoIPEDDoc)));
	return (CAutoIPEDDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDView message handlers

/////////////////////////////////////////////////
//
// ��Ӧ�����²��ϲ����⡱
//
void CAutoIPEDView::OnMenuitem61() 
{

	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_MAT WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("���²��ϲ�����"),_T("A_MAT"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

////////////////////////////////////////////////
//
// ��Ӧ���༭ԭʼ���ݡ��˵�
//
void CAutoIPEDView::OnEditOriginalDataMenu() 
{
	//2005.2.24
	CString strCaption = "�༭ԭʼ����" ;
	CWnd* pWnd;
	pWnd = GetModelessDlgInMap( strCaption );
	if( pWnd != NULL )
	{
		pWnd->SetForegroundWindow();
		pWnd->ShowWindow( SW_RESTORE );
		return;
	}
	CEditOriginalData* pEditOriginal;
	pEditOriginal = new CEditOriginalData(this);
	pEditOriginal->SetCurrentProjectConnect(theApp.m_pConnection);
	pEditOriginal->SetPublicConnect(theApp.m_pConnectionCODE);
	pEditOriginal->SetProjectID(EDIBgbl::SelPrjID);
	pEditOriginal->SetIsAutoSelectMatOnRoll(bIsAutoSelectPre);
	pEditOriginal->Create(CEditOriginalData::IDD, this);
	m_ModelessDlgMap.SetAt(strCaption, pEditOriginal);
	//
	/*by zsy
	CEditOriginalData EditOriginalDataDlg;

	EditOriginalDataDlg.SetCurrentProjectConnect(theApp.m_pConnection);
	EditOriginalDataDlg.SetPublicConnect(theApp.m_pConnectionCODE);
	EditOriginalDataDlg.SetProjectID(EDIBgbl::SelPrjID);

	EditOriginalDataDlg.SetIsAutoSelectMatOnRoll(bIsAutoSelectPre);

	EditOriginalDataDlg.DoModal();

*/	//
	// дPASS ��
	//
	_RecordsetPtr IRecordset_Pass;
	CString SQL,strTemp;
	int Pos;
	_variant_t varTemp;
	HRESULT hr;
	hr=IRecordset_Pass.CreateInstance(__uuidof(Recordset));

	if(FAILED(hr))
	{
		ReportExceptionErrorLV1(_com_error((HRESULT)hr));
		return;
	}

	try
	{
		IRecordset_Pass->CursorLocation=adUseClient;

		SQL.Format(_T("SELECT * FROM PASS WHERE EnginID='%s'"), EDIBgbl::SelPrjID);

		IRecordset_Pass->Open(_variant_t(SQL),_variant_t((IDispatch*)theApp.m_pConnection),
							  adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return;
	}

	try
	{
		if ( IRecordset_Pass->GetRecordCount() <= 0 )
		{	//�ڿ��Ʋ������̵ı���û�е�ǰ���̵ļ�¼.
			return;
		}
		Pos=0;
		IRecordset_Pass->MoveFirst();
		while(!IRecordset_Pass->adoEOF)
		{
			Pos++;

			varTemp=IRecordset_Pass->GetCollect(_variant_t("pass_mod1"));

			if(varTemp.vt==VT_NULL)
				strTemp=_T("");
			else
			{
				strTemp=(LPCTSTR)(_bstr_t)varTemp;
				strTemp.TrimLeft();
				strTemp.TrimRight();

				strTemp.MakeUpper();
			}

			if(strTemp==_T("E_CALTAB"))
				break;

			IRecordset_Pass->MoveNext();
		}

		if(IRecordset_Pass->adoEOF)
		{
			ReportExceptionErrorLV1(_T(" cann't find pass_mod1==E_PREDAT"));
			return;
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return;
	}

	try
	{

		IRecordset_Pass->MoveFirst();

		while(Pos>0)
		{
			IRecordset_Pass->PutCollect(_variant_t("pass_mark1"),_variant_t("T"));

			IRecordset_Pass->MoveNext();

			Pos--;
		}

		while(!IRecordset_Pass->adoEOF)
		{
			IRecordset_Pass->PutCollect(_variant_t("pass_mark1"),_variant_t("F"));
			IRecordset_Pass->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return;
	}
}

//////////////////////////////////////////////////
//
// ��Ӧ�����������˵�
//
void CAutoIPEDView::OnMenuitem33() 
{
	CString SQL;

	SQL.Format(_T("SELECT * FROM PRE_CALC WHERE EnginID='%s' ORDER BY ID"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("������"),_T("PRE_CALC"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

void CAutoIPEDView::OnMenuitem34() 
{
	// TODO: Add your command handler code here
	try
	{
		ToExcel toexcel;
		toexcel.Menu34();
	}
	catch(_com_error& e)
	{
		ReportExceptionErrorLV1(e);
		return;
	}
}

///////////////////////////////////////////////
//
// ��Ӧ���༭����ԭʼ���ݡ��˵�
//
void CAutoIPEDView::OnMenuitem36() 
{
	//2005.2.24  ���Ի����Ϊ��ģʽ��
	CString strCaption = "�༭����ԭʼ����";
	CWnd* pWnd;
	pWnd = GetModelessDlgInMap( strCaption );
	if( pWnd != NULL )
	{
		pWnd->SetForegroundWindow();
		pWnd->ShowWindow( SW_RESTORE );
		return;
	}
	CEditOilPaintDataDlg * pEditOilPaint;
	pEditOilPaint = new CEditOilPaintDataDlg(this);

	pEditOilPaint->SetCurrentProjectConnect(theApp.m_pConnection);
	pEditOilPaint->SetPublicConnect(theApp.m_pConnectionCODE);
	pEditOilPaint->SetProjectID(EDIBgbl::SelPrjID);
	pEditOilPaint->Create(CEditOilPaintDataDlg::IDD,this);

	m_ModelessDlgMap.SetAt( strCaption, pEditOilPaint );
	//
	/*
	CEditOilPaintDataDlg EditOilPaintDataDlg;

	EditOilPaintDataDlg.SetCurrentProjectConnect(theApp.m_pConnection);
	EditOilPaintDataDlg.SetPublicConnect(theApp.m_pConnectionCODE);
	EditOilPaintDataDlg.SetProjectID(EDIBgbl::SelPrjID);

	EditOilPaintDataDlg.DoModal();

	*/

}

//���ɱ���˵����
void CAutoIPEDView::OnMenuitem41() 
{
	CExplainTable tabl;
	tabl.m_Pconnection = theApp.m_pConnection;
	tabl.m_pConnectionCODE = theApp.m_pConnectionCODE;
	CString sql;
	sql.Format("select ���,����,����,���,�ص�,�¶�T,�¶�T0,�ڱ��²���,�ڱ��º�,"
			   "�ڵ����,��ȫ���,�Ᵽ�²���,�Ᵽ�º�,�ⵥ���,��ȫ���,��������,"
			   "������,���������,����ȫ���,������,������¶�,��ע,EnginID from "
			   "e_preexp where EnginID='%s' ORDER BY ���",EDIBgbl::SelPrjID);

	if( tabl.CreateBWTable())
	{
		m_strMessage+="\n���ɱ���˵����ɹ�!";//10/13 pfg
		if(m_autoTotal.m_autoTotal[11]||m_stateAutoTotal )//10/13 pfg
		{
			OpenATableToShow(sql, theApp.m_pConnection, _T("����˵����"),"e_preexp" );
		}
	}
	else//10/14pfg
	{
		//������˵��������ʧ��ʱ��������صı�Ͳ����Զ�����
		m_autoTotal.m_autoTotal[0]=false;//10/14pfg
		int i;//10/14pfg
		for(i=2;i<15;i++)//10/14pfg
		{
			m_autoTotal.m_autoTotal[i]=false;//10/14pfg
		}
	}

	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
}
//�༭����˵����
void CAutoIPEDView::OnMenuitem42() 
{
	CExplainTable ETbl;
	ETbl.m_Pconnection = theApp.m_pConnection;
	if( !ETbl.EditBWTable() )
	{
		return;
	}
	CString sql;
	sql.Format("select ���,����,����,���,�ص�,�¶�T,�¶�T0,�ڱ��²���,�ڱ��º�,�ڵ����,��ȫ���,�Ᵽ�²���,�Ᵽ�º�,�ⵥ���,��ȫ���,��������,������,���������,����ȫ���,������,������¶�,��ע,EnginID from e_preexp where EnginID='%s' ORDER BY ��� ",EDIBgbl::SelPrjID);
	OpenATableToShow(sql, theApp.m_pConnection, _T("����˵����"),"e_preexp");
}
//�������˵����
void CAutoIPEDView::OnMenuitem43() 
{
	CExplainTable pTbl;
	pTbl.m_Pconnection = theApp.m_pConnection;
	if( !pTbl.IsPass("pass_mod1", "P_PREEXP", "pass_last1", "Pass_mark1") )
	{
		AfxMessageBox("�������, �������ɱ���˵����!");
		return;
	}
	ToExcel toexcel;
	if(toexcel.Menu43())//10/13pfg
	{
		if( m_stateAutoTotal )
		{
			AfxMessageBox("����˵��������Excel�ɹ�!");
		}
		m_strMessage+="\n����˵��������Excel�ɹ�!";//10/13pfg
	}
	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
}
//��������˵����
void CAutoIPEDView::OnMenuitem44() 
{
	CExplainTable tabl;
	tabl.m_Pconnection = theApp.m_pConnection;
	tabl.m_pConnectionCODE = theApp.m_pConnectionCODE;
	CString sql;
//	sql.Format("select ���,����,PAI_SIZE as ���,PAI_AMOU as �ܵ����Ȼ��豸��,�������,������,�������,\
//		��ÿ������,�׺ϼ�����,������,PAI_C_FACE as ������ɫ,�������,��ÿ������,��ϼ�����,\
//		��ע,EnginID from e_paiexp where EnginID='"+EDIBgbl::SelPrjID+"' ORDER BY ���");
	sql.Format("select ���,����,PAI_SIZE,PAI_AMOU,�������,������,�������,\
		��ÿ������,�׺ϼ�����,������,PAI_C_FACE,�������,��ÿ������,��ϼ�����,\
		��ע,EnginID from e_paiexp where EnginID='"+EDIBgbl::SelPrjID+"' ORDER BY ���");

	if( tabl.CreatePaintTable())
	{
		m_strMessage+="\n��������˵����ɹ�!";//10/13 pfg
		if(m_autoTotal.m_autoTotal[12]||m_stateAutoTotal)//10/13 pfg
		{
			OpenATableToShow(sql, theApp.m_pConnection, _T("����˵����"),"e_paiexp");
		}
	}
	else//10/14/pfg
	{
		//��������˵����ʧ�ܺ�������ص�������Ͳ����Զ�����
		m_autoTotal.m_autoTotal[4]=false;
		m_autoTotal.m_autoTotal[7]=false;
		m_autoTotal.m_autoTotal[9]=false;
		m_autoTotal.m_autoTotal[12]=false;
		m_autoTotal.m_autoTotal[14]=false;

	}

	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);

}
//�༭����˵����
void CAutoIPEDView::OnMenuitem45() 
{
	CExplainTable ETbl;
	ETbl.m_Pconnection = theApp.m_pConnection;
	if( !ETbl.EditPaint() )
	{
		return;
	}
	CString sql;
//	sql.Format("select ���,����,PAI_SIZE as ���,PAI_AMOU as �ܵ����Ȼ��豸��,�������,������,�������,\
//		��ÿ������,�׺ϼ�����,������,PAI_C_FACE as ������ɫ,�������,��ÿ������,��ϼ�����,\
//		��ע,EnginID from e_paiexp where EnginID='"+EDIBgbl::SelPrjID+"' ORDER BY ���");

	sql.Format("select ���,����,PAI_SIZE,PAI_AMOU,�������,������,�������,\
		��ÿ������,�׺ϼ�����,������,PAI_C_FACE,�������,��ÿ������,��ϼ�����,\
		��ע,EnginID from e_paiexp where EnginID='"+EDIBgbl::SelPrjID+"' ORDER BY ���");

	OpenATableToShow(sql, theApp.m_pConnection, _T("����˵����"),"e_paiexp");
}
//�������˵����
void CAutoIPEDView::OnMenuitem46() 
{
	CExplainTable pTbl;
	pTbl.m_Pconnection  = theApp.m_pConnection;
	if(!pTbl.IsPass("pass_mod2", "P_PAIEXP", "pass_last2", "pass_mark2") )
	{
		AfxMessageBox("���������������������˵����!");
		return;
	}
	ToExcel toexcel;
	if(toexcel.Menu46())//10/13pfg
	{
		if(m_stateAutoTotal)
		{
			AfxMessageBox("����˵��������Excel�гɹ�!");
		}
		m_strMessage+="\n����˵��������Excel�гɹ�!";
	}
	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
}


//���ɱ��²��ϻ��ܱ�
void CAutoIPEDView::OnMenuitem51() 
{
	CString strSQL;
	CTotalTableIPED preast;
	preast.m_Pconnection=theApp.m_pConnection;
	preast.m_pConnectionCODE=theApp.m_pConnectionCODE;
	if(EDIBgbl::SelPrjID.IsEmpty())	//9/23
	{
		AfxMessageBox("δѡ�񹤳�!");
		return;
	}
	try
	{
		if(preast.c_precol())
		{
			m_strMessage+="\n���ɱ��²��ϻ��ܱ�(��׼+����)�ɹ�!";//10/13pfg
			if(m_autoTotal.m_autoTotal[13]||m_stateAutoTotal)//10/13 pfg
			{
				if(m_stateAutoTotal)//10/13pfg
				{
					AfxMessageBox("���ɱ��²��ϻ��ܱ�(��׼+����)�ɹ�!");//10/13pfg
				}
//				strSQL="select ID, col_num as ���,col_name as ������,col_amount as �������,gen_amount as ʩ������, col_size as ���,col_unit as ��λ, col_mark as ��ע from e_precol where enginid='"+EDIBgbl::SelPrjID+"'";
				strSQL="select * from e_precol where enginid='"+EDIBgbl::SelPrjID+"'";
				OpenATableToShow(strSQL,theApp.m_pConnection,_T("���²��ϻ��ܱ�(��׼+����)"),_T("E_PRECOL"));

			}
		}
		else//10/14 pfg
		{
			m_autoTotal.m_autoTotal[8]=false;//10/14pfg
			m_autoTotal.m_autoTotal[13]=false;//10/14pfg
		}
	}

	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
}

void CAutoIPEDView::OnMenuitem510() 
{
	CString sql;
   //	sql.Format("select �Ᵽ�²���,���,�Ᵽ�º�,size_unit,��ȫ���,size_mark,EnginID from e_presiz where EnginID='%s'",EDIBgbl::SelPrjID);//pfg 2005/1/6
 //   sql.Format("select �Ᵽ�²���,���,�Ᵽ�º�,size_unit as ��λ,��ȫ���,size_amoun as ����,size_mark as ��ע,EnginID from e_presiz where EnginID='%s'",EDIBgbl::SelPrjID);//pfg 2005/1/6
    sql.Format("select �Ᵽ�²���,���,�Ᵽ�º�,size_unit,��ȫ���,size_amoun,size_mark,EnginID from e_presiz where EnginID='%s'",EDIBgbl::SelPrjID);//pfg 2005/1/6
	try
	{
		OpenATableToShow(sql,theApp.m_pConnection,_T("�༭���²��ϻ��ܱ�(���)"),_T("E_PRESIZ"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);	
	}
}

void CAutoIPEDView::OnMenuitem511() 
{
	ToExcel toexcel;
	if(toexcel.Menu511())
	{
		m_strMessage+="\n���²��ϻ��ܱ�(���)������Excel�гɹ�!";
	}
	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
}

void CAutoIPEDView::OnMenuitem52() 
{
	CString strSQL;
	strSQL="select * from e_precol where enginid='"+EDIBgbl::SelPrjID+"'";		
//	strSQL="select ID, col_num as ���,col_name as ������,col_amount as �������,gen_amount as ʩ������, col_size as ���,col_unit as ��λ, col_mark as ��ע from e_precol where enginid='"+EDIBgbl::SelPrjID+"'";

	try
	{
		OpenATableToShow(strSQL,theApp.m_pConnection,_T("�༭���²��ϻ��ܱ�(��׼+����)"),_T("E_PRECOL"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
	passModiTotal("pass_mod1","E_PRECOL","pass_mark1");
}

void CAutoIPEDView::OnMenuitem53() 
{
	CString strSQL;
	strSQL="select * from e_preast where enginid='"+EDIBgbl::SelPrjID+"'";		
//	strSQL="select ID, col_num as ���,col_name as ������,col_amount as �������,gen_amount as ʩ������, col_size as ���,col_unit as ��λ, col_mark as ��ע from e_preast where enginid='"+EDIBgbl::SelPrjID+"'";

	try
	{
		OpenATableToShow(strSQL,theApp.m_pConnection,_T("�༭���²��ϻ��ܱ�(�˹�����)"),_T("E_PREAST"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}

	passModiTotal("pass_mod1","E_PREAST","pass_mark1");
}

void CAutoIPEDView::OnMenuitem54() 
{
	ToExcel toexcel;
	if(toexcel.Menu54())//10/14pfg
	{
		m_strMessage+="\n���²��ϻ��ܱ�(��׼+����)������Excel�гɹ�!";
	}
	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);	
}
void CAutoIPEDView::OnMenuitem55() //9/13
{
	CString strSQL;
	CTotalTableIPED oilpaint;
	oilpaint.m_Pconnection=theApp.m_pConnection;
	oilpaint.m_pConnectionCODE = theApp.m_pConnectionCODE;
	if(EDIBgbl::SelPrjID.IsEmpty())	//9/23
	{
		AfxMessageBox("δѡ�񹤳�!");
		return;
	}

	try
	{
		if(oilpaint.c_paicol())
		{
			m_strMessage+="\n����������ϻ��ܱ�(��׼+����)�ɹ�!";//10/13
			if(m_autoTotal.m_autoTotal[14]||m_stateAutoTotal)
			{
				if(m_stateAutoTotal)
				{
					AfxMessageBox("����������ϻ��ܱ�(��׼+����)�ɹ�! ");
				}
				strSQL="select * from e_paicol where enginid='"+EDIBgbl::SelPrjID+"'";	
			//	strSQL="select col_name as ������,col_amount as ����, col_size as ���,col_unit as ��λ, col_mark as ��ע from e_paicol where enginid='"+EDIBgbl::SelPrjID+"'";
				OpenATableToShow(strSQL,theApp.m_pConnection,_T("������ϻ��ܱ�(��׼+����)"),_T("E_PAICOL"));
			}
		}
		else//10/4 pfg
		{
			m_autoTotal.m_autoTotal[9]=false;//10/4pfg
			m_autoTotal.m_autoTotal[14]=false;//10/4pfg
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		
	}
}

void CAutoIPEDView::OnMenuitem56() 
{
	CString strSQL;
	strSQL="select * from e_paicol where enginid='"+EDIBgbl::SelPrjID+"'";
//	strSQL="select col_name as ������,col_amount as ����, col_size as ���,col_unit as ��λ, col_mark as ��ע from e_paicol where enginid='"+EDIBgbl::SelPrjID+"'";

	try
	{
		OpenATableToShow(strSQL,theApp.m_pConnection,_T("�༭������ϻ��ܱ�(��׼+����)"),_T("E_PAICOL"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
	passModiTotal("pass_mod2","E_PAICOL","pass_mark2");
}

void CAutoIPEDView::OnMenuitem57()
{
	CString strSQL;
	strSQL="select * from e_paiast where enginid='"+EDIBgbl::SelPrjID+"'";
//	strSQL="select col_name as ������,col_amount as ����, col_size as ���,col_unit as ��λ, col_mark as ��ע from e_paiast where enginid='"+EDIBgbl::SelPrjID+"'";

	try
	{
		OpenATableToShow(strSQL,theApp.m_pConnection,_T("�༭������ϻ��ܱ�(�˹�����)"),_T("E_PAIAST"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}

	passModiTotal("pass_mod2","E_PAIAST","pass_mark2");
}

void CAutoIPEDView::OnMenuitem58() 
{
	ToExcel toexcel;
	if(toexcel.Menu58())//10/13pfg
	{
		m_strMessage+="\n������ϻ��ܱ�(��׼+����)������Excel�гɹ�!";
	}
	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
}

void CAutoIPEDView::OnMenuitem59() //9/13
{
	CTotalTableIPED presiz;
	presiz.m_Pconnection=theApp.m_pConnection;
	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�!"));
		return;
	}
	try
	{
		if(presiz.c_presiz())
		{
			m_strMessage+="\n���ɱ��²��ϻ��ܱ�(���)�ɹ�!";//10/13pfg
			if(m_autoTotal.m_autoTotal[0]||m_stateAutoTotal)//10/13 pfg
			{
				if(m_stateAutoTotal)//10/13pfg
				{
					AfxMessageBox("���ɱ��²��ϻ��ܱ�(���)�ɹ�!");//10/13pfg
				}
				CString sql;
			//	sql.Format("select �Ᵽ�²���,���,�Ᵽ�º�,size_unit,��ȫ���,size_mark,EnginID from e_presiz where EnginID='%s'",EDIBgbl::SelPrjID);//pfg 2005/1/5ԭ����
			//	sql.Format("select �Ᵽ�²���,���,�Ᵽ�º�,size_unit as ��λ,��ȫ���,size_amoun as ����,size_mark as ��ע,EnginID from e_presiz where EnginID='%s'",EDIBgbl::SelPrjID);//pfg 2005/1/5
				sql.Format("select �Ᵽ�²���,���,�Ᵽ�º�,size_unit,��ȫ���,size_amoun,size_mark,EnginID from e_presiz where EnginID='%s'",EDIBgbl::SelPrjID);//pfg 2005/1/6
				OpenATableToShow(sql,theApp.m_pConnection,_T("���²��ϻ��ܱ�(���)"),_T("E_PRESIZ"));
			}
		}
		else//10/14 pfg
		{
			m_autoTotal.m_autoTotal[10]=false;//10/14 pfg
			m_autoTotal.m_autoTotal[0]=false;//10/14 pfg
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

//////////////////////////////////////////
//
// ��Ӧ��ɫ����ͷ������ɫ׼��⡱�˵�
//
void CAutoIPEDView::OnMenuitem610() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_COLOR WHERE EnginID='%s' ORDER BY COLR_CODE"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("ɫ����ͷ������ɫ׼���"),_T("A_COLOR"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

////////////////////////////////////////////////
//
// ��Ӧ��ɫ����ͷ�ߴ�׼��⡱�˵�
//
void CAutoIPEDView::OnMenuitem611() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_ARROW WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("ɫ����ͷ�ߴ�׼���"),_T("A_ARROW"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

/////////////////////////////////////////
//
// ��Ӧ�������ᵥλ�����⡱�˵�
//
void CAutoIPEDView::OnMenuitem612() 
{
	CString SQL;

	SQL.Format(_T("SELECT * FROM A_COLR_L"));

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnectionCODE,_T("�����ᵥλ������"),_T("A_COLR_L"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

///////////////////////////////////////////////////
//
// ��Ӧ�����ϱ��½ṹ�⡱�˵�
//
void CAutoIPEDView::OnMenuitem62() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_COMPND WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("���ϱ��½ṹ��"),_T("A_COMPND"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

////////////////////////////////////////////////////
//
// ��Ӧ�����²������׼��⡱�˵�
//
void CAutoIPEDView::OnMenuitem63() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_PRE WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("���²������׼���"),_T("A_PRE"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

///////////////////////////////////////
//
// ��Ӧ�������������׼��⡱�˵�
//
void CAutoIPEDView::OnMenuitem64() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_PRO WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("�����������׼���"),_T("A_PRO"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

////////////////////////////////////////
//
// ��Ӧ������ṹ���׼��⡱�˵�
//
void CAutoIPEDView::OnMenuitem65() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_PAI WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("����ṹ���׼���"),_T("A_PAI"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

/////////////////////////////////////////
//
// ��Ӧ��������Ƴ����⡱�˵�
//
void CAutoIPEDView::OnMenuitem66() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_CONFIG WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("������Ƴ�����"),_T("A_CONFIG"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

//////////////////////////////////////////
//
// ��Ӧ�����¸����������ƿ⡱�˵�
//
void CAutoIPEDView::OnMenuitem67() 
{
	CString SQL;

	SQL.Format(_T("SELECT * FROM A_MATAST WHERE EnginID='"+EDIBgbl::SelPrjID+"' "));

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("���¸����������ƿ�"),_T("A_MATAST"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

///////////////////////////////////////////
//
// ��Ӧ�����¸����������׼��⡱�˵�
//
void CAutoIPEDView::OnMenuitem68() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_WIRE WHERE EnginID='%s' ORDER BY WIRE_NUM ASC"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("���¸����������׼���"),_T("A_WIRE"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

////////////////////////////////////////
//
// ���¸���������ϼ���
//
void CAutoIPEDView::OnMenuitem69() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_VERT WHERE EnginID='%s' ORDER BY VERT_NUM ASC"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("���¸���������ϼ���"),_T("A_VERT"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

/////////////////////////////////////////
//
// ��Ӧ��ʩ��ͼ���Ŀ¼�⡱�˵�
//
void CAutoIPEDView::OnMenuitem71() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	//
	// EnginID,ID�ֶν��ᱻ����
	//
	SQL.Format(_T("SELECT VolumeID as ID,jcdm,jcmc,SJHYID,SJJDID,ZYID,EnginID")
			   _T(" FROM Volume WHERE EnginID='%s' ORDER BY jcdm"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConAllPrj,_T("ʩ��ͼ���Ŀ¼��"),_T("Volume"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

///////////////////////////////////////////////
//
// ��Ӧ�������ܵ����豸���ƿ⡱�˵�
//
void CAutoIPEDView::OnMenuitem72() 
{
	CString SQL;

	SQL.Format(_T("SELECT * FROM A_VOL ORDER BY VOL ASC"));

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnectionCODE,_T("�����ܵ����豸���ƿ�"),_T("A_VOL"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

//////////////////////////////////////////
//
// ��Ӧ����װ�ص�⡱�˵�
//
void CAutoIPEDView::OnMenuitem73() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_LOC WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("��װ�ص��"),_T("A_LOC"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

///////////////////////////////////////////
//
// ��Ӧ���ܵ������⡱�˵�
//
void CAutoIPEDView::OnMenuitem74() 
{
	CString SQL;

	SQL.Format(_T("SELECT * FROM A_PIPE"));

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnectionCODE,_T("�ܵ�������"),_T("A_PIPE"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

/////////////////////////////////////////////
//
// ��Ӧ���򿪲������ݿ⡱
//
void CAutoIPEDView::OnMaterialDatabase() 
{
	// ���ö�̬����ʾ���� [2006/01/12]
	if ( m_pMaterialDlg == NULL )
	{
		m_pMaterialDlg = new CUeMaterial;
	}
	m_pMaterialDlg->SetCodeID(1);
	m_pMaterialDlg->SetMaterialTableToOpen(0);
	m_pMaterialDlg->SetShowMaterial(TRUE);		// ��ʾ�¾ɲ��϶��ձ�
	m_pMaterialDlg->SetMaterialConnection( theApp.m_pConMaterial );	// �������ݿ������
	m_pMaterialDlg->SetMaterialNameConnection( theApp.m_pConMaterialName );//��������,���ڶ����԰汾
	m_pMaterialDlg->AddCustomMaterialToDB( this, FALSE );
}


////////////////////////////////////////////
//
// ��Ӧ������ʩ�������⡱�˵�
//
void CAutoIPEDView::OnMenuitem613() 
{
	CString SQL;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return;
	}

	SQL.Format(_T("SELECT * FROM A_YL WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		OpenATableToShow(SQL,theApp.m_pConnection,_T("����ʩ��������"),_T("A_YL"));
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
}

void CAutoIPEDView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	int LineCount;	
	CAutoIPEDDoc* pDoc = GetDocument();

	if(!IsWindow(GetEditCtrl().GetSafeHwnd()))
		return;

	GetEditCtrl().SetWindowText(pDoc->m_Result);
    
	LineCount=GetEditCtrl().GetLineCount();

	if(LineCount>0)
	{
		ShowScrollBar(SB_BOTH,TRUE);
		GetEditCtrl().LineScroll(LineCount);	
	}
	else
	{
		ShowScrollBar(SB_BOTH,FALSE);
	}
}

void CAutoIPEDView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
//	ShowScrollBar(SB_BOTH,false);
}


void CAutoIPEDView::OnSize(UINT nType, int cx, int cy) 
{
	CEditView::OnSize(nType, cx, cy);
}

void CAutoIPEDView::OnSelPrjVolume() 
{
	BOOL bRet;

	//
	// ��һ������Ҳû��ʱ�����öԻ���
	//
	try
	{
		bRet=((CAutoIPEDApp*)AfxGetApp())->IsNoOneEngin();
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return;
	}

	if(bRet)
	{
		newproject Dlg; 
		Dlg.m_strCaption="�½�����";
		Dlg.DoModal();
		return;
	}

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
		//�ڱ���������ʾ��ǰ���̵�����
		((CMainFrame*)::AfxGetMainWnd())->ShowCurrentProjectName();
		return ;
	}
	//ѡ��ͬ�Ĺ��̣��ȹر����е�����Ի���
	if( EDIBgbl::SelPrjID != dlg.m_SelPrjID )
	{
		CloseAllBrowseWindow();
		GetDocument()->m_Result = _T("");
		OnUpdate(NULL, 0, NULL);
		theApp.InitializeProjectTable( dlg.m_SelPrjID, dlg.m_SelVlmCODE, dlg.m_SelPrjName );
	}
	EDIBgbl::SelSpecID=dlg.m_SelSpecID;
	EDIBgbl::SelPrjID=dlg.m_SelPrjID;
	EDIBgbl::SelDsgnID=dlg.m_SelDsgnID;
	EDIBgbl::SelHyID=dlg.m_SelHyID;
	EDIBgbl::SelVlmID=dlg.m_SelVlmID;
	EDIBgbl::SelVlmCODE=dlg.m_SelVlmCODE;

	//�ڱ���������ʾ��ǰ���̵�����
	((CMainFrame*)::AfxGetMainWnd())->ShowCurrentProjectName();

}

void CAutoIPEDView::OnCopyFromExistPrj() 
{
	//���û��½��������ݿ�
	newproject newproject1;

	newproject1.DoModal();
}

void CAutoIPEDView::OnFileSeldir() 
{
	CFrmFolderLocation dlgFrm;
	dlgFrm.DoModal();
}

void CAutoIPEDView::OnOption() 
{
	CDlgOption  dlg;
	if (IDOK != dlg.DoModal())
	{
		return;
	}
}

void CAutoIPEDView::OnImportOtherIpedUE() 
{
	// TODO: Add your command handler code here
	InputOtherData otherdata;
	otherdata.SelectFile(1);
}

void CAutoIPEDView::OnSelectCriterionDatabase() 
{

	CDlgSelCritDB  dlg;
	if( IDOK == dlg.DoModal() )
	{
		//�ı��˹淶���ݿ�֮���ȹر������Ѿ��򿪵�����Ի���.
		CloseAllBrowseWindow();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��һ�����ݼ�����ʾ
//
// SQL[in]					SQL������ڴ�ָ�������ݼ�
// IConnection[in]			���ݿ�����ӣ��򿪵ļ�¼�����ڴ����ݿ���
// strDlgCaption[in]		�Ի���ı���
// strDataGridCaption[in]	DataGrid�ؼ��ı���
// IsAllowUpdate[in]		�Ƿ�������£�TRUEΪ������£�FALSEΪ������
// IsAllowAdd[in]			�Ƿ����������¼�¼��TRUEΪ������£�FALSEΪ������
// IsAllowDel[in]			�Ƿ�����ɾ����¼��TRUEΪ������£�FALSEΪ������
//
// ���򿪵����빤����ص����ݿ�ʱ��EnginID�ֶλ����ء�
// �������¼�¼ʱ��������ӵļ�¼�빤����أ�EnginID�ֶλ��Զ�������ѡ���̵�ID
//
void CAutoIPEDView::OpenATableToShow(CString SQL, _ConnectionPtr &IConnection, CString strDlgCaption,
									 CString strDataGridCaption, BOOL IsAllowUpdate, BOOL IsAllowAdd, BOOL IsAllowDel)
{
	CString strTemp;
	_RecordsetPtr IRecordset;
	HRESULT hr;
	CWnd *pWnd;

	SQL.TrimLeft();
	if(SQL.IsEmpty() || IConnection==NULL)
	{
		//����������
		_com_error e(E_INVALIDARG);
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
		IRecordset->CursorLocation=adUseClient;
		IRecordset->Open(_variant_t(SQL),_variant_t((IDispatch*)IConnection),
						 adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV2(e);
		throw ;
	}

	pWnd=GetModelessDlgInMap(strDlgCaption);

	// ����˱���ĶԻ����Ѿ����ھͻ�����,���¼�¼��
	if(pWnd!=NULL)
	{
		pWnd->SetForegroundWindow();
		CDataShowDlg *pdlg;
		pdlg = (CDataShowDlg*)pWnd;
		pdlg->SetRecordset(IRecordset);
		pdlg->UpdateBrowWindow();

		return;
	}

	CDataShowDlg *pdlg;
	pdlg=new CDataShowDlg(this);

	pdlg->SetRecordset(IRecordset);

	pdlg->SetDlgCaption(strDlgCaption);
	pdlg->SetDataGridCaption(strDataGridCaption);

	pdlg->SetAllowUpdate(IsAllowUpdate);
	pdlg->SetAllowAddNew(IsAllowAdd);
	pdlg->SetAllowDelete(IsAllowDel);

	//
	//��Ϊ�������ݿ�ʱ��Ҫ����EnginID
	//
	if(theApp.m_pConnection==IConnection)
	{
		//��Ϊ���±�ʱIDҲҪ����
		LPCTSTR HideId[]=
		{
			_T("E_PRECOL"),
			_T("E_PREAST"),
			_T("E_PAICOL"),
			_T("E_PAIAST"),
			_T("VOLUME"),
		};

		//��Ϊ���±�ʱ��c_num�ֶ�ҲӦ������
		LPCTSTR HideNum[]=
		{
			_T("PRE_CALC")
		};

		strTemp=_T("EnginID");

		SQL.MakeUpper();

		for(int i=0;i<sizeof(HideId)/sizeof(HideId[0]);i++)
		{
			if(SQL.Find(HideId[i])>=0)
			{
				strTemp+=_T("|ID");
				break;
			}
		}

		for(i=0;i<sizeof(HideNum)/sizeof(HideNum[0]);i++)
		{
			if(SQL.Find(HideNum[i])>=0)
			{
				strTemp+=_T("|c_num");
				break;
			}
		}

		pdlg->SetHideColumns(strTemp);

		CDataShowDlg::tag_DefaultFieldValue DefaultFieldValue;

		DefaultFieldValue.strFieldName=_T("EnginID");
		DefaultFieldValue.varFieldValue=EDIBgbl::SelPrjID;

		pdlg->SetDefaultFieldValue(&DefaultFieldValue,1);
	}

//	pdlg->SetAutoDelete(TRUE);
	pdlg->Create(CDataShowDlg::IDD,this);
	
	m_ModelessDlgMap.SetAt(strDlgCaption,pdlg);
}


//9/17
void CAutoIPEDView::OnUpdateMenuitem51(CCmdUI* pCmdUI) 
{
	CString str;
	_variant_t key;
	CString strTemp;
	try
	{
		if(OpenPassTable())
		{
			if( (!m_passSet->adoEOF)&&(!m_passSet->BOF))
			{
				m_passState51=true;
				m_passSet->MoveFirst();
				str="pass_mod1='C_PRECOL'";
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_last1"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					str="pass_mod1='"+strTemp+"'";
					m_passSet->MoveFirst();
					m_passSet->Find(_bstr_t(str),0,adSearchForward);
					if(!m_passSet->adoEOF)
					{
						key=m_passSet->GetCollect(_T("pass_mark1"));
						strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
						if(strTemp=="T")
						{
							pCmdUI->Enable(true);
							return;
						}
					}

				}
			}
		}
	}
	catch(_com_error e)
	{
	//	pCmdUI->Enable(false);//pfg 2005/1/4ԭ����
    //���ܱ�������
		pCmdUI->Enable(true);//pfg 2005/1/4
	}
//	pCmdUI->Enable(false);//pfg 2005/1/4ԭ����
		pCmdUI->Enable(true);//pfg 2005/1/4

	// zsy [2005/11/24]
	if ( EDIBgbl::SelPrjID.IsEmpty() )
		pCmdUI->Enable( FALSE );		//û��ѡ�񹤳̣��˵�������

}
//�˵�����ñ��
void CAutoIPEDView::OnUpdateMenuitem55(CCmdUI* pCmdUI) 
{
	// zsy [2005/11/24]
	if ( EDIBgbl::SelPrjID.IsEmpty() )
		pCmdUI->Enable( FALSE );		//û��ѡ�񹤳̣��˵�������
	else
		pCmdUI->Enable( TRUE );		//����PASS����״̬�Ŀ���
	return;

	_variant_t key;
	CString str;
	CString strTemp;
	try
	{
		if(m_passState51)
		{
			m_passSet->MoveFirst();
			str="pass_mod2='C_PAICOL'";
			m_passSet->Find(_bstr_t(str),0,adSearchForward);
			if(!m_passSet->adoEOF)
			{
				key=m_passSet->GetCollect(_T("pass_last2"));
				strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
				str="pass_mod2='"+strTemp+"'";
				m_passSet->MoveFirst();
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_mark2"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					if(strTemp=="T")
					{
						pCmdUI->Enable(true);
						return;
					}
				}
			}
		}
	}catch(_com_error e)
	{
	//	pCmdUI->Enable(false);pfg2005/1/4ԭ����
	//���ܱ�������
		pCmdUI->Enable(true);//pfg2005/1/4

	}
//	pCmdUI->Enable(false);pfg2005/1/4ԭ����
		pCmdUI->Enable(true);//pfg2005/1/4

}

extern	CAutoIPEDApp theApp;
BOOL CAutoIPEDView::OpenPassTable()
{
	_ConnectionPtr pConnection;
	pConnection=theApp.m_pConnection;
	m_passSet.CreateInstance(__uuidof(Recordset));
	try
	{
		m_passSet->Open(_variant_t(_T("select * from pass where enginid='"+EDIBgbl::SelPrjID+"' ")),(IDispatch*)pConnection,adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		return false;
	}
	return true;
}
//9/17

///////////////////////////////////////////////////////////////
//
// ���ڸ����롰׼���ά��������������ά��������
// ������ص����ݿ�˵��ģ������벻����
//
void CAutoIPEDView::OnUpdateProjectConnectMenu(CCmdUI* pCmdUI) 
{
	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}		

}

//9/20
void CAutoIPEDView::OnUpdateMenuitem59(CCmdUI* pCmdUI) 
{
	// zsy [2005/11/24]
	if ( EDIBgbl::SelPrjID.IsEmpty() )
		pCmdUI->Enable( FALSE );		//û��ѡ�񹤳̣��˵�������
	else
		pCmdUI->Enable( TRUE );		//����PASS����״̬�Ŀ���
	return;

	_variant_t key;
	CString str;
	CString strTemp;
	try
	{
		if(m_passState51)
		{
			m_passSet->MoveFirst();
			str="pass_mod1='C_PRESIZ'";
			m_passSet->Find(_bstr_t(str),0,adSearchForward);
			if(!m_passSet->adoEOF)
			{
				key=m_passSet->GetCollect(_T("pass_last1"));
				strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
				str="pass_mod1='"+strTemp+"'";
				m_passSet->MoveFirst();
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_mark1"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					if(strTemp=="T")
					{
						pCmdUI->Enable(true);
						return;
					}
				}
			}
		}
	}catch(_com_error e)
	{
	//	pCmdUI->Enable(false);pfg2005/1/4ԭ����
		pCmdUI->Enable(true);//pfg2005/1/4
	}
//	pCmdUI->Enable(false);pfg2005/1/4ԭ����
	pCmdUI->Enable(true);//pfg2005/1/4
		
}


//9/20

void CAutoIPEDView::OnUpdateMenuitem510(CCmdUI* pCmdUI) 
{
	_variant_t key;
	CString str;
	CString strTemp;
	try
	{
		if(m_passState51)
		{
			m_passSet->MoveFirst();
			str="pass_mod1='E_PRESIZ'";
			m_passSet->Find(_bstr_t(str),0,adSearchForward);
			if(!m_passSet->adoEOF)
			{
				key=m_passSet->GetCollect(_T("pass_last1"));
				strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
				str="pass_mod1='"+strTemp+"'";
				m_passSet->MoveFirst();
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_mark1"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					if(strTemp=="T")
					{
						pCmdUI->Enable(true);
						return;
					}
				}
			}
		}
	}catch(_com_error e)
	{
		pCmdUI->Enable(false);
	}
	pCmdUI->Enable(false);
}

void CAutoIPEDView::OnUpdateMenuitem511(CCmdUI* pCmdUI) 
{
	_variant_t key;
	CString str;
	CString strTemp;
	try
	{
		if(m_passState51)
		{
			m_passSet->MoveFirst();
			str="pass_mod1='P_PRESIZ'";
			m_passSet->Find(_bstr_t(str),0,adSearchForward);
			if(!m_passSet->adoEOF)
			{
				key=m_passSet->GetCollect(_T("pass_last1"));
				strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
				str="pass_mod1='"+strTemp+"'";
				m_passSet->MoveFirst();
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_mark1"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					if(strTemp=="T")
					{
						pCmdUI->Enable(true);
						return;
					}
				}
			}
		}
	}catch(_com_error e)
	{
		pCmdUI->Enable(false);
	}
	pCmdUI->Enable(false);
}

//�༭������ϻ��ܱ�(��׼+����)
void CAutoIPEDView::OnUpdateMenuitem57(CCmdUI* pCmdUI) 
{
	_variant_t key;
	CString str;
	CString strTemp;
	try
	{
		if(m_passState51)
		{
			m_passSet->MoveFirst();
			str="pass_mod2='E_PAIAST'";
			m_passSet->Find(_bstr_t(str),0,adSearchForward);
			if(!m_passSet->adoEOF)
			{
				key=m_passSet->GetCollect(_T("pass_last2"));
				strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
				str="pass_mod2='"+strTemp+"'";
				m_passSet->MoveFirst();
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_mark2"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					if(strTemp=="T")
					{
						pCmdUI->Enable(true);
						return;
					}
				}
			}
		}
	}catch(_com_error e)
	{
		pCmdUI->Enable(false);
	}
	pCmdUI->Enable(false);	
}

void CAutoIPEDView::OnUpdateMenuitem56(CCmdUI* pCmdUI) 
{
	_variant_t key;
	CString str;
	CString strTemp;
	try
	{
		if(m_passState51)
		{
			m_passSet->MoveFirst();
			str="pass_mod2='E_PAICOL'";
			m_passSet->Find(_bstr_t(str),0,adSearchForward);
			if(!m_passSet->adoEOF)
			{
				key=m_passSet->GetCollect(_T("pass_last2"));
				strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
				str="pass_mod2='"+strTemp+"'";
				m_passSet->MoveFirst();
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_mark2"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					if(strTemp=="T")
					{
						pCmdUI->Enable(true);
						return;
					}
				}
			}
		}
	}catch(_com_error e)
	{
		pCmdUI->Enable(false);
	}
	pCmdUI->Enable(false);		
}

void CAutoIPEDView::OnUpdateMenuitem58(CCmdUI* pCmdUI) 
{
	_variant_t key;
	CString str;
	CString strTemp;
	try
	{
		if(m_passState51)
		{
			m_passSet->MoveFirst();
			str="pass_mod2='P_PAICOL'";
			m_passSet->Find(_bstr_t(str),0,adSearchForward);
			if(!m_passSet->adoEOF)
			{
				key=m_passSet->GetCollect(_T("pass_last2"));
				strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
				str="pass_mod2='"+strTemp+"'";
				m_passSet->MoveFirst();
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_mark2"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					if(strTemp=="T")
					{
						pCmdUI->Enable(true);
						return;
					}
				}
			}
		}
	}catch(_com_error e)
	{
		pCmdUI->Enable(false);
	}
	pCmdUI->Enable(false);	
}

void CAutoIPEDView::OnUpdateMenuitem52(CCmdUI* pCmdUI) 
{
	_variant_t key;
	CString str;
	CString strTemp;
	try
	{
		if(m_passState51)
		{
			m_passSet->MoveFirst();
			str="pass_mod1='E_PRECOL'";
			m_passSet->Find(_bstr_t(str),0,adSearchForward);
			if(!m_passSet->adoEOF)
			{
				key=m_passSet->GetCollect(_T("pass_last1"));
				strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
				str="pass_mod1='"+strTemp+"'";
				m_passSet->MoveFirst();
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_mark1"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					if(strTemp=="T")
					{
						pCmdUI->Enable(true);
						return;
					}
				}
			}
		}
	}catch(_com_error e)
	{
		pCmdUI->Enable(false);
	}
	pCmdUI->Enable(false);		
}

void CAutoIPEDView::OnUpdateMenuitem53(CCmdUI* pCmdUI) 
{
	_variant_t key;
	CString str;
	CString strTemp;
	try
	{
		if(m_passState51)
		{
			m_passSet->MoveFirst();
			str="pass_mod1='E_PREAST'";
			m_passSet->Find(_bstr_t(str),0,adSearchForward);
			if(!m_passSet->adoEOF)
			{
				key=m_passSet->GetCollect(_T("pass_last1"));
				strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
				str="pass_mod1='"+strTemp+"'";
				m_passSet->MoveFirst();
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_mark1"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					if(strTemp=="T")
					{
						pCmdUI->Enable(true);
						return;
					}
				}
			}
		}
	}catch(_com_error e)
	{
		pCmdUI->Enable(false);
	}
	pCmdUI->Enable(false);		
}

void CAutoIPEDView::OnUpdateMenuitem54(CCmdUI* pCmdUI) 
{
	_variant_t key;
	CString str;
	CString strTemp;
	try
	{
		if(m_passState51)
		{
			m_passSet->MoveFirst();
			str="pass_mod1='P_PRECOL'";
			m_passSet->Find(_bstr_t(str),0,adSearchForward);
			if(!m_passSet->adoEOF)
			{
				key=m_passSet->GetCollect(_T("pass_last1"));
				strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
				str="pass_mod1='"+strTemp+"'";
				m_passSet->MoveFirst();
				m_passSet->Find(_bstr_t(str),0,adSearchForward);
				if(!m_passSet->adoEOF)
				{
					key=m_passSet->GetCollect(_T("pass_mark1"));
					strTemp=(key.vt==VT_EMPTY||key.vt==VT_NULL)?"":(CString)key.bstrVal;
					if(strTemp=="T")
					{
						pCmdUI->Enable(true);
						return;
					}
				}
			}
		}
	}catch(_com_error e)
	{
		pCmdUI->Enable(false);
	}
	pCmdUI->Enable(false);		
}

void CAutoIPEDView::OnUpdateMenuitem33(CCmdUI* pCmdUI) 
{
	CString SQL,strTemp;
	_RecordsetPtr IRecordset_Pass;
	_variant_t varTemp;
	HRESULT hr;

	struct
	{
		UINT Id;
		LPCTSTR pass_mod1_Param;
	}Info[]=
	{
		{ID_MENUITEM33,_T("BROW_RES")},
		{ID_MENUITEM34,_T("P_CALTAB")},
		{ID_MENUITEM35,_T("E_PREDAT")}
	};
	
	for(int i=0;i<sizeof(Info)/sizeof(Info[0]);i++)
	{
		if(Info[i].Id==pCmdUI->m_nID)
		{
			strTemp=Info[i].pass_mod1_Param;
			break;
		}
	}

	if(i>=sizeof(Info)/sizeof(Info[0]))
		return;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		pCmdUI->Enable(FALSE);
		return;
	}


	hr=IRecordset_Pass.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionErrorLV1(_com_error((HRESULT)hr));
		return;
	}
	
	IRecordset_Pass->CursorLocation=adUseClient;

	SQL.Format(_T("SELECT * FROM PASS WHERE EnginID='%s'"),EDIBgbl::SelPrjID);
	
	try
	{
		IRecordset_Pass->Open(_variant_t(SQL),_variant_t((IDispatch*)theApp.m_pConnection),
							  adOpenStatic,adLockOptimistic,adCmdText);

//		SQL.Format(_T("pass_mod1='BROW_RES'"));
		SQL.Format(_T("pass_mod1='%s'"),strTemp);
		IRecordset_Pass->Filter=_variant_t(SQL);

		varTemp=IRecordset_Pass->GetCollect(_variant_t("pass_last1"));

		SQL.Format(_T("pass_mod1='%s'"),(LPCTSTR)(_bstr_t)varTemp);

		IRecordset_Pass->Filter=_variant_t(SQL);

		varTemp=IRecordset_Pass->GetCollect(_variant_t("pass_mark1"));
		strTemp=(LPCTSTR)(_bstr_t)varTemp;

		strTemp.MakeUpper();
		if(strTemp!=_T("T"))
		{
			pCmdUI->Enable(FALSE);
			return;
		}
	}
	catch(_com_error)
	{
	//	ReportExceptionErrorLV1(e);
		return;
	}


}

//�޸�pass��
void CAutoIPEDView::passModiTotal(CString str1, CString str2, CString str3)
{
	_variant_t key;
	CString str;
	_RecordsetPtr pass_set;
	int flagPass=0;
	pass_set.CreateInstance(__uuidof(Recordset));
	pass_set->Open(_variant_t(_T("select * from pass where enginid='"+EDIBgbl::SelPrjID+"'")),(IDispatch*)theApp.m_pConnection,adOpenStatic,adLockOptimistic,adCmdText);
	if(pass_set->GetRecordCount()>0)
	{
		pass_set->MoveFirst();
		for(;!pass_set->adoEOF;pass_set->MoveNext())
		{
			if(flagPass==0)
			{
				pass_set->PutCollect(_variant_t(str3),_T("T"));
			}
			else
			{
				pass_set->PutCollect(_variant_t(str3),_T("F"));
			}
			key=pass_set->GetCollect(_variant_t(str1));
			if((!(key.vt==VT_NULL||key.vt==VT_EMPTY))&&(flagPass==0))
			{
				str=key.bstrVal;
				flagPass=(str.Compare(str2)==0)?1:0;
			}
		}
	}	
	if(pass_set->State==adStateOpen)
	{
		pass_set->Close();
	}
}

void CAutoIPEDView::OnUpdateExplainTblPass(CCmdUI* pCmdUI) 
{
	//��ǰ����Ϊ��ʱ��
	if( EDIBgbl::SelPrjID.IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	int nIndex;
	CExplainTable tbl;
	tbl.m_Pconnection = theApp.m_pConnection;
	nIndex = pCmdUI->m_nIndex;
	switch( nIndex )
	{
		//˵����
		case 0 :
		/*	if( !tbl.IsPass("pass_mod1", "C_PREEXP", "pass_last1", "pass_mark1") )
			{
				pCmdUI->Enable(FALSE);
			}else
			{
				pCmdUI->Enable(TRUE);
			}
		*/
			pCmdUI->Enable(TRUE);	//����
			break;
		case 1 :
			if( !tbl.IsPass("pass_mod1", "E_PREEXP", "pass_last1", "pass_mark1") )
			{
				pCmdUI->Enable(FALSE);
			}else
			{
				pCmdUI->Enable(TRUE);
			}
			break;
		case 2 :
			if( !tbl.IsPass("pass_mod1", "P_PREEXP", "pass_last1", "pass_mark1") )
			{
				pCmdUI->Enable(FALSE);
			}else
			{
				pCmdUI->Enable(TRUE);
			}
			break;

		//�����
		case 4:
		/*	if( !tbl.IsPass("pass_mod2", "C_PAIEXP", "pass_last2", "pass_mark2", true, "E_PREEXP") )
			{
				pCmdUI->Enable(FALSE);
				bIsExplainPass = FALSE;
			}else
			{
				pCmdUI->Enable(TRUE);
				bIsExplainPass = TRUE;
			}
			*/
			pCmdUI->Enable(TRUE);		//�����˵������PASS����
			bIsExplainPass = TRUE;
			break;
		case 5:
			if( !tbl.IsPass("pass_mod2", "E_PAIEXP", "pass_last2", "pass_mark2") )
			{
				pCmdUI->Enable(FALSE);
			}else
			{
				if( !bIsExplainPass )  //���������������ܱ༭
				{
					pCmdUI->Enable(FALSE);
				}else
				{
					pCmdUI->Enable(TRUE);
				}
			}
			break;
		case 6:
			if( !tbl.IsPass("pass_mod2", "P_PAIEXP", "pass_last2", "pass_mark2") )
			{
				pCmdUI->Enable(FALSE);
			}else
			{
				if( !bIsExplainPass )
				{
					pCmdUI->Enable(FALSE);
				}else
				{
					pCmdUI->Enable(TRUE);
				}
			}
			break;
	}
}

////////////////////////////////////////////////////////////////
//
// ��һ��map�л��ָ���ı���ĶԻ������
//
// strDlgCaption [in]	�Ի������
//
// ��������Ѿ����٣���û�д˱���ĶԻ��򽫷���NULL
//
// ��������Ѿ����٣�����map���Ƴ��˶���delete
//
CWnd* CAutoIPEDView::GetModelessDlgInMap(CString strDlgCaption)
{
	CString strKey;
	CObject *pObject;
	CWnd *pWnd;
	POSITION pos;

	pos=m_ModelessDlgMap.GetStartPosition();
	
	//
	//���Ѿ����ٵĴ��ڴ�map���Ƴ�
	//
	while(pos!=NULL)
	{
		m_ModelessDlgMap.GetNextAssoc(pos,strKey,pObject);

		pWnd=(CWnd*)pObject;

		if(!IsWindow(pWnd->GetSafeHwnd()))
		{
			m_ModelessDlgMap.RemoveKey(strKey);

			delete pWnd;

			pos=m_ModelessDlgMap.GetStartPosition();
		}
	}

	if(m_ModelessDlgMap.Lookup(strDlgCaption,pObject)==0)
	{
		return NULL;
	}

	pWnd=(CWnd*)pObject;

	return pWnd;
}

int CAutoIPEDView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CFont Font;
	LOGFONT lf;

	memset(&lf,0,sizeof(LOGFONT));

	lf.lfHeight=20;
	Font.Attach(::GetStockObject(ANSI_FIXED_FONT));
	Font.GetLogFont(&lf);

	lf.lfHeight=20;

	Font.DeleteObject();

	Font.CreateFontIndirect(&lf);

	GetEditCtrl().SetFont(&Font);
	
	return 0;
}


//�Զ�����˵�������ܱ�������Excel��ȥ
void CAutoIPEDView::OnButtonAutoTotal() 
{
	//�Զ����ɺ��˹����ɵ�״̬�Զ�����Ϊ��false
	m_stateAutoTotal=false;
	CExplainTable explainTable;
	//m_strMessage="��ʾ:"; //ZSY
	m_strMessage = "";       

	explainTable.m_Pconnection=theApp.m_pConnection;
	if(m_autoTotal.DoModal()==IDOK)
	{
		if(m_autoTotal.m_autoTotal[1]==true)
		{
			//���ɱ���˵����
			OnMenuitem41();
			//дpass��
			explainTable.WritePass("pass_mod1","E_PREEXP","pass_mark1");
		}

		if(m_autoTotal.m_autoTotal[2]==true)
		{
			//��������˵����
			OnMenuitem44();
			//дpass��
			explainTable.WritePass("pass_mod2","E_PAIEXP","pass_mark2");
		}
		
		if(m_autoTotal.m_autoTotal[3]==true)
		{
			//���ɱ��²��ϻ��ܱ�(��׼+����)
			OnMenuitem51();
			//�޸�pass��
			passModiTotal("pass_mod1","E_PRECOL","pass_mark1");
			passModiTotal("pass_mod1","E_PREAST","pass_mark1");
		}

		if(m_autoTotal.m_autoTotal[4]==true)
		{
			//����������ϻ��ܱ�
			OnMenuitem55();
			//�޸�pass��
			passModiTotal("pass_mod2","E_PAICOL","pass_mark2");
			passModiTotal("pass_mod2","E_PAIAST","pass_mark2");
		}

		if(m_autoTotal.m_autoTotal[5]==true)
		{
			//���ɱ��²��ϻ��ܱ�(���)
			OnMenuitem59();
		}

		if(m_autoTotal.m_autoTotal[6]==true)
		{
			//��������˵����
			OnMenuitem43();
		}

		if(m_autoTotal.m_autoTotal[7]==true)
		{
			//��������˵����
			OnMenuitem46();
		}

		if(m_autoTotal.m_autoTotal[8]==true)
		{
			OnMenuitem54();
		}
		if(m_autoTotal.m_autoTotal[9]==true)
		{
			OnMenuitem58();
		}
		if(m_autoTotal.m_autoTotal[10]==true)
		{
			OnMenuitem511();
		}
		
		((CMainFrame*)::AfxGetMainWnd())->ShowWindow(SW_MINIMIZE);
		((CMainFrame*)::AfxGetMainWnd())->ShowWindow(SW_SHOWMAXIMIZED);
		if( !m_strMessage.IsEmpty() )
		{
			m_strMessage = "��ʾ:" + m_strMessage;
			AfxMessageBox(m_strMessage);
		}
	}
		
	m_stateAutoTotal=true;
}

///////////////////////////////////////////////
//
// ��Ӧ "���ⲿ���뱣��ԭʼ����" �˵�
//
void CAutoIPEDView::OnImportOriginalData() 
{
	CImportFromPreCalcXLSDlg dlg;

	//���ù��̺�
	dlg.SetProjectID(EDIBgbl::SelPrjID);

	// ���ù��̵�����
	dlg.SetProjectDbConnect(theApp.m_pConnection);
	
	// ���ÿ��Ƶ����ݿ�����
	dlg.SetIPEDSortDBConnect( theApp.m_pConIPEDsort );

	// �����ӿؼ��ı���� �����ӿؼ��ı���
	dlg.SetItemTblName( _T("����ԭʼ����"), _T("PD2IPED") );

	dlg.DoModal();	
}

////////////////////////////////////////////////
//
// ��Ӧ����ʾ���������Ϣ��
//
void CAutoIPEDView::OnDisplayCalErrorInfo() 
{
	((CMainFrame*)::AfxGetMainWnd())->SetReportBarContent(GetDocument()->GetAllCalErrorInfo());

	if(!GetDocument()->GetAllCalErrorInfo().IsEmpty())
		((CMainFrame*)::AfxGetMainWnd())->ShowReportBar(true);
}

///////////////////////////////////////////////
//
// ��Ӧ "���ⲿ������Ŀ¼" �˵�
//
void CAutoIPEDView::OnImportDirData() 
{
	CImportDirFromXlsDlg dlg;

	//���ù��̺�
	dlg.SetProjectID(EDIBgbl::SelPrjID);

	// ���ù��̵�����
	dlg.SetProjectDbConnect(theApp.m_pConnection);

	// ���ÿ��Ƶ����ݿ�����
	dlg.SetIPEDSortDBConnect( theApp.m_pConIPEDsort );

	// �����ӿؼ��ı���� �����ӿؼ��ı���
	dlg.SetItemTblName( _T("���ԭʼ����"), _T("VOL2IPED") );

	dlg.DoModal();	
}

void CAutoIPEDView::OnModiRule1() 
{
	//test
	CWnd *pWnd;
	CString strDlgCaption = "����׼���ά��";
	pWnd = 	GetModelessDlgInMap( strDlgCaption );
	if ( pWnd != NULL )
	{
		//�Ѿ����ˣ���������
		pWnd->SetActiveWindow();
		//pWnd->ShowWindow( SW_MAXIMIZE );
		return;
	}
	CDlgPaintRuleModi* pDlg;
	pDlg = new CDlgPaintRuleModi( this );
	pDlg->Create( CDlgPaintRuleModi::IDD, this );

	m_ModelessDlgMap.SetAt( strDlgCaption, pDlg );
	//
//	CDlgPaintRuleModi dlg;
//	dlg.DoModal();
}


//�ر����е�����Ի���
bool CAutoIPEDView::CloseAllBrowseWindow()
{
	CString key;
	POSITION pos;
	CObject  * pObject;
	CWnd *  pWnd;
	pos =  m_ModelessDlgMap.GetStartPosition();
	while( pos != NULL )
	{
		m_ModelessDlgMap.GetNextAssoc(pos, key, pObject);
		pWnd = (CWnd* )pObject;
		if( pWnd->GetSafeHwnd() )
		{
			::SendMessage(pWnd->GetSafeHwnd(), WM_DESTROY, 0, 0 );
		}
		m_ModelessDlgMap.RemoveKey(key);
		delete pWnd;
		pos = m_ModelessDlgMap.GetStartPosition();
	}
	// �ö�̬��ʵ�ֵ���ʾ���ϵĶԻ���
	if ( m_pMaterialDlg != NULL )
	{
		delete m_pMaterialDlg;
		m_pMaterialDlg = NULL;
	}

	return true;
}

void CAutoIPEDView::OnCloseAllWindow() 
{
	CloseAllBrowseWindow();	
}
//��������ԭʼ����
void CAutoIPEDView::OnImportPaintData() 
{
	CImportPaintXLSDlg dlg;
	//���ù��̺�
	dlg.SetProjectID( EDIBgbl::SelPrjID );
	// ���ù��̵�����
	dlg.SetProjectDbConnect( theApp.m_pConnection );

	// ���ÿ��Ƶ����ݿ�����
	dlg.SetIPEDSortDBConnect( theApp.m_pConIPEDsort );

	// �����ӿؼ��ı���� �����ӿؼ��ı��� 
	dlg.SetItemTblName( _T("����ԭʼ����"), _T("PAINT2IPED") );

	dlg.DoModal();
	
}

void CAutoIPEDView::OnUpdateImportPaintData(CCmdUI* pCmdUI) 
{
	if ( EDIBgbl::SelPrjID.IsEmpty() )
	{
		pCmdUI->Enable( FALSE );
	}
	else
	{
		pCmdUI->Enable( TRUE );
	}
	
}
//��Ӧ�¾ɲ��϶��ձ�˵���
void CAutoIPEDView::OnNewOldMaterial() 
{
	CString strSQL;
	strSQL.Format("SELECT * FROM [Material_OLD_NEW] ");
	try
	{
		OpenATableToShow(strSQL, theApp.m_pConMaterial, "�¾ɲ��϶��ձ�", "Material_OLD_NEW" );
	}
	catch(_com_error &e )
	{
		ReportExceptionError(e);
	}
}
/*
void CAutoIPEDView::OnViewOutput() 
{
//	((CMaingFrme*)::AfxGetMainWnd())
	if( IsWindow(((CMainFrame*)::AfxGetMainWnd())->m_ReportCalInfoBar.GetSafeHwnd()) )
	{
		((CMainFrame*)::AfxGetMainWnd())->m_ReportCalInfoBar.ShowWindow( SW_HIDE );
	}
	else
	{
		((CMainFrame*)::AfxGetMainWnd())->m_ReportCalInfoBar.ShowWindow( SW_SHOW );
	}
}
*/

void CAutoIPEDView::OnOutView() 
{
	if( ((CMainFrame*)::AfxGetMainWnd())->m_ReportCalInfoBar.IsWindowVisible() )
	{
		((CMainFrame*)::AfxGetMainWnd())->ShowReportBar( false );
	}
	else
	{
		((CMainFrame*)::AfxGetMainWnd())->ShowReportBar( true );
	}
}
///////////////////////////
//����: ���׼����һ����
//
void CAutoIPEDView::OnCheckCoherence() 
{
	CString m_strRemarks;	//��ʾ�����о�����Ϣ
	CString strMessage;		//ÿһ�μ�����Ϣ
	CString	strSQL;			//SQL���
	_variant_t   var;		//��ʱ����
	_RecordsetPtr pRs;
	_RecordsetPtr pRsA_MAT;	//���²��ϲ����⣨A_AMT���ļ�¼��
	_RecordsetPtr pRsA_COMPND;//���ϱ��½ṹ��
	pRs.CreateInstance(__uuidof(Recordset));
	pRsA_MAT.CreateInstance(__uuidof(Recordset));
	pRsA_COMPND.CreateInstance(__uuidof(Recordset));
	int NO=1;				//���
	CString strHead;		//ÿ����Ϣ�ļ���ͱ��
	
	try
	{
		//�����ʾ����ϸ��Ϣ.
		m_strRemarks = "";
		//��顶���¸����������ƿ⡷���ֶ�A_MATAST���Ƿ����ظ��Ĳ���.
		CheckFieldRepeat( "A_MATAST","���¸����������ƿ�", "MAT_NAME","���²�����",strMessage);
		if (!strMessage.IsEmpty())
		{
			strHead.Format("%d.",NO++);
			m_strRemarks += strHead + strMessage;
		}
		//��顶���²��ϲ����⡷��a_mat���롶����ʩ�������⡷��a_yl���еĲ����Ƿ���ͬ
		strSQL = "SELECT DISTINCT MAT_NAME FROM [A_MAT] WHERE EnginID='"+EDIBgbl::SelPrjID+"' AND \
			MAT_NAME NOT IN (SELECT COL_NAME FROM [A_YL] WHERE EnginID='"+EDIBgbl::SelPrjID+"')";
		pRs->Open(_variant_t(strSQL), theApp.m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if( pRs->GetRecordCount() > 0 )
		{
			strMessage = "������ʩ�������⡷�롶���²��ϲ����⡷��һ�¡�������ʩ�������⡷��û�����²��ϣ������!\r\n";
			for(int i=1; !pRs->adoEOF; pRs->MoveNext(), i++ )
			{
				var = pRs->GetCollect("MAT_NAME");
				strMessage += vtos(var);
				if( i % 5 == 0 )
				{
					strMessage += "\r\n";
				}
				else
				{
					strMessage += ",\t";
				}
			}
			//�������ʾ����.
			strHead.Format("\r\n\r\n%d.",NO++);
			m_strRemarks += strHead + strMessage;
		}
		//�򿪱��²��ϲ�����
		strSQL = "SELECT * FROM [A_MAT] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
		pRsA_MAT->Open(_variant_t(strSQL), theApp.m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if (pRsA_MAT->GetRecordCount() <= 0)
		{
			strMessage.Format("\r\n\r\n%d.�����²��ϲ����⡷Ϊ�գ�",NO++);
			m_strRemarks += strMessage;
		}
		//�򿪸��ϱ��½ṹ��
		strSQL = "SELECT COMPLEX, COMP_IN AS COMP1, COMP_OUT AS COMP2 FROM [A_COMPND] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
		pRsA_COMPND->Open(_variant_t(strSQL), theApp.m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);

		//��顶���²������׼��⡷�ı��²����Ƿ񶼴����ڡ����²��ϲ����⡷��
		if (pRs->State == adStateOpen)
		{
			pRs->Close();
		}
		strSQL = "SELECT * FROM [A_PRE] WHERE AUTO_OBJ IS NOT NULL AND EnginID='"+EDIBgbl::SelPrjID+"' ";
		pRs->Open(_variant_t(strSQL), theApp.m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if (pRs->GetRecordCount() <= 0)
		{
			strMessage.Format("\r\n\r\n%d.�����²������׼��⡷Ϊ�գ�",NO++);
			m_strRemarks += strMessage;
		}
		else
		{
			//�����[���²������׼���]���м�⣬���������Ҫ�͸��ϱ��½ṹ��Ƚϡ�
			strMessage = "";
			if( CheckMaterialExists(pRsA_MAT, pRs, "AUTO_CON", pRs->GetFields()->GetCount()-4, strMessage, pRsA_COMPND) )
			{
				if ( !strMessage.IsEmpty())
				{
					strHead.Format("\r\n\r\n%d.�����²������׼��⡷�롶���²��ϲ����⡷�еı��²��ϲ�һ�¡������²��ϲ����⡷��û�����±��²��ϣ������!\r\n",
						NO++);
					m_strRemarks += strHead + strMessage;
				}
			}
		}
		//��顶�����������׼��⡷�ı��������Ƿ񶼴����ڡ����²��ϲ����⡷��
		if (pRs->State == adStateOpen)
		{
			pRs->Close();
		}
		strSQL = "SELECT * FROM [A_PRO] WHERE AUTO_OBJ IS NOT NULL AND EnginID='"+EDIBgbl::SelPrjID+"' ";
		pRs->Open(_variant_t(strSQL), theApp.m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);

		if (pRs->GetRecordCount() <= 0)
		{
			strMessage.Format("\r\n\r\n%d.�������������׼��⡷Ϊ�գ�",NO++);
			m_strRemarks += strMessage;
		}
		else
		{		
			strMessage = "";
			if( CheckMaterialExists(pRsA_MAT, pRs, "AUTO_CON", pRs->GetFields()->GetCount()-3, strMessage) )
			{
				if ( !strMessage.IsEmpty())
				{
					strHead.Format("\r\n\r\n%d.�������������׼��⡷�롶���²��ϲ����⡷�еı������ϲ�һ�¡������²��ϲ����⡷��û�����±������ϣ������!\r\n",
									NO++);
					m_strRemarks += strHead + strMessage;
				}
			}
		}
		//��顶���ϱ��½ṹ�⡷�ı��²����Ƿ񶼴����ڡ����²��ϲ����⡷��
		if (pRsA_COMPND->GetRecordCount() > 0)
		{
			strMessage = "";
			if( CheckMaterialExists(pRsA_MAT, pRsA_COMPND, "COMP", 2, strMessage) )
			{
				if ( !strMessage.IsEmpty())
				{
					strHead.Format("\r\n\r\n%d.�����ϱ��½ṹ�⡷�롶���²��ϲ����⡷�еı��²��ϲ�һ�¡������²��ϲ����⡷��û�����±��²��ϣ������!\r\n",
									NO++);
					m_strRemarks += strHead + strMessage;
				}
			}
		}

	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return;
	}
	if (!m_strRemarks.IsEmpty())
	{
		//��ʾ������Ϣ
		DisplayRemarksInfo(m_strRemarks);
	}
	else
	{
		//û�д���.
		DisplayRemarksInfo("");
		strMessage.LoadString(IDS_STR_CHECK);
		AfxMessageBox(strMessage);
	}
}
//
// ����:��Ŀ���ݿ⣨AutoIPED.mdb�������һ�����е�ĳ���ֶ��Ƿ����ظ���ֵ.
//û���ظ���ֵ,����	TRUE.���򷵻�	FALSE.
bool CAutoIPEDView::CheckFieldRepeat(CString strTblName, CString strTblCaption, CString strFieldName,CString strFieldCaption,CString& strRemarks)
{
	_RecordsetPtr pRs;
	_variant_t var;
	CString strSQL;
	CString strMessage;
	CString  strValue, strLastVal;

	try
	{
		pRs.CreateInstance(__uuidof(Recordset));
		strSQL = _T("SELECT "+strFieldName+" FROM ["+strTblName+"] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ORDER BY "+strFieldName+" ");
		pRs->Open(_variant_t(strSQL), theApp.m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if( pRs->GetRecordCount() <= 0  )
		{
			strRemarks += " ��"+strTblCaption+"��Ϊ��!";
			return false;
		}
		strMessage = "�ڡ�"+strTblCaption+"����, ["+strFieldCaption+"] �������ظ�ֵ��\r\n";
		strSQL = "";
		int i=1;
		for(pRs->MoveFirst(); !pRs->adoEOF; pRs->MoveNext())
		{
			var = pRs->GetCollect(_variant_t(strFieldName));
			strValue = vtos(var);
			if( !strLastVal.CompareNoCase(strValue) ) 
			{ 
				strSQL += strValue;
				if( i%5 == 0 )
				{
					strSQL += "\r\n";
				}
				else
				{
					strSQL += ",\t";
				}
				i++;
			}
			strLastVal = strValue;
		}
		if( !strSQL.IsEmpty() )
		{
			strMessage += strSQL;
			//�����ȡ�����������¼�⡣
//			if(IDYES == AfxMessageBox(strMessage, MB_YESNO) )
			{
				strRemarks += strMessage;
				return false;
			}
		}

	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}

	return true;
}

void CAutoIPEDView::OnThicknessRegular() 
{
	CString strSQL;
	strSQL.Format("SELECT * FROM [thicknessRegular] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ");
	try
	{
		OpenATableToShow(strSQL, theApp.m_pConnection, "���º�ȹ����", "thicknessRegular" );
	}
	catch(_com_error &e )
	{
		ReportExceptionError(e);
	}
}

void CAutoIPEDView::OnShowAllTable() 
{
	CString strTmp;
	strTmp = "������ṹ�鿴����";
	CWnd   *pWnd;
	pWnd = this->GetModelessDlgInMap(strTmp);
	if( pWnd != NULL )
	{	//�Ѿ��򿪣���Ϊ��ǰ��
		pWnd->SetForegroundWindow();
		pWnd->ShowWindow( SW_MAXIMIZE );
		return ;
	}
	//�����Ի���
	CDlgShowAllTable *pDlg;
	pDlg = new CDlgShowAllTable(this);

	pDlg->SetAllowAddNew(TRUE);
	pDlg->SetAllowDelete(TRUE);
	pDlg->SetAllowUpdate(TRUE);
	pDlg->SetDlgCaption(strTmp);

	pDlg->Create(CDlgShowAllTable::IDD, this);

	this->m_ModelessDlgMap.SetAt(strTmp, pDlg);
//	dlg.DoModal();
}

//���������
void CAutoIPEDView::OnCityWeather() 
{
	CDlgSelCityWeather dlg;
	dlg.DoModal();
	
}


//------------------------------------------------------------------                
// DATE         :[2005/05/30]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :��ʾ������Ϣ,(�ڲ��������е���ʾ�����)
//------------------------------------------------------------------
short CAutoIPEDView::DisplayRemarksInfo(CString strRemarks)
{
	strRemarks.TrimLeft();
	strRemarks.TrimRight();

	((CMainFrame*)AfxGetMainWnd())->SetReportBarContent(strRemarks);
	((CMainFrame*)AfxGetMainWnd())->ShowReportBar(true);

	return	1;
}

//------------------------------------------------------------------                
// DATE         :[2005/06/28]
// Author       :ZSY
// Parameter(s) :pRsA_MAT ��׼��pRsCheck �����ı�
// Return       :
// Remark       :��鱻����еı��²������Ƿ񶼴�����һ����׼���С�
//------------------------------------------------------------------
BOOL CAutoIPEDView::CheckMaterialExists(_RecordsetPtr &pRsA_MAT, _RecordsetPtr &pRsCheck, CString strField, short nFieldCount, CString& strMessage,_RecordsetPtr pRsA_Com)
{
	if (pRsA_MAT == NULL || pRsCheck == NULL || pRsA_MAT->RecordCount <= 0 || pRsCheck->GetRecordCount() <= 0)
	{
		return FALSE;
	}
	CString	strFieldName;		//�ֶ���
	CString strSQL;				//SQL���
	CString strMat;				//���²�����
	CString strTempTblName = "tmpCheckMat";		//��ʱ����
	_ConnectionPtr pCon = theApp.m_pConWorkPrj;	//���ʱĿ�����ݿ�

	_RecordsetPtr	pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	try
	{
		if (!IsTableExists(pCon, strTempTblName))
		{
			//������ʱ��
			strSQL = "CREATE TABLE ["+strTempTblName+"] (TmpMat char)" ;
			pCon->Execute(_bstr_t(strSQL), NULL, adCmdText);
		}
		strSQL = "DELETE * FROM ["+strTempTblName+"]";
		pCon->Execute(_bstr_t(strSQL), NULL, adCmdText);

		strSQL = "SELECT * FROM ["+strTempTblName+"]";
		pRs->Open(_variant_t(strSQL), pCon.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		
		for (int i=1; i<=nFieldCount; i++)
		{
			strFieldName.Format("%s%d",strField,i);
			pRsCheck->MoveFirst();
			while ( !pRsCheck->adoEOF )
			{
				strMat = vtos(pRsCheck->GetCollect(_variant_t(strFieldName)));
				if (!strMat.IsEmpty())
				{
					//���������ӵ���ʱ����ı���
					pRs->AddNew();
					pRs->PutCollect(_variant_t("TmpMat"), _variant_t(strMat));
					pRs->Update();
				}
				pRsCheck->MoveNext();
			}		
		}
		if (pRs->State == adStateOpen)
		{
			pRs->Close();
		}
		strSQL = "SELECT DISTINCT TmpMAT FROM ["+strTempTblName+"]";
		pRs->Open(_variant_t(strSQL), pCon.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);

		//��鱣�²����Ƿ���Ŀ�����ݿ��д��ڡ�
		for (int pos=1; !pRs->adoEOF; pRs->MoveNext())
		{	
			pRsA_MAT->MoveFirst();
			strMat = vtos( pRs->GetCollect(_variant_t("TmpMat")) );
			strSQL = "MAT_NAME='"+strMat+"' ";
			pRsA_MAT->Find(_bstr_t(strSQL), 0, adSearchForward);
			if (pRsA_MAT->adoEOF)
			{
				if (pRsA_Com != NULL && pRsA_Com->GetRecordCount() > 0)
				{
					//�����[���²������׼���]���м�⣬���������Ҫ�͸��ϱ��½ṹ��Ƚϡ�
					pRsA_Com->MoveFirst();
					strSQL = "COMPLEX='"+strMat+"' ";
					pRsA_Com->Find(_bstr_t(strSQL), 0, adSearchForward);
					if (!pRsA_Com->adoEOF)
					{
						//�������Ǹ��ϱ��½ṹ
						continue;
					}
				}
				strMessage += strMat;
				if( pos++ % 5 == 0 )
				{
					strMessage += "\r\n";
				}
				else
				{
					strMessage += ",\t";
				}
			}
		}
	}
	catch (_com_error)
	{
		return	FALSE;
	}

	return	TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/09/01]
// Parameter(s) :
// Return       :
// Remark       :����֧�����
//------------------------------------------------------------------
void CAutoIPEDView::OnCalcSupportSpan() 
{
	
	CCalcSupportSpanDll  dlg;

	dlg.m_strMaterialDbPath =EDIBgbl::sMaterialPath + _T("Material.mdb");
	dlg.m_strMediaDbPath = gsShareMdbDir + "Medium.mdb";
	dlg.m_strPipeDbPath = EDIBgbl::sCritPath + EDIBgbl::sCur_CritDbName;
	dlg.SetMaterialConnect( theApp.m_pConMaterial );
	dlg.SetMaterialNameConnect( theApp.m_pConMaterialName );
	dlg.ShowSupportDlg();

}

//��Ӧ��ɫ����ͷ����������ơ�
void CAutoIPEDView::OnMenuiRingName() 
{
	CString SQL;
	if (EDIBgbl::SelPrjID.IsEmpty()) 
	{
		AfxMessageBox("δѡ�񹤳�");
		return;
	}
	try
	{
		SQL.Format("SELECT * FROM [A_RING_PAINT] ");
		OpenATableToShow(SQL,theApp.m_pConnectionCODE,_T("ɫ����ͷ����������ƿ�"),_T("A_RING_PAINT"));

	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}

}

void CAutoIPEDView::OnUpdateOutView(CCmdUI* pCmdUI) 
{
	if ( ((CMainFrame*)::AfxGetMainWnd())->m_ReportCalInfoBar.IsWindowVisible() )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

// �����е����������С��
BOOL CAutoIPEDView::MinimizeAllWindow()
{
	CString strKey;
	POSITION pos;
	CObject  * pObject;
	CWnd *  pWnd;

	pos =  m_ModelessDlgMap.GetStartPosition();
	while(pos!=NULL)
	{
		m_ModelessDlgMap.GetNextAssoc(pos,strKey,pObject);
		pWnd = (CWnd*)pObject;
		if(!IsWindow(pWnd->GetSafeHwnd()))
		{
			//���Ѿ����ٵĴ��ڴ�map���Ƴ�
			m_ModelessDlgMap.RemoveKey(strKey);
			delete pWnd;
			pos=m_ModelessDlgMap.GetStartPosition();
		}
		else
		{
			pWnd->ShowWindow(SW_MINIMIZE);
		}
	}	

	return TRUE;
}

void CAutoIPEDView::OnAllMinimize() 
{
	// �����е����������С��
	MinimizeAllWindow();
}

void CAutoIPEDView::OnUpdateAllMinimize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	POSITION pos;
	pos = m_ModelessDlgMap.GetStartPosition();
	if (pos != NULL)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
	
}

//------------------------------------------------------------------                
// Parameter(s) : 
// Return       : 
// Remark       : �滻��λ
//------------------------------------------------------------------
void CAutoIPEDView::OnMenuReplaceUnit() 
{
	if (EDIBgbl::ReplaceFieldValue(EDIBgbl::SelPrjID))
	{
		CString strMessage;
		strMessage.LoadString(IDS_STRING_REPLACE_SUCCESS);
		AfxMessageBox(strMessage);
	}
}
//------------------------------------------------------------------                
// Parameter(s) : 
// Return       : 
// Remark       : �¾ɵ�λ����
//------------------------------------------------------------------
void CAutoIPEDView::OnNewOldUnit() 
{
	CString strSQL;
	strSQL.Format("SELECT * FROM [A_UNIT] ");
	try
	{
		OpenATableToShow(strSQL, theApp.m_pConnectionCODE, "�¾ɵ�λ���ձ�", _T("A_UNIT"));
	}
	catch(_com_error &e )
	{
		ReportExceptionError(e);
	}	
}

void CAutoIPEDView::OnImportOtherIpedSw() 
{
	// TODO: Add your command handler code here
	InputOtherData otherdata;
	otherdata.SelectFile(2);
}

void CAutoIPEDView::OnImportOtherIpedJs() 
{
	// TODO: Add your command handler code here
	InputOtherData otherdata;
	otherdata.SelectFile(3);
	//AfxMessageBox("xxxx");
}
