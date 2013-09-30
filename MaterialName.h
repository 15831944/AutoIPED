//
// MaterialName.h: interface for the CMaterialName class.
//
// �������ƹ�����
//
// ʵ�ֲ����£������Ƶ��滻
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIALNAME_H__3A9723FC_4F29_4C56_8144_D5C7B2018BF5__INCLUDED_)
#define AFX_MATERIALNAME_H__3A9723FC_4F29_4C56_8144_D5C7B2018BF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FoxBase.h"

class CMaterialName:public CFoxBase 
{
public:

	CMaterialName();
	virtual ~CMaterialName();

public:
	struct tagReplaceStruct			// �滻��Ϣ
	{
		CString strTableName;		// ��Ҫ�滻�ı���
		CString pstrFieldsName;		// ��Ҫ�滻���ֶ���������ֶ���"|"����
	};

public:
	// �����£��ɲ��϶��ձ�ļ�¼������
	void SetNameRefRecordset(_RecordsetPtr& IRecordset);
	void SetNameRefRecordset(_ConnectionPtr IConnection,LPCTSTR szTableName);	//throw(_com_error)
	// �����£��ɲ��϶��ձ�ļ�¼������
	_RecordsetPtr& GetNameRefRecordset();


	// �ò��ϵ��������滻���ϵľ����� throw(_com_error);
	void ReplaceNameOldToNew(_ConnectionPtr &IConnection,tagReplaceStruct &ReplaceStruct);
	void ReplaceNameOldToNew(_RecordsetPtr &IRecordset, tagReplaceStruct &ReplaceStruct);


private:
	// �Ӱ����ֶε��ַ����л���ֶ���
	CString GetFieldsNameFromString(LPCTSTR szFields,int Pos);

private:
	_RecordsetPtr m_IRecordsetNameRef;	//�£��ɲ��϶��ձ�ļ�¼������
};

#endif // !defined(AFX_MATERIALNAME_H__3A9723FC_4F29_4C56_8144_D5C7B2018BF5__INCLUDED_)
