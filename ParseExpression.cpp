// ParseExpression.cpp: implementation of the CParseExpression class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ParseExpression.h"

#include <TCHAR.H>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParseExpression::CParseExpression()
{
	m_TemporarilyTableName=_T("TemporarilyTable1");
}

CParseExpression::~CParseExpression()
{
	CString SQL;

	if(GetConnectionForParse()==NULL || GetConnectionForParse()->GetState()==adStateClosed)
	{
		return;
	}

	SQL.Format(_T("DROP TABLE %s"),GetTemporarilyTableName());

	try
	{
		GetConnectionForParse()->Execute(_bstr_t(SQL),NULL, adCmdText);
	}
	catch(_com_error /*&e*/)
	{
	}

}

////////////////////////////////////////////////////////////
//
//����ָ����ֵ��ָ��
//
//Key[in]		�����ҵ�pDouble����ļ�ֵ�ַ���
//pDouble[in]	Double�ͱ�����ָ��
//
void CParseExpression::SetMapValue(CString &Key, double *pDouble)
{
	m_DoubleMap.SetAt(Key,(void*)pDouble);
}

////////////////////////////////////////////////////////////////////
//
//����ָ����ֵ��ָ��
//
//Key[in]			�����ҵ�pDouble��ָ��ļ�ֵ�ַ���
//pDouble[out]		���ڷ��ص�Double�ͱ�����ָ��
//
//�����Ԫ�ҵ����ط�0�����򷵻�0
//
BOOL CParseExpression::GetMapValue(CString &Key,void*& pDouble)
{
	return m_DoubleMap.Lookup(Key,pDouble);
}

/////////////////////////////////////////////////////////////
//
// ���ַ�����Ѱ�ҷ��ڸ�������"."��λ��
//
// szString[in]	������ַ���
// nStart[in]	��ʼѰ�ҵ�λ��,λ�ô�0��ʼ,��С��0ʱҲ��0��ʼ
//
// ��������ҵ�������0Ϊ�����λ��,���û�ҵ�����-1
//
int CParseExpression::FindPoint(LPCTSTR szString, int nStart)
{
	CString strTemp;
	int Pos;

	if(szString==NULL)
		return -1;

	if(nStart<0)
		Pos=0;
	else
		Pos=nStart;

	strTemp=szString;

	Pos=strTemp.Find(_T('.'),Pos);

	while(TRUE)
	{
		if(Pos>=0)
		{
			if(strTemp[Pos+1]>=_T('0') && strTemp[Pos+1]<=_T('9'))
			{
				Pos=strTemp.Find(_T('.'),Pos+1);
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	return Pos;
}


//////////////////////////////////////////////////////////////////////
//
// ��FOXPRO�еļ򵥹�ϵ���ʽ���н���
//
// pExpression[in]	FOXPROC���ʽ�ַ���
// pStatue[out]		���ؽ����Ƿ�ɹ���־
//
// ����������ʽ�ɹ������ر��ʽ�Ľ��TRUE��FALSE
//
// ע�⣺pStatue��ΪNULL����pStatue��ΪNULLʱ��
//       �����ʽ�����ɹ�ʱ���ط�0�������ʽ����
//		 ʧ��ʱ����0����ʱ��������ֵ������
//
BOOL CParseExpression::Parse(LPCTSTR pExpression,int *pStatue)
{
	CString ExpressionStr;
	CString LogRelation;
	CString TempStr;
	BOOL bRet,bRight;
	int PrevPos=0;
	int Pos=0;

	if(pExpression==NULL)
	{
		if(pStatue!=NULL)
			*pStatue=0;

		return FALSE;
	}

	ExpressionStr=pExpression;

	Pos=FindPoint(ExpressionStr,Pos);

	if(Pos==-1)
	{
		return ElementParse(ExpressionStr,pStatue);
	}
	else
	{	
		TempStr=ExpressionStr.Mid(PrevPos,Pos-PrevPos);
		bRet=ElementParse(TempStr,pStatue);
	}

	PrevPos=Pos;

	while(Pos!=-1)
	{
		Pos=FindPoint(ExpressionStr,Pos+1);
		if(Pos==-1)
		{
			if(pStatue!=NULL)
				*pStatue=0;

			return FALSE;
		}

		LogRelation=ExpressionStr.Mid(PrevPos+1,Pos-PrevPos-1);
		PrevPos=Pos;

		Pos=FindPoint(ExpressionStr,Pos+1);
		if(Pos==-1)
		{
			TempStr=ExpressionStr.Mid(PrevPos+1);
		}
		else
		{
			TempStr=ExpressionStr.Mid(PrevPos+1,Pos-PrevPos-1);
		}
		PrevPos=Pos;

		bRight=ElementParse(TempStr,pStatue);

		LogRelation.MakeUpper();
		LogRelation.TrimLeft();
		LogRelation.TrimRight();

		if(LogRelation==_T("AND"))
		{
			bRet=bRet&&bRight;
		}
		else if(LogRelation==_T("OR"))
		{
			bRet=bRet||bRight;
		}
		else
		{
			if(pStatue!=NULL)
				*pStatue=0;

			return FALSE;
		}
	}


	if(pStatue!=NULL)
		*pStatue=1;

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
// �Գ����ʽ�е�С��Ԫ���н���
//
// pExpression[in]	FOXPROC���ʽ�ַ���
// pStatue[out]		���ؽ����Ƿ�ɹ���־
//
// ����������ʽ�ɹ������ر��ʽ�Ľ��TRUE��FALSE
//
// ע�⣺pStatue��ΪNULL����pStatue��ΪNULLʱ��
//       �����ʽ�����ɹ�ʱ���ط�0�������ʽ����
//		 ʧ��ʱ����0����ʱ��������ֵ������
//
BOOL CParseExpression::ElementParse(LPCTSTR pExpression, int *pStatue)
{
	CString ExpresionStr;
	CString TempStr;
	double *MapData;
	double DataLeft,DataRight;
	int Pos;
	BOOL bRet;
	
	if(pExpression==NULL)
	{
		if(pStatue!=NULL)
		{
			*pStatue=0;
		}
		return FALSE;
	}

	ExpresionStr=pExpression;
	ExpresionStr.TrimLeft();
	ExpresionStr.TrimRight();

	//�жα��ʽ���Ƿ���>=
	if((Pos=ExpresionStr.Find(_T(">=")))!=-1)
	{
		TempStr=ExpresionStr.Left(Pos);
		TempStr.TrimRight();

		//
		// ��>=���ȡ�ַ��������ӱ���Ѱ����Ӧ�ı���
		// ����ҵ���ȡ���ж�Ӧ��ֵ��Ϊ��>=����߱���
		// ���������ַ��������ַ���������ת��
		//
		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataLeft=*MapData;
		}
		else
		{
			DataLeft=atof(TempStr);
		}

		TempStr=ExpresionStr.Mid(Pos+2);
		TempStr.TrimLeft();

		//
		// ��>=�ұ�ȡ�ַ��������ӱ���Ѱ����Ӧ�ı���
		// ����ҵ���ȡ���ж�Ӧ��ֵ��Ϊ��>=���ұ߱���
		// ������ұ��ַ��������ַ���������ת��
		//
		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataRight=*MapData;
		}
		else
		{
			DataRight=atof(TempStr);
		}

		bRet=(DataLeft>=DataRight)?TRUE:FALSE;
	}
	else if((Pos=ExpresionStr.Find(_T(">")))!=-1)
	{
		TempStr=ExpresionStr.Left(Pos);
		TempStr.TrimRight();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataLeft=*MapData;
		}
		else
		{
			DataLeft=atof(TempStr);
		}

		TempStr=ExpresionStr.Mid(Pos+1);
		TempStr.TrimLeft();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataRight=*MapData;
		}
		else
		{
			DataRight=atof(TempStr);
		}

		bRet=(DataLeft>DataRight)?TRUE:FALSE;
	}
	else if((Pos=ExpresionStr.Find(_T("<=")))!=-1)
	{
		TempStr=ExpresionStr.Left(Pos);
		TempStr.TrimRight();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataLeft=*MapData;
		}
		else
		{
			DataLeft=atof(TempStr);
		}

		TempStr=ExpresionStr.Mid(Pos+2);
		TempStr.TrimLeft();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataRight=*MapData;
		}
		else
		{
			DataRight=atof(TempStr);
		}

		bRet=(DataLeft<=DataRight)?TRUE:FALSE;
	}
	else if((Pos=ExpresionStr.Find(_T("<")))!=-1)
	{
		TempStr=ExpresionStr.Left(Pos);
		TempStr.TrimRight();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataLeft=*MapData;
		}
		else
		{
			DataLeft=atof(TempStr);
		}

		TempStr=ExpresionStr.Mid(Pos+1);
		TempStr.TrimLeft();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataRight=*MapData;
		}
		else
		{
			DataRight=atof(TempStr);
		}

		bRet=(DataLeft<DataRight)?TRUE:FALSE;
	}
	else if((Pos=ExpresionStr.Find(_T("<>")))!=-1)
	{
		TempStr=ExpresionStr.Left(Pos);
		TempStr.TrimRight();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataLeft=*MapData;
		}
		else
		{
			DataLeft=atof(TempStr);
		}

		TempStr=ExpresionStr.Mid(Pos+2);
		TempStr.TrimLeft();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataRight=*MapData;
		}
		else
		{
			DataRight=atof(TempStr);
		}

		bRet=(DataLeft!=DataRight)?TRUE:FALSE;
	}
	else if((Pos=ExpresionStr.Find(_T("==")))!=-1)
	{
		TempStr=ExpresionStr.Left(Pos);
		TempStr.TrimRight();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataLeft=*MapData;
		}
		else
		{
			DataLeft=atof(TempStr);
		}

		TempStr=ExpresionStr.Mid(Pos+2);
		TempStr.TrimLeft();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataRight=*MapData;
		}
		else
		{
			DataRight=atof(TempStr);
		}

		bRet=(DataLeft==DataRight)?TRUE:FALSE;
	}
	else if((Pos=ExpresionStr.Find(_T("=")))!=-1)
	{
		TempStr=ExpresionStr.Left(Pos);
		TempStr.TrimRight();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataLeft=*MapData;
		}
		else
		{
			DataLeft=atof(TempStr);
		}

		TempStr=ExpresionStr.Mid(Pos+1);
		TempStr.TrimLeft();

		if(GetMapValue(TempStr,(void*&)MapData))
		{
			DataRight=*MapData;
		}
		else
		{
			DataRight=atof(TempStr);
		}

		bRet=(DataLeft==DataRight)?TRUE:FALSE;
	}
	else
	{
		if(pStatue!=NULL)
		{
			*pStatue=0;
		}
		return FALSE;
	}

	if(pStatue!=NULL)
	{
		if(bRet==TRUE)
		{
			*pStatue=1;
		}
		else
		{
			*pStatue=0;
		}
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////
//
// �������ڽ��������ݿ�����
//
// IConnect[in]	�Դ򿪵����ݿ������
//
void CParseExpression::SetConnectionForParse(_ConnectionPtr IConnect)
{
	m_ConnectionForParse=IConnect;
}

///////////////////////////////////////////////////////////////
//
// �������ڽ��������ݿ�����
//
_ConnectionPtr& CParseExpression::GetConnectionForParse()
{
	return m_ConnectionForParse;
}


///////////////////////////////////////////////////////////////////////////////////////
//
// ��FOXPRO�е��߼����ʽ���н���
//
// pExpression[in]	����FOXPRO�߼����ʽ
// pVarStruct[in]	tagExpressionVar�ṹ���ڳ�ʼ��pExpression�ı���
// VarStructNum[in]	pVarStruct�ṹ����Ŀ
// pState[out]		���ؽ����Ƿ�ɹ���־
//
// ����������ʽ�ɹ������ر��ʽ�Ľ��TRUE��FALSE
//
// ע�⣺pStatue��ΪNULL����pStatue��ΪNULLʱ��
//       �����ʽ�����ɹ�ʱ���ط�0�������ʽ����
//		 ʧ��ʱ����0����ʱ��������ֵ������
//
BOOL CParseExpression::ParseUseConn_Bool(LPCTSTR pExpression, tagExpressionVar *pVarStruct, 
										 int VarStructNum, int *pState)
{
	CString WhereStr;
	CString SQL;
	_RecordsetPtr IRecordTable;

	if(pExpression==NULL || pVarStruct==NULL || VarStructNum<=0)
	{
		if(pState)
			*pState=0;
		return FALSE;
	}

	WhereStr=InitExpressionUseConnect(pExpression);

	if(WhereStr.IsEmpty())
	{
		if(pState)
			*pState=0;
		return FALSE;
	}

	try
	{
		IRecordTable.Attach(CreateTemporarilyTable(pVarStruct,VarStructNum));

		if(IRecordTable==NULL)
		{
			if(pState)
				*pState=0;
			return FALSE;
		}

		IRecordTable->Close();

	}
	catch(_com_error)
	{
		throw;
	}

	SQL.Format("SELECT * FROM %s WHERE %s",GetTemporarilyTableName(),WhereStr);
	
	try
	{
		IRecordTable->Open(_variant_t(SQL),_variant_t((IDispatch*)GetConnectionForParse()),
						   adOpenDynamic,adLockOptimistic,adCmdText);

		if(pState)
			*pState=1;
		if(IRecordTable->adoEOF && IRecordTable->BOF)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	catch(_com_error)
	{
		throw;
	}

	DropTemporarilyTable();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////
//
// �������FOXPRO���ʽ���г�ʼ������(���������ݿ����ӷ�ʽ)
//
// pExpression[in]	����FOXPRO���ʽ
//
// ��������ɹ�����SQL���ʽ��ʧ�ܷ��ؿմ�
//
CString CParseExpression::InitExpressionUseConnect(LPCTSTR pExpression)
{
	struct tagREPLCAEMAP
	{
		LPTSTR Source;
		LPTSTR ReplaceStr;
	}RelpcaeMap[]=
	{
		{_T("RIGH"),	_T(" RIGHT")},
		{_T(".AND."),	_T(" AND ")},
		{_T(".OR."),	_T(" OR ")},
		{_T("$"),		_T(" LIKE ")},
		//{_T("$p"),		_T(" =TRIM(C_Place) ")},
		{_T("��"),		_T("=")},
		{_T("��"),		_T("<")},
		{_T("��"),		_T(">")},
		{_T("��"),		_T("<=")},
		{_T("��"),		_T(">=")},
		{_T("�Q"),		_T("<=")},
		{_T("�R"),		_T(">=")},
	};

	CString Expression=_T("");
	CString TempRight,TempLeft,Temp;
	int Pos;

	if(pExpression==NULL)
		return CString("");

	Expression=pExpression;
	Expression.TrimLeft();
	Expression.TrimRight();

	if(Expression.IsEmpty())
		return CString("");

	Expression.MakeUpper();

	Pos=Expression.Find(_T("$"),0);

	while(Pos!=-1)
	{
		int CurPos=Pos+1;
		while(Expression[CurPos]==_T(' ') || Expression[CurPos]==_T('\t'))CurPos++;

		while(CurPos<Expression.GetLength() && Expression[CurPos]!=_T('\''))
		{
			if(Expression[CurPos]>=_T('A') && Expression[CurPos]<=_T('Z'))
				break;

			CurPos++;
		}

		if(CurPos>=Expression.GetLength())
			return CString("");

		if(Expression[CurPos]==_T('\''))
		{
			TempRight=_T("'%");
			CurPos++;

			while(CurPos<Expression.GetLength() && Expression[CurPos]!=_T('\''))
			{
				TempRight+=Expression[CurPos];
				CurPos++;
			}

			if(CurPos>=Expression.GetLength())
			{
				return CString("");
			}

			TempRight+=_T("%'");

			TempLeft=Expression.Left(Pos+1);
			TempLeft+=TempRight;
			TempLeft+=Expression.Mid(CurPos+1);
			Expression=TempLeft;
		}

		if(Expression[CurPos]>=_T('A') && Expression[CurPos]<=_T('Z'))
		{
			int ReplaceRightStart,ReplaceRightEnd;
			int ReplaceLeftStart,ReplaceLeftEnd;

			ReplaceRightStart=CurPos;
			TempRight=_T("");
			while(CurPos<Expression.GetLength() && Expression[CurPos]>=_T('A') && Expression[CurPos]<=_T('Z') )
			{
				TempRight+=Expression[CurPos];
				CurPos++;
			}

			ReplaceRightEnd=CurPos-1;

			CurPos=Pos;
			ReplaceLeftEnd=ReplaceLeftStart=-1;
			while(CurPos>=0)
			{
				if(ReplaceLeftEnd<0)
				{
					if(Expression[CurPos]==_T('\''))
					{
						ReplaceLeftEnd=CurPos;
						CurPos--;
						continue;
					}
				}

				if(ReplaceLeftStart<0 && ReplaceLeftEnd>=0)
				{
					if(Expression[CurPos]==_T('\''))
					{
						ReplaceLeftStart=CurPos;
						break;
					}
				}
				CurPos--;
			}//end while(CurPos>=0)

			if(CurPos<0)
				return CString("");

			TempLeft=_T("'%");
			CurPos=ReplaceLeftStart+1;
			while(CurPos<ReplaceLeftEnd)
			{
				TempLeft+=Expression[CurPos];
				CurPos++;
			}

			TempLeft+=_T("%'");
			
			Temp=Expression.Left(ReplaceLeftStart);
			Temp+=TempRight;
			Temp+=Expression.Mid(ReplaceLeftEnd+1,ReplaceRightStart-ReplaceLeftEnd-1);
			Temp+=TempLeft;
			Temp+=Expression.Mid(ReplaceRightStart+1);

			Expression=Temp;


		}//end if(Expression[CurPos]>=_T('A') && Expression[CurPos]<=_T('Z'))

		Pos++;
		Pos=Expression.Find(_T("$"),Pos);

	}

	for(int i=0;i<sizeof(RelpcaeMap)/sizeof(RelpcaeMap[0]);i++)
	{
		Expression.Replace(RelpcaeMap[i].Source,RelpcaeMap[i].ReplaceStr);
	}

	return Expression;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �������ڱ��ʽ��������ʱ��
//
// pVarStruct[in]	pVarStruct�ṹ�����ڶԱ��ʽ�ڵı������г�ʼ����
// VarStructNum[in]	�����pVarStruct�ṹ����Ŀ
//
// ��������ɹ������Դ򿪵���ʱ��ļ�¼���Ľӿڵ�ָ��
// ���򷵻�NULL
//
_Recordset* CParseExpression::CreateTemporarilyTable(tagExpressionVar *pVarStruct, int VarStructNum)
{
	CString CreateTableStr;
	CString TableName,SQL;
	_RecordsetPtr IRecordTable;
	int i;

	if(pVarStruct==NULL || VarStructNum<=0)
		return NULL;

	try
	{
		if(GetConnectionForParse()==NULL || GetConnectionForParse()->State==adStateClosed)
		{
			return NULL;
		}


		TableName=GetTemporarilyTableName();
		SQL.Format("DROP TABLE %s",TableName);

		GetConnectionForParse()->Execute(_bstr_t(SQL),NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		//������
		if(e.Error()!=DB_E_NOTABLE)
		{
			AfxMessageBox(e.Description());
			return NULL;
		}
	}

	i=0;
	CreateTableStr.Format(_T("CREATE TABLE %s("),TableName);
	while(i<VarStructNum)
	{
		CreateTableStr+=pVarStruct[i].VarName;
		CreateTableStr+=_T(" ");

		switch(pVarStruct[i].VarValue.vt)
		{
		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_R4:
		case VT_R8:
			 {
				 CreateTableStr+=_T("float");
				 break;
			 }

		case VT_BSTR:
			{
				CreateTableStr+=_T("text");
				break;
			}
		default:
			return NULL;
		}

		if(i<VarStructNum-1)
		{
			CreateTableStr+=_T(",");
		}
		
		i++;

	}

	CreateTableStr+=_T(")");

	try
	{

		GetConnectionForParse()->Execute(_bstr_t(CreateTableStr),NULL,adCmdText);

		IRecordTable.CreateInstance(__uuidof(Recordset));

		SQL.Format(_T("SELECT* FROM %s"),TableName);
		IRecordTable->Open(_variant_t(SQL),_variant_t((IDispatch*)GetConnectionForParse()),
							 adOpenDynamic,adLockOptimistic,adCmdText);

		i=0;
		IRecordTable->AddNew();
		while(i<VarStructNum)
		{
			IRecordTable->PutCollect(_variant_t(pVarStruct[i].VarName),pVarStruct[i].VarValue);
			i++;
		}
		IRecordTable->Update();
	}
	catch(_com_error)
	{
		throw;
	}

	return IRecordTable.Detach();
}

/////////////////////////////////////////////////
//
// ɾ����ʱ��
//
void CParseExpression::DropTemporarilyTable()
{
	CString SQL;

	try
	{
		if(GetConnectionForParse()==NULL || GetConnectionForParse()->State==adStateClosed)
		{
			return;
		}

		SQL.Format(_T("DROP TABLE %s"),GetTemporarilyTableName());

		GetConnectionForParse()->Execute(_bstr_t(SQL),NULL,adCmdText);
	}
	catch(_com_error)
	{
		return;
	}

}

///////////////////////////////////////////////////////////////////
//
// ������ʱ��ı���
//
// TableName[in]	��ʱ��ı���
//
void CParseExpression::SetTemporarilyTableName(LPCTSTR TableName)
{
	if(TableName==NULL)
		return;

	m_TemporarilyTableName=TableName;
}

/////////////////////////////////////////////////////////////////
//
// ������ʱ��ı���
// ��Ҫ�ͷŷ��ص��ַ���
//
LPCTSTR CParseExpression::GetTemporarilyTableName()const
{
	return m_TemporarilyTableName;
}



//////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��FOXPRO�е��������ʽ���н���
//
// pExpression[in]	����FOXPRO�߼����ʽ
// pVarStruct[in]	tagExpressionVar�ṹ���ڳ�ʼ��pExpression�ı���
// VarStructNum[in]	pVarStruct�ṹ����Ŀ
// pState[out]		���ؽ����Ƿ�ɹ���־
//
// ����������ʽ�ɹ������ر��ʽ��ֵ
//
// ע�⣺pStatue��ΪNULL����pStatue��ΪNULLʱ��
//       �����ʽ�����ɹ�ʱ���ط�0�������ʽ����
//		 ʧ��ʱ����0����ʱ��������ֵ������
//
double CParseExpression::ParseUseConn_Evaluation(LPCTSTR pExpression, tagExpressionVar *pVarStruct, 
												 int VarStructNum, int *pState)
{
	_RecordsetPtr IRecordTable;
	CString SQL;
	_variant_t RetValue;

	if(pExpression==NULL || pVarStruct==NULL || VarStructNum<=0)
	{
		if(pState)
			*pState=0;
		return 0;
	}

	pVarStruct[0].VarValue=0.0;
	try
	{
		IRecordTable.Attach(CreateTemporarilyTable(pVarStruct,VarStructNum));

		if(IRecordTable==NULL)
		{
			if(pState)
				*pState=0;
			return FALSE;
		}

		IRecordTable->Close();
	}
	catch(_com_error)
	{
		throw;
	}

	SQL.Format("UPDATE %s SET %s",GetTemporarilyTableName(),pExpression);
	
	try
	{
		GetConnectionForParse()->Execute(_bstr_t(SQL),NULL,adCmdText);

		SQL.Format("SELECT %s FROM %s",pVarStruct[0].VarName,GetTemporarilyTableName());

		IRecordTable->Open(_variant_t(SQL),_variant_t((IDispatch*)GetConnectionForParse()),
						   adOpenDynamic,adLockOptimistic,adCmdText);

		if(pState)
			*pState=1;

		RetValue=IRecordTable->GetCollect(_variant_t((short)0));
	}
	catch(_com_error)
	{
		throw;
	}

	DropTemporarilyTable();

	return RetValue;
}



