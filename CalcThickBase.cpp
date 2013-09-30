// CalcThickBase.cpp: implementation of the CCalcThickBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "CalcThickBase.h"
#include <math.h>

#include "edibgbl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		PREVENT_SCALD    3			//������

extern CAutoIPEDApp	theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcThickBase::CCalcThickBase()
{
}

CCalcThickBase::~CCalcThickBase()
{

}
//��ʼ�������ϵ��ʱ�õ��ı���
#define INIT_ALPHADATA(CLASSN) \
	CLASSN::dA_ta	= ta;\
	CLASSN::dA_ts	= ts;\
	CLASSN::dA_W	= speed_wind;\
	CLASSN::dA_B	= B;\
	CLASSN::dA_hedu	= hedu;\
	CLASSN::dA_AlphaIndex = nAlphaIndex;

//------------------------------------------------------------------                
// DATE         :[2005/04/25]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :����ƽ�ڵ��� ,������¶ȵ�ֵ
//------------------------------------------------------------------	
short CCalcThickBase::CalcPlain_One_TsTemp(const double delta, double &dResTs)
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

		//���²��²�
		float tta=(t-ta)/(1+lamda/ALPHA*1000/delta);
		if(t>=ta)//����
			ts = t-tta;
		else //����
			ts = t+tta;
		if (fabs(ts-dResTs) < TsDiff || nBreak >= MaxCycCount)
		{
			break;
		}
		dResTs = ts;	//
		nBreak++;
	}
	return 1;
}

//------------------------------------------------------------------                
// DATE         :[2005/04/25]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :����ܵ�����,������¶ȵ�ֵ
//------------------------------------------------------------------
short CCalcThickBase::CalcPipe_One_TsTemp(const double delta, double &dResTs)
{	
	double ts;	// ��ʱ��������¶� [2005/06/07]
	int    nBreak = 0;
	D1 = D0 + 2 * delta;			//���²��⾶��
	
	dResTs = ts = ( t - ta ) / 2 + ta;		//������¶ȸ���ֵ��

	while (TRUE)
	{
		//���ݲ�ͬ����ҵ�淶�������ϵ��
		ALPHA = GetPipeAlpha(ts, D1);

		//���ݸı��������¶ȼ��㵼��ϵ��
		lamda = GetLamda( t, ts );

		//���²��²�
		float tta=(t-ta)/(1+lamda/ALPHA*2000/log(D1/D0)/D1);
		ts = t-tta;
		
		if (fabs(ts-dResTs) < TsDiff || nBreak >= MaxCycCount)
		{
			break;
		}
		dResTs = ts;	//
		nBreak++;
	}
	return 1;
}

// ������عܵ��ı����¶�
void CCalcThickBase::CalcSubPipe_TsTemp(const double dPipeTemp, const double dDW, const double delta, double &dResTs, short nMark)
{
	double ts;//���²�����¶�
	double dD1;//���²��⾶
	int	   nBreak = 0;

	dD1 = dDW + 2 * delta;
	dResTs = (dPipeTemp + ta) / 2.0 + ta;
	while ( TRUE )
	{
		ALPHA = GetPipeAlpha(ts, dD1);
		lamda = GetLamdaA(dPipeTemp, ts, nMark);

		//���²��²�
		float tta=(t-ta)/(1+lamda/ALPHA*2000/log(dD1 / dDW)/D1);
		if(t>=ta)//����
			ts = t-tta;
		else //����
			ts = t+tta;
		if (fabs(ts - dResTs) < TsDiff || nBreak >= MaxCycCount)
		{
			break;
		}
		dResTs = ts;
		
	}
}
//------------------------------------------------------------------                
// DATE         :[2005/04/25]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :����ƽ��˫��,�����ͽ��洦���¶ȵ�ֵ
//------------------------------------------------------------------
short CCalcThickBase::CalcPlain_Com_TsTb(const double delta1, const double delta2, double &dResTs, double &dResTb, int flg)
{	
	double ts;	// ��ʱ��������¶� [2005/06/07]
	double tb;	// ��ʱ�Ľ����¶�	[2005/06/07]
	int    nBreak = 0;
	if( flg == 1 )		//����ts(�����¶�)
	{
		//���ݱ��º�Ⱥ�������¶�,��������¶�;	
		ts		= dResTs;						//������¶�Ϊ��֪
		dResTb	= tb = (t-ta)/3.0*2.0+ta;		//���洦���¶ȸ���ֵ��
	}
	else
	{
		dResTs = ts = (t-ta)/3.0+ta;		//������¶ȸ���ֵ��
		dResTb = tb = (t-ta)/3.0*2.0+ta;	//���洦���¶ȸ���ֵ��
	}
	while (TRUE)
	{
		//���ݲ�ͬ����ҵ�淶�������ϵ��
		ALPHA = GetPlainAlpha(ts);
		
		//���ݸı��������¶Ⱥͽ����¶ȼ��㵼��ϵ��
		lamda1 = GetLamda1( t, tb );
		lamda2 = GetLamda2( tb, ts);
		
		if( flg == 1 )
		{
			//���ݱ��º�Ⱥ�������¶�,��������¶�;
			//������¶�Ϊ��֪��,���ټ���.
		}
		else
		{
			//����������¶ȵ�ֵ��
			ts = (delta1/1000/lamda1*ta+delta2/1000/lamda2*ta+1/ALPHA*t)/(delta1/1000/lamda1+delta2/1000/lamda2+1/ALPHA);
		}

		//�����¶�
		tb = (delta1/1000/lamda1*ta+delta2/1000/lamda2*t+1/ALPHA*t)/(delta1/1000/lamda1+delta2/1000/lamda2+1/ALPHA);
				
		//���������εĽ��֮�����һ��������0.1��ʱ�����ء�
		if ((fabs(dResTs-ts)<TsDiff) && (fabs(dResTb-tb)<TsDiff) || nBreak >= MaxCycCount)
		{
			break;
		}
		dResTs = ts;
		dResTb = tb;
		nBreak++;
	}
	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/04/25]
// Author       :ZSY
// Parameter(s) :flg = 1 ʱ,���ݱ��º�Ⱥ�������¶�,��������¶�;
//				:����,���ݱ��º��,��������¶Ⱥ�������¶�.
// Return       :
// Remark       :����ܵ�˫��,�����ͽ��洦���¶ȵ�ֵ
//------------------------------------------------------------------
short CCalcThickBase::CalcPipe_Com_TsTb(const double delta1, const double delta2, double &dResTs, double &dResTb,int flg)
{
	double ts;	// ��ʱ��������¶� [2005/06/07]
	double tb;	// ��ʱ�Ľ����¶�	[2005/06/07]
	int    nBreak = 0;

	if( flg == 1 )		//����ts(�����¶�)
	{
		//���ݱ��º�Ⱥ�������¶�,��������¶�;	
		ts		= dResTs;						//������¶�Ϊ��֪
		dResTb	= tb = (t-ta)/3.0*2.0+ta;		//���洦���¶ȸ���ֵ.
	}
	else
	{
		dResTs = ts = (t-ta)/3.0+ta;		//������¶ȸ���ֵ��
		dResTb = tb = (t-ta)/3.0*2.0+ta;	//���洦���¶ȸ���ֵ��
	}
	D1 = D0 + 2.0 * delta1;
	D2 = D1 + 2.0 * delta2;
	while (TRUE)
	{
		//���ݲ�ͬ����ҵ�淶�������ϵ��
		ALPHA = GetPipeAlpha(ts,D2);

		//���ݸı��������¶Ⱥͽ����¶ȼ��㵼��ϵ��
		lamda1 = GetLamda1( t, tb );
		lamda2 = GetLamda2( tb, ts);
	
		//����������¶ȵ�ֵ��
		if( flg == 1 )
		{
			//���ݱ��º�Ⱥ�������¶�,��������¶�
			//������¶�Ϊ��֪��,���ټ���
		}
		else
		{
			ts = (log(D1/D0)/lamda1*ta+log(D2/D1)/lamda2*ta+2000/ALPHA/D2*t)/(log(D1/D0)/lamda1+log(D2/D1)/lamda2+2000/ALPHA/D2);
		}
		//�����¶�
		tb = (log(D1/D0)/lamda1*ta+log(D2/D1)/lamda2*t+2000/ALPHA/D2*t)/(log(D1/D0)/lamda1+log(D2/D1)/lamda2+2000/ALPHA/D2);
				
		//���������εĽ��֮�����һ��������0.1��ʱ�����ء�
		if ((fabs(dResTs-ts)<TsDiff) && (fabs(dResTb-tb)<TsDiff) || nBreak >= MaxCycCount)
		{
			break;
		}
		dResTs = ts;
		dResTb = tb;
		nBreak++;
	}
	return 1;
}

//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :���㱣�º�ȣ�ƽ��˫�㡣
//------------------------------------------------------------------
short CCalcThickBase::CalcPlain_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	return 1;
}

//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :���㱣�º�ȣ�ƽ�浥�㡣
//------------------------------------------------------------------
short CCalcThickBase::CalcPlain_One(double &thick_resu, double &ts_resu)
{
	return 1;
}	

//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :���㱣�º�ȣ����¶���Ϊ�ܵ���˫��
//------------------------------------------------------------------
short CCalcThickBase::CalcPipe_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :���㱣�º�ȣ����¶���Ϊ�ܵ�������
//------------------------------------------------------------------
short CCalcThickBase::CalcPipe_One(double &thick_resu, double &ts_resu)
{
	return 1;	
}

//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :��
// Return       :
// Remark       :�����ݿ���ȡ������,�������Ա������.
//
				//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
				//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
				//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
				//�ӱ��º�ȹ����thicknessRegular����ȡ�������ֵ��
//------------------------------------------------------------------
short CCalcThickBase::GetThicknessRegular()
{
	CString strSQL,strMessage;
	_variant_t var;
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	try
	{		
		strSQL = "SELECT * FROM [thicknessRegular] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
		pRs->Open(_bstr_t(strSQL), m_pConAutoIPED.GetInterfacePtr(),
				adOpenStatic, adLockOptimistic, adCmdText);
		if( pRs->GetRecordCount() > 0 )
		{
			var = pRs->GetCollect(_variant_t("thick1Start"));		//�ڲ㱣��������С���	
			m_thick1Start = vtoi( var );

			var = pRs->GetCollect(_variant_t("thick1Max"));			//�ڲ㱣�����������
			m_thick1Max = vtoi( var );

			var = pRs->GetCollect(_variant_t("thick1Increment"));	//�ڲ㱣�º����С����
			m_thick1Increment = vtoi( var );

			var = pRs->GetCollect(_variant_t("thick2Start"));	//��㱣��������С���
			m_thick2Start = vtoi( var );

			var = pRs->GetCollect(_variant_t("thick2Max"));		//��㱣�����������
			m_thick2Max = vtoi( var );

			var = pRs->GetCollect(_variant_t("thick2Increment"));//��㱣�º����С����
			m_thick2Increment = vtoi( var );
			
			//������ݵ���ȷ�ԡ�
			if( (m_thick1Max<m_thick1Start) )
			{
				strMessage.Format("�ڲ㱣�����������%d mmС���ڲ㱣��������С���%d mm����",m_thick1Max,m_thick1Start);
				AfxMessageBox(strMessage);
				return 0;
			}
			if(m_thick2Max < m_thick2Start)
			{
				strMessage.Format("��㱣�����������%d mmС����㱣��������С���%d mm����",m_thick2Max,m_thick2Start);
				AfxMessageBox(strMessage);
				return 0;
			}
			if( m_thick1Increment <= DZero )
			{
				strMessage.Format("�ڲ㱣�º����С����%d mm̫С��",m_thick1Increment);
				AfxMessageBox(strMessage);
				return 0;
			}
			if( m_thick2Increment <= DZero )
			{
				strMessage.Format("��㱣�º����С����%d mm̫С.",m_thick2Increment);
				AfxMessageBox(strMessage);
				return 0;
			}
			if( m_thick1Start <= DZero )
			{
				strMessage.Format("�ڲ㱣��������С���%d mm̫С��",m_thick1Start);
				AfxMessageBox(strMessage);
				return 0;
			}
			if( m_thick2Start <= DZero )
			{
				strMessage.Format("��㱣��������С���%d mm̫С��",m_thick2Start);
				AfxMessageBox(strMessage);
				return 0;
			}
			if( m_thick1Max <= DZero )
			{
				strMessage.Format("�ڲ㱣�����������%d mm̫С��",m_thick1Max);
				AfxMessageBox(strMessage);
				return 0;
			}
			if( m_thick2Max <= DZero )
			{
				strMessage.Format("��㱣�����������%d mm̫С��",m_thick2Max);
				AfxMessageBox(strMessage);
				return 0;
			}
		}
		else
		{
			if( IDNO == AfxMessageBox("���º�ȹ������û�м�¼���Ƿ�Ĭ�ϵ����ݽ��м��㣿",MB_YESNO) )
			{
				return 0;
			}
			m_thick1Start = 30;		//�ڲ㱣��������С���	
			m_thick1Max= 300;		//�ڲ㱣�����������
			m_thick1Increment=10;	//�ڲ㱣�º����С����

			m_thick2Start = 30;		//��㱣��������С���
			m_thick2Max = 300;		//��㱣�����������
			m_thick2Increment=10;	//��㱣�º����С����

			//��Ĭ�ϵ�����д�뵽���ݿ��С�
			strSQL.Format("INSERT INTO [thicknessRegular] (thick1Start,thick1Max ,thick1Increment ,thick2Start ,thick2Max ,thick2Increment ,EnginID )\
				VALUES(%d,%d,%d,%d,%d,%d,'%s') ",m_thick1Start,m_thick1Max,m_thick1Increment,m_thick2Start,m_thick2Max,m_thick2Increment,EDIBgbl::SelPrjID);
			theApp.m_pConnection->Execute(_bstr_t(strSQL), NULL, adCmdText );
		}

	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return 0;
	}
	return	1;
}


//------------------------------------------------------------------                
// DATE         :[2005/04/25]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :���ݽ����¶�,ȡ���½ṹ������������ɢ���ܶ�
//------------------------------------------------------------------
bool CCalcThickBase::GetHeatLoss(_RecordsetPtr &pRs, double &t, double &YearRun, double &SeasonRun, double &QMax)
{
	try
	{		
		YearRun = SeasonRun = QMax= 0;
		if( pRs->adoEOF && pRs->BOF )
		{
			//û�м�¼.
			return false;
		}
			
		YearRun=GetMaxHeatLoss(pRs,t,"YearRun");
		SeasonRun=GetMaxHeatLoss(pRs,t,"SeasonRun");
		if( hour_work )
		{	//�����й���
			QMax = YearRun;
		}
		else
		{	//�������й���
			QMax = SeasonRun;
		}
		return true;
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return false;
	}	
}


//------------------------------------------------------------------                
// DATE         :[2005/04/28]
// Author       :ZSY
// Parameter(s) :���ڴ�ż������õ�ɢ���ܶ�.(������Ա�����ڵ���֮ǰ�ȳ�ʼ��)
// Return       :�ɹ�����1,���򷵻�0
// Remark       :���㱣�½ṹ�����ɢ���ܶ�.(�ܵ�.����)
//------------------------------------------------------------------
short CCalcThickBase::Calc_Q_PipeOne(double &Q)
{
	//��ñ��κ�ȵ�ɢ���ܶ�
	//��ʽ:
	//(�����¶�-�����¶�)/((���²��⾶/2000.0/���ϵ�����*ln(���²��⾶/�ܵ��⾶)) + 1/����ϵ��)
	Q = fabs( t - ta ) / ( D1 / 2000.0 / lamda * log( D1 / D0 ) + 1.0 / ALPHA );

	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/04/28]
// Author       :ZSY
// Parameter(s) :���ڴ�ż������õ�ɢ���ܶ�.(������Ա�����ڵ���֮ǰ�ȳ�ʼ��)
// Return       :�ɹ�����1,���򷵻�0
// Remark       :���㱣�½ṹ�����ɢ���ܶ�.(�ܵ�.˫��)
//------------------------------------------------------------------
short CCalcThickBase::Calc_Q_PipeCom(double &Q)
{
	//��ñ��κ�ȵ�ɢ���ܶ�
	//��ʽ:
	//(�����¶�-�����¶�)/((�Ᵽ�²��⾶/2000.0)*(1.0/�ڱ��²��ϵ�����*ln(�ڱ��²��⾶/�ܵ��⾶) + 1.0/�Ᵽ�²��ϵ�����*ln(�Ᵽ�²��⾶/�ڱ��²��⾶) ) + 1/����ϵ��)
	Q = fabs( t - ta ) / ( (D2 / 2000.0) * (1.0 / lamda1 * log(D1 / D0) + 1.0 / lamda2 * log(D2 / D1)) + 1.0 / ALPHA );

	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/04/28]
// Author       :ZSY
// Parameter(s) :���ڴ�ż������õ�ɢ���ܶ�.(������Ա�����ڵ���֮ǰ�ȳ�ʼ��)
// Return       :�ɹ�����1,���򷵻�0
// Remark       :���㱣�½ṹ�����ɢ���ܶ�.(ƽ��.����)
//------------------------------------------------------------------
short CCalcThickBase::Calc_Q_PlainOne(double &Q)
{
	//��ñ��κ�ȵ�ɢ���ܶ�
	//��ʽ:
	//(�����¶�-�����¶�)/(��ǰ���º�/(1000*���ϵ�����) + 1/����ϵ��)
	Q = fabs(t - ta) / (thick / (1000 * lamda) + 1.0 / ALPHA); 

	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/04/28]
// Author       :ZSY
// Parameter(s) :���ڴ�ż������õ�ɢ���ܶ�.(������Ա�����ڵ���֮ǰ�ȳ�ʼ��)
// Return       :�ɹ�����1,���򷵻�0
// Remark       :���㱣�½ṹ�����ɢ���ܶ�.(ƽ��.˫��)
//------------------------------------------------------------------
short CCalcThickBase::Calc_Q_PlainCom(double &Q)
{
	//��ñ��κ�ȵ�ɢ���ܶ�
	//��ʽ:
	//(�����¶�-�����¶�)/(�ڱ��º�/1000/�ڱ��²��ϵ����� + �Ᵽ�º�/1000/�Ᵽ�²��ϵ����� + 1/����ϵ��)
	Q = fabs(t - ta) / (thick1/1000.0/lamda1+thick2/1000.0/lamda2+1.0/ALPHA);

	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/04/28]
// Author       :ZSY
// Parameter(s) :���ڴ�ż������õ�ɢ�����ܶ�.(������Ա�����ڵ���֮ǰ�ȳ�ʼ��)
// Return       :�ɹ�����1,���򷵻�0
// Remark       :���㱣�½ṹɢ�����ܶ�.(�ܵ�.����)
//------------------------------------------------------------------
short CCalcThickBase::Calc_QL_PipeOne(double &QL)
{
	//��ñ��κ�ȵ�ɢ�����ܶ�
	//��ʽ:
	//(2.0*pi*(�����¶�-�����¶�)) / ((1.0/���ϵ�����*ln(���²��⾶/�ܵ��⾶)) + 2000/����ϵ��/���²��⾶)
	QL = (2.0 * pi * fabs(t - ta)) / (1.0 / lamda * log(D1 / D0) + 2000.0 / ALPHA / D1);
	
	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/04/28]
// Author       :ZSY
// Parameter(s) :���ڴ�ż������õ�ɢ�����ܶ�.(������Ա�����ڵ���֮ǰ�ȳ�ʼ��)
// Return       :�ɹ�����1,���򷵻�0
// Remark       :���㱣�½ṹɢ�����ܶ�.(�ܵ�.˫��)
//------------------------------------------------------------------
short CCalcThickBase::Calc_QL_PipeCom(double &QL)
{
	//��ñ��κ�ȵ�ɢ�����ܶ�
	//��ʽ:
	//
	QL = 2 * pi * fabs(t - ta) / (log(D1 / D0) / lamda1 + log(D2 / D1) / lamda2 + 2000.0 / ALPHA / D2);
		
	return 1;
}



//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       ::������ȼ���������¶�(��������¶�)��ƽ�浥�㡣
//------------------------------------------------------------------
short CCalcThickBase::CalcPlain_One_InputThick(double thick_resu, double &ts_resu)
{
	thick = thick_resu;
	//����������¶�.
	CalcPlain_One_TsTemp(thick,ts_resu);
	//ɢ���ܶ�
	Calc_Q_PlainOne(dQ);

	return 1;
}	

//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :thick1_resu--�ڲ㱣�º��
// Parameter(s) :thick2_resu--��㱣�º��
// Return       :
// Remark       ::������ȼ���������¶�(��������¶�)�����¶���Ϊ�ܵ���˫��
//------------------------------------------------------------------
short CCalcThickBase::CalcPipe_Com_InputThick(double thick1_resu, double thick2_resu, double &tb_resu, double &ts_resu)
{
	thick1 = thick1_resu;
	thick2 = thick2_resu;
	//����������¶Ⱥͽ����¶�.
	CalcPipe_Com_TsTb(thick1,thick2,ts_resu,tb_resu);
	//D2���Ᵽ�²��⾶,�����¶�ʱ�Ѹ�ֵ�������ϵ��
	ALPHA = GetPipeAlpha(ts_resu, D2);
	//ɢ���ܶ�
	Calc_Q_PipeCom(dQ);

	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       ::������ȼ���������¶�(��������¶�)�����¶���Ϊ�ܵ�������
//------------------------------------------------------------------
short CCalcThickBase::CalcPipe_One_InputThick(double thick_resu, double &ts_resu)
{
	thick = thick_resu;
	//����������¶�.
	CalcPipe_One_TsTemp(thick, ts_resu);

	// 
	ALPHA = GetPipeAlpha(ts_resu, D1);
	//����ɢ���ܶ�
	Calc_Q_PipeOne(dQ);

	return 1;
}
//------------------------------------------------------------------                
// DATE         :[2005/04/22]
// Author       :ZSY
// Parameter(s) :thick1_resu--�ڲ㱣�º��
// Parameter(s) :thick2_resu--��㱣�º��
// Return       :
// Remark       :������ȼ���������¶�(��������¶�)��ƽ��˫�㡣
//------------------------------------------------------------------
short CCalcThickBase::CalcPlain_Com_InputThick(double thick1_resu, double thick2_resu, double &tb_resu, double &ts_resu)
{
	CString	strTmp;

	thick1 = thick1_resu;
	thick2 = thick2_resu;		//����ָ���ĺ��.
	
	//����ָ���ĺ�ȼ���������¶Ⱥͽ����¶�
	CalcPlain_Com_TsTb(thick1,thick2,ts_resu,tb_resu);
	
	//����ɢ���ܶ�
	Calc_Q_PlainCom(dQ);
	
	//����ý����¶���������ɢ���ܶ�
	if( GetHeatLoss(m_pRsHeatLoss, t, nYearVal, nSeasonVal, QMax) )
	{
		if ( dQ > QMax )
		{
			//��ǰ�����¶��µ�ʵ��ɢ���ܶ�,������������ɢ���ܶ�.
			strTmp.Format("ԭʼ�������Ϊ%d�ļ�¼,�����¶�(%.1f��)�µ�ʵ��ɢ���ܶȴ�����������ɢ���ܶ�\r\n", m_nID,t);
			strExceptionInfo += strTmp;
		}
	}


	return 1; 
}

//------------------------------------------------------------------                
// DATE         :[2005/05/26]
// Author       :ZSY
// Parameter(s) :
// Return       :���û����ʾ�ַ����򷵻�0�����򷵻�1
// Remark       :��ü��㵱ǰ��¼ʱ�����ڴ�����ʾ�򾯸�
//------------------------------------------------------------------
short CCalcThickBase::GetExceptionInfo(CString &strInfo)
{
	if ( strExceptionInfo.IsEmpty() )
	{
		return 0 ;
	}
	else
	{
		strInfo = strExceptionInfo;
		return	1;
	}

}

//------------------------------------------------------------------
// DATE         :[2005/12/21]
// Parameter(s) :t  �����¶�
//				:ts	������¶�
// Return       :���²��ϵĵ���ϵ��
// Remark       : ���㱣�²��ϵĵ���ϵ��
//------------------------------------------------------------------
double CCalcThickBase::GetLamda(double t, double ts)
{
	double dLamda;//����ϵ��
	dLamda = in_a0 + in_a1 * ( t + ts ) / 2.0 + in_a2 * ( t + ts ) * ( t + ts ) / 4.0;
	if ( t < ta )
	{	// ���Ϊ�������ʱ������ϵ������һ����ֵ
		in_ratio = ( in_ratio <= 0 ) ? 1 : in_ratio;
		dLamda *= in_ratio;
	}

	return dLamda;
}


//------------------------------------------------------------------                
// DATE         :[2005/06/06]
// Author       :ZSY
// Parameter(s) :t--�����¶�
// Parameter(s) :tb--˫��ʱ�����¶�,����Ϊ������¶�
// Return       :
// Remark       :���㵼��ϵ��
//------------------------------------------------------------------
double CCalcThickBase::GetLamda1(double t, double tb)
{
	double dLamda;
	dLamda = in_a0 + in_a1 * ( t + tb ) / 2.0 + in_a2 * ( t + tb ) * ( t + tb ) / 4.0;
	if ( t < ta )
	{	// ���Ϊ�������ʱ������ϵ������һ����ֵ
		in_ratio = ( in_ratio <= 0 ) ? 1 : in_ratio;
		dLamda *= in_ratio;
	}
	return dLamda;
}

// ��õ���ϵ����nMark=0���ڱ��²��ϻ򵥲�ģ� 1Ϊ�Ᵽ�²���
double CCalcThickBase::GetLamdaA(double t1, double t2, short nMark)
{
	double dLamda;
	double dRatio;
	switch(nMark)
	{
	case 1:
		dRatio = ( out_ratio <= 0 ) ? 1 : out_ratio;
		dLamda = out_a0 + out_a1 * (t1 + t2) / 2.0  + out_a2 * (t1 + t2) * (t1 + t2) / 4.0;
		break;
	default:	// nMark == 0
		dRatio = ( in_ratio <= 0 ) ? 1 : in_ratio;
		dLamda = in_a0 + in_a1 * (t1 + t2) / 2.0 + in_a2 * (t1 + t2) * (t1 + t2) / 4.0;
	}
	if (t < ta)
	{
		dLamda *= dRatio;
	}
	
	return dLamda;

}
//------------------------------------------------------------------                
// DATE         :[2005/06/06]
// Author       :ZSY
// Parameter(s) :tsΪ������¶�,tb�����¶�
// Return       :
// Remark       :���㵼��ϵ��
//------------------------------------------------------------------
double CCalcThickBase::GetLamda2(double tb, double ts)
{
	double dLamda;
	dLamda = out_a0 + out_a1 * ( tb + ts ) / 2.0 + out_a2 * ( tb + ts ) * ( tb + ts ) / 4.0;
	if ( t < ta )
	{	// ���Ϊ�������ʱ������ϵ������һ����ֵ
		out_ratio = ( out_ratio <= 0 ) ? 1 : out_ratio;
		dLamda *= out_ratio;
	}
	return dLamda;
}

//------------------------------------------------------------------
// DATE         :[2005/08/15]
// Parameter(s) :ts:������¶�.
// Parameter(s) :D1:��Ϊ˫��ܵ�ʱΪ���������⾶,����Ϊ�ڲ���⾶. ƽ��ķ���ϵ��ʱD1Ϊ0
// Return       :
// Remark       :���ݲ�ͬ����ҵ�淶,���ƽ��˫��ķ���ϵ��
//				:�����������ļ������ϵ��ʹ�ù��ұ�׼��
//------------------------------------------------------------------
double CCalcThickBase::GetPlainAlpha(double ts, double D1)
{
	double dAlpha=co_alf;
	InitCalcAlphaData(ts);
	if (EDIBgbl::iCur_CodeKey == nKey_CODE_GB8175_2008 || EDIBgbl::iCur_CodeKey == nKey_CODE_CJJ34_2002) //���ұ�׼�ͳ�����������׼
	{
		if (nMethodIndex == nEconomicalThicknessMethod || nMethodIndex == nHeatFlowrateMethod)
		{
			//�ڽ��о��ú�ȷ����������ɢ����ʧ����
			CCalcAlpha_CodeGB::CalcEconomy_Alpha(dAlpha);
			
		}else if (nMethodIndex == nSurfaceTemperatureMethod)
		{

			if (PREVENT_SCALD == nTaIndex)
			{	//������
				CCalcAlpha_CodeGB::CalcPreventScald_Alpha(dAlpha);
				
			}else
			{	//�����¶ȷ�
				CCalcAlpha_CodeGB::CalcEconomy_Alpha(dAlpha);
			}			

		}else if (nMethodIndex == nPreventCongealMethod)
		{
			//�������㷨������ȡ����������ƽ������.
			CCalcAlpha_CodeGB::CalcEconomy_Alpha(dAlpha);
			
		}else
		{
			//Ĭ�ϵĹ�ʽ
			CCalcAlpha_CodeGB::CalcEconomy_Alpha(dAlpha);
		}
//		
	}else if (EDIBgbl::iCur_CodeKey == nKey_CODE_SH3010_2000)//ʯ�ͻ�����׼
	{
		//�ڱ��½ṹ������¶ȼ����С�

		if (nMethodIndex == nSurfaceTemperatureMethod)
		{
			CCalcAlpha_CodePC::CalcFaceTemp_Alpha(dAlpha);
		}else //if (nMethodIndex == nEconomicalThicknessMethod || nMethodIndex == nHeatFlowrateMethod)
		{	//�ھ��ú�ȼ��㼰ɢ����ʧ����ʱ
			CCalcAlpha_CodePC::CalcEconomy_Alpha(dAlpha);

		}
	}else											//Ĭ��Ϊ������׼
	{	
		if (!strPlace.CompareNoCase("����"))
		{
			dAlpha = co_alf;			//��װ�ص�Ϊ����ʱ,����ϵ���ñ��е�ֵ
		}
		else
		{
			CCalcAlpha_Code::CalcPlain_Alpha(dAlpha);
		}
	}
	return dAlpha;
}

//------------------------------------------------------------------
// DATE         :[2005/08/16]
// Parameter(s) :ts:������¶�.
// Parameter(s) :D1:��Ϊ˫��ܵ�ʱΪ���������⾶,����Ϊ�ڲ���⾶. ƽ��ķ���ϵ��ʱD1Ϊ0
// Return       :
// Remark       :���ݲ�ͬ����ҵ�淶,���ƽ�浥��ķ���ϵ��
//				:�����������ļ������ϵ��ʹ�ù��ұ�׼��
//------------------------------------------------------------------
double CCalcThickBase::GetPipeAlpha(double ts, double D1)
{
	double dresAlpha = co_alf;
	InitCalcAlphaData(ts);
	if (EDIBgbl::iCur_CodeKey == nKey_CODE_GB8175_2008 || EDIBgbl::iCur_CodeKey == nKey_CODE_CJJ34_2002)//���ұ�׼��׼�������������׼
	{
		//ƽ����ܵ��ķ���ϵ���Ĺ涨��һ���ġ�
		dresAlpha = GetPlainAlpha(ts,D1);

	}else if (EDIBgbl::iCur_CodeKey == nKey_CODE_SH3010_2000)//ʯ�ͻ�����׼
	{
		//ƽ����ܵ��ķ���ϵ���Ĺ涨��һ���ġ�
		dresAlpha = GetPlainAlpha(ts,D1);
	}else										
	{	//Ĭ��Ϊ������׼		
		if (-1 != strPlace.Find("����",0))
		{
			dresAlpha = co_alf;		//��װ�ص�Ϊ����ʱ���ñ��еķ���ϵ��
		}
		else
			
		{
			CCalcAlpha_Code::CalcPipe_Alpha(D1, dresAlpha);
		}
	}

	return dresAlpha;
}

//------------------------------------------------------------------
// DATE         :[2005/08/17]
// Parameter(s) :ts:������¶�.
// Parameter(s) :D1:��Ϊ˫��ܵ�ʱΪ���������⾶,����Ϊ�ڲ���⾶. ƽ��ķ���ϵ��ʱD1Ϊ0
// Return       :
// Remark       :��ʼ�������ϵ���ı���
//				:�����������ļ������ϵ��ʹ�ù��ұ�׼��
//------------------------------------------------------------------
BOOL CCalcThickBase::InitCalcAlphaData(double ts) 
{
	if (EDIBgbl::iCur_CodeKey == nKey_CODE_GB8175_2008 || EDIBgbl::iCur_CodeKey == nKey_CODE_CJJ34_2002)
	{ 
		//���ұ�׼�ͳ�����������׼ 
		INIT_ALPHADATA(CCalcAlpha_CodeGB);
	}else if (EDIBgbl::iCur_CodeKey == nKey_CODE_SH3010_2000) 
	{ 
		//ʯ�ͻ��� 
		INIT_ALPHADATA(CCalcAlpha_CodePC); 
	}else 
	{
		//Ĭ��Ϊ����
		INIT_ALPHADATA(CCalcAlpha_Code);
	}
	return TRUE;
}

//------------------------------------------------------------------                
// DATE         :[2010/07/09]
// Author       :LIGB
// Parameter(s) :
// Return       :
// Remark       :���ݽ����¶�,���ȡ���½ṹ������������ɢ���ܶ�
//------------------------------------------------------------------
float CCalcThickBase::GetMaxHeatLoss(_RecordsetPtr &pRs, double &t, CString strFieldName)
{
	try
	{
		float fTempCur, fTempPre, fValCur, fValPre, fVal;
		bool bFirst=true;
		for(pRs->MoveFirst(); !pRs->adoEOF; pRs->MoveNext() )
		{
			fTempCur = vtof(pRs->GetCollect(_variant_t("t")));
			fValCur = vtof(pRs->GetCollect(_variant_t(strFieldName))); 
			
			if( DZero > fValCur )
			{//���ֶ�ֵΪ0��գ�����ǰһ����¼�������ֵ�ļ�¼
				fVal = fValPre;
				break;
			}
			else
			{//�ò�ֵ�����������ܶ�
				if( fTempCur >= t )
				{
					if(!bFirst)
					{//��ǰ��¼���¶�ֵ���ڻ����Ҫ���ҵ��¶�.
						fVal = (t-fTempPre)/(fTempCur-fTempPre)*(fValCur-fValPre)+fValPre;					
					}
					else
						fVal=fValCur;
					break;
				}
				else
				{	//��ס��ǰ��ֵ
					fTempPre = fTempCur;
					fValPre = fValCur;
				}
			}
			bFirst=false;
		}
			
		if( pRs->adoEOF )
		{//�������һ����¼
			fVal = fValCur;
		}
		
		return fVal;
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return 0;
	}	

}
