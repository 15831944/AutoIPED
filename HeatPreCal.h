// HeatPreCal.h: interface for the CHeatPreCal class.
//
// ��ɱ��µļ���
//
// �ڵ���C_ANALYS���м���ǰ
//
// ��Ҫ�ȵ���		
//		SetProjectName;			���ù�����
//		SetConnect;				���ù������ݿ�����
//		SetAssistantDbConnect(IConnectionCODE);	���ø��������������ݿ�
//		SetMaterialPath(gsProjectDBDir);		���ò������ݿ��·��
//
//////////////////////////////////////////////////////////////////////



#if !defined(AFX_HEATPRECAL_H__DE887BF7_4B9B_40F3_92E5_FAD4E263A3DF__INCLUDED_)
#define AFX_HEATPRECAL_H__DE887BF7_4B9B_40F3_92E5_FAD4E263A3DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FoxBase.h"
#include "GetPropertyofMaterial2.h"	// Added by ClassView
#include "CalcThick_FaceTemp.h"
#include "CalcThick_Economy.h"
#include "CalcThick_HeatDensity.h"
#include "CalcThick_PreventCongeal.h"
#include "CalcSupportSpanDll.h"
#include "CalcThick_SubterraneanObstruct.h"
#include "CalcThick_SubterraneanNoChannel.h"

class CHeatPreCal:public CFoxBase,public CCalcThick_FaceTemp,public CCalcThick_Economy,public CCalcThick_HeatDensity,public CCalcThick_PreventCongeal,public CCalcThick_SubterraneanObstruct, public CCalcThick_SubterraneanNoChannel
{
public:
	CHeatPreCal();
	virtual ~CHeatPreCal();

//method
public:

	//���¼���
	void C_ANALYS();

// property
public:
	virtual	void MinimizeAllWindow(){}
	void GetWeatherProperty(CString strVariableName, double& dRValue);
	double GetSupportSpan(const double dw,const double s,const CString strMaterialName,const double dPressure,const double dMediumDensity,const double temp,const double dInsuWei);
	static double GetMediumDensity(const CString strMediumName, CString* pMediumName = NULL);
	static BOOL SetConstantDefine(CString strTblName, CString ConstName, _variant_t ConstValue);
	//�ڹ淶���ݿ��з��ʳ�����.
	static bool GetConstantDefine(CString strTblName, CString ConstName, CString& ConstValue);
	static bool GetConstantDefine(CString strTblName, CString ConstName, double& dConstValue);
	//����Material���ݿ����ڵ��ļ���
	LPCTSTR GetMaterialPath();

	//����Material���ݿ��·�� 
	void SetMaterialPath(LPCTSTR pMaterialPath); 
 
	//���ع�����(ID)
	CString& GetProjectName();

	//���ù�����(ID)
	void SetProjectName(CString &Name);

	//���ø������ݿ������
	void SetAssistantDbConnect(_ConnectionPtr &IConnection);

	//���ظ������ݿ������
	_ConnectionPtr& GetAssistantDbConnect();

	//���ؼ���ѡ��
	int GetChCal();

	//���ü���ѡ��
	void SetChCal(int Ch);

protected:
	//�򿪸������ݿ�ı�(�ڹ淶���ݿ���)
	BOOL OpenAssistantProjectTable(_RecordsetPtr &IRecordset, LPCTSTR pTableName);

	//��ĳ�����̵ı�ļ�¼
	BOOL OpenAProjectTable(_RecordsetPtr &IRecordset,LPCTSTR pTableName,LPCTSTR  pProjectName=NULL);


protected:

	int GetCalcFormula(int nFormulaIndex, CString& strFormula, CString& strTsFormula, CString& strQFormula);

	short CalcInsulThick(CString strThickFormula,CString strThickFormula2,CString strTbFormula, CString strTsFormula, CString strQFormula ,double &dThick1, double &dThick2);	//������㱣�º�ȵĹ�ʽ,��ʼ������,������.

	//������ڵ��豸��ܵ����½ṹ����洫��ϵ����
	bool GetAlfaValue(_RecordsetPtr pRs, CString sSearchFieldName,  CString sValFieldName, double dCompareVal,double& dAlfaVal);
	double CalcAlfaValue(double dTs, double dD1);
	//���ݽ����¶�,ȡ���½ṹ������������ɢ���ܶ�
	bool GetHeatLoss(_RecordsetPtr& pRs,double& t,double& YearRun, double& SeasonRun, double &QMax);

	bool InsertToReckonCost(double& ts,double&tb,double& thick1,double& thick2,double& cost_min, bool bIsFirst=false);

	virtual short GetThicknessRegular();
	bool E_Predat_Cubage();

	//����Ҫѡ����Ҫ���������ʱ����
	virtual void SelectToCal(_RecordsetPtr &IRecordset,int *pIsStop);

	//����Ҫָ������ġ���ʼ���롮��������Χʱ����
	virtual	BOOL RangeDlgshow(long &Start,long &End);

	//�ж��Ƿ�ֹͣ����ʱ���ô˺���
	virtual void Cancel(int *pState);

protected:
	// ��Material���ݿ��е�CL_OLD_NEW����Ѱ����MatName���������Ӧ�Ĳ��ϡ�
	BOOL FindMaterialNameOldOrNew(LPCTSTR MatName,CString* &pFindName,int &Num);

	void temp_plain_one(double &thick_t,double &ts);
	void plain_one(double &thick_resu,double &ts_resu);
	void pip_one(double &thick_resu,double &ts_resu);
	void plain_com(double &thick1_resu,double &thick2_resu,double &tb_resu,double &ts_resu);
	void pip_com(double &thick1_resu,double &thick2_resu,double &tb_resu,double &ts_resu);
	void SUPPORT(double &dw, double &s, double &temp, double &wei1,double &wei2,double &lmax, CString &v, CString &cl,BOOL &pg,CString &st, double &t0);
	void temp_pip_com(double &thick1_t,double &thick2_t,double &ts,double &tb);
	void temp_plain_com(double &thick1_t,double &thick2_t,double &ts,double &tb);
	void temp_pip_one(double &thick_t,double &ts);

protected:
	double Temp_env;		// �����¶�
	double heat_price;		// �����ȼ�
	double S;				// �������

	double out_a0,out_a1,out_a2;	// �Ᵽ�²��ϵĵ���ϵ��
	double in_a0,in_a1,in_a2;		// �ڱ��²��ϵĵ���ϵ��
	double out_price,in_price,pro_price;	// ���ϼ۸�
	double in_dens,out_dens,pro_dens;		// �����ܶ�
	double in_ratio;	// �ڱ��²��ϵĵ���ϵ�����ӵı���
	double out_ratio;	// �Ᵽ�²��ϵĵ���ϵ�����ӵı���
	
	double	Mhours;		// ������Сʱ��
	double  Yong;		// �ȼ۱�������(������ֵϵ��)
	double	Temp_pip;	//�����¶�
	double	D0;			//�⾶
	double	hedu;		//�ڶ�

	CString out_mat;		//�Ᵽ�²�������
	CString	in_mat;			//�ڱ��²�������
	CString	pro_mat;		//������������
	CString	pro_name;		//������������,��ȥ���������ŵ�����.��"��Ƥ(0.75)"���ֵΪ"��Ƥ"

	double	co_alf;
	double	distan;
	double	in_tmax;
	double	speed_wind;		//����.
	double	B;				//�ط��ٷ����ƽ�ڿ��
	double	m_dPipePressure;	//����ѹ��
	double	m_dMediumDensity;	//�����ܶ�
	//����ϵ��������.
	long	m_nIndexAlpha;

	double	m_HotRatio;		//���½����¶Ȳ�Ӧ������㱣�²������ʹ���¶ȵı���.
	double	m_CoolRatio;	//��������¶Ȳ�Ӧ������㱣��������ʹ���¶ȵı���.   
	double	MaxTs;			//�������������¶� 

	//GLOBAL ?

	long	start_num;
	long	stop_num;

	//���㷽ʽѡ��
	int ch_cal;
	//������Ϣ
	CString ch;
	//���й���.
	//0:�������й���.
	//1:�����й���.
	int hour_work;

	double	m_dMaxD0;	//ƽ����Բ�ܵķֽ��⾶.  ��ǰΪ2000
	long	m_nID;			//ԭʼ���ݵ�ǰ��¼�����

	//
	double	thick_3;
	
	int m_thick1Start;			//�ڲ㱣��������С���	
	int m_thick1Max;			//�ڲ㱣�����������
	int m_thick1Increment;		//�ڲ㱣�º����С����

	int m_thick2Start;			//��㱣��������С���
	int m_thick2Max;			//��㱣�����������
	int m_thick2Increment;		//��㱣�º����С����

	_RecordsetPtr m_pRsCon_Out;		//��Ӧ�豸����������ϵ����.
	_RecordsetPtr m_pRs_a_Hedu;		//�Ᵽ�²��ϵĺڶ�
	_RecordsetPtr IRecPipe;			//�ܵ�������
	_RecordsetPtr m_pRsHeat_alfa;	//�������ϵ����ʽȡֵ������.
	_RecordsetPtr m_pRs_CodeMat;	//��׼�ı��²��ϱ�.

	_RecordsetPtr m_pRsCalcThick;	//���㱣�º�ȵĹ�ʽ.

	_RecordsetPtr IRecCalc;			//����ԭʼ���ݱ�

	_RecordsetPtr m_pRsHeatLoss;	//���½ṹ������������ɢ���ܶ�	

	_RecordsetPtr m_pRsCongeal;		//����������������ӵ�һЩ����
	
	_RecordsetPtr m_pRsSubterranean;// ��عܵ����¼������
	
	_RecordsetPtr m_pRsFaceResistance;	// ���²����������Χ������ɢ����ļ�¼��

	_RecordsetPtr m_pRsWeather;		//�������

	// �����ĵ���ϵ��
	_RecordsetPtr m_pRsLSo;

protected:
	short GetPipeWeithAndWaterWeith(_RecordsetPtr &pRs, const double dPipe_Dw, const double dPipe_S, CString strField1, double &dValue1,CString strField2,  double &dValue2);
//	GetPropertyofMaterial CCalcDll;
	CCalcSupportSpanDll	  CalSupportSpan;

	CString m_ProjectName;	//������
	CString m_MaterialPath;
	_ConnectionPtr m_IMaterialCon;

	_ConnectionPtr m_AssistantConnection; //�������ݿ������
private:
	BOOL GetSubterraneanTypeAndPipeCount(const long m_nID, short& nType, short& nPipeCount) const;
	BOOL FindStandardMat(_RecordsetPtr& pRsCurMat,CString strMaterialName);
	void InitOriginalToMethod();

};

#endif // !defined(AFX_HEATPRECAL_H__DE887BF7_4B9B_40F3_92E5_FAD4E263A3DF__INCLUDED_)

BOOL InputMaterialParameter(CString strDlgCaption, CString strMaterialName, int TableID);
