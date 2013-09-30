// CalcAlpha_CodeGB.cpp: implementation of the CCalcAlpha_CodeGB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "CalcAlpha_CodeGB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------
// DATE         :[2005/08/12]
// Parameter(s) :
// Return       :
// Remark       :���ù��ұ�׼�������ϵ��
//------------------------------------------------------------------
CCalcAlpha_CodeGB::CCalcAlpha_CodeGB()
{

}

CCalcAlpha_CodeGB::~CCalcAlpha_CodeGB()
{

}

//------------------------------------------------------------------
// DATE         :[2005/08/11]
// Parameter(s) :
// Return       :
// Remark       :���ұ�׼�淶:�ھ��ú��ʱ�������ϵ���ļ��㹫ʽ
//------------------------------------------------------------------
BOOL CCalcAlpha_CodeGB::CalcEconomy_Alpha(double &dresAlpha)
{
	dresAlpha = 1.163*(10 +6*sqrt(dA_W));

	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/08/11]
// Parameter(s) :D1,��Ϊ˫��ʱӦ���������⾶, dresAlpha,���ؼ������õķ���ϵ��
// Return       :
// Remark       :���ұ�׼�淶:��Ч������о��еķ���ϵ��.
//------------------------------------------------------------------
BOOL CCalcAlpha_CodeGB::CalcEffectTest_Alpha(double D1,double &dresAlpha)
{
	double dAr=0;		//�������ϵ��
	double dAc=0;		//��������ϵ��
 
	dAr = 5.669*dA_hedu/(dA_ts-dA_ta)*(pow((273+dA_ts)/100,4)-pow((273+dA_ta)/100,4));
	if (dA_W <= 0)
	{
		//�޷�ʱ,��������ϵ����ȡֵ
		dAc = 26.4/sqrt(397 + 0.5*(dA_ts+dA_ta)) * pow((dA_ts-dA_ta)/D1,0.25);
	}
	else
	{	//�з�ʱ
		
		if (dA_W*D1 <= 0.8) 
		{
			dAc = 4.04 * pow(dA_W, 0.613)/pow(D1, 0.382);
		}
		else
		{ 
			dAc = 4.24*pow(dA_W, 0.805)/pow(D1, 0.15);
		}
	}
	dresAlpha = dAr + dAc;
	
	return TRUE;
}


//------------------------------------------------------------------
// DATE         :[2005/08/25]
// Parameter(s) :
// Return       :
// Remark       :���ұ�׼�淶:������(������¶ȷ�)�����еķ���ϵ��
//------------------------------------------------------------------
BOOL CCalcAlpha_CodeGB::CalcPreventScald_Alpha(double &dresAlpha)
{
	dresAlpha = 8.141;

	return TRUE;
}
