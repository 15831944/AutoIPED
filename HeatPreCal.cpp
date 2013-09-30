// HeatPreCal.cpp: implementation of the CHeatPreCal class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "AutoIPED.h"
#include "HeatPreCal.h"
#include "ParseExpression.h"
#include "vtot.h"
#include <math.h>
#include <stdlib.h>
#include "AutoIPEDView.h"
#include "EDIBgbl.h"
#include "EditOriginalData.h"

#include "CalcThickBase.h"
#include "uematerial.h"

//�򿪲��Ͽ⡣��������Ӧ�ã�����ģ��������ϵ��
//extern BOOL InputMaterialParameter(CString strDlgCaption, CString strMaterialName, int TableID);
extern CAutoIPEDApp theApp;
BOOL InputMaterialParameter(CString strDlgCaption, CString strMaterialName, int TableID)
{
	// ʹ�ö�̬�������ʾ
	CUeMaterial MatDlg;

	MatDlg.SetCodeID(1);	// �淶
	MatDlg.SetMaterialTableToOpen( TableID );
	MatDlg.SetDlgCaption(strDlgCaption);	// �Ի������
	MatDlg.SetShowMaterial(TRUE);		// ��ʾ�¾ɲ��϶��ձ�
	MatDlg.SetMaterialConnection( theApp.m_pConMaterial );	// �������ݿ������
	MatDlg.SetCurMaterialName( strMaterialName );	// ��ǰ��ʾ�Ĳ���
	MatDlg.AddCustomMaterialToDB( NULL );

	return TRUE;
}

//#import"c:\program files\common files\system\ado\msado15.dll"	\
//		no_namespace rename("EOF","adoEOF")

#define GRAVITY_ACCELERATION 9.807
//#define D_MAXTEMP	50000.0		//

#define OPENATABLE(INTERFACE,TABLENAME)						\
		_RecordsetPtr INTERFACE;							\
		OpenAProjectTable(INTERFACE,#TABLENAME);			\


#define OPENASSISTANTPROJECTTABLE(INTERFACE,TABLENAME)						\
		_RecordsetPtr INTERFACE;											\
		OpenAssistantProjectTable(INTERFACE,#TABLENAME);					\

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHeatPreCal::CHeatPreCal()
{
	m_AssistantConnection = NULL;

	m_ProjectName=_T("");
	ch_cal=2;
}

CHeatPreCal::~CHeatPreCal()
{

}

//------------------------------------------------------------------                
// DATE         :[2005/04/29]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :����ǰҪ�����ԭʼ���ݸ���ѡ��ļ��㷽�������Ա����.
//------------------------------------------------------------------
void CHeatPreCal::InitOriginalToMethod()
{
	
#define	INITDATA(CLASSN)		CLASSN::D0 = D0;\
	CLASSN::in_mat		= in_mat;\
	CLASSN::out_mat		= out_mat;\
	CLASSN::pro_mat		= pro_mat;\
	CLASSN::pro_name	= pro_name;\
	CLASSN::in_a0		= in_a0;\
	CLASSN::in_a1		= in_a1;\
	CLASSN::in_a2		= in_a2;\
	CLASSN::in_ratio	= in_ratio;\
	CLASSN::out_a0		= out_a0;\
	CLASSN::out_a1		= out_a1;\
	CLASSN::out_a2		= out_a2;\
	CLASSN::out_ratio	= out_ratio;\
	CLASSN::ts			= temp_ts;\
	CLASSN::tb			= temp_tb;\
	CLASSN::ta			= Temp_env;\
	CLASSN::t			= Temp_pip;\
	CLASSN::Mhours		= Mhours;\
	CLASSN::Yong		= Yong;\
	CLASSN::heat_price	= heat_price;\
	CLASSN::in_price	= in_price;\
	CLASSN::out_price	= out_price;\
	CLASSN::pro_price	= pro_price;\
	CLASSN::S			= S;\
	CLASSN::in_dens		= in_dens;\
	CLASSN::out_dens	= out_dens;\
	CLASSN::pro_dens	= pro_dens;\
	CLASSN::speed_wind	= speed_wind;\
	CLASSN::m_nID		= m_nID;\
	CLASSN::co_alf		= co_alf;\
	CLASSN::ALPHA		= co_alf;\
	CLASSN::strPlace	= strPlace;\
	CLASSN::m_HotRatio	= m_HotRatio;\
	CLASSN::m_CoolRatio	= m_CoolRatio;\
	CLASSN::MaxTs		= MaxTs;\
	CLASSN::hour_work	= hour_work;\
	CLASSN::hedu		= hedu;\
	CLASSN::m_dMaxD0	= m_dMaxD0;\
	CLASSN::distan		= distan;\
	CLASSN::in_tmax		= in_tmax;\
	CLASSN::m_MaxHeatDensityRatio=m_HotRatio;\
	CLASSN::m_thick1Start		= m_thick1Start;\
	CLASSN::m_thick1Max			= m_thick1Max;\
	CLASSN::m_thick1Increment	= m_thick1Increment;\
	CLASSN::m_thick2Start		= m_thick2Start;\
	CLASSN::m_thick2Max			= m_thick2Max;\
	CLASSN::m_thick2Increment	= m_thick2Increment;\
	CLASSN::m_pRsHeatLoss		= m_pRsHeatLoss;\
	CLASSN::m_pRsHeat_alfa		= m_pRsHeat_alfa;\
	CLASSN::m_pRsCongeal		= m_pRsCongeal;\
	CLASSN::m_pRsSubterranean	= m_pRsSubterranean;\
	CLASSN::m_pRsFaceResistance	= m_pRsFaceResistance;\
	CLASSN::m_pRsLSo			= m_pRsLSo;\
	CLASSN::thick				= thick_2;\
	CLASSN::thick1				= thick_1;\
	CLASSN::thick2				= thick_2;\
	CLASSN::pi_thi				= pi_thi;\
	CLASSN::dQ					= 0.0;\
	CLASSN::nMethodIndex		= nMethodIndex;\
	CLASSN::nAlphaIndex			= m_nIndexAlpha;\
	CLASSN::nTaIndex			= nTaIndex;\
	CLASSN::B					= B;\
	CLASSN::strExceptionInfo	= "";


/*
//�����ᱣ�¼���ʱ�������ӵ�һЩ����.
#define INITCONGEALDATA(CLASSN)			\
		CLASSN::Kr		=	Kr;\
		CLASSN::taofr	=	taofr;\
		CLASSN::tfr		=	tfr;\
		CLASSN::V		=	V;\
		CLASSN::ro		=	ro;\
		CLASSN::C		=	C;\
		CLASSN::Vp		=	Vp;\
		CLASSN::rop		=	rop;\
		CLASSN::Cp		=	Cp;\
		CLASSN::Hfr		=	Hfr;\
		CLASSN::dFlux	=	dFlux;
*/

}

//���ݺ�ȼ���������¶�.
void CHeatPreCal::temp_plain_one(double &thick_t, double &ts)
{
	double ts1,condu_r_t,condu_out_t;

	ts=(Temp_pip-Temp_env)/2.0+Temp_env;

	while(TRUE)
	{
		condu_r_t=in_a0+in_a1*(ts+Temp_pip)/2.0+in_a2*(ts+Temp_pip)*(ts+Temp_pip)/4.0;

		condu_out_t=co_alf;
		ts1=thick_t*Temp_env/1000.0/condu_r_t+Temp_pip/condu_out_t;
		ts1=ts1/(thick_t/1000.0/condu_r_t+1.0/condu_out_t);
		
		if(fabs(ts-ts1)<0.1)
			break;
		ts=ts1;
	}

	return;
}



void CHeatPreCal::temp_pip_one(double &thick_t, double &ts)
{
	double ts1,condu_r_t,condu_out_t,D1_t;

	ts=(Temp_pip-Temp_env)/2.0+Temp_env;
	D1_t=D0+thick_t*2.0;
	while(TRUE)
	{
//		* the following line is corrected in next step
		condu_out_t=co_alf;
		condu_r_t=in_a0+in_a1*(ts+Temp_pip)/2.0+in_a2*(ts+Temp_pip)*(ts+Temp_pip)/4.0;
		ts1=1.0/condu_r_t*log(D1_t/D0)*Temp_env+2000.0*Temp_pip/condu_out_t/D1_t;
		ts1=ts1/(log(D1_t/D0)/condu_r_t+2000.0/condu_out_t/D1_t);
		if (fabs(ts-ts1)<0.1)
			break;
		ts=ts1;
	}
	return;
}

void CHeatPreCal::temp_plain_com(double &thick1_t, double &thick2_t, double &ts, double &tb)
{
	double tb1,ts1,condu_out_t,condu_r1_t;
	double condu_r2_t;

	ts=(Temp_pip-Temp_env)/3.0+Temp_env;
	tb=(Temp_pip-Temp_env)/3.0*2.0+Temp_env;
	while(TRUE)
	{
	//	* the following line is corrected in next step
		condu_out_t=co_alf;
		condu_r1_t=in_a0+in_a1*(Temp_pip+tb)/2.0+in_a2*(Temp_pip+tb)*(Temp_pip+tb)/4.0;
		condu_r2_t=out_a0+out_a1*(tb+ts)/2.0+out_a2*(tb+ts)*(tb+ts)/4.0;
		
		ts1=thick1_t*Temp_env/1000.0/condu_r1_t+thick2_t*Temp_env/1000.0/condu_r2_t+Temp_pip/condu_out_t;
		ts1=ts1/(thick1_t/1000.0/condu_r1_t+thick2_t/1000.0/condu_r2_t+1.0/condu_out_t);
		tb1=thick1_t*Temp_env/1000.0/condu_r1_t+thick2_t*Temp_pip/1000.0/condu_r2_t+Temp_pip/condu_out_t;
		tb1=tb1/(thick1_t/1000.0/condu_r1_t+thick2_t/1000.0/condu_r2_t+1.0/condu_out_t);
		
		if ((fabs(ts-ts1)<0.1)&&(fabs(tb-tb1)<0.1))
			break;
		ts=ts1;
		tb=tb1;
	}
	return;
}

void CHeatPreCal::temp_pip_com(double &thick1_t, double &thick2_t, double &ts, double &tb)
{
	double tb1,ts1,condu_r1_t,condu_out_t,D1_t,D2_t;
	double condu_r2_t;

	ts=(Temp_pip-Temp_env)/3.0+Temp_env;
	tb=(Temp_pip-Temp_env)/3.0*2.0+Temp_env;
	D1_t=D0+2.0*thick1_t;
	D2_t=D1_t+2.0*thick2_t;

	while(TRUE)
	{
//		* the following line is corrected in next step
		condu_out_t=co_alf;
		condu_r1_t=in_a0+in_a1*(Temp_pip+tb)/2.0+in_a2*(Temp_pip+tb)*(Temp_pip+tb)/4.0;
		condu_r2_t=out_a0+out_a1*(tb+ts)/2.0+out_a2*(tb+ts)*(tb+ts)/4.0;
		ts1=Temp_env*log(D1_t/D0)/condu_r1_t+Temp_env*log(D2_t/D1_t)/condu_r2_t+2000.0*Temp_pip/condu_out_t/D2_t;
		ts1=ts1/(log(D1_t/D0)/condu_r1_t+log(D2_t/D1_t)/condu_r2_t+2000.0/condu_out_t/D2_t);
		tb1=Temp_env*log(D1_t/D0)/condu_r1_t+Temp_pip*log(D2_t/D1_t)/condu_r2_t+2000.0*Temp_pip/condu_out_t/D2_t;
		tb1=tb1/(log(D1_t/D0)/condu_r1_t+log(D2_t/D1_t)/condu_r2_t+2000.0/condu_out_t/D2_t);
		if ((fabs(ts-ts1)<0.1)&&(fabs(tb-tb1)<0.1))
			break;
		
		ts=ts1;
		tb=tb1;
	}
	return;


}


//////////////////////////////////////////
//ģ �� ��: support.prg            //
//��    ��: ����֧���ܼ��         //
//�ϼ�ģ��: C_ANALYS               //
//�¼�ģ��:                        //
//�� �� ��: (6)  a_pipe,           //
//          (8)a_elas			   //
//�޸�����:                        //
///////////////////////////////////////////
void CHeatPreCal::SUPPORT(double &dw, double &s, double &temp, double &wei1,double &wei2,double &lmax, 
						  CString &v, CString &cl,BOOL &pg,CString &st, double &t0)
{
	//�ܾ�,�ں�,�¶�, ��ˮ����,��ˮ����, ���, 
	//����,����,��ˮ��ʶ,��̬�¶�,�����¶�
/*	double d=0,w=0,i=0;
	double elastic;
	double l1,l2;
	double stres_t0=0.0,stres_t1=0.0,stress=0.0,stress3=0.0;
	double circle=0,l3,maxl12;
	CString k,mk;
	CString TempStr;
	_variant_t TempValue;
	CString strDlgCaption;
	int		TableID;
	 
	cl.TrimLeft();
	cl.TrimRight();

	SetMapValue(CString(_T("S")),&S);
	//����¼��IRecPipe��Ϊ��Ա�ġ�ÿ�μ���ֻ��һ�Ρ�
	//Ŀ�ģ�����ٶȡ�		ZSY	2005/4/8
//	OPENASSISTANTPROJECTTABLE(IRecPipe,A_PIPE);

/*	try
	{
		IRecPipe->MoveFirst();
		while(!IRecPipe->adoEOF)
		{
			TempValue=IRecPipe->GetCollect(_variant_t("PIPE_DW"));
			tempVtf = vtof(TempValue);
			if(fabs(tempVtf - dw) > 1E-6)
			{
				IRecPipe->MoveNext();
				continue;
			}

			TempValue=IRecPipe->GetCollect(_variant_t("PIPE_S"));
			tempVtf = vtof(TempValue);
			if(fabs(tempVtf - s) > 1E-6)
			{
				IRecPipe->MoveNext();
				continue;
			}
			
			GetTbValue(IRecPipe,_variant_t("PIPE_I"),i);
			GetTbValue(IRecPipe,_variant_t("PIPE_W"),w);

			break;
		}

	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV2(e);
		throw;
	}

	if(IRecPipe->adoEOF)
	{
		d=(dw-2*s);
		w=pi*(dw*dw*dw*dw/10000-d*d*d*d/10000)/(dw/10*32);
		i=w*dw/20;

	}

*/
	//�ܵ�������Ծ�(PIPE_I) ����^4
	//�ܵ����濹���(PIPE_W) ����^3
	//�ӹܵ��������в��Ҷ�Ӧ�⾶�ͱں�ļ�¼��
/*	if( 0 == GetPipeWeithAndWaterWeith(IRecPipe,dw,s,"PIPE_I",i,"PIPE_W",w) )
	{
		d=(dw-2*s);
		//�ܵ����濹���(PIPE_W) ����^3
		w=pi*(dw*dw*dw*dw/10000-d*d*d*d/10000)/(dw/10*32);
		//�ܵ�������Ծ�(PIPE_I) ����^4
		i=w*dw/20;
	}
*/
/*

	//AutoIPED7.0�Ѿ������µķ�����������Ӧ��������ģ��
	//�����ö�̬�⺯���� material.mdb�в�������Ӧ��/����ģ��������ĵ�λ��MPa,kN/mm2
	stress3=CCalcDll.GetMaterialSIGMAt(cl,(float)t0,(float)s)/GRAVITY_ACCELERATION;

	CString strDlgCaption, strMaterialName, *pStrArr;
	int		TableID, MatNum;
	if( stress3 < 0 )
	{
		if( FindMaterialNameOldOrNew(cl, pStrArr, MatNum) ) //�����¾ɲ��϶��ձ�.
		{
			strMaterialName = pStrArr[0];
			delete[] pStrArr;
		}
		else
		{
			strMaterialName = cl;
		}
		TableID = 0;
		strDlgCaption.Format(_T("������ %s �ڻ����¶��µ�����Ӧ��(MPa):"),strMaterialName);
		//��������
		InputMaterialParameter(strDlgCaption,  strMaterialName,  TableID);
		//����һ��
		stress3=CCalcDll.GetMaterialSIGMAt(cl,(float)t0,(float)s)/GRAVITY_ACCELERATION;
	}

	stress=CCalcDll.GetMaterialSIGMAt(cl,(float)temp,(float)s)/GRAVITY_ACCELERATION;
	if( stress < 0 )
	{
		if( FindMaterialNameOldOrNew(cl, pStrArr, MatNum) ) //�����¾ɲ��϶��ձ�.
		{
			strMaterialName = pStrArr[0];
			delete[] pStrArr;
		}
		else
		{
			strMaterialName = cl;
		}

		TableID = 0;
		strDlgCaption.Format(_T("������ %s �� %f  ���µ�����Ӧ��(MPa):"),strMaterialName,temp);
//		strMaterialName = cl;
		//��������
		InputMaterialParameter(strDlgCaption,  strMaterialName,  TableID);
		stress=CCalcDll.GetMaterialSIGMAt(cl,(float)temp,(float)s)/GRAVITY_ACCELERATION;
	}
	//
	elastic=CCalcDll.GetMaterialEt(cl,(float)temp)/GRAVITY_ACCELERATION*100000;
	if( elastic < 0 )
	{
		if( FindMaterialNameOldOrNew(cl, pStrArr, MatNum) ) //�����¾ɲ��϶��ձ�.
		{
			strMaterialName = pStrArr[0];
			delete[] pStrArr;
		}
		else
		{
			strMaterialName = cl;
		}

		TableID = 1;
		strDlgCaption.Format(_T("������ %s �� %f  ���µĵ���ģ��(kN/mm2)"),strMaterialName,temp);
//		strMaterialName = cl;
		//��������
		InputMaterialParameter(strDlgCaption,  strMaterialName,  TableID);
		elastic=CCalcDll.GetMaterialEt(cl,(float)temp)/GRAVITY_ACCELERATION*100000;
	}

//	***Ӧ��   stress
//	***����ģ��   elastic
	BOOL IsFind=FALSE;
	//��û�����ݣ����û�����ʱ��ѭ��һ��
	while( 1 )
	{
		//����¼��IRec��Ϊ��Ա�ġ�ÿ�μ���ֻ��һ�Ρ�
		//Ŀ�ģ�����ٶȡ�		ZSY	2005/4/8
	//	OPENASSISTANTPROJECTTABLE(IRec,A_C09);
		m_pRs_a_c09->MoveFirst();
		if(LocateFor(m_pRs_a_c09,_variant_t("PIPE_MAT"),CFoxBase::EQUAL,_variant_t(cl)))
		{
			try
			{
				GetTbValue(m_pRs_a_c09,_variant_t("PIPE_C"),circle);
				break;
			}
			catch(_com_error &e)
			{
				TempStr.Format(_T("��\"���򺸷�Ӧ��ϵ����(A_C09)\"��PIPE_MAT=%sʱ��ȡ�����д�"),cl);

				Exception::SetAdditiveInfo(TempStr);

				ReportExceptionErrorLV2(e);

				throw;
			}
		}
		else
		{	// ��������Ҳ�����������Ϊ���ϵ��¾�����
			//
			CString *pMatNameArr;
			int Num;
			if(FindMaterialNameOldOrNew(cl,pMatNameArr,Num))
			{
				while(Num>0)
				{
					Num--;
					strMaterialName = pMatNameArr[Num]; //zsy
					if(LocateFor(m_pRs_a_c09,_variant_t("PIPE_MAT"),CFoxBase::EQUAL,_variant_t(pMatNameArr[Num])))
					{
						try
						{
							GetTbValue(m_pRs_a_c09,_variant_t("PIPE_C"),circle);
							IsFind=TRUE;
							break;
						}
						catch(_com_error &e)
						{
							TempStr.Format(_T("��\"���򺸷�Ӧ��ϵ����(A_C09)\"��PIPE_MAT=%sʱ��ȡ�����д�"),cl);
							Exception::SetAdditiveInfo(TempStr);
							ReportExceptionErrorLV2(e);
							throw;
						}
					}
				}
				delete[] pMatNameArr;
			}
			if(IsFind==FALSE)
			{
				if( strMaterialName.IsEmpty() )
				{
					strMaterialName = cl;
				}
				TableID = 3;
				strDlgCaption.Format(_T("������ %s �Ļ��򺸷�Ӧ��ϵ��:"),strMaterialName);
				//��������
				InputMaterialParameter(strDlgCaption,  strMaterialName,  TableID);	
				IsFind = TRUE;
			}
			else
			{
				break;
			}
		}
	} //while(1)

	if(st==_T("ˮ��"))
	{

		if(pg==_T("Y") || pg==_T("y"))
		{
			l1=sqrt(w*circle*stress/wei2)*2.0;
		}
		else
		{
			l1=sqrt(w*circle*stress/wei2)*2.24;
		}
		l2=0.0241*pow((elastic*i/wei2),(1.0/3.0));
		lmax=min(l1,l2);
	}
	else if(st==_T("����"))
	{
		if(pg==_T("Y") || pg==_T("y"))
		{
			l1=sqrt(w*circle*stress/wei1)*2.0;
			l3=sqrt(w*circle*stress3/wei2)*2.0;
		}
		else
		{
			l1=sqrt(w*circle*stress/wei1)*2.24;
			l3=sqrt(w*circle*stress3/wei2)*2.24; 
		}
		l2=0.0241*pow((elastic*i/wei1),(1.0/3.0));
		maxl12=min(l1,l2);
		lmax=min(maxl12,l3);
	}
	else
	{
	    lmax=0;
	}
*/
}	

//////////////////////////////////////
//economic Pip thickness calculation//
//////////////////////////////////////

void CHeatPreCal::pip_com(double &thick1_resu, double &thick2_resu, 
						  double &tb_resu, double &ts_resu)
{
	double condu1_r,condu2_r,condu_out,cost_q,cost_s,cost_tot,cost_min,thick1,thick2,tb,ts;
	double D1,D2;


//	double MaxTb = in_tmax*m_HotRatio;	//�������������¶�=�Ᵽ�²��ϵ�����¶� * һ��ϵ��(��̹涨Ϊ0.9).
	double  MaxTb = in_tmax;//�������������¶�,�ӱ��²��ϲ�����(a_mat)��ȡMAT_TMAX�ֶ�,���޸ĸ��ֶο��ƽ����¶�ֵ.

	double nYearVal, nSeasonVal, Q;
	double QMax;		//�������ɢ���ܶ�
	bool   flg=true;	//����ɢ���ܶȵ��ж�

	thick1_resu=0;
	thick2_resu=0;
	cost_min=0;
	
/*  //Ӧ��û���¶ȵ�����
	if (Temp_pip<350.0)
	{
		return;
	}
*/
	//ѡ����ѡ�����ж�ɢ���ܶ�,����
	if(bIsHeatLoss)
	{
		//����ý����¶���������ɢ���ܶ�
		if( !GetHeatLoss(m_pRsHeatLoss, Temp_pip, nYearVal, nSeasonVal, QMax) )
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
		//	do temp_pip_com with thick1,thick2,ts,tb 
			temp_pip_com(thick1,thick2,ts,tb);
			condu1_r=in_a0+in_a1*(Temp_pip+tb)/2.0+in_a2*(Temp_pip+tb)*(Temp_pip+tb)/4.0;
			condu2_r=out_a0+out_a1*(tb+ts)/2.0+out_a2*(tb+ts)*(tb+ts)/4.0;
			condu_out=co_alf;
			D1=2*thick1+D0;
			D2=2*thick2+D1;

			cost_q=7.2*pi*Mhours*heat_price*Yong*fabs(Temp_pip-Temp_env)*1e-6
					/(1.0/condu1_r*log(D1/D0)+1.0/condu2_r*log(D2/D1)+2000.0/condu_out/D2);

			cost_s=(pi/4.0*(D1*D1-D0*D0)*in_price*1e-6+pi/4.0*(D2*D2-D1*D1)*out_price*1e-6
					+pi*D2*pro_price*1e-3)*S;

			cost_tot=cost_q+cost_s;

			//tb<350)
			if((ts<MaxTs && tb<MaxTb) && (fabs(cost_min)<1E-6 || cost_min>cost_tot))
			{
				flg = true;
				//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.
				if(bIsHeatLoss)
				{
					//��ñ��κ�ȵ�ɢ���ܶ�
					//��ʽ:
					//(�����¶�-�����¶�)/((�Ᵽ�²��⾶/2000.0)*(1.0/�ڱ��²��ϵ�����*ln(�ڱ��²��⾶/�ܵ��⾶) + 1.0/�Ᵽ�²��ϵ�����*ln(�Ᵽ�²��⾶/�ڱ��²��⾶) ) + 1/����ϵ��)
					Q = (Temp_pip-Temp_env) / ( (D2/2000.0) * (1.0/condu1_r*log(D1/D0)+1.0/condu2_r*log(D2/D1)) + 1.0/co_alf );

					if(fabs(QMax) <= 1E-6 || Q <= QMax)
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
				}
			}
		}
	}
	return;

}


////////////////////////////////////////////////////
// economic plain thickness for complex calculation//
/////////////////////////////////////////////////////

void CHeatPreCal::plain_com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu)
{
	double condu1_r,condu2_r,condu_out,cost_q,cost_s,cost_tot,cost_min,thick1,thick2,tb,ts;
	
//	double MaxTb = in_tmax*m_HotRatio;//�������������¶�=�Ᵽ�²��ϵ�����¶� * һ��ϵ��(��̹涨Ϊ0.9).
	double  MaxTb = in_tmax;//�������������¶�,�ӱ��²��ϲ�����(a_mat)��ȡMAT_TMAX�ֶ�,���޸ĸ��ֶν��п��ƽ����¶�ֵ.

	double MaxTs;	//�������������¶� 
	//���ڷ����˱��£����½ṹ������¶Ȳ�Ӧ����60���϶ȡ� 
	if(	Temp_env <= 27 )
	{
		//�����¶Ȳ�����27���϶�ʱ���豸�͹ܵ����½ṹ������¶Ȳ�Ӧ����50���϶ȣ�
		MaxTs = 50;
	}
	else
	{
		//�����¶ȸ���27���϶�ʱ�����½ṹ������¶ȿɱȻ����¶ȸ�25x���϶ȡ�
		MaxTs = Temp_env + 25;
	}

	double nYearVal, nSeasonVal,
		   Q;			//���½ṹ�����ɢ���ܶ�
	double QMax;		//�������ɢ���ܶ�
	bool   flg=true;	//����ɢ���ܶȵ��ж�

	thick1_resu=0;
	thick2_resu=0;
	cost_min=0;
	//ѡ����ѡ�����ж�ɢ���ܶ�,����
	if(bIsHeatLoss)
	{
		//����ý����¶���������ɢ���ܶ�
		if( !GetHeatLoss(m_pRsHeatLoss, Temp_pip, nYearVal, nSeasonVal, QMax) )
		{		
		}
	}
	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����thicknessRegular���
	for(thick1=m_thick1Start;thick1<=m_thick1Max;thick1+=m_thick1Increment)
	{
		for(thick2=m_thick2Start;thick2<=m_thick2Max;thick2+=m_thick2Increment)
		{
		//	do temp_plain_com with thick1,thick2,ts,tb 
			temp_plain_com(thick1,thick2,ts,tb);
			condu1_r=in_a0+in_a1*(Temp_pip+tb)/2.0+in_a2*(Temp_pip+tb)*(Temp_pip+tb)/4.0;
			condu2_r=out_a0+out_a1*(tb+ts)/2.0+out_a2*(tb+ts)*(tb+ts)/4.0;
			condu_out=co_alf;

			cost_q=3.6*Mhours*heat_price*Yong*fabs(Temp_pip-Temp_env)*1e-6
				/(thick1/1000.0/condu1_r+thick2/1000.0/condu2_r+1.0/condu_out);

			cost_s=(thick1/1000.0*in_price+thick2/1000.0*out_price+pro_price)*S;
			cost_tot=cost_q+cost_s;
			if((ts<MaxTs && tb<MaxTb))	//ts<50 && tb<350
			{
				if(fabs(cost_min)<1E-6 )
				{//��һ��
					flg = true;
					//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.
					if(bIsHeatLoss)
					{
						//��ñ��κ�ȵ�ɢ���ܶ�
						//��ʽ:
						//(�����¶�-�����¶�)/(�ڱ��º�/1000/�ڱ��²��ϵ����� + �Ᵽ�º�/1000/�Ᵽ�²��ϵ����� + 1/����ϵ��)
						Q = (Temp_pip-Temp_env)/(thick1/1000.0/condu1_r+thick2/1000.0/condu2_r+1.0/co_alf);
						if(fabs(QMax) <= 1E-6 || Q <= QMax)
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
					}
			//		InsertToReckonCost(ts,tb,thick1,thick2,cost_tot,true);
				}
				else
				{//���ǵ�һ��
					if(cost_min>cost_tot)
					{
						flg = true;
						//ѡѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.
						if(bIsHeatLoss)
						{
							//��ñ��κ�ȵ�ɢ���ܶ�
							//��ʽ:
							//(�����¶�-�����¶�)/(�ڱ��º�/1000/�ڱ��²��ϵ����� + �Ᵽ�º�/1000/�Ᵽ�²��ϵ����� + 1/����ϵ��)
							Q = (Temp_pip-Temp_env)/(thick1/1000.0/condu1_r+thick2/1000.0/condu2_r+1.0/co_alf);
							if(fabs(QMax) <= 1E-6 || Q <= QMax)
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
							//ǰ�η��ô��ڱ��η���
							cost_min=cost_tot;
							thick1_resu=thick1;
							thick2_resu=thick2;
							ts_resu=ts;
							tb_resu=tb;
						}
					//	InsertToReckonCost(ts,tb,thick1,thick2,cost_tot,false);
					}
					else
					{//ǰ�η���С�ڱ��η���
					}
				}
			}
		}
	}
	return;
}


//////////////////////////////////////
//economic Pip thickness calculation//
//////////////////////////////////////

void CHeatPreCal::pip_one(double &thick_resu, double &ts_resu)
{
	double condu_r,condu_out,cost_q,cost_s,cost_tot,cost_min,thick,ts;
	double D1;
	double nYearVal, nSeasonVal, Q;
	double QMax;		//�������ɢ���ܶ�
	bool   flg=true;	//����ɢ���ܶȵ��ж�

	thick_resu=0;
	ts_resu=0;
	cost_min=0;
	//ѡ����ѡ�����ж�ɢ���ܶ�,����
	if(bIsHeatLoss)
	{
		//����ý����¶���������ɢ���ܶ�
		if( !GetHeatLoss(m_pRsHeatLoss, Temp_pip, nYearVal, nSeasonVal, QMax) )
		{		
		}
	}
	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����thicknessRegular���
	//
	for(thick=m_thick2Start;thick<=m_thick2Max;thick+=m_thick2Increment)
	{
	//	do temp_pip_one with thick,ts
		temp_pip_one(thick,ts);
		condu_r=in_a0+in_a1*(ts+Temp_pip)/2.0+in_a2*(ts+Temp_pip)*(ts+Temp_pip)/4.0;
		condu_out=co_alf;
		D1=2.0*thick+D0;

		cost_q=7.2*pi*Mhours*heat_price*Yong*fabs(Temp_pip-Temp_env)*1e-6
				/(1.0/condu_r*log(D1/D0)+2000.0/condu_out/D1);
 
		cost_s=(pi/4.0*(D1*D1-D0*D0)*in_price*1e-6+pi*D1*pro_price*1e-3)*S;
		cost_tot=cost_q+cost_s;
		if(ts<MaxTs && (cost_tot<cost_min || fabs(cost_min)<1E-6))
		{
			flg = true;
			//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.
			if(bIsHeatLoss)
			{
				//��ñ��κ�ȵ�ɢ���ܶ�
				//��ʽ:
				//(�����¶�-�����¶�)/((���²��⾶/2000.0/���ϵ�����*ln(���²��⾶/�ܵ��⾶)) + 1/����ϵ��)
				Q = (Temp_pip-Temp_env)/(D1/2000.0/condu_r*log(D1/D0) + 1.0/co_alf);
				if(fabs(QMax) <= 1E-6 || Q <= QMax)
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
			}
		}
	}
	return;
}


/////////////////////////////////////////
//economic plain thickness calculation //
//main program                         //
////////////////////////////////////////
void CHeatPreCal::plain_one(double &thick_resu, double &ts_resu)
{
	double condu_r,condu_out,cost_q,cost_s,cost_tot,cost_min,thick,ts;
	double nYearVal, nSeasonVal, Q ;	
	double QMax;		//�������ɢ���ܶ�
	bool   flg=true;	//����ɢ���ܶȵ��ж�

	thick_resu=0;
	ts_resu=0;
	cost_min=0;
	//ѡ����ѡ�����ж�ɢ���ܶ�,����
	if(bIsHeatLoss)
	{
		//����ý����¶���������ɢ���ܶ�
		if( !GetHeatLoss(m_pRsHeatLoss, Temp_pip, nYearVal, nSeasonVal, QMax) )
		{ 		
		}
	}
	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����thicknessRegular���
	//
	for(thick=m_thick2Start;thick<=m_thick2Max;thick+=m_thick2Increment)
	{
		//	do temp_plain_one with thick,ts
		temp_plain_one(thick,ts);
		condu_r=in_a0+in_a1*(ts+Temp_pip)/2.0+in_a2*(ts+Temp_pip)*(ts+Temp_pip)/4.0;
		condu_out=co_alf;

		cost_q=3.6*Mhours*heat_price*Yong*fabs(Temp_pip-Temp_env)*1e-6
				/((thick/1000.0/condu_r)+1.0/condu_out);

		cost_s=(thick/1000*in_price+pro_price)*S;
		cost_tot=cost_q+cost_s;
	
		if((cost_tot<cost_min || fabs(cost_min)<1E-6) && ts<MaxTs)
		{
			flg = true;
			//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.
			if(bIsHeatLoss)
			{
				//��ñ��κ�ȵ�ɢ���ܶ�
				//��ʽ:
				//(�����¶�-�����¶�)/(��ǰ���º�/(1000*���ϵ�����) + 1/����ϵ��)
				Q = (Temp_pip-Temp_env) / (thick / (1000*condu_r) + 1.0 / co_alf); 
				if(fabs(QMax) <= 1E-6 || Q <= QMax)
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
			}
		}
	}
	return;
}

//------------------------------------���¼���------------------------------------------
//////////////////////////////////////////////////////////////
//                        C_ANALYS                         //
/////////////////////////////////////////////////////////////

/////////////////////////////////////
//ģ �� ��: c_analys.prg           //
//��    ��: ���½ṹ���ú�ȼ���   //
//�ϼ�ģ��: MAIN_MENU              //
//�¼�ģ��:  *                     //
//          SUPPORT*			   //
//�� �� ��: (1)pre_calc,           //
//          (3)a_mat,              //
//          (6)a_pipe,             //
//          (8)a_stress,a_elas     //
//�޸�����:                        //
/////////////////////////////////////

void CHeatPreCal::C_ANALYS()
{
	_variant_t TempValue;
	double thick_1,thick_2,temp_ts,temp_ts1,temp_tb;
	double pre_v1,pre_v2,pro_v,pro_wei;
	double pro_conduct;
	double pro_thi,pi_thi;
	double pre_wei1,pre_wei2;
	double d1,d2,d3,p_wei,w_wei,no_w,with_w,face_area;
	double a_config_hour;
	double dAmount;	//����.
	double tempVtf;
	long   rec;
	long   ERR;
	int    IsStop;
	HRESULT hr;
	BOOL	pi_pg=FALSE;	//ѹ����ʶ������3.92MPaΪTRUE������ΪFALSE
	CString tempVts;
	CString pi_mat,mark,vol,steam;
	CString pi_site;
	CString last_mod;
	CString unit;
	CString strPlace;		//��װ�ص㡣�����ڻ����⣩
	CString TempStr;		//��ʱ�ģ��������Ļ���ַ���
	CString cPipeType;		//�ܵ����豸������,Ϊ�������һ���ַ�.
	CString strMedium;		//��������
	int		nMethodIndex=0;	//���㷽��������ֵ
	int		nTaIndex=0;		//�����¶ȵ�ȡֵ����
	bool	bNoCalInThi;	//�Ƿ�����ڱ��²���
	bool	bNoCalPreThi;	//�Ƿ�����Ᵽ�²���
	double  Temp_dew_point = 23;	// ¶���¶�
	double  Temp_coefficient = 1;	// ��ͬ�Ĺ淶���ӵĲ�ͬϵ��
	double  dUnitLoss;
	double  dAreaLoss;
	BOOL	bIsReCalc = FALSE;		// ����Ǳ�����㣬�Ƿ���Ҫ�����ñ����¶ȷ�У��������¶ȵ�ֵ���ͺ��
	
	ch_cal=GetChCal();
	if(ch_cal==4)
	{
		return;
	}

//	*ע��
	OPENATABLE(IRecordset,PASS);

	if(!LocateFor(IRecordset,_variant_t("PASS_MOD1"), 
			CFoxBase::EQUAL,_variant_t("C_ANALYS")))
	{
		TempStr.Format("�޷���PASS���е�\"PASS_MOD1\"�ֶ��ҵ�C_ANALYS \n�п������ݱ�Ϊ�ջ�δ�������");
		ExceptionInfo(TempStr);
		return;
	}
	try
	{
		GetTbValue(IRecordset,_variant_t("PASS_LAST1"),last_mod);
	}
	catch(_com_error &e)
	{
		TempStr.Format(_T("��\"PASS��\"��PASS_MOD1=C_ANALYSʱ��ȡPASS_LAST1�����д�"));
		Exception::SetAdditiveInfo(TempStr);
		ReportExceptionErrorLV2(e);
		throw;
	}

	if(!LocateFor(IRecordset,_variant_t("PASS_MOD1"),
			CFoxBase::EQUAL,_variant_t(last_mod)))
	{
		TempStr.Format("�޷���PASS���е�\"PASS_MOD1\"�ֶ��ҵ�%s \r\n�п������ݱ�Ϊ�ջ�δ�������",last_mod);
		ExceptionInfo(TempStr);

		return;
	}
	TempValue=IRecordset->GetCollect(_variant_t("PASS_MARK1"));
	if(TempValue!=_variant_t("T"))
	{
		this->MessageBox(_T("���ܼ���, �����Ƚ���ԭʼ���ݱ༭!"));
		IRecordset->Close();
		return;
	}
	
	IRecordset->Close();
	CString sql;
    if (ch_cal==1)
	{
		try
		{
			//��C_PASS�ֶ�ȫ����'Y'
			sql = "UPDATE [PRE_CALC] SET C_PASS='Y' WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
			theApp.m_pConnection->Execute(_bstr_t(sql), NULL, adCmdText );

			OPENATABLE(IRecordset2,PRE_CALC);
			if(IRecordset2->adoEOF && IRecordset2->BOF)
			{
				ExceptionInfo(_T("ԭʼ���ݱ�Ϊ��\n"));
				return;
			}
			start_num=1;
			IRecordset2->MoveLast();
			stop_num=RecNo(IRecordset2);     
			IRecordset2->Close();
		}
		catch(_com_error &e)
		{
			ReportExceptionErrorLV2(e);
			throw;
		}
	}
	else if(ch_cal==2)
	{
		try
		{
			//���Ƚ�C_PASS�ֶ�ȫ����ա�
			sql = "UPDATE [PRE_CALC] SET C_PASS='' WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
			theApp.m_pConnection->Execute(_bstr_t(sql), NULL, adCmdText );

			sql = "SELECT * FROM [PRE_CALC] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ORDER BY ID ASC";
			_RecordsetPtr pRsCalc;
			pRsCalc.CreateInstance(__uuidof(Recordset));
			pRsCalc->Open(_variant_t(sql), (IDispatch*)GetConnect(),
					adOpenStatic, adLockOptimistic, adCmdText);
			if(pRsCalc->adoEOF && pRsCalc->BOF)
			{
				ExceptionInfo(_T("ԭʼ���ݱ�Ϊ��\n"));
				return;
			}

			start_num=1;
			pRsCalc->MoveLast();
			stop_num=RecNo(pRsCalc);
			if( FALSE == RangeDlgshow(start_num,stop_num) )
			{
				return;
			}
			//Ȼ����ָ���ķ�Χ�ڣ���C_PASS�ֶθ�'Y'
			ReplaceAreaValue(pRsCalc, "c_pass", "Y", start_num, stop_num);
			pRsCalc->Close();
		}
		catch(_com_error &e)
		{
			ReportExceptionErrorLV2(e);
			throw;
		}

	}
	else if(ch_cal==3)
	{
		try
		{
			_RecordsetPtr IRecordset2;
			IRecordset2.CreateInstance(__uuidof(Recordset));
			int iIsStop=0;
			//���Ƚ�C_PASS�ֶ�ȫ����ա�
			sql = "UPDATE [PRE_CALC] SET C_PASS='' WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
			theApp.m_pConnection->Execute(_bstr_t(sql), NULL, adCmdText );
			//
			sql.Format(_T("SELECT c_pass,id,c_vol,c_name1,c_size,c_pi_thi,c_temp,c_name2,							\
				c_name3,c_pre_thi,c_place,c_mark ,EnginID FROM PRE_CALC where EnginID='%s' ORDER BY id ASC"),GetProjectName());
			
			IRecordset2->CursorLocation = adUseClient;
			
			IRecordset2->Open(_variant_t(sql),_variant_t((IDispatch*)GetConnect()),
				adOpenDynamic,adLockOptimistic,adCmdText);
			SelectToCal(IRecordset2,&iIsStop);
			//MessageBox("��������ļ�¼C_PASS�ֶδ�����'Y'");
			IRecordset2->Close();
			if(iIsStop)
			{
				return;
			}
		}
		catch(_com_error &e)
		{
			ReportExceptionErrorLV2(e);
			throw;
		}
	}
	OPENATABLE(IRecConfig,A_CONFIG);
	
	if(IRecConfig->adoEOF && IRecConfig->BOF)
	{
		ExceptionInfo(_T("������Ƴ�����(A_CONFIG)����Ϊ��\n"));
		return;
	}
	try
	{
		IRecConfig->MoveFirst();
		
		GetTbValue(IRecConfig,_variant_t("��λ��"),unit);
		unit.MakeUpper();
		GetTbValue(IRecConfig,_variant_t("�����ȼ�"),heat_price);
		GetTbValue( IRecConfig, _variant_t("�������"), S ); // by zsy 2007.01.11
		GetTbValue(IRecConfig,_variant_t("������Сʱ"),a_config_hour);
		m_dMaxD0 = vtof(IRecConfig->GetCollect(_variant_t("ƽ����Բ�ܵķֽ��⾶")) );
		if( m_dMaxD0 <= 0 )
		{	//���ֽ��⾶������ʱ,��һ��Ĭ��ֵ.
			m_dMaxD0 = 2000;
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV2(e);
		throw;
	}
	if(unit != _T("SI"))
	{
		//�ڲ���ʽȫ��ʹ�ù��ʵ�λ�Ƽ���,����Ժ����ʽʹ�ù����Ƽ���.
		//������Ժ�ķ���ϵ��ALPHA���õ��ǹ��ʵ�λ��,���߲�һ�¼������д���.
		heat_price=heat_price/4.1868;
	}
	IRecConfig->Close();
	
	if( IRecCalc == NULL )
	{
		hr=IRecCalc.CreateInstance(__uuidof(Recordset));
	}
	TempStr.Format(_T("SELECT * FROM PRE_CALC WHERE EnginID='%s' ORDER BY ID ASC"),this->GetProjectName());
	try
	{
		if(IRecCalc->State == adStateOpen)
		{
			IRecCalc->Close();
		}
		IRecCalc->Open(_variant_t(TempStr),_variant_t((IDispatch*)GetConnect()),
			adOpenStatic,adLockOptimistic,adCmdText);
		
		EDIBgbl::pRsCalc = IRecCalc;				//��ԭʼ���ݵļ�¼������������������ʹ��.
		IRecCalc->MoveFirst();
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV2(e);
		throw;
	}
	OPENATABLE(IRecMat,A_mat);
	if(IRecMat->adoEOF && IRecMat->BOF)
	{
	//	ExceptionInfo(_T("���²��ϲ�����(A_MAT)����Ϊ��\n"));
	//	return;
	}
	//�򿪱�׼�Ĳ��Ͽ�.(������Ϊ�յļ�¼)
	OpenAProjectTable(m_pRs_CodeMat,"A_MAT","");
	//�򿪷�������ʱ�������ݵı�
	OpenAProjectTable(m_pRsCongeal,"pre_calc_congeal",GetProjectName());
	// ����عܵ����¼���ʱ�����ӵ����ݱ�
	OpenAProjectTable(m_pRsSubterranean, "Pre_calc_Subterranean", GetProjectName());
	//
	
	
	//����¼��IRecPipe��Ϊ��Ա�ġ�ÿ�μ���ֻ��һ�Ρ�
	//Ŀ�ģ�����ٶȡ�		
	//�򿪹ܵ������⡣
	if(!OpenAssistantProjectTable(IRecPipe, "A_PIPE") )
	{
		return;
	}
	//�򿪱��½ṹ������������ɢ���ܶ�
	//��IPEDcode.mdb���С�
	if( !OpenAssistantProjectTable(m_pRsHeatLoss, "HeatLoss") )
	{
		return ;
	}
	//�򿪹ܵ�/��������洫��ϵ��.
	if( !OpenAssistantProjectTable(m_pRsCon_Out, "CON_OUT"))
	{
		return ;
	}
	//�򿪼������ϵ��ʱ�ļ��㹫ʽ��
	if( !OpenAssistantProjectTable(m_pRsHeat_alfa, "����ϵ��ȡֵ��") )
	{
		return ;
	}
	//�򿪱������ϵĺڶ�
	if( !OpenAssistantProjectTable(m_pRs_a_Hedu, "a_hedu") )
	{
		return ;
	}
	// �����ĵ���ϵ��
	if( !OpenAssistantProjectTable(m_pRsLSo, "�����ĵ���ϵ��"))
		return;

/*
	// ���²����������Χ�����ķ�����
	if ( !OpenAssistantProjectTable(m_pRsFaceResistance, "Resistance") )
	{
		return;
	}
*/
	
	// ������������¶���¶�
	GetWeatherProperty( "Td_DewPoint", Temp_dew_point );

	CCalcThickBase *lpMethodClass;		// ָ��ͬ������Ӧ����
	bool	bIsPlane;	 	// ���¶������͵ı�־��true--ƽ��,false--�ܵ�
	
	//���
	//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	//�ӱ��º�ȹ����thicknessRegular����ȡ�������ֵ��
	if( !GetThicknessRegular() )
	{
		return ;
	}

	//���½����¶Ȳ�Ӧ������㱣�²������ʹ���¶ȵı���.	
	if(	!this->GetConstantDefine("ConstantDefine", "Ratio_MaxHotTemp", TempStr) )
	{
		m_HotRatio = 1;	//Ĭ�ϵ�ֵ
	}else
	{
		m_HotRatio = _tcstod(TempStr, NULL);
	}
	//��������¶Ȳ�Ӧ������㱣��������ʹ���¶ȵı���.   
	if(	!this->GetConstantDefine("ConstantDefine", "Ratio_MaxCoolTemp", TempStr) )
	{
		m_CoolRatio = 1;	//Ĭ�ϵ�ֵ
	}else
	{
		m_CoolRatio = _tcstod(TempStr, NULL);
	}
	// �������������¶�
	if (!GetConstantDefine("ConstantDefine","FaceMaxTemp",TempStr))
	{
		MaxTs = 50;
	}else
	{
		MaxTs = strtod(TempStr,NULL);
	}

	// ����ʱ,�涨Ϊ¶���¶����ӵ�һ������
	if ( !GetConstantDefine( "ConstantDefine", "DeltaTsd", Temp_coefficient ) )
	{
		Temp_coefficient = 1;
	}

	// ����������Զ�ѡ���½ṹ
	if( giInsulStruct == 1 )
	{
		if(IDYES==AfxMessageBox(IDS_REALLYAUTOSELALLMAT,MB_YESNO+MB_DEFBUTTON2+MB_ICONQUESTION))
		{
			CEditOriginalData  Original;
			Original.SetCurrentProjectConnect(theApp.m_pConnection);
			Original.SetPublicConnect(theApp.m_pConnectionCODE);
			Original.SetProjectID(EDIBgbl::SelPrjID);

			if( !Original.AutoSelAllMat() )
			{
				return;
			}
		}
	}
	MinimizeAllWindow();	// ��С�����е��������

	Say(_T("��ʼ���¼��� "));
	
	//	����ѭ����
	while(!IRecCalc->adoEOF)
	{
		try
		{
			GetTbValue(IRecCalc,_variant_t("c_pass"),TempStr);
			if( TempStr.CompareNoCase("Y") )
			{
				IRecCalc->MoveNext();
				continue;
			}
			//ԭʼ���ݵ����
			m_nID = vtoi(IRecCalc->GetCollect(_variant_t("ID")));
			
			// ***���йز��ϼ۸񡢵���ϵ�����㹫ʽ(��ֵ)
			//	 	&&��㱣�²���
			GetTbValue(IRecCalc,_variant_t("c_name2"),out_mat);
			out_mat.TrimRight();
			
			//		&&�ڲ㱣�²���
			GetTbValue(IRecCalc,_variant_t("c_name_in"),in_mat);
			in_mat.TrimRight();
			
			//	 	&&���������
			GetTbValue(IRecCalc,_variant_t("c_name3"),pro_mat);
			pro_mat.TrimRight();
			
			//�ںڶȱ��в��ұ������ϵĺڶȵ�ʱ��
			//������������,��ȥ���������ŵ�����.��"��Ƥ(0.75)"���ֵΪ"��Ƥ"
			pro_name = pro_mat;
			if( -1 != pro_name.Find("(",0) )
			{
				pro_name = pro_name.Mid( 0, pro_name.Find("(",0) );
			}
		}
		catch(_com_error &e)
		{
			TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�ڶ�ȡ\"ԭʼ���ݱ�(PRE_CALC)\"�г���\r\n"),
				m_nID);
			if(Exception::GetAdditiveInfo())
			{
				TempStr+=Exception::GetAdditiveInfo();
			}
			ExceptionInfo(TempStr);
			Exception::SetAdditiveInfo(NULL);
			ReportExceptionErrorLV2(e);
			IRecCalc->MoveNext();
			continue;
		}   
		//		***���㱣��
		if (out_mat.IsEmpty())
		{
			TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼,û���Ᵽ�²���!"),m_nID);
			ExceptionInfo(TempStr);
			IRecCalc->MoveNext();
			continue;
		}
		if (pro_mat.IsEmpty())
		{
			TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼,û�б��������!"),m_nID);
			ExceptionInfo(TempStr);			
			IRecCalc->MoveNext();
			continue;			
		}
		try
		{
			if (!(IRecMat->adoEOF && IRecMat->BOF))
			{
				IRecMat->MoveFirst();
			}
		}
		catch(_com_error &e)
		{
			ReportExceptionErrorLV2(e);
			throw;
		}
		
		if(!LocateFor(IRecMat,_variant_t("MAT_NAME"),CFoxBase::EQUAL,
			_variant_t(out_mat)))
		{
			//�ڵ�ǰ���̵Ĳ��Ͽ���û�ҵ�,��ȥ��׼������(������Ϊ��).�ҵ��ͽ���׼���еĲ������ӵ���ǰ�Ĺ�������			
			if (!FindStandardMat(IRecMat,out_mat))
			{
				TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�޷���\"���²��ϲ�����(A_MAT)\"�е�\"��������\"�ֶ�")
					_T("�ҵ�%s\r\n�п������ݱ�Ϊ�ջ�δ�������"),
					m_nID,out_mat);
				ExceptionInfo(TempStr);
				IRecCalc->MoveNext();
				continue;
			}
		}
		
		try
		{
			//�۸�
			GetTbValue(IRecMat,_variant_t("MAT_PRIC"),in_price);
			
			//��㱣�²��ϵ����ʹ���¶�
			//GetTbValue(IRecMat,_variant_t("MAT_TEMP"),in_tmax);
			
			//"MAT_TMAX"�ֶ���"��������������¶�"
			GetTbValue(IRecMat,_variant_t("MAT_TMAX"),in_tmax);
			if (in_tmax <= 0)
			{   //����������������¶�δ����
				double dTemp =  vtof(IRecMat->GetCollect(_variant_t("MAT_TEMP")));				
				in_tmax = dTemp / 2;//��������������¶�������㱣�²��ϵ����ʹ���¶ȵ�1/2
				if (in_tmax <= 0)
				{
					//����㱣�²��ϵ����ʹ���¶�δ���ã��򸴺�����������¶�Ĭ��Ϊ350C��
					in_tmax = 350;
				}
			}
			//����ϵ������
			GetTbValue(IRecMat,_variant_t("MAT_A0"),in_a0);
			//����ϵ��һ����ϵ��
			GetTbValue(IRecMat,_variant_t("MAT_A"),in_a1);
			//����ϵ��������ϵ��
			GetTbValue(IRecMat,_variant_t("MAT_A1"),in_a2);
			//����
			GetTbValue(IRecMat,_variant_t("MAT_DENS"),in_dens);
			// ����ϵ�����ӵı���
			GetTbValue( IRecMat, _variant_t("MAT_RATIO"), in_ratio);

			//�ڲ���ʽȫ��ʹ�ù��ʵ�λ�Ƽ���,����Ժ����ʽʹ�ù����Ƽ���.
			//������Ժ�ķ���ϵ��ALPHA���õ��ǹ��ʵ�λ��,���߲�һ�¼������д���.
			if(unit != _T("SI"))
			{
				in_a0/=0.8598;
				in_a1/=0.8598;
				in_a2/=0.8598;
			}
		}
		catch(_com_error &e)
		{
			TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�ڶ�ȡ\"���²��ϲ�����(A_MAT)\"��")
				_T("��������=%sʱ����\r\n"),m_nID,out_mat);
			if(Exception::GetAdditiveInfo())
			{
				TempStr+=Exception::GetAdditiveInfo();
			}
			ExceptionInfo(TempStr);
			Exception::SetAdditiveInfo(NULL);
			ReportExceptionErrorLV2(e);
			IRecCalc->MoveNext();
			continue;
		}
		IRecMat->MoveFirst();
		
		//���ұ������ϵ�����
		if(!LocateFor(IRecMat,_variant_t("MAT_NAME"),CFoxBase::EQUAL,
			_variant_t(pro_mat)))
		{
			//�ڵ�ǰ���̵Ĳ��Ͽ���û�ҵ�,��ȥ��׼������(������Ϊ��).�ҵ��ͽ���׼���еĲ������ӵ���ǰ�Ĺ�������
			if (!FindStandardMat(IRecMat, pro_mat))
			{
				TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�޷���\"���²��ϲ�����(A_MAT)\"�е�\"��������\"�ֶ�")
					_T("�ҵ�%s\r\n�п������ݱ�Ϊ�ջ�δ�������"),
					m_nID,pro_mat);			
				ExceptionInfo(TempStr);
				IRecCalc->MoveNext();
				continue;
			}
		}
		try
		{
			//�۸�
			GetTbValue(IRecMat,_variant_t("MAT_PRIC"),pro_price);
			//����ϵ������
			GetTbValue(IRecMat,_variant_t("MAT_A0"),pro_conduct);
			//����
			GetTbValue(IRecMat,_variant_t("MAT_DENS"),pro_dens);
		}
		catch(_com_error &e)
		{
			TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�ڶ�ȡ\"���²��ϲ�����(A_MAT)\"��")
				_T("��������=%sʱ����\n"),m_nID,pro_mat);
			if(Exception::GetAdditiveInfo())
			{
				TempStr+=Exception::GetAdditiveInfo();
			}
			ExceptionInfo(TempStr);
			Exception::SetAdditiveInfo(NULL);
			ReportExceptionErrorLV2(e);
			IRecCalc->MoveNext();
			continue;
		}
		
		//���ϱ���
		in_mat.TrimRight();
		if (!in_mat.IsEmpty())
		{
			//��ǰ���õ��Ᵽ�²��ϵĲ��������Ᵽ�²��ϵı���
			out_a0 = in_a0;
			out_a1 = in_a1;
			out_a2 = in_a2;
			out_price = in_price;
			out_dens  = in_dens;
			out_ratio = in_ratio;
			
			//�����ڱ��²���
			IRecMat->MoveFirst();
			if(!LocateFor(IRecMat,_variant_t("MAT_NAME"),CFoxBase::EQUAL,
							_variant_t(in_mat)))
			{
				//�ڵ�ǰ���̵Ĳ��Ͽ���û�ҵ�,��ȥ��׼������(������Ϊ��).�ҵ��ͽ���׼���еĲ������ӵ���ǰ�Ĺ�������
				if (!FindStandardMat(IRecMat, in_mat))
				{
					TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�޷���\"���²��ϲ�����(A_MAT)\"�е�\"��������\"�ֶ�")
						_T("�ҵ�%s\r\n�п������ݱ�Ϊ�ջ�δ�������"),
						m_nID,in_mat);			
					ExceptionInfo(TempStr);
					IRecCalc->MoveNext();
					continue;
				}
			}
			try
			{
				//�۸�
				GetTbValue(IRecMat,_variant_t("MAT_PRIC"),in_price);
				//����ϵ������
				GetTbValue(IRecMat,_variant_t("MAT_A0"),in_a0);
				//����ϵ��һ����ϵ��
				GetTbValue(IRecMat,_variant_t("MAT_A"),in_a1);
				//����ϵ��������ϵ��
				GetTbValue(IRecMat,_variant_t("MAT_A1"),in_a2);
				//����
				GetTbValue(IRecMat,_variant_t("MAT_DENS"),in_dens);
				// ����ϵ�����ӵı��ʣ����Ϊ����(1.5 ����1.7)
				GetTbValue( IRecMat, _variant_t("MAT_RATIO"), in_ratio );

				//�ڲ���ʽȫ��ʹ�ù��ʵ�λ�Ƽ���,����Ժ����ʽʹ�ù����Ƽ���.
				//������Ժ�ķ���ϵ��ALPHA���õ��ǹ��ʵ�λ��,���߲�һ�¼������д���.
				if(unit != _T("SI"))
				{
					in_a0/=0.8598;
					in_a1/=0.8598;
					in_a2/=0.8598;
				}				
			}
			catch(_com_error &e)
			{
				TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�ڶ�ȡ\"���²��ϲ�����(A_MAT)\"��")
					_T("��������=%sʱ����\r\n"),m_nID,in_mat);
				if(Exception::GetAdditiveInfo())
				{
					TempStr+=Exception::GetAdditiveInfo();
				}
				ExceptionInfo(TempStr);
				Exception::SetAdditiveInfo(NULL);
				ReportExceptionErrorLV2(e);
				IRecCalc->MoveNext();
				continue;
			}
		}
		try
		{
			//�ȼ۱�������
			GetTbValue(IRecCalc,_variant_t("c_price"),Yong);
			//����¶�
			GetTbValue(IRecCalc,_variant_t("c_temp"),Temp_pip);
			//�⾶
			GetTbValue(IRecCalc,_variant_t("c_size"),D0);
			// Modifier zsy 2007.01.11
			// �������Ӧ��ֱ���ó����ݿ��е�����.
//			//�������
//			GetTbValue(IRecCalc,_variant_t("c_rate"),S);
			//������Сʱ
			GetTbValue(IRecCalc,_variant_t("c_hour"),Mhours);
			//�ж����й���
			if( Mhours >= a_config_hour )
			{
				hour_work = 1;	//�����й���
			}
			else
			{
				hour_work = 0; //�������й���.
			}
			//�����¶�
			GetTbValue(IRecCalc,_variant_t("c_con_temp"),Temp_env);
			//�����¶ȵ�ȡֵ����
			nTaIndex = vtoi(IRecCalc->GetCollect(_variant_t("c_Env_Temp_Index")));
			//����
			GetTbValue(IRecCalc,_variant_t("c_wind"),speed_wind);
			if (speed_wind < 0)
			{
				speed_wind = 0;
			}
			//�������
			GetTbValue(IRecCalc,_variant_t("c_pro_thi"),pro_thi);
			//�ں�
			GetTbValue(IRecCalc,_variant_t("c_pi_thi"),pi_thi);
			//�ܵ�����
			GetTbValue(IRecCalc,_variant_t("c_pi_mat"),pi_mat);
			//ѹ����ʾ
			//GetTbValue(IRecCalc,_variant_t("c_pg"),pi_pg);
			//����ѹ��
			m_dPipePressure = vtof(IRecCalc->GetCollect(_variant_t("c_Pressure")));
			if (m_dPipePressure <= 3.92) // ���ڹܵ��ϼ���֧���ܼ��Ĳ���
			{
				pi_pg = TRUE;
			}
			else{
				pi_pg = FALSE;
			}

			//��ע
			GetTbValue(IRecCalc,_variant_t("c_mark"),mark);
			//����
			GetTbValue(IRecCalc,_variant_t("c_vol"),vol);			
			//�ܵ����豸������,Ϊ�������һ���ַ�.
			cPipeType = vol.Right(1);
			//���ܻ�ˮ�ܵı�ʾ����Ҫ���ڼ���ܵ���֧���ܵļ�ࡣ
			GetTbValue(IRecCalc,_variant_t("c_steam"),steam);			
			//��װ�ص�.
			strPlace = vtos(IRecCalc->GetCollect(_variant_t("c_place")));			
			//��ü������ϵ���Ĺ�ʽ,������.
			m_nIndexAlpha = vtoi(IRecCalc->GetCollect(_variant_t("c_Alfa_Index")));			
			//����
			dAmount = vtof(IRecCalc->GetCollect(_variant_t("c_amount")));			
			//���㷽��������ֵ
			nMethodIndex = vtoi(IRecCalc->GetCollect(_variant_t("C_CalcMethod_Index")));
			// 
			switch( bIsReCalc )
			{
			case 1: nMethodIndex = nSurfaceTemperatureMethod;	// ��������ʹ��������¶ȷ�У�ˡ�
					bIsReCalc = FALSE;		// ֻ�е�ǰ��һ����¼��У��
					break;
			default: ;	// ���ı���㷽��
			}
			//�Ƿ�����ڱ��²���
			bNoCalInThi = vtob(IRecCalc->GetCollect(_variant_t("c_CalInThi")));
			//�Ƿ�����Ᵽ�²���
			bNoCalPreThi = vtob(IRecCalc->GetCollect(_variant_t("c_CalPreThi")));
			//����¶�
			temp_ts = vtof(IRecCalc->GetCollect(_variant_t("c_tb3")));
			//�ڲ㱣�º�
			thick_1 = vtof(IRecCalc->GetCollect(_variant_t("c_in_thi")));
			//��㱣�º�
			thick_2 = vtof(IRecCalc->GetCollect(_variant_t("c_pre_thi")));
			//�ط��ٷ����ƽ�ڿ��
			B = vtof(IRecCalc->GetCollect(_variant_t("c_B")));
			if (B <= 0)
			{
				//Ĭ��ֵ
				B = 1000;
			}
			//��������
			strMedium = vtos(IRecCalc->GetCollect(_variant_t("c_Medium")));
			m_dMediumDensity = GetMediumDensity(strMedium);	//���ݽ�����,�������ܶ�
		}
		catch(_com_error &e)
		{
			TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�ڶ�ȡ\"ԭʼ���ݿ�(PRE_CALC)\"��ʱ����\r\n"),m_nID);
			if(Exception::GetAdditiveInfo())
			{
				TempStr+=Exception::GetAdditiveInfo();
			}
			ExceptionInfo(TempStr);
			Exception::SetAdditiveInfo(NULL);
			ReportExceptionErrorLV2(e);
			IRecCalc->MoveNext();
			continue;
		}
		ERR=0;
		//�����豸�͹ܵ����½ṹ����洫��ϵ��.
		try
		{
			if(pro_mat.Find(_T("Ĩ��")) != -1)	//��������ΪĨ��ʱ.
			{
				GetAlfaValue(m_pRsCon_Out, "out_dia", "con2", D0, co_alf);
			}
			else 
			{
				GetAlfaValue(m_pRsCon_Out, "out_dia", "con1", D0, co_alf);
			}
		}
		catch(_com_error &e)
		{
			TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�ڶ�ȡ\"con_out��\"��ʱ����\r\n"),m_nID);
			if(Exception::GetAdditiveInfo())
			{
				TempStr+=Exception::GetAdditiveInfo();
			}
			ExceptionInfo(TempStr);
			Exception::SetAdditiveInfo(NULL);
			ReportExceptionErrorLV2(e);
			IRecCalc->MoveNext();
			continue;
		}
		//��һ����Ա�ļ�¼ָ�룬�ڼ���֮ǰ�ʹ򿪱�[a_hedu]
		m_pRs_a_Hedu->MoveFirst();
		//�������ϵĺڶ�
		if(LocateFor(m_pRs_a_Hedu,_variant_t("c_name"),CFoxBase::EQUAL,
			_variant_t(pro_name))==TRUE)
		{
			TempValue=m_pRs_a_Hedu->GetCollect(_variant_t("n_hedu"));
			hedu=TempValue;
		}
		else
		{
			hedu=0.23;     //Ĭ�϶�п��Ƥ��Ӧ�ĺڶ�
		}
		
		if(Yong*Temp_pip<-0.001)
		{
			TempStr.Format(_T("���ִ����ڼ�¼: %d"),RecNo(IRecCalc));
			AfxMessageBox(TempStr);
			ERR=1;
		}
		
		if(D0*in_price*in_a0*S*Mhours*speed_wind*pro_thi*pi_thi<0)
		{
			TempStr.Format(_T("���Ϊ %d �ļ�¼,����ԭ���Դ���! "),RecNo(IRecCalc));
			AfxMessageBox(TempStr);
			ERR=1;			
		}
		
		if(ERR==1)
		{
			if( IDYES == AfxMessageBox("�Ƿ�ֹͣ���㣿", MB_YESNO ) )
			{
				return;
			}
			else
			{
				IRecCalc->MoveNext();
				continue;
			}
		}
		// ȷ�����¶�������
		if( D0 >= m_dMaxD0 )		
		{	//ƽ��
			bIsPlane = true;
		}
		else
		{	//�ܵ�
			bIsPlane = false;
		}

		switch( nMethodIndex )
		{
		case nSurfaceTemperatureMethod:
			//������¶ȷ����㱣�º�� 
			//������¶�Ϊ��֪
			INITDATA(CCalcThick_FaceTemp);		//��ʼ����Աֵ������ǰ��¼��ԭʼ���ݴ��뵽���㷽��������
			lpMethodClass = (CCalcThick_FaceTemp *)this;
			break;
		case nHeatFlowrateMethod:
			//����ɢ���ܶȷ����㱣�º��
			INITDATA(CCalcThick_HeatDensity);	//
			lpMethodClass = (CCalcThick_HeatDensity *)this;
			break;
		case nPreventCongealMethod:
			//������(��ƽ��)�����㱣�º��
			INITDATA(CCalcThick_PreventCongeal)	//
			lpMethodClass = (CCalcThick_PreventCongeal *)this;
			break;
		case nSubterraneanMethod: 
			// ��عܵ����¼��㷽��
			short nType;
			short nPipeCount;
			GetSubterraneanTypeAndPipeCount(m_nID, nType, nPipeCount);
			switch(nType)
			{
			case 1:	// �йܹ�ͨ��
			case 2:	// �йܹ���ͨ��
				INITDATA(CCalcThick_SubterraneanObstruct);
				lpMethodClass = (CCalcThick_SubterraneanObstruct*)this;
				break;
			case 0:	// ��ص��޹ܹ�ֱ��
			default:
				INITDATA(CCalcThick_SubterraneanNoChannel);		//.
				lpMethodClass = (CCalcThick_SubterraneanNoChannel*)this;
			}
			if (nPipeCount <= 1)
			{
				in_mat = "";	// �ڱ��²��϶�Ӧ��صĵڶ����ܵ��ı��²�������
			}
			bIsPlane = false;	// ��ؼ��㷽��,ֻ�йܵ��ı��µļ��㹫ʽ
			break;
		default:	//if( nMethodIndex == nEconomicalThicknessMethod )
			//Ĭ��Ϊ���÷����㱣�º�
			INITDATA(CCalcThick_Economy);		//��ʼ����Աֵ������ǰ��¼��ԭʼ���ݴ��뵽���㷽��������.
			lpMethodClass = (CCalcThick_Economy *)this;
		}


		if( nMethodIndex == nSurfaceTemperatureMethod )
		{
			//������¶ȷ����㱣�º��
			//������¶�Ϊ��֪
			INITDATA(CCalcThick_FaceTemp);		//��ʼ����Աֵ������ǰ��¼��ԭʼ���ݴ��뵽���㷽��������
			lpMethodClass = (CCalcThick_FaceTemp *)this;
		}
		else if( nMethodIndex == nHeatFlowrateMethod )
		{	
			//����ɢ���ܶȷ����㱣�º��
			INITDATA(CCalcThick_HeatDensity);	//��ʼ����Աֵ������ǰ��¼��ԭʼ���ݴ��뵽���㷽��������
			lpMethodClass = (CCalcThick_HeatDensity *)this;
		}
		else if ( nMethodIndex == nPreventCongealMethod )
		{
			//������(��ƽ��)�����㱣�º��
			INITDATA(CCalcThick_PreventCongeal);	//��ʼ����Աֵ������ǰ��¼��ԭʼ���ݴ��뵽���㷽��������
			lpMethodClass = (CCalcThick_PreventCongeal *)this;
		}
		else	//if( nMethodIndex == nEconomicalThicknessMethod )
		{	
			//Ĭ��Ϊ���÷����㱣�º�
			INITDATA(CCalcThick_Economy);		//��ʼ����Աֵ������ǰ��¼��ԭʼ���ݴ��뵽���㷽��������.
			lpMethodClass = (CCalcThick_Economy *)this;
		}

				
		//����ǰ���Ƚ���������ʼ��
		
		temp_tb = 0.0;							//�������������¶�
		pre_wei1 = pre_wei2 = pro_wei = 0.0;	//��������㡢����������
		p_wei = w_wei = with_w = no_w = 0.0;
		
		pre_v1 = pre_v2 = pro_v = 0.0;			//������������
		distan = 0.0;							//֧���ܼ��
		face_area = 1;							//�����ƽ��ʱ����Ϊ1, ����Ϊ�ܵ������¼���.		
		
		thick_3 = pro_thi;						//thick_3=������
		
		in_mat.TrimRight();
		if( !in_mat.IsEmpty() )//Ϊ���ϱ���
		{
			if( bIsPlane )
			{
				//���ϱ���ƽ��.
				//���㱣�º�.		2005.05.24
				if ( bNoCalInThi || bNoCalPreThi )		//���뱣�²���
				{
					// ������֪�ı��º��,����������¶�,�ͽ����¶�
					lpMethodClass->CalcPlain_Com_InputThick(thick_1,thick_2,temp_tb,temp_ts);
				}
				else
				{
					// ���º��û�����룬��Ҫ����
					lpMethodClass->CalcPlain_Com(thick_1,thick_2,temp_tb,temp_ts);
				//////////////////////////////////////////////////////////////////////////
				// ����ϵ���ڼ���֮ǰ��ȷ�����ǲ���ǰ���ʽ���� [11/17/2005]
					/*
					while(1)
					{
						temp_ts1 = temp_ts;
						//�����û�ѡ��Ĺ�ʽ���¼������ϵ��. �ɹ�����1.
						if( CalcAlfaValue(temp_ts, D0+2*thick_1+2*thick_2) )
						{
							//�����µķ���ϵ��,�ټ��㱣�º�.
							lpMethodClass->CalcPlain_Com(thick_1,thick_2,temp_tb,temp_ts);
							if( fabs(temp_ts1-temp_ts) <= TsDiff)
							{
								break;
							}
						}
						else
						{
							//�������ϵ��ʱ,����.�������ò�����õ�ֵȥ������,����Ҫ��ѭ��.
							break;
						}
					}
				*/
				}
				//�������Ļ���ַ���
				TempStr.Format(_T("ƽ�� \t��¼�� \t�¶� \t�ڲ㱣�º� \t��㱣�º� \t������¶� \t������¶� \r\n")
					_T("\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f"),
					m_nID,Temp_pip,thick_1,thick_2,temp_tb,temp_ts);
				
				pre_v1=thick_1/1000;		//�ڱ��²�����.
				pre_v2=thick_2/1000;		//�Ᵽ�²�����.
				pro_v=thick_3/1000;			//����������
				pre_wei1=pre_v1*in_dens;	//�ڱ��²������
				pre_wei2=pre_v2*out_dens;
				pro_wei=pro_v*out_dens;
				//end if ( ���ϱ���ƽ�� )
			}
			else
			{
				//���ϱ��¹ܵ�
				if ( bNoCalPreThi || bNoCalInThi )	//2005.05.24
				{
					//ָ�����º��,����������¶Ⱥͽ����¶�
					lpMethodClass->CalcPipe_Com_InputThick(thick_1,thick_2,temp_tb,temp_ts);
				}
				else
				{
					//���ϱ��¹ܵ�
					lpMethodClass->CalcPipe_Com(thick_1,thick_2,temp_tb,temp_ts);
					
				/*	while(1)
					{
						temp_ts1 = temp_ts;
						if(	CalcAlfaValue(temp_ts, D0+2*thick_1+2*thick_2) )
						{	//�����µķ���ϵ��,�ټ��㱣�º�.
							lpMethodClass->CalcPipe_Com(thick_1,thick_2,temp_tb,temp_ts);
							if( fabs(temp_ts1-temp_ts) <= TsDiff )
							{
								break;
							}
						}
						else
						{
							//�������ϵ��ʱ,����.�������ò�����õ�ֵȥ������,����Ҫ��ѭ��.
							break;
						}
					}
				*/
				}
				//�������Ļ���ַ���
				TempStr.Format(_T("�ܵ� \t��¼�� \t�¶� \tֱ�� \t�ڲ㱣�º� \t��㱣�º� \t������¶� \t������¶� \r\n")
					_T("\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f"),
					m_nID,Temp_pip,D0,thick_1,thick_2,temp_tb,temp_ts);
				
				//�����������
				d1=D0+2*thick_1;
				d2=d1+2*thick_2;
				d3=d2+2*pro_thi;
				pre_v1=pi/4*(d1*d1-D0*D0)/1E6;
				pre_wei1=pre_v1*in_dens;
				pre_v2=pi/4*(d2*d2-d1*d1)/1E6;
				pre_wei2=pre_v2*out_dens;
				pro_v=pi/4*(d3*d3-d2*d2)/1E6;
				pro_wei=pro_v*pro_dens;
				
				//�ڹܵ��������л��ָ���⾶�ͱں�Ĺ��غ�ˮ��
				if( !GetPipeWeithAndWaterWeith(IRecPipe,D0,pi_thi,"PIPE_WEI",p_wei,"WATER_WEI",w_wei) )
				{
					//�ڹܵ���������û���ҵ���Ӧ�⾶�ļ�¼.
					//���¼���
					p_wei=pi/4*7850*(D0*D0-(D0-2*pi_thi)*(D0-2*pi_thi))/1000000;
					w_wei=pi/4*(D0-2*pi_thi)*(D0-2*pi_thi)/1000;
				}
				//��ˮ����
				no_w=p_wei+pre_wei1+pre_wei2+pro_wei;
				//��ˮ����
				with_w=no_w+w_wei;
				//������
				face_area=pi*d3/1000;
				
				//�ܵ����豸���Ͳ�ΪS��M��O
				if( cPipeType.CompareNoCase(_T("S")) && cPipeType.CompareNoCase(_T("M")) && cPipeType.CompareNoCase(_T("O")) )
				{
					p_wei=w_wei=no_w=with_w=0;
				}
				
				//***����ˮ�ܵ����͹ܵ�����
				distan=0;
				
				//if( !cPipeType.CompareNoCase(_T("S")) || !cPipeType.CompareNoCase(_T("M")) || !cPipeType.CompareNoCase(_T("O")) )
				//{
					try
					{
						distan = GetSupportSpan(D0,pi_thi,pi_mat,m_dPipePressure,m_dMediumDensity,
											Temp_pip,no_w-p_wei);
					}
					catch(_com_error &e)
					{
						TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�ڼ���ʱ����\r\n"),
							m_nID);
						if(Exception::GetAdditiveInfo())
						{
							TempStr+=Exception::GetAdditiveInfo();
						}
						ExceptionInfo(TempStr);
						Exception::SetAdditiveInfo(NULL);
						ReportExceptionErrorLV2(e);
						IRecCalc->MoveNext();
						continue;
					}
					TempStr.Format(_T("�ܵ� \t��¼�� \t���Lmax\t�¶� \tֱ�� \t�ڲ㱣�º� \t��㱣�º� \t������¶� \t������¶� \r\n")
						_T("\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f"),
						m_nID,distan,Temp_pip,D0,thick_1,thick_2,temp_tb,temp_ts);
				//}	
					
			}// end   (���ϱ��¹ܵ�)
		}
		else//���㱣��
		{
			temp_tb = 0;	// ����û�н����¶�
			thick_1 = 0;	// ����û���ڱ��º��

			if( bIsPlane )//���㱣��ƽ��
			{
				if ( bNoCalPreThi || bNoCalInThi )	//�ֹ����뱣�²���
				{
					//���ݱ��º��,����������¶�
					lpMethodClass->CalcPlain_One_InputThick(thick_2,temp_ts);
				}
				else
				{
					lpMethodClass->CalcPlain_One(thick_2,temp_ts);
				/*
					while(1)
					{
						temp_ts1 = temp_ts;
						//�����µķ���ϵ��,�ټ��㱣�º�.
						if( CalcAlfaValue(temp_ts, D0+2*thick_2) )
						{
							lpMethodClass->CalcPlain_One(thick_2,temp_ts);
							if( fabs(temp_ts1-temp_ts) <= TsDiff)
							{
								break;
							}
						}
						else
						{
							//�������ϵ��ʱ,����.�������ò�����õ�ֵȥ������,����Ҫ��ѭ��.
							break;
						}
					}
				*/
				}
				//�������Ļ���ַ���
				TempStr.Format(_T("ƽ�� \t��¼�� \t�¶� \t���º� \t������¶� \r\n")
					_T("\t%d\t%.2f\t%.2f\t%.2f"),
					m_nID,Temp_pip,thick_2,temp_ts);
				
				pre_v2=thick_2/1000;	//���㱣�²����	
				pro_v=pro_thi/1000;		//���������
				pre_wei2=pre_v2*in_dens;//���²�����
				pro_wei=pro_v*pro_dens;
				
				//end if (���㱣��ƽ��)
			}
			else
			{//���㱣�¹ܵ�
				if ( bNoCalPreThi || bNoCalInThi )
				{
					//���ݱ��º��,����������¶�.
					lpMethodClass->CalcPipe_One_InputThick(thick_2,temp_ts);					
				}
				else
				{
					lpMethodClass->CalcPipe_One(thick_2,temp_ts);
				/*
					while(1)
					{
						//�����µķ���ϵ��,�ټ��㱣�º�.
						temp_ts1 = temp_ts;
						if( CalcAlfaValue(temp_ts, D0+2*thick_2) )
						{
							lpMethodClass->CalcPipe_One(thick_2,temp_ts);
							if( fabs(temp_ts1-temp_ts) <= TsDiff )
							{
								break;
							}
						}
						else
						{	//�������ϵ��ʱ,����.�������ò�����õ�ֵȥ������,����Ҫ��ѭ��.
							break;
						}
					}
				*/
				}
				TempStr.Format(_T("�ܵ� \t��¼�� \t�¶� \tֱ�� \t���º� \t������¶� \r\n")
					_T("\t%d\t%.2f\t%.2f\t%.2f\t%.2f"), 
					m_nID,Temp_pip,D0,thick_2,temp_ts);
				//�������������
				d1=D0+2*thick_2;
				d3=D0+2*thick_2+2*pro_thi;
				pre_v2=pi/4*(d1*d1-D0*D0)/1000000;
				pre_wei2=pre_v2*in_dens;
				pro_v=pi/4*(d3*d3-d1*d1)/1000000;
				pro_wei=pro_v*pro_dens; 
				
				//�ڹܵ��������л��ָ���⾶�ͱں�Ĺ��غ�ˮ��
				if( !GetPipeWeithAndWaterWeith(IRecPipe,D0,pi_thi,"PIPE_WEI",p_wei,"WATER_WEI",w_wei) )
				{
					//�ڹܵ���������û���ҵ���Ӧ�⾶�ļ�¼.
					//���¼���
					p_wei=pi/4*7850*(D0*D0-(D0-2*pi_thi)*(D0-2*pi_thi))/1000000;
					//	w_wei=pi/4*(D0-2*D0)*(D0-2*D0)/1000; // FoxPro�еĹ�ʽ
					w_wei = pi / 4 * ( D0 - 2 * pi_thi ) * ( D0 - 2 * pi_thi ) / 1000;
				}
				//��ˮ����
				no_w=p_wei+pre_wei2+pro_wei;
				//��ˮ����
				with_w=no_w+w_wei;
				//������
				face_area=pi*(D0+2*thick_2+2*pro_thi)/1000;			
				//����ܵ����Ͳ�Ϊ��ˮ�ܵ����͹ܵ�
				if( cPipeType.CompareNoCase(_T("S")) && cPipeType.CompareNoCase(_T("M")) && cPipeType.CompareNoCase(_T("O")) )
				{
					p_wei=w_wei=no_w=with_w=0;
				}
				//***����ˮ�ܵ����͹ܵ�����
				distan=0;

				
				//if( !cPipeType.CompareNoCase(_T("S")) || !cPipeType.CompareNoCase(_T("M")) || !cPipeType.CompareNoCase(_T("O")) )
				//{
					try
					{
						distan = GetSupportSpan(D0,pi_thi,pi_mat,m_dPipePressure,m_dMediumDensity,
											Temp_pip,no_w-p_wei);
					}
					catch(_com_error &e)
					{
						TempStr.Format(_T("ԭʼ�������Ϊ %d �ļ�¼�ڼ���ʱ����\r\n"),
							m_nID);
						if(Exception::GetAdditiveInfo())
						{
							TempStr+=Exception::GetAdditiveInfo();
						}
						ExceptionInfo(TempStr);
						Exception::SetAdditiveInfo(NULL);
						ReportExceptionErrorLV2(e);
						IRecCalc->MoveNext();
						continue;
					}
					TempStr.Format(_T("�ܵ� \t��¼�� \t���Lmax\t�¶� \tֱ�� \t���º� \t������¶� \r\n")
						_T("\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f"),
						m_nID,distan,Temp_pip,D0,thick_2, temp_ts);
				//}
				
			}// end �����㱣�¹ܵ���
			
		}//end ����

		if ( !bNoCalPreThi &&  !bNoCalInThi )	// ָ���ı��º��
		{
			if ( Temp_pip < Temp_env )
			{
				// �����ڲ�ͬ����ҵ�����㷽��
				if ( nMethodIndex != nSurfaceTemperatureMethod )
				{
					if ( temp_ts < Temp_dew_point + Temp_coefficient )	// ¶���¶ȼ�һ������ֵ(��ͬ�Ĺ淶���ܻ�������ͬ,1-3��)
					{
						//IRecCalc->PutCollect( _variant_t("c_CalcMethod_Index"), _variant_t((long)nSurfaceTemperatureMethod) );
						IRecCalc->PutCollect( _variant_t("c_tb3"), _variant_t(Temp_dew_point + Temp_coefficient));
						bIsReCalc = 1;	// ʹ�ñ����¶ȷ�����У��

						TempStr.Format("��ʾ: ���Ϊ %d �ļ�¼�ڼ���ʱ,ʹ����������¶ȷ�����У�ˣ�\r\n", m_nID);
						ExceptionInfo( TempStr );
						// ����ǰһ�μ�����м�ֵ
						IRecCalc->Update();
 						continue;
					}
				}
			}

		}
		//���ַ�����ʾ����Ļ�ϡ���д�뵽�ĵ��У�
		DisplayRes(TempStr);
		
		//��ʾ������Ϣ
		if ( lpMethodClass->GetExceptionInfo(TempStr) )
		{
			if(Exception::GetAdditiveInfo())
			{
				TempStr+=Exception::GetAdditiveInfo();
			}
			ExceptionInfo(TempStr);	
			Exception::SetAdditiveInfo(NULL);
		}		
		//�Ƿ�ֹͣ����
		IsStop=1;
		Cancel(&IsStop);
		if(IsStop==0)
		{
			return;
		}
		dUnitLoss = lpMethodClass->dQ;
		if (dUnitLoss < 0)
		{
			dUnitLoss = 0;
		}
		dAreaLoss = dUnitLoss * face_area * dAmount;
		
		//----------------------------
		//������Ľ��д�뵽���ݿ���
		try
		{
			//�ڱ��º�
			IRecCalc->PutCollect(_variant_t("c_in_thi"),_variant_t(thick_1));
			//�Ᵽ�º�/����ı��²���
			IRecCalc->PutCollect(_variant_t("c_pre_thi"),_variant_t(thick_2));
			//������¶�
			IRecCalc->PutCollect(_variant_t("c_tb3"),_variant_t(temp_ts));
			//�����¶�
			IRecCalc->PutCollect(_variant_t("c_ts"),_variant_t(temp_tb));
			//��λɢ���ܶ�
			IRecCalc->PutCollect(_variant_t("c_HeatFlowrate"),_variant_t(dUnitLoss));
			//����ʧ=������*����*��λɢ���ܶ�
			IRecCalc->PutCollect(_variant_t("c_lost"),_variant_t(dAreaLoss));
			//���½ṹͶ�����ܷ���
			IRecCalc->PutCollect(_variant_t("c_srsb"),_variant_t((double)0));
			//��ȷֵ��Ӧ���½ṹͶ�����ܷ���
			IRecCalc->PutCollect(_variant_t("cal_srsb"),_variant_t((double)0));
			//����²��Ⱦ�ȷֵ
			IRecCalc->PutCollect(_variant_t("cal_thi"),_variant_t((double)0));
			//�ڱ��²㵥λ����
			IRecCalc->PutCollect(_variant_t("c_in_wei"),_variant_t(pre_wei1));
			//�Ᵽ�²㵥λ����/����ı��²㵥��
			IRecCalc->PutCollect(_variant_t("c_pre_wei"),_variant_t(pre_wei2));
			//�����㵥λ����
			IRecCalc->PutCollect(_variant_t("c_pro_wei"),_variant_t(pro_wei));
			//�ܵ�����
			IRecCalc->PutCollect(_variant_t("c_pipe_wei"),_variant_t(p_wei));
			//ˮ����
			IRecCalc->PutCollect(_variant_t("c_wat_wei"),_variant_t(w_wei));
			//��ˮ����
			IRecCalc->PutCollect(_variant_t("c_with_wat"),_variant_t(with_w));
			//��ˮ����
			IRecCalc->PutCollect(_variant_t("c_no_wat"),_variant_t(no_w));
			//������
			IRecCalc->PutCollect(_variant_t("c_area"),_variant_t(face_area));
			//�ڱ��²㵥λ���
			IRecCalc->PutCollect(_variant_t("c_v_in"),_variant_t(pre_v1));
			//�Ᵽ�²㵥λ���/����ı��²㵥λ���
			IRecCalc->PutCollect(_variant_t("c_v_pre"),_variant_t(pre_v2));
			//�����㵥λ���
			IRecCalc->PutCollect(_variant_t("c_v_pro"),_variant_t(pro_v));
			//�ܵ�֧���ܼ��
			IRecCalc->PutCollect(_variant_t("c_distan"),_variant_t(distan));
			
			IRecCalc->Update();
		}
		catch(_com_error &e)
		{
			ReportExceptionErrorLV2(e);
			throw;
		}		
		
		IRecCalc->MoveNext();
		continue;
		
		//���´���Ϊ�ϰ汾��,����û���õ�.
		//ɾ����û��Ӱ�� by ligb on 2010.06.15

	}//ENDDO
	//���㱣�²��ϵ�ȫ���.
	this->E_Predat_Cubage();
	//

	Say(_T("�������!"));
//	CLOSE DATA

	OPENATABLE(IRecPass,PASS);
	
//	LOCATE FOR pass_mod1="C_ANALYS"
	if(!LocateFor(IRecPass,_variant_t("PASS_MOD1"),CFoxBase::EQUAL,
			  _variant_t("C_ANALYS")))
	{
		TempStr.Format(_T("�޷���PASS���е�\"PASS_MOD1\"�ֶ��ҵ�C_ANALYS \r\n�п������ݱ�Ϊ�ջ�δ�������"));
		ExceptionInfo(TempStr);

		return;
	}

//	rec=RECNO()
	rec=RecNo(IRecPass);
	IRecPass->MoveFirst();

//	REPL NEXT rec pass_mark1 WITH "T"
	ReplNext(IRecPass,_variant_t("PASS_MARK1"),_variant_t("T"),rec);

//	SKIP
	IRecPass->MoveNext();

//	REPL NEXT 130 pass_mark1 WITH "F"
	ReplNext(IRecPass,_variant_t("PASS_MARK1"),_variant_t("T"),130);

//	USE
//	RETURN
	IRecPass->Close();
	IRecCalc->Close();
	IRecMat->Close();
	IRecPipe->Close();
}

void CHeatPreCal::SetChCal(int Ch)
{
	ch_cal=Ch;
}

int CHeatPreCal::GetChCal()
{
	return ch_cal;
}

////////////////////////////////////////////////////////////
//
// ����Ҫָ������ġ���ʼ���롮��������Χʱ����
//
// Start	��Ҫ�������ʼλ��
// End		��Ҫ����Ľ���λ��
//
BOOL CHeatPreCal::RangeDlgshow(long &Start,long &End)
{
	AfxMessageBox("��Χ�Ի���");
	return TRUE;
	
}

///////////////////////////////////////////////////////////
//
// ����Ҫѡ����Ҫ���������ʱ����
//
// IRecordset	����һ�ű�ļ�¼����
//
// �����ű��C_PASS���ֶ�����Y��������¼���ᱻ����
//
void CHeatPreCal::SelectToCal(_RecordsetPtr &IRecordset,int *pIsStop)
{

}

////////////////////////////////////////////////////
//
// ���ù�����(ID)
//
// Name[in]	���̵�ID��
//
void CHeatPreCal::SetProjectName(CString &Name)
{
	m_ProjectName=Name;
}

/////////////////////////////////////////////////////
//
//  ���ع�����(ID)
//
CString& CHeatPreCal::GetProjectName()
{
	return m_ProjectName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ĳ�����̵ı�ļ�¼
//
// IRecordsetPtr[out]	������Ҫ�򿪵ı�ļ�¼��
// pTableName[in]		��Ҫ�򿪵ı���
// pProjectName[in]		��򿪵Ĺ�������ID��
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
// ���ô˺���ǰ�� SetConnect �������ݿ������
//
BOOL CHeatPreCal::OpenAProjectTable(_RecordsetPtr &IRecordset, LPCTSTR pTableName, LPCTSTR pProjectName)
{
	CString SQL;


	if(GetConnect()==NULL)
	{
		ReportExceptionErrorLV1(_T("���Ӳ�����"));
		return FALSE;
	}

	if(pTableName==NULL)
	{
		ReportExceptionErrorLV1(_T("����������"));
		return FALSE;
	}

	if(pProjectName==NULL)
	{
		SQL.Format(_T("SELECT * FROM %s WHERE EnginID='%s'"),pTableName,m_ProjectName);
	}
	else if(pProjectName==_T(""))
	{
		SQL.Format(_T("SELECT * FROM %s where EnginID is NULL"),pTableName);
	}
	else		
	{
		SQL.Format(_T("SELECT * FROM %s WHERE EnginID='%s'"),pTableName,pProjectName);
	}

	try
	{
		IRecordset.CreateInstance(__uuidof(Recordset));
		IRecordset->Open(_variant_t(SQL),_variant_t((IDispatch*)GetConnect()),
						 adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return FALSE;
	}

	return TRUE;
}


//////////////////////////////////////////////
//
// �ж��Ƿ�ֹͣ����ʱ���ô˺���
//
// pState[out]��pState����0ʱ��ֹͣ����,
// ����������㡣
//
void CHeatPreCal::Cancel(int *pState)
{
	if(pState)
	{
		*pState=1;
	}
}


//////////////////////////////////////////////////
//
// ���ø������ݿ������
//
// IConnection[in]	���ݿ�����ӵ�����ָ�������
//
void CHeatPreCal::SetAssistantDbConnect(_ConnectionPtr &IConnection)
{
	m_AssistantConnection=IConnection; 
}

///////////////////////////////////////////////////////
//
//���ظ������ݿ������
//
_ConnectionPtr& CHeatPreCal::GetAssistantDbConnect()
{
	return m_AssistantConnection;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// �򿪸������ݿ�ı�
// 
// IRecordset[out]	�����Դ򿪱�ļ�¼��������ָ��
// pTableName[in]	��򿪵ı���
//
// �����ɹ�����TRUE�����򷵻�FALSE
//
BOOL CHeatPreCal::OpenAssistantProjectTable(_RecordsetPtr &IRecordset, LPCTSTR pTableName)
{
	
	CString SQL;


	if(GetAssistantDbConnect()==NULL)
	{
		ReportExceptionErrorLV1(_T("���Ӳ�����"));
		return FALSE;
	}

	if(pTableName==NULL)
	{
		ReportExceptionErrorLV1(_T("����������"));
		return FALSE;
	}

	SQL.Format(_T("SELECT * FROM %s"),pTableName);

	try
	{
		if ( IRecordset == NULL )
			IRecordset.CreateInstance(__uuidof(Recordset));

		if ( IRecordset->State == adStateOpen )
			IRecordset->Close();
		
		IRecordset->Open(_variant_t(SQL),_variant_t((IDispatch*)GetAssistantDbConnect()),
						 adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////
//
// ����Material���ݿ��·��
// 
// pMaterialPath[in]	���ݿ����ڵ��ļ���
//
void CHeatPreCal::SetMaterialPath(LPCTSTR pMaterialPath)
{
	HRESULT hr;
	CString strSQL;
	BOOL IsPathChange=FALSE;	//���ݿ�·������ǰ��·���Ƿ���ͬ
	CString FullDbFilePath;

	CalSupportSpan.m_strMaterialDbPath = m_MaterialPath+"Material.mdb";		//����֧���ܼ��
	CalSupportSpan.m_pConMaterial = theApp.m_pConMaterial;
	m_IMaterialCon = theApp.m_pConMaterial;

	return ;

	try
	{
		if(pMaterialPath==NULL)
		{
			if(m_IMaterialCon==NULL)
			{
				return;
			}
			else if(m_IMaterialCon->State & adStateOpen)
			{
				m_IMaterialCon->Close();
			}
			return;
		}
		
		if(m_MaterialPath==pMaterialPath)
			IsPathChange=TRUE;
		
		m_MaterialPath=pMaterialPath;
//		CCalcDll.m_MaterialPath=m_MaterialPath+"material.mdb";
		FullDbFilePath=m_MaterialPath+"material.mdb";
		
		if(m_IMaterialCon==NULL)
		{
			hr=m_IMaterialCon.CreateInstance(__uuidof(Connection));
			
			if(FAILED(hr))
				return;
		}
		
		if(m_IMaterialCon->State==adStateClosed)
		{
			strSQL = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source='"+FullDbFilePath+"'");
			m_IMaterialCon->Open(_bstr_t(strSQL),"","",-1);
			
			return;
		}

		if(IsPathChange==FALSE)
			return;

		strSQL =_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source='"+FullDbFilePath+"'");

		m_IMaterialCon->Close();
		m_IMaterialCon->Open(_bstr_t(strSQL),"","",-1);
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return;
	}
}

////////////////////////////////////////////////////////
//
// ����Material���ݿ����ڵ��ļ���
//
LPCTSTR CHeatPreCal::GetMaterialPath()
{
	return m_MaterialPath;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// ��Material���ݿ��е�CL_OLD_NEW����Ѱ����MatName���������Ӧ�Ĳ��ϡ�
// ������MatName��Ӧ���²��ϣ����δ�ҵ�������MatName��Ӧ�ľɲ�����
//
// MatName[in]		����Ѱ����֮��Ӧ�Ĳ�����
// pFindName[out]	�����ҵ��Ĳ�����������
// Num[out]			pFindName�������С
//
// ��������ɹ��ҵ�����TRUE�����򷵻�FALSE
// �������TRUE��pFindName����delete�ͷ��ڴ档
// �������FALSE��pFindName��Num��������
//
BOOL CHeatPreCal::FindMaterialNameOldOrNew(LPCTSTR MatName, CString *&pFindName, int &Num)
{
	int FindNum=0;
	CString MatStr;
	CString SQL;
	_RecordsetPtr IRecord;

	if(m_IMaterialCon==NULL || m_IMaterialCon->State==adStateClosed || MatName==NULL)
		return FALSE;

	MatStr=MatName;
	MatStr.TrimLeft();
	MatStr.TrimRight();
	if(MatStr.IsEmpty())
		return FALSE;

	try
	{
		IRecord.CreateInstance(__uuidof(Recordset));

		SQL.Format(_T("SELECT NEWCL FROM Material_OLD_NEW WHERE OLD='%s'"),MatStr);

		IRecord->Open(_variant_t(SQL),_variant_t((IDispatch*)m_IMaterialCon),
					  adOpenStatic,adLockOptimistic,adCmdText);

		if(IRecord->adoEOF && IRecord->BOF)
		{
			SQL.Format(_T("SELECT OLD FROM Material_OLD_NEW WHERE NEWCL='%s'"),MatStr);

			IRecord->Close();
			IRecord->Open(_variant_t(SQL),_variant_t((IDispatch*)m_IMaterialCon),
						  adOpenStatic,adLockOptimistic,adCmdText);

			if(IRecord->adoEOF && IRecord->BOF)
				return FALSE;
		}

		IRecord->MoveFirst();
		FindNum=0;
		while(!IRecord->adoEOF)
		{
			FindNum++;
			IRecord->MoveNext();
		}

		pFindName=new CString[FindNum];
		Num=FindNum;

		IRecord->MoveFirst();
		FindNum=0;
		while(!IRecord->adoEOF)
		{
			GetTbValue(IRecord,_variant_t((short)0),pFindName[FindNum]);
			FindNum++;
			IRecord->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
		return FALSE;
	}

	return TRUE;
}


//����:
//���ݵ����,����ȫ���.
//
bool CHeatPreCal::E_Predat_Cubage()
{
	CString sql;
	CString amou,v_pre,v_in,tv_pre,tv_in,tv_pro,v_pro;
	double f_v_pre,f_amou,f_v_in,f_v_pro,amount,pro_thi;
	_variant_t num,varTemp;
	_RecordsetPtr m_pRecordset;
	m_pRecordset.CreateInstance(__uuidof(Recordset));

	sql.Format(_T("SELECT * FROM pre_calc where EnginID='%s'"),EDIBgbl::SelPrjID);

	try
	{
		m_pRecordset->Open(_variant_t(sql),               
								theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
								adOpenDynamic,
								adLockOptimistic,
								adCmdText);

		while(!m_pRecordset->adoEOF)
		{
	
	//      ����Ҫ�Ѷ����,	
	//		num=m_pRecordset->GetCollect("c_num");
	//		if (num.vt != VT_NULL && num.vt != VT_EMPTY)
			{
				GetTbValue(m_pRecordset,_variant_t("c_amount"),amount);

				m_pRecordset->PutCollect(_variant_t("c_amou"),_variant_t(amount));

				GetTbValue(m_pRecordset,_variant_t("c_amou"),f_amou);

				GetTbValue(m_pRecordset,_variant_t("c_v_pre"),f_v_pre);

				GetTbValue(m_pRecordset,_variant_t("c_v_in"),f_v_in);

	    		tv_pre.Format(_T("%f"),f_v_pre*f_amou);
	    		tv_in.Format(_T("%f"),f_v_in*f_amou);

	    		m_pRecordset->PutCollect("c_tv_pre",_variant_t(tv_pre));
	    		m_pRecordset->PutCollect("c_tv_in",_variant_t(tv_in));

				GetTbValue(m_pRecordset,_variant_t("c_pro_thi"),pro_thi);

				if(pro_thi > 5.0)
				{
					GetTbValue(m_pRecordset,_variant_t("c_v_pro"),f_v_pro);

					tv_pro.Format("%f",f_v_pro*f_amou);
					m_pRecordset->PutCollect("c_tv_pro",_variant_t(tv_pro));
				}
				else
				{
					m_pRecordset->PutCollect("c_tv_pro",_variant_t("0"));
				}

				m_pRecordset->Update();

			}// if(num.vt!=VT_NULL)

			m_pRecordset->MoveNext();

		}//while(!m_pRecordset->adoEOF)
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return false;
	}
	return true;
}
//
short CHeatPreCal::GetThicknessRegular()
{
	//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
	//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
	CString strSQL,strMessage;
	_variant_t var;
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	try
	{
		strSQL = "SELECT * FROM [thicknessRegular] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
		pRs->Open(_variant_t(strSQL), GetConnect().GetInterfacePtr(),
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
			if( (m_thick1Max<m_thick1Start) )//|| () || || fabs(m_thick2Increment) <= 1e-6)
			{
				strMessage.Format("�ڲ㱣�����������%dmmС���ڲ㱣��������С���%dmm",m_thick1Max,m_thick1Start);
				AfxMessageBox(strMessage);
				return false;
			}
			if(m_thick2Max < m_thick2Start)
			{
				strMessage.Format("��㱣�����������%dmmС����㱣��������С���%dmm",m_thick2Max,m_thick2Start);
				AfxMessageBox(strMessage);
				return false;
			}
			if( m_thick1Increment <= 1e-6 )
			{
				strMessage.Format("�ڲ㱣�º����С����%dmm̫С.",m_thick1Increment);
				AfxMessageBox(strMessage);
				return false;
			}
			if( m_thick2Increment <= 1e-6 )
			{
				strMessage.Format("��㱣�º����С����%dmm̫С.",m_thick2Increment);
				AfxMessageBox(strMessage);
				return false;
			}
			if( m_thick1Start <= 1e-6 )
			{
				strMessage.Format("�ڲ㱣��������С���%dmm̫С.",m_thick1Start);
				AfxMessageBox(strMessage);
				return false;
			}
			if( m_thick2Start <= 1e-6 )
			{
				strMessage.Format("��㱣��������С���%dmm̫С.",m_thick2Start);
				AfxMessageBox(strMessage);
				return false;
			}
			if( m_thick1Max <= 1e-6 )
			{
				strMessage.Format("�ڲ㱣�����������%dmm̫С.",m_thick1Max);
				AfxMessageBox(strMessage);
				return false;
			}
			if( m_thick2Max <= 1e-6 )
			{
				strMessage.Format("��㱣�����������%dmm̫С.",m_thick2Max);
				AfxMessageBox(strMessage);
				return false;
			}
		}
		else
		{
			if( IDNO == AfxMessageBox("���º�ȹ������û�м�¼���Ƿ�Ĭ�ϵ����ݽ��м���?",MB_YESNO) )
			{
				return false;
			}
			m_thick1Start = 30;		//�ڲ㱣��������С���	
			m_thick1Max= 300;		//�ڲ㱣�����������
			m_thick1Increment=10;	//�ڲ㱣�º����С����

			m_thick2Start = 30;		//��㱣��������С���
			m_thick2Max = 300;			//��㱣�����������
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
		return false;
	}
	return true;
}
//
//
bool CHeatPreCal::InsertToReckonCost(double &ts, double &tb, double &thick1, double &thick2, double &cost_min, bool bIsFirst)
{
	CString strSQL;
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));

	try
	{
		//��Ϊ��һ��ʱ,��������еļ�¼.
		if( bIsFirst )
		{
			try
			{
				strSQL = "DELETE * FROM [ReckonCost]";
				theApp.m_pConnection->Execute(_bstr_t(strSQL),NULL,adCmdText);
			}
			catch(_com_error& e)
			{	
				//û�иñ�ʱ�� �򴴽�.
				if( e.Error() == DB_E_NOTABLE )
				{
					strSQL = "CREATE TABLE [ReckonCost] (ts double, tb double,thick1 double,thick2 double,cost_min double)";
					theApp.m_pConnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
				}			
			}
		}
		//����һ����¼��
		strSQL.Format("INSERT INTO [ReckonCost] VALUES(%f,%f,%f,%f,%f)",ts,tb,thick1,thick2,cost_min); 
		theApp.m_pConnection->Execute(_bstr_t(strSQL),NULL,adCmdText);
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return false;
	}
	return true;
}
////////////////////////
//���ݽ����¶�,ȡ���½ṹ������������ɢ���ܶ�
bool CHeatPreCal::GetHeatLoss(_RecordsetPtr& pRs,double &t/*�¶�*/, double &YearRun/*������*/, double &SeasonRun/*��������*/, double &QMax/*���ɢ���ܶ�,��������Сʱ��ȷ������,(�����л򼾽�����)*/)
{
	try
	{		
		YearRun = SeasonRun = QMax= 0;
		if( pRs->adoEOF && pRs->BOF )
		{
			//û�м�¼.
			return false;
		}
		//
		double nTempCur, nTempPre, nYearCur, nYearPre, nSeasonCur, nSeasonPre;
		bool   bFirst = true;
		for(pRs->MoveFirst(); !pRs->adoEOF; pRs->MoveNext() )
		{
			nTempCur = vtof(pRs->GetCollect(_variant_t("t")));
			nYearCur = vtof(pRs->GetCollect(_variant_t("YearRun"))); 
			nSeasonCur = vtof(pRs->GetCollect(_variant_t("SeasonRun")));

			if( nTempCur >= t )		//��ǰ��¼���¶�ֵ���ڻ����Ҫ���ҵ��¶�.
			{
				if( bFirst )  //��Ϊ��һ����¼ʱ 
				{
					YearRun = nYearCur;
					SeasonRun = nSeasonCur;
					if( hour_work )
					{	//�����й��� 
						QMax = nYearCur;
					}
					else
					{	//�������й���
						QMax = nSeasonCur;
					}
					if( fabs(QMax) < 1e-6 )		//����������ɢ���ܶ�Ϊ0ʱ(����û������), ����һ������.
					{
						QMax  = D_MAXTEMP;
					}
					return true;
				}
				else
				{
					YearRun = (t-nTempPre)/(nTempCur-nTempPre)*(nYearCur-nYearPre)+nYearPre;
					SeasonRun = (t-nTempPre)/(nTempCur-nTempPre)*(nSeasonCur-nSeasonPre)+nSeasonPre;
					if( hour_work )
					{	//�����й���
						QMax = nYearCur;
					}
					else
					{	//�������й���
						QMax = nSeasonCur;
					}
					if( fabs(QMax) < 1e-6 )		//����������ɢ���ܶ�Ϊ0ʱ(����û������), ����һ������.
					{
						QMax  = D_MAXTEMP;
					}
					return true;
				}
			}
			else
			{	//��ס��ǰ��ֵ
				nTempPre = nTempCur;
				nYearPre = nYearCur;
				nSeasonPre = nSeasonCur;
			}
			bFirst = false;
		}
		//ȡ���һ����¼��ֵ
		if( pRs->adoEOF )
		{
			YearRun = nYearCur;
			SeasonRun = nSeasonCur;
			if( hour_work )
			{	//�����й���
				QMax = nYearCur;
			}
			else
			{	//�������й���
				QMax = nSeasonCur;
			}
			if( fabs(QMax) < 1e-6 )		//����������ɢ���ܶ�Ϊ0ʱ(����û������), ����һ������.
			{
				QMax  = D_MAXTEMP;
			}

			return true;
		}
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return false;
	}
	return true;
}
//
// �ӳ���,��������ȡ������.
bool CHeatPreCal::GetConstantDefine(CString strTblName, CString ConstName, CString &ConstValue)
{
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));

	CString strSQL;
	strTblName.TrimRight();
	strSQL = "SELECT * FROM ["+strTblName+"] ";
	try
	{
		pRs->Open(_variant_t(strSQL), theApp.m_pConnectionCODE.GetInterfacePtr(), 
			adOpenStatic, adLockOptimistic, adCmdText);
		if( pRs->GetRecordCount() <= 0)
		{
			return false; 
		}
		strSQL = "ConstantDefine_Name='"+ConstName+"' " ;
		pRs->Find(_bstr_t(strSQL), NULL, adSearchForward);
		if( !pRs->adoEOF  )
		{
			ConstValue = vtos(pRs->GetCollect("ConstantDefine_VALUE"));
			return true;
		}
		else
			return false;
	}
	catch(_com_error& e)
	{
		ReportExceptionError( e);
		return false;
	}
	return true;
}
// �ڳ���,��������ȡ������.
bool CHeatPreCal::GetConstantDefine(CString strTblName, CString ConstName, double &dConstValue)
{
	CString strConstValue;
	if( GetConstantDefine(strTblName, ConstName, strConstValue) )
	{
		dConstValue = strtod( strConstValue, NULL );
		return true;
	}
	return false;
}

//����:
//���㱣�½ṹ����洫��ϵ��.
//����:
//dTs,������¶�.
//dD1,���²��⾶.
double CHeatPreCal::CalcAlfaValue(double dTs, double dD1)
{
	CString	sSQL,		//���SQL���.
			sVariableName,	//���еı�������
			sTmp,		//��ʱ����
			sFormula,	//�������ϵ���Ĺ�ʽ
			sIndex;		//����������ת��Ϊ�ַ���,��ʱ��
	double  dTmpVal;

	CString sCalTblName = "tmpCalcInsulThick";//��ʱ�����м����ı���.

	try
	{
		//���Ҽ�������洫��ϵ���Ĺ�ʽ.
		//{{�����ʽΪ��,���ǲ���÷���ϵ����.ֱ�ӷ���.
		if(m_pRsHeat_alfa->GetRecordCount() <= 0 )
		{
			//�����ڼ�¼ʱ.
			return 0;	
		}
		sIndex.Format("%d", m_nIndexAlpha);
		sSQL = "Index= "+sIndex+" ";
		m_pRsHeat_alfa->MoveFirst();
		m_pRsHeat_alfa->Find(_bstr_t(sSQL), NULL, adSearchForward);
		if( !m_pRsHeat_alfa->adoEOF )
		{
			//��λ���˸�������ֵ�ļ�¼.
			//����������ϵ���Ĺ�ʽ
			sFormula = vtos(m_pRsHeat_alfa->GetCollect(_variant_t("Formula")) );
			if( sFormula.IsEmpty() )
			{
				//û�м��㹫ʽ����������ȡ�÷���ϵ����
				//������ڼ���ÿ����¼�Ŀ�ʼ����ʼ��ʱ�����С�
				return 0;
			}
		}
		else
		{
			//���в����ڸ�����ֵ��Ӧ�ļ�¼.
			return 0;
		}
		//}}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return 0;
	}

	
	_RecordsetPtr pRsTmp;		//��ʱ�ļ�¼��.
	_RecordsetPtr pRsTmpVal;	//�������ļ�¼��

	pRsTmp.CreateInstance(__uuidof(Recordset));
	pRsTmpVal.CreateInstance(__uuidof(Recordset));
	try
	{
		//����ձ��е����м�¼��
//		theApp.m_pConWorkPrj->Execute(_bstr_t("DELETE * FROM ["+sCalTblName+"] "), NULL, adCmdText);
		//��ʼ���㹫ʽ�е���������ֵ
 		sSQL = "SELECT * FROM ["+sCalTblName+"] ";
		pRsTmp->Open(_variant_t(sSQL), theApp.m_pConWorkPrj.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if( pRsTmp->GetRecordCount() <= 0 )
		{	//�����һ��ʹ�øñ�,��������һ����¼.
			pRsTmp->AddNew();
		}
		else
		{	//�Ժ�ֻ�ı��һ����¼��ֵ
			pRsTmp->MoveFirst();
		}
		//������¶�
		pRsTmp->PutCollect(_variant_t("ts"), _variant_t(dTs));
		//���²��⾶,(��Ϊ���ϱ���ʱ, Ӧ����D2)
		pRsTmp->PutCollect(_variant_t("D1"), _variant_t(dD1));
		//�����¶�
		pRsTmp->PutCollect(_variant_t("ta"), _variant_t(this->Temp_env));
		//����
		pRsTmp->PutCollect(_variant_t("W"), _variant_t(this->speed_wind));
		//�ط��ٷ����ƽ�ڿ�ȡ�
		pRsTmp->PutCollect(_variant_t("B"), _variant_t(this->D0));
		//��������Ϻڶ�
		pRsTmp->PutCollect(_variant_t("Epsilon"), _variant_t(this->hedu));
		//�ܵ��⾶��
		pRsTmp->PutCollect(_variant_t("D0"), _variant_t(this->D0));
		//�ܵ���ƽ�ڵķֽ�ֵ��
		pRsTmp->PutCollect(_variant_t("Dmax"), _variant_t(this->m_dMaxD0));


		pRsTmp->Update();
		pRsTmp->Close();
		 
		//
		for(m_pRsHeat_alfa->MoveFirst(); !m_pRsHeat_alfa->adoEOF; m_pRsHeat_alfa->MoveNext())
		{
			sVariableName = vtos(m_pRsHeat_alfa->GetCollect(_variant_t("Variable_Name_Desc")));
			if( !sVariableName.IsEmpty() && sVariableName.CompareNoCase("ALPHA")  )	
			{	
				//�������ϵ�����м乫ʽ��
				sTmp = vtos(m_pRsHeat_alfa->GetCollect(_variant_t("Formula")));
				sSQL = "UPDATE ["+sCalTblName+"] SET "+sVariableName+" = "+sTmp+" ";
				theApp.m_pConWorkPrj->Execute(_bstr_t(sSQL), NULL, adCmdText);
			}
		}
		sSQL =    "SELECT ("+sFormula+") AS val  FROM ["+sCalTblName+"] ";
		pRsTmpVal->Open(_variant_t(sSQL), theApp.m_pConWorkPrj.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		//����ϵ��ֵ
		dTmpVal = vtof( pRsTmpVal->GetCollect(_variant_t("val"))); 
		if( dTmpVal > 0 )
		{
			this->co_alf = dTmpVal;
			return 1;				//�ù�ʽ�������һ����Ч�ķ���ϵ��.�ɹ�����
		}
	}
	catch(_com_error)
	{
		return false;
	}
	return 0;
}
//���ܣ�	
//��һ�����и���ָ�������ݣ����ֵ��
//���磺���ݱ��²��⾶������ϵ����͡�
//������ڵ��豸��ܵ����½ṹ����洫��ϵ����
//
bool CHeatPreCal::GetAlfaValue(_RecordsetPtr pRs, CString sSearchFieldName,  CString sValFieldName, double dCompareVal,double& dAlfaVal)
{
	dAlfaVal = 0.0;			//�������û�м�¼,���ֶ���Ϊ��ʱ,����ֵΪ0��
	if( pRs->adoEOF && pRs->BOF )
	{
		return false;
	}
	sValFieldName.TrimLeft();
	sValFieldName.TrimRight();
	if(	sValFieldName.IsEmpty() || sSearchFieldName.IsEmpty() )
	{
		//Ҫ���ҵ��ֶ���,��ؼ��ֶ���Ϊ�ա�
		return false;
	}
	double  dCurDia,	//��ǰ���Ᵽ�º�
			dPreDia,	//ǰһ�ε��Ᵽ�º�
			dCurValue,	//���β��ҵ���ֵ��
			dPreValue;	//ǰһ�ε�ֵ

	bool	bFirst = true;	//��ǲ��ҵĴ���.

	for(pRs->MoveFirst(); !pRs->adoEOF; pRs->MoveNext())
	{
		dCurDia   = vtof(pRs->GetCollect(_variant_t(sSearchFieldName)));
		dCurValue = vtof(pRs->GetCollect(_variant_t(sValFieldName)));
		if( dCurDia >= dCompareVal )
		{	
			if( bFirst )
			{
				//��һ��ʱ��
				dAlfaVal = dCurValue;
				return true;
			}
			else
			{	//���ֵ.
				dAlfaVal = (dCompareVal-dPreDia)/(dCurDia-dPreDia)*(dCurValue-dPreValue)+dPreValue;
				return true;
			}
		}
		else
		{
			//���浱ǰ��ֵ���������²��ҡ�
			dPreDia = dCurDia;
			dPreValue = dCurValue;
		}
		bFirst = false;
	}
	//û���ҵ����������ļ�¼��ȡ���һ����¼��ֵ��
	if(pRs->adoEOF)
	{
		dAlfaVal = dCurValue;
		return true;
	}
	return true;
}

/////////////
//����:
//������㱣�º�ȵĹ�ʽ,��ʼ������,������.
//����ɹ�����1.����Ϊ0.
//������
//strFormula    ���º�ȼ��㹫ʽ.
//strTsFormula	������¶ȼ��㹫ʽ
//strQFormula	ɢ���ܶȼ��㹫ʽ
//thick1		���º�(�ڲ㡣) 
//thick2		��㱣�º�
short CHeatPreCal::CalcInsulThick(CString strThickFormula,CString strThickFormula2,CString strTbFormula, CString strTsFormula, CString strQFormula ,double &dThick1, double &dThick2)
{
	CString	strTmp,		//��ʱ����
			strSQL;		//����SQL���.


	double	thick1, //�ڱ��º�
			thick2,//�Ᵽ�º�
			thick, //���㱣�º�.
			ts,		//������¶ȡ�
			tb;		//�����¶�

	double  condu_r,
			condu1_r,condu2_r,condu_out,cost_q,cost_s,cost_tot,
			cost_min=0;//���ٵļ۸�.
	double  D1,
			D2;

	double nYearVal, nSeasonVal,
		   Q;			//���½ṹ�����ɢ���ܶ�
	double QMax;		//�������ɢ���ܶ�
	bool   flg=true;	//����ɢ���ܶȵ��ж�
	
//	double MaxTb = in_tmax*m_HotRatio;//�������������¶�=�Ᵽ�²��ϵ�����¶� * һ��ϵ��(��̹涨Ϊ0.9).

	double  MaxTb = in_tmax;//�������������¶�,�ӱ��²��ϲ�����(a_mat)��ȡMAT_TMAX�ֶ�,���޸ĸ��ֶν��п��ƽ����¶�ֵ.

	if( strThickFormula.IsEmpty() )	//���㱣�º�Ĺ�ʽΪ��
	{
		return 0;
	}
	//ѡ����ѡ�����ж�ɢ���ܶ�,����
	if(bIsHeatLoss)
	{
		//����ý����¶���������ɢ���ܶ�
		if( !GetHeatLoss(m_pRsHeatLoss, Temp_pip, nYearVal, nSeasonVal, QMax) )
		{		
		}
	}
	//

	
	_RecordsetPtr pRsTmp;		//��ʼ��ǰ���м�����ļ�¼��.
	_RecordsetPtr pRsTmpVal;	//�������ļ�¼��

	CString strCalTblName = "tmpCalcInsulThick";//��ʱ�����м����ı���.

	pRsTmp.CreateInstance(__uuidof(Recordset));
	pRsTmpVal.CreateInstance(__uuidof(Recordset));
	try
	{
		//����ձ��е����м�¼��
//		theApp.m_pConWorkPrj->Execute(_bstr_t("DELETE * FROM ["+sCalTblName+"] "), NULL, adCmdText);
		//��ʼ���㹫ʽ�е���������ֵ
 		strSQL = "SELECT * FROM ["+strCalTblName+"] ";
		pRsTmp->Open(_variant_t(strSQL), theApp.m_pConWorkPrj.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if( pRsTmp->GetRecordCount() <= 0 )
		{	//�����һ��ʹ�øñ�,��������һ����¼.
			pRsTmp->AddNew();
		}
		else
		{	//�Ժ�ֻ�ı��һ����¼��ֵ
			pRsTmp->MoveFirst();
		}
		//��ʼ���㱣�º��ʱҪ�õ���,�����м����.
		
		//���½ṹ������¶�
//		pRsTmp->PutCollect(_variant_t("ts"), _variant_t(dTs));
		//���ϱ����������洦�¶�
		pRsTmp->PutCollect(_variant_t("tb"), _variant_t());
		//���²��⾶,(��Ϊ���ϱ���ʱ, Ӧ����D2)
//		pRsTmp->PutCollect(_variant_t("D1"), _variant_t(dD1));
		//�����¶�
		pRsTmp->PutCollect(_variant_t("ta"), _variant_t(this->Temp_env));
		//����
		pRsTmp->PutCollect(_variant_t("W"), _variant_t(this->speed_wind));
		//�ط��ٷ����ƽ�ڿ�ȡ�
		pRsTmp->PutCollect(_variant_t("B"), _variant_t(this->D0));
		//��������Ϻڶ�
		pRsTmp->PutCollect(_variant_t("Epsilon"), _variant_t(this->hedu));
		//�ܵ��⾶��
		pRsTmp->PutCollect(_variant_t("D0"), _variant_t(this->D0));
		//�ܵ���ƽ�ڵķֽ�ֵ��
		pRsTmp->PutCollect(_variant_t("Dmax"), _variant_t(this->m_dMaxD0));

		//���㱣�º��ʱ�õ�������������
		//
		//���Ȳ�����Ʒ����ϵϵ����W/(m*C)
		pRsTmp->PutCollect(_variant_t("lamda"), _variant_t());
		//�ȼ�
		pRsTmp->PutCollect(_variant_t("Ph"), _variant_t());
		//����ӵ��ϵ��.
		pRsTmp->PutCollect(_variant_t("Ae"), _variant_t());
		//�豸�͹ܵ��¶�
		pRsTmp->PutCollect(_variant_t("t"), _variant_t(this->Temp_pip));
		//������ʱ��
		pRsTmp->PutCollect(_variant_t("tao"), _variant_t());
		//���²㵥λ���,���ϱ����ڲ㵥λ���
		pRsTmp->PutCollect(_variant_t("P1"), _variant_t());
		//���ϱ�����㵥λ���
		pRsTmp->PutCollect(_variant_t("P2"), _variant_t());
		//�����㵥λ���
		pRsTmp->PutCollect(_variant_t("P3"), _variant_t());
		//���¹���Ͷ�ʴ������̯��
		pRsTmp->PutCollect(_variant_t("S"), _variant_t());
		//���ϱ�������⾶
		pRsTmp->PutCollect(_variant_t("D2"), _variant_t());
		//���½ṹ�����ɢ���ܶ�
		pRsTmp->PutCollect(_variant_t("Q"), _variant_t());
		//Բ����
		pRsTmp->PutCollect(_variant_t("pi"), _variant_t((double)3.1415927));
		//�ܵ�ͨ��֧���ܴ����ȣ����䣩��ʧ�ĸ���ϵ������ȡ1.05 ~ 1.15
		pRsTmp->PutCollect(_variant_t("Kr"), _variant_t((double)1.1));
		//������ܵ�����Һͣ��ʱ��
		pRsTmp->PutCollect(_variant_t("taofr"), _variant_t());
		//�����ڹ��ڶ����¶�
		pRsTmp->PutCollect(_variant_t("tfr"), _variant_t());
		//ÿ�׹ܳ��������
		pRsTmp->PutCollect(_variant_t("V"), _variant_t());
		//�����ܶ�
		pRsTmp->PutCollect(_variant_t("ro"), _variant_t());
		//���ʵı���
		pRsTmp->PutCollect(_variant_t("C"), _variant_t());
		//ÿ�׹ܳ��ܱ����
		pRsTmp->PutCollect(_variant_t("Vp"), _variant_t());
		//�ܲ��ܶ�
		pRsTmp->PutCollect(_variant_t("rop"), _variant_t());
		//�ܲĵı���
		pRsTmp->PutCollect(_variant_t("Cp"), _variant_t());
		//�����ܽ���
		pRsTmp->PutCollect(_variant_t("Hfr"), _variant_t());
		//���²��ȡ�
		pRsTmp->PutCollect(_variant_t("delta"), _variant_t());
		//���ϱ����ڲ���
		pRsTmp->PutCollect(_variant_t("delta1"), _variant_t());
		//���ϱ��������
		pRsTmp->PutCollect(_variant_t("delta2"), _variant_t());
		//
		pRsTmp->Update();
		
		//��ʼ����

		//�����ڲ㱣�º�Ĺ�ʽΪ��ʱ.��Ϊ���㴦��
		if( strThickFormula2.IsEmpty() )
		{
			//�����ڲ㱣�º�Ĺ�ʽΪ��ʱ.��Ϊ���㴦��

			//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
			//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
			//�ӱ��º�ȹ����thicknessRegular���
			//
			for(thick=m_thick2Start;thick<=m_thick2Max;thick+=m_thick2Increment)
			{
			//	do temp_pip_one with thick,ts
				temp_pip_one(thick,ts);
				condu_r=in_a0+in_a1*(ts+Temp_pip)/2.0+in_a2*(ts+Temp_pip)*(ts+Temp_pip)/4.0;
				condu_out=co_alf;
				D1=2.0*thick+D0;

				cost_q=7.2*pi*Mhours*heat_price*Yong*fabs(Temp_pip-Temp_env)*1e-6
						/(1.0/condu_r*log(D1/D0)+2000.0/condu_out/D1);

				cost_s=(pi/4.0*(D1*D1-D0*D0)*in_price*1e-6+pi*D1*pro_price*1e-3)*S;
				cost_tot=cost_q+cost_s;



				pRsTmp->PutCollect(_variant_t("ALPHA"), _variant_t(condu_out));

				//���Ȳ�����Ʒ����ϵϵ����W/(m*C)
				pRsTmp->PutCollect(_variant_t("lamda"), _variant_t(condu_r));

				//���²���⾶
				pRsTmp->PutCollect(_variant_t("D1"), _variant_t(D1));
				
				try
				{
					//
					//���²��ȡ�
					pRsTmp->PutCollect(_variant_t("delta"), _variant_t(thick));
					//���½ṹ������¶�
					pRsTmp->PutCollect(_variant_t("ts"), _variant_t(ts));
					//���ϱ����������洦�¶�
					tb = ts;
					pRsTmp->PutCollect(_variant_t("tb"), _variant_t(tb));

					pRsTmp->Update();//����;

					//��������¶ȣ������˫����Ͻ����¶ȡ�
					//strSQL = "SELECT ("+strTsFormula+") AS TSVal FROM ["+strCalTblName+"] ";

					strSQL = "UPDATE ["+strCalTblName+"] SET ts= ("+strTsFormula+") ";
					theApp.m_pConWorkPrj->Execute(_bstr_t(strSQL), NULL, adCmdText);

					//��ñ��κ�ȵ�ɢ���ܶ� 
					//��ʽ:
					//�����ݿ��л�ã����ݱ��½ṹȡ����Ӧ�ļ��㹫ʽ
					strSQL = "UPDATE ["+strCalTblName+"] SET Q=("+strQFormula+") ";
					theApp.m_pConWorkPrj->Execute(_bstr_t(strSQL), NULL, adCmdText);
				}
				catch(_com_error& e)
				{
					AfxMessageBox ( e.Description() );					
				}


				if(ts<MaxTs && (cost_tot<cost_min || fabs(cost_min)<1E-6))
				{
					flg = true;
					//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.
					if(bIsHeatLoss)
					{
						//��ñ��κ�ȵ�ɢ���ܶ�
						//��ʽ:Ϊ�˼��ݱ�����㣬�¶Ȳ�Ӧȡ����ֵ by ligb on 2010.06.14
						//(�����¶�-�����¶�)/((���²��⾶/2000.0/���ϵ�����*ln(���²��⾶/�ܵ��⾶)) + 1/����ϵ��)
						Q = fabs(Temp_pip-Temp_env)/(D1/2000.0/condu_r*log(D1/D0) + 1.0/co_alf);
						if(fabs(QMax) <= 1E-6 || Q <= QMax)
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
					}
				}
			}

		}
		else
		{
			
			//test
			//thick1�ڲ㱣�º�ȡ�thick2��㱣�º��
			//thick1Start�ڲ㱣��������С���,thick1Max�ڲ㱣�����������,thick1Increment�ڲ㱣�º����С����
			//thick2Start��㱣��������С���,thick2Max��㱣�����������,thick2Increment��㱣�º����С����
			//�ӱ��º�ȹ����thicknessRegular���

//			theApp.m_pConWorkPrj->BeginTrans();	//��ʼ����.
			for(thick1=m_thick1Start;thick1<=m_thick1Max;thick1+=m_thick1Increment)
			{
				for(thick2=m_thick2Start;thick2<=m_thick2Max;thick2+=m_thick2Increment)
				{
					temp_pip_com(thick1,thick2,ts,tb);
					condu1_r=in_a0+in_a1*(Temp_pip+tb)/2.0+in_a2*(Temp_pip+tb)*(Temp_pip+tb)/4.0;
					condu2_r=out_a0+out_a1*(tb+ts)/2.0+out_a2*(tb+ts)*(tb+ts)/4.0;
					condu_out=co_alf;
					D1=2*thick1+D0;
					D2=2*thick2+D1;
					//test	{{
					//���㱣�½ṹ����洫��ϵ��.
					condu_out = CalcAlfaValue(ts, tb);
					if( fabs(condu_out) < 1e-1 )
					{	//Ϊ��ʱ�ò�����õĴ���ϵ��.
						condu_out = co_alf;
					}
					pRsTmp->PutCollect(_variant_t("ALPHA"), _variant_t(condu_out));

					//���Ȳ�����Ʒ����ϵϵ����W/(m*C)
					pRsTmp->PutCollect(_variant_t("lamda"), _variant_t(condu1_r));
					//���Ȳ�����Ʒ����ϵϵ����W/(m*C)
					pRsTmp->PutCollect(_variant_t("lamda1"), _variant_t(condu1_r));
					//���Ȳ�����Ʒ����ϵϵ����W/(m*C)
					pRsTmp->PutCollect(_variant_t("lamda2"), _variant_t(condu2_r));

					//test }}

					//���ϱ��½ṹ���ڲ㱣���⾶
					pRsTmp->PutCollect(_variant_t("D1"), _variant_t(D1));
					//���ϱ��½ṹ����㱣���⾶
					pRsTmp->PutCollect(_variant_t("D2"), _variant_t(D2));

					cost_q=7.2*pi*Mhours*heat_price*Yong*fabs(Temp_pip-Temp_env)*1e-6
							/(1.0/condu1_r*log(D1/D0)+1.0/condu2_r*log(D2/D1)+2000.0/condu_out/D2);

					cost_s=(pi/4.0*(D1*D1-D0*D0)*in_price*1e-6+pi/4.0*(D2*D2-D1*D1)*out_price*1e-6
							+pi*D2*pro_price*1e-3)*S;

					cost_tot=cost_q+cost_s;

					
					try
					{
						//
						//���²��ȡ�
						pRsTmp->PutCollect(_variant_t("delta"), _variant_t(thick1));
						//���ϱ����ڲ���
						pRsTmp->PutCollect(_variant_t("delta1"), _variant_t(thick1));
						//���ϱ��������
						pRsTmp->PutCollect(_variant_t("delta2"), _variant_t(thick2));
						//���½ṹ������¶�
						pRsTmp->PutCollect(_variant_t("ts"), _variant_t(ts));
						//���ϱ����������洦�¶�
						pRsTmp->PutCollect(_variant_t("tb"), _variant_t(tb));

						pRsTmp->Update();//����;

						//��������¶ȣ������˫����Ͻ����¶ȡ�
						//strSQL = "SELECT ("+strTsFormula+") AS TSVal FROM ["+strCalTblName+"] ";

						strSQL = "UPDATE ["+strCalTblName+"] SET ts= ("+strTsFormula+") ";
						theApp.m_pConWorkPrj->Execute(_bstr_t(strSQL), NULL, adCmdText);

						//��ñ��κ�ȵ�ɢ���ܶ� 
						//��ʽ:
						//�����ݿ��л�ã����ݱ��½ṹȡ����Ӧ�ļ��㹫ʽ
						strSQL = "UPDATE ["+strCalTblName+"] SET Q=("+strQFormula+") ";
						theApp.m_pConWorkPrj->Execute(_bstr_t(strSQL), NULL, adCmdText);

						//���º��
				/**/	//	strSQL = "UPDATE ["+strCalTblName+"] SET delta=("+strThickFormula+") ";
					//	theApp.m_pConWorkPrj->Execute(_bstr_t(strSQL), NULL, adCmdText);

				/*		//��������¶ȣ������˫����Ͻ����¶ȡ�
 						strSQL = "SELECT ("+strTsFormula+") AS TSVal FROM ["+strCalTblName+"] ";
						if(pRsTmpVal->State == adStateOpen)
						{
							pRsTmpVal->Close();
						}
						pRsTmpVal->Open(_bstr_t(strSQL), theApp.m_pConWorkPrj.GetInterfacePtr(),
										adOpenStatic, adLockOptimistic, adCmdText);
						if(pRsTmpVal->GetRecordCount() <= 0 )
						{
							//����������¶�ʧ�ܡ�
							pRsTmp->PutCollect(_variant_t("ts"), _variant_t((double)35));
						}
						else
						{
							ts = vtof(pRsTmpVal->GetCollect(_T("TSVal")));
							pRsTmp->PutCollect(_variant_t("ts"), _variant_t(ts));
						}

						//������¶ȡ�tb
						if( !strTbFormula.IsEmpty() )
						{
							strSQL = "SELECT ("+strTbFormula+") AS TBVal FROM ["+strCalTblName+"] ";
							if(pRsTmpVal->State == adStateOpen)
							{
								pRsTmpVal->Close();
							}
							pRsTmpVal->Open(_bstr_t(strSQL), theApp.m_pConWorkPrj.GetInterfacePtr(),
											adOpenStatic, adLockOptimistic, adCmdText);
							if(pRsTmpVal->GetRecordCount() <= 0 )
							{
								//��������¶�ʧ��
								pRsTmp->PutCollect(_variant_t("tb"), _variant_t((double)35));
							}
							else
							{
								tb = vtof(pRsTmpVal->GetCollect(_T("TBVal")));
								pRsTmp->PutCollect(_variant_t("tb"),_variant_t(tb));
							}

						}

						//��ñ��κ�ȵ�ɢ���ܶ�
						//��ʽ:
						//�����ݿ��л�ã����ݱ��½ṹȡ����Ӧ�ļ��㹫ʽ
						strSQL = "SELECT ("+strQFormula+") AS QVal FROM ["+strCalTblName+"] ";
						if(pRsTmpVal->State == adStateOpen)
						{
							pRsTmpVal->Close();
						}
						pRsTmpVal->Open(_bstr_t(strSQL), theApp.m_pConWorkPrj.GetInterfacePtr(),
										adOpenStatic, adLockOptimistic, adCmdText);
						if(pRsTmpVal->GetRecordCount() <= 0 )
						{
							//����ɢ���ܶ�ʧ�ܡ�
							//��һ��Ĭ�ϵ�ֵ,�������ݿ����á�
							pRsTmp->PutCollect(_variant_t("Q"), _variant_t(QMax));
						}
						else
						{
							Q = vtof(pRsTmpVal->GetCollect(_T("QVal")));
							pRsTmp->PutCollect(_variant_t("Q"), _variant_t(Q));
						}
						
						//���º��
						strSQL = "SELECT ("+strThickFormula+") AS Thick FROM ["+strCalTblName+"] ";
						if( pRsTmpVal->State == adStateOpen)
						{
							pRsTmpVal->Close();
						}

						pRsTmpVal->Open(_bstr_t(strSQL), theApp.m_pConWorkPrj.GetInterfacePtr(),
									adOpenStatic, adLockOptimistic, adCmdText);
						if( pRsTmpVal->GetRecordCount() <= 0 )
						{
							return 0; 
						}
						dThick1 = vtof(pRsTmpVal->GetCollect(_T("Thick")));
			/*	*/	
					}
					catch(_com_error& e)
					{
						AfxMessageBox(e.Description());
						return 0;
					}
					//tb<350)
					if((ts<MaxTs && tb<MaxTb) && (fabs(cost_min)<1E-6 || cost_min>cost_tot))
					{
						flg = true;
						//ѡ����ѡ����ж�ɢ���ܶ�,�ͽ��бȽ�.
						if(bIsHeatLoss)
						{
							//��ñ��κ�ȵ�ɢ���ܶ�
							//��ʽ:
							//(�����¶�-�����¶�)/((�Ᵽ�²��⾶/2000.0)*(1.0/�ڱ��²��ϵ�����*ln(�ڱ��²��⾶/�ܵ��⾶) + 1.0/�Ᵽ�²��ϵ�����*ln(�Ᵽ�²��⾶/�ڱ��²��⾶) ) + 1/����ϵ��)
							Q = (Temp_pip-Temp_env) / ( (D2/2000.0) * (1.0/condu1_r*log(D1/D0)+1.0/condu2_r*log(D2/D1)) + 1.0/co_alf );

							if(fabs(QMax) <= 1E-6 || Q <= QMax)
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
							dThick1=thick1;
							dThick2=thick2;
						//	ts_resu=ts;
						//	tb_resu=tb;//
						}
					}
				}
			}
	//		theApp.m_pConWorkPrj->CommitTrans();	//�ύ����.
	//		theApp.m_pConWorkPrj->RollbackTrans();
		}//˫�㱣��

	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return 1;
}
//���ܣ�
//��ü��㵱ǰ��¼�ļ��㱣�º�Ĺ�ʽ.����������¶ȵĹ�ʽ,����ɢ���ܶȵĹ�ʽ
//	nFormulaIndex	//�ṹ��Ӧ�Ĺ�ʽ����.
//	strFormula,		//���ؼ��㹫ʽ.
int CHeatPreCal::GetCalcFormula(int nFormulaIndex, CString &strFormula, CString& strTsFormula, CString& strQFormula)
{
	CString	strTmp,		//��ʱ����
			strSQL;		//����SQL���.

	int		nMethod=0;	//���㷽��������
	try
	{
		//��ԭʼ���ݿ��л�ü��㷽����������
		nMethod = vtoi(IRecCalc->GetCollect(_T("c_Method_Index")));

		if(m_pRsCalcThick->GetRecordCount() <= 0 )
		{
			return 0;
		}
		m_pRsCalcThick->Filter = (long)adFilterNone;
		strSQL.Format("InsulationCalcMethodIndex=%d AND Index=%d",nMethod,nFormulaIndex);
		m_pRsCalcThick->Filter = variant_t(strSQL);
		if( !m_pRsCalcThick->adoEOF )		 
		{
			//�ҵ����¼��㷽����Ӧ�Ĺ�ʽ.
			strFormula = vtos(m_pRsCalcThick->GetCollect(_T("Formula")));

			if( strFormula.IsEmpty() )
			{  
				//����Ĺ�ʽΪ��
				return 0;
			}
		}
		else
		{
			return 0;		//�Ҳ������¼��㷽������Ӧ�Ĺ�ʽ.
		}
		//�����������tsֵ�ļ��㹫ʽ
		m_pRsCalcThick->Filter = (long)adFilterNone;
		//��Ϊһ������,�涨20Ϊ����������¶ȵķ���������.
		nMethod = 20;			
		strSQL.Format("InsulationCalcMethodIndex=%d AND Index=%d",nMethod,nFormulaIndex);
		m_pRsCalcThick->Filter = _variant_t(strSQL);
		if(m_pRsCalcThick->GetRecordCount() <= 0 )
		{
			return 0;
		}
		strTsFormula = vtos(m_pRsCalcThick->GetCollect(_T("Formula")));
		//
		//���Ҽ���ɢ���ܶ�(Q)�ļ��㹫ʽ
		m_pRsCalcThick->Filter = (long)adFilterNone;
		//��Ϊһ������,�涨21Ϊ����ɢ���ܶȵķ���������.
		nMethod = 21;	
		strSQL.Format("InsulationCalcMethodIndex=%d AND Index=%d",nMethod, nFormulaIndex);
		m_pRsCalcThick->Filter = _variant_t(strSQL);
		if(m_pRsCalcThick->GetRecordCount() <= 0 )
		{
			return 0;
		}
		strQFormula = vtos(m_pRsCalcThick->GetCollect(_variant_t("Formula")));

		//ȡ�����еĹ���
		m_pRsCalcThick->Filter = (long)adFilterNone;
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return 0;
	}
	
	return 1;
}

//-----------------------------------------------------
//���ܣ�
//�ڹܵ��������и��ݹܵ��⾶�ͱں񣬻����Ӧ�Ĺ��أ���ˮ��.
short CHeatPreCal::GetPipeWeithAndWaterWeith(_RecordsetPtr &pRs, const double dPipe_Dw, const double dPipe_S, CString strField1, double &dValue1,CString strField2,  double &dValue2)
{
	try
	{
		if(pRs == NULL || pRs->State == adStateClosed || pRs->GetRecordCount() <= 0 )
		{	//��¼��Ϊ��
			return 0;
		}

		CString strFilter;
		strFilter.Format("PIPE_DW=%f AND PIPE_S=%f",dPipe_Dw, dPipe_S);
		pRs->Filter = (long)adFilterNone;		//ɾ����ǰɸѡ�������ָ��鿴�����м�¼��

		pRs->Filter = _variant_t(strFilter);	//ɸѡ�������⾶�ͱں�ļ�¼.

		while(!pRs->adoEOF)
		{
			try
			{
				//��λ���⾶�ͱں��Ӧ�ļ�¼
				//
				dValue1 = vtof(pRs->GetCollect(_variant_t(strField1) ));
				//ȡ�������ֶ�����Ӧ��ֵ
				dValue2 = vtof(pRs->GetCollect(_variant_t(strField2) ));

				pRs->Filter = (long)adFilterNone;		//ɾ����ǰɸѡ�������ָ��鿴�����м�¼��			
				return 1;
			}
			catch(_com_error &e)
			{
				CString TempStr;
				TempStr.Format(_T("ԭʼ�������Ϊ%d�ļ�¼�ڶ�ȡ\"�ܵ�������(A_PIPE)\"��ʱ����\r\n"),
							   m_nID );
				if(Exception::GetAdditiveInfo())
				{
					TempStr+=Exception::GetAdditiveInfo();
				}
				ExceptionInfo(TempStr);

				Exception::SetAdditiveInfo(NULL);
				ReportExceptionErrorLV2(e);

				IRecCalc->MoveNext();
				continue;
			}
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV2(e);
		pRs->Filter = (long)adFilterNone;		//ɾ����ǰɸѡ�������ָ��鿴�����м�¼��			
		return 0;
	}
	//û�г��������⾶�ͱں�ļ�¼
	pRs->Filter = (long)adFilterNone;		//ɾ����ǰɸѡ�������ָ��鿴�����м�¼��			
	return 0;
}

//------------------------------------------------------------------
// DATE         :[2005/08/02]
// Parameter(s) :
// Return       :
// Remark       :�ڹ淶�������ó�������ָ�����ֵ.
//------------------------------------------------------------------
BOOL CHeatPreCal::SetConstantDefine(CString strTblName, CString ConstName, _variant_t ConstValue)
{
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));

	CString strSQL;
	strTblName.TrimRight();
	strSQL = "SELECT * FROM ["+strTblName+"] ";
	try
	{
		pRs->Open(_variant_t(strSQL), theApp.m_pConnectionCODE.GetInterfacePtr(), 
			adOpenStatic, adLockOptimistic, adCmdText);
		if( pRs->GetRecordCount() <= 0)
		{
			return false; 
		}
		strSQL = "ConstantDefine_Name='"+ConstName+"' " ;
		pRs->Find(_bstr_t(strSQL), NULL, adSearchForward);
		if( !pRs->adoEOF  )
		{
			pRs->PutCollect(_variant_t("ConstantDefine_VALUE"),ConstValue);
			pRs->Update();
			return true;
		}
		else
		{
			return false;
		}
	}
	catch(_com_error& e)
	{
		ReportExceptionError( e);
		return false;
	}
	return TRUE;
}

//�ڱ�׼���в��ұ��²���
BOOL CHeatPreCal::FindStandardMat(_RecordsetPtr& pRsCurMat,CString strMaterialName)
{
	try
	{
		
		if (NULL == m_pRs_CodeMat || m_pRs_CodeMat->State != adStateOpen || (m_pRs_CodeMat->adoEOF && m_pRs_CodeMat->BOF))
		{
			return FALSE;
		}
		m_pRs_CodeMat->MoveFirst();
		if (!LocateFor(m_pRs_CodeMat,_variant_t("MAT_NAME"),CFoxBase::EQUAL, _variant_t(strMaterialName)))
		{
			return FALSE;
		}
		pRsCurMat->AddNew();
		pRsCurMat->Update();
		for (long i=0, nFieldCount=m_pRs_CodeMat->Fields->GetCount(); i < nFieldCount; i++)
		{
			pRsCurMat->PutCollect(_variant_t(i),m_pRs_CodeMat->GetCollect(_variant_t(i)));
		}
		pRsCurMat->PutCollect(_variant_t("EnginID"),_variant_t(m_ProjectName));
		pRsCurMat->Update();
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;	
	}
	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/09/13]
// Parameter(s) :
// Return       :
// Remark       :���ݽ������ƻ������ʵ��ܶ�
//------------------------------------------------------------------
double CHeatPreCal::GetMediumDensity(const CString strMediumName, CString* pMediumName)
{
	double	dMediumDensity = 0.0;   //�����ܶ�
	CString strSQL;					//SQL���		
	
	if (strMediumName.IsEmpty())
	{
		return 0.0;
	}
	try
	{
		_RecordsetPtr pRs;
		pRs.CreateInstance(__uuidof(Recordset));
		strSQL = "SELECT * FROM [MediumSpec] WHERE MediumID IN (SELECT MediumID FROM [MediumAlias] WHERE MediumAlias='"+strMediumName+"') ";
		pRs->Open(_variant_t(strSQL), theApp.m_pConMedium.GetInterfacePtr(),
			adOpenStatic, adLockOptimistic, adCmdText);
		if (pRs->adoEOF && pRs->BOF)
		{
			//���ñ���,ֱ���ڽ����ܶȱ��в���.
			pRs->Close();
			strSQL = "SELECT * FROM [MediumSpec] WHERE MediumName='"+strMediumName+"' ";
			pRs->Open(_variant_t(strSQL), theApp.m_pConMedium.GetInterfacePtr(),
				adOpenStatic, adLockOptimistic, adCmdText);
			if (pRs->adoEOF && pRs->BOF)
			{
				return 0.0;
			}
		}//
		dMediumDensity  = vtof( pRs->GetCollect(_variant_t("Density")) );
		if ( pMediumName != NULL )
		{
			*pMediumName = vtos( pRs->GetCollect(_variant_t("MediumName")));
		}
	}
	catch (_com_error& e) {
		AfxMessageBox(e.Description());
		return 0.0;
	}
	return dMediumDensity;
}

//------------------------------------------------------------------
// DATE         :[2005/09/15]
// Parameter(s) :dw:			�ܵ����⾶
//				:s:				�ں�
//				:dMediumDensity:�ܵ��ڽ��ʵ��ܶ� 
//				:temp:			���ʵ��¶�
//				:dInsuWei:		���²��ϵ�����
// Return       :���ݱ�׼������ļ��
// Remark       :ʹ�ö�̬�����֧���ܵļ�� 
//------------------------------------------------------------------
double CHeatPreCal::GetSupportSpan(const double dw, const double s, const CString strMaterialName,const double dPressure,const double dMediumDensity, const double temp, const double dInsuWei)
{
	double dLmax = 0.0;		//֧���ܵļ��

	CString strDlgCaption;	//�Ի���ı���
	double	LmaxR=0.0;
	double	LmaxS=0.0;
	double	fQ=0.0;
	double	fQp=0.0;
	int		TableID = -1;

	BOOL	bCalFlag;			//��̬�����ķ��ر�־
	BOOL	bPreviousFlag=-120;	//ǰһ�εļ����־
	BOOL	pg;
	if (3.92 < dPressure)	//ѹ��
		pg = true;
	else
		pg = false;

	CalSupportSpan.SetMaterialCODE( EDIBgbl::sCur_CodeNO );	// ���������õĲ��Ϲ淶��

	for (int nCont=1; nCont <= 4; nCont++)		//�������û����,��ֱ�ӷ���,���������˲��ϵ�����,�����¼���һ��
	{
		//����֧���ܼ��
		if (0) 
		{
		}else
		{	//Ĭ�ϵ�����ҵ
			bCalFlag = CalSupportSpan.CalcSupportSpan(dw,s,dMediumDensity,temp,dInsuWei, 
							strMaterialName,pg,2,LmaxR,LmaxS,fQ,fQp); 
		}

		dLmax = min(LmaxR,LmaxS);		//����������ǿ����������С��һ��ֵ

		if (-1 == bCalFlag)
		{
			//ָ���Ĳ���û��Ӧ��ϵ��
			TableID = 0; 
			strDlgCaption.Format(_T("������ %s �� %f  ���µ�����Ӧ��(MPa):"),strMaterialName,temp);
			InputMaterialParameter(strDlgCaption,strMaterialName,TableID); 
		}else if (-2 == bCalFlag)
		{ 
			//ָ���Ĳ���û�е���ģ��
			TableID = 1;
			strDlgCaption.Format(_T("������ %s �� %f  ���µĵ���ģ��(KN/mm2)"),strMaterialName,temp);
			InputMaterialParameter(strDlgCaption,strMaterialName,TableID); 
		}else if (-3 == bCalFlag) 
		{
			//ָ���Ĳ���û�л��򺸷�Ӧ��ϵ��
			TableID = 4;
			strDlgCaption.Format(_T("������ %s �Ļ��򺸷�Ӧ��ϵ��:"),strMaterialName);
			//�������� 
			InputMaterialParameter(strDlgCaption,strMaterialName,TableID);
		}else
		{ 
			//����ɹ�
			break; 
		}
		//

		if(bPreviousFlag != bCalFlag)		//���û����Ӧ���ϵ����ԣ�һ����������������Ρ�
			bPreviousFlag = bCalFlag;
		else
			break;
	}
	return (dLmax<0)?0:dLmax;
}

//------------------------------------------------------------------
// DATE         :[2006/01/09]
// Parameter(s) :strVariableName[in] ���������������
//				:dRValue[out] ��Ӧ������ֵ
// Return       :
// Remark       : �����������ֵ
//------------------------------------------------------------------
void CHeatPreCal::GetWeatherProperty(CString strVariableName, double &dRValue)
{
	if ( strVariableName.IsEmpty() )
		return;
	try
	{
		if ( !OpenAssistantProjectTable( m_pRsWeather, "Ta_Variable" ) )
		{
			return;
		}
		if ( m_pRsWeather->adoEOF && m_pRsWeather->BOF )
		{
			return;
		}

		CString strSQL;
		strSQL = " Ta_Variable_Name='"+strVariableName+"' ";
		
		m_pRsWeather->MoveFirst();
		m_pRsWeather->Find( _bstr_t( strSQL ), NULL, adSearchForward );
		if ( !m_pRsWeather->adoEOF && !m_pRsWeather->BOF )
		{
			dRValue = vtof( m_pRsWeather->GetCollect( _variant_t( "Ta_Variable_VALUE" ) ) );
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox( e.Description() );
		return ;
	}
}
//------------------------------------------------------------------
// Parameter(s) : m_nID[in] ��¼��
// Return       : ��ر�������
// Remark       : ��ǰ���¼�¼���������
//------------------------------------------------------------------
//  
BOOL CHeatPreCal::GetSubterraneanTypeAndPipeCount(const long m_nID, short& nType, short& nPipeCount) const
{
	nType = 0;
	nPipeCount = 1;
	try
	{
		if (NULL == m_pRsSubterranean || (m_pRsSubterranean->GetState() == adStateClosed) || (m_pRsSubterranean->GetadoEOF() && m_pRsSubterranean->GetBOF()))
		{
			return FALSE;
		}
		CString strSQL;
		m_pRsSubterranean->MoveFirst();
		strSQL.Format("ID=%d", m_nID);
		m_pRsSubterranean->Find(_bstr_t(strSQL), 0, adSearchForward);
		if (!m_pRsSubterranean->GetadoEOF())
		{
			nType			= vtoi(m_pRsSubterranean->GetCollect(_variant_t(_T("c_lay_mark"))));
			nPipeCount      = vtoi(m_pRsSubterranean->GetCollect(_variant_t(_T("c_Pipe_Count"))));
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;
}
