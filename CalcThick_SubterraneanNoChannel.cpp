// CalcThick_SubterraneanNoChannel.cpp: implementation of the CCalcThick_SubterraneanNoChannel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "CalcThick_SubterraneanNoChannel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcThick_SubterraneanNoChannel::CCalcThick_SubterraneanNoChannel()
{

}

CCalcThick_SubterraneanNoChannel::~CCalcThick_SubterraneanNoChannel()
{

}

//------------------------------------------------------------------
// Parameter(s) : dQ1 [out] ��һ���ܵ�������ʧ
//				: dQ2 [out] �ڶ����ܵ�������ʧ
// Return       : void http://www.sailordata.com
// Remark       : �޹ܹ������ܵ�������ʧ
//------------------------------------------------------------------
void CCalcThick_SubterraneanNoChannel::Calc_Q_PipeTwo(double &dReQ1, double &dReQ2)
{

	double dRso;	// ����������
//	double dR1;		// ��һ���ܵ����²�����
//	double dR2;		// �ڶ���
	double dQ1;
	double dQ2;
	double dRo;
	
	Calc_Rso(dRso);
	Calc_Ro(dRo);

//	Tf1	= 20;
	Tf2 = Tf1;	// test

//	dRo	= 0.1;
//	dRso = 2.2;

	double Tp1 = (Tf1 - Tso);	// ��һ���ܵ���������������²�
	double Tp2 = (Tf2 - Tso);	// �ܶ����ܵ���������������²�
	
	double dTmpA;	// �м�ĳ���
	double dTmpB;
	double dTmpC;
	double dTmpDt1;
	double dTmpDt2;
	double dTmpMr;
	
	double dTmpDelta1;
	double dTmpDelta2;
//		double B1 = (dR1 + dRso);
//		double B2 = (dR2 + dRso);

	double dCalcQ1A;	// ���̵���֪������ax^2 + bx + c = 0��
	double dCalcQ1B;
	double dCalcQ1C;
	double dCalcQ2A;
	double dCalcQ2B;
	double dCalcQ2C;
	
	double dQi	= QMax;
	double dQii = QMax;
//		double M;
//		double C1 = 0;
	
	// ����һ����ʼֵ
	dReQ1 = dQ1 = QMax / 2;
	dReQ2 = dQ2 = QMax / 2;
//	do                                     
//	{
		dReQ1 = dQ1;
		dReQ2 = dQ2;
//		Calc_Rpipe_Two(dQ1, dQ2, dR1, dR2);
		
		Calc_Rso(dRso);
		Calc_Ro(dRo);

		// �м����֪����һЩ����������
		dTmpA = Tp2 * dRo * dRso;
		dTmpB = Tp1 * dRo * dRso;
		dTmpC = Tp1 * Tp2 * pow(dRo, 2) + pow(dRso, 2) - pow(dRo, 2);
		dTmpMr = Tp1 * Tp2 * dRo;
		dTmpDt1 = Tp1 * dRso - dRo * Tp2;
		dTmpDt2 = Tp2 * dRso - dRo * Tp1;
		
		// ��һ����
		dCalcQ1A = dTmpA + dTmpB * pow((dTmpDt2 - dTmpMr) / (dTmpDt1 - dTmpMr), 2) + 
					(dTmpDt2 - dTmpMr) / (dTmpDt1 - dTmpMr) * dTmpC;

		dCalcQ1B = -(dTmpDt2 + (dTmpDt2 - dTmpMr) / (dTmpDt1 - dTmpMr) * dTmpMr);
		
		dCalcQ1C = 0;

		// �ڶ�����
		dCalcQ2A = dTmpA * pow((dTmpDt1 - dTmpMr) / (dTmpDt2 - dTmpMr), 2) + dTmpB + 
					(dTmpDt1 - dTmpMr) / (dTmpDt2 - dTmpMr) * dTmpC;
	
		dCalcQ2B = -(dTmpDt1 + (dTmpDt1 - dTmpMr) / (dTmpDt2 - dTmpMr) * dTmpMr);

		dCalcQ2C = 0;
 
		// b^2 - 4*a*c 
		dTmpDelta1 = pow(dCalcQ1B, 2) - 4 * dCalcQ1A * dCalcQ1C; 
		dTmpDelta2 = pow(dCalcQ2B, 2) - 4 * dCalcQ2A * dCalcQ2C;
		if (dTmpDelta1 >= 0)
		{
			// �ܵ�������ʧֵ
			dReQ1 = (-dCalcQ1B - dCalcQ1B) / (2 * dCalcQ1A);		// (-b - sqrt(b^2 - 4*a*c) ) / (2*a)

			dReQ2 = (-dCalcQ2B - dCalcQ2B) / (2 * dCalcQ2A);	// (-b - sqrt(b^2 - 4*a*c) ) / (2*a)
		}
		else
		{
			dReQ1 = dReQ2 = 0;
		}

//		dQ1 = ((Tf1 - Tso) * (dR2 + dRso) - (Tf2 - Tso) * dRo) / ((dR1 + dRso) * (dR2 + dRso) - pow(dRo, 2));
//		dQ2 = ((Tf1 - Tso) * (dR1 + dRso) - (Tf1 - Tso) * dRo) / ((dR1 + dRso) * (dR2 + dRso) - pow(dRo, 2)); 

//	} while(fabs(dReQ1 - dQ1) > TsDiff || fabs(dReQ2 - dQ2) > TsDiff); 

}

//------------------------------------------------------------------
// Parameter(s) : dOneQ [out] �θ��ܵ�����ʧ
// Return       : 1
// Remark       : �޹ܹ��θ��ܵ�����ʧ
//------------------------------------------------------------------
short CCalcThick_SubterraneanNoChannel::Calc_Q_PipeOne(double &dOneQ)
{
	double dRso;	// ����������
	double dR1;		// ���²�����
	double dLamda;	// ���²�ĵ���ϵ��
	
	dLamda = GetLamda(Tf1, Tso); //  
	D1 = D0 + 2 * thick;	// ���²��⾶

	Calc_Rso(dRso);
	Calc_Rpipe_One(D1, dLamda, dR1);

	dOneQ = (Tf1 - Tso) / (dR1 + dRso);

	if (dOneQ < DZero)
	{ 		
		dOneQ = fabs(dOneQ);
	}

	return 1;
}

//------------------------------------------------------------------
// DATE         : [2006/03/28]
// Author       : ZSY
// Parameter(s) : dRso[out] ��������
// Return       : void
// Remark       : �޹ܹ�������������
//------------------------------------------------------------------
void CCalcThick_SubterraneanNoChannel::Calc_Rso(double &dRso)
{
	if (sub_h < DZero) // �����ľ����ľ���ı���С����
	{
		dRso = Rso;
		return;
	}
	float dLSo;
	dLSo = GetLamdaSo(ts);

	D1 = D0 + 2 * thick;

	dRso = log(4 * sub_h / D1) / (2 * pi * dLSo);
 
	return;

	try
	{
		CString strSQL;
		CString strPipeLay;
		_RecordsetPtr pRs;
		pRs.CreateInstance(__uuidof(Recordset));
		strSQL = "select * from [��عܵ�����״̬]";
		pRs->Open(_variant_t(strSQL), _variant_t(m_pConAutoIPED.GetInterfacePtr()), \
			adOpenStatic, adLockOptimistic, adCmdText);
		if (pRs->GetRecordCount() < 0)
		{
			return;
		}
		strSQL.Format("index=%d", m_nID);
		pRs->Find(_bstr_t(strSQL), 1, adSearchForward);
		if (!pRs->adoEOF)
		{
			strPipeLay = vtos(pRs->GetCollect(_variant_t("pipelay")));
			m_dMaxD0 = vtof(pRs->GetCollect(_variant_t("Formula")));
		}
		if (strPipeLay.CompareNoCase(_T("�йܹ���ͨ��")))
		{
			for (pRs->MoveFirst(); !pRs->adoEOF; pRs->MoveNext())
			{
				strPlace = vtos(pRs->GetCollect(_T("Formula")));
				out_mat = vtos(pRs->GetCollect(_T("Index")));
			}
		}
	
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}
}

//------------------------------------------------------------------
// DATE         : [2006/03/28]
// Author       : ZSY
// Parameter(s) : dQ1[in] ��һ���ܵ���ɢ���ܶ�
//				: dQ2[in] �ڶ����ܵ���ɢ���ܶ�
//				: dR1[out] ��һ���ܵ�������
//				: dR2[out]	�ڶ����ܵ�������
// Return       : void
// Remark       : �޹ܹ��ܵ����� (�����ܵ�����)
//------------------------------------------------------------------
void CCalcThick_SubterraneanNoChannel::Calc_Rpipe_Two(double dQ1, double dQ2, double &dR1, double &dR2)
{
	double dRo;
	Calc_Ro(dRo);

	dR1 = (Tf1 - Tso) * dQ2 * dRo / dQ1;
	dR2 = (Tf2 - Tso) * dQ1 * dRo / dQ2; 
}

//------------------------------------------------------------------
// Parameter(s) : D1[in] ���²��⾶
//				: dLamda[in] ���²�ĵ���ϵ��
//				: dR1[out] ���²�����
// Return       : void
// Remark       : �޹ܹ��ĵ������²�����
//------------------------------------------------------------------
void CCalcThick_SubterraneanNoChannel::Calc_Rpipe_One(double D1, double dLamda, double &dR1)
{
	D1 = D0 + 2 * thick;

	dR1 = log(D1 / D0) / (2 * pi * dLamda);
}

//------------------------------------------------------------------
// Parameter(s) : dRo [out] ��������
// Return       : void
// Remark       : �޹ܹ�����������ܵ�֮��ĵ�������
//------------------------------------------------------------------
void CCalcThick_SubterraneanNoChannel::Calc_Ro(double &dRo)
{
	float dLSo = GetLamdaSo(ts); 

	dRo = log(sqrt(1 + pow((2 * sub_h) / sub_b, 2))) / (2 * pi * dLSo); 
}

//------------------------------------------------------------------                
// Author       : ZSY
// Parameter(s) : thick_resu[out] ���º�ȣ� ts_resu[out] ���²�������¶�
// Return       : 
// Remark       : ���㣬�޹ܹ�ֱ����豣�²���, �ͱ��²�������¶�
//------------------------------------------------------------------
short CCalcThick_SubterraneanNoChannel::CalcPipe_One(double &thick_resu, double &ts_resu)
{
	double	ts=0.0;			//������¶�ֵ
	double	dYearVal;		//�����й��������ɢ���ܶ� 
	double	dSeasonVal;		//�������й������ɢ���ܶ� 
	double	Q = 1;			//��ǰ����µ�ɢ���ܶ�.
	double	QMax = 0;		//���ݵ�ǰ��¼�Ĺ���;�������ɢ���ܶ�

	double	dLSo;			// �����ĵ���ϵ��
	double	dASo;			//
	double  dDiff;
	double  dMinDiff = 0;
	bool	flg		 = true;//����ɢ���ܶȵ��ж�

	thick_resu  = 0;
	ts_resu		= 0;
	// ���ԭʼ���� 	
	GetSubterraneanOriginalData(m_nID);
	GetHeatLoss(m_pRsHeatLoss, Tf1, dYearVal, dSeasonVal, QMax);
	dQA = QMax;
	Calc_Ro(Ro);
	if (Ro <= DZero)
	{
		Ro = 0.6;
	}
	if (fabs(QMax) >= DZero)
	{
		Q	= QMax * m_MaxHeatDensityRatio;			//���½ṹ���������ɢ���ܶȣ������в�����������ɢ���ܶȵĳ���һ��ϵ��ȡֵ.(������ҵ��̹涨Ϊ90%).
		dQA = QMax;
	}
	else
	{
		dQA = fabs(QMax);	// ������ʧΪһ������ʱ
	}
	//thick���㱣�º�ȡ�
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����ThicknessRegular���

	for (thick=m_thick2Start; thick <= m_thick2Max; thick += m_thick2Increment)
	{
		D1 = D0 + 2 * thick;

		//����ָ���ı��º񣬼���������¶�    
		CalcPipe_One_TsTemp(thick, ts);
		
		dLSo  = GetLamdaSo(ts);		// �����ĵ���ϵ��
		lamda = GetLamda(t, ts);	// ���²�ĵ���ϵ��
		dASo  = GetPipeAlpha(ts, D1); 
		dDiff = log(D1 / D0) - 2.0 * pi * lamda * dLSo / (dLSo - lamda) * ((Tf1 - Tso) / dQA - (log(4 * sub_h / D0) / (2 * pi * dLSo)));

		if (ts<MaxTs)
		{
			if (fabs(dMinDiff) <= DZero || fabs(dDiff) < fabs(dMinDiff))
			{
				//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�
				if(bIsHeatLoss)
				{
					//��ñ��κ�ȵ�ɢ���ܶ�   
					Calc_Q_PipeOne(Q);				
					if(fabs(QMax) <= DZero || Q <= QMax)
					{
						flg = true; // ��ǰ��������������ɢ�ȹ���
					} 
					else
					{
						flg = false;
					}
				}
				if( flg )
				{
					dMinDiff   = fabs(dDiff);
					thick_resu = thick;
					ts_resu	   = ts;
					//���㵱ǰ����µ�ɢ���ܶ�
					Calc_Q_PipeOne(dQ);
				}
			}
		}
	}
	return 1;
}

 
//------------------------------------------------------------------
// Parameter(s) : thick1_resu ��һ���ܵ��ı��º��
//				��thcik2_resu �ڶ����ܵ��ı��º��
//				: ts1_resu �����¶�
//				: ts2_resu 
// Return       : 
// Remark       : ����ֱ������˫���ܵ����º�ȼ���
//------------------------------------------------------------------
short CCalcThick_SubterraneanNoChannel::CalcPipe_Com(double &thick1_resu, double &thick2_resu, double &ts1_resu, double &ts2_resu)
{

	double	ts = 0.0;			//������¶�ֵ
	double  ts2 = 0.0;
	double	dYearVal;		//�����й��������ɢ���ܶ� 
	double	dSeasonVal;		//�������й������ɢ���ܶ� 
	double	Q = 1;				//��ǰ����µ�ɢ���ܶ�.
	double	QMax = 0;			//���ݵ�ǰ��¼�Ĺ���;�������ɢ���ܶ�

	double	dLSo;	// �����ĵ���ϵ��
	double	dASo;	//
	double  dMinDiff1 = DZero;
	double  dMinDiff2 = DZero;
	double  dDiff1;
	double	dDiff2;
	double  dQ1;
	double  dQ2;
// testzsy
	Tso = 12;
	double	Dii0;
	double  Dii1;
	CString strTmp;
	bool	flg=true;		//����ɢ���ܶȵ��ж�
	
	thick1_resu = thick2_resu = 0;
	ts1_resu = ts2_resu = 0;
	if( !GetSubterraneanOriginalData(m_nID))
	{
		strTmp.Format("���Ϊ%d�ļ�¼���ڶ�ȡԭʼ����ʱ����\r\n", m_nID);
		strExceptionInfo += strTmp;
		return 0;
	}
	GetHeatLoss(m_pRsHeatLoss, t, dYearVal, dSeasonVal, QMax);

	if (fabs(QMax) >= DZero)
	{
		Q = QMax * m_MaxHeatDensityRatio;			//���½ṹ���������ɢ���ܶȣ������в�����������ɢ���ܶȵĳ���һ��ϵ��ȡֵ.(��̹涨Ϊ90%).
	}
	else
	{
		dQA = QMax; 
	}
	Calc_Ro(Ro);
	if (Ro <= DZero)
	{
		Ro = 0.6;
	}
	//thick���㱣�º�ȡ�
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����ThicknessRegular��� 

	for (thick1=m_thick1Start; thick1 <= m_thick1Max; thick1 += m_thick1Increment)
	{
		for (thick2 = m_thick2Start; thick2 <= m_thick2Max; thick2 += m_thick2Increment)
		{
			D1 = D0 + 2 * thick1;
			D2 = D1 + 2 * thick2;
			Dii1 = D2;
			Dii0 = D0;
			
			//����ָ���ı��º񣬼���������¶�
//			CalcPipe_Com_TsTb(thick1, thick2, ts, tb);
			
//			CalcSubPipe_TsTemp(D0, thick1, ts, 1);
//			CalcSubPipe_TsTemp(Dii1, thick2, ts2, 2);

			Calc_Q_PipeTwo(dQ1, dQ2);
			
			dLSo   = GetLamdaSo(ts);	// �����ĵ���ϵ��
			lamda  = GetLamda(t, ts);	// ���²�ĵ���ϵ��			
			dASo   = GetPipeAlpha(ts, D1);
			dDiff1 = log(D1 / D0) - 2.0 * pi * lamda * dLSo / (dLSo - lamda) *
				     ((Tf1 - Tso - dQ2 * Ro) / dQ1 - (log(4 * sub_h / D0) / (2 * pi * dLSo)));
			dDiff2 = log(Dii1 / Dii0) - 2.0 * pi * dLSo * lamda2 / (dLSo - lamda2) *
					 ((Tf2 - Tso - dQ1 * Ro) / dQ2 - log(4 * sub_h / Dii0) / (2 * pi * dLSo));
			dDiff1 = fabs(dDiff1);
			dDiff2 = fabs(dDiff2);
			
			if (ts < MaxTs && ts2 < MaxTs)
			{
				if ((dMinDiff1 <= DZero && dMinDiff2 <= DZero) || (dDiff1 < dMinDiff1 && dDiff2 < dMinDiff2))
				{
					flg = true;
					//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�
					if(bIsHeatLoss)
					{
						//��ñ��κ�ȵ�ɢ���ܶ�
						Calc_Q_PipeTwo(dQ1, dQ2);
						
						if(fabs(QMax) <= DZero || (dQ1 <= QMax && dQ2 <= QMax))
						{
							flg = true; // ��ǰ��������������ɢ�ȹ��� 
						}
						else 
						{
							flg = false; 
						}
					}
					//��������
					if( flg )
					{
						dMinDiff1   = dDiff1;
						dMinDiff2   = dDiff2;
						thick1_resu = thick1;
						thick2_resu = thick2;
						ts1_resu	= tb;
						ts2_resu    = ts;
						
						//���㵱ǰ����µ�ɢ���ܶ�
						Calc_Q_PipeOne(dQ);
						//	Calc_Q_PipeTwo(dQ1, dQ2);
					}
				}
			}
		}
	}
	return 1;
}/*

//------------------------------------------------------------------	
short CCalcThick_SubterraneanNoChanneal::CalcPlain_One_TsTemp(const double delta, double &dResTs)
{	
	double ts;	// ��ʱ��������¶� [2005/06/07]
	int nBreak = 0;
	dResTs = ts = ( t - ta ) / 2 + ta;		//������¶ȸ���ֵ��
	while (TRUE)
	{
		// ���ݲ�ͬ����ҵ�淶�������ϵ��
		ALPHA = GetPlainAlpha(ts);

		//���ݸı��������¶ȼ��㵼��ϵ��
		lamda = GetLamda( t, ts );

		ts = (delta/1000/lamda*ta+1/ALPHA*t)/(delta/1000/lamda+1/ALPHA);
		if (fabs(ts-dResTs) < TsDiff || nBreak >= MaxCycCount)
		{
			break;
		}
		dResTs = ts;	//
		nBreak++;
	}
	return 1;
}
*/





