// CalcOriginal.h: interface for the CCalcOriginalData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCORIGINAL_H__0EDA007A_D6E1_4AF5_A248_0CE0FA182794__INCLUDED_)
#define AFX_CALCORIGINAL_H__0EDA007A_D6E1_4AF5_A248_0CE0FA182794__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GetPropertyofMaterial2.h"	// Added by ClassView


//�������ϵ��ʱ����������������¶ȵĲ�ֵ.
#define  TsDiff		0.1
#define  MaxCycCount 2000
//
#define	 pi			3.1415927
//�����������¶�Ϊ0ʱ(����û���¶�����), ����һ������.
#define	 D_MAXTEMP 	50000.0	

class CCalcOriginalData
{
public:
	CCalcOriginalData();
	virtual ~CCalcOriginalData();

	_RecordsetPtr m_pRsCon_Out;		//��Ӧ�豸����������ϵ����.
	_RecordsetPtr m_pRs_a_Hedu;		//�Ᵽ�²��ϵĺڶ�
	_RecordsetPtr IRecPipe;			//�ܵ�������
	_RecordsetPtr m_pRsHeat_alfa;	//�������ϵ����ʽȡֵ������.
	//  [2005/06/02] 
	_ConnectionPtr m_pConAutoIPED;	//������Ŀ���ݿ�(AutoIPED.mdb)��ָ��.

	_RecordsetPtr m_pRsCalcThick;	//���㱣�º�ȵĹ�ʽ.

	_RecordsetPtr IRecCalc;			//����ԭʼ���ݱ�

	_RecordsetPtr m_pRsHeatLoss;	// �������ɢ���ܶ�

	_RecordsetPtr m_pRsCongeal;		//����������������ӵ�һЩ����
	
	_RecordsetPtr m_pRsSubterranean;// ��عܵ����¼���ʱ�Ĳ�����¼��
	
	_RecordsetPtr m_pRsFaceResistance;// �ܵ���ƽ�ڱ��½ṹ�����������Χ�����ķ������¼��
	
	// �����ĵ���ϵ����
	_RecordsetPtr m_pRsLSo;

	int m_thick1Start;		//�ڲ㱣��������С���
	int m_thick1Max;		//�ڲ㱣�����������
	int m_thick1Increment;	//�ڲ㱣�º����С����

	int m_thick2Start;		//��㱣��������С���
	int m_thick2Max;		//��㱣�����������
	int m_thick2Increment;	//��㱣�º����С����

	double	dQ;				//ȷ���˱��º��ʱ���ڴ˺���µ�ɢ���ܶ�ֵ

protected:
	//{{//		���㱣�º��ʱ���õ��ı���		[2005/04/26]

	long		m_nID;			//ԭʼ���ݵ�ǰ��¼�����
	int			nMethodIndex;	//���㷽�������� 

	double		out_price;		//�Ᵽ�²��ϵļ۸�.
	double		in_price;		//�ڱ��²��ϵļ۸�.
	double		pro_price;		//�������ϵļ۸�.

	double		out_dens;		//�Ᵽ�²��ϵ�����.
	double		in_dens;		//�ڱ��²��ϵ�����
	double		pro_dens;		//�������ϵ�����

	double		co_alf; 		//�����⾶�ڱ��л�õĴ���ϵ��
	double		Mhours;			//������Сʱ��
	double		Yong;			//�ȼ۱�������
	double		heat_price;		//�����ȼ�
//	double		Temp_pip;		//�����¶�
//	double		Temp_env;		//�����¶�

	double		nYearVal;		//�����й���ɢ���ܶ�
	double		nSeasonVal;		//�������й���ɢ���ܶ�.
	double		QMax;			//���ݵ�ǰ�Ĺ�����ȷ����ɢ���ܶȡ�
	double		in_tmax;		//���ϱ���ʱ���Ᵽ�²�������Ľ����¶ȡ�

	int			hour_work;		//���й���.0:�������й���.1:�����й���.

	double		D0;	 			//�豸��ܵ��⾶
	double		D1;				//���²��⾶�����ϱ����ڲ��⾶
	double		D2;				//���ϱ�������⾶
	double		S;				//���½ṹͶ���������

	double		thick;			//���㱣�²���
	double		thick1;			//���ϱ����ڲ���
	double		thick2;			//���ϱ��������
	double		thick3;			//��������
			
	double		lamda;			//���㱣�²���ϵĵ���ϵ��
	double		lamda1;			//�ڱ��²���ϵĵ���ϵ��.
	double		lamda2;			//�Ᵽ�²���ϵĵ���ϵ��.
	double		ALPHA;			//�����еĴ���ϵ��

	double		t;				//�豸��ܵ�������¶ȣ������¶ȣ�
	double		ta;				//�����¶�
	double		ts;				//���½ṹ������¶�
	double		tb;				//���ϱ���������洦�¶�

	double		in_a0;			//�ڱ��²��ϵ���ϵ������
	double		in_a1;			//�ڱ��²��ϵ���ϵ��һ����ϵ��
	double		in_a2;			//�ڱ��²��ϵ���ϵ��������ϵ��
	double		in_ratio;		//�ڱ��²��ϵĵ���ϵ�����ӵı���

	double		out_a0;			//�Ᵽ�²��ϵ���ϵ������
	double		out_a1;			//�Ᵽ�²��ϵ���ϵ��һ����ϵ��
	double		out_a2;			//�Ᵽ�²��ϵ���ϵ��������ϵ��
	double		out_ratio;		//�Ᵽ�²��ϵĵ���ϵ�����ӵı���

	double		m_HotRatio;		//���½����¶Ȳ�Ӧ������㱣�²������ʹ���¶ȵı���.
	double		m_CoolRatio;	//��������¶Ȳ�Ӧ������㱣��������ʹ���¶ȵı���.
	
	double		m_MaxHeatDensityRatio; //���½ṹ���������ɢ���ܶȣ��������������ɢ���ܶȵı�ֵ.
	double		m_dMaxD0;		//ƽ����Բ�ܵķֽ��⾶.  ��ǰΪ2000
	double		MaxTs;			//�������������¶�ֵ
	double		speed_wind;		//����.
	double		pi_thi;			//�ܵ��ں�
	double		distan;			//�ܵ�֧���ܼ��
	
	int			nAlphaIndex;	//����ϵ����ȡֵ����.
	int			nTaIndex;		//�����¶ȵ�ȡֵ����

	CString		out_mat;		//�Ᵽ�²�������
	CString		in_mat;			//�ڱ��²�������
	CString		pro_mat;		//������������
	CString		pro_name;		//������������,��ȥ���������ŵ�����.��"��Ƥ(0.75)"���ֵΪ"��Ƥ"
	CString		strPlace;		//�ܵ����豸�İ�װ�ص�

	CString		strExceptionInfo;//����ʱ�����ڴ�����Ϣ��
 
	//}}
/*
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

*/
	//�������ϵ���ı���
	double	hedu;				//�ڶ�
	double	B;					//�ط��ٷ����ƽ�ڿ�ȣ�m
//	double	W;					//��ƽ������
	
protected:
//	GetPropertyofMaterial CCalcDll;
//	CString m_ProjectName;	//������
//	CString m_MaterialPath;
//	_ConnectionPtr m_IMaterialCon;

//	_ConnectionPtr m_AssistantConnection; //�������ݿ������

};
#endif // !defined(AFX_CALCORIGINAL_H__0EDA007A_D6E1_4AF5_A248_0CE0FA182794__INCLUDED_)





