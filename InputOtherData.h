// InputOtherData.h: interface for the InputOtherData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTOTHERDATA_H__B79B579A_6C80_41B6_8B3B_D17935461C7C__INCLUDED_)
#define AFX_INPUTOTHERDATA_H__B79B579A_6C80_41B6_8B3B_D17935461C7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "odbcinst.h"

class InputOtherData  
{
public:
	bool MeetRequirement(CString AllPath);
	bool Have_Table(_ConnectionPtr m_pCon, CString str);
	struct CONNFEATURE {
		CString strCONN;
		CString strEnginTable;
		CString strEnginIDFieldName;
		CString strEnginName;
		CString strFILENAME;
		CString strFilter;
		CString strExtension;
	} strConnFeature[5];
	int cancel;
	CString *Pname;//��������
	CString *ProID;//���̴���
	int count;
	bool ImportTable_a_dir(_ConnectionPtr pcon_source,CString strProID);
	bool ImportTable_abnormal(_ConnectionPtr pcon_source,CString strTableName, CString strProID);
	CString strFileName;   //����Ĺ���·������DATA�ļ�����
	CString strPathName;  //����Ĺ���·�����������̴���
	CString strPathProID;  //����Ĺ��̴��ţ���·��
	bool InputData(CString strBWPath,CString strPrjID);    //��������
	bool SelectPro();    //ѡ��Ҫ����Ĺ���
	void SelectFile(int);   //ѡ��Ҫ����Ĺ���·��
	InputOtherData();
	virtual ~InputOtherData();

private:
	long GetMaxValue(CString SQL,_ConnectionPtr pConn,CString strFieldName);
	bool PutFieldValue(_RecordsetPtr pSource,_RecordsetPtr pTarget,CString strFieldName);
	int iCONN;

	//����Volume��throw(_com_error);
	void UpdateVolumeTable(LPCTSTR szProjectName);
protected:
	bool GetImportTblNames(CString* &pTableName,int &nTblCount,CString pNotTbl[],int nNotCount=0);
	enum nIPEDFilterIndex{
	nDBFileterIndex_CUSTOM=0,//�û��Զ��������
	nDBFileterIndex_UESOFT=1,
	nDBFileterIndex_VFP_SW=2,
	nDBFileterIndex_DB3_JS=3
	};
};

#endif // !defined(AFX_INPUTOTHERDATA_H__B79B579A_6C80_41B6_8B3B_D17935461C7C__INCLUDED_)
