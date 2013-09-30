// EDIBgbl.cpp: implementation of the EDIBgbl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EDIBgbl.h"
#include "AutoIPED.h"
#include "vtot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CAutoIPEDApp theApp;

CString EDIBgbl::SelPrjID = "";  //���̴���
CString EDIBgbl::SelPrjName = "";//��������
CString EDIBgbl::SelVlmCODE = "";//������

long EDIBgbl::SelSpecID = 3;  //�Ȼ�
long EDIBgbl::SelDsgnID = 4;  //ʩ��ͼ
long EDIBgbl::SelHyID =0;	  //����
long EDIBgbl::SelVlmID = 0;
const double EDIBgbl::kgf2N= 9.80665;

CString EDIBgbl::sMaterialPath;
CString EDIBgbl::sCur_MaterialCodeName;
CString EDIBgbl::sCritPath;
CString EDIBgbl::sCur_CritDbName;
CString EDIBgbl::sCur_CodeName;
long EDIBgbl::iCur_CodeKey;
CString EDIBgbl::sCur_CodeNO;

//CString EDIBgbl::strCur_MediumDBPath;	//�������ݿ��·��

_RecordsetPtr EDIBgbl::pRsCalc;		//ָ��ԭʼ���ݱ�[pre_calc]��ָ�롣

EDIBgbl::EDIBgbl()
{

}

EDIBgbl::~EDIBgbl()
{

}

bool EDIBgbl::GetCurDBName()
{
	CString strTmpPath = gsIPEDInsDir + "IPED.ini";
	if( !FileExists( strTmpPath ) )
	{
		//��Ϣ�ļ������ڣ�����Ĭ�ϵ����ݿ���
		WritePrivateProfileString("specification", "lastCriterion","IPEDcode.mdb", strTmpPath);
		WritePrivateProfileString("specification", "lastCriterionName","������ҵ", strTmpPath);//��ҵ
		WritePrivateProfileString("specification", "lastMaterial","Material.mdb", strTmpPath);
		WritePrivateProfileString("specification", "lastCodeNO", "DL/T5054-1996", strTmpPath ); // �淶����
		WritePrivateProfileString("specification", "lastCodeKey","", strTmpPath ); // �淶��־��
	}
	char cszRet[0x100];
	memset(cszRet, '\0', sizeof(char)*256);  // ����������
	//���ǰһ��ѡ��Ĺ淶�淶���ݿ�����
	GetPrivateProfileString("specification","lastCriterion","***.!!!",cszRet,256,strTmpPath);
	EDIBgbl::sCur_CritDbName = cszRet;

	memset(cszRet, '\0', sizeof(char)*256);
	GetPrivateProfileString("specification","lastCriterionName","",cszRet,256,strTmpPath);
	EDIBgbl::sCur_CodeName = cszRet;

	memset(cszRet, '\0', sizeof(char)*256);
	//���ǰһ��ѡ��Ĳ������ݿ�����
	GetPrivateProfileString( "specification","MaterialCodeName","***.!!!",cszRet,256,strTmpPath );
	EDIBgbl::sCur_MaterialCodeName = cszRet;

	memset( cszRet, '\0', sizeof(char)*256 );
	GetPrivateProfileString( "specification", "lastCodeNO", "", cszRet, 256, strTmpPath );
	EDIBgbl::sCur_CodeNO = cszRet;

	EDIBgbl::iCur_CodeKey =GetPrivateProfileInt("specification", "lastCodeKey", nKey_CODE_DL5072_1997,  strTmpPath );
	
	//��ȡʧ�ܣ�����ȱʡֵ
	if( EDIBgbl::sCur_CritDbName=="" || EDIBgbl::sCur_CritDbName=="***.!!!" )
	{
		EDIBgbl::sCur_CritDbName = "IPEDcode.mdb";
	}
	if( EDIBgbl::sCur_MaterialCodeName=="" || EDIBgbl::sCur_MaterialCodeName=="***.!!!" )
	{
		EDIBgbl::sCur_MaterialCodeName = "ALL CODE";
	}

	return true;
}
bool EDIBgbl::SetCurDBName()
{
	CString strTmpPath;
	strTmpPath = gsIPEDInsDir + "IPED.ini";
	//����ѡ��ı�׼������
	if( EDIBgbl::sCur_CritDbName != "" )
	{
		// �淶���ݿ�����
		WritePrivateProfileString("specification", "lastCriterion",	EDIBgbl::sCur_CritDbName,strTmpPath);
		
		// ������ҵ����
		WritePrivateProfileString("specification", "lastCriterionName",	EDIBgbl::sCur_CodeName, strTmpPath);

		// �淶���� 
		WritePrivateProfileString( _T("specification"), _T("lastCodeNO"), EDIBgbl::sCur_CodeNO, strTmpPath );

		// �淶��־��
		CString strTmp;
		strTmp.Format("%d",EDIBgbl::iCur_CodeKey);
		WritePrivateProfileString("specification", "lastCodeKey",strTmp, strTmpPath ); 
	}
	else
	{

		AfxMessageBox("�����ļ����ƻ�,�����°�װ AutoIPED8.0 !");
		return false;
	}
	//����ѡ��Ĳ��Ͽ�����
	if( EDIBgbl::sCur_MaterialCodeName != "" )
	{
		WritePrivateProfileString("specification", "MaterialCodeName",
			EDIBgbl::sCur_MaterialCodeName, strTmpPath);
	}
	else 
	{
		AfxMessageBox("�����ļ����ƻ�,�����°�װ AutoIPED8.0 !");
		return false;
	}
	return true;
}
//
//�����ݿ�pCon��,��ָ���ı���в���.��Ĭ�ϵļ�¼���뵽ָ���Ĺ�����
bool EDIBgbl::InsertDefTOEng(_ConnectionPtr &pCon, CString strTblName, CString strPrjID, CString strDefKey)
{
	CString			strSQL;
	CString strTmpTblName;
	strTmpTblName = "ppp";		//��ʱ����
	try
	{
		try
		{	//ɾ����ʱ��
			pCon->Execute(_bstr_t("drop table "+strTmpTblName+" "), NULL, adCmdText);
		}
		catch(_com_error& e)
		{
			if( e.Error() != DB_E_NOTABLE )
			{
				ReportExceptionError(e);
				return false;
			}
		}
		//��Ĭ�ϵļ�¼�Ȳ��뵽��ʱ����.WHERE EnginID='"+strDefKey+"'
		strSQL = "SELECT * INTO ["+strTmpTblName+"] FROM ["+strTblName+"] ";
		if(	strDefKey.IsEmpty() )
		{
			strSQL += "	WHERE EnginID IS NULL ";
		}
		else
		{
			strSQL += " WHERE EnginID='"+strDefKey+"' ";
		}
		pCon->Execute(_bstr_t(strSQL), NULL, adCmdText);

		//�ı乤�̺�.
		strSQL = "UPDATE ["+strTmpTblName+"] SET EnginID='"+strPrjID+"' ";
		pCon->Execute(_bstr_t(strSQL), NULL, adCmdText);

		//������뵽����,��Ӧ��ָ���Ĺ���.
		strSQL = "INSERT INTO ["+strTblName+"] SELECT * FROM ["+strTmpTblName+"] WHERE EnginID='"+strPrjID+"' ";
		pCon->Execute(_bstr_t(strSQL), NULL, adCmdText);

		//ɾ����ʱ��.
		pCon->Execute(_bstr_t("drop table "+strTmpTblName+" "), NULL, adCmdText);

	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return false;
	}
	return true;
}

//���ܣ�
//�½�һ����ʱ�ģ�������㹫ʽ�ı�
//����Ѿ���������ɾ����
//�ñ���ڹ�����Ŀ���ݿ��С�(workprj.mdb)
int EDIBgbl::NewCalcAlfaTable()
{
	CString sSQL;
	CString strTblName = "tmpCalcInsulThick";		//�������ϵ���ı���.

	try
	{
		theApp.m_pConWorkPrj->Execute(_bstr_t("DROP TABLE "+strTblName+" "), NULL, adCmdText);
	}
	catch(_com_error& e)
	{	//�����ڸñ�
		if(e.Error() != DB_E_NOTABLE )
		{
			return 0;
		}
	}
	//����һ�����м��㹫ʽ�ı�

	sSQL = "CREATE TABLE ["+strTblName+"] (ALPHA double, ALPHAn double, ALPHAc double,\
			Epsilon double, ts double, ta double, W double,	B double,D0 double, D1 double, Dmax double ";
	//����Ϊ�������ϵ��Ҫʹ�õ����ֶ�����
	//������㱣�º�ʱ�����ӵ��ֶΡ�
	sSQL += " , lamda double, Ph double, Ae double, t double,tao double, P1 double, P2 double, P3 double,\
			S double, tb double, D2 double, Q double, pi double, Kr double,	\
			taofr double, tfr double, V double, ro double, C double, Vp double, rop double, \
			Cp	double,	Hfr double, delta double, delta1 double, delta2 double	";
	//����������¶��õ����ֶ���.
	sSQL += ", lamda1 double, lamda2 double 	\
			) ";
	try
	{
		theApp.m_pConWorkPrj->Execute(_bstr_t(sSQL), NULL, adCmdText);
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return 0;
	}
	return 1;

}

//------------------------------------------------------------------                
// Parameter(s) : strEnginID[in] ָ���Ĺ��̻����й���
// Return       : �ɹ�����TRUE�����򷵻�FALSE
// Remark       : �滻���ݿ���һЩ�ֶε�ֵ
//------------------------------------------------------------------
BOOL EDIBgbl::ReplaceFieldValue(CString strEnginID)
{
	CString strWhereEngin;
	if (strEnginID.IsEmpty())
		strWhereEngin = _T(" AND EnginID IS NOT NULL ");
	else
		strWhereEngin = _T(" AND EnginID='"+strEnginID+"' ");

	CString SQL;
	_RecordsetPtr pRsInfo;
	_RecordsetPtr pRsFormat;
	_RecordsetPtr pRsUnit;
	pRsFormat.CreateInstance(__uuidof(Recordset));
	pRsInfo.CreateInstance(__uuidof(Recordset));
	pRsUnit.CreateInstance(__uuidof(Recordset));
	CString strDataTblName;
	CString strFormatTblName;
	CString strFieldName;
	CString strOldUnit;
	CString strNewUnit;

	try
	{
		SQL = _T("SELECT * FROM [A_UNIT] ");
		pRsUnit->Open(_variant_t(SQL), theApp.m_pConnectionCODE.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		if (pRsUnit->GetadoEOF() && pRsUnit->GetBOF())
		{
			return FALSE;
		}
		SQL = _T("SELECT * FROM [TableINFO] WHERE [ReplaceUnit]=1");
		pRsInfo->Open(_variant_t(SQL), theApp.m_pConIPEDsort.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);		
		if (pRsInfo->GetadoEOF() && pRsInfo->GetBOF())
		{
			return FALSE;
		}

		for (pRsInfo->MoveFirst(); !pRsInfo->GetadoEOF(); pRsInfo->MoveNext())
		{
			strDataTblName	 = vtos(pRsInfo->GetCollect(_variant_t("ManuTBName")));
			strFormatTblName = vtos(pRsInfo->GetCollect(_variant_t("StructTblName")));
			if (strDataTblName.IsEmpty() || strFormatTblName.IsEmpty())
			{
				continue;
			}
			SQL = _T("SELECT * FROM ["+strFormatTblName+"] WHERE [ReplaceVal]=1");
			if (pRsFormat->GetState() == adStateOpen)
				pRsFormat->Close();
			
			pRsFormat->Open(_variant_t(SQL), theApp.m_pConIPEDsort.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
			if (pRsFormat->GetadoEOF() && pRsFormat->GetBOF())
			{
				continue;
			}
			for (pRsFormat->MoveFirst(); !pRsFormat->GetadoEOF(); pRsFormat->MoveNext())
			{
				strFieldName = vtos(pRsFormat->GetCollect(_variant_t("FieldName")));
				if (strFieldName.IsEmpty())
					continue;

				for (pRsUnit->MoveFirst(); !pRsUnit->GetadoEOF(); pRsUnit->MoveNext())
				{
					strOldUnit	 = vtos(pRsUnit->GetCollect(_variant_t("OldUnit")));
					strNewUnit   = vtos(pRsUnit->GetCollect(_variant_t("NewUnit")));
					if (strOldUnit.IsEmpty())
						continue;

					SQL = _T("UPDATE ["+strDataTblName+"] SET "+strFieldName+"= '"+strNewUnit+"' WHERE "+strFieldName+"='"+strOldUnit+"' ");
					SQL += strWhereEngin;
					theApp.m_pConnection->Execute(_bstr_t(SQL), NULL, adCmdText);
				}
			}
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;
}

CString EDIBgbl::GetShareDbPath()
{
	HKEY hKey;
	char sv[256];
	unsigned long vlen = 256;
	unsigned long nType = REG_SZ;
	CString strDbPath = _T("");
	CString strSubKey = "SOFTWARE\\��ɳ��������������޹�˾\\shareMDB\\";
	if(RegOpenKey(HKEY_LOCAL_MACHINE,strSubKey,&hKey)!=ERROR_SUCCESS)
	{
		return strDbPath;
	}

	if(RegQueryValueEx(hKey,_T("shareMDB"),NULL,&nType,(unsigned char*)sv,&vlen)!=ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return strDbPath;
	}

	strDbPath.Format("%s",sv);
	strDbPath.TrimLeft();
	strDbPath.TrimRight();

	if( strDbPath.Right(1) != "\\" )
		strDbPath += "\\";

	RegCloseKey(hKey);

	return strDbPath;
}
