// CalcThick_Economy.cpp: implementation of the CCalcThick_Economy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "CalcThick_Economy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcThick_Economy::CCalcThick_Economy()
{

}

CCalcThick_Economy::~CCalcThick_Economy()
{

}
//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :ʹ�þ��ú�ȷ�,���㱣�²�ĺ�ȣ�ƽ��˫�㡣
//------------------------------------------------------------------
short CCalcThick_Economy::CalcPlain_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	double	ts=0.0,			//������¶�ֵ
			tb=0.0,			//���洦�¶�
			nYearVal,		//�����й��������ɢ���ܶ�
			nSeasonVal,		//�������й������ɢ���ܶ�
			Q,				//��ǰ����µ�ɢ���ܶ�.
			QMax;			//���ݵ�ǰ��¼�Ĺ���,�������ɢ���ܶ�	
//			MaxTb = in_tmax*m_HotRatio;	//�������������¶�=�Ᵽ�²��ϵ�����¶� * һ��ϵ��(��̹涨Ϊ0.9).
	double  MaxTb = in_tmax;//�������������¶�,�ӱ��²��ϲ�����(a_mat)��ȡMAT_TMAX�ֶ�,���޸ĸ��ֶν��п��ƽ����¶�ֵ.

	double	cost_q,			//??����
			cost_s,
			cost_tot,
			cost_min;		//��С�ķ���.

	bool	flg=true;		//����ɢ���ܶȵ��ж�

	thick1_resu=0;
	thick2_resu=0;
	ts_resu=0;
	tb_resu=0;

	cost_min=0;
	//ѡ����ѡ�����ж�ɢ���ܶ�,����
	if(bIsHeatLoss)
	{
		//����ý����¶���������ɢ���ܶ�
		GetHeatLoss(m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax);
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
			//����ϵ��
			//���ݲ�ͬ����ҵ�淶�������ϵ��
			ALPHA = GetPlainAlpha(ts);

			//���ݸı��������¶Ⱥͽ����¶ȼ��㵼��ϵ��
			lamda1 = GetLamda1(t, tb);
			lamda2 = GetLamda2( tb, ts);

			//���½ṹ��ɢ����ʧ�����  			��ʽ��DL/T 5072��1997���������糧����������ƹ�̡���93
			cost_q=3.6*Mhours*heat_price*Yong*fabs(t-ta)*1e-6
				/(thick1/1000.0/lamda1+thick2/1000.0/lamda2+1.0/ALPHA);
			//���½ṹͶ�����̯���á�
			cost_s=(thick1/1000.0*in_price+thick2/1000.0*out_price+(gbWithoutProtectionCost?0:pro_price))*S;
			//���¶������ܷ���
			cost_tot=cost_q+cost_s;

			if((ts<MaxTs && tb<MaxTb))	//ts<50 && tb<350
			{
				if(fabs(cost_min)<DZero/*��һ��*/ || cost_min>cost_tot)
				{
					flg = true;
					//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.
					if(bIsHeatLoss)
					{
						//��ñ��κ�ȵ�ɢ���ܶ�
						//��ʽ:
						//(�����¶�-�����¶�)/(�ڱ��º�/1000/�ڱ��²��ϵ����� + �Ᵽ�º�/1000/�Ᵽ�²��ϵ����� + 1/����ϵ��)
						Calc_Q_PlainCom(Q);
						//	Q = (t-ta)/(thick1/1000.0/lamda1+thick2/1000.0/lamda2+1.0/ALPHA);
						
						if(fabs(QMax) <= DZero || Q <= QMax)
						{	//��ǰ��������������ɢ�ȹ���.
							flg = true;
						}
						else
						{	
							flg = false;
						}
					}
					//��������.
					if( flg )
					{
						cost_min=cost_tot;
						thick1_resu=thick1;
						thick2_resu=thick2;
						ts_resu=ts;
						tb_resu=tb;

						//���㵱ǰ����µ�ɢ���ܶ�
						this->Calc_Q_PlainCom(dQ);
					}
				}
			}
		}//end thick2
	}//end thick1
	return 1;
}

//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :ʹ�þ��ú�ȷ�,���㱣�²�ĺ�ȣ�ƽ�浥�㡣
//------------------------------------------------------------------
short CCalcThick_Economy::CalcPlain_One(double &thick_resu, double &ts_resu)
{
	double	ts=0.0,			//������¶�ֵ
			nYearVal,		//�����й��������ɢ���ܶ�
			nSeasonVal,		//�������й������ɢ���ܶ�
			Q,				//��ǰ����µ�ɢ���ܶ�.
			QMax;			//���ݵ�ǰ��¼�Ĺ���,�������ɢ���ܶ�

	double	cost_q,			//??����
			cost_s,
			cost_tot,
			cost_min;		//��С�ķ���

	bool	flg=true;		//����ɢ���ܶȵ��ж�

	thick_resu=0;
	ts_resu=0;
	cost_min=0;
	//ѡ����ѡ�����ж�ɢ���ܶ�,����
	if(bIsHeatLoss)
	{
		//����ý����¶���������ɢ���ܶ�
		GetHeatLoss(m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax);
	} 
	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	for (thick=m_thick2Start; thick<=m_thick2Max; thick+=m_thick2Increment)
	{
		//����ָ���ı��º񣬼���������¶ȡ�
		CalcPlain_One_TsTemp(thick,ts);
		//����ϵ��
		lamda = GetLamda(t, ts);

		// ���ݲ�ͬ����ҵ�淶�������ϵ��
		ALPHA = GetPlainAlpha(ts);
	
		//���½ṹ��ɢ����ʧ�����  			��ʽ��DL/T 5072��1997���������糧����������ƹ�̡���93
		cost_q=3.6*Mhours*heat_price*Yong*fabs(t-ta)*1e-6
				/((thick/1000.0/lamda)+1.0/ALPHA);
		//���½ṹͶ�����̯����
		cost_s=(thick/1000*in_price+(gbWithoutProtectionCost?0:pro_price))*S;
		//���¶������ܷ���
		cost_tot=cost_q+cost_s;	

		if( ts<MaxTs && (cost_tot<cost_min || fabs(cost_min)<DZero) )
		{
			flg = true;
			//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.
			if(bIsHeatLoss)
			{
				//��ñ��κ�ȵ�ɢ���ܶ�
				//��ʽ:	
				//(�����¶�-�����¶�)/(��ǰ���º�/(1000*���ϵ�����) + 1/����ϵ��)
				Calc_Q_PlainOne(Q);
				//	Q = (t-ta) / (thick / (1000*lamda) + 1.0 / ALPHA); 
				if(fabs(QMax) <= DZero || Q <= QMax)
				{	//��ǰ��������������ɢ�ȹ���.
					flg = true;
				}
				else
				{	
					flg = false;
				}
			}
			//��������.
			if( flg )
			{
				cost_min=cost_tot;
				thick_resu=thick;
				ts_resu=ts;
				//���㵱ǰ����µ�ɢ���ܶ�
				Calc_Q_PlainOne(dQ);
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
// Remark       :ʹ�þ��ú�ȷ�,���㱣�²�ĺ�ȣ����¶���Ϊ�ܵ���˫��
//------------------------------------------------------------------
short CCalcThick_Economy::CalcPipe_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	double	ts=0.0;			//������¶�ֵ
	double	tb=0.0;			//���洦�¶�
	double	nYearVal;		//�����й��������ɢ���ܶ�
	double	nSeasonVal;		//�������й������ɢ���ܶ�
	double	Q;				//��ǰ����µ�ɢ���ܶ�.
	double	QMax;			//���ݵ�ǰ��¼�Ĺ���,�������ɢ���ܶ�	
//	double	MaxTb = in_tmax*m_HotRatio;	//�������������¶�=�Ᵽ�²��ϵ�����¶� * һ��ϵ��(��̹涨Ϊ0.9).
	double  MaxTb = in_tmax;//�������������¶�,�ӱ��²��ϲ�����(a_mat)��ȡMAT_TMAX�ֶ�,���޸ĸ��ֶν��п��ƽ����¶�ֵ.

	double	cost_q;			//??����
	double	cost_s;
	double	cost_tot;
	double	cost_min;		//��С�ķ���.

	bool	flg=true;		//����ɢ���ܶȵ��ж�

	thick1_resu=0;
	thick2_resu=0;
	ts_resu=0;
	tb_resu=0;

	cost_min=0;
	//ѡ����ѡ�����ж�ɢ���ܶ�,����
	if(bIsHeatLoss)
	{
		//����ý����¶���������ɢ���ܶ�
		if( !GetHeatLoss(m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax) )
		{ 		
		}
	}

	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����thicknessRegular���
	//
	for(thick1=m_thick1Start;thick1<=m_thick1Max;thick1+=m_thick1Increment)
	{
		for(thick2=m_thick2Start;thick2<=m_thick2Max;thick2+=m_thick2Increment)
		{
			//���㵱ǰ��Ƚ����¶Ⱥ�������¶�
			CalcPipe_Com_TsTb(thick1,thick2,ts,tb);

			//�ڱ��²��⾶���Ᵽ�²��⾶
			D1=2*thick1+D0;
			D2=2*thick2+D1;	

			//���ݲ�ͬ����ҵ�淶�������ϵ��
			ALPHA =	GetPipeAlpha(ts,D2);

			//���ݸı��������¶Ⱥͽ����¶ȼ��㵼��ϵ��
			lamda1 = GetLamda1(t, tb);
			lamda2 = GetLamda2( tb, ts);
			//���½ṹ��ɢ����ʧ�����  			��ʽ��DL/T 5072��1997���������糧����������ƹ�̡���93
			cost_q=7.2*pi*Mhours*heat_price*Yong*fabs(t-ta)*1e-6
					/(1.0/lamda1*log(D1/D0)+1.0/lamda2*log(D2/D1)+2000.0/ALPHA/D2);
			//���½ṹͶ�����̯����
			cost_s=(pi/4.0*(D1*D1-D0*D0)*in_price*1e-6+pi/4.0*(D2*D2-D1*D1)*out_price*1e-6
					+(gbWithoutProtectionCost?0:pi*D2*pro_price*1e-3))*S;
			//���¶������ܷ���
			cost_tot=cost_q+cost_s;
			//tb<350)
			if((ts<MaxTs && tb<MaxTb) && (fabs(cost_min)<DZero || cost_min>cost_tot))
			{
				flg = true;
				//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.
				if(bIsHeatLoss)
				{
					//��ñ��κ�ȵ�ɢ���ܶ�
					//��ʽ:
					//(�����¶�-�����¶�)/((�Ᵽ�²��⾶/2000.0)*(1.0/�ڱ��²��ϵ�����*ln(�ڱ��²��⾶/�ܵ��⾶) + 1.0/�Ᵽ�²��ϵ�����*ln(�Ᵽ�²��⾶/�ڱ��²��⾶) ) + 1/����ϵ��)
					Calc_Q_PipeCom(Q);
					//	Q = (t-ta) / ( (D2/2000.0) * (1.0/lamda1*log(D1/D0)+1.0/lamda2*log(D2/D1)) + 1.0/ALPHA );

					if(fabs(QMax) <= DZero || Q <= QMax)
					{	//��ǰ��������������ɢ�ȹ���.
						flg = true;
					}
					else
					{	
						flg = false;
					}
				}
				//��������.
				if( flg )
				{					
					cost_min=cost_tot;
					thick1_resu=thick1;
					thick2_resu=thick2;
					ts_resu=ts;
					tb_resu=tb;//

					//���㵱ǰ����µ�ɢ���ܶ�
					Calc_Q_PipeCom(dQ);
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
// Remark       :ʹ�þ��ú�ȷ�,���㱣�²�ĺ�ȣ����¶���Ϊ�ܵ�������
//------------------------------------------------------------------
short CCalcThick_Economy::CalcPipe_One(double &thick_resu, double &ts_resu)
{
	double	ts=0.0;			//������¶�ֵ
	double	nYearVal;		//�����й��������ɢ���ܶ�
	double	nSeasonVal;		//�������й������ɢ���ܶ�
	double	Q;				//��ǰ����µ�ɢ���ܶ�.
	double	QMax;			//���ݵ�ǰ��¼�Ĺ���;�������ɢ���ܶ�

	double	cost_q;			//ɢ�ȷ���
	double	cost_s;			//���Ϸ���
	double	cost_tot;	    //�ܷ���
	double	cost_min;		//��С�ķ���.

	bool	flg=false;		//����ı��º������Ҫ�󣬳�ʼΪ������

	thick_resu=0;
	ts_resu=0;
	cost_min=0;
	//ѡ����ѡ�����ж�ɢ���ܶ�,����
	if(bIsHeatLoss)
	{
		//����ý����¶���������ɢ���ܶ�
		if( !GetHeatLoss(m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax) )
		{ 		
		}
	}
	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	for (thick=m_thick2Start; thick<=m_thick2Max; thick+=m_thick2Increment)
	{
		//����ָ���ı��º񣬼���������¶ȡ�
		CalcPipe_One_TsTemp(thick,ts);

		D1=2.0*thick+D0;
		//����ϵ��
		lamda = GetLamda(t, ts);
		//���ݲ�ͬ����ҵ�淶�������ϵ��
		ALPHA = GetPipeAlpha(ts,D1);

		//���½ṹ��ɢ����ʧ�����  			��ʽ��DL/T 5072��1997���������糧����������ƹ�̡�P93
		cost_q=7.2*pi*Mhours*heat_price*Yong*fabs(t-ta)*1e-6
				/(1.0/lamda*log(D1/D0)+2000.0/ALPHA/D1);
		//���½ṹͶ�����̯����
		cost_s=(pi/4.0*(D1*D1-D0*D0)*in_price*1e-6+(gbWithoutProtectionCost?0:pi*D1*pro_price*1e-3))*S;
		//���¶������ܷ���
		cost_tot=cost_q+cost_s;
		//��ñ��κ�ȵ�ɢ���ܶ�
		Calc_Q_PipeOne(Q);
		
		if(ts<MaxTs && (cost_tot<cost_min || fabs(cost_min)<DZero))
		{
			flg = true;
			//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.


			if(bIsHeatLoss)
			{
				//��ñ��κ�ȵ�ɢ���ܶ�
				//��ʽ:
				//(�����¶�-�����¶�)/((���²��⾶/2000.0/���ϵ�����*ln(���²��⾶/�ܵ��⾶)) + 1/����ϵ��)
				Calc_Q_PipeOne(Q);
				//	Q = (t-ta)/(D1/2000.0/lamda*log(D1/D0) + 1.0/ALPHA);

				if(fabs(QMax) <= DZero || Q <= QMax)

					flg = true; // ��ǰ��������������ɢ�ȹ���.

				else
					flg = false;
			}
			//��������
			if( flg )
			{

				cost_min = cost_tot;
				thick_resu = thick;
				ts_resu = ts;
				//���㵱ǰ����µ�ɢ���ܶ�
				Calc_Q_PipeOne(dQ);
			}
		}
	}

	return 1;	
}
