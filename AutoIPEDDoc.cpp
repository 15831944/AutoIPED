// AutoIPEDDoc.cpp : implementation of the CAutoIPEDDoc class
//

#include "stdafx.h"
#include "AutoIPED.h"
#include "calc.h"
#include "AutoIPEDDoc.h"
#include "RangeDlg.h"
#include "Input.h"
#include "Edibgbl.h"

#include "DataShowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAutoIPEDApp theApp;

#include "AutoIPEDView.h"
#include "CalThread.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDDoc

IMPLEMENT_DYNCREATE(CAutoIPEDDoc, CDocument)

BEGIN_MESSAGE_MAP(CAutoIPEDDoc, CDocument)
	//{{AFX_MSG_MAP(CAutoIPEDDoc)
	ON_COMMAND(ID_MENUITEM32, OnMenuitem_calc)
	ON_COMMAND(ID_BUTTON_STOPCAL, OnButtonStopcal)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_STOPCAL, OnUpdateButtonStopcal)
	//ON_UPDATE_COMMAND_UI(ID_MENUITEM32, OnUpdateMenuitem_calc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDDoc construction/destruction

CAutoIPEDDoc::CAutoIPEDDoc()
{
	IsStopCalc=TRUE;

	gbIsStopCalc = TRUE;	//ȫ��

	m_pCalThread=NULL;

	m_ErrorInfo=_T("");
}

CAutoIPEDDoc::~CAutoIPEDDoc()
{
	if(m_pCalThread!=NULL)
	{
		if(m_pCalThread->m_hThread!=NULL)
		{
			IsStopCalc=TRUE;
			gbIsStopCalc = TRUE;
			::WaitForSingleObject(m_pCalThread->m_hThread,INFINITE);
			::CloseHandle(m_pCalThread->m_hThread);
			m_pCalThread->m_hThread=NULL;

		}
		delete m_pCalThread;
	}
}

BOOL CAutoIPEDDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	this->SetTitle("");

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDDoc serialization

void CAutoIPEDDoc::Serialize(CArchive& ar)
{

	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);

	if (ar.IsStoring())
	{
	}
	else
	{
		((CEditView*)m_viewList.GetHead())->GetWindowText(m_Result);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDDoc diagnostics

#ifdef _DEBUG
void CAutoIPEDDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAutoIPEDDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAutoIPEDDoc commands

void CAutoIPEDDoc::DisplayRes(LPCTSTR pStr)
{

	m_Result+=pStr;
//	m_Result=pStr;
	m_Result+=_T("\r\n");
    m_Result+=_T("\r\n");
//	this->UpdateAllViews(NULL);
	POSITION Pos;
	CView *pView;
	CAutoIPEDView *pAutoIPEDView;
//	m_Result+=pStr;
//	m_Result+=_T("\r\n");

//	this->UpdateAllViews(NULL);

	Pos=this->GetFirstViewPosition();
	while(Pos!=NULL)
	{
		pView=this->GetNextView(Pos);

		if(pView->IsKindOf(RUNTIME_CLASS(CAutoIPEDView)))
		{
			pAutoIPEDView=(CAutoIPEDView*)pView;
			pAutoIPEDView->OnUpdate(NULL,0,NULL);
		}
		pView->UpdateWindow();
	}

//	SetModifiedFlag(TRUE);
}

void CAutoIPEDDoc::Say(LPCTSTR pStr)
{
	DisplayRes(pStr);
}

//////////////////////////////////////////
//
// ��ʼ���¼���
//
void CAutoIPEDDoc::OnMenuitem_calc() 
{
	HRESULT hr;
	CString strProjectID;

	strProjectID=EDIBgbl::SelPrjID;

	strProjectID.TrimLeft();
	strProjectID.TrimRight();
	if(strProjectID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�!"));
		return;
	}

	if(m_pCalThread!=NULL)
	{
		if(m_pCalThread->m_hThread!=NULL)
		{
			IsStopCalc=TRUE;
			gbIsStopCalc = TRUE;
			::WaitForSingleObject(m_pCalThread->m_hThread,INFINITE);
			::CloseHandle(m_pCalThread->m_hThread);
			m_pCalThread->m_hThread=NULL;

		}
		delete m_pCalThread;
	}

	m_pCalThread=new CCalThread;
	m_pCalThread->m_bAutoDelete=FALSE;

	calc calc1;
	if(calc1.DoModal()==IDOK)
	{

		CWaitCursor wait;

		SetChCal(calc1.i);

		m_pCalThread->m_pDoc=this;

		hr=::CoMarshalInterThreadInterfaceInStream(__uuidof(_Connection),
										(LPUNKNOWN)theApp.m_pConnection,
										&m_pCalThread->m_IStream);

		if(FAILED(hr))
		{
			ReportExceptionErrorLV1(_com_error(hr));
			return;
		}

		hr=::CoMarshalInterThreadInterfaceInStream(__uuidof(_Connection),
												   (LPUNKNOWN)theApp.m_pConnectionCODE,
												   &m_pCalThread->m_IStreamCODE);

		if(FAILED(hr))
		{
			ReportExceptionErrorLV1(_com_error(hr));
			return;
		}

		m_pCalThread->SetProjectName(EDIBgbl::SelPrjID);

		//����ϴμ�¼�Ĵ����¼
		this->m_ErrorInfo=_T(""); 
 
		//����ϴεļ�����
		this->m_Result=_T(""); 

		m_pCalThread->CreateThread();

		IsStopCalc=FALSE;
		gbIsStopCalc = FALSE;
	}
	else
	{
		IsStopCalc=TRUE;
		gbIsStopCalc = TRUE;
	}

}

BOOL CAutoIPEDDoc::RangeDlgshow(long &Start, long &End)
{
	RangeDlg RangeDlg1;
	RangeDlg1.m_start=1;
	RangeDlg1.m_end=End;

	if (IDOK == RangeDlg1.DoModal())
	{
		Start=RangeDlg1.m_start;
		End=RangeDlg1.m_end;
		return TRUE;
	}
	return FALSE;

}


void CAutoIPEDDoc::InputD(LPCTSTR TempStr,double &stres)
{
	Input input1;
	input1.m_static=TempStr;
	input1.DoModal();
	stres=input1.m_input;
	CString str;
	str.Format("%s%f",TempStr,stres);
	int strlen;
	strlen=str.GetLength();
	str=str.Right(strlen-6);
	DisplayRes(LPCTSTR (str));
}

void CAutoIPEDDoc::OnButtonStopcal() 
{
	IsStopCalc=TRUE;
	gbIsStopCalc = TRUE;
}

void CAutoIPEDDoc::Cancel(int *pState)
{
	if(IsStopCalc==FALSE)
	{
		*pState=1;
	}
	else
	{
		*pState=0;
	}
}

void CAutoIPEDDoc::SelectToCal(_RecordsetPtr &IRecordset,int *pIsStop)
{
	if(IRecordset==NULL || IRecordset->GetState()==adStateClosed)
	{
		ReportExceptionError(_T("������"));

		*pIsStop=1;
	}
	
	CDataShowDlg dlg;
	dlg.SetRecordset(IRecordset);

	dlg.SetDlgCaption(_T("ѡ�����������"));
	dlg.SetDataGridCaption(_T("PRE_CALC"));

	dlg.SetAllowUpdate(TRUE);

	//����EnginID�ֶ�
	dlg.SetHideColumns(_T("EnginID"));

	dlg.SetPassField(_T("c_pass"));

	//��λ����ע��.
	dlg.SetCursorCol(11);

	dlg.DoModal();

}

void CAutoIPEDDoc::OnUpdateButtonStopcal(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsStopCalc==TRUE ? FALSE:TRUE);	
}

void CAutoIPEDDoc::OnUpdateMenuitem_calc(CCmdUI* pCmdUI) 
{
	_RecordsetPtr IRecordset;
	CString SQL;
	HRESULT hr;

	if(EDIBgbl::SelPrjID.IsEmpty())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//
	// ���ݿ�Ϊ��ʱ����Ҳ������
	//
	hr=IRecordset.CreateInstance(__uuidof(Recordset));

	if(FAILED(hr))
	{
		ReportExceptionErrorLV1(_com_error(hr));
		return;
	}

	SQL.Format(_T("SELECT * FROM PRE_CALC WHERE EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		IRecordset->Open(_variant_t(SQL),_variant_t((IDispatch*)theApp.m_pConnection),
						adOpenForwardOnly,adLockReadOnly,adCmdText);

		if(IRecordset->adoEOF && IRecordset->BOF)
		{
			pCmdUI->Enable(FALSE);
			return;
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return;
	}
					

	pCmdUI->Enable(IsStopCalc==TRUE ? TRUE:FALSE);
	
}

///////////////////////////////////////////////////////
//
// ����CFoxBase��ExceptionInfo
//
// ���м������ʱ����ô˺���
//
void CAutoIPEDDoc::ExceptionInfo(LPCTSTR pErrorInfo)
{
	// ��¼����
	m_ErrorInfo+=pErrorInfo;

	m_ErrorInfo+=_T("\r\n");
	m_ErrorInfo+=_T("\r\n");
}

CString CAutoIPEDDoc::GetAllCalErrorInfo()
{
	return m_ErrorInfo;
}

////////////////////////////////////////
//
// �������ʱ��������
//
// �п��ܼ���ɹ�Ҳ�п��ܼ���ʧ��
//
void CAutoIPEDDoc::OnEndCalculate()
{

	POSITION pos = GetFirstViewPosition();
    CAutoIPEDView *pFirstView =(CAutoIPEDView *)GetNextView( pos );

	//
	// ������ͼ���͡���ʾ���������Ϣ����������Ϣ
	//
	if(!::IsBadReadPtr(pFirstView,sizeof(CAutoIPEDView)))
	{
		if(IsWindow(pFirstView->GetSafeHwnd()))
		{
			pFirstView->PostMessage(WM_COMMAND,IDM_DISPLAY_CALERROR_INFO,0);
		}
	}

}

////////////////////////////////////
//
// �ж��Ƿ����ڼ���
//
// �������true,��ʾ���ڼ���
// ����false����ʾֹͣ����
//
bool CAutoIPEDDoc::Operation()
{
	if(IsStopCalc)
		return false;
	else
		return true;
}
// ��С�����е��������
void CAutoIPEDDoc::MinimizeAllWindow()
{
	CAutoIPEDView*pIPEDView;
	CView*		  pView;
	POSITION      pos;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView != NULL)
		{
			if ( pView->IsKindOf(RUNTIME_CLASS(CAutoIPEDView)) )
			{
				pIPEDView = (CAutoIPEDView*)pView;
				pIPEDView->MinimizeAllWindow();
			}
		}
	}
}
