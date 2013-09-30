// CalcThick_SubterraneanObstruct.cpp: implementation of the CCalcThick_SubterraneanObstruct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "CalcThick_SubterraneanObstruct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcThick_SubterraneanObstruct::CCalcThick_SubterraneanObstruct()
{

}

CCalcThick_SubterraneanObstruct::~CCalcThick_SubterraneanObstruct()
{

}

//------------------------------------------------------------------                
// Author       : ZSY
// Parameter(s) : 
// Return       : 
// Remark       : ������عܵ���˫�㲻ͨ��)���²��
//------------------------------------------------------------------
short CCalcThick_SubterraneanObstruct::CalcPipe_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	double MaxTb = 350;
	double dDiff1;
	double dDiff2;
	double dMinDiff1 = 0;
	double dMinDiff2 = 0;
	double dCur_Q;
	
	double Q;
	
	bool   flg;

	thick1_resu=0;
	thick2_resu=0;
	ts_resu=0;
	tb_resu=0;

	//����ý����¶���������ɢ����
	GetHeatLoss( m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax );

	// ���ԭʼ����
	GetSubterraneanOriginalData( m_nID );
	
	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����thicknessRegular���
	for(thick1=m_thick1Start;thick1<=m_thick1Max;thick1+=m_thick1Increment)
	{
		for(thick2=m_thick2Start;thick2<=m_thick2Max;thick2+=m_thick2Increment)
		{
			CalcPipe_Com_TsTb(thick1, thick2, ts, tb);
			
			//�ڱ��²��⾶
			D1=2*thick1+D0;  
			//�Ᵽ�²��⾶
			D2=2*thick2+D1;
			//���ݲ�ͬ����ҵ�淶�������ϵ��
			ALPHA = GetPipeAlpha(ts,D2);

			//���ݸı��������¶Ⱥͽ����¶ȼ��㵼��ϵ��  
			lamda1 = GetLamda1( t, tb ); 
			lamda2 = GetLamda2( tb, ts); 

			Calc_Q_PipeCom(Q); 
			//ȷ���ڱ��º������ 
			dDiff1 = log(D1 / D0) - 2000 * lamda1 * fabs(t-tb) / Q / D2; 
			//�Ᵽ�º�  
			//lamda1 = GetLamda1()

			dDiff2 = D2 * log(D2 / D0) - 2000 * ( (lamda1 * fabs(t-tb) + lamda2 * fabs(tb-ta)) / Q - lamda2 / ALPHA );

			dDiff1 = fabs( dDiff1 );
			dDiff2 = fabs( dDiff2 );
			ALPHA  = GetPipeAlpha( ts, D2 );
			
			if ( ts<MaxTs && tb<MaxTb )
			{
				if ((fabs(dMinDiff1) < DZero && fabs(dMinDiff2) < DZero) || (dDiff1 < dMinDiff1 && dDiff2 < dMinDiff2))
				{
					flg = true;
					if ( bIsHeatLoss )
					{
						//���㵱ǰ����µ�ɢ���ܶ�
						Calc_Q_PipeCom(dCur_Q);
						if ( fabs( QMax ) <= DZero || dCur_Q <= QMax )
							flg = true;
						else
							flg = false;
					}
					if ( flg )
					{
						dMinDiff1 = dDiff1;			//������С�Ĳ�ֵ
						dMinDiff2 = fabs( dDiff2 );	// 
						
						thick1_resu = thick1;			//�ڱ��º�
						thick2_resu = thick2;
						ts_resu = ts;					// ������¶�
						tb_resu = tb;					//�����¶�
						//���㵱ǰ����µ�ɢ���ܶ�
						this->Calc_Q_PipeCom(dQ);
					}
				}
			}
		}
	}       

	return 1;
}

//------------------------------------------------------------------
// Author       : ZSY
// Parameter(s) : thick_resu[out] ���²���
//				: ts_resu[out] ������¶�
// Return       : ����ɹ�����TRUE�����򷵻�FALSE
// Remark       : ��عܵ�(��ͨ��) ����
//------------------------------------------------------------------
short CCalcThick_SubterraneanObstruct::CalcPipe_One(double &thick_resu, double &ts_resu)
{
	double	ts=0.0;			//������¶�ֵ
	double	dYearVal;		//�����й��������ɢ���ܶ�
	double	dSeasonVal;		//�������й������ɢ���ܶ�
	double	Q = 1;				//��ǰ����µ�ɢ���ܶ�.
	double	QMax = 0;			//���ݵ�ǰ��¼�Ĺ���;�������ɢ���ܶ�
	double	dLSo;	// �����ĵ���ϵ��
	double	dASo;	//
	double  dDiff;
	double  dMinDiff = 0;
	bool	flg=true;		//����ɢ���ܶȵ��ж�

	thick_resu=0;
	ts_resu=0;
	GetSubterraneanOriginalData(m_nID);
	GetHeatLoss(m_pRsHeatLoss, Tf1, dYearVal, dSeasonVal,QMax);
	if (fabs(QMax) >= DZero)
	{
		Q = QMax * m_MaxHeatDensityRatio;			//���½ṹ���������ɢ���ܶȣ������в�����������ɢ���ܶȵĳ���һ��ϵ��ȡֵ.(��̹涨Ϊ90%).
	}

	//thick���㱣�º�ȡ�
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����ThicknessRegular���
	for (thick=m_thick2Start; thick <= m_thick2Max; thick += m_thick2Increment)
	{
		//����ָ���ı��º񣬼���������¶�
		CalcPipe_One_TsTemp(thick,ts);
		D1    = D0 + 2 * thick;
		
		dLSo  = GetLamdaSo(ts);		// �����ĵ���ϵ��
		lamda = GetLamda(t, ts);	// ���²�ĵ���ϵ��
		CalcPipe_Alpha(D0, dASo);
		dDiff = log(D1 / D0) - 2.0 * pi * lamda * ((Tf1 - Tso) / Q - (1 / (A1pre * pi * D1) + 1 / (Aaw * pi * Dag) + (log(4 * sub_h / Dag) / (2 * pi * dLSo))));
		if (ts<MaxTs && (dMinDiff <= DZero || dDiff < dMinDiff))
		{
			flg = true;
			//ѡ����ѡ����ж�ɢ���ܶȣ��ͽ��бȽ�
			if(bIsHeatLoss)
			{	
				//��ñ��κ�ȵ�ɢ���ܶ�
				Calc_Q_PipeOne(Q);
				
				if(fabs(QMax) <= DZero || Q <= QMax)
				{
					flg = true; // ��ǰ��������������ɢ�ȹ���
				}else
				{
					flg = false;
				}
			}
			//�������� 
			if( flg ) 
			{
				dMinDiff = dDiff;
				thick_resu = thick;
				ts_resu = ts;	
				//���㵱ǰ����µ�ɢ���ܶ� 
				Calc_Q_PipeOne(dQ);
			}
		}
	}
	return TRUE;
}

//------------------------------------------------------------------
// Author       : ZSY
// Parameter(s) : dQ1[out]:��һ��������ʧ����λ[Kcal/(m*h)]
//				: dQ2[out]:�ڶ���������ʧ����λ[Kcal/(m*h)]
// Return       : void
// Remark       : ���㲻ͨ�еع��з�������(���)ʱ������ʧ
//------------------------------------------------------------------
void CCalcThick_SubterraneanObstruct::Calc_Q_PipeTwo(double &dQ1, double &dQ2)
{
	double dTaw;
	Calc_Taw_Two(dTaw);
	dQ1 = (Tf1 - dTaw) / (R11 + Rs1);
	dQ2 = (Tf2 - dTaw) / (R12 + Rs2);
	
}
//------------------------------------------------------------------
// Author       : ZSY
// Parameter(s) : dQ[out]: ����������ʧ����λ [Kcal/(m*h)]
// Return       : short 1
// Remark       : ���㲻ͨ�еع��з��赥��������ʧ
//------------------------------------------------------------------
short CCalcThick_SubterraneanObstruct::Calc_Q_PipeOne(double &dOneQ)
{
	double dRso; 
	
//	double dQ1;
//	double dQ2;

	Calc_Rso(dRso);
	dOneQ = (Tf1 - Tso) / (R1 + Rs1 + Raw + dRso);
	return 1; 
}
