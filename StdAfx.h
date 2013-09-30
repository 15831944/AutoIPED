// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8F862087_B060_11D7_9BCC_000AE616B8F1__INCLUDED_)
#define AFX_STDAFX_H__8F862087_B060_11D7_9BCC_000AE616B8F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT 0x0400

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxpriv.h>		// required for WM_SETMESSAGESTRING
#include <afxtempl.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "OLEDBERR.H"
#include <comdef.h>
#include "ExceptionInfoHandle.h"
#include "afxdb.h"
#include <math.h>

#import "c:\program files\common files\system\ado\msado15.dll" \
	no_namespace \
	rename("EOF","adoEOF") rename("EditModeEnum","adoEditModeEnum")\
	rename("LockTypeEnum","adoLockTypeEnum") rename("FieldAttributeEnum","adoFieldAttributeEnum")\
	rename("DataTypeEnum","adoDataTypeEnum") rename("ParameterDirectionEnum","adoParameterDirectionEnum")\
	rename("RecordStatusEnum","adoRecordStatusEnum") 

#define CONNECTSTRING "Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source="
#define CONNECTSTRING_EXCEL "Provider=Microsoft.Jet.OLEDB.4.0;Extended properties=Excel 5.0;Data Source="
#define CONNECTSTRING_DBDRV "Provider=Microsoft.Jet.OLEDB.4.0;Extended properties=%s;Data Source=%s"


enum
{
	nEconomicalThicknessMethod=0,				//���ú�ȷ����㱣�º��
	nSurfaceTemperatureMethod=1,				//�����¶ȷ����㱣�º��
	nHeatFlowrateMethod=2,						//����ɢ���ܶȷ����㱣�º��
	nPreventCongealMethod=3,					//�������·����㱣�º��
	nSubterraneanMethod=4,						// ��عܵ����º�ȵļ��㷽��

};
enum
{
	nNoChannel=0,	// ����ֱ�����(�޹ܹ�)
	nObstruct=1,	// ���·����йܹ�����ͨ�еع���

};

#include "afxdao.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8F862087_B060_11D7_9BCC_000AE616B8F1__INCLUDED_)
