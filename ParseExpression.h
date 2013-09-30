//
// ParseExpression.h: interface for the CParseExpression class.
//
// ��FOXPRO�еļ򵥹�ϵ���ʽ���н���
// ��"S>=46.AND.S<65"
// 
// �����ַ�ʽ�Ա��ʽ���н������������ݿ����ӡ���ͨ��ʽ
// �������ݿ�Ա��ʽ���н��������ȵ���SetConnectionForParse
// �������ݿ������
//
// Write by ��ΰ��
//
// Time:2004,7,25
//	

#if !defined(AFX_PARSEEXPRESSION_H__821D2B2D_B8D8_4177_912E_79157020E63A__INCLUDED_)
#define AFX_PARSEEXPRESSION_H__821D2B2D_B8D8_4177_912E_79157020E63A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
class CParseExpression  
{
public:
	CParseExpression();
	virtual ~CParseExpression();

	//���ڶԱ��ʽ�еı������г�ʼ��
	struct tagExpressionVar
	{
		CString VarName;		//���ʽ�еı�����
		_variant_t VarValue;	//������ֵ
	};

public:
	//��FOXPRO�е��������ʽ���н���
	double ParseUseConn_Evaluation(LPCTSTR pExpression,tagExpressionVar *pVarStruct,int VarStructNum,int *pState);

	//��FOXPRO�е��߼����ʽ���н���
	BOOL ParseUseConn_Bool(LPCTSTR pExpression,tagExpressionVar *pVarStruct,int VarStructNum,int* pState);

	//������ʱ��ı���
	LPCTSTR GetTemporarilyTableName()const;

	//������ʱ��ı���
	void SetTemporarilyTableName(LPCTSTR TableName);

	//�������ڽ��������ݿ�����
	_ConnectionPtr& GetConnectionForParse();

	//�������ڽ��������ݿ�����
	void SetConnectionForParse(_ConnectionPtr IConnect);

	//��FOXPRO�еļ򵥹�ϵ���ʽ���н���
	BOOL Parse(LPCTSTR pExpression,int *pStatue);

	//����ָ����ֵ��ָ��
	BOOL GetMapValue(CString &Key,void*& pDouble);

	//����ָ����ֵ��ָ��
	void SetMapValue(CString &Key,double *pDouble);

private:
	//���ַ�����Ѱ�ҷ��ڸ�������"."��λ��
	int FindPoint(LPCTSTR szString, int nStart);

	//�Գ����ʽ�е�С��Ԫ���н���
	BOOL ElementParse(LPCTSTR pExpression,int *pStatue);

	//�������FOXPRO���ʽ���г�ʼ������(���������ݿ����ӷ�ʽ)
	CString InitExpressionUseConnect(LPCTSTR pExpression);

	//�������ڱ��ʽ��������ʱ��
	_Recordset* CreateTemporarilyTable(tagExpressionVar *pVarStruct,int VarStructNum);

	// ɾ����ʱ��
	void DropTemporarilyTable();

private:
//	CString m_ResultFieldName;
	CString m_TemporarilyTableName;			//�������ݿ��������ʱ����
	CMapStringToPtr m_DoubleMap;			//��ֵ��DOUBLE�ֱ���ָ����ձ�
	_ConnectionPtr m_ConnectionForParse;	//�������ݿ���������ݿ�����
};

#endif // !defined(AFX_PARSEEXPRESSION_H__821D2B2D_B8D8_4177_912E_79157020E63A__INCLUDED_)
