// CalcThick_PreventCongeal.h: interface for the CCalcThick_PreventCongeal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCTHICK_PREVENTCONGEAL_H__2CF7030F_BD5C_4ABC_B069_A99ECCF5B200__INCLUDED_)
#define AFX_CALCTHICK_PREVENTCONGEAL_H__2CF7030F_BD5C_4ABC_B069_A99ECCF5B200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcThickBase.h"

class CCalcThick_PreventCongeal : public CCalcThickBase   
{
public:
	CCalcThick_PreventCongeal();
	virtual ~CCalcThick_PreventCongeal();

	//���㱣�º��
	virtual short CalcPipe_One(double &thick_resu, double &ts_resu);
	virtual short CalcPipe_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu);
	virtual short CalcPlain_One(double &thick_resu, double &ts_resu);
	virtual short CalcPlain_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu);
 
	BOOL GetCongealOriginalData(long nID);

protected:
	short CalcThickFormula(double& thick_resu, double& ts_resu, BOOL bIsPipe=TRUE);
	
private:
	//��������ʱ�����ӵı���
	double	Kr;					//�ܵ�ͨ��֧���ܴ�����(����)��ʧ�ĸ���ϵ��
	double	taofr;				//������ܵ�����Һ��ͣ��ʱ��(Сʱ)
	double	tfr;				//�����ڹ��ڶ����¶�
	double	V;					//ÿ�׹ܳ����������m3/m;
	double	ro;					//�����ܶ�
	double	C;					//���ʱ���
	double	Vp;					//ÿ�׹ܳ��ܱ����,m3/m;
	double	rop;				//�ܲ��ܶ�
	double	Cp;					//�ܲı���
	double	Hfr;				//�����ڽ���
	double	dFlux;				//����

};

#endif // !defined(AFX_CALCTHICK_PREVENTCONGEAL_H__2CF7030F_BD5C_4ABC_B069_A99ECCF5B200__INCLUDED_)
