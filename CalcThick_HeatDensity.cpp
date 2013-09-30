// CalcThick_HeatDensity.cpp: implementation of the CCalcThick_HeatDensity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CalcThick_HeatDensity.h"
#include "EDIBgbl.h"
#include "AutoIPED.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcThick_HeatDensity::CCalcThick_HeatDensity()
{

}

CCalcThick_HeatDensity::~CCalcThick_HeatDensity()
{

}


//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :ʹ������ɢ���ܶȷ�,���㱣�²�ĺ�ȣ�ƽ��˫�㡣
//------------------------------------------------------------------
short CCalcThick_HeatDensity::CalcPlain_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	double	ts=0.0;			//������¶�ֵ
	double	tb=0.0;			//���洦�¶�
	double	nYearVal;		//�����й��������ɢ���ܶ�
	double	nSeasonVal;		//�������й������ɢ���ܶ�
	double	Q;				//��ǰ����µ�ɢ���ܶ�.
	double	QMax;			//���ݵ�ǰ��¼�Ĺ���,�������ɢ���ܶ�	
//	double	MaxTb = in_tmax*m_HotRatio;	//�������������¶�=�Ᵽ�²��ϵ�����¶� * һ��ϵ��(��̹涨Ϊ0.9).
	double  MaxTb = in_tmax;//�������������¶�,�ӱ��²��ϲ�����(a_mat)��ȡMAT_TMAX�ֶ�,���޸ĸ��ֶν��п��ƽ����¶�ֵ.

	thick1_resu=0;
	thick2_resu=0;
	ts_resu=0;
	tb_resu=0;

	//����ý����¶���������ɢ���ܶ�
	if( !GetHeatLoss(m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax) )
	{ 		
	}
	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����thicknessRegular���
	for(thick1=m_thick1Start;thick1<=m_thick1Max;thick1+=m_thick1Increment)
	{
		for(thick2=m_thick2Start;thick2<=m_thick2Max;thick2+=m_thick2Increment)
		{
			CalcPlain_Com_TsTb(thick1,thick2,ts,tb);

			//���ݲ�ͬ����ҵ�淶�������ϵ��
			ALPHA = GetPlainAlpha(ts);

			//���ݸı��������¶Ⱥͽ����¶ȼ��㵼��ϵ��
			lamda1 = GetLamda1( t, tb );
			lamda2 = GetLamda2( tb, ts);

			if( ts<MaxTs && tb<MaxTb )	//ts<50 && tb<350
			{
				//��ñ��κ�ȵ�ɢ���ܶ�
				//��ʽ:
				//(�����¶�-�����¶�)/(�ڱ��º�/1000/�ڱ��²��ϵ����� + �Ᵽ�º�/1000/�Ᵽ�²��ϵ����� + 1/����ϵ��)
				Calc_Q_PlainCom(Q);
				//	Q = (t-ta)/(thick1/1000.0/lamda1+thick2/1000.0/lamda2+1.0/ALPHA);
				
				if(fabs(QMax) <= DZero || Q <= QMax)
				{	//��ǰ��������������ɢ�ȹ���.
					thick1_resu=thick1;
					thick2_resu=thick2;
					ts_resu=ts;
					tb_resu=tb;
					//���㵱ǰ����µ�ɢ���ܶ�
					this->Calc_Q_PlainCom(dQ);

					return 1;
				}
			}
		}
	}
	return 1;
}

//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :ʹ������ɢ���ܶȷ�,���㱣�²�ĺ�ȣ�ƽ�浥�㡣
//------------------------------------------------------------------
short CCalcThick_HeatDensity::CalcPlain_One(double &thick_resu, double &ts_resu)
{
	double	ts=0.0;			//������¶�ֵ
	double	nYearVal;		//�����й��������ɢ���ܶ�
	double	nSeasonVal;		//�������й������ɢ���ܶ�
	double	Q;				//��ǰ����µ�ɢ���ܶ�.
	double	QMax;			//���ݵ�ǰ��¼�Ĺ���,�������ɢ���ܶ�	

	thick_resu=0;
	ts_resu=0;
	//����ý����¶���������ɢ���ܶ�
	if( !GetHeatLoss(m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax) )
	{ 		
	}

	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	for (thick=m_thick2Start; thick<=m_thick2Max; thick+=m_thick2Increment)
	{
		//����ָ���ı��º񣬼���������¶ȡ�
		CalcPlain_One_TsTemp(thick,ts);
		//����ϵ��			
		lamda = GetLamda( t, ts );

		//���ݲ�ͬ����ҵ�淶�������ϵ��
		ALPHA = GetPlainAlpha(ts);
		
		if(ts<MaxTs)
		{
			//��ñ��κ�ȵ�ɢ���ܶ�
			//��ʽ:	
			//(�����¶�-�����¶�)/(��ǰ���º�/(1000*���ϵ�����) + 1/����ϵ��)
			Calc_Q_PlainOne(Q);
			//	Q = (t-ta) / (thick / (1000*lamda) + 1.0 / ALPHA); 
			if(fabs(QMax) <= DZero || Q <= QMax)
			{
				//��ǰ��������������ɢ�ȹ���. 
				thick_resu=thick;
				ts_resu=ts;
				//���㵱ǰ����µ�ɢ���ܶ�
				this->Calc_Q_PlainOne(dQ);

				return 1;
			}
		}
	}
	return 1;
}	

//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :ʹ������ɢ���ܶȷ�,���㱣�²�ĺ�ȣ����¶���Ϊ�ܵ���˫��
//------------------------------------------------------------------
short CCalcThick_HeatDensity::CalcPipe_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	double	ts=0.0;			//������¶�ֵ
	double	tb=0.0;			//���洦�¶�
	double	nYearVal;		//�����й��������ɢ���ܶ�
	double	nSeasonVal;		//�������й������ɢ���ܶ�

	double  dCur_Q;			//��ǰ����µ�ɢ���ܶ�.
	double	Q;				//�������ɢ���ܶ���һ����ֵ�ĳ˻�����Ϊ���㱣�º��ʱ��ɢ���ܶ�.
	double	QMax;			//���ݵ�ǰ��¼�Ĺ���,�������ɢ���ܶ�

	bool	flg=true;		//����ɢ���ܶȵ��ж�

	double  dDiff1;			//ȷ���ڱ��º�Ĳ�ֵ
	double  dDiff2;
	double  dMinDiff1=0;	//��С�Ĳ�ֵ
	double  dMinDiff2=0;

//	double	MaxTb = in_tmax*m_HotRatio;	//�������������¶�=�Ᵽ�²��ϵ�����¶� * һ��ϵ��(��̹涨Ϊ0.9).
	double  MaxTb = in_tmax;//�������������¶�,�ӱ��²��ϲ�����(a_mat)��ȡMAT_TMAX�ֶ�,���޸ĸ��ֶν��п��ƽ����¶�ֵ.

	thick1_resu=0;
	thick2_resu=0;
	ts_resu=0;
	tb_resu=0;

	//����ý����¶���������ɢ���ܶ�
	if( !GetHeatLoss( m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax ) )
	{
	}

	Q = QMax * m_MaxHeatDensityRatio;			//���½ṹ���������ɢ���ܶȣ������в�����������ɢ���ܶȵĳ���һ��ϵ��ȡֵ.(��̹涨Ϊ90%).

	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����thicknessRegular���
	//
	for(thick1=m_thick1Start;thick1<=m_thick1Max;thick1+=m_thick1Increment)
	{
		for(thick2=m_thick2Start;thick2<=m_thick2Max;thick2+=m_thick2Increment)
		{
			CalcPipe_Com_TsTb(thick1,thick2,ts,tb);

			//�ڱ��²��⾶
			D1=2*thick1+D0;
			//�Ᵽ�²��⾶
			D2=2*thick2+D1;
			
			//���ݲ�ͬ����ҵ�淶�������ϵ��
			ALPHA = GetPipeAlpha(ts,D2);

			//���ݸı��������¶Ⱥͽ����¶ȼ��㵼��ϵ��
			lamda1 = GetLamda1( t, tb );
			lamda2 = GetLamda2( tb, ts);
			
			//ȷ���ڱ��º������.
			dDiff1 = log(D1 / D0) - 2000 * lamda1 * fabs(t-tb) / Q / D2;
			//�Ᵽ�º�
			dDiff2 = D2 * log(D2 / D0) - 2000 * ((lamda1 * fabs(t-tb) + lamda2 * fabs(tb-ta)) / Q - lamda2 / ALPHA);

			//  [2005/11/25]
			dDiff1 = fabs( dDiff1 );	
			dDiff2 = fabs( dDiff2 );
			//tb<350)
			if(	(ts<MaxTs && tb<MaxTb) && 
				((fabs(dMinDiff1) < DZero && fabs(dMinDiff2) < DZero) || (dDiff1 < dMinDiff1 && dDiff2 < dMinDiff2)))
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
					dMinDiff2 = dDiff2;
		
 					thick1_resu=thick1;			//�ڱ��º�
					thick2_resu=thick2;
					ts_resu=ts;
					tb_resu=tb;					//�����¶� 

					//���㵱ǰ����µ�ɢ���ܶ�
					this->Calc_Q_PipeCom(dQ);
				}
			}
		}
	}                     
	return 1;
}

 
//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :ʹ������ɢ���ܶȷ�,���㱣�²�ĺ�ȣ����¶���Ϊ�ܵ�������
//------------------------------------------------------------------
short CCalcThick_HeatDensity::CalcPipe_One(double &thick_resu, double &ts_resu)
{
	double	ts=0.0;			//������¶�ֵ
	double	nYearVal;		//�����й��������ɢ���ܶ�
	double	nSeasonVal;		//�������й������ɢ���ܶ�
	double  dCur_Q;			//��ǰ����µ�ɢ���ܶ�.
	double	Q;				//�������ɢ���ܶȵ���һ����ֵ�ĳ˻�����Ϊ���㱣�º��ʱ��ɢ���ܶ�.
	double	QMax;			//���ݵ�ǰ��¼�Ĺ���,�������ɢ���ܶ�

	double	dMinDiff=0;
	double	dDiff;			//��ֵ,

	bool	flg=true;		//����ɢ���ܶȵ��ж�

	thick_resu=0;
	ts_resu=0;

	//����ý����¶���������ɢ���ܶ�
	if( !GetHeatLoss(m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax) )
	{
	}
	Q = QMax * m_MaxHeatDensityRatio;			//���½ṹ���������ɢ���ܶȣ������в�����������ɢ���ܶȵĳ���һ��ϵ��ȡֵ.(��̹涨Ϊ90%).

	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	for (thick=m_thick2Start; thick<=m_thick2Max; thick+=m_thick2Increment)
	{
		//����ָ���ı��º񣬼���������¶ȡ�
		CalcPipe_One_TsTemp(thick,ts);
		//����ϵ��
		lamda = GetLamda(t, ts);

		//���²��⾶
		D1=2.0*thick+D0;
		//���ݲ�ͬ����ҵ�淶�������ϵ��
		ALPHA = GetPipeAlpha(ts, D1);

		//ȷ�����º�������������ֵ
		dDiff = D1 * log(D1 / D0) - 2000 * lamda * (fabs(t - ta) / Q - 1 / ALPHA);
		dDiff = fabs( dDiff );
		
		if(ts<MaxTs && (fabs(dMinDiff) < DZero || fabs(dDiff) < fabs(dMinDiff)) )
		{
			flg = true;
			if ( bIsHeatLoss )
			{
				//���㵱ǰ����µ�ɢ���ܶ�
				this->Calc_Q_PipeOne( dCur_Q );
				if ( fabs(QMax) <= DZero || dCur_Q <= QMax )
					flg = true;
				else
					flg = false;
			}
			if ( flg )
			{
 				dMinDiff = dDiff;
				thick_resu=thick;
				ts_resu=ts;
				//���㵱ǰ����µ�ɢ���ܶ�
				this->Calc_Q_PipeOne(dQ);
			}
		}
	}

	return 1;	
}
