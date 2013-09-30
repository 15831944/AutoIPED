// DataGridEx.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "DataGridEx.h"

#include "Column.h"
#include "Columns.h"
#include "vtot.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataGridEx

CDataGridEx::CDataGridEx()
{
	m_RecordCount=-1;
	m_PreVScrollPos=0;
	m_PreVScrollFlag=FALSE;
}

CDataGridEx::~CDataGridEx()
{
}

CMap<DWORD,DWORD,CDataGridEx::tagHookCountInfo*,CDataGridEx::tagHookCountInfo*> CDataGridEx::m_MouseThreadHookMap;

BEGIN_MESSAGE_MAP(CDataGridEx, CDataGrid)
	//{{AFX_MSG_MAP(CDataGridEx)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSEWHEEL2,OnMouseWheel2)
	ON_COMMAND(IDC_GRID_ASC,OnSortAsc)
	ON_COMMAND(IDC_GRID_DESC,OnSortDesc)
	ON_COMMAND(IDC_GRID_HIDE,OnHideField)
	ON_COMMAND(IDC_GRID_CANCEL_HIDE,OnCancelHideField)

END_MESSAGE_MAP()

////////////////////////////////////////////////
//
// ��ʼ��TOOLTIP�ؼ�
//
void CDataGridEx::InitToolTipControl()
{
	if(m_RecordCount<0)
	{
		LPUNKNOWN pUnknown;
		HRESULT hr;
		_RecordsetPtr IRecordset;

		pUnknown=GetDataSource();
		
		if(pUnknown==NULL)
			return;

		try
		{
			hr=pUnknown->QueryInterface(__uuidof(_Recordset),(void**)&IRecordset);

			pUnknown->Release();

			if(FAILED(hr))
			{
				throw _com_error(hr);
			}

			m_RecordCount=IRecordset->GetRecordCount();
		}
		catch(_com_error &e)
		{
			ReportExceptionError(e);
		}
	}

	if(IsWindow(m_ToolTip.GetSafeHwnd()))
		return;

	m_ToolTip.Create(this);
	
	TOOLINFO ToolInfo;

	memset(&ToolInfo,0,sizeof(TOOLINFO));

	ToolInfo.cbSize=sizeof(TOOLINFO);
	ToolInfo.uFlags=TTF_TRACK|TTF_IDISHWND| TTF_ABSOLUTE;
	ToolInfo.hwnd=this->m_hWnd;
	ToolInfo.uId=(UINT)this->m_hWnd;
	ToolInfo.lpszText=LPSTR_TEXTCALLBACK;

	m_ToolTip.SendMessage(TTM_ADDTOOL,0,(LPARAM)&ToolInfo);
}


/////////////////////////////////////////////////////////////////////////////
// CDataGridEx message handlers


/////////////////////////////////////////////////////////////////////////////
//
// ��Ӧ�������
//
void CDataGridEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

	if(nSBCode==SB_THUMBTRACK)
	{
		short Col;

		Col=this->GetLeftCol();
		try
		{
			this->Scroll(nPos-Col,0);
		}
		catch (COleDispatchException * e)
		{
			//���õķ�Χ���ܳ���ʵ�ʵķ�Χ.
			e->Delete();
		}
	}

	CDataGrid::OnHScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
//
// ��Ӧ��ֱ����
//
void CDataGridEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(nSBCode==SB_THUMBTRACK || nSBCode==SB_THUMBPOSITION)
	{
		TOOLINFO ToolInfo;
		_variant_t varTemp;
		POINT pt;

		InitToolTipControl();

		memset(&ToolInfo,0,sizeof(TOOLINFO));

		ToolInfo.cbSize=sizeof(TOOLINFO);
		ToolInfo.uFlags=TTF_TRACK|TTF_IDISHWND| TTF_ABSOLUTE;
		ToolInfo.hwnd=this->m_hWnd;
		ToolInfo.uId=(UINT)this->m_hWnd;
		ToolInfo.lpszText=LPSTR_TEXTCALLBACK;

		if(nSBCode==SB_THUMBTRACK)
		{
//			varTemp=GetFirstRow();
//			Scroll(0,nPos-(long)varTemp+1);

			if(m_PreVScrollFlag==FALSE)
			{
				m_PreVScrollPos=nPos;
				m_PreVScrollFlag=TRUE;
			}
			else
			{
				Scroll(0,nPos-m_PreVScrollPos);

				m_PreVScrollPos=nPos;
			}

			::_stprintf(m_ToolTipInfo,_T("��¼��%d���м�¼��%d"),nPos+1,m_RecordCount);

			::GetCursorPos(&pt);
			m_ToolTip.SendMessage(TTM_TRACKPOSITION,0,MAKEWPARAM(pt.x+15,pt.y));
			m_ToolTip.SendMessage(TTM_TRACKACTIVATE,TRUE,(LPARAM)&ToolInfo);

			//����ToolTip�ؼ��ں�����ʾ��λ��
			PlaceToolTipWndToRightPosition();
		}
		else
		{
			m_ToolTip.SendMessage(TTM_TRACKACTIVATE,FALSE,(LPARAM)&ToolInfo);
		}

		if(nSBCode==SB_THUMBPOSITION)
		{
			m_PreVScrollFlag=FALSE;
		}

	}

	CDataGrid::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDataGridEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{
	LPNMTTDISPINFO 	lpnmtdi=(LPNMTTDISPINFO)lParam;

	if(lpnmtdi->hdr.code==TTN_NEEDTEXT)
	{
		lpnmtdi->lpszText=m_ToolTipInfo;
	}

	return CDataGrid::OnNotify(wParam,lParam,pResult);
}

///////////////////////////////////////////////////////
//
// ����ToolTip�ؼ��ں�����ʾ��λ��
//
void CDataGridEx::PlaceToolTipWndToRightPosition()
{
	CRect ToolTipRect,ScreenRect;

	if(!::IsWindow(this->m_ToolTip.GetSafeHwnd()))
		return;

	m_ToolTip.GetWindowRect(&ToolTipRect);

	if(ToolTipRect.IsRectEmpty())
		return;

	::GetWindowRect(::GetDesktopWindow(),&ScreenRect);

	if(ToolTipRect.right>ScreenRect.right)
	{
		ToolTipRect.OffsetRect(ScreenRect.right-ToolTipRect.right,0);

		m_ToolTip.SendMessage(TTM_TRACKPOSITION,0,MAKEWPARAM(ToolTipRect.left,ToolTipRect.top));
	}


}

//////////////////////////////////////////////
//
// ��ʼ���м������Ĺ�����Ӧ
//
// Ӧ���ڴ��ڴ����Ѻ�ŵ���
//
void CDataGridEx::InitMouseWheel()
{
	HANDLE Handle;
	tagHookCountInfo* pHookCountInfo;

	if(!IsWindow(GetSafeHwnd()))
		return;

	Handle=::GetProp(GetSafeHwnd(),_T("MouseWheel"));

	if(Handle!=NULL)
		return;

	::SetProp(GetSafeHwnd(),_T("MouseWheel"),(HANDLE)this);

	if(!m_MouseThreadHookMap.Lookup(::GetCurrentThreadId(),pHookCountInfo))
	{
		pHookCountInfo=new tagHookCountInfo;

		pHookCountInfo->hHook=::SetWindowsHookEx(WH_GETMESSAGE,CDataGridEx::GetMsgProc,
												 NULL,GetCurrentThreadId());
		
		pHookCountInfo->Count=1;

		m_MouseThreadHookMap.SetAt(GetCurrentThreadId(),pHookCountInfo);
	}
	else
	{
		pHookCountInfo->Count++;
	}
}

/////////////////////////////////////////////////////
//
// ���ӵĻ������
//
LRESULT CALLBACK CDataGridEx::GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	HANDLE Handle;
	tagHookCountInfo* pHookCountInfo;
	HWND hWnd;
	MSG *pMsg;

	pMsg=(MSG*)lParam;

	m_MouseThreadHookMap.Lookup(::GetCurrentThreadId(),pHookCountInfo);

	if(code<0 || pMsg->message!=WM_MOUSEWHEEL)
		return CallNextHookEx(pHookCountInfo->hHook,code,wParam,lParam);


	hWnd=pMsg->hwnd;

	while(TRUE)
	{
		if(!IsWindow(hWnd))
			break;

		Handle=::GetProp(hWnd,_T("MouseWheel"));

		if(Handle==NULL)
			hWnd=::GetParent(hWnd);
		else
		{
			::PostMessage(hWnd,WM_MOUSEWHEEL2,pMsg->wParam,pMsg->lParam);
			break;
		}
	}

	return CallNextHookEx(pHookCountInfo->hHook,code,wParam,lParam); 
}

LRESULT CDataGridEx::OnMouseWheel2(WPARAM wParam, LPARAM lParam)
{
	int Ro;

	Ro=int(wParam);

	if(Ro>=0)
	{
		this->Scroll(0,-2);
	}
	else
	{
		this->Scroll(0,2);
	}

	return 0;	
}

void CDataGridEx::OnRButtonDown(UINT nFlags, CPoint point) 
{
	POINT ScreenPt;
	CMenu menu;

	try
	{
//		m_SelectCol=ColContaining(static_cast<float>(point.x));

		m_SelectCol=this->GetSelStartCol();

		if(m_SelectCol<0)
			return;
	}
	catch(COleDispatchException *e)
	{
		ReportExceptionErrorLV1(e);
		e->Delete();
		return;
	}

	ScreenPt.x=point.x;
	ScreenPt.y=point.y;

	ClientToScreen(&ScreenPt);

	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING,IDC_GRID_ASC,"����(&A)");
	menu.AppendMenu(MF_STRING,IDC_GRID_DESC,"����(&C)");
	menu.AppendMenu(MF_STRING,IDC_GRID_HIDE,"������(&H)");
	menu.AppendMenu(MF_STRING,IDC_GRID_CANCEL_HIDE,"ȡ������(&U)");

	menu.TrackPopupMenu(TPM_LEFTALIGN,ScreenPt.x,ScreenPt.y,this);

	CDataGrid::OnRButtonDown(nFlags, point);
}

LRESULT CDataGridEx::OnSortAsc()
{
	CString strTemp;
	_RecordsetPtr IRecordset;
	LPUNKNOWN pUnknown;
	HRESULT hr;

	try
	{
		pUnknown=this->GetDataSource();

		hr=pUnknown->QueryInterface(__uuidof(_RecordsetPtr),(void**)&IRecordset);

		if(FAILED(hr))
		{
			pUnknown->Release();
			ReportExceptionErrorLV1(_com_error(hr));
			return 0;
		}

		pUnknown->Release();

	//	strTemp=GetColumns().GetItem(_variant_t(m_SelectCol)).GetCaption();
		
		strTemp = (LPCSTR)IRecordset->GetFields()->GetItem(_variant_t(m_SelectCol))->GetName();

		strTemp+=_T(" ASC");
		IRecordset->Sort=_bstr_t(strTemp);
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return 0;
	}
	catch(COleDispatchException *e)
	{
		ReportExceptionErrorLV1(e);
		e->Delete();
		return 0;
	}

	return 0;
}
//����ѡ����С�
LRESULT CDataGridEx::OnHideField()
{

	try
	{
		GetColumns().GetItem(_variant_t(m_SelectCol)).SetVisible(FALSE);//������
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return 0;
	}
	catch(COleDispatchException *e)
	{
		ReportExceptionErrorLV1(e);
		e->Delete();
		return 0;
	}	
	return 0;
}
//ȡ������ѡ����С�
LRESULT CDataGridEx::OnCancelHideField()
{

	try
	{
		GetColumns().GetItem(_variant_t(m_SelectCol)).SetVisible(TRUE);//ȡ��������
		
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return 0;
	}
	catch(COleDispatchException *e)
	{
		ReportExceptionErrorLV1(e);
		e->Delete();
		return 0;
	}	
	return 0;
}

LRESULT CDataGridEx::OnSortDesc()
{
	CString strTemp;
	_RecordsetPtr IRecordset;
	LPUNKNOWN pUnknown;
	HRESULT hr;

	try
	{
		pUnknown=this->GetDataSource();

		hr=pUnknown->QueryInterface(__uuidof(_RecordsetPtr),(void**)&IRecordset);

		if(FAILED(hr))
		{
			pUnknown->Release();
			ReportExceptionErrorLV1(_com_error(hr));
			return 0;
		}

		pUnknown->Release();

//		strTemp=GetColumns().GetItem(_variant_t(m_SelectCol)).GetCaption();
		strTemp = (LPCSTR)IRecordset->GetFields()->GetItem(_variant_t(m_SelectCol))->GetName();

		strTemp+=_T(" DESC");
		IRecordset->Sort=_bstr_t(strTemp);
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return 0;
	}
	catch(COleDispatchException *e)
	{
		ReportExceptionErrorLV1(e);
		e->Delete();
		return 0;
	}

	
	return 0;
}

////////////////////////////////////////////
//
// �ͷŹ�����Դ��Ӧ�ڴ�������ǰ����
//
void CDataGridEx::UnInitMouseWheel()
{
	tagHookCountInfo* pHookCountInfo;
	HANDLE Handle;

	if(!IsWindow(GetSafeHwnd()))
	{
		return;
	}

	Handle=::GetProp(GetSafeHwnd(),_T("MouseWheel"));

	if(Handle!=NULL)
	{
		RemoveProp(GetSafeHwnd(),_T("MouseWheel"));
	}
	else
	{
		return;
	}

	//
	// ����ʹ�ô��̹߳��ӵļ���,������Ϊ0ʱ
	// ��������̵߳Ĺ���,����map���Ƴ�
	//
	if(m_MouseThreadHookMap.Lookup(::GetCurrentThreadId(),pHookCountInfo))
	{
		pHookCountInfo->Count--;

		if(pHookCountInfo->Count==0)
		{
			::UnhookWindowsHookEx(pHookCountInfo->hHook);

			m_MouseThreadHookMap.RemoveKey(::GetCurrentThreadId());

			delete pHookCountInfo;
		}
	}	
}
