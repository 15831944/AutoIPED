// ImportFromPreCalcXLSDlg.h: interface for the CImportFromPreCalcXLSDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMPORTFROMPRECALCXLSDLG_H__7F095399_65C7_4863_B7E5_3CC7A4DF9511__INCLUDED_)
#define AFX_IMPORTFROMPRECALCXLSDLG_H__7F095399_65C7_4863_B7E5_3CC7A4DF9511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImportFromXLSDlg.h"

/////////////////////////////////////////////////////////////
//
// ��CImportFromXLSDlgΪ������ⲿEXCEL�ļ��������ݵ�
// PRE_CALC��ĶԻ���
//
//////////////////////////////////////////////////////////////

class CImportFromPreCalcXLSDlg : public CImportFromXLSDlg  
{
public:
	CImportFromPreCalcXLSDlg(CWnd* pParent = NULL);
	virtual ~CImportFromPreCalcXLSDlg();

protected:
	virtual void BeginImport();
	virtual BOOL InitPropertyWnd();
	virtual BOOL ValidateData();

};

#endif // !defined(AFX_IMPORTFROMPRECALCXLSDLG_H__7F095399_65C7_4863_B7E5_3CC7A4DF9511__INCLUDED_)
