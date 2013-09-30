// DialogBarExContainer.cpp : implementation file
//

#include "stdafx.h"
#include "DialogBarExContainer.h"

#include "DialogBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogBarExContainer

CDialogBarExContainer::CDialogBarExContainer()
{
	m_pSubDialogBar=NULL;	//��ʼָ�򽫰��������е�DialogBar
}

CDialogBarExContainer::~CDialogBarExContainer()
{
}


BEGIN_MESSAGE_MAP(CDialogBarExContainer, CWnd)
	//{{AFX_MSG_MAP(CDialogBarExContainer)
	ON_WM_SIZE()
	ON_WM_MOVING()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDialogBarExContainer message handlers

void CDialogBarExContainer::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if(m_pSubDialogBar && IsWindow(m_pSubDialogBar->GetSafeHwnd()))
	{
		m_pSubDialogBar->m_FloatingSize.cx=cx;
		m_pSubDialogBar->m_FloatingSize.cy=cy;

		m_pSubDialogBar->MoveWindow(0,0,cx,cy);
	}
	
}


//////////////////////////////////////////////////////////////
//
// ��Ӧ���ڵ�WM_MOVING
//
// �жϴ����Ƿ�Ӧ��ͣ��
//
// 
void CDialogBarExContainer::OnMoving(UINT fwSide, LPRECT pRect) 
{
	CWnd::OnMoving(fwSide, pRect);
	
	CFrameWnd *pFrame;
	CRect FrameRect;
	CPoint ScreenPt;

	this->MoveWindow(pRect);

	::GetCursorPos(&ScreenPt);

	if(m_pSubDialogBar && IsWindow(m_pSubDialogBar->GetSafeHwnd()))
	{
		pFrame=m_pSubDialogBar->GetDockingFrame();
		pFrame->GetWindowRect(&FrameRect);

		//ͣ������� 
		if(abs(ScreenPt.x-FrameRect.left)<10)
		{
			pFrame->DockControlBar(m_pSubDialogBar,AFX_IDW_DOCKBAR_LEFT);

			pFrame->RecalcLayout();
			this->ShowWindow(SW_HIDE);

			this->m_pSubDialogBar=NULL;
		}
		//ͣ�����ұ�
		else if(abs(ScreenPt.x-FrameRect.right)<10)
		{
			pFrame->DockControlBar(m_pSubDialogBar,AFX_IDW_DOCKBAR_RIGHT);
			pFrame->RecalcLayout();
			this->ShowWindow(SW_HIDE);

			this->m_pSubDialogBar=NULL;
		}
		// ͣ�����ϱ�
		else if(abs(ScreenPt.y-FrameRect.top)<10)
		{
			pFrame->DockControlBar(m_pSubDialogBar,AFX_IDW_DOCKBAR_TOP);

			pFrame->RecalcLayout();

			this->ShowWindow(SW_HIDE);

			this->m_pSubDialogBar=NULL;
		}
		// ͣ�����±�
		else if(abs(ScreenPt.y-FrameRect.bottom)<10)
		{
			pFrame->DockControlBar(m_pSubDialogBar,AFX_IDW_DOCKBAR_BOTTOM);

			pFrame->RecalcLayout();

			this->ShowWindow(SW_HIDE);

			this->m_pSubDialogBar=NULL;
		}
	}	
}

void CDialogBarExContainer::OnClose() 
{
	//������Ͻǵġ�X���������ٴ��ڣ�ֻ������
	this->ShowWindow(SW_HIDE);	
//	CWnd::OnClose();
}

//////////////////////////////////////////////////////////////////////////////
//
// ���ð��������е�DialogBar,��ͣ������һ���踡��ǰ����������
//
// pDialogBar[in]	DialogBar�����ָ��
// pt[in]			��ʼ�Ĵ��ڸ���λ�ã���Ļ���꣩
//
void CDialogBarExContainer::SetSubDialogBar(CDialogBarEx *pDialogBar,CPoint pt)
{
	CSize size;

	if(!IsWindow(this->GetSafeHwnd()))
		return;

	if(pDialogBar && IsWindow(pDialogBar->GetSafeHwnd()))
	{
		//
		// �������û�и���ʹ֮����
		//
		if(!pDialogBar->IsFloating())
		{
			pDialogBar->GetDockingFrame()->FloatControlBar(pDialogBar,pt);
		}

		//
		// ʹ���ݵĴ�����DialogBarͬ���Ĵ�
		// ʹDialogBarԭ���ĸ���������
		// �ı丸���ڵ�����
		//
		if(pDialogBar->GetParentOwner()!=this)
		{
			pDialogBar->GetFloatingSize(size);
			this->MoveWindow(pt.x,pt.y,size.cx,size.cy);

			pDialogBar->GetParentOwner()->ShowWindow(SW_HIDE);

			pDialogBar->SetParent(this);

			this->ShowWindow(SW_NORMAL);
		}
		else
		{
			pDialogBar->GetFloatingSize(size);

			this->MoveWindow(pt.x,pt.y,size.cx,size.cy);
		}
	}

	m_pSubDialogBar=pDialogBar;

}

////////////////////////////////////////////////////////
//
// ���ذ��������е�DialogBar
//
CDialogBarEx* CDialogBarExContainer::GetSubDialogBar()
{
	return m_pSubDialogBar;
}

