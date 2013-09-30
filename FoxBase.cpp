// FoxBase.cpp: implementation of the CFoxBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FoxBase.h"
#include "vtot.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFoxBase::CFoxBase()
{
	m_hStdout=NULL;
}

CFoxBase::~CFoxBase()
{

}

////////////////////////////////////////////////////
//
//�ӱ�ĵ�ǰ��¼��ʼѰ��ָ�����߼���ϵ
//
//IRecordset[in] ��¼������ָ��
//FieldsName[in] �ֶ���
//Relations[in]  �߼���ϵ�����ڡ������ڡ���
//Value[in]		 ������FieldsName�Ƚϵ�ֵ
//
//�����ɹ�����TRUE,���򷵻�FALSE
//�����Ƚϵ��ֶ����ϵ��ƥ����׳�_com_error�쳣
//ָ��ָ���ҵ��ļ�¼��
//
BOOL CFoxBase::LocateForCurrent(_RecordsetPtr IRecordset, _variant_t FieldsName, int Relations, _variant_t Value)
{
	_variant_t tempvalue;
	CString tempstr;

	if(IRecordset==NULL || Value.vt==VT_NULL)
		return FALSE;
	

	while(!IRecordset->adoEOF)
	{
		tempvalue=IRecordset->GetCollect(FieldsName);

		if(tempvalue.vt==VT_NULL)
		{
			IRecordset->MoveNext();
			continue;
		}

		switch(Relations)
		{
		case CFoxBase::EQUAL:			// ����
			if(Value.vt==VT_BSTR)
			{
			//	tempstr=(TCHAR*)(_bstr_t)tempvalue;
			//	if(tempstr.Find((TCHAR*)(_bstr_t)Value) != -1)
			//		return TRUE;
			//	else
			//		break;
				tempstr = vtos(tempvalue);
				if (!tempstr.Compare(vtos(Value)))
				{
					return TRUE;
				}
				else
				{
					break;
				}

			}

			if(tempvalue==Value)
				return TRUE;
			else
				break;

		case CFoxBase::UNEQUAL:				//������
			if(tempvalue!=Value)
				return TRUE;
			else
				break;

		case CFoxBase::GREATER:				//����
			if((double)tempvalue>(double)Value)
				return TRUE;
			else
				break;

		case CFoxBase::GREATER_OR_EQUAL:	//���ڵ���
			if((double)tempvalue>=(double)Value)
				return TRUE;
			else
				break;

		case CFoxBase::LESS:				//С��
			if((double)tempvalue<(double)Value)
				return TRUE;
			else
				break;

		case CFoxBase::LESS_OR_EQUAL:		//С�ڵ���
			if((double)tempvalue<=(double)Value)
				return TRUE;
			else
				break;

		default:
			return FALSE;

		}

		IRecordset->MoveNext();
	}

	if(IRecordset->adoEOF)
		return FALSE;

	return TRUE;
}

////////////////////////////////////////////////////
//
//�ӱ�ĵ�һ����¼��ʼѰ��ָ�����߼���ϵ
//
//IRecordset[in] ��¼������ָ��
//FieldsName[in] �ֶ���
//Relations[in]  �߼���ϵ�����ڡ������ڡ���
//Value[in]		 ������FieldsName�Ƚϵ�ֵ
//
//�����ɹ�����TRUE,���򷵻�FALSE
//�����Ƚϵ��ֶ����ϵ��ƥ����׳�_com_error�쳣
//ָ��ָ���ҵ��ļ�¼��
//
BOOL CFoxBase::LocateFor(_RecordsetPtr IRecordset, _variant_t FieldsName, 
						 int Relations, _variant_t Value)
{
	_variant_t tempvalue;
	CString tempstr;

	if(IRecordset==NULL || Value.vt==VT_NULL)
		return FALSE;
	
	try
	{
		IRecordset->MoveFirst();
	}
	catch(_com_error &e)
	{
		ExceptionInfo(e);
		return FALSE;
	}

	return LocateForCurrent(IRecordset,FieldsName,Relations,Value);

}

///////////////////////////////////////////////
//
//���ص�ǰ��¼�����α�λ��
//
//IRecordset[in] ��¼������ָ��
//
//�����ɹ����ص�ǰ��¼�����α�λ�ã����򷵻�-1
//
long CFoxBase::RecNo(_RecordsetPtr IRecordset)
{
	long pos=0;
	long i;

	if(IRecordset==NULL)
		return -1;

	if(IRecordset->adoEOF && IRecordset->BOF)
		return -1;

	while(!IRecordset->GetBOF())
	{
		pos++;
		IRecordset->MovePrevious();
	}

	i=pos;

	i--;
	IRecordset->MoveFirst();

	while(i>0)
	{
		i--;
		IRecordset->MoveNext();
	}

	return pos;
}


///////////////////////////////////////////////////////////
//
//�滻��¼����ѡ���ֶ����е�ֵ
//
//IRecordset[in] ��¼������ָ��
//FieldsName[in] �ֶ���
//Value[in]		 �����滻FieldsName�ֶε�ֵ
//
//�����ɹ�����TRUE�����򷵻�FALSE
//�������ɹ�����ʱ����ǰ�α�ָ��ָ���¼�����һ����¼�ĺ���
//
BOOL CFoxBase::ReplAll(_RecordsetPtr IRecordset, _variant_t FieldsName, _variant_t Value)
{
	if(IRecordset==NULL)
		return FALSE;

	IRecordset->MoveFirst();

	while(!IRecordset->adoEOF)
	{
		IRecordset->PutCollect(FieldsName,Value);
		IRecordset->Update();
		IRecordset->MoveNext();
	}
	return TRUE;
}

//////////////////////////////////////////
//
//�ж��ļ��Ƿ����
//
//pFilePath[in]�ļ��ľ���·��
//
//����ļ������򷵻�TRUE,���򷵻�FALSE
//
BOOL CFoxBase::FILE(LPCTSTR pFilePath)
{
	HANDLE hFile;
	if(pFilePath==NULL)
		return FALSE;

	hFile=CreateFile(pFilePath,
					 GENERIC_READ,
					 FILE_SHARE_READ,
					 NULL,
					 OPEN_EXISTING,
					 FILE_ATTRIBUTE_NORMAL,
					 NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		//�ļ�������  
		if(GetLastError()==ERROR_FILE_NOT_FOUND)
			return FALSE;
	}

	CloseHandle(hFile);
	return TRUE;
}

///////////////////////////////////////////////////////////
//
//�滻�Ӽ�¼����ѡ���ֶ�ָ������¼��ֵ(�൱��FOXPRO�� Replace Next)
//
//IRecordset[in]	��¼������ָ��
//FieldsName[in]	�ֶ���
//Value[in]			�����滻FieldsName�ֶε�ֵ
//num[in]			���滻�ļ�¼��
//
//�����ɹ�����TRUE�����򷵻�FALSE
//�����ӵ�ǰ�α�λ�ÿ�ʼ�滻
//�������ɹ�����ʱ����ǰ�α�ָ��ָ������޸ĵļ�¼����
//�����滻�ļ�¼���������һ����¼ʱ,�α�ָ�����һ����¼�ĺ���
//
BOOL CFoxBase::ReplNext(_RecordsetPtr IRecordset, _variant_t FieldsName, _variant_t Value, int num)
{
	if(IRecordset==NULL || num<=0)
		return FALSE;


	while(!IRecordset->adoEOF)
	{
		IRecordset->PutCollect(FieldsName,Value);
		IRecordset->Update();

		num--;
		if(num<=0)
			break;
		
		IRecordset->MoveNext();
	}
	return TRUE;
}

int CFoxBase::MessageBox(LPCTSTR pStr)
{
	return ::MessageBox(NULL,"",pStr,MB_OK);
}

////////////////////////////////////////////
//
//�˳�Ӧ�ó������
//
void CFoxBase::Quit()
{

}

///////////////////////////////////////////////
//
//	����������Ϣʱ���ô˺���
//
//	pStr[in]	������ʾ����Ϣ�ַ���
//
void CFoxBase::Say(LPCTSTR pStr)
{
	::MessageBox(NULL,pStr,"",MB_OK);
}

////////////////////////////////////////////////////////
//
// ���ȴ������ַ���ʱ���ô˺���
//
// pInfoStr[in]		������ʾ����Ϣ�ַ���
// strRet[out]		�践�ص��ַ���
//
void CFoxBase::Wait(LPCTSTR pInfoStr,CString &strRet)
{
	::MessageBox(NULL,"",pInfoStr,MB_OK);
	strRet=_T("F");
}

////////////////////////////////////////////////////////
//
// ����������ֵʱ���ô˺���
// 
// pstr[in]			������ʾ����Ϣ�ַ���
// val[out]			�践�صı���
//
void CFoxBase::InputD(LPCTSTR pstr, double &val)
{
	::MessageBox(NULL,pstr,"",MB_OK);
}

///////////////////////////////////////////////////////
//
// ����������Ϣʱ���ô˺���
//
// pStr[in]			������ʾ����Ϣ�ַ���
//
void CFoxBase::DisplayRes(LPCTSTR pStr)
{
//	AfxMessageBox(pStr);
	DWORD cWritten;
    
	if(AllocConsole())
	{
		m_hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
	}
	
	m_hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
	WriteFile(m_hStdout,pStr,lstrlen(pStr),&cWritten,NULL);
	WriteFile(m_hStdout,"\n",lstrlen("\n"),&cWritten,NULL);	
}

//////////////////////////////////////////////////
//
//�����쳣ʱ���ô˺���
//
//pErrorInfo[in]�쳣��Ϣ
//
void CFoxBase::ExceptionInfo(LPCTSTR pErrorInfo)
{
	AfxMessageBox(pErrorInfo);
}

void CFoxBase::ExceptionInfo(_com_error &e)
{
	if(e.Description().length()>0)
		ExceptionInfo(e.Description());
	else
		ExceptionInfo(e.ErrorMessage());
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// �Ӽ�¼���з���ָ���ֶε�ֵ
//
// IRecordset[in]	��¼������ָ��	
// FieldsName[in]	�ֶ���
// RetValue[out]	���صı���
//
// ������쳣������ExceptionInfo����
//
void CFoxBase::GetTbValue(_RecordsetPtr IRecordset, _variant_t FieldsName, double &RetValue)
{
	_variant_t TempValue;
	CString strTemp;

	if(IRecordset==NULL)
		return;
	
	try
	{
		TempValue=IRecordset->GetCollect(FieldsName);
		
		if(TempValue.vt!=VT_NULL)
		{
			if(TempValue.vt==VT_BSTR)
			{
				CString Temp;
				Temp=(TCHAR*)(_bstr_t)TempValue;
				Temp.TrimLeft();

				if(Temp.GetLength()==0)
				{
					RetValue=0.0;
					//break;
					return;
				}

			}
			RetValue=TempValue;
		}
		else
		{
			RetValue=0.0;
		}
	}
	catch(_com_error &e)
	{
		RetValue=0.0;
//		ExceptionInfo(e);

		strTemp.Format(_T("�����ڴ�%s�ֶζ�ȡ����ʱ"),(LPCTSTR)(_bstr_t)FieldsName);
		Exception::SetAdditiveInfo(strTemp);
		ReportExceptionErrorLV2(e);

		throw;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �Ӽ�¼���з���ָ���ֶε�ֵ
//
// IRecordset[in]	��¼������ָ��	
// FieldsName[in]	�ֶ���
// RetValue[out]	���صı���
//
// ������쳣������ExceptionInfo����
//
void CFoxBase::GetTbValue(_RecordsetPtr IRecordset, _variant_t FieldsName, float &RetValue)
{
	_variant_t TempValue;
	CString strTemp;

	if(IRecordset==NULL)
		return;
	
	try
	{
		TempValue=IRecordset->GetCollect(FieldsName);
		
		if(TempValue.vt!=VT_NULL)
		{
			if(TempValue.vt==VT_BSTR)
			{
				CString Temp;
				Temp=(TCHAR*)(_bstr_t)TempValue;
				Temp.TrimLeft();

				if(Temp.GetLength()==0)
				{
					RetValue=0.0;
					//break;
					return;
				}

			}
			RetValue=TempValue;
		}
		else
		{
			RetValue=0.0;
		}
	}
	catch(_com_error &e)
	{
		RetValue=0.0;
//		ExceptionInfo(e);

		strTemp.Format(_T("�����ڴ�%s�ֶζ�ȡ����ʱ"),(LPCTSTR)(_bstr_t)FieldsName);
		Exception::SetAdditiveInfo(strTemp);

		ReportExceptionErrorLV2(e);
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �Ӽ�¼���з���ָ���ֶε�ֵ
//
// IRecordset[in]	��¼������ָ��	
// FieldsName[in]	�ֶ���
// RetValue[out]	���صı���
//
// ������쳣������ExceptionInfo����
//
void CFoxBase::GetTbValue(_RecordsetPtr IRecordset, _variant_t FieldsName, int &RetValue)
{
	_variant_t TempValue;
	CString strTemp;

	if(IRecordset==NULL)
		return;
	
	try
	{
		TempValue=IRecordset->GetCollect(FieldsName);
		
		if(TempValue.vt!=VT_NULL)
		{
			if(TempValue.vt==VT_BSTR)
			{
				CString Temp;
				Temp=(TCHAR*)(_bstr_t)TempValue;
				Temp.TrimLeft();

				if(Temp.GetLength()==0)
				{
					RetValue=0;
					//break;
					return;
				}

			}
			RetValue=(long)TempValue;
		}
		else
		{
			RetValue=0;
		}
	}
	catch(_com_error &e)
	{
		RetValue=0;
//		ExceptionInfo(e);

		strTemp.Format(_T("�����ڴ�%s�ֶζ�ȡ����ʱ"),(LPCTSTR)(_bstr_t)FieldsName);
		Exception::SetAdditiveInfo(strTemp);

		ReportExceptionErrorLV2(e);
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// �Ӽ�¼���з���ָ���ֶε�ֵ
//
// IRecordset[in]	��¼������ָ��	
// FieldsName[in]	�ֶ���
// RetValue[out]	���صı���
//
// ������쳣������ExceptionInfo����
//
void CFoxBase::GetTbValue(_RecordsetPtr IRecordset, _variant_t FieldsName, CString &RetValue)
{
	_variant_t TempValue;
	CString strTemp;

	if(IRecordset==NULL)
		return;
	
	try
	{
		TempValue=IRecordset->GetCollect(FieldsName);
		
		if(TempValue.vt!=VT_NULL)
		{
			RetValue=(TCHAR*)(_bstr_t)TempValue;

			RetValue.TrimLeft();
			RetValue.TrimRight();
		}
		else
		{
			RetValue=_T("");
		}
	}
	catch(_com_error &e)
	{
		RetValue=_T("");
//		ExceptionInfo(e);

		strTemp.Format(_T("�����ڴ�%s�ֶζ�ȡ����ʱ"),(LPCTSTR)(_bstr_t)FieldsName);
		Exception::SetAdditiveInfo(strTemp);

		ReportExceptionErrorLV2(e);
		throw;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// �ڼ�¼���е�ǰ�α�λ������ָ�����ֶε�ֵ
//
// IRecordset[in]	��¼������ָ��	
// FieldsName[in]	�ֶ���
// Value[in]		�µ�ֵ
//
// ������쳣������ExceptionInfo����
//
void CFoxBase::PutTbValue(_RecordsetPtr IRecordset, _variant_t FieldsName, double Value)
{
	if(IRecordset==NULL)
		return;

	try
	{
		IRecordset->PutCollect(FieldsName,_variant_t(Value));
		IRecordset->Update();
	}
	catch(_com_error /*&e*/)
	{
//		ExceptionInfo(e);
		throw;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// �ڼ�¼���е�ǰ�α�λ������ָ�����ֶε�ֵ
//
// IRecordset[in]	��¼������ָ��	
// FieldsName[in]	�ֶ���
// Value[in]		�µ�ֵ
//
// ������쳣������ExceptionInfo����
//
void CFoxBase::PutTbValue(_RecordsetPtr IRecordset, _variant_t FieldsName, float Value)
{
	PutTbValue(IRecordset,FieldsName,(double)Value);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// �ڼ�¼���е�ǰ�α�λ������ָ�����ֶε�ֵ
//
// IRecordset[in]	��¼������ָ��	
// FieldsName[in]	�ֶ���
// Value[in]		�µ�ֵ
//
// ������쳣������ExceptionInfo����
//
void CFoxBase::PutTbValue(_RecordsetPtr IRecordset, _variant_t FieldsName, int Value)
{
	PutTbValue(IRecordset,FieldsName,(double)Value);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// �ڼ�¼���е�ǰ�α�λ������ָ�����ֶε�ֵ
//
// IRecordset[in]	��¼������ָ��	
// FieldsName[in]	�ֶ���
// Value[in]		�µ�ֵ
//
// ������쳣������ExceptionInfo����
//
// ���ValueΪ�ջ�ȫΪ�ո��ַ�����ָ�����ֶε�����Ϊ�ַ�����ʱ���ֶε�ֵΪ��
// ����ֶε�����Ϊ�����ͣ��ֶε�ֵΪ0
//
void CFoxBase::PutTbValue(_RecordsetPtr IRecordset, _variant_t FieldsName, CString Value)
{
	adoDataTypeEnum DataType;

	if(IRecordset==NULL)
		return;

	Value.TrimLeft();
	Value.TrimRight();

	try
	{
		if(Value.IsEmpty())
		{
			IRecordset->GetFields()->GetItem(FieldsName)->get_Type(&DataType);
			
			if(DataType==adBSTR || DataType==adChar || DataType==adVarChar || 
			   DataType==adVarWChar ||DataType==adWChar)
			{
				Value=_T("");
			}
			else
			{
				Value=_T("0");
			}
		}

		IRecordset->PutCollect(FieldsName,_variant_t(Value));
		IRecordset->Update();
	}
	catch(_com_error /*&e*/)
	{
//		ExceptionInfo(e);
		throw;
	}

}

//////////////////////////////////////////////////
//
// �������ݿ������
//
// IConnection[in]	���ݿ�����ӵ�����ָ�������
//
void CFoxBase::SetConnect(_ConnectionPtr &IConnection)
{
	m_Connection=IConnection;
}

//////////////////////////////////////////////////
//
// �������ݿ������
//
// �����������ݿ�����ӵ�����ָ�������
//
_ConnectionPtr& CFoxBase::GetConnect()
{
	return m_Connection;
}

CString CFoxBase::CreateTableSQL(_RecordsetPtr &IRecord,LPCTSTR pTableName)
{
	short Item;
	CString CreateSQL;
	CString DefList;
	FieldsPtr IFields;
	FieldPtr IField;

	CreateSQL=_T("");

	if(IRecord==NULL)
	{
		ExceptionInfo(_T("Recordset interface cann't be NULL"));
		return CreateSQL;
	}

	if(pTableName==NULL)
	{
		ExceptionInfo(_T("Table name cann't be NULL"));
		return CreateSQL;
	}

	IRecord->get_Fields(&IFields);

	DefList=_T("");
	for(Item=0;Item<IFields->GetCount();Item++)
	{
		IFields->get_Item(_variant_t(Item),&IField);
		DefList+=IField->GetName();
		DefList+=_T(" ");

		switch(IField->GetType())
		{
		case adVarWChar:
			{
				CString Temp;
				Temp.Format(_T("varchar(%d)"),IField->GetDefinedSize());
				DefList+=Temp;
				break;
			}

		case adLongVarWChar:
			DefList+=_T("text");
			break;

		case adVarBinary:
			{
				CString Temp;
				Temp.Format(_T("varbinary(%d)"),IField->GetDefinedSize());
				DefList+=Temp;
				break;
			}

//		case adNumeric:
//		case adGUID:
//		case adLongVarBinary:
//			{
//				ExceptionInfo(_T("��֧��LongVarBinary"));
//				return CreateSQL;
//			}
//			break;

		case adInteger:
			DefList+=_T("int");
			break;

		case adUnsignedTinyInt:
		case adSmallInt:
			DefList+=_T("smallint");
			break;

		case adSingle:
		case adDouble:
			DefList+=_T("float");
			break;

		case adDBTimeStamp:
		case adDate:
			DefList+=_T("date");
			break;

//		case adBoolean:
//			break;

		default:
			{
				ExceptionInfo(_T("��֧�ִ�����"));
				return CreateSQL;
			}

		}

		if(Item < IFields->GetCount()-1)
		{
			DefList+=_T(",");
		}

		IField.Release();
	}

	CreateSQL.Format(_T("CREATE TABLE %s(%s)"),pTableName,DefList);
	return CreateSQL;
}

BOOL CFoxBase::CopyData(_RecordsetPtr &IRecordS, _RecordsetPtr &IRecordD)
{
	_variant_t TempValue;
	short Item;
	FieldsPtr IFields;
	FieldPtr IField;

	if(IRecordS==NULL || IRecordD==NULL)
	{
		ExceptionInfo(_T("Source Recordset or destination Recordset cann't be empty"));
		return FALSE;
	}

	if(IRecordS->adoEOF && IRecordS->BOF)
	{
		return TRUE;
	}

	if(!IRecordD->adoEOF || !IRecordD->BOF)
	{
		try
		{
			IRecordD->MoveLast();
		}
		catch(_com_error &e)
		{
			ExceptionInfo(e);
			return FALSE;
		}
	}

	try
	{
		IRecordS->MoveFirst();
	}
	catch(_com_error &e)
	{
		ExceptionInfo(e);
		return FALSE;
	}

	try
	{
		while(!IRecordS->adoEOF)
		{
			IRecordS->get_Fields(&IFields);
			IRecordD->AddNew();
			for(Item=0;Item<IFields->GetCount();Item++)
			{
				IFields->get_Item(_variant_t(Item),&IField);

				TempValue=IRecordS->GetCollect(_variant_t(IField->GetName()));
				IRecordD->PutCollect(_variant_t(IField->GetName()),TempValue);

				IField.Release();
			}
			IRecordD->Update();
			IFields.Release();
			IRecordS->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ExceptionInfo(e);
		return FALSE;
	}


	return TRUE;
}

BOOL CFoxBase::CopyTo(LPCTSTR pSourceTable, LPCTSTR pDestinationTable)
{
	CString SourceTable,DestinationTable;
	CString SQL;
	_RecordsetPtr IRecordS,IRecordD;
	FieldsPtr IFields;
	FieldPtr IField;

	if(pSourceTable==NULL || pDestinationTable==NULL)
	{
		ExceptionInfo(_T("Source table or destination table cann't be empty"));
		return FALSE;
	}

	SourceTable=pSourceTable;
	DestinationTable=pDestinationTable;

	SourceTable.TrimLeft();
	SourceTable.TrimRight();
	DestinationTable.TrimLeft();
	DestinationTable.TrimRight();

	if(SourceTable.IsEmpty() || DestinationTable.IsEmpty())
	{
		ExceptionInfo(_T("Source table or destination table cann't be empty"));
		return FALSE;
	}

	try
	{
		IRecordS.CreateInstance(__uuidof(Recordset));

		IRecordS->Open(_variant_t(pSourceTable),_variant_t((IDispatch*)GetConnect()),	
					  adOpenKeyset,adLockOptimistic,adCmdTable);
	}
	catch(_com_error &e)
	{
		ExceptionInfo(e);
		return FALSE;
	}
	
	SQL=CreateTableSQL(IRecordS,pDestinationTable);

	if(SQL.IsEmpty())
	{
		return FALSE;
	}

	try
	{
		GetConnect()->Execute(_bstr_t(SQL),NULL,adExecuteNoRecords);
	}
	catch(_com_error &e)
	{
		ExceptionInfo(e);
		return FALSE;
	}

	try
	{
		IRecordD.CreateInstance(__uuidof(Recordset));

		IRecordD->Open(_variant_t(pDestinationTable),_variant_t((IDispatch*)GetConnect()),	
					  adOpenKeyset,adLockOptimistic,adCmdTable);
	}
	catch(_com_error &e)
	{
		ExceptionInfo(e);
		return FALSE;
	}

	if(!CopyData(IRecordS,IRecordD))
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////
//
// �ڼ�¼���ĵ�ǰλ�ò���һ���¼�¼
//
// IRecord[in]	��¼������ָ��
//
// �����ɹ�����TRUE�����򷵻�FALSE
// ������쳣������ExceptionInfo����
//
// �����ɹ����¼����ָ���²���Ŀռ�¼
//
BOOL CFoxBase::InsertNew(_RecordsetPtr &IRecord,int after)
{
	int pos;
	CMap<short,short&,_variant_t,_variant_t&> FieldMap;
	CMap<short,short&,_variant_t,_variant_t&> newFieldMap;		//�������ӵļ�¼��ֵ������
	_variant_t TempValue;
	short Item;
	FieldsPtr IFields;
	FieldPtr IField;
    int pos1;
	int	nCount;		//�ֶθ���.

	if(IRecord==NULL)
	{
		ExceptionInfo(_T("Recordset cann't be empty"));
		return FALSE;
	}

	//�����¼��Ϊ�ս�����һ���¼�¼
	if(IRecord->adoEOF && IRecord->BOF)
	{
		try
		{
			IRecord->AddNew();
			IRecord->Update();
		}
		catch(_com_error &e)
		{
			ExceptionInfo(e);
			return FALSE;
		}
		return TRUE;
	}
	else if(IRecord->adoEOF)
	{
		IRecord->AddNew();
		IRecord->Update();
		IRecord->MoveLast();
		return true;
	}

	for(pos=0; !IRecord->adoEOF ;pos++)
	{
		try
		{
			IRecord->MoveNext();
		}
		catch(_com_error &e)
		{
			ExceptionInfo(e);
			return FALSE;
		}
	}

	//
	// ����һ���¼�¼�������տ�ʼ��ָ���Ժ�ļ�¼�����
	//
	try
	{
		IRecord->AddNew();
		IRecord->Update();
		IRecord->MoveLast();

		//�������ӵļ�¼��ֵ������.
		IFields = IRecord->GetFields();
		nCount = IFields->GetCount();   //�ֶθ���

		for (Item = 0; Item < nCount; Item++)
		{
			newFieldMap[Item] = IRecord->GetCollect(_variant_t(Item));
		}
		IFields.Release();
		//		

		IRecord->MovePrevious();
	}
	catch(_com_error &e)
	{
		ExceptionInfo(e);
		return FALSE;
	}
	if(after==1)
    pos--;  //ʹpos�ټ�һ�������ָ��ĵ�ǰλ���ϼ���һ���¼�¼��insert blank
	pos1=pos;
	while(pos>0)
	{
		try
		{
			for(Item=0;Item<nCount;Item++)
			{
				TempValue=IRecord->GetCollect(_variant_t(Item));
				FieldMap[Item]=TempValue;
			}

			IRecord->MoveNext();

			for(Item=0;Item<nCount;Item++)
			{
				IRecord->PutCollect(_variant_t(Item),FieldMap[Item]);
			}
			IRecord->Update();

			IRecord->MovePrevious();

			pos--;
			if(pos>0)
				IRecord->MovePrevious();
		}
		catch(_com_error &e)
		{
			ExceptionInfo(e);
			return FALSE;
		}
	}

	// ʹ�ʼ��ָ�ļ�¼����Ϊ��
	if(pos1==0 && after==1) {IRecord->MoveNext();}
	else
	{

	try
	{
		TempValue.Clear();

		for(Item=0;Item<nCount;Item++)
		{
			IRecord->PutCollect(_variant_t(Item),newFieldMap[Item]);
		}
		IRecord->Update();
	}
	catch(_com_error &e)
	{
		ExceptionInfo(e);
		return FALSE;
	}
	}

	return TRUE;
}

//�滻ָ����Χ��ĳ�ֶε�ֵ.
bool CFoxBase::ReplaceAreaValue(_RecordsetPtr pRs, CString strField, CString strValue, int startRow, int endRow)
{
	if(startRow > endRow)
	{
		endRow = startRow;
	}
	if( endRow <= 0 )
	{
		return false;
	}
	if( endRow > pRs->GetRecordCount() )
	{
		return false;
	}
	if( pRs->GetRecordCount() <= 0 )
	{
		return false;
	}
	pRs->MoveFirst();
	for(int i=1; i<=endRow && !pRs->adoEOF; i++, pRs->MoveNext())
	{
		if( i >= startRow )
		{
			pRs->PutCollect(_variant_t(strField), _variant_t(strValue));
			pRs->Update();
		}
	}
	return true;
}
