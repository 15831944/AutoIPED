// CalcThick_FaceTemp.cpp: implementation of the CCalcThick_FaceTemp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "CalcThick_FaceTemp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcThick_FaceTemp::CCalcThick_FaceTemp()
{
}

CCalcThick_FaceTemp::~CCalcThick_FaceTemp()
{

}


//------------------------------------------------------------------                
// DATE         :[2005/04/25]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :������¶ȷ����㱣�º�ȣ�ƽ�浥�㡣
//------------------------------------------------------------------
short CCalcThick_FaceTemp::CalcPlain_One(double &thick_resu, double &ts_resu)
{
	double	ts=ts_resu;		//������¶�ֵ


	//���ݸı��������¶ȼ��㵼��ϵ��
	lamda = GetLamda( t, ts );
	//���ݲ�ͬ����ҵ�淶�������ϵ��
	ALPHA =	GetPlainAlpha(ts);

	//���Ϊ�����¶ȷ����㱣�²���ʱ,������¶Ⱥͽ����¶�Ϊ��֪��.��������������.
	thick_resu = 1000*lamda*(t-ts)/ALPHA/(ts-ta);
	
	// zsy ���º�ȵ�ֵΪһ������ [10/26/2005]
	thick_resu = 10 * (int)((thick_resu + INT_ADD) / 10);
	
	if ( thick_resu < 0 )
	{
		CString strMsg;
		strMsg.Format("ԭʼ�������Ϊ%d�ļ�¼, ������ı��²���Ϊ��������ȷ��ԭʼ�����Ƿ���ȷ?", m_nID);
		if ( -1 == strExceptionInfo.FindOneOf(strMsg) )
		{
			strExceptionInfo += strMsg;
		}
	}
	//���㵱ǰ����µ�ɢ���ܶ�
	thick = thick_resu;
	this->Calc_Q_PlainOne(dQ);

	return	1;

}	
//------------------------------------------------------------------                
// DATE         :[2005/04/25]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�ñ����¶ȷ����㱣�º�ȣ�ƽ��˫�㡣
//------------------------------------------------------------------
short CCalcThick_FaceTemp::CalcPlain_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	double	ts=ts_resu;		//����������������������¶ȡ�
	double	tb;				//�����¶ȡ�

//	double	MaxTb = in_tmax*m_HotRatio;//�������������¶�=�Ᵽ�²��ϵ�����¶� * һ��ϵ��(��̹涨Ϊ0.9).
	double  MaxTb = in_tmax;//�������������¶�,�ӱ��²��ϲ�����(a_mat)��ȡMAT_TMAX�ֶ�,���޸ĸ��ֶν��п��ƽ����¶�ֵ.

	double  dCur_Q;			//��ǰ����µ�ɢ���ܶ�.
	double	QMax;			//���ݵ�ǰ��¼�Ĺ���,�������ɢ���ܶ�
	bool	flg = true;		//����ɢ���ܶȵ��ж�

	//����ý����¶���������ɢ���ܶ�
	if( !GetHeatLoss( m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax ) )
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
			this->CalcPlain_Com_TsTb(thick1,thick2,ts,tb,1);

			//���ݲ�ͬ����ҵ�淶�������ϵ��
			ALPHA =	GetPlainAlpha(ts);

			//���ݸı��������¶Ⱥͽ����¶ȼ��㵼��ϵ��
			lamda1 = GetLamda1(t, tb);
			lamda2 = GetLamda2( tb, ts);
			if(tb<MaxTb )
			{
				flg = true;
				if ( bIsHeatLoss )		//�Ƿ��ж��������ɢ���ܶ�
				{
					Calc_Q_PlainCom(dCur_Q);
					if ( fabs(QMax) <= 1e-6 || dCur_Q <= QMax )
						flg = true;
					else
						flg = false;
				}
				if ( flg )
				{
					thick1_resu = thick1;
					thick2_resu = thick2;
					CString		strTmp;
					strTmp.Format("%.0f",tb);
					tb_resu		= _tcstod(strTmp,NULL);

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
// DATE         :[2005/04/26]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�����¶ȷ�,����ܵ�˫�㱣�º��
//------------------------------------------------------------------
short CCalcThick_FaceTemp::CalcPipe_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	double	ts = ts_resu;			//�����¶ȷ�����ʱ��������¶�Ϊ��֪�ġ��ɲ������ݹ�����
	double	tb;						//�����¶ȡ�
	double	dDiff1;					//�����ڲ��ȵĲ�ֵ.
	double	dDiff2;					//��������ȵĲ�ֵ.	
	double	dDiffMin1=0;			//�ڲ��ȵ���С��ֵ.
	double	dDiffMin2=0;			//����ȵ���С��ֵ.

//	double	dMaxTb = in_tmax*m_HotRatio;//�������������¶�=�Ᵽ�²��ϵ�����¶� * һ��ϵ��(��̹涨Ϊ0.9).
	double  MaxTb = in_tmax;//�������������¶�,�ӱ��²��ϲ�����(a_mat)��ȡMAT_TMAX�ֶ�,���޸ĸ��ֶν��п��ƽ����¶�ֵ.

	thick1_resu = 0.0;
	thick2_resu = 0.0;

	double  dCur_Q;			//��ǰ����µ�ɢ���ܶ�.
	double	QMax;			//���ݵ�ǰ��¼�Ĺ���,�������ɢ���ܶ�
	bool	flg = true;		//����ɢ���ܶȵ��ж�

	//����ý����¶���������ɢ���ܶ�
	if( !GetHeatLoss( m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax ) )
	{ 		
	}

	for (thick1=m_thick1Start; thick1<=m_thick1Max; thick1+=m_thick1Increment)
	{
		D1 = D0 + 2*thick1;
		for (thick2=m_thick2Start; thick2<=m_thick2Max; thick2+=m_thick2Increment)
		{
			//���ݵ�ǰ�ı��º�Ⱥ�������¶�,����������¶�.
			CalcPipe_Com_TsTb(thick1,thick2,ts,tb,1);

			D2 = D1 + 2*thick2;

			//���ݲ�ͬ����ҵ�淶�������ϵ��
			ALPHA =	GetPipeAlpha(ts,D2);
			
			//���ݸı��������¶Ⱥͽ����¶ȼ��㵼��ϵ��
			lamda1 = GetLamda1(t, tb);
			lamda2 = GetLamda2( tb, ts);
			
			//�������ı��º�Ĺ�ʽ,
			dDiff2 = D2*log(D2/D0)-2000/ALPHA/(ts-ta)*(lamda1*(t-tb)+lamda2*(tb-ts));	
//			dDiff2 = D2 * log( D2 / D0 ) - 2000 / ALPHA / fabs( ts - ta ) * ( lamda1 * fabs( t - tb ) + lamd2 * fabs( tb - ts ) );

			//�����ڲ㱣�º�Ĺ�ʽ
			dDiff1 = log(D1/D0)-2000*lamda1*(t-tb)/(ALPHA*D2*(ts-ta));
//			dDiff1 = log( D1 / D0 ) - 2000 * lamda1 * fabs( t - tb ) / ( ALPHA * D2 * fabs( ts - ta ) );

			if ((tb < MaxTb) &&							//�����¶Ȳ�Ӧ������㱣�²������ʹ���¶�
					( fabs(dDiffMin1) < 1e-6 && fabs(dDiffMin2) < 1e-6  ||					//Ϊ��һ��.
					  fabs(dDiff1) < fabs(dDiffMin1) && fabs(dDiff2) < fabs(dDiffMin2)) )	//��С��ֵʱ�ĺ��
			{
				flg = true;
				if ( bIsHeatLoss )
				{					
					Calc_Q_PipeCom( dCur_Q );		//���㵱ǰ����µ�ɢ���ܶ�
					if ( fabs(QMax) <= 1e-6 || dCur_Q <= QMax )
						flg = true;
					else
						flg = false;
				}
				if ( flg )
				{
					thick1_resu = thick1;		//���
					thick2_resu = thick2;
					tb_resu		= tb;			//�����¶�

					//���㵱ǰ����µ�ɢ���ܶ�
					this->Calc_Q_PipeCom(dQ);

					dDiffMin1	= dDiff1;		//��С�Ĳ�ֵ
					dDiffMin2	= dDiff2;
				}
			}
		}
	}
	return	1;
}


//------------------------------------------------------------------                
// DATE         :[2005/04/26]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�����¶ȷ�������ܵ�����
//------------------------------------------------------------------
short CCalcThick_FaceTemp::CalcPipe_One(double &thick_resu, double &ts_resu)
{
	double	ts = ts_resu;			//�����¶ȷ�����ʱ,������¶�Ϊ��֪�ġ�
	double	dDiff;					//��ʱ����	
	double  dDiffMin=0;				//��С�Ĳ�ֵ. 
	
	thick_resu = 0.0;				//���º�Ϊ0.
	
	double  dCur_Q;			//��ǰ����µ�ɢ���ܶ�.
	double	QMax;			//���ݵ�ǰ��¼�Ĺ���,�������ɢ���ܶ�
	bool	flg = true;		//����ɢ���ܶȵ��ж�

	//����ý����¶���������ɢ���ܶ�
	if( !GetHeatLoss( m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax ) )
	{ 		
	}
 
	for(thick=m_thick2Start;thick<=m_thick2Max;thick+=m_thick2Increment)
	{
		D1 = D0 + 2*thick;
		//���ݲ�ͬ����ҵ�淶�������ϵ��
		ALPHA =	GetPipeAlpha(ts, D1);

		//���ݸı��������¶ȼ��㵼��ϵ��
		lamda = GetLamda( t, ts );
		//�����¶ȷ�������ܵ����㱣�º�Ĺ�ʽ
		dDiff = D1*log(D1/D0)-2000*lamda*(t-ts)/ALPHA/(ts-ta);
//		dDiff = D1 * log( D1 / D0 ) - 2000 * lamda * fabs( t - ts ) / ALPHA / fabs( ts - ta );
		if (fabs(dDiffMin) < 1e-6 || fabs(dDiffMin) > fabs(dDiff) )
		{
			flg = true;
			if ( bIsHeatLoss )
			{
				this->Calc_Q_PipeOne(dCur_Q);
				if ( fabs(QMax) <= 1e-6 || dCur_Q <= QMax )
					flg = true;
				else
					flg = false;
			}
			if ( flg )
			{
				thick_resu	= thick;		//���ؼ�����ı��º�ȡ�
				dDiffMin	= fabs( dDiff );		//������С�Ĳ�ֵ

				//���㵱ǰ����µ�ɢ���ܶ�
				this->Calc_Q_PipeOne(dQ);
			}
		}
	}
	return	1;
}


//--------------------------------------------------------------------------
// DATE         :[2005/05/26]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :������Ⱥ�������¶ȼ���������¶�()��ƽ��˫�㡣
//--------------------------------------------------------------------------
short CCalcThick_FaceTemp::CalcPlain_Com_InputThick(double thick1_resu, double thick2_resu, double &tb_resu, double &ts_resu)
{
	thick1 = thick1_resu;
	thick2 = thick2_resu;		//����ָ���ĺ��.
	ts	   = ts_resu;

	//����ָ���ĺ�Ⱥ�������¶�,��������¶�
	CalcPlain_Com_TsTb(thick1,thick2,ts_resu,tb_resu,1);
	//����ɢ���ܶ�
	Calc_Q_PlainCom(dQ);

	return 1;

}


//--------------------------------------------------------------------------
// DATE         :[2005/05/26]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :������Ⱥ�������¶ȼ���������¶�()�����¶���Ϊ�ܵ���˫��.
//--------------------------------------------------------------------------
short CCalcThick_FaceTemp::CalcPipe_Com_InputThick(double thick1_resu, double thick2_resu, double &tb_resu, double &ts_resu)
{
	thick1 = thick1_resu;
	thick2 = thick2_resu;
	//��������¶�.
	CalcPipe_Com_TsTb(thick1,thick2,ts_resu,tb_resu,1);
	//����ɢ���ܶ�ʱ�õ��ĵ���ϵ���ͷ���ϵ���ڼ����¶�ʱ�Ѿ��õ���ֵ.
	//ɢ���ܶ�
	Calc_Q_PipeCom(dQ);


	return 1;
}


//--------------------------------------------------------------------------
// DATE         :[2005/05/26]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :ָ�����,��������¶�.ֻ��Ҫ����ɢ���ܶ�. �ܵ�����
//--------------------------------------------------------------------------
short CCalcThick_FaceTemp::CalcPipe_One_InputThick(double thick_resu, double &ts_resu)
{
	thick = thick_resu;
	ts	  = ts_resu;
	D1	  = D0 + 2 * thick;

	//����ϵ��
	lamda = GetLamda( t, ts );
	//���ݲ�ͬ����ҵ�淶�������ϵ��
	ALPHA = GetPipeAlpha(ts, D1);
	//�������ϳ�ʼ�ı���,���¼���ɢ���ܶ�.
	Calc_Q_PipeOne(dQ);

	return 1;
}
//--------------------------------------------------------------------------
// DATE         :[2005/05/26]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :ָ�����,��������¶�.ֻ��Ҫ����ɢ���ܶ�.		ƽ�ڵ���
//--------------------------------------------------------------------------
short CCalcThick_FaceTemp::CalcPlain_One_InputThick(double thick_resu, double &ts_resu)
{
	thick = thick_resu;
	ts	  = ts_resu;

	//����ϵ��
	lamda = GetLamda( t, ts );
	//���ݲ�ͬ����ҵ�淶�������ϵ��
	ALPHA = GetPlainAlpha(ts);
	//�������ϳ�ʼ�ı���,���¼���ɢ���ܶ�.
	Calc_Q_PlainOne(dQ);

	return 1;
}
