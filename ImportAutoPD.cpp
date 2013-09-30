// ImportAutoPD.cpp: implementation of the CImportAutoPD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "ImportAutoPD.h"

#include "vtot.h"
#include "edibgbl.h"
//#include "newproject.h"
#include "projectoperate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CAutoIPEDApp theApp;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImportAutoPD::CImportAutoPD()
{

}

CImportAutoPD::~CImportAutoPD()
{

}

//����PD�еı�������
bool CImportAutoPD::ImportAutoPDInsul( CString strCommandLine )
{
	int nk = 0;				//�ؼ��ֵĸ���
	int nStart=0;
	CString  strTemp;
	CString  strFindValue;

	typedef	struct 
	{
		LPCTSTR strKey;		//�ؼ���
		CString strValue;	//��Ӧ�Ĳ���
	}COMMAND1;
	COMMAND1  CommandArr[10];
	CommandArr[nk++].strKey   ="  -DATA_FILENAME ";		 //�����Excel�ļ�·��

	CommandArr[nk++].strKey   = "  -PROJECT_ID ";			 //����ID

	CommandArr[nk++].strKey   = "  -PROJECT_ENGVOL ";		  //���ID

	CommandArr[nk++].strKey   = "  -RECORD_COUNT ";		//ԭʼ���ݵ�����.

	//���������С�
	for(int k=0; k<nk; k++)
	{
		strFindValue = CommandArr[k].strKey;
		nStart = strCommandLine.Find(CommandArr[k].strKey);
		if( nStart == -1 )
		{
			continue;
		}
		else
		{
			nStart += strFindValue.GetLength();
		}
		strTemp = strCommandLine.Mid( nStart );
		strTemp.TrimLeft(" ");
		if( k == nk-1 )
		{
			strTemp.TrimRight(" ");
			CommandArr[k].strValue = strTemp;
		}
		else
		{
			strTemp = strTemp.Mid(0,strTemp.Find(" -")+1);
			strTemp.TrimRight(" ");
			CommandArr[k].strValue = strTemp;
		}
	}
	if( !FileExists(CommandArr[0].strValue) )
	{
		//�������ļ�������.
		AfxMessageBox("�Ҳ����ļ� "+CommandArr[0].strValue+"  , ��ȷ���Ƿ���� !");
		return false;
	}

	//{{������¹���, ���ʼ������Ӧ�ı�.
// 	newproject dlg;
// 	dlg.m_engin =EDIBgbl::SelPrjName  = "autoiped";
// 	dlg.m_eng_code = EDIBgbl::SelPrjID =CommandArr[1].strValue; 
// 	EDIBgbl::SelVlmCODE = CommandArr[2].strValue;
// 	dlg.SetAuto( true );
// 	dlg.insertitem();
	theApp.InitializeProjectTable( CommandArr[1].strValue, CommandArr[2].strValue );
	//}}

	//���뱣��ԭʼ���ݵ���[pre_calc]
	CProjectOperate   project;
	long	nDataRowCount;

	CProjectOperate::ImportFromXLSStruct ImportTable;
	CProjectOperate::ImportFromXLSElement ImportTableItem[22];
	struct
	{
		LPCTSTR NameCh;		// pre_calc���ֶ������������� ����ԭʼ����
		LPCTSTR NameField;	// pre_calc���ֶ���
	}FieldsName[]=
	{
	//	{_T("���"),			_T("c_num")},		//	���Ҫ���⴦��
		{_T("����"),			_T("c_vol")},
		{_T("ɫ��"),			_T("c_color")},
		{_T("�ܵ�/�豸����"),	_T("c_name1")},
		{_T("�ܵ��⾶/���"),	_T("c_size")},
		{_T("�ܵ��ں�"),		_T("c_pi_thi")},
		{_T("�����¶�"),		_T("c_temp")},
		{_T("��װ�ص�"),		_T("c_place")},		// ֻ��ŵص㣬 [2005/06/02] 
//		{_T("��װ�ص�"),		_T("c_p_s")},
		{_T("�ܵ�����"),		_T("c_pi_mat")},
		{_T("��ע"),			_T("c_mark")},
		{_T("�ڱ��²��������"),_T("c_name_in")},
		{_T("�Ᵽ�²��������"),_T("c_name2")},
		{_T("�Ᵽ�²���"),    _T("c_pre_thi")},
		{_T("�ڱ��²���"),    _T("c_in_thi")},
		{_T("�������������"),	_T("c_name3")},
		{_T("����"),			_T("c_wind")},
		{_T("�ȼ۱�������"),	_T("c_price")},
		{_T("������Сʱ��"),	_T("c_hour")},
		{_T("�͹ܵ����º�"),	_T("c_pre_thi")},
		{_T("����"),			_T("c_amount")},
		{_T("����ѹ��"),		_T("c_Pressure")},
		{_T("���ڽ���"),		_T("c_Medium")},
		
	//	{_T("���̴���"),		_T("EnginID")},
	};	
	for( int i=0, n=sizeof(FieldsName)/sizeof(FieldsName[0]); i<n; i++ )
	{
		ImportTableItem[i].DestinationName =  FieldsName[i].NameField;
		ImportTableItem[i].SourceFieldName =  'B' + i;
	}
	ImportTable.ProjectDBTableName = "pre_calc";
	ImportTable.ProjectID = EDIBgbl::SelPrjID; 
	ImportTable.XLSFilePath = CommandArr[0].strValue;
	ImportTable.XLSTableName = "ԭʼ����";
	ImportTable.BeginRow    = 2;
	ImportTable.RowCount  = nDataRowCount = _tcstol( CommandArr[3].strValue, NULL, 10);

	if( ImportTable.RowCount < 1 )
	{
		AfxMessageBox("ԭʼ����Ϊ��!");
		return false;
	}
	ImportTable.pElement = ImportTableItem;
	ImportTable.ElementNum = n;
	project.SetProjectDbConnect( theApp.m_pConnection );

	//�Զ���Excel�е������ݵ�����.
//	project.ImportTblEnginIDXLS(&ImportTable, TRUE );

	//��EXCEL�е������ݵ�ACCESS��������ǰ������ͬ���͵�����.
//	this->ImportExcelToAccess(CommandArr[0].strValue,"ԭʼ����",theApp.m_pConnection, "pre_calc",EDIBgbl::SelPrjID);

	//test zsy 2005.11.5
	//�������ݣ�������ͬ���͵����ݣ�
	UpdateImportAmount(CommandArr[0].strValue,"ԭʼ����",theApp.m_pConnection, "pre_calc",EDIBgbl::SelPrjID,EDIBgbl::SelVlmCODE, nDataRowCount);

	//��ԭʼ���ݽ�һ������.()
	RefreshData( EDIBgbl::SelPrjID );
 
	return true;
}

//����ָ�����̵ļ�¼, ����֪���ֶ�,��������ֶε�ֵ.
bool CImportAutoPD::RefreshData(CString EnginID)
{
	if( EnginID.IsEmpty() )
	{ 
		return false;
	}
	CString strSQL;
	_RecordsetPtr  pRs;

	_RecordsetPtr  pRs_Weather;		//������.
	pRs_Weather.CreateInstance(__uuidof(Recordset));
	HRESULT hr;
	double  e_rate, con_temp1, con_temp2, e_hours, e_wind, Pro_thi;
	double	e_wind_in;	//���ڷ���

	double   dWeatherTemp=0;							//��������еĻ����¶�ֵ
	try
	{
		strSQL = "SELECT * FROM [Ta_Variable] ";
		pRs_Weather->Open(_variant_t(strSQL), theApp.m_pConnectionCODE.GetInterfacePtr(),
						adOpenStatic, adLockOptimistic, adCmdText);
		if (pRs_Weather->GetRecordCount() > 0)
		{
			//�����¶ȵ�����ֵ Ϊ0
			strSQL = "Index=0";
			pRs_Weather->Find(_bstr_t(strSQL), 0, adSearchForward); 
			if (!pRs_Weather->adoEOF)
			{
				//����������е�"���ڻ����¶�"ֵ
				dWeatherTemp = vtof(pRs_Weather->GetCollect(_variant_t("Ta_Variable_VALUE")) );					
			}
		}

		hr = pRs.CreateInstance(__uuidof(Recordset));
		if( FAILED(hr) )
		{
			return false;
		}
		strSQL = "SELECT * FROM [a_config] WHERE EnginID='"+EnginID+"' ";
		if( pRs->State == adStateOpen )
		{
			pRs->Close();
		}
		pRs->PutCursorLocation(adUseClient);
		pRs->Open(_variant_t(strSQL), (IDispatch*)theApp.m_pConnection,
					adOpenStatic, adLockOptimistic, adCmdText);

		if( pRs->GetRecordCount() < 1 )
		{	
			e_rate = con_temp1 = con_temp2 = e_hours = e_wind = 0;
		}
		else
		{
			//ȡ��[a_config]���е�ֵ,
			pRs->MoveFirst();
			e_rate    = vtof( pRs->GetCollect("�������") );
			con_temp1 = vtof( pRs->GetCollect("�����¶�") );
			con_temp2 = vtof( pRs->GetCollect("�����¶�") );
			e_hours   = vtof( pRs->GetCollect("������Сʱ") );
			e_wind    = vtof( pRs->GetCollect("�������") );
		}
		e_wind_in = e_wind;    //���ڷ���Ϊ�������
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	//���������еĻ����¶�û��ֵʱ����ʹ�ó������еĻ����¶�
	con_temp1 = (dWeatherTemp <=0)? con_temp1:dWeatherTemp;

	strSQL = "SELECT * FROM [pre_calc] WHERE EnginID='"+EnginID+"' ";
	try
	{
		if (pRs->GetState() == adStateOpen)
		{
			pRs->Close();
		}
		
		pRs->Open(_variant_t(strSQL), theApp.m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);

		if( pRs->GetRecordCount() <= 0 )
		{	//û�м�¼ʱ
			return true;
		}
		CString tempStr, place, steam;
		pRs->MoveFirst();//
		//ѭ��.��һЩ�ֶν��д���.
		//��:
		// ��װ�ص�(c_p_s)�ֶ�ֵΪ(��������) �����Ϊ"����"��浽c_place;
		//											 "����"�浽c_steam
		while( !pRs->adoEOF )
		{
			//�������
			pRs->PutCollect(_variant_t("c_rate"), _variant_t(e_rate));
			//������Сʱ
			pRs->PutCollect(_variant_t("c_hour"), _variant_t(e_hours));

			tempStr = vtos( pRs->GetCollect("c_name3") );
			if( !tempStr.IsEmpty() )
			{
				if( tempStr.Find("(") == -1 )
				{
					Pro_thi = 20.0;
				}
				else
				{
					tempStr = tempStr.Mid( tempStr.Find("(")+1 );
					Pro_thi = strtod(tempStr, NULL);
				}
				pRs->PutCollect(_variant_t("c_pro_thi"), _variant_t(Pro_thi) );
			}
			//����c_place�ֶ�
			tempStr = vtos(pRs->GetCollect("c_place"));		// �Զ�����PD�е�����ʱ���԰�װ�ص���д��� [2005/06/02] 
			tempStr.TrimLeft(); 
			tempStr.TrimRight();
			tempStr.MakeUpper();

			if( !tempStr.IsEmpty() )
			{
				place = tempStr.Left(4);
				tempStr = tempStr.Mid(4);
				steam = tempStr.Right(4);

				//��װ�ص��ֶβ��洢���ܻ�ˮ������
//				pRs->PutCollect(_variant_t("c_place"), _variant_t(place));
//				pRs->PutCollect(_variant_t("c_steam"), _variant_t(steam));

				if( -1 != place.Find("����") )
				{
					pRs->PutCollect(_variant_t("c_con_temp"), _variant_t(con_temp1));
					//���ڷ���Ϊ0
					pRs->PutCollect(_variant_t("c_wind"), _variant_t(e_wind_in));
				}
				else
				{
					pRs->PutCollect(_variant_t("c_con_temp"), _variant_t(con_temp2));
					//�������
					pRs->PutCollect(_variant_t("c_wind"), _variant_t(e_wind));
				}
			}
			pRs->Update();
			pRs->MoveNext();
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

//------------------------------------------------------------------
// DATE         :[2005/07/14]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :���ӷ���
//------------------------------------------------------------------
BOOL CImportAutoPD::AddValve()
{
	_RecordsetPtr pRs_ValveType;
	CString	strSQL;						//SQL���
	int		nValveIndex;				//�������͵�����
	CString	strValveName;				//��������
	pRs_ValveType.CreateInstance(__uuidof(Recordset));
 	pRs_ValveType->PutCursorLocation(adUseClient);

	try
	{
		strSQL = "SELECT * FROM [ValveType] ";		//�򿪷������ͱ�
		pRs_ValveType->Open(_variant_t(strSQL), theApp.m_pConnectionCODE.GetInterfacePtr(),
			adOpenStatic, adLockOptimistic, adCmdText);
		if (pRs_ValveType->GetRecordCount() <= 0)
		{
			return FALSE;
		}
		for (; !pRs_ValveType->adoEOF; pRs_ValveType->MoveNext())
		{

			nValveIndex  = vtoi(pRs_ValveType->GetCollect(_variant_t("ValveTypeID")));
			strValveName = vtos(pRs_ValveType->GetCollect(_variant_t("TypeName")));
			if (strValveName.IsEmpty())
			{
				continue;
			}
			if (FileExists(strValveName))
			{
				CFile::Remove(strValveName);
			}		
		}
	}
	catch (_com_error)
	{
		return FALSE;
	}

	return TRUE;
}
 
//------------------------------------------------------------------
// DATE         :[2005/09/30]
// Parameter(s) :
// Return       :
// Remark       :��EXCEL�ļ�������ACCESS���ݿ����
//------------------------------------------------------------------
BOOL CImportAutoPD::ImportExcelToAccess(CString strExcelFileName, CString strWorksheetName, _ConnectionPtr pConDes, CString strTblName,CString strCurProID,CString KeyFieldName,CString ProFieldName)
{
	CString		strSQL;					//SQL���
	_ConnectionPtr pConExcel;			//����EXCEL�ļ�
	_RecordsetPtr  pRsExcel;
	_RecordsetPtr  pRsAccess;
	_RecordsetPtr  pRsTmp;
	pConExcel.CreateInstance(__uuidof(Connection));
	pRsExcel.CreateInstance(__uuidof(Recordset));
	pRsAccess.CreateInstance(__uuidof(Recordset));
	pRsTmp.CreateInstance(__uuidof(Recordset));
	STRUCT_ENG_ID listID[10];		//��ͬ���̵������ź͹�������
	int	 nProCount=0;		//��ͬ���̵ĸ���
	CString strTemp;
	int Rj=0;				//���̺���Excel�е���
	int ProjNum=1;
	int ProjIndex=0;
	int	ID;

	if (NULL == pConDes)
	{
		return FALSE;
	}
	try
	{
		EDIBgbl::CAPTION2FIELD* pFieldStruct=NULL;
		_variant_t varTmp;
		//���EXCEL�е��ֶ�����ACCESS�е��ֶεĶ�Ӧֵ,�����ֶθ���
		int nFieldCount = GetField2Caption(pFieldStruct);
		if (nFieldCount <= 0)
		{
			return FALSE;
		}
		
		strSQL = CONNECTSTRING_EXCEL + strExcelFileName;//����EXCEL�ļ�
		pConExcel->Open(_bstr_t(strSQL), "", "", -1);

		//��Excel��������һ������$������������Բ���$�ٴ�һ��.
		strSQL = "SELECT * FROM ["+strWorksheetName+"$]";
		try
		{
			pRsExcel->Open(_variant_t(strSQL), pConExcel.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		}
		catch (_com_error)
		{
			strSQL = "SELECT * FROM ["+strWorksheetName+"]";
			pRsExcel->Open(_variant_t(strSQL), pConExcel.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		}

		if (pRsExcel->adoEOF && pRsExcel->BOF)
		{
			AfxMessageBox("�ļ���û�м�¼��");
			return FALSE;
		}
		//��Access��
		strSQL = "SELECT * FROM ["+strTblName+"] WHERE "+ProFieldName+"='"+strCurProID+"' ORDER BY "+KeyFieldName+" ";
		
		pRsAccess->Open(_variant_t(strSQL), pConDes.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		
		if (pRsAccess->adoEOF && pRsAccess->BOF)
		{
			ID=0;
		}
		else
		{
			pRsAccess->MoveLast();
			ID = vtoi(pRsAccess->GetCollect(_variant_t(KeyFieldName)));
		}
		ID++;
		listID[0].ID = ID;		//��ǰ�����������
		listID[0].EnginID = strCurProID;

		for(;Rj < nFieldCount;Rj++)
		{
			if( !ProFieldName.CompareNoCase(pFieldStruct[Rj].strField) )
			{
				break;
			}
		}
		_RecordsetPtr pRsTmp;
		pRsTmp.CreateInstance(__uuidof(Recordset));

		CString strGroup;		//��Ϊ�ؼ��ֵ������ֶ����
		//strGroup = "[�ܵ��⾶/���], [�ܵ��ں�], [����ѹ��], [�����¶�], [�ܵ�����]";

/*		strGroup = "[����],[ɫ��],[�ܵ�/�豸����],[�ܵ��⾶/���],[�ܵ��ں�],[�����¶�],[��װ�ص�],[�ܵ�����],[��ע],\
					[�ڱ��²��������],[�Ᵽ�²��������],[�������������],[����],[�ȼ۱�������],[������Сʱ��],\
					[�͹ܵ����º�],[����ѹ��],[���ڽ���],[���̴���]";
		
		strSQL = "SELECT "+strGroup+" FROM ["+strTblName+"] WHERE "+ProFieldName+"="+strCurProID+" ";
		pRsTmp->Open(_variant_t(strSQL), pConDes.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		if (pRsTmp->adoEOF && pRsTmp->BOF)
		{
			variant_t var = pRsTmp->GetCollect(_variant_t("enginid"));
		}
*/		//�������ݵ�ACCESS���ݿ��С� 
		while (!pRsExcel->adoEOF)
		{
			pRsAccess->AddNew();
			for (int i=0; i < nFieldCount; i++)
			{
				try
				{
					if (pFieldStruct[i].strCaption.IsEmpty())
					{
						continue;
					}
					varTmp = pRsExcel->GetCollect(_variant_t(pFieldStruct[i].strCaption));
				}catch (_com_error& e) 
				{
					if (e.Error() == -2146825023)
					{
						strTemp = "ԭʼ���ݱ���û���ֶ� '"+pFieldStruct[i].strCaption+"' ��";
						AfxMessageBox(strTemp);
						pFieldStruct[i].strCaption.Empty();
						continue;
					}
					AfxMessageBox(e.Description());
					return FALSE;
				}
				
				if (Rj < nFieldCount && i == Rj)	//�������Ƶ��ֶ�
				{
					strTemp = vtos(varTmp);
					for(int c=0; c<ProjNum; c++)
					{
						if( !strTemp.CompareNoCase(listID[c].EnginID) || (c==0 && strTemp.IsEmpty()))
						{
							ProjIndex = c;
							break;
						}
					}
					if( c == ProjNum )		//��ͬ�Ĺ����ҳ��������.
					{
						listID[c].EnginID = strTemp;
						strSQL = "SELECT * FROM ["+strTblName+"] \
							WHERE "+ProFieldName+"='"+listID[c].EnginID+"' ORDER BY "+KeyFieldName+" ";
						if(pRsTmp->State == adStateOpen)
							pRsTmp->Close();
						pRsTmp->Open(_variant_t(strSQL), pConDes.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
						if( pRsTmp->adoEOF && pRsTmp->BOF )
						{
							ID = 0;
						}
						else
						{
							pRsTmp->MoveLast();
							varTmp = pRsTmp->GetCollect(_variant_t(KeyFieldName));
							ID = vtoi(varTmp);
						}
						//�������,����¼�ӵ�ĩβ.
						listID[c].ID = ++ID;
						ProjIndex = c;
						ProjNum++; 
					}
				}
				
				pRsAccess->PutCollect(_variant_t(pFieldStruct[i].strField),varTmp);
			}
			//���ùؼ�ֵ�͹���ID
			pRsAccess->PutCollect(_variant_t(KeyFieldName), _variant_t((long)listID[ProjIndex].ID++));
			pRsAccess->PutCollect(_variant_t(ProFieldName),_variant_t(listID[ProjIndex].EnginID));
			pRsAccess->PutCollect(_variant_t("c_bImport"),_variant_t((short)1));	//�����־
			pRsAccess->PutCollect(_variant_t("c_CalInThi"),_variant_t((short)1));	//�Ƿ��Զ������ڱ��µĺ�� 1:������
			pRsAccess->PutCollect(_variant_t("c_CalPreThi"),_variant_t((short)1));	//�Ƿ��Զ������Ᵽ�µĺ��


			pRsAccess->Update();
			pRsExcel->MoveNext();
		}

		if (NULL != pFieldStruct)
		{
			delete [] pFieldStruct;
		}
	}
	catch (_com_error& e) 
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}

	return TRUE;
}
                                              
//-------------------  -----------------------------------------------
// DATE         :[2005/10/08]
// Parameter(s) :EDIBgbl::CAPTION2FIELD* &pFieldStruct	�����ݿ���ȡ�����ֶζ�Ӧ��ϵ���ڽṹ�����С�
//				��const BOOL bFlag				ȡ�ֶεĹ��������ı�� Ĭ��Ϊ0��Ϊ1ʱ���ڵ���PD���ݵĹؼ��ֶΡ�
// Return       :�����ֶθ���
// Remark       :��IPEDsort.mdb��PD2IPED����ȡ��EXCEL�е��ֶ�����ACCESS�е��ֶεĶ�Ӧֵ
//------------------------------------------------------------------
int CImportAutoPD::GetField2Caption(EDIBgbl::CAPTION2FIELD* &pFieldStruct, const BOOL bFlag)
{
	_RecordsetPtr pRs;
	CString strSQL;
	int		nFieldCount;	//�ֶεĸ���    
	pRs.CreateInstance(__uuidof(Recordset));
	try
	{
		switch(bFlag) {
		case 0:strSQL = "SELECT * FROM [PD2IPED]";
			break;
		case 1:strSQL = "SELECT * FROM [PD2IPED] WHERE CADFieldSeq IS NOT NULL";
			break;
		default:strSQL = "SELECT * FROM [PD2IPED]";
		}
		
		pRs->Open(_variant_t(strSQL), theApp.m_pConIPEDsort.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		nFieldCount = pRs->GetRecordCount();
		if (nFieldCount <= 0)
		{
			return FALSE;
		} 
		pFieldStruct = new EDIBgbl::CAPTION2FIELD[nFieldCount];
		for (int i=0; !pRs->adoEOF && i < nFieldCount; pRs->MoveNext(),i++)
		{
			pFieldStruct[i].strCaption = vtos(pRs->GetCollect("LocalCaption"));		//EXCEL�е��ֶ�����
			pFieldStruct[i].strField   = vtos(pRs->GetCollect("FieldName"));		//ACCESS�е��ֶ�����
			pFieldStruct[i].nFieldType = vtoi(pRs->GetCollect("FieldType"));		//�ֶ�����
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return nFieldCount;
}

//------------------------------------------------------------------
// DATE         :[2005/10/27]
// Parameter(s) :
// Return       :
// Remark       :��PD�������ݵ�IPEDʱ�����ͬһ�ֹܵ����ӻ���ٳ���ʱ����ֻ�ı������ֶε�ֵ��
//				:�����ֶε����ݻ�����IPED�п����޸��˵�ֵ��
//------------------------------------------------------------------
/*BOOL CImportAutoPD::UpdateImportAmount1(CString strExcelFileName, CString strWorksheetName,
					_ConnectionPtr pConDes, CString strTblName,CString strCurProID,CString strCurVol,
					long nExcelRecCount,CString KeyFieldName,CString ProFieldName)
*/
BOOL CImportAutoPD::UpdateImportAmount(const CString strExcelFileName, CString strWorksheetName, 
					const _ConnectionPtr pConDes, const CString strTblName, const CString strCurProID,
					CString strCurVol, long nExcelRecCount, const CString KeyFieldName, const CString ProFieldName)

{
	CString strSQL;			//SQL���
	short	nGroupFieldCount;
	short   nAllFieldCount;
	_ConnectionPtr pConExcel;
	_RecordsetPtr pRsExcel;
	_RecordsetPtr pRsAccess;
	pRsExcel.CreateInstance(__uuidof(Recordset));
	pRsAccess.CreateInstance(__uuidof(Recordset));
	try
	{
		if (pConDes == NULL)
		{
			return FALSE;
		}
		EDIBgbl::CAPTION2FIELD * pAllFieldStruct = NULL;
		EDIBgbl::CAPTION2FIELD * pGroupFieldStruct = NULL;
		nGroupFieldCount = GetField2Caption(pGroupFieldStruct, 1);		//�����Ϲؼ��ֶζ�Ӧ����
		nAllFieldCount	 = GetField2Caption(pAllFieldStruct);			//��Ҫ����������ֶ�����
		if (nGroupFieldCount <= 0 || nAllFieldCount <= 0)
		{
			return FALSE;
		}
		
		//����EXCEL�ļ�
		if ( !OpenExcelTable( pRsExcel, strWorksheetName, strExcelFileName ) )
		{
			return FALSE;
		}
		if (pRsExcel->adoEOF && pRsExcel->BOF)
		{
			return TRUE;
		}
		//��ACCESSԭʼ���ݱ�
		strSQL = "SELECT * FROM ["+strTblName+"] WHERE "+ProFieldName+"='"+strCurProID+"' AND c_vol LIKE '_"+strCurVol+"_' AND c_bImport=1 ";
		pRsAccess->Open(_variant_t(strSQL), pConDes.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		for (; !pRsAccess->adoEOF; pRsAccess->MoveNext())
		{
			pRsAccess->PutCollect(_variant_t("c_bImport"), _variant_t((short)14));
		}

		CString strAccessVal;
		CString strExcelVal;
		CString strTmp;
		CString strWhere;
		_variant_t varTmp;
		CString strIDField = "ID";
		int			  nAmountFieldCount=1;		//�ؼ��ֶεĸ���.
		EDIBgbl::CAPTION2FIELD amountFieldName[1];			//�����ֶ���
		amountFieldName[0].nFieldType=1;
		amountFieldName[0].strCaption="����";
		amountFieldName[0].strField="c_amount";
		if (nExcelRecCount <= 0)
		{
			nExcelRecCount = pRsExcel->GetRecordCount();
		}
		//��ACCESSԭʼ���д�����ǰ�������ͬһ���͵ļ�¼��
		for (; !pRsExcel->adoEOF && nExcelRecCount > 0; pRsExcel->MoveNext(), nExcelRecCount--)
		{
			strWhere = "";
			for (int i=0; i < nGroupFieldCount; i++)
			{
				strExcelVal = vtos(pRsExcel->GetCollect(_variant_t(pGroupFieldStruct[i].strCaption)));
				switch(pGroupFieldStruct[i].nFieldType)
				{
				case 0:		//�ֶ�����Ϊ�ַ���
					strTmp = "='"+strExcelVal+"'";
					break;
				case 1:		//�ֶ�����Ϊ������
					strTmp = "="+strExcelVal+" ";
					break;
				default: strTmp="IS NOT NULL";
				}
				if (i < nGroupFieldCount-1)  //�������������
				{
					strTmp += " AND ";
				}
				strWhere += "[" + pGroupFieldStruct[i].strField + "]" + strTmp;
			}

			try
			{
				pRsAccess->PutFilter(_variant_t((long)adFilterNone));
				pRsAccess->PutFilter(_variant_t(strWhere));
			}catch (_com_error)
			{
				//��EXCEL���жϼ�¼���������ܳ���
				continue;
			}
			//�ı���ͬ���͵�����
			if (!(pRsAccess->adoEOF && pRsAccess->BOF))
			{
				for (int j=0; j < nAmountFieldCount; j++)
				{
					varTmp = pRsExcel->GetCollect(_variant_t(amountFieldName[j].strCaption));
					pRsAccess->PutCollect(_variant_t(amountFieldName[j].strField), varTmp);
					pRsAccess->PutCollect(_variant_t("c_bImport"), _variant_t((short)1));
					pRsAccess->Update();
				}
			}else
			{
				pRsAccess->AddNew();
				for (int i=0; i < nAllFieldCount; i++)
				{
					try
					{
						//����ֶ���Ϊ��ʱ����� 
						if (pAllFieldStruct[i].strCaption.IsEmpty() || pAllFieldStruct[i].strField.IsEmpty())
						{
							continue;
						}
						varTmp = pRsExcel->GetCollect(_variant_t(pAllFieldStruct[i].strCaption));
					}catch (_com_error& e) 
					{
						if (e.Error() == -2146825023)
						{
							strTmp = "ԭʼ���ݱ���û���ֶ� '"+pAllFieldStruct[i].strCaption+"' ��";
							AfxMessageBox(strTmp);
							pAllFieldStruct[i].strCaption.Empty();
							continue;
						}
						AfxMessageBox(e.Description());
						return FALSE;
					}
					
					pRsAccess->PutCollect(_variant_t(pAllFieldStruct[i].strField),varTmp);
				}
				//���ùؼ�ֵ�͹���ID
				pRsAccess->PutCollect(_variant_t(ProFieldName), _variant_t(strCurProID));
				pRsAccess->PutCollect(_variant_t("c_bImport"),_variant_t((short)1));	//�����־
				pRsAccess->PutCollect(_variant_t("c_CalInThi"),_variant_t((short)1));	//�Ƿ��Զ������ڱ��µĺ�� 1:������
				pRsAccess->PutCollect(_variant_t("c_CalPreThi"),_variant_t((short)1));	//�Ƿ��Զ������Ᵽ�µĺ��
				
				
				pRsAccess->Update();
			}//
		}
		//ɾ��IPED�е���ĵ��޸������͵Ĺܵ���¼
		strSQL = "DELETE FROM ["+strTblName+"] WHERE EnginID='"+strCurProID+"' AND c_vol LIKE '_"+strCurVol+"_' AND c_bImport=14";
		pConDes->Execute(_bstr_t(strSQL),NULL, -1);

		//�Ե�ǰ���̵ļ�¼���±��
		strSQL = "SELECT * FROM ["+strTblName+"] WHERE EnginID='"+strCurProID+"' ORDER BY c_bImport desc, c_vol asc ";
		if (pRsAccess->State == adStateOpen)
		{
			pRsAccess->Close();
		}
		pRsAccess->PutFilter(_variant_t((long)adFilterNone));
		pRsAccess->Open(_variant_t(strSQL), pConDes.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		if (!(pRsAccess->adoEOF && pRsAccess->BOF))
		{
			pRsAccess->MoveFirst();
		}
		for (long NO=1; !pRsAccess->adoEOF; pRsAccess->MoveNext(),NO++)
		{
			pRsAccess->PutCollect(_variant_t(strIDField), _variant_t(NO));
		}
		if (NULL != pGroupFieldStruct)
		{
			delete [] pGroupFieldStruct;
		}
		if (NULL != pAllFieldStruct)
		{
			delete [] pAllFieldStruct;
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;
}


//------------------------------------------------------------------
// DATE         :[2005/12/05]
// Parameter(s) :
// Return       :
// Remark       :��ADO�ķ�ʽ������EXCEL�ļ�.
//------------------------------------------------------------------
BOOL CImportAutoPD::ConnectExcelFile(const CString strExcelName, _ConnectionPtr &pConExcel)
{
	CString strSQL;
	try
	{
		if (pConExcel == NULL)
		{
			pConExcel.CreateInstance(__uuidof(Connection));
		}
		if ( pConExcel->State == adStateOpen )
		{
			pConExcel->Close();
		}

		strSQL = CONNECTSTRING_EXCEL + strExcelName;
		pConExcel->Open(_bstr_t(strSQL), "", "", adCmdUnspecified);
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE; 

}
//------------------------------------------------------------------
// DATE         :[2005/12/05]
// Parameter(s) :CString  strExcelFileName   EXCEL�ļ�ȫ��
//				:CString& strSheetName	     ��EXCEL�е���Ҫ�򿪵ı���
//				:_RecordsetPtr& pRs			 ���ش򿪱�ļ�¼��
// Return       :�ɹ�ΪTRUE�����򷵻�FALSE
// Remark       :��ADO��¼���ķ�ʽ�򿪣�EXCEL�ļ��еı�
//------------------------------------------------------------------
BOOL CImportAutoPD::OpenExcelTable(_RecordsetPtr& pRsTbl, CString& strSheetName, CString strExcelFileName)
{ 
	CString strSQL;
	try
	{ 
		if ( m_pConExcel == NULL || m_pConExcel->State == adStateClosed )
		{
			if ( !ConnectExcelFile( strExcelFileName, m_pConExcel ) )
			{
				return FALSE;
			}
		}
		if (pRsTbl == NULL)
		{
			pRsTbl.CreateInstance(__uuidof(Recordset));
		}
		try
		{
			if (pRsTbl->State == adStateOpen)
			{
				pRsTbl->Close();
			}
			strSQL = "SELECT * FROM ["+strSheetName+"$] ";		//��EXCEL�еı�������$
			pRsTbl->Open(_variant_t(strSQL), m_pConExcel.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
			strSheetName += "$";								//�ڵ��øú���֮��ĳ�������ΪEXCEL����
		}
		catch (_com_error)
		{
			strSQL = "SELECT * FROM ["+strSheetName+"] ";
			pRsTbl->Open(_variant_t(strSQL), m_pConExcel.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		} 
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;
}
