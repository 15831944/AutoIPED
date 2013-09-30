// CalcThickSubterranean.h: interface for the CCalcThickSubterranean class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCTHICKSUBTERRANEAN_H__7E20FEB9_1A1A_4D8D_B10A_345D92112F79__INCLUDED_)
#define AFX_CALCTHICKSUBTERRANEAN_H__7E20FEB9_1A1A_4D8D_B10A_345D92112F79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "afx.h"
#include "CalcThickBase.h"
//#include <afxtempl.h>

class CCalcThickSubterranean : public CCalcThickBase
{
public:
	BOOL Calc_Taw_One(double& dTaw);
	BOOL Calc_Taw_Two(double& dTaw);
	double GetSubterraneanRs(const double& D0,const double& dTemp, CString strPlace="����");
	virtual BOOL GetSubterraneanOriginalData(long nID);
	void Calc_Ro(double& dRo);
	double GetLamdaSo(double ts);	// �����ĵ���ϵ��
	virtual void Calc_Rso(double& dRso);
	virtual void Calc_Q_noTwo(double &dQ1, double & dQ2);
	inline	virtual void Calc_Q_noOne(double& dQ);
	inline	virtual void Calc_R_Two(double& dR);
	inline	virtual void Calc_R_One(double& dR);
	inline	virtual void Calc_Q_PipeTwo(double& dQ1, double & dQ2);
	inline	virtual short Calc_Q_PipeOne(double& dQ);

	CCalcThickSubterranean();
	virtual ~CCalcThickSubterranean();
	
protected:		// ��عܵ����������ӵı���
	int		subLayMark;		// ��صķ���״̬
	BOOL	bTawCalc;		// 

//	double  subEdaphicTemp;	// �������¶�
	double  Tso;			// �����¶�	

//	double  subEdaphicLamda;// �����ĵ���ϵ��
	double  LamdaSo;		// �����ĵ���ϵ��
	
//	double	subPipeDeep;	// �ܵ����������
	double  sub_h;			// �ܵ��������( �����ĵ�������� ) m 
//	double  subPipeSpan;	// ���ܵ�֮��ľ���
	double  sub_b;			// �����ܵ������ľ��� m 

//	double  Taw;
	double  R11;
	double  R12;
	double  Rs1;
	double  Rs2;
	double  Tf1;	// ��һ���ܵ��Ľ����¶�
	double  Tf2;	// �ڶ����ܵ��Ľ����¶�
	double	Ro;		// �����ܵ��໥��Ӱ��ĵ�������
	double  Rso;	// ���������� 
	double  R1;		// ��һ���ܵ�����
	double  R2;		// �ڶ����ܵ�����
	double  Raw;	// �ܹ��ڿ����¹ܹ��ڱڵ�����
	double  AlphaSo;// �����ķ���ϵ��
	double  Dag;	// �ܹ��ĵ���ֱ��
	double  dQA;	// ֱ�񵥸�����������ʧ
	double	Aaw;	// �ܹ��ڿ������ܹ��ڵĸ���ϵ��
	double  A1pre;	// Aaw = A1pre = 9
	double  K;

};

#endif // !defined(AFX_CALCTHICKSUBTERRANEAN_H__7E20FEB9_1A1A_4D8D_B10A_345D92112F79__INCLUDED_)
