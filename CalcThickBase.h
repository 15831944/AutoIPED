// CalcThickBase.h: interface for the CCalcThickBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCTHICKBASE_H__1BF4CE0E_9FFC_41C7_B7AF_6E72380B8356__INCLUDED_)
#define AFX_CALCTHICKBASE_H__1BF4CE0E_9FFC_41C7_B7AF_6E72380B8356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalcOriginal.h"
#include "CalcAlpha_Code.h"
#include "CalcAlpha_CodePC.h"
#include "CalcAlpha_CodeGB.h"
#include "CalcAlpha_CodeCJJ.h"
#include "vtot.h"

#define  INT_ADD	9			//���º��Բ��ʱ�ӵ�һ����
#define  DZero		(1e-6)		// �Ƚ�ʱ��Ϊ��
//////////////////////////////
//�����¶ȷ�.			CCalcThick_FaceTemp
//ɢ��(����)�ܶȷ�		CCalcThick_HeatDensity
//���ú�ȷ�			CCalcThick_Economy
//�������·�			CCalcThick_Antifreezing
class CCalcThickBase  : public CCalcOriginalData ,public CCalcAlpha_Code,public CCalcAlpha_CodePC,public CCalcAlpha_CodeCJJ
{
public:
	//
	virtual short CalcPipe_One(double &thick_resu, double &ts_resu);
	virtual short CalcPipe_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu);
	virtual short CalcPlain_One(double &thick_resu, double &ts_resu);
	virtual short CalcPlain_Com(double &thick1_resu, double &thick2_resu, double &tb_resu, double &ts_resu);

	//ָ�����
	virtual short CalcPipe_One_InputThick(double thick_resu, double &ts_resu);
	virtual short CalcPlain_One_InputThick(double thick_resu, double &ts_resu);
	virtual short CalcPipe_Com_InputThick(double thick1_resu, double thick2_resu, double &tb_resu, double &ts_resu);
	virtual short CalcPlain_Com_InputThick(double thick1_resu, double thick2_resu, double &tb_resu, double &ts_resu);

	
	virtual short CalcPlain_One_TsTemp(const double delta, double &dResTs);		//ƽ�ڵ��㣬������¶ȡ�
	virtual short CalcPipe_One_TsTemp(const double delta, double &dResTs);
	
	virtual short CalcPlain_Com_TsTb(const double delta1, const double delta2, double &dResTs, double &dResTb, int flg=0);
	virtual short CalcPipe_Com_TsTb(const double delta1, const double delta2, double &dResTs, double &dResTb, int flg=0);
	
	void CalcSubPipe_TsTemp(const double dPipeTemp, const double dDW, const double delta, double &dResTs, short nMark = 0);	// ��عܵı����¶�

	CCalcThickBase();
	virtual ~CCalcThickBase(); 

public:
	BOOL InitCalcAlphaData(double ts);
	double GetPipeAlpha(double ts, double D1);
	double GetPlainAlpha(double ts, double D1=0.0);
	short GetExceptionInfo(CString& strInfo);
//	short Calc_Q_PipeOneNoInit(double& Q);
//	short Calc_Q_PlainOneNoInit(double& Q);
	inline 	virtual short Calc_QL_PipeCom(double &QL);
	inline 	virtual short Calc_QL_PipeOne(double &QL);
	inline 	virtual short Calc_Q_PlainCom(double& Q);
	inline	virtual short Calc_Q_PlainOne(double& Q);
	inline	virtual short Calc_Q_PipeCom(double& Q);
	inline	virtual short Calc_Q_PipeOne(double& Q);
	
//	double	dQ;				//ȷ�����º���µ�ɢ���ܶ�

protected:
	inline	double GetLamda2(double tb, double ts);	// �Ᵽ�²��ϵĵ���ϵ��
	inline	double GetLamda1(double t, double tb);	// �ڱ��²��ϵĵ���ϵ��
	inline	double GetLamda(double t, double ts);	// ���㱣�²��ϵĵ���ϵ��

	inline  double GetLamdaA(double t1, double t2, short nMark=0);	// ��õ���ϵ����nMark=0���ڱ��²��ϻ򵥲�ģ� 1Ϊ�Ᵽ�²���

	bool GetHeatLoss(_RecordsetPtr& pRs,double& t,double& YearRun, double& SeasonRun, double &QMax);

	virtual short GetThicknessRegular();

/*	_ConnectionPtr m_pConAutoIPED;		//������Ŀ���ݿ�(AutoIPED.mdb)��ָ��.
	_ConnectionPtr m_pConIPEDCode;		//�淶���ݿ�(IPEDCode.mdb)
	_ConnectionPtr m_pConMaterial;		//�������ݿ�(Material.mdb)

	_RecordsetPtr m_pRsHeatLoss;		//���²��Ϸ���ϵ����.()
	CString m_strThickFormula;		//���㱣�º�Ĺ�ʽ.
	CString m_strThick1Formula;		//���㸴�ϱ���ʱ�ڲ�Ĺ�ʽ.
	CString m_strThick2Formula;		//���㸴�ϱ���ʱ���Ĺ�ʽ.

	CString m_strQFormula;			//�������ϵ���Ĺ�ʽ
	CString m_strTbFormula;			//��������¶ȵĹ�ʽ.
	CString m_strTsFormula;			//����������¶ȵĹ�ʽ

	double	m_dThick;
	*/

private:
	float GetMaxHeatLoss(_RecordsetPtr &pRs, double &t, CString strFieldName);

};
#endif // !defined(AFX_CALCTHICKBASE_H__1BF4CE0E_9FFC_41C7_B7AF_6E72380B8356__INCLUDED_)
