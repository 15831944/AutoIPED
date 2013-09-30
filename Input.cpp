// Input.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "Input.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Input dialog


Input::Input(CWnd* pParent /*=NULL*/)
	: CDialog(Input::IDD, pParent)
{
	//{{AFX_DATA_INIT(Input)
	m_static = _T("");
	m_input = 0.0;
	m_StringInput = _T("");
	//}}AFX_DATA_INIT

	m_IsStringInput=false;	// Ĭ�ϵ�����Ĳ����ַ��������������һ��doubleֵ
}


void Input::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Input)
	DDX_Text(pDX, IDC_EDIT_static, m_static);
	//}}AFX_DATA_MAP

	if(m_IsStringInput)
	{
		DDX_Text(pDX, IDC_EDIT_input, m_StringInput);
	}
	else
	{
		DDX_Text(pDX, IDC_EDIT_input, m_input);
	}
}


BEGIN_MESSAGE_MAP(Input, CDialog)
	//{{AFX_MSG_MAP(Input)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Input message handlers

///////////////////////////////////////////////
//
// ������������ַ�����������
//
// IsStringInput[in]	���Ϊtrue��������ַ���
//						���Ϊfalse���������
//
void Input::SetStringInput(bool IsStringInput)
{
	m_IsStringInput=IsStringInput;
}

////////////////////////////////////////////
//
// ������������ַ���������ֵ
//
// ���Ϊtrue����õ����ַ���������m_StringInput���
// ���Ϊfalse����õ�����ֵ������m_input���
//
bool Input::GetStringInput()
{
	return m_IsStringInput;
}
