// CalcAlphaOriginalData.h: interface for the CCalcAlphaOriginalData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCALPHAORIGINALDATA_H__6B052138_5D6B_4DE6_BDA5_ACE00D6518AD__INCLUDED_)
#define AFX_CALCALPHAORIGINALDATA_H__6B052138_5D6B_4DE6_BDA5_ACE00D6518AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------------------------------------------------------------
// DATE         :[2005/08/16]
// Parameter(s) :
// Return       :
// Remark       :�������ϵ��ʱ���õ�������.�ڵ���ʱ���и���ֵ
//------------------------------------------------------------------
class CCalcAlphaOriginalData  
{
public:
	CCalcAlphaOriginalData();
	virtual ~CCalcAlphaOriginalData();

	double	dA_ta;		//�����¶�
	double  dA_ts;		//������¶�
	double	dA_W;		//��ƽ������
	double	dA_hedu;	//�ڶ�
	double	dA_B;		//�ط��ٷ����ƽ�ڿ�ȣ�m
	double	dA_AlphaIndex;//ALPHA��ȡֵ����

};

#endif // !defined(AFX_CALCALPHAORIGINALDATA_H__6B052138_5D6B_4DE6_BDA5_ACE00D6518AD__INCLUDED_)
