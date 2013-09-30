// ProjectOperate.cpp: implementation of the CProjectOperate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "ProjectOperate.h"

#include "excel9.h"
#include "vtot.h"
#include "mobject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjectOperate::CProjectOperate()
{
	m_bAuto = false;
}

CProjectOperate::~CProjectOperate()
{

}

//////////////////////////////////////////////////////////
//
// ���ù��̵�ID
//
// strProjectID	[in]���̵�ID
//
void CProjectOperate::SetProjectID(LPCTSTR strProjectID)
{
	m_strProjectID=strProjectID; 
}

//////////////////////////////////////////////////////////////
//
// ���ع��̵�ID
//
CString CProjectOperate::GetProjectID()
{
	return m_strProjectID;
}

///////////////////////////////////////////////////////////////////////
//
// ���ù������ݿ������
//
// IConnection[in]	���ݿ������
//
void CProjectOperate::SetProjectDbConnect(_ConnectionPtr IConnection)
{
	m_ProjectDbConnect=IConnection;
}

////////////////////////////////////////////////////////////
//
// ���ع������ݿ������
//
_ConnectionPtr CProjectOperate::GetProjectDbConnect()
{
	return m_ProjectDbConnect;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// ��ImportFromXLSStruct�ṹ��pElement����Ѱ��ƥ��ĵ�Ԫ
//
// szSource[in]	��ImportFromXLSElement��SourceFieldNameƥ����ַ���
// szDest[in]	��ImportFromXLSElement��DestinationNameƥ����ַ���
//
// ����ҵ�����ָ��˵�Ԫ��ָ�룬���û�ҵ�����NULL
// 
// szSource,szDest��Ϊ�գ������ΪNULL������NULL
// ��Сд������
//
CProjectOperate::ImportFromXLSElement* 
CProjectOperate::CImportFromXLSStruct::FindInElement(LPCTSTR szSource, LPCTSTR szDest)
{
	ImportFromXLSElement *pElement;
	CString strTemp;
	int iIsFind=0;

	if(szSource==NULL && szDest==NULL)
		return NULL;

	if(this->pElement==NULL || this->ElementNum<=0)
		return NULL;

	for(int i=0;i<this->ElementNum;i++)
	{
		iIsFind=1;
		pElement=&this->pElement[i];

		if(szSource)
		{
			strTemp=szSource;
			strTemp.MakeUpper();
			pElement->SourceFieldName.MakeUpper();

			if(pElement->SourceFieldName==strTemp)
			{
				iIsFind &= 1;
			}
			else
			{
				iIsFind &=0;
			}
		}

		if(szDest)
		{
			strTemp=szDest;
			strTemp.MakeUpper();
			pElement->DestinationName.MakeUpper();

			if(pElement->DestinationName==strTemp)
			{
				iIsFind &= 1;
			}
			else
			{
				iIsFind &=0;
			}
		}

		if(iIsFind==1)
			break;
	}

	if(i==this->ElementNum)
		return NULL;

	return &this->pElement[i];
}

/////////////////////////////////////////////////////////////////////////
//
// ��Excel��XLS�ļ���������
//
// pImportStruct[in]	ImportFromXLSStruct�ṹ���ں��������Ϣ
//
// throw(_com_error)
// throw(COleDispatchException*)
// throw(COleException*)
//
void CProjectOperate::ImportFromXLS(ImportFromXLSStruct *pImportStruct)
{
	BOOL bRet;
	LPDISPATCH pDispatch;
	_Application Application;
	Workbooks workbooks;
	_Workbook workbook;
	Worksheets worksheets;
	_Worksheet worksheet;
	Range range;

	_RecordsetPtr IRecordset;
	HRESULT hr;
	CString SQL;
	

	if(pImportStruct==NULL || pImportStruct->pElement==NULL || 
	   pImportStruct->ElementNum<=0 || GetProjectDbConnect()==NULL)
	{
		_com_error e(E_INVALIDARG);

		ReportExceptionErrorLV2(e);
		throw e;
	}

	bRet=Application.CreateDispatch(_T("Excel.Application"));

	if(!bRet)
	{
		ReportExceptionErrorLV1(_T("���Ȱ�װExcel!"));
		return;
	}

	//��ù������ļ���
	pDispatch=Application.GetWorkbooks();
	workbooks.AttachDispatch(pDispatch);

	try
	{
		//��ָ����EXCEL�ļ�
		pDispatch=workbooks.Open(pImportStruct->XLSFilePath,vtMissing,vtMissing,vtMissing,vtMissing,
								 vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,
								 vtMissing);
		workbook.AttachDispatch(pDispatch);

		//���ָ��������
		pDispatch=workbook.GetWorksheets();
		worksheets.AttachDispatch(pDispatch);

		//���ָ���Ĺ�����
		pDispatch=worksheets.GetItem(_variant_t(pImportStruct->XLSTableName));
		worksheet.AttachDispatch(pDispatch);
	}
	catch(COleDispatchException *e)
	{
		Application.Quit();

		CString str;
		str.Format(_T("��%s�ļ����%s���������"),pImportStruct->XLSFilePath,pImportStruct->XLSTableName);
		Exception::SetAdditiveInfo(_T(str));

		ReportExceptionErrorLV2(e);
		throw;
	}
	catch(COleException *e)
	{
		Application.Quit();

		_com_error e2(e->m_sc);
		e->Delete();
		ReportExceptionErrorLV2(e2);
		throw e2;
	}


	hr=IRecordset.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		Application.Quit();

		_com_error e(hr);
		ReportExceptionErrorLV2(e);
		throw e;
	}

	//
	// ��ָ�����̵ı�
	// 
	//
	if(pImportStruct->ProjectID.IsEmpty())
	{
		SQL.Format(_T("SELECT * FROM %s"),pImportStruct->ProjectDBTableName);
	}
	else
	{
		SQL.Format(_T("SELECT * FROM %s WHERE EnginID='%s'"),pImportStruct->ProjectDBTableName,pImportStruct->ProjectID);
	}

	try
	{
		IRecordset->CursorLocation=adUseClient;

		IRecordset->Open(_variant_t(SQL),
						 _variant_t((IDispatch*)this->GetProjectDbConnect()),
					     adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		Application.Quit();

		ReportExceptionErrorLV2(e);
		throw;
	}

	int iRow;	// ��ʼ������к�
	int Count;	// �ѵ���ļ�¼��
	CString strTemp,strSourceElementName;
	_variant_t varTemp;

	//��ʼ����
	this->GetProjectDbConnect()->BeginTrans();

	//
	// ��ָ�����п�ʼ���������д�EXCEL�ļ������ݵ����ݿ�
	//
	for(iRow=pImportStruct->BeginRow,Count=0;Count<pImportStruct->RowCount;Count++,iRow++)
	{
		try
		{
			IRecordset->AddNew();

			if(!pImportStruct->ProjectID.IsEmpty())
			{
				IRecordset->PutCollect(_variant_t("EnginId"),_variant_t(pImportStruct->ProjectID));
			}
		}
		catch(_com_error &e)
		{
			Application.Quit();

			//�ع�
			this->GetProjectDbConnect()->RollbackTrans();

			ReportExceptionErrorLV2(e);
			throw;
		}

		//
		// ��һ�м�¼�д�EXCEL��Ԫ�е���Ӧ�Ĺ������ݿ���ֶ�
		//
		for(int i=0;i<pImportStruct->ElementNum;i++)
		{
			strSourceElementName.Format(_T("%s%d"),pImportStruct->pElement[i].SourceFieldName,iRow);

			try
			{
				pDispatch=worksheet.GetRange(_variant_t(strSourceElementName),vtMissing);
				range.AttachDispatch(pDispatch);

				varTemp=range.GetValue();

				range.ReleaseDispatch();
			}
			catch(COleDispatchException *e)
			{
				Application.Quit();
				this->GetProjectDbConnect()->RollbackTrans();
		
				strTemp.Format(_T("����Excel��%s��Ԫ��������ʱ����"),strSourceElementName);
				Exception::SetAdditiveInfo(strTemp);

				ReportExceptionErrorLV2(e);
				throw;
			}
			catch(COleException *e)
			{
				Application.Quit();
				this->GetProjectDbConnect()->RollbackTrans();

				_com_error e2(e->m_sc);
				e->Delete();
				ReportExceptionErrorLV2(e2);
				throw e2;
			}

			try
			{
				IRecordset->PutCollect(_variant_t(pImportStruct->pElement[i].DestinationName),varTemp);
			}
			catch(_com_error &e)
			{
				Application.Quit();

				this->GetProjectDbConnect()->RollbackTrans();


				//
				// ���쳣�������������Ϣ
				//
				strTemp.Format(_T("��%s%d�������д���"),
							   pImportStruct->pElement[i].SourceFieldName,
							   iRow);
				Exception::SetAdditiveInfo(strTemp);

				ReportExceptionErrorLV2(e);
				throw;
			}
		}
	}

	try
	{
		IRecordset->Update();
	}
	catch(_com_error &e)
	{
		Application.Quit();

		this->GetProjectDbConnect()->RollbackTrans();

		ReportExceptionErrorLV2(e);
		throw;
	}

	//�������и��Ĳ�������ǰ����
	this->GetProjectDbConnect()->CommitTrans();

	workbook.Close(_variant_t(false),vtMissing,vtMissing);
	workbooks.Close();
	Application.Quit();
}

/////////////////////////////////////////////////////////////////////////
//
// ��Excel��XLS�ļ��������ݵ�PRE_CALC��
//
// pImportStruct[in]	ImportFromXLSStruct�ṹ���ں��������Ϣ
// IsAutoMakeID[in]		�Ƿ��Զ���ID���ֶΣ�TRUEΪ�Զ���д����ʱ
//						��ImportFromXLSStruct�Ľṹ�в�Ӧ�ú���ID�ֶε���Ϣ
// KeyField[in]			�ؼ��ֶε�����
//
//
// throw(_com_error)
// throw(COleDispatchException*)
// throw(COleException*)
//
//
// ע��:ʵ���ϴ˺��������Ե��������ı�������PRE_CALC��
// ������������ı�IsAutoMakeIDӦ����KeyField�ֶε�ֵ
// ����++��������ֵ�ͱ����ſ�ΪTRUE������ΪFALSE��
// 
// ��IsAutoMakeIDΪFALSEʱ��������ݿ�������KeyField��ͬ��ֵʱ
// ������¼���ᱻ�����¼�¼�滻
//
void CProjectOperate::ImportPre_CalcFromXLS(ImportFromXLSStruct *pImportStruct, BOOL IsAutoMakeID,LPCTSTR KeyField)
{
	BOOL bRet;
	LPDISPATCH pDispatch;
	_Application Application;
	Workbooks workbooks;
	_Workbook workbook;
	Worksheets worksheets;
	_Worksheet worksheet;
	Range range;

	_RecordsetPtr IRecordset;
	HRESULT hr;
	CString SQL;
	

	if(pImportStruct==NULL || pImportStruct->pElement==NULL || 
	   pImportStruct->ElementNum<=0 || GetProjectDbConnect()==NULL)
	{
		_com_error e(E_INVALIDARG);

		ReportExceptionErrorLV2(e);
		throw e;
	}

	bRet=Application.CreateDispatch(_T("Excel.Application"));

	if(!bRet)
	{
		ReportExceptionErrorLV1(_T("���Ȱ�װExcel!"));
		return;
	}

	//��ù������ļ���
	pDispatch=Application.GetWorkbooks();
	workbooks.AttachDispatch(pDispatch);

	try
	{
		//��ָ����EXCEL�ļ�
		pDispatch=workbooks.Open(pImportStruct->XLSFilePath,vtMissing,vtMissing,vtMissing,vtMissing,
								 vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,
								 vtMissing);
		workbook.AttachDispatch(pDispatch);

		//���ָ��������
		pDispatch=workbook.GetWorksheets();
		worksheets.AttachDispatch(pDispatch);

		//���ָ���Ĺ�����
		pDispatch=worksheets.GetItem(_variant_t(pImportStruct->XLSTableName));
		worksheet.AttachDispatch(pDispatch);
	}
	catch(COleDispatchException *e)
	{
		Application.Quit();

		CString str;
		str.Format(_T("��%s�ļ����%s���������"),pImportStruct->XLSFilePath,pImportStruct->XLSTableName);
		Exception::SetAdditiveInfo(_T(str));

		ReportExceptionErrorLV2(e);
		throw;
	}
	catch(COleException *e)
	{
		Application.Quit();

		_com_error e2(e->m_sc);
		e->Delete();
		ReportExceptionErrorLV2(e2);
		throw e2;
	}


	hr=IRecordset.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		Application.Quit();

		_com_error e(hr);
		ReportExceptionErrorLV2(e);
		throw e;
	}

	//
	// ��ָ�����̵ı�
	// 
	//
	if(pImportStruct->ProjectID.IsEmpty())
	{
		SQL.Format(_T("SELECT * FROM %s ORDER BY %s"),pImportStruct->ProjectDBTableName,KeyField);
	}
	else
	{
		SQL.Format(_T("SELECT * FROM %s WHERE EnginID='%s' ORDER BY %s"),
				   pImportStruct->ProjectDBTableName,pImportStruct->ProjectID,KeyField);
	}

	try
	{
	//	IRecordset->CursorLocation=adUseClient;

		IRecordset->Open(_variant_t(SQL),
						 _variant_t((IDispatch*)this->GetProjectDbConnect()),
					     adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		Application.Quit();

		ReportExceptionErrorLV2(e);
		throw;
	}

	int iRow;	// ��ʼ������к�
	int Count;	// �ѵ���ļ�¼��
	long ID;
	CString strTemp,strSourceElementName;
	_variant_t varTemp;

	//��ʼ����
	this->GetProjectDbConnect()->BeginTrans();

	//
	// ����Զ���ID�򷵻�����ID����Ϊ��һ����¼��ID
	//
	if(IsAutoMakeID)
	{
		if(IRecordset->adoEOF && IRecordset->BOF)
		{
			ID=0;
		}
		else
		{
			IRecordset->MoveLast();

			varTemp=IRecordset->GetCollect(_variant_t(KeyField));

			ID = vtoi( varTemp );
		}

		ID++;
	}
	else
	{
		//
		for(int i=0;i<pImportStruct->ElementNum;i++)
		{
			pImportStruct->pElement[i].DestinationName.MakeUpper();

			if(pImportStruct->pElement[i].DestinationName==KeyField)
			{				
				break;
			}
		}

		if(i==pImportStruct->ElementNum)
		{
			CString strTemp;
			Application.Quit();

			this->GetProjectDbConnect()->RollbackTrans();

			strTemp.Format(_T("û���ҵ���%s��Ŷ�Ӧ��Excel��"),KeyField);

			ReportExceptionErrorLV1(_T("û���ҵ���ID��Ŷ�Ӧ��Excel��"));
			
			return;
		}
	
		//
		// �����Զ�����򣬻�ɾ��ԭ������ͬID�ŵļ�¼
		//
		for(iRow=pImportStruct->BeginRow,Count=0;Count<pImportStruct->RowCount;Count++,iRow++)
		{
			strSourceElementName.Format(_T("%s%d"),pImportStruct->pElement[i].SourceFieldName,iRow);

			pDispatch=worksheet.GetRange(_variant_t(strSourceElementName),vtMissing);
			range.AttachDispatch(pDispatch);

			varTemp=range.GetValue();

			range.ReleaseDispatch();
			
			if ( vtos(varTemp).IsEmpty() )
			{
				continue;	// �ؼ��ֵ�ֵΪ��
			}

			strTemp.Format(_T("%s=%s"),KeyField,(LPCTSTR)(_bstr_t)varTemp);

			try
			{
				IRecordset->Filter = _variant_t(strTemp);

				// �п����ж������˺�ļ�¼
				for (; !IRecordset->adoEOF; IRecordset->MoveNext())
				{
					IRecordset->Delete( adAffectCurrent );//
				}
			}
			catch(_com_error &e)
			{
				strSourceElementName+=_T(" ����д���");
				Exception::SetAdditiveInfo(strSourceElementName);
				ReportExceptionErrorLV2(e);
				GetProjectDbConnect()->RollbackTrans();
				throw;
			}
		}

	}

	//
	// ��ָ�����п�ʼ���������д�EXCEL�ļ������ݵ����ݿ�
	//
	for(iRow=pImportStruct->BeginRow,Count=0;Count<pImportStruct->RowCount;Count++,iRow++)
	{
		try
		{
			IRecordset->AddNew();

			//
			// �Զ���ID
			//
			if(IsAutoMakeID)
			{
				IRecordset->PutCollect(_variant_t(KeyField),_variant_t(ID));
				ID++;
			}

			if(!pImportStruct->ProjectID.IsEmpty())
			{
				IRecordset->PutCollect(_variant_t("EnginId"),_variant_t(pImportStruct->ProjectID));
			}
		}
		catch(_com_error &e)
		{
			Application.Quit();

			//�ع�
			this->GetProjectDbConnect()->RollbackTrans();

			ReportExceptionErrorLV2(e);
			throw;
		}

		//
		// ��һ�м�¼�д�EXCEL��Ԫ�е���Ӧ�Ĺ������ݿ���ֶ�
		//
		for(int i=0;i<pImportStruct->ElementNum;i++)
		{
			strSourceElementName.Format(_T("%s%d"),pImportStruct->pElement[i].SourceFieldName,iRow);

			try
			{
				pDispatch=worksheet.GetRange(_variant_t(strSourceElementName),vtMissing);
				range.AttachDispatch(pDispatch);

				varTemp=range.GetValue();

				range.ReleaseDispatch();
			}
			catch(COleDispatchException *e)
			{
				Application.Quit();
				this->GetProjectDbConnect()->RollbackTrans();
		
				strTemp.Format(_T("����Excel��%s��Ԫ��������ʱ����"),strSourceElementName);
				Exception::SetAdditiveInfo(strTemp);

				ReportExceptionErrorLV2(e);
				throw;
			}
			catch(COleException *e)
			{
				Application.Quit();
				this->GetProjectDbConnect()->RollbackTrans();

				_com_error e2(e->m_sc);
				e->Delete();
				ReportExceptionErrorLV2(e2);
				throw e2;
			}

			try
			{
				IRecordset->PutCollect(_variant_t(pImportStruct->pElement[i].DestinationName),varTemp);
			}
			catch(_com_error &e)
			{
				Application.Quit();

				this->GetProjectDbConnect()->RollbackTrans();

				//
				// ���쳣�������������Ϣ
				//
				strTemp.Format(_T("��%s%d�������д���"),
							   pImportStruct->pElement[i].SourceFieldName,
							   iRow);
				Exception::SetAdditiveInfo(strTemp);

				ReportExceptionErrorLV2(e);

				throw;
			}
		}
	}

	try
	{
		IRecordset->Update();
	}
	catch(_com_error &e)
	{
		Application.Quit();

		this->GetProjectDbConnect()->RollbackTrans();

		ReportExceptionErrorLV2(e);
		throw;
	}

	//�������и��Ĳ�������ǰ����
	this->GetProjectDbConnect()->CommitTrans();

	workbook.Close(_variant_t(false),vtMissing,vtMissing);
	workbooks.Close();
	Application.Quit();
}

/////////////////////////////////////////////////////////////////////////
//
// ��Excel��XLS�ļ��������ݵ�VOLUME��
//
// pImportStruct[in]	ImportFromXLSStruct�ṹ���ں��������Ϣ
//
void CProjectOperate::ImportA_DirFromXLS(ImportFromXLSStruct *pImportStruct)
{
	BOOL bRet;
	LPDISPATCH pDispatch;
	_Application Application;
	Workbooks workbooks;
	_Workbook workbook;
	Worksheets worksheets;
	_Worksheet worksheet;
	Range range;

	_RecordsetPtr IRecordset;
	HRESULT hr;
	CString SQL;
	CString strSourceJcmc;
	CString strSourceJcdm;
	ImportFromXLSElement *pElement;
	long VolumeID;	//
	_variant_t varTemp;


	if(pImportStruct==NULL || pImportStruct->pElement==NULL || 
	   pImportStruct->ElementNum<=0 || GetProjectDbConnect()==NULL)
	{
		_com_error e(E_INVALIDARG);

		ReportExceptionErrorLV2(e);
		throw e;
	}

	//���̺Ų���Ϊ��
	if(pImportStruct->ProjectID.IsEmpty())
	{
		_com_error e(E_FAIL);
		Exception::SetAdditiveInfo(_T("���̺Ų���Ϊ��"));

		ReportExceptionErrorLV2(e);

		throw e;
	}


	//
	// jcdm,jcmc�����ж�Ӧ��EXCEL��
	//
	pElement=((CImportFromXLSStruct*)pImportStruct)->FindInElement(NULL,_T("jcdm"));
	if(pElement==NULL)
	{
		ReportExceptionErrorLV1(_T("����Ҫ����jcdm(������)�ֶζ�Ӧ����"));
		return;
	}
	strSourceJcdm=pElement->SourceFieldName;

	pElement=((CImportFromXLSStruct*)pImportStruct)->FindInElement(NULL,_T("jcmc"));
	if(pElement==NULL)
	{
		ReportExceptionErrorLV1(_T("����Ҫ����jcmc(�������)�ֶζ�Ӧ����"));
		return;
	}
	strSourceJcmc=pElement->SourceFieldName;

	//
	// ����Excel����
	//
	bRet=Application.CreateDispatch(_T("Excel.Application"));

	if(!bRet)
	{
		ReportExceptionErrorLV1(_T("���Ȱ�װExcel!"));
		return;
	}

	//��ù������ļ���
	pDispatch=Application.GetWorkbooks();
	workbooks.AttachDispatch(pDispatch);

	try
	{
		//��ָ����EXCEL�ļ�
		pDispatch=workbooks.Open(pImportStruct->XLSFilePath,vtMissing,vtMissing,vtMissing,vtMissing,
								 vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,
								 vtMissing);
		workbook.AttachDispatch(pDispatch);

		//���ָ��������
		pDispatch=workbook.GetWorksheets();
		worksheets.AttachDispatch(pDispatch);

		//���ָ���Ĺ�����
		pDispatch=worksheets.GetItem(_variant_t(pImportStruct->XLSTableName));
		worksheet.AttachDispatch(pDispatch);
	}
	catch(COleDispatchException *e)
	{
		Application.Quit();

		CString str;
		str.Format(_T("��%s�ļ����%s���������"),pImportStruct->XLSFilePath,pImportStruct->XLSTableName);
		Exception::SetAdditiveInfo(_T(str));

		ReportExceptionErrorLV2(e);
		throw;
	}
	catch(COleException *e)
	{
		Application.Quit();

		_com_error e2(e->m_sc);
		e->Delete();
		ReportExceptionErrorLV2(e2);
		throw e2;
	}


	hr=IRecordset.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		Application.Quit();

		_com_error e(hr);
		ReportExceptionErrorLV2(e);
		throw e;
	}

	//
	// ��ָ�����̵ı�
	// 
	//
	SQL.Format(_T("SELECT * FROM %s ORDER BY VolumeID ASC"),pImportStruct->ProjectDBTableName);

	try
	{
		IRecordset->CursorLocation=adUseClient;

		IRecordset->Open(_variant_t(SQL),
						 _variant_t((IDispatch*)this->GetProjectDbConnect()),
					     adOpenDynamic,adLockOptimistic,adCmdText);

		//����޼�¼��1��ʼ
		if(IRecordset->adoEOF && IRecordset->BOF)
		{
			VolumeID=1;
		}
		else
		{
			//
			// �����һ����¼��ID
			//
			IRecordset->MoveLast();

			varTemp=IRecordset->GetCollect(_variant_t("VolumeID"));

			VolumeID=varTemp;

			VolumeID++;
		}
	}
	catch(_com_error &e)
	{
		Application.Quit();

		ReportExceptionErrorLV2(e);
		throw;
	}

	int iRow;	// ��ʼ������к�
	int Count;	// �ѵ���ļ�¼��
	CString strTemp,strSourceElementName;

	//��ʼ����
	this->GetProjectDbConnect()->BeginTrans();

	//
	// ��ָ�����п�ʼ���������д�EXCEL�ļ������ݵ����ݿ�
	//
	for(iRow=pImportStruct->BeginRow,Count=0;Count<pImportStruct->RowCount;Count++,iRow++)
	{
		strSourceElementName.Format(_T("%s%d"),strSourceJcdm,iRow);

		try
		{
			pDispatch=worksheet.GetRange(_variant_t(strSourceElementName),vtMissing);
			range.AttachDispatch(pDispatch);

			varTemp=range.GetValue();

			range.ReleaseDispatch();
		}
		catch(COleDispatchException *e)
		{
			Application.Quit();
			this->GetProjectDbConnect()->RollbackTrans();
	
			strTemp.Format(_T("����Excel��%s��Ԫ��������ʱ����"),strSourceElementName);
			Exception::SetAdditiveInfo(strTemp);

			ReportExceptionErrorLV2(e);
			throw;
		}
		catch(COleException *e)
		{
			Application.Quit();
			this->GetProjectDbConnect()->RollbackTrans();

			_com_error e2(e->m_sc);
			e->Delete();
			ReportExceptionErrorLV2(e2);
			throw e2;
		}


		if(varTemp.vt==VT_NULL || varTemp.vt==VT_EMPTY)
		{
			Application.Quit();
			this->GetProjectDbConnect()->RollbackTrans();

			_com_error e(E_FAIL);

			strTemp.Format(_T("%s��Ԫ����Ϊ��"),strSourceElementName);
			Exception::SetAdditiveInfo(strTemp);

			ReportExceptionErrorLV2(e);
			throw e;
		}

		SQL.Format(_T("jcdm='%s' AND EnginID='%s'"),(LPCTSTR)(_bstr_t)varTemp,pImportStruct->ProjectID);


		try
		{
			//
			// �����ͬ���ľ���jcdm���������ڵ������滻ԭ�������ݲ�����VolumeID
			// ���û������������.
			//
			IRecordset->Filter=_variant_t(SQL);

			if(IRecordset->adoEOF && IRecordset->BOF)
			{
				IRecordset->AddNew();

				IRecordset->PutCollect(_variant_t("VolumeID"),_variant_t(VolumeID));

				VolumeID++;
			}
			else
			{
				varTemp=IRecordset->GetCollect(_variant_t("VolumeID"));

				IRecordset->Delete(adAffectCurrent);
				IRecordset->MoveNext();

				IRecordset->AddNew();

				IRecordset->PutCollect(_variant_t("VolumeID"),varTemp);
			}

			if(!pImportStruct->ProjectID.IsEmpty())
			{
				IRecordset->PutCollect(_variant_t("EnginId"),_variant_t(pImportStruct->ProjectID));
			}

			//
			// ����sjhyid,sjjdid,zyidĬ�ϵ�ֵΪ0��4��3
			//
			IRecordset->PutCollect(_variant_t("SJHYID"),_variant_t((long)0));
			IRecordset->PutCollect(_variant_t("SJJDID"),_variant_t((long)4));
			IRecordset->PutCollect(_variant_t("ZYID"),_variant_t((long)3));
		}
		catch(_com_error &e)
		{
			Application.Quit();

			//�ع�
			this->GetProjectDbConnect()->RollbackTrans();

			ReportExceptionErrorLV2(e);
			throw;
		}

		//
		// ��һ�м�¼�д�EXCEL��Ԫ�е���Ӧ�Ĺ������ݿ���ֶ�
		//
		for(int i=0;i<pImportStruct->ElementNum;i++)
		{
			strSourceElementName.Format(_T("%s%d"),pImportStruct->pElement[i].SourceFieldName,iRow);

			try
			{
				pDispatch=worksheet.GetRange(_variant_t(strSourceElementName),vtMissing);
				range.AttachDispatch(pDispatch);

				varTemp=range.GetValue();

				range.ReleaseDispatch();
			}
			catch(COleDispatchException *e)
			{
				Application.Quit();
				this->GetProjectDbConnect()->RollbackTrans();
		
				strTemp.Format(_T("����Excel��%s��Ԫ��������ʱ����"),strSourceElementName);
				Exception::SetAdditiveInfo(strTemp);

				ReportExceptionErrorLV2(e);
				throw;
			}
			catch(COleException *e)
			{
				Application.Quit();
				this->GetProjectDbConnect()->RollbackTrans();

				_com_error e2(e->m_sc);
				e->Delete();
				ReportExceptionErrorLV2(e2);
				throw e2;
			}

			try
			{
				IRecordset->PutCollect(_variant_t(pImportStruct->pElement[i].DestinationName),varTemp);
			}
			catch(_com_error &e)
			{
				Application.Quit();

				this->GetProjectDbConnect()->RollbackTrans();


				//
				// ���쳣�������������Ϣ
				//
				strTemp.Format(_T("��%s%d�������д���"),
							   pImportStruct->pElement[i].SourceFieldName,
							   iRow);
				Exception::SetAdditiveInfo(strTemp);

				ReportExceptionErrorLV2(e);
				throw;
			}
		}
	}

	try
	{
		IRecordset->Update();
	}
	catch(_com_error &e)
	{
		Application.Quit();

		this->GetProjectDbConnect()->RollbackTrans();

		ReportExceptionErrorLV2(e);
		return;
	}

	//�������и��Ĳ�������ǰ����
	this->GetProjectDbConnect()->CommitTrans();

	workbook.Close(_variant_t(false),vtMissing,vtMissing);
	workbooks.Close();
	Application.Quit();
}



bool CProjectOperate::ImportTblEnginIDXLS(ImportFromXLSStruct *pImportStruct, BOOL IsAutoMakeID, LPCTSTR KeyField, CString EnginID)
{
	BOOL bRet;
	LPDISPATCH pDispatch;
	_Application Application;
	Workbooks workbooks;
	_Workbook workbook;
	Worksheets worksheets;
	_Worksheet worksheet;
	Range		range;
	CString strTemp;

//	CMObject  ExcelApp;

	_RecordsetPtr IRecordset, pRs;
	HRESULT hr;
	CString SQL;
	LIST_ID_ENGIN listID[10];  //��ͬ���̶�Ӧ�����ID��
	
	if(pImportStruct==NULL || pImportStruct->pElement==NULL || 
	   pImportStruct->ElementNum<=0 || GetProjectDbConnect()==NULL)
	{
		_com_error e(E_INVALIDARG);		
		ReportExceptionErrorLV2(e);
		return FALSE;
	}
	try
	{
/*		if( FAILED(ExcelApp.GetActiveObject(_T("Excel.Application"))) )  //zsy
		{
			if( FAILED(ExcelApp.CreateObject(_T("Excel.Application"))) )
			{
				AfxMessageBox("��������Excel��û�а�װExcel !");
				return false;
			}
		}
		Application.AttachDispatch( ExcelApp );
*/

		//��������EXCEL 
		bRet=Application.CreateDispatch(_T("Excel.Application"));		
		if(!bRet)
		{
			ReportExceptionErrorLV1(_T("��������Excel, ��ȷ���Ƿ�װExcel !"));
			return false;
		}
		
		//��ù������ļ���
		pDispatch=Application.GetWorkbooks();
		workbooks.AttachDispatch(pDispatch);
	}
	catch (COleDispatchException * e)
	{
		e->Delete();
		return FALSE;
	}

	try
	{
		//��ָ����EXCEL�ļ�
		pDispatch=workbooks.Open(pImportStruct->XLSFilePath,vtMissing,vtMissing,vtMissing,vtMissing,
								 vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,
								 vtMissing);
		workbook.AttachDispatch(pDispatch);
		//���ָ��������
		pDispatch=workbook.GetWorksheets();
		worksheets.AttachDispatch(pDispatch);
		//���ָ���Ĺ�����
		pDispatch=worksheets.GetItem(_variant_t(pImportStruct->XLSTableName));
		worksheet.AttachDispatch(pDispatch);		
	}
	catch(COleDispatchException *e)
	{
		Application.Quit();
		strTemp.Format(_T("��%s�ļ����%s���������"),pImportStruct->XLSFilePath,pImportStruct->XLSTableName);
		Exception::SetAdditiveInfo( strTemp );
		ReportExceptionErrorLV1(e);
		return FALSE;
	}
	catch(COleException *e)
	{
		Application.Quit();
		_com_error e2(e->m_sc);
		e->Delete();
		ReportExceptionErrorLV2(e2);
		return FALSE ;
	}
	hr=IRecordset.CreateInstance(__uuidof(Recordset));
	pRs.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		Application.Quit();
		_com_error e(hr);
		ReportExceptionErrorLV2(e);
		return FALSE;
	}
	//
	// ��ָ�����̵ı�
	// 
	if(pImportStruct->ProjectID.IsEmpty())
	{
		SQL.Format(_T("SELECT * FROM %s ORDER BY %s"),pImportStruct->ProjectDBTableName,KeyField);
	}
	else
	{
		SQL.Format(_T("SELECT * FROM %s WHERE EnginID='%s' ORDER BY %s"),
				   pImportStruct->ProjectDBTableName,pImportStruct->ProjectID,KeyField);
	}
	try
	{
		IRecordset->Open(_variant_t(SQL),
						 _variant_t((IDispatch*)this->GetProjectDbConnect()),
					     adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		Application.Quit();
		ReportExceptionErrorLV2(e);
		return FALSE;
	}

	int iRow;	// ��ʼ������к�
	int Count;	// �ѵ���ļ�¼��
	long ID;
	CString strSourceElementName;
	_variant_t varTemp;
	int ProjNum = 1;
	int ProjIndex = 0;
	int Rj=0;			// ���̺���Excel�е���
	for(; Rj < pImportStruct->ElementNum; Rj++)
	{
		pImportStruct->pElement[Rj].DestinationName.MakeUpper();
		if( !EnginID.CompareNoCase(pImportStruct->pElement[Rj].DestinationName) )
			break;
	}
	//��ʼ����
	this->GetProjectDbConnect()->BeginTrans();

	if(IsAutoMakeID)
	{
		if(IRecordset->adoEOF && IRecordset->BOF)
			ID=0;
		else
		{
			IRecordset->MoveLast();
			varTemp=IRecordset->GetCollect(_variant_t(KeyField));
			ID = vtoi( varTemp );
		}
		ID++;
	}
	else
	{
		ID = -1;	// ��������Զ����ʱ,ʹ���ļ��е����
		for(int i = 0; i < pImportStruct->ElementNum; i++)
		{
			pImportStruct->pElement[i].DestinationName.MakeUpper();
			if ( pImportStruct->pElement[i].DestinationName == KeyField )
				break;
		}
		if ( i == pImportStruct->ElementNum )
		{
			Application.Quit();
			this->GetProjectDbConnect()->RollbackTrans();
			strTemp.Format(_T("û���ҵ���%s��Ŷ�Ӧ��Excel��"),KeyField);
			ReportExceptionErrorLV1( strTemp );			
			return FALSE;
		}
		CString strNum;		//���
		CString strEng;		//���̺�
		// �����Զ�����򣬻�ɾ��ԭ������ͬID�ŵļ�¼(��ǰ���̺ŵļ�¼)
		for(iRow=pImportStruct->BeginRow, Count=0; Count<pImportStruct->RowCount;Count++,iRow++)
		{
			if ( Rj < pImportStruct->ElementNum )
			{
				strSourceElementName.Format(_T("%s%d"), pImportStruct->pElement[Rj].SourceFieldName,iRow);
				pDispatch = worksheet.GetRange(_variant_t( strSourceElementName ), vtMissing);
				range.AttachDispatch( pDispatch );
				varTemp = range.GetValue();
				strEng = vtos( varTemp );
				range.ReleaseDispatch();
				if ( !strEng.IsEmpty() && strEng.CompareNoCase( pImportStruct->ProjectID ) )
					continue;
			}
			strSourceElementName.Format(_T("%s%d"), pImportStruct->pElement[i].SourceFieldName,iRow);
			pDispatch = worksheet.GetRange(_variant_t(strSourceElementName),vtMissing);
			range.AttachDispatch( pDispatch );
			varTemp = range.GetValue();
			strNum = vtos( varTemp );
			range.ReleaseDispatch();
			if ( strNum.IsEmpty() )
				continue;	// �ؼ��ֵ�ֵΪ��

			strTemp.Format(_T("%s=%s"), KeyField, strNum );
			try
			{
				IRecordset->Filter = _variant_t(strTemp);
				// �п����ж������˺�ļ�¼
				for (; !IRecordset->adoEOF; IRecordset->MoveNext())
				{
					IRecordset->Delete( adAffectCurrent );//
				}
			}
			catch(_com_error &e)
			{
				strSourceElementName+=_T(" ����д���");
				Exception::SetAdditiveInfo(strSourceElementName);
				ReportExceptionErrorLV1(e);
				GetProjectDbConnect()->RollbackTrans();
				return FALSE;
			}
		}
	}
	//��1λ���õ�ǰ����
	listID[0].ID = ID;
	listID[0].EnginID = pImportStruct->ProjectID;
	//
	// ��ָ�����п�ʼ���������д�EXCEL�ļ������ݵ����ݿ�
	//
	for(iRow=pImportStruct->BeginRow,Count=0;Count<pImportStruct->RowCount;Count++,iRow++)
	{
		try
		{
			IRecordset->AddNew();
		}
		catch(_com_error &e)
		{
			Application.Quit();

			//�ع�
			this->GetProjectDbConnect()->RollbackTrans();

			ReportExceptionErrorLV2(e);
			return FALSE;
		}

		//
		// ��һ�м�¼�д�EXCEL��Ԫ�е���Ӧ�Ĺ������ݿ���ֶ�
		//
		for(int i=0;i<pImportStruct->ElementNum;i++)
		{
			strSourceElementName.Format(_T("%s%d"),pImportStruct->pElement[i].SourceFieldName,iRow);

			try
			{
				pDispatch=worksheet.GetRange(_variant_t(strSourceElementName),vtMissing);
				range.AttachDispatch(pDispatch);

				varTemp=range.GetValue();

				range.ReleaseDispatch();
			}
			catch(COleDispatchException *e)
			{
				Application.Quit();
				this->GetProjectDbConnect()->RollbackTrans();
		
				strTemp.Format(_T("����Excel��%s��Ԫ��������ʱ����"),strSourceElementName);
				Exception::SetAdditiveInfo(strTemp);

				ReportExceptionErrorLV1(e);
				return FALSE;
			}
			catch(COleException *e)
			{
				Application.Quit();
				this->GetProjectDbConnect()->RollbackTrans();

				_com_error e2(e->m_sc);
				e->Delete();
				ReportExceptionErrorLV2(e2);
				return FALSE;
			}

			try
			{
				IRecordset->PutCollect(_variant_t(pImportStruct->pElement[i].DestinationName),varTemp);

				//{{2��ͬ�Ĺ������ƣ��Ӳ�ͬ��ID��
				if( i == Rj /*���������ڵ���*/)
				{
					strTemp = vtos(varTemp);
					for(int c=0; c<ProjNum; c++)
					{
						if( !strTemp.CompareNoCase(listID[c].EnginID) || (c==0 && strTemp.IsEmpty()))
						{
							ProjIndex = c;
							break;
						}
					}
					if( c == ProjNum )
					{
						listID[c].EnginID = strTemp;
						SQL = "SELECT * FROM ["+pImportStruct->ProjectDBTableName+"]  \
							WHERE EnginID='"+listID[c].EnginID+"' ORDER BY "+KeyField+" ";
						if(pRs->State == adStateOpen)
						{
							pRs->Close();
						}

						pRs->Open(_bstr_t(SQL), (IDispatch*)m_ProjectDbConnect, 
								adOpenStatic, adLockOptimistic, adCmdText);
						if( pRs->adoEOF && pRs->BOF )
						{
							ID = 0;
						}
						else
						{
							pRs->MoveLast();
							varTemp = pRs->GetCollect(_bstr_t(KeyField));
							ID = vtoi(varTemp);
						}
						//�������,����¼�ӵ�ĩβ.
						listID[c].ID = ++ID;
						ProjIndex = c;
						ProjNum++;
					}
				}
				//2}}
			}
			catch(_com_error &e)
			{
				Application.Quit();

				this->GetProjectDbConnect()->RollbackTrans();

				//
				// ���쳣�������������Ϣ
				//
				strTemp.Format(_T("��%s%d�������д���"),
							   pImportStruct->pElement[i].SourceFieldName,
							   iRow);
				Exception::SetAdditiveInfo(strTemp);

				ReportExceptionErrorLV1(e);

				return FALSE;
			}
		}
		try
		{
			if ( listID[ProjIndex].ID > 0 )
			{   // ��Ϊ�Զ�ͬʱ��Ŵ�����ʱ,���б��
				strTemp.Format("%d",listID[ProjIndex].ID);
				IRecordset->PutCollect(_variant_t(KeyField),_variant_t(strTemp));
				listID[ProjIndex].ID++;
			}
			IRecordset->PutCollect(_variant_t("EnginId"),_variant_t(listID[ProjIndex].EnginID));
		}catch(_com_error& e)
		{
			AfxMessageBox(e.Description());
			return false;
		}
	}

	try
	{
		IRecordset->Update();
	}
	catch(_com_error &e)
	{
		Application.Quit();

		this->GetProjectDbConnect()->RollbackTrans();

		ReportExceptionErrorLV1(e);
		return FALSE;
	}

	//�������и��Ĳ�������ǰ����
	this->GetProjectDbConnect()->CommitTrans();


	workbook.Close(_variant_t(false),vtMissing,vtMissing);
	workbooks.Close();	
	Application.Quit();
	Application.ReleaseDispatch();

	return true;
}
//����AutoPHS��֧������������.
bool CProjectOperate::ImportAutoPHSPaint(CString strFilePath)
{
	CString strSQL, strValue;
	try
	{
		if( !ConnectExcelFile( strFilePath ) )
		{
			CString strMessage;
			strMessage = "���������ļ�,��ȷ�� ' "+strFilePath+" ' ���ļ��Ƿ����!";
			AfxMessageBox( strMessage );
			return false;
		}
		CRecordset pRs;
		pRs.m_pDatabase = &m_db;//SUM(CLzz) as CLzzs
	
		strSQL.Format("SELECT SUM(CLzz) as CLzzs FROM [�ܲ����ܱ�] ");
	//	m_db.ExecuteSQL(strSQL);
		pRs.Open(AFX_DB_USE_DEFAULT_TYPE, strSQL, CRecordset::readOnly);
		if( !pRs.IsBOF() )
		{
			pRs.GetFieldValue((short)0, strValue );
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
	}
	return true;
	

}

//����Excel�ļ�.
bool CProjectOperate::ConnectExcelFile(CString strFileName)
{
	//�½��ļ�	
	CString strDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; // Excel��װ����	
	CString strCon, strSQL;
	
	try
	{
		// �������д�ȡ���ַ���
		strCon.Format("DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",
			strDriver, strFileName, strFileName);
		
		// �������ݿ� (��Excel����ļ�)
		if( !m_db.OpenEx(strCon,CDatabase::noOdbcDialog) )
		{
			return false;
		}
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	return TRUE;

}
//////////////////////////////////
//����:
//����һ��ָ�������ݿ�
bool CProjectOperate::ConnectDB(_ConnectionPtr& pCon,CString& strManageTbl,CString& strExcelTbl,CString& strDataTbl,int& nStart,int& nRowNum, CString strDBFile,int nIndex)
{
	_variant_t var;
	CString strSQL, strTMP;
	HRESULT hr;
	_RecordsetPtr pRs;

	hr = pCon.CreateInstance(__uuidof(Connection));
	if( FAILED(hr) )
	{
		_com_error e(hr);
		ReportExceptionErrorLV1( e );
		return false;
	}
	strSQL = CONNECTSTRING + strDBFile;
	try
	{   //�������ݿ�
		pCon->Open(_bstr_t(strSQL),"","",-1);

		hr = pRs.CreateInstance(__uuidof(Recordset));
		if( FAILED(hr) )
		{
			_com_error e(hr);
			ReportExceptionErrorLV1(e);				
			return false;
		}
		//�򿪹����.��ÿ��Ʊ���.
		strTMP.Format("%d",nIndex);
		strSQL = "SELECT * FROM [TableExcelToAccess] WHERE ID="+strTMP+" ";
		pRs->Open(_bstr_t(strSQL), pCon.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if( pRs->GetRecordCount() <= 0 )
		{
			return false;
		}
		//�����,�����ֶεĶ�Ӧ��ϵ.
		var = pRs->GetCollect("TableName");
		strManageTbl = vtos(var);
		//Excel����
		var = pRs->GetCollect("ExcelTblName");
		strExcelTbl = vtos(var);
		//Accessԭʼ���ݱ�
		var = pRs->GetCollect("AccessTblName");
		strDataTbl = vtos(var);

		//Excel�п�ʼ������к�
		var = pRs->GetCollect("StartRow");
		nStart = vtoi(var);
		//�ܹ��������ݵ�����
		var = pRs->GetCollect("RecordNum");
		nRowNum = vtoi(var);
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}	
	return true;
}

////////////////////////////////
//����:
//ʹ��һ���������ݿ�,����excel���ݵ�AutoIPED.
//(ͨ�õ�һ������.)
bool CProjectOperate::ImportOriginalData(CString strDataPath/*���ݿ�·��*/, CString strManageDB, int nIndex)
{
	_ConnectionPtr pCon;
	_RecordsetPtr pRs, pRsOrig;
	HRESULT hr;
	_variant_t var;
	CString strSQL,			//SQL���
			strTMP,			//��ʱ����.
			strManageTbl,   //�����,�����ֶεĶ�Ӧ��ϵ.
			strExcelTbl,	//Excel����
			strDataTbl		//Accessԭʼ���ݱ�.
			;
	int		nStart,   //Excel�п�ʼ������к�(��1��ʼ)
			nRowNum;  //�ܹ��������ݵ�����.
	try
	{
		//�������ݿ�,����ù�����е�ֵ.
		if( !ConnectDB(pCon,strManageTbl,strExcelTbl,strDataTbl,nStart,nRowNum,strDataPath,nIndex) )
		{
			return false;
		}
		CProjectOperate::ImportFromXLSElement  element[30]; //��Ӧ
		int elementNum = 0 ;//���������.

		GetFieldAsCol(pCon,element,elementNum, strManageTbl);
		CProjectOperate::ImportFromXLSStruct   structPrj;
		structPrj.pElement = element;

		hr = pRs.CreateInstance(__uuidof(Recordset));
		if( FAILED(hr) )
		{
			_com_error e(hr);
			AfxMessageBox(e.Description());
			return false;
		}
		strSQL = "SELECT * FROM ["+strManageTbl+"] ";
		pRs->Open(_bstr_t(strSQL), pCon.GetInterfacePtr(), 
			adOpenStatic, adLockOptimistic, adCmdText);

	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}

	return true;
}
//���Access���ݿ��е��ֶ���Excel���е��еĶ�Ӧ��ϵ.
//
bool CProjectOperate::GetFieldAsCol(_ConnectionPtr& pCon,CProjectOperate::ImportFromXLSElement element[],int& elementNum, CString& strTblName)
{
	_RecordsetPtr pRs;
	HRESULT hr;
	CString strSQL;
	_variant_t var;

	hr = pRs.CreateInstance(__uuidof(Recordset));
	if( FAILED(hr) )
	{
		_com_error e(hr);
		AfxMessageBox(e.Description());
		return false;
	}
	strSQL = "SELECT * FROM ["+strTblName+"] WHERE ExcelColNO IS NOT NULL ORDER BY SEQ";
	try
	{
		pRs->Open(_bstr_t(strSQL), pCon.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		for(elementNum=0; !pRs->adoEOF; pRs->MoveNext(), elementNum++ )
		{
			var = pRs->GetCollect("FileName");		//���ݿ��е��ֶ���
			element[elementNum].DestinationName = vtos(var);
			var = pRs->GetCollect("ExcelColNO");	//��Excel��Ӧ����
			element[elementNum].SourceFieldName = vtos(var);
			var = pRs->GetCollect("ExcelColNum");	//Excel�ж�Ӧһ���ֶε�����.
			element[elementNum].ExcelColNum = vtoi(var);
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}

	return true;
}
