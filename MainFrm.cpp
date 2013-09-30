// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "AutoIPED.h"

#include "MainFrm.h"

#include "AutoIPEDView.h"
#include "htmlhelp.h"
#include "EDIBgbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_MOVE()
	ON_COMMAND(ID_MENU_HELP, OnMenuHelp)
	ON_COMMAND(ID_MENU_Index, OnMENUIndex)
	ON_COMMAND(ID_MENU_Search, OnMENUSearch)
	ON_COMMAND(IDM_VIEW_INFORMATION_BAR, OnViewInformationBar)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_INFORMATION_BAR, OnUpdateViewInformationBar)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ADDPROGRESS,AddProgress)
	ON_MESSAGE(WM_REMOVEPROGRESS,RemoveProgress)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
static UINT indicators1[] =
{
	ID_SEPARATOR,           // status line indicator
    ID_PROGRESS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

class CAutoIPEDView;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	bmp8.LoadBitmap(IDB_BITMAP9);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// ���������������BAR
	this->m_ReportCalInfoBar.Create(this,CReportCalErrorDlgBar::IDD,CBRS_BOTTOM,IDB_REPORT_ERROR_BAR);
	this->m_ReportCalInfoBar.SetWindowText(_T("��Ϣ��"));

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	this->m_ReportCalInfoBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&this->m_ReportCalInfoBar);

	//���ر����������BAR
	this->m_ReportCalInfoBar.HideDlgBar();

//	CWnd*parent=GetParent();
	CMenu*pmenubar=this->GetMenu();
//	CMenu*pmenu=pmenubar->GetSubMenu(6);
	pmenubar->SetMenuItemBitmaps(ID_APP_ABOUT,MF_BYCOMMAND,&bmp8,&bmp8);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	
    CAutoIPEDView *pView =(CAutoIPEDView *)GetActiveView(); 

	if(pView != NULL)
	{
		::SendMessage(pView->GetSafeHwnd(),WM_SIZE,0,0);
	}
}

LRESULT CMainFrame::AddProgress(WPARAM wParam,LPARAM lParam)
{
	m_wndStatusBar.SetIndicators(indicators1,sizeof(indicators1)/sizeof(UINT));

	m_wndStatusBar.SetProgressWndPos(1);
	//m_wndStatusBar.SetPaneText(0,_T("���Ժ�..."),true);
	return 1;
}


LRESULT CMainFrame::RemoveProgress(WPARAM wParam,LPARAM lParam)
{
	m_wndStatusBar.ShowProgressWnd(FALSE);
	m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));
	return 1;
}

void CMainFrame::OnMenuHelp() 
{
	HtmlHelp(NULL,AfxGetApp()->m_pszHelpFilePath,HH_DISPLAY_TOC,NULL);

}

void CMainFrame::OnMENUIndex() 
{
	HtmlHelp(NULL,AfxGetApp()->m_pszHelpFilePath,HH_DISPLAY_INDEX,NULL);
}

void CMainFrame::OnMENUSearch() 
{
	HtmlHelp(NULL,AfxGetApp()->m_pszHelpFilePath,HH_HELP_FINDER,NULL);
}

////////////////////////////////////////////
//
// ��Ӧ"�鿴��Ϣ��"
//
void CMainFrame::OnViewInformationBar() 
{
	//
	// �����Ϣ����ʾ������
	// ��������ʾ
	//
	if(this->m_ReportCalInfoBar.IsWindowVisible())
	{
		this->m_ReportCalInfoBar.HideDlgBar();
	}
	else
	{
		this->m_ReportCalInfoBar.ShowDlgBar();
	}
}

void CMainFrame::OnUpdateViewInformationBar(CCmdUI* pCmdUI) 
{
	if(this->m_ReportCalInfoBar.IsWindowVisible())
	{
		pCmdUI->SetCheck();
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

//////////////////////////////////////////////////////
//
// ������Ϣ���ڵ�����
//
// szInfo[in]	����ʾ����Ϣ��������
//
void CMainFrame::SetReportBarContent(LPCTSTR szInfo)
{
	this->m_ReportCalInfoBar.SetReportContent(szInfo);
}

//////////////////////////////////////////////////
//
// ������Ϣ���ڵ�����
//
CString CMainFrame::GetReportBarContent()
{
	return this->m_ReportCalInfoBar.GetReportContent();
}

void CMainFrame::ShowReportBar(bool bShow)
{
	if(bShow)
	{
		this->m_ReportCalInfoBar.ShowDlgBar();
	}
	else
	{
		this->m_ReportCalInfoBar.HideDlgBar();
	}
}

/////////////////////////////////////////////
//
// ��ʾ��ǰ�Ĺ������ڱ�������
//
void CMainFrame::ShowCurrentProjectName()
{
	_RecordsetPtr IRecordset;
	CString SQL,strTemp;
	HRESULT hr;
	_variant_t varTemp;

	//
	// ���ѡ���˹��̽����̵�������ʾ�ڱ�������
	//
	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		this->SetWindowText(_T("���ױ������Ṥ�����AutoIPED"));
	}
	else
	{
		hr=IRecordset.CreateInstance(__uuidof(Recordset));

		if(FAILED(hr))
		{
			_com_error e(hr);
			ReportExceptionErrorLV1(e);
			return;
		}

		SQL.Format(_T("SELECT gcmc FROM Engin WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

		try
		{
			IRecordset->Open(_variant_t(SQL),_variant_t((IDispatch*)(((CAutoIPEDApp*)::AfxGetApp())->m_pConAllPrj)),
							 adOpenDynamic,adLockOptimistic,adCmdText);

			if(IRecordset->adoEOF && IRecordset->BOF)
			{
				ReportExceptionErrorLV1(_T("û�д˹���"));
				return;
			}

			varTemp=IRecordset->GetCollect(_variant_t("gcmc"));
		}
		catch(_com_error &e)
		{

			ReportExceptionErrorLV1(e);
			return;
		}
		//
		EDIBgbl::SelPrjName = vtos(varTemp);
		strTemp.Format(_T("���ױ������Ṥ�����AutoIPED-[%s]    (%s %s)"),EDIBgbl::SelPrjName, EDIBgbl::\
			sCur_CodeName,EDIBgbl::sCur_CodeNO);

		this->SetWindowText(strTemp);
	}
}



void CMainFrame::OnClose() 
{
	//
	// �����������͹ر����㣬��ÿ��0.3�������ж��Ƿ�������
	// ���ֹͣ�����˾͹ر�Ӧ�ó��򣬷�������ж�
	//
	if(((CAutoIPEDDoc*)GetActiveDocument())->Operation())
	{
		((CAutoIPEDDoc*)GetActiveDocument())->OnButtonStopcal();

		this->SetTimer(ID_STOP_TIME,300,NULL);

		return;
	}

	CFrameWnd::OnClose();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==ID_STOP_TIME)
	{
		//
		// �ж��Ƿ����ڼ��㣬���ֹͣ����͹ر�Ӧ�ó���
		//
		if(((CAutoIPEDDoc*)GetActiveDocument())->Operation())
		{
			return;
		}

		KillTimer(nIDEvent);

		this->PostMessage(WM_CLOSE);
	}

	CFrameWnd::OnTimer(nIDEvent);
}
