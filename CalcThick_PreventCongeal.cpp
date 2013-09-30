// CalcThick_PreventCongeal.cpp: implementation of the CCalcThick_PreventCongeal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "CalcThick_PreventCongeal.h"
#include "vtot.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcThick_PreventCongeal::CCalcThick_PreventCongeal()
{

}

CCalcThick_PreventCongeal::~CCalcThick_PreventCongeal()
{

}


//------------------------------------------------------------------                
// DATE         :[2005/06/02]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�����ᱣ��(��ƽ��)�����㱣�²��
//------------------------------------------------------------------
short CCalcThick_PreventCongeal::CalcPipe_One(double &thick_resu, double &ts_resu)
{
	CalcThickFormula(thick_resu,ts_resu);
	return 1;
}

//------------------------------------------------------------------                
// DATE         :[2005/06/02]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�����ᱣ��(��ƽ��)�����㱣�²��
//------------------------------------------------------------------
short CCalcThick_PreventCongeal::CalcPipe_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	thick1_resu = thick2_resu = tb_resu = ts_resu = 0;
	//ֻ�������ı��º�.
	CalcThickFormula(thick2_resu, ts_resu);
	
	return 1;
}

//------------------------------------------------------------------                
// DATE         :[2005/06/02]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�����ᱣ��(��ƽ��)�����㱣�²��
//------------------------------------------------------------------
short CCalcThick_PreventCongeal::CalcPlain_One(double &thick_resu, double &ts_resu)
{
	CalcThickFormula(thick_resu,ts_resu, FALSE);
	return 1;
}

//------------------------------------------------------------------                
// DATE         :[2005/06/02]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�����ᱣ��(��ƽ��)�����㱣�²��
//------------------------------------------------------------------
short CCalcThick_PreventCongeal::CalcPlain_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	thick1_resu = thick2_resu = tb_resu = ts_resu = 0 ;

	//ֻ�������ı��º�
	CalcThickFormula(thick2_resu,ts_resu, FALSE);

	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/06/02]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :��������Ĺ�ʽ��ͨ����һ����ʽ��
//------------------------------------------------------------------
short CCalcThick_PreventCongeal::CalcThickFormula( double& thick_resu, double& ts_resu, BOOL bIsPipe)
{
	double ts;	//������¶�
	double dCurQ = 0;
	bool   flg = true;
	thick_resu = ts_resu = 0;

	double QMax;
	//����ý����¶���������ɢ���ܶ�
	GetHeatLoss(m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax);

	//��÷��������ԭʼ����
	GetCongealOriginalData(m_nID);

	double Diff;					//��ֵ
	double dMaxDiff = DZero;		//��С�Ĳ�ֵ
	//
	for (thick2=m_thick2Start; thick2 <= m_thick2Max; thick2 += m_thick2Increment)
	{
		//
		if (bIsPipe)
		{
			CalcPipe_One_TsTemp(thick2,ts);
		}
		else
		{
			CalcPlain_One_TsTemp(thick2,ts);
		}
		D1 = D0 + 2 * thick2;			//���²��⾶
		lamda = GetLamda(t,ts);			//���㵼��ϵ��
		//���ݲ�ͬ����ҵ�淶�������ϵ��
		ALPHA = GetPipeAlpha(ts,D1);
		//
		Diff = log(D1/D0)+2*lamda/D1/ALPHA-2*lamda*(Kr*taofr/(2*(t-tfr)*(D0*D0/4*ro*C+(D0+pi_thi)*pi_thi*rop*Cp)/(t+tfr-2*ta)+0.25*D0*D0/4*ro*Hfr/(tfr-ta)));
		
		Calc_Q_PipeOne(dCurQ);
		if ( ts < MaxTs && (dMaxDiff <= DZero || fabs(Diff) < fabs(dMaxDiff)) )
		{
			flg = true;
			if (bIsHeatLoss)
			{
				Calc_Q_PipeOne(dCurQ);
			}
			if ( flg )
			{
				dMaxDiff	= Diff;		//���µ�ǰ��С�Ĳ�ֵ
				thick_resu	= thick2;	//���º�
				ts_resu		= ts;
				Calc_Q_PipeOne(dQ);
			}
		}
	}
	return 1;
}

//------------------------------------------------------------------
// DATE         :[2005/06/06]
// Author       :ZSY
// Parameter(s) :��ǰ��¼�����nID
// Return       :
// Remark       :��õ�ǰ�����¼�ķ������µĲ���.
//------------------------------------------------------------------
BOOL CCalcThick_PreventCongeal::GetCongealOriginalData(long nID)
{
	try
	{	
		if (m_pRsCongeal==NULL || m_pRsCongeal->State == adStateClosed || m_pRsCongeal->GetRecordCount() <= 0 )
		{
			return FALSE;
		}
		CString	strSQL;		//SQL���
		strSQL.Format("ID=%d",nID);

		m_pRsCongeal->MoveFirst();
		m_pRsCongeal->Find(_bstr_t(strSQL), 0, adSearchForward);

		//����ָ����ŵķ������¼���Ĳ���
		if ( !m_pRsCongeal->adoEOF )
		{
			//�ܵ�ͨ��֧���ܴ�����(����)��ʧ�ĸ���ϵ��
			Kr		= vtof( m_pRsCongeal->GetCollect(_variant_t("c_Heat_Loss_Data")) );
			//������ܵ�����Һ��ͣ��ʱ��(Сʱ)
			taofr	= vtof( m_pRsCongeal->GetCollect(_variant_t("c_Stop_Time")) );
			taofr *= 3600;
			//�����ڹ��ڶ����¶�
			tfr		= vtof( m_pRsCongeal->GetCollect(_variant_t("c_Medium_Congeal_Temp")) );
			//�����ܶ�
			ro		= vtof( m_pRsCongeal->GetCollect(_variant_t("c_Medium_Density")) );
			//���ʱ���
			C		= vtof( m_pRsCongeal->GetCollect(_variant_t("c_Medium_Heat")) );
			//�ܲ��ܶ�
			rop		= vtof( m_pRsCongeal->GetCollect(_variant_t("c_Pipe_Density")) );
			//�ܲı���
			Cp		= vtof( m_pRsCongeal->GetCollect(_variant_t("c_Pipe_Heat")) );
			//�����ڽ���
			Hfr		= vtof( m_pRsCongeal->GetCollect(_variant_t("c_Medium_Melt_Heat")) );
			//����
			dFlux	= vtof( m_pRsCongeal->GetCollect(_variant_t("c_Flux")) );

			//ÿ�׹ܳ����������m3/m;	
			V	=  pi / 4 * pow(D0 - 2 * pi_thi, 2) / 1000000;
			//ÿ�׹ܳ��ܱ����,m3/m;
//			Vp	= pi / 4 * (D0 * D0 - pow(D0 - 2 * pi_thi, 2)) / 1000000;			
			Vp	= pi * pow(D0/2.0,2) / 1000000 - V;
		}
	}
	catch (_com_error)
	{
		return FALSE;
	}

	return TRUE;
}
