// CalcAlpha_CodePC.cpp: implementation of the CCalcAlpha_CodePC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "CalcAlpha_CodePC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ABREAST_LAY		4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcAlpha_CodePC::CCalcAlpha_CodePC()
{

}

CCalcAlpha_CodePC::~CCalcAlpha_CodePC()
{

}

//------------------------------------------------------------------
// DATE         :[2005/08/12]
// Parameter(s) :
// Return       :
// Remark       :ʯ�ͻ�����ҵ��׼:�ڱ��½ṹ������¶�ʱ�������ϵ����ʽ
//------------------------------------------------------------------
BOOL CCalcAlpha_CodePC::CalcFaceTemp_Alpha(double &dresAlpha)
{
	//test 
	dresAlpha = 0;
	//�����¶ȷ���
	if (ABREAST_LAY == dA_AlphaIndex)
	{
		//���ŷ���
		dresAlpha = 7.0 + 3.5 * sqrt(dA_W);		
	}
	else
	{
		//��������
		dresAlpha = 11.6 + 7.0 * sqrt(dA_W);
	}

	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/08/12]
// Parameter(s) :
// Return       :
// Remark       :ʯ�ͻ�����ҵ��׼:���ú�ȷ�ʱ�������ϵ���Ĺ�ʽ
//------------------------------------------------------------------
BOOL CCalcAlpha_CodePC::CalcEconomy_Alpha(double &dresAlpha)
{
	dresAlpha = 11.6;


	return TRUE;
}
