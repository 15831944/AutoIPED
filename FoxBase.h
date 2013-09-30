//
// FoxBase.h: interface for the CFoxBase class.
//
// �ṩ��FOXPRO�е�һЩ�����ĺ���
//
// Write by ��ΰ��
// Time:	2004,7,29
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOXBASE_H__6C072190_9140_4AC7_8100_09DA2694738E__INCLUDED_)
#define AFX_FOXBASE_H__6C072190_9140_4AC7_8100_09DA2694738E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import"c:\program files\common files\system\ado\msado15.dll"	\
		no_namespace rename("EOF","adoEOF")
#include "ParseExpression.h"

typedef struct _Txtup{
	int dID;
	CString txt;
} Txtup;

class CFoxBase:public CParseExpression  
{
public:
	CFoxBase();
	virtual ~CFoxBase();

public:
	enum{
		EQUAL,				//����
		UNEQUAL,			//������
		GREATER,			//����
		GREATER_OR_EQUAL,	//���ڵ���
		LESS,				//С��
		LESS_OR_EQUAL		//С�ڵ���
	};

public:
	//�ڼ�¼���е�ǰ�α�λ������ָ�����ֶε�ֵ
	void PutTbValue(_RecordsetPtr IRecordset,_variant_t FieldsName,double Value);
	void PutTbValue(_RecordsetPtr IRecordset,_variant_t FieldsName,float Value);
	void PutTbValue(_RecordsetPtr IRecordset,_variant_t FieldsName,int Value);
	void PutTbValue(_RecordsetPtr IRecordset,_variant_t FieldsName,CString Value);

	//�Ӽ�¼���з���ָ���ֶε�ֵ
	void GetTbValue(_RecordsetPtr IRecordset,_variant_t FieldsName,double &RetValue);
	void GetTbValue(_RecordsetPtr IRecordset,_variant_t FieldsName,float &RetValue);
	void GetTbValue(_RecordsetPtr IRecordset,_variant_t FieldsName,int &RetValue);
	void GetTbValue(_RecordsetPtr IRecordset,_variant_t FieldsName,CString &RetValue);

	//�ڼ�¼���ĵ�ǰλ�ò���һ���¼�¼
	BOOL InsertNew(_RecordsetPtr &IRecord,int after);
	//���after==1������insert blank
	//���afterΪ����ֵ����insert blank befo



	//�滻��¼����ѡ���ֶ�ָ������¼��ֵ(�൱��FOXPRO�� Replace Next)
	BOOL ReplNext(_RecordsetPtr IRecordset, _variant_t FieldsName, _variant_t Value,int num);

	//�ж��ļ��Ƿ����
	BOOL FILE(LPCTSTR pFilePath);

	//�滻��¼����ѡ���ֶ����е�ֵ
	BOOL ReplAll(_RecordsetPtr IRecordset,_variant_t FieldsName,_variant_t Value);

	//���ص�ǰ��¼�����α�λ��
	long RecNo(_RecordsetPtr IRecordset);

	//
	//�ӱ�ĵ�һ����¼��ʼѰ��ָ�����߼���ϵ
	//
	BOOL LocateFor(_RecordsetPtr IRecordset,_variant_t FieldsName,int Relations,_variant_t Value);

	//
	//�ӱ�ĵ�ǰ��¼��ʼѰ��ָ�����߼���ϵ
	//
	BOOL LocateForCurrent(_RecordsetPtr IRecordset, _variant_t FieldsName, int Relations,_variant_t Value);


	//�������ݿ������
	_ConnectionPtr& GetConnect();

	//�������ݿ������
	void SetConnect(_ConnectionPtr &IConnection);

	BOOL CopyTo(LPCTSTR pSourceTable,LPCTSTR pDestinationTable);

	//�滻ָ����Χ��ĳ�ֶε�ֵ.
	bool ReplaceAreaValue(_RecordsetPtr pRs, CString strField,CString strValue,int startRow=0, int endRow=0);

protected:
	//�����쳣ʱ���ô˺���
	virtual void ExceptionInfo(LPCTSTR pErrorInfo);
	virtual void ExceptionInfo(_com_error &e);

	//����������Ϣʱ���ô˺���
	virtual void DisplayRes(LPCTSTR pStr);

	//����������ֵʱ���ô˺���
	virtual void InputD(LPCTSTR pstr,double &val);

	//���ȴ������ַ���ʱ���ô˺���
	virtual void Wait(LPCTSTR pInfoStr,CString &strRet);

	//����������Ϣʱ���ô˺���
	virtual void Say(LPCTSTR pStr);

	//�����˳�Ӧ�ó���ʱ���ô˺���
	virtual void Quit();

	//�����ԶԻ�����ʽ��ʾ��Ϣʱ���ô˺���
	virtual int MessageBox(LPCTSTR pStr);

	BOOL CopyData(_RecordsetPtr &IRecordS,_RecordsetPtr &IRecordD);
	CString CreateTableSQL(_RecordsetPtr &IRecord,LPCTSTR pTableName);
private:
	HANDLE m_hStdout;				//��׼������
	_ConnectionPtr m_Connection;	//���ݿ���������ָ��
};

#endif // !defined(AFX_FOXBASE_H__6C072190_9140_4AC7_8100_09DA2694738E__INCLUDED_)
