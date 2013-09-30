// CalcThickSubterranean.cpp: implementation of the CCalcThickSubterranean class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "CalcThickSubterranean.h"
#include "exceptioninfohandle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcThickSubterranean::CCalcThickSubterranean()
{

}

CCalcThickSubterranean::~CCalcThickSubterranean()
{

}

//------------------------------------------------------------------
// DATE         : [2006/03/06]
// Author       : ZSY
// Parameter(s) : dQ[out]: ����������ʧ����λ [Kcal/(m*h)]
// Return       : short 1
// Remark       : ���㲻ͨ�еع��з��赥��������ʧ
//------------------------------------------------------------------
short CCalcThickSubterranean::Calc_Q_PipeOne(double &dOneQ)
{
	double dRso;
	Calc_R_One(R1);
	Calc_Rso(dRso);
	
	dOneQ = (Tf1 - Tso) / (R1 + Rs1 + Raw + dRso);
	
	return 1;
}

//------------------------------------------------------------------
// Parameter(s) : dQ1[out]:��һ��������ʧ����λ[Kcal/(m*h)]
//				: dQ2[out]:�ڶ���������ʧ����λ[Kcal/(m*h)]
// Return       : void
// Remark       : ���㲻ͨ�еع��з�������(���)ʱ������ʧ
//------------------------------------------------------------------
void CCalcThickSubterranean::Calc_Q_PipeTwo(double &dQ1, double & dQ2)
{
	double dTaw;
	Calc_Taw_Two(dTaw);

	double dR1 = QMax / 2;
	double dR2;
	double dRo;
	double dTmpA1;
	double dTmpA2;
	
/* 
	( a - q2 * (A1 * a2 * Ro^2 + Rso^2 - Ro^2) )		
		= ( Ro * Rso * A1 * q2^2 - q2^2 * (B + C) ) / q1 + Ro * Rso * a2 * q1;

	q1 * ( a - q2 * (A1 * a2 * Ro^2 + Rso^2 - Ro^2) ) - Ro * Rso * a2 * q1^2		
		= ( Ro * Rso * A1 * q2^2 - q2^2 * (B - C) );
*/

	do 
	{
		R11 = dR1;
		R12 = dR2;

		Calc_R_One(dR1);
		Calc_R_Two(dR2);
		Calc_Ro(dRo);

		dQ = QMax / 2;
		
		dQ1 = (Tf1 - dTaw) / (R11 + Rs1);
		dQ2 = (Tf2 - dTaw) / (R12 + Rs2);
 
	} while(fabs(dQ1 - dR1) > DZero && fabs(dQ2 - dR2) > DZero);

}

//------------------------------------------------------------------
// DATE         : [2006/03/06]
// Author       : ZSY
// Parameter(s) : dR[out]:��һ���ܵ�������
// Return       : void
// Remark       : �޹ܹ��Ĺܵ�����
//------------------------------------------------------------------
void CCalcThickSubterranean::Calc_R_One(double &dR)
{
	double q1;		// ��һ��������ʧ
	double q2;		// �ڶ���������ʧ

	Calc_Q_PipeTwo( q1, q2 );
	Calc_Q_noOne(dQ); 

	dR = (Tf1 - Tso) * q2 * Ro / q1;
}

//------------------------------------------------------------------
// DATE         : [2006/03/06]
// Author       : ZSY
// Parameter(s) : dR[out]:�ڶ����ܵ�������
// Return       : void
// Remark       : �޹ܹ��Ĺܵ�����
//------------------------------------------------------------------
void CCalcThickSubterranean::Calc_R_Two(double &dR)
{
	double q1;		// ��һ��������ʧ
	double q2;		// �ڶ���������ʧ
	double dRo;
 
	Calc_Q_PipeTwo( q1, q2 );
	Calc_Ro(dRo);

	dR = (Tf1 - Tso) * q1 * dRo / q2; 
}

//------------------------------------------------------------------
// DATE         : [2006/03/06]
// Author       : ZSY
// Parameter(s) : dQ[out]:�����ܵ�������ʧ
// Return       : void
// Remark       : �޹ܹ���������ʧ
//------------------------------------------------------------------
void CCalcThickSubterranean::Calc_Q_noOne(double &dQ)
{
	double dRso;	// ����������
	Calc_R_One(R1);
	Calc_Rso(dRso);
	
	double dQTmp;
	dQ = QMax / 2;
	double dA;
	double dB;
	dA = pow((Tf1 - Tso) / dRso, 2);
	
	do
	{
		dQTmp = dQ;
		Calc_R_Two(R1);
		dQ = (Tf1 - Tso) / (R1 + dRso);
		
	} while(fabs(dQ - dQTmp) < DZero);
	
}

//------------------------------------------------------------------
// DATE         : [2006/03/06]
// Author       : ZSY
// Parameter(s) : dQ1[out] : ��һ���ܵ�������ʧ
//				: dQ2[out] : �ڶ����ܵ�������ʧ
// Return       : void
// Remark       : �޹ܹ��������ʧ
//------------------------------------------------------------------
void CCalcThickSubterranean::Calc_Q_noTwo(double &dQ1, double & dQ2)
{
	Calc_R_One(R1);
	Calc_R_Two(R2);
	Calc_Ro(Ro);
	double dPreQ1;
	double dPreQ2;

	do 
	{
		dPreQ1 = dQ1;
		dPreQ2 = dQ2;
		
		dQ1 = ((Tf1 - Tso) * (R2 + Rso) - (Tf2 - Tso) * Ro) / ((R1 + Rso) * (R2 + Rso) - (Ro * Ro));
		dQ2 = ((Tf1 - Tso) * (R1 + Rso) - (Tf1 - Tso) * Ro) / ((R1 + Rso) * (R2 + Rso) - (Ro * Ro));

		Calc_R_One(R1);
		Calc_R_Two(R2);
		
	} while(fabs(dPreQ1 - dQ1) < DZero && fabs(dPreQ2 - dQ2) < DZero);

}

//------------------------------------------------------------------
// DATE         : [2006/03/07]
// Author       : ZSY
// Parameter(s) : dRso[out] ����������
// Return       : void
// Remark       : ��������������
//------------------------------------------------------------------
void CCalcThickSubterranean::Calc_Rso(double &dRso)
{
	double Hm;	// ������ȼ���ֵ��
	if (sub_h <= DZero)
	{
		return;
	}
	Hm = sub_h + LamdaSo / AlphaSo;
	dRso = 1 / (2 * pi * LamdaSo) * log(2 * Hm * sqrt(4 * Hm * Hm + Dag * Dag) / Dag);
}

//------------------------------------------------------------------
// DATE         : [2006/03/07]
// Author       : ZSY
// Parameter(s) : ts[in] ������¶�
// Return       : ����ϵ��
// Remark       : �����ĵ���ϵ��
//-----------------------------------------------------------------
double CCalcThickSubterranean::GetLamdaSo(double ts)
{
	double dLSo;
	double dHm = 20+1E-22; //testzsy
	// �����ĵ���ϵ�� 
	// ��������ֲ��ϸ���������ʪ����ѡ����ϵ��
	dLSo = 0.58;	// ������
	//	dLSo = 1.163;	// ��̫ʪ������//	dLSo = 1.74;	// ��ʪ������//	dLSo = 2.33;	// ��ʪ������

//testzsy 
	m_nID++;
	if (fabs(ts-dHm) <= DZero)
	{
		return dLSo;
	}
	
	try
	{
		CString strSQL;
		if (m_pRsLSo == NULL || m_pRsLSo->GetState() == adStateClosed || (m_pRsLSo->GetadoEOF() && m_pRsLSo->GetBOF()))
		{
			return  1.163;	// ��̫ʪ�������ĵ���ϵ��
		}

		strSQL.Format("H=%f", dHm);
		m_pRsLSo->MoveFirst();
		m_pRsLSo->Find(_bstr_t(strSQL), NULL, adSearchForward);
		if (!m_pRsLSo->GetadoEOF())
		{
			dLSo = vtof(m_pRsLSo->GetCollect(_variant_t("Lso")));
			return dLSo;
		}
	}
	catch (_com_error& e)
	{
		ReportExceptionErrorLV2(e);
		return 0;
	}
	return dLSo;
}

//------------------------------------------------------------------
// DATE         : [2006/03/07]
// Author       : ZSY
// Parameter(s) : dRo[out] ��������
// Return       : void
// Remark       : �����ܵ�֮��ĵ�������
//------------------------------------------------------------------
void CCalcThickSubterranean::Calc_Ro(double &dRo)
{
//	LamdaSo = GetLamdaSo(ts);

	dRo = log(sqrt(1 + pow((2 * sub_h) / sub_b, 2))) / (2 * pi * LamdaSo);
}

//------------------------------------------------------------------
// DATE         : [2006/03/06]
// Author       : ZSY
// Parameter(s) : nID[in]  ��¼��ID��
// Return       : �ɹ�����TRUE�����򷵻�FALSE
// Remark       : ��õ�ǰ�����¼����عܵ��ı��¼������
//------------------------------------------------------------------
BOOL CCalcThickSubterranean::GetSubterraneanOriginalData(long nID)
{
	try
	{
		if (m_pRsSubterranean == NULL || m_pRsSubterranean->GetState() == adStateClosed || (m_pRsSubterranean->GetadoEOF() && m_pRsSubterranean->GetBOF()) )
		{
			return FALSE;
		}
		Tf1 = t;	// �ò�ͬ�ı�����������
		Tso = ta;

		CString	strSQL;		// SQL���
		double	dTwoD0;		// �ڶ����ܵ����⾶

		strSQL.Format("ID=%d",nID);
		m_pRsSubterranean->MoveFirst();
		m_pRsSubterranean->Find(_bstr_t(strSQL), 0, adSearchForward);		
		//����ָ����ŵ���عܵ����¼���Ĳ���
		if ( !m_pRsSubterranean->adoEOF )
		{
			// ��عܵ�����״̬
			subLayMark		= vtoi( m_pRsSubterranean->GetCollect( _variant_t("c_lay_mark") ) );
			// �ܵ���������ȣ��ر��浽�ܵ����ĵľ��룩
			sub_h			= vtof( m_pRsSubterranean->GetCollect( _variant_t("c_Pipe_Sub_Depth") ) );
			// �������¶ȡ�
			Tso				= vtof( m_pRsSubterranean->GetCollect( _variant_t("c_Edaphic_Temp") ) );
			// �����ܵ�֮��ĵ������
			sub_b			= vtof( m_pRsSubterranean->GetCollect( _variant_t("c_Pipe_Span") ) );
			// �����ĵ���ϵ��
			LamdaSo			= vtof( m_pRsSubterranean->GetCollect( _variant_t("c_Edaphic_Lamda") ) );
			// �ڶ����ܵ����⾶
			dTwoD0			= vtof( m_pRsSubterranean->GetCollect( _variant_t("c_Pipe_Two_D0") ) );
			
			Dag				= vtof( m_pRsSubterranean->GetCollect( _variant_t("c_Pipe_Dag") ) );  // �ܹ�����ֱ��
		}
		Rs1 = GetSubterraneanRs(D0, Tf1);
		// ��ñ��²���������
		if (D1 > 0)
		{
			Rs2 = GetSubterraneanRs(D1, Tf1);
		}
		// ��λת����-> mm)
		sub_h *= 1000;
		sub_b *= 1000;
		
		Aaw = A1pre = 9;	// ����
	}
	catch (_com_error)
	{
		return FALSE;
	}
	
	return TRUE;
}

//------------------------------------------------------------------
// DATE         : [2006/03/08]
// Author       : ZSY
// Parameter(s) : D0[in] �ܵ���ƽ�ڵ��⾶
//				: dTemp[in] �����¶�
//				: strPlace[in] ��װ�ص�
// Return       : ���²���������
// Remark       : ȡ���ܵ���ƽ�ڱ��½ṹ�����������Χ�����ķ�����
//------------------------------------------------------------------
double CCalcThickSubterranean::GetSubterraneanRs(const double &D0, const double &dTemp, CString strPlace /*=����*/)
{
	double dRsVal = 0;
	if (D0 < DZero)
	{
		return dRsVal;
	}
	try
	{
		if (m_pRsFaceResistance == NULL || m_pRsFaceResistance->GetState() == adStateClosed || m_pRsFaceResistance->GetRecordCount() <= 0)
		{
			return dRsVal; //
		}

		CString strSQL;
		strSQL.Format("dw=%f", D0);
		m_pRsFaceResistance->MoveFirst();
		
		if (strPlace.Compare("����"))
		{
			strSQL += " AND PLACE<>'�҃�' ";
		}
		m_pRsFaceResistance->Find(_bstr_t(strSQL), 0, adSearchForward);

		BOOL bFirst = TRUE;	// û�н���ѭ����
		double dPreVal = 0;	// ǰһ�ε�ֵ
		double dPreKey = 0;	// ǰһ�εĹؼ���
		double dCurVal;		// ��ǰ��ֵ
		double dCurKey;		// ��ǰ�Ĺؼ���
		for (; !m_pRsFaceResistance->adoEOF; m_pRsFaceResistance->MoveNext())
		{
			dCurVal = vtof(m_pRsFaceResistance->GetCollect(_variant_t("Val")));
			dCurKey = vtof(m_pRsFaceResistance->GetCollect(_variant_t("Key")));
			
			if (dCurKey >= D0)
			{
				if (bFirst)
					dRsVal = dCurVal;	// ��Ҫ���ҵĹؼ���С�ڻ���ڱ�����С�Ĺؼ�ֵ
				else
					dRsVal = (dCurVal - dPreVal) / (dCurKey - dPreKey) * (D0 - dPreKey) + dPreVal;

				return dRsVal;
			}
			bFirst = FALSE;
			dPreKey = dCurKey;
			dPreVal = dCurVal;
		}
		
		if ( bFirst == FALSE && m_pRsFaceResistance->adoEOF)	// ���ҵĹؼ��ֳ����˼�¼��������
		{
			dRsVal = dPreVal;	// ʹ������ֵ
		}
	}
	catch (_com_error& e)
	{
		ReportExceptionError(e);
		return dRsVal;
	}

	return dRsVal;
}

// ����ܵ��Ĳ�ͨ�йܹ��ڿ������¶�
BOOL CCalcThickSubterranean::Calc_Taw_One(double& dTaw)
{
	if (D0 < DZero)
	{
		return FALSE;
	}

	double dRsVal = 0;
	double dQ1;
	double dQ2;
	double dRso;
	double dRo;
	if (K < DZero)
	{
		K = 1;
	}
	
	Calc_Q_PipeTwo(dQ1, dQ2);
	Calc_Rso(dRso);
	Calc_Ro(dRo);
	
	if(bTawCalc)	// ����ܹ��ڿ������¶���������ʽ�����ݱ�����bTawCalc����ѡ�����
		dTaw = Tso + K * (dQ1 + dQ2) * (Raw + dRso);
	else
		dTaw = (Tf1 / (R11 + Rs1) + Tf2 / (R12 + Rs2) + Tso / (K * (Raw + dRso))) / (1 / (R11 + Rs1) + 1 / (R12 + Rs2) + 1 / ( K * (Raw + dRso)));

	return TRUE;
}

// �����ܵ��Ĳ�ͨ�йܹ��ڿ����¶�
BOOL CCalcThickSubterranean::Calc_Taw_Two(double& dTaw)
{
	if (D0 <= DZero)
	{
		return FALSE;
	}

	if (K <= DZero)//
	{
		K = 1;
	}

	double dRso;
	double dQ;

	Calc_Rso(dRso);
	Calc_Q_PipeOne(dQ); // ����ϵ��
	
	if (bTawCalc)
		dTaw = Tso + K * dQ * (Raw + dRso);
	else
		dTaw = Tso + (K * (Tf1 - Tso) * (Raw + Rso)) / (R1 + Rs1 + Raw + Rso);

	return TRUE;
}
