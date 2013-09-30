// ProjectOperate.h: interface for the CProjectOperate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTOPERATE_H__C62D5FD8_0560_4AAB_8D04_051D6425E8AC__INCLUDED_)
#define AFX_PROJECTOPERATE_H__C62D5FD8_0560_4AAB_8D04_051D6425E8AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////
//
// �Թ��̵Ĳ�����
//
////////////////////////////////////////////////

class CProjectOperate  
{
public:
	CProjectOperate();
	virtual ~CProjectOperate();

	// ��EXCEL�ļ��������ݵ��ӽṹ
	struct ImportFromXLSElement		
	{
		CString SourceFieldName;	// Excel����ʼ�к�
		int		ExcelColNum;		// Excel������.
		CString DestinationName;	// ��Ӧ��Ŀ�����ݿ���ֶ���
	};
	typedef struct
	{
		int ID;
		CString EnginID;
	}LIST_ID_ENGIN;

	// ����EXCEL�ļ��������ݵĽṹ
	struct ImportFromXLSStruct
	{
		CString XLSFilePath;		// EXCEL�ļ���·��
		CString	XLSTableName;		// EXCEL�Ĺ�������
		int		BeginRow;			// ��ʼ������к�
		int		RowCount;			// ��Ҫ�ĵ���ļ�¼��

		CString ProjectDBTableName;	// ���뵽��Ŀ�Ĺ��̱���
		CString ProjectID;			// ���뵽��Ŀ�Ĺ�����

		ImportFromXLSElement *pElement;	// ָ���ӵ�Ԫ�Ľṹ
		int ElementNum;	//�ӵ�Ԫ�Ľṹ��Ŀ
	};

	class CImportFromXLSStruct:public ImportFromXLSStruct
	{
	public:
		ImportFromXLSElement* FindInElement(LPCTSTR szSource,LPCTSTR szDest);

	};

public:
	_ConnectionPtr GetProjectDbConnect();
	void SetProjectDbConnect(_ConnectionPtr IConnection);

	CString GetProjectID();
	void SetProjectID(LPCTSTR strProjectID);

public:
	bool ImportOriginalData(CString strDataPath, CString strManageDB, int nIndex);
	bool ConnectExcelFile(CString strFileName);
	bool ImportAutoPHSPaint(CString strFilePath);
	bool ImportTblEnginIDXLS(ImportFromXLSStruct *pImportStruct,BOOL IsAutoMakeID,LPCTSTR KeyField=_T("ID"), CString EnginID=_T("EnginID"));
	void ImportA_DirFromXLS(ImportFromXLSStruct *pImportStruct);
	void ImportPre_CalcFromXLS(ImportFromXLSStruct *pImportStruct,BOOL IsAutoMakeID,LPCTSTR KeyField=_T("ID"));
	void ImportFromXLS(ImportFromXLSStruct *pImportStruct);


private:
	bool m_bAuto;			//�Զ������ʾ
	CString m_strProjectID;				// ���̵ı��
	_ConnectionPtr m_ProjectDbConnect;	// ������ص����ݿ�

protected:
	bool GetFieldAsCol(_ConnectionPtr& pCon,ImportFromXLSElement element[],int& elementNum,CString& strTblName);
	bool ConnectDB(_ConnectionPtr& pCon,CString& strManageTbl,CString& strExcelTbl,CString& strDataTbl,int& nStart,int& nRowNum, CString strDBFile,int nIndex);
	CDatabase m_db;
};

#endif // !defined(AFX_PROJECTOPERATE_H__C62D5FD8_0560_4AAB_8D04_051D6425E8AC__INCLUDED_)
