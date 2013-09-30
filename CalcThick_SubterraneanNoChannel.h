// CalcThick_SubterraneanNoChannel.h: interface for the CCalcThick_SubterraneanNoChannel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCTHICK_SUBTERRANEANNOCHANNEL_H__0C32A533_DF05_4CC6_88AB_F82A6DCD8CB8__INCLUDED_)
#define AFX_CALCTHICK_SUBTERRANEANNOCHANNEL_H__0C32A533_DF05_4CC6_88AB_F82A6DCD8CB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcThickSubterranean.h"

//////////////////////////////////////////////////////////////////////////
//
//�������ֱ�����(�޹ܹ�)���ܵ��ı��º��
//
//////////////////////////////////////////////////////////////////////////

class CCalcThick_SubterraneanNoChannel : public CCalcThickSubterranean 
{
public:
	short CalcPipe_Com(double &thick1_resu, double &thick2_resu, double &ts1_resu, double &ts2_resu);
	// �ܵ�֮��ĵ������� 
	virtual void Calc_Ro(double &dRo);
	// �޹ܹ��ĵ������²�����
	void Calc_Rpipe_One(double D1, double dLamda, double& dR1);	
	// �޹ܹ���˫���ܵ����²�����
	void Calc_Rpipe_Two(double dQ1, double dQ2, double& dR1, double& dR2);
	// �޹ܹ�������������
	void Calc_Rso(double &dRso);
	// �޹ܹ������ܵ�������ʧ
	short Calc_Q_PipeOne(double &dOneQ);
	// �޹ܹ������ܵ�������ʧ 
	void Calc_Q_PipeTwo(double &dReQ1, double &dReQ2); 
	// �޹ܹ��ܵ� ���¼��� 
	short CalcPipe_One(double &thick_resu, double &ts_resu); 

public:
	CCalcThick_SubterraneanNoChannel();
	virtual ~CCalcThick_SubterraneanNoChannel();
	
};

#endif // !defined(AFX_CALCTHICK_SUBTERRANEANNOCHANNEL_H__0C32A533_DF05_4CC6_88AB_F82A6DCD8CB8__INCLUDED_)
