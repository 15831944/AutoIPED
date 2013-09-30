// CalcAlpha_Code.cpp: implementation of the CCalcAlpha_Code class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "CalcAlpha_Code.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcAlpha_Code::CCalcAlpha_Code()
{

}

CCalcAlpha_Code::~CCalcAlpha_Code()
{

}


//------------------------------------------------------------------
// DATE         :[2005/08/11]
// Parameter(s) :
// Return       :
// Remark       :���ݵ�����ҵ�Ĺ淶����ƽ��ķ���ϵ��
//------------------------------------------------------------------
BOOL CCalcAlpha_Code::CalcPlain_Alpha(double &dresAlpha)
{
	double dAn=0;		//���䴫��ϵ��
	double dAc=0;		//������ϵ��

	dAn = GetEradiateCoefficient();
	
	dAc = (5.93-0.015*dA_ta) * pow(dA_W,0.8)/pow(dA_B,0.2);
	//����ƽ��Ĵ���ϵ��
	dresAlpha = dAn + dAc;

	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/08/11]
// Parameter(s) :D1,��Ϊ˫��ʱӦ���������⾶, dresAlpha,���ؼ������õķ���ϵ��
// Return       :
// Remark       :���ݵ�����ҵ�Ĺ淶����ܵ��ķ���ϵ��
//------------------------------------------------------------------
BOOL CCalcAlpha_Code::CalcPipe_Alpha(double D1,double &dresAlpha)
{
	double dAn;		//���䴫��ϵ��.
	double dAc;		//��������ϵ��.

	dAn = GetEradiateCoefficient();
	dAc = 72.81*pow(dA_W,0.6)/pow(D1,0.4);

	dresAlpha = dAn + dAc;

	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/08/11]
// Parameter(s) :
// Return       :
// Remark       :��÷��䴫��ϵ��
//------------------------------------------------------------------
double CCalcAlpha_Code::GetEradiateCoefficient()
{
	double dAn;		//���䴫��ϵ��
	dAn = 5.67*dA_hedu/(dA_ts-dA_ta)*(pow((273+dA_ts)/100,4.0)-pow((273+dA_ta)/100,4.0));
	
	return dAn;
}
