// InputOtherData.cpp: implementation of the InputOtherData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "autoipedview.h"
#include "InputOtherData.h"
#include "Dialog_pform.h"
#include "BrowseForFolerModule.h"
#include "EDIBgbl.h"
#include "MainFrm.h"
#include "afxwin.h"
#include "vtot.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CAutoIPEDApp theApp;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
InputOtherData::InputOtherData()
{
	cancel=0;
	ProID=NULL;
	Pname=NULL;
	strConnFeature[nDBFileterIndex_UESOFT].strCONN=";";
	strConnFeature[nDBFileterIndex_UESOFT].strEnginTable="Engin";
	strConnFeature[nDBFileterIndex_UESOFT].strEnginIDFieldName="EnginID";
	strConnFeature[nDBFileterIndex_UESOFT].strEnginName="gcmc";
	strConnFeature[nDBFileterIndex_UESOFT].strFILENAME="AutoIPED";
	strConnFeature[nDBFileterIndex_UESOFT].strExtension="mdb";
	strConnFeature[nDBFileterIndex_UESOFT].strFilter="UESOFT AutoIPED(*.mdb)|*.mdb|All Files(*.*)|*.*||";

	
	strConnFeature[nDBFileterIndex_VFP_SW].strCONN="dBASE IV;";
	strConnFeature[nDBFileterIndex_VFP_SW].strEnginTable="a_eng";
	strConnFeature[nDBFileterIndex_VFP_SW].strEnginIDFieldName="eng_code";
	strConnFeature[nDBFileterIndex_VFP_SW].strEnginName="engin";
	strConnFeature[nDBFileterIndex_VFP_SW].strFILENAME="a_eng";
	strConnFeature[nDBFileterIndex_VFP_SW].strExtension="dbf";
	strConnFeature[nDBFileterIndex_VFP_SW].strFilter="����Ժ�ӿ�(*.dbf)|*.dbf|All Files(*.*)|*.*||";
	
	strConnFeature[nDBFileterIndex_DB3_JS].strCONN="dBASE IV;";
	strConnFeature[nDBFileterIndex_DB3_JS].strEnginTable="a_eng";
	strConnFeature[nDBFileterIndex_DB3_JS].strEnginIDFieldName="eng_code";
	strConnFeature[nDBFileterIndex_DB3_JS].strEnginName="engin";
	strConnFeature[nDBFileterIndex_DB3_JS].strFILENAME="a_eng";
	strConnFeature[nDBFileterIndex_DB3_JS].strExtension="dbf";
	strConnFeature[nDBFileterIndex_DB3_JS].strFilter="����Ժ�ӿ�(*.dbf)|*.dbf|a_eng.dbf|a_eng*.dbf|All Files(*.*)|*.*||";
	
}

InputOtherData::~InputOtherData()
{
	if(ProID)
	{
		delete[] ProID;
		ProID=NULL;
	}

	if(Pname)
	{
		delete[] Pname;
		Pname=NULL;
	}
}

void InputOtherData::SelectFile(int iConn)
{
	//ѡ����������������������װ·��
	
	CString strStart;
	CString strSelect;
	CString strMsg,strField;
	iCONN=iConn;

	switch(iCONN){
	case nDBFileterIndex_UESOFT:
		strStart="D:\\����ɽ�����ݹ��̱������ݿ�\\�������ݿ�\\IPEDprjdb7.0";
		break;
	case nDBFileterIndex_VFP_SW:
		strStart=_T("d:\\fox24\\data"); //Ĭ�ϳ�ʼ·����������������������һ�㶼��װ������
		break;
	case nDBFileterIndex_DB3_JS:
		strStart=_T("d:\\fox24\\bw"); //Ĭ�ϳ�ʼ·����������������������һ�㶼��װ������
		break;
	default:
		;
	}
	

	short i=1;
	CString strFilter;
	strFilter=strConnFeature[iCONN].strFilter;


	BrowseForFolerModule::BrowseForFoldersFromPathStart(NULL,strStart, strSelect);

	if(strSelect.IsEmpty())
	{
		return ;
	}
	strFileName.Format("%s\\%s.%s",strSelect,strConnFeature[iCONN].strFILENAME,strConnFeature[iCONN].strExtension);//·��=Ŀ¼+�ļ���+��չ��

	CFileFind filefind;
	BOOL bool1;
	BOOL bool2;

	//bool1=filefind.FindFile(LPCTSTR(strFileName));	//����Ҫǿ��ת��Ϊָ�롣
	bool1=filefind.FindFile(strFileName);	
	
	if(!bool1)
	{
		//ѡ������Ŀ¼�²�������Ҫ����Ŀ¼strFileName������ѡ��һ������strFileName�ļ����ڵ�Ŀ¼��ΪԴĿ¼
		strMsg.Format("�Ҳ���Ŀ¼���ļ�%s!\n������%s�ļ����ڵ�Ŀ¼��?",strFileName,\
			strFileName);
		if(IDYES==AfxMessageBox(strMsg,MB_YESNO+MB_ICONQUESTION+MB_DEFBUTTON1))
		{
			LPTSTR lpstrFile;
			lpstrFile=strConnFeature[iCONN].strFILENAME.GetBuffer(strConnFeature[iCONN].strFILENAME.GetLength()-1);
			
			CFileDialog fdlg(TRUE,strConnFeature[iCONN].strExtension,lpstrFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter,NULL);
			
			
			//fdlg.m_ofn.lpstrDefExt=strConnFeature[iCONN].strExtension;
			//fdlg.m_ofn.lpstrFile= lpstrFile;
			//fdlg.m_ofn.lpstrFilter=strFilter;
			//fdlg.m_ofn.nFilterIndex=iCONN;
			fdlg.m_ofn.lpstrInitialDir=strSelect;
			
			//����m_ofn�ṹ���ʹ�ó������⡣ע�͵��Ա���ԡ�



			if(IDOK==fdlg.DoModal())//��ʾ�ļ�ѡ��Dialog
			{
				strFileName=fdlg.GetPathName();
				bool1=filefind.FindFile(LPCTSTR(strFileName));
				if(bool1)
				{
					//�����ж��ļ��Ƿ��Ƿ���Ҫ����ļ���
					if(MeetRequirement(strFileName))
					{
						strSelect=strFileName.Left(strFileName.ReverseFind('\\'));//ȥ���ļ��������Ŀ¼
						if(nDBFileterIndex_UESOFT!=iCONN)
						{
							strFileName=strSelect;
						}
					}
					else
						return;
				}
				else
					return;
			}
			else 
				return;//����������fdlg.m_ofn.nFileterIndex��ȡֵԽ�硣
		}
		else 
			return;
	}
	else
	{
		if(nDBFileterIndex_UESOFT!=iCONN)
			strFileName=strSelect;
	}
			

	if(bool1)
	{
		//���ԴĿ¼�´���a_eng


		//ѡ�񹤳̡��ڱ�Engin�С�
		if(!SelectPro()) 
		
			return;

		int n = 0;
		CString sql1;
		//Access���
		if(nDBFileterIndex_UESOFT==iCONN)
		{
			while(n<count)
			{		
				strPathProID=strFileName;
				bool2=filefind.FindFile(LPCTSTR(strFileName));
				
				if(!bool2)
				{
					strMsg.Format(_T("�ڵ�ǰĿ¼��û�й���:  %s"),ProID[n]);
					AfxMessageBox(strMsg);
					n++;
				}
				else
				{
					sql1.Format("insert into Engin (EnginID,gcmc) values('%s','%s')",ProID[n],Pname[n]);
					try
					{
						theApp.m_pConAllPrj->Execute(_bstr_t(sql1),NULL,adCmdText);
					}
					catch(_com_error)
					{
						if(cancel==1)
						{}
						else 
							AfxMessageBox("�����Ѿ����ڣ����ܵ������ݣ�");
						
						//n++;
						//continue;//mark_hubb_continue
					}
					
					if ( !InputData(strSelect,ProID[n]))/*strSelect,��Ϊ��Access���ݿ��²���ֱ�����ļ���·��*/
					{
						strMsg = _T("���̡�"+ProID[n]+"������ʧ�ܣ�");
						((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
						((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0, strMsg, TRUE);
					}
					n++;
				}			
			}
		}
		///��Access���
		else
		{

			while(n<count)
			{		
				strPathProID=strSelect+_T("\\")+ProID[n];
				bool2=filefind.FindFile(LPCTSTR(strPathProID));

				if(!bool2)
				{
					strMsg.Format(_T("�ڵ�ǰĿ¼��û�й���:  %s"),ProID[n]);
					AfxMessageBox(strMsg);
					n++;
				}
				else
				{
					sql1.Format("insert into Engin (EnginID,gcmc) values('%s','%s')",ProID[n],Pname[n]);
					try
					{
						theApp.m_pConAllPrj->Execute(_bstr_t(sql1),NULL,adCmdText);
					}
					catch(_com_error)
					{
						if(cancel==1)
						{}
						else 
							AfxMessageBox("�����Ѿ����ڣ����ܵ������ݣ�");

						n++;
						//continue;//mark_hubb_continue
					}

					if ( !InputData(strSelect,ProID[n]))
					{
						strMsg = _T("���̡�"+ProID[n]+"������ʧ�ܣ�");
						((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
						((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0, strMsg, TRUE);
					}
					n++;
				}			
			}
		}
	}

	return;

}

bool InputOtherData::SelectPro()
{
	CString sql;

	try
	{
		theApp.m_pConnection->Execute("drop table foxpro_eng",NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		if(e.Error()!=DB_E_NOTABLE)
		{
			ReportExceptionError(e);
			//AfxMessageBox("xx1");
			return false;
		}
	}
	if(nDBFileterIndex_UESOFT==iCONN)//�ڵ���UEsoft��ʱ�����Ҫ���Ĳ�����
	{
		try
		{
			//�����ж��Ƿ����Engin��ĺ�����
			
			//--------------->
			_ConnectionPtr m_pConSelect;
			_RecordsetPtr m_pRecSelect1;
			_RecordsetPtr m_pRecSelect2;
			CString strFileName_x;
			strFileName_x=CONNECTSTRING+strFileName;
			//�ѵ�б��·�����˫б��·��
			//---->
			strFileName_x.Replace(LPCTSTR("\\"),LPCTSTR("\\\\"));
			//<-----
			m_pConSelect.CreateInstance(__uuidof(Connection));
			m_pRecSelect1.CreateInstance(__uuidof(Recordset));
			m_pRecSelect2.CreateInstance(__uuidof(Recordset));
			m_pConSelect->Open(_bstr_t(strFileName_x),"","",-1);
			if(  !(Have_Table(m_pConSelect,"Engin"))  )
			{
				//���Engin������->
				//����һ��Engin��
				m_pConSelect->Execute(_bstr_t("CREATE TABLE [Engin] (EnginID varchar(20), Name varchar(20))"),NULL,adCmdText);
				m_pRecSelect1->Open(_bstr_t("select * from Engin"),m_pConSelect.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
				//��pre_calc���г�ȡEngin�ֶεļ�¼
				m_pRecSelect2=m_pConSelect->Execute(_bstr_t("SELECT DISTINCT ENGINID FROM PRE_CALC"),NULL,adCmdText);
				
				m_pRecSelect2->MoveFirst();
				while(!m_pRecSelect2->adoEOF)
				{
					_variant_t EnginID;
					EnginID=m_pRecSelect2->GetCollect("EnginID");
					if(EnginID.vt!=VT_NULL)
					{
						m_pRecSelect1->AddNew();
						m_pRecSelect1->PutCollect("EnginID",EnginID);
						m_pRecSelect1->PutCollect("Name",_variant_t("��������"));
					}
					m_pRecSelect2->MoveNext();
				}
				m_pRecSelect1->Update();
					
				
				m_pRecSelect1->Close();
				m_pRecSelect2->Close();
				
			}
			m_pConSelect->Close();

			
			/////<-------------------------
		}
		catch(_com_error &e)
		{
			e;
			//ReportExceptionError(e);
			AfxMessageBox("��ѡ���ݿⲻ����Ҫ�󡣲��ܵ��롣");
			return false;
		}
	}

	try
	{
		//��a_eng ���� Engin���ݵ�foxpro_eng
		sql.Format(_T("SELECT * INTO foxpro_eng FROM %s IN '%s'[%s]"),\
			strConnFeature[iCONN].strEnginTable,strFileName,strConnFeature[iCONN].strCONN);
		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		AfxMessageBox("û���ҵ����̱�������ѡ������ݿ�������ȷ����ѡ������ȷ�����ݿ⡣");
		
		//ReportExceptionError(e);
		e;
		return false;
	}


	_RecordsetPtr  prec_source,prec_engin;
	CString eng_code,sql_eng,strMsg;

	prec_source.CreateInstance(_uuidof(Recordset));
	prec_engin.CreateInstance(_uuidof(Recordset));
	sql_eng.Format(_T("select * from foxpro_eng where %s is not null"),strConnFeature[iCONN].strEnginIDFieldName);

	prec_source->Open(_variant_t(sql_eng),
		              theApp.m_pConnection.GetInterfacePtr(),
					  adOpenDynamic,
					  adLockOptimistic,
					  adCmdText);

	if(!prec_source->adoEOF)
	{
		prec_source->MoveFirst();
	}
	else
		AfxMessageBox("ѡ������ݿⲻ����Ҫ����ȷ����ѡ������ȷ�����ݿ⡣");
	while(!prec_source->adoEOF)
	{		
		//���Դ��Ŀ��ÿ��������Allprjdb.mdb��Engin���Ƿ��м�¼���м�¼˵���Ѿ���������Ͳ�Ҫ�г��������ظ�����
 	//	eng_code=(LPCSTR)_bstr_t(prec_source->GetCollect("eng_code"));		
    	eng_code =	vtos(prec_source->GetCollect(_variant_t(strConnFeature[iCONN].strEnginIDFieldName)));

		sql_eng.Format("select * from Engin where EnginID='%s'",eng_code);

		prec_engin->Open(_variant_t(sql_eng),
				        theApp.m_pConAllPrj.GetInterfacePtr(),
						  adOpenDynamic,
						  adLockOptimistic,
						  adCmdText);

		if(prec_engin->adoEOF && prec_engin->BOF)
			prec_source->MoveNext();
		else
		{
			try
			{
				strMsg=strMsg+" "+eng_code;
				prec_source->Delete(adAffectCurrent);
				prec_source->MoveNext();
			}
			catch(_com_error &e)
			{
//				AfxMessageBox(e.Description());
				ReportExceptionError(e);
				return false;
			}
		}

		prec_engin->Close();
	}

	prec_source->Close();

	sql_eng.Format("select distinct * from foxpro_eng where %s is not null",strConnFeature[iCONN].strEnginIDFieldName);
	prec_source->Open(_variant_t(sql_eng),
		              theApp.m_pConnection.GetInterfacePtr(),
					  adOpenDynamic,
					  adLockOptimistic,
					  adCmdText);


	if(prec_source->adoEOF && prec_source->BOF)
	{
		CString strMsg1;
		strMsg1.Format("%s������Ŀ��¼�Ѿ����ڻ�����ѡ������ݿⲻ����Ҫ��\n��Ҫ���µ�����Щ�������ݣ���ɾ��allprjdb.mdb��engin��\
			EnginID�ֶ�ֵƥ����Щ������Ŀ���ŵļ�¼��",strMsg);
		AfxMessageBox(strMsg1);
		return false;
	}

	Dialog_pform Dialog_pform1;

	Dialog_pform1.m_pRecordset=prec_source;
	Dialog_pform1.index=1;
	Dialog_pform1.old="";

	if(Dialog_pform1.DoModal()==IDCANCEL)
	{
		return false;
	}
//	CString *name;
//	name=Dialog_pform1.m_pform1;

	int n;
	CString sql1;

	n=0;
	count=Dialog_pform1.i;

	if(ProID)
		delete[] ProID;

	if(Pname)
		delete[] Pname;

	ProID=new CString[count];
	Pname=new CString[count];
	
	while(n<Dialog_pform1.i)
	{
		if(nDBFileterIndex_UESOFT==iCONN)
		{
			ProID[n]=Dialog_pform1.engin_name[n];
			Pname[n]=Dialog_pform1.engin[n];
		}
		else
		{
			ProID[n]=Dialog_pform1.engin[n];
			Pname[n]=Dialog_pform1.engin_name[n];
		}
			
		n++;
	}
	
	
	prec_source->Close();
	prec_source=NULL;
	theApp.m_pConnection->Execute("drop table foxpro_eng",NULL,adCmdText);
	return true;
}

bool InputOtherData::InputData(CString strBWPath,CString strPrjID)
{
	int pos;
	CString SQL,strPath;
	CString strTableName;
	CString strMsg;
	_variant_t varTableName;
	_RecordsetPtr  prec_STableName;
	_ConnectionPtr pcon_source;

	pcon_source.CreateInstance(_uuidof(Connection));

	//SQL=strFileName;
	if(nDBFileterIndex_UESOFT==iCONN)
	{
		strPath.Format("%s\\%s.%s",strBWPath,strConnFeature[iCONN].strFILENAME,strConnFeature[iCONN].strExtension);
	}
	else
	{
		strPath.Format("%s\\%s",strBWPath,strPrjID);
	}
	SQL.Format(CONNECTSTRING_DBDRV,strConnFeature[iCONN].strCONN,strPath);
	//mark_hubb_pcon_source->open();
	try
	{

		pcon_source->Open(_bstr_t (SQL),"","",-1);
	}
	catch(_com_error e)
	{
		ASSERT(e.ErrorMessage());
	}
//	prec_STableName.CreateInstance(__uuidof(Recordset));

	prec_STableName=pcon_source->OpenSchema(adSchemaTables);

	strMsg.Format("���ڵ��빤�̣�%s ...",strPrjID);

	pos=1;

	//�����ݿ��ж�ȡ��Ҫ����ı���.
	//----->
	CString * szTableName;
	int     nTblCount;

	CString  pNotTbl[] =
	{
		_T("Volume"),
		_T("Engin")
	};				
	if(	!GetImportTblNames(szTableName, nTblCount, pNotTbl, sizeof(pNotTbl)/sizeof(pNotTbl[0])) )//�õ�32����������Volume��Engin�ģ�������szTableName�С�
	{
		delete szTableName;
		return FALSE;
	}
	//<-------
	//����ϰ汾��û��ָ���ı�,����Ĭ�ϵļ�¼���ù���.
	CMapStringToString		strArray;
//	CString strTmp;
	//��prec_STableName���ӵ����ݿ��У����û�ѡ����Ҫ�ĵ����·���µ����ݿ⡣������ɽ·���µ�AutoIPED.MDB���ݿ⣩
	//ȡ�����еı�������strArray��ӳ���С�
	//----->
	for(; !prec_STableName->adoEOF; prec_STableName->MoveNext() )
	{
		strTableName=vtos( prec_STableName->GetCollect("TABLE_NAME") );
		strTableName.MakeUpper();
		strArray.SetAt(strTableName, strTableName);
	}
	//<-------
	for(int i=0; i<nTblCount; i++)
	{
		pos = 100 / nTblCount * i + 1;
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos);
		
		
		//ȫ��ת��Ϊ��д��ĸ,�ٽ���ƥ��
		if( strArray.Lookup( szTableName[i], strTableName) )
		{
			//�ϰ汾�д��ڸñ�,���뵽ָ���Ĺ��̡�
			if(strTableName==_T("A_DIR"))
			{
				ImportTable_a_dir(pcon_source,strPrjID);//
			}
			else
			{
				try
				{
					ImportTable_abnormal(pcon_source,strTableName,strPrjID);
				}
				catch(_com_error &e)
				{
					ReportExceptionError(e);
					delete [] szTableName;
					return false;
				}
			}
		}
		else
		{   //�ϰ汾��û��ָ���ı�,��
			//��Ĭ�ϵļ�¼���뵽�ù���.
			if( !EDIBgbl::InsertDefTOEng(theApp.m_pConnection, szTableName[i], strPrjID,"") )
			{
				return false;
			}
		}
	}
	try
	{
		UpdateVolumeTable(strPrjID);

		pos++;
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos);
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		delete [] szTableName;
		return false;
	}

	
//	while(pos<100)
//	{
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos);
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,strMsg,true);
//		Sleep(20);
//		pos++;
//	}
	if (gbIsReplaceUnit )	// ���뱣����������ʱ�滻��λ
	{
		EDIBgbl::ReplaceFieldValue(strPrjID);
	}


	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
	
	strMsg.Format("�ѵ��빤�̣�%s !",strPrjID);
	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,strMsg,true);

	delete [] szTableName;
	return true;
}


//DEL bool InputOtherData::ImportTable(_ConnectionPtr pcon_source, CString strTableName, CString strProID)
//DEL {
//DEL 	CString sql;
//DEL 
//DEL 	try
//DEL 	{
//DEL 		theApp.m_pConnection->Execute("drop table ppp",NULL,adCmdText);
//DEL 	}
//DEL 	catch(_com_error &e)
//DEL 	{
//DEL 		if(e.Error()!=DB_E_NOTABLE)
//DEL 		{
//DEL 			ReportExceptionError(e);
//DEL 			throw;
//DEL 		}
//DEL 	}
//DEL  
//DEL 	try
//DEL 	{
//DEL 		sql.Format(_T("SELECT * INTO ppp FROM %s IN '%s'[%s]"),strTableName,strPathProID,strConnFeature[iCONN].strCONN);
//DEL 		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
//DEL 
//DEL 		try
//DEL 		{
//DEL 			sql=_T("alter table ppp add column EnginID varchar(100)");
//DEL 			theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
//DEL 		}
//DEL 		catch(_com_error)
//DEL 		{
//DEL 			//��û������ʱ,�п��ܳ���.������Ҫ������.
//DEL 		}
//DEL 		try
//DEL 		{
//DEL 			sql.Format(_T("UPDATE ppp SET EnginID='%s'"),strProID);
//DEL 			theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
//DEL 		}
//DEL 		catch(_com_error)
//DEL 		{
//DEL 			//��û������ʱ,�п��ܳ���.������Ҫ������.
//DEL 		}
//DEL 		try
//DEL 		{		
//DEL 			//����ǰɾ����ǰ��Ŀ���ݿ�����뵼�빤����ص����м�¼
//DEL 			sql.Format(_T("DELETE * FROM %s WHERE EnginID='%s'"),strTableName,strProID);
//DEL 			theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
//DEL 		}
//DEL 		catch(_com_error)
//DEL 		{
//DEL 			//��û������ʱ,�п��ܳ���.������Ҫ������.
//DEL 		}
//DEL 
//DEL 		//�ٲ���
//DEL 		sql.Format(_T("insert into %s select * from ppp"),strTableName);
//DEL 		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
//DEL 
//DEL 		sql=_T("drop table ppp");
//DEL 		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
//DEL 	}
//DEL 	catch(_com_error &e)
//DEL 	{
//DEL 		ReportExceptionError(e);
//DEL 		throw;
//DEL 	}
//DEL 
//DEL 	return true;
//DEL }

bool InputOtherData::ImportTable_a_dir(_ConnectionPtr pcon_source, CString strProID)
{
	CString sql;

	try
	{
		theApp.m_pConnection->Execute(_T("drop table ppp"),NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		if(e.Error()!=DB_E_NOTABLE)
		{
			ReportExceptionError(e);
			return false;
		}
	}
 
	sql.Format(_T("SELECT * INTO ppp FROM a_dir IN '%s'[%s]"),strPathProID,strConnFeature[iCONN].strCONN);
	theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);

	try{
		sql=_T("alter table ppp add column EnginID varchar(100)");
		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
	}
	catch(_com_error )
	{
	}
	try{
		sql=_T("alter table ppp add column SJHYID long");
		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
	}
	catch(_com_error )
	{
	}
	try{
		sql=_T("alter table ppp add column SJJDID long");
		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
	}
	catch(_com_error )
	{
	}
	try{
		sql=_T("alter table ppp add column ZYID long");
		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
	}
	catch(_com_error )
	{
	}


	//����ǰ���µ�ǰ��Ŀ���ݿ�����뵼�빤����ص����м�¼
	try   
	{
		sql.Format(_T("UPDATE ppp SET EnginID='%s',SJHYID=0,SJJDID=4,ZYID=3"),strProID);
		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
	}
	catch(_com_error )
	{
		//��û������ʱ,�п��ܳ���.������Ҫ������.
	}
	//����ǰɾ����ǰ��Ŀ���ݿ�����뵼�빤����ص����м�¼
	try   
	{
		sql.Format(_T("DELETE FROM Volume WHERE EnginID='%s' AND SJHYID=0 AND SJJDID=4 AND ZYID=3"),strProID);
		theApp.m_pConAllPrj->Execute(_bstr_t(sql),NULL,adCmdText);
	}
	catch(_com_error )
	{
		//��û������ʱ,�п��ܳ���.������Ҫ������.
	}
	//�ٲ���
//	sql.Format("insert into %s select * from ppp",strTableName);
//	theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);

	_RecordsetPtr  m_pRecordset,m_pRecordset1;

	m_pRecordset.CreateInstance(_uuidof(Recordset));
	m_pRecordset1.CreateInstance(_uuidof(Recordset));

	m_pRecordset->Open(_T("select * from ppp"),
		               theApp.m_pConnection.GetInterfacePtr(),
					   adOpenDynamic,
					   adLockOptimistic,
					   adCmdText);

	m_pRecordset1->Open(_T("select * from Volume order by VolumeID"),
		               theApp.m_pConAllPrj.GetInterfacePtr(),
					   adOpenDynamic,
					   adLockOptimistic,
					   adCmdText);

	m_pRecordset->MoveFirst();

	CString vol;
	_variant_t var_vol;
	long id;

	m_pRecordset1->MoveLast();
	id=(m_pRecordset1->GetCollect("VolumeID"));

	while(!m_pRecordset->adoEOF)
	{
		var_vol=m_pRecordset->GetCollect("DIR_VOL");

		vol = vtos(var_vol);
		vol=vol.Right(4);
		if( !vol.IsEmpty() )  //ֻ������Ų�Ϊ�յļ�¼.
		{
			id++;
			try
			{
				m_pRecordset1->AddNew();
				m_pRecordset1->PutCollect("EnginID",m_pRecordset->GetCollect("EnginID"));
				m_pRecordset1->PutCollect("jcdm",_variant_t(vol));
				m_pRecordset1->PutCollect("jcmc",m_pRecordset->GetCollect("DIR_NAME"));
				m_pRecordset1->PutCollect("SJHYID",m_pRecordset->GetCollect("SJHYID"));
				m_pRecordset1->PutCollect("SJJDID",m_pRecordset->GetCollect("SJJDID"));
				m_pRecordset1->PutCollect("ZYID",m_pRecordset->GetCollect("ZYID"));
				m_pRecordset1->PutCollect("VolumeID",_variant_t(id));

				m_pRecordset1->Update();
			}
			catch(_com_error &e)
			{
	//			AfxMessageBox(e.Description());
				ReportExceptionError(e);
				return false;
			}
		}
		m_pRecordset->MoveNext();
	}

	m_pRecordset->Close();
//	m_pRecordset=NULL;
	try
	{    
		sql=_T("INSERT INTO A_DIR SELECT DIR_VOL,DIR_NAME,DIR_BAK,EnginID FROM PPP");
		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);

		sql=_T("drop table ppp");
		theApp.m_pConnection->Execute(_bstr_t(sql),NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return false;
	}

	return true;
}

bool InputOtherData::ImportTable_abnormal(_ConnectionPtr pcon_source,CString strTableName, CString strProID)
{
	_RecordsetPtr  m_pRecord_foxpro,m_pRecord_access;
	CString sql;
	CString sqlID;//�洢��ID�ֶεĲ�ѯ
	CString sqlID1;//�洢��ID1�ֶεĲ�ѯ
	//long lFoxRecNums;//Դ��Ҫ�����¼��

	m_pRecord_foxpro.CreateInstance(_uuidof(Recordset));
	m_pRecord_access.CreateInstance(_uuidof(Recordset));

	try
	{
		sql.Format("select * from %s WHERE EnginID='%s'",strTableName,strProID);
		sqlID.Format("select * from %s WHERE EnginID='%s' ORDER BY ID",strTableName,strProID);
		sqlID1.Format("select * from %s WHERE EnginID='%s' ORDER BY ID1",strTableName,strProID);
		m_pRecord_foxpro->Open(_variant_t(sql),
			pcon_source.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error e1)
	{
		sql.Format("select * from %s",strTableName);
		sqlID.Format("select * from %s ORDER BY ID",strTableName);
		sqlID1.Format("select * from %s ORDER BY ID1",strTableName);
	}
	try
	{
		if(adStateOpen==m_pRecord_foxpro->State)
			m_pRecord_foxpro->Close();
		m_pRecord_foxpro->Open(_variant_t(sql),
			pcon_source.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		throw;
	}
	if(m_pRecord_foxpro->adoEOF && m_pRecord_foxpro->BOF)
	{
		CString nulltable;

		nulltable.Format("���� %s �ı�%s Ϊ�գ�",strProID,strTableName);
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,nulltable,true);
		//AfxMessageBox(nulltable);

		return false;
	}
	else
	{
		/*
		try{
			m_pRecord_foxpro->MoveLast();
		}
		catch(...)
		{			
		}
		try{
			lFoxRecNums=m_pRecord_foxpro->GetRecordCount();
		}
		catch(...)
		{			
		}*/
		try{
			m_pRecord_foxpro->MoveFirst();
		}
		catch(...)
		{			
		}
	}
	
	try
	{
		//ɾ�������Ѿ����ڸù��̵ļ�¼.
		theApp.m_pConnection->Execute(_bstr_t("DELETE FROM ["+strTableName+"] WHERE EnginID='"+strProID+"' "),
										NULL, adCmdText);
	}
	catch(_com_error)
	{
		//��û������ʱ,�п��ܳ���.������Ҫ������.
	}

	m_pRecord_access->Open(_variant_t(sql),
		                   theApp.m_pConnection.GetInterfacePtr(),
						   adOpenDynamic,
						   adLockOptimistic,
						   adCmdText);

 	long Fieldcount;			//Fox���е��ֶθ���
	long FieldCountAcc;			//ACCESS���е��ֶθ���

	long access_index,foxpro_index,index,have_id;
	CString FieldName;			//ACCESS���е��ֶ�����
	CString	FieldNameFox;		//Fox���е��ֶ�����

	//ACCESS���е��ֶθ���
	FieldCountAcc = m_pRecord_access->GetFields()->GetCount();
	//Fox���е��ֶθ���
	Fieldcount=m_pRecord_foxpro->GetFields()->GetCount();

	index=1;
	have_id=0;

	_variant_t vv;
	CString strTemp;
	bool	bIsFirst = true;		//����.��һ����¼
	CString *strFieldArr;
	strFieldArr = new	CString[Fieldcount];

	
	try
	{
		//��1�����ҳ�Դ��(Foxpro��)��Ŀ�ı�(ACCESS��)��ͬһ�������ͬ�ֶΣ���ŵ�strFieldArr[Index]
		for(long Index=0; Index < Fieldcount; Index++)
		{
			FieldNameFox = (LPCSTR)m_pRecord_foxpro->GetFields()->GetItem(_variant_t(Index))->GetName();
			//��Access���дӵ�һ���ֶ����²��ҡ�
			for (long pos=0; pos < FieldCountAcc; pos++)
			{
				FieldName = (LPCSTR)m_pRecord_access->GetFields()->GetItem(_variant_t(pos))->GetName();
				if(!FieldName.CompareNoCase("ID"))
					have_id=1;
				else if(!FieldName.CompareNoCase("ID1"))
					have_id=2;
				if ( !FieldName.CompareNoCase(FieldNameFox) )
				{
					//����������ͬ���ֶΣ����¡�								
					strFieldArr[Index] = FieldName;
					break;
				}
			}
		}

		long lID=0;
		if(1==have_id)
			lID=GetMaxValue(sqlID,theApp.m_pConnection,"ID");
		else if(2==have_id)
			lID=GetMaxValue(sqlID1,theApp.m_pConnection,"ID1");

		//��2������Դ��(Foxpro��)�ļ�¼���ֶ�strFieldArr[Index]��ֵ���Ƶ�Ŀ�ı�(ACCESS��)��ͬһ�������ͬ�ֶ�
		access_index=0;
		while(!m_pRecord_foxpro->adoEOF)
		{
			foxpro_index=0;
			
			m_pRecord_access->AddNew();
			
			while(foxpro_index<Fieldcount)
			{
				FieldName = strFieldArr[foxpro_index];
				if ( FieldName.IsEmpty() )
				{
					//��������û����ͬ���ֶΡ�
					continue;
				}
				vv=m_pRecord_foxpro->GetCollect(_variant_t( FieldName ));

				if(vv.vt==VT_BSTR)
				{
					strTemp=(LPCTSTR)(_bstr_t)vv;
					strTemp.TrimLeft();
					strTemp.TrimRight();

					if(strTemp.IsEmpty())
					{
						vv.Clear();
						m_pRecord_access->PutCollect(_variant_t( FieldName ),vv);
					}
					else
					{
						m_pRecord_access->PutCollect(_variant_t( FieldName ),_variant_t(strTemp));
					}
				}
				else
				{
					m_pRecord_access->PutCollect(_variant_t( FieldName ),vv);
				}

				foxpro_index++;
			}

			try
			{
				if(1==have_id)
					m_pRecord_access->PutCollect("ID",_variant_t(lID+access_index));
				else if(2==have_id)
					m_pRecord_access->PutCollect("ID1",_variant_t(lID+access_index));
				m_pRecord_access->PutCollect("EnginID",_variant_t(strProID));
				m_pRecord_access->Update();
			}
			catch(_com_error &e)
			{
//				AfxMessageBox(e.Description());
				ReportExceptionError(e);
				delete [] strFieldArr;
				throw;
			}
			access_index++;

			CString strMsg;
			strMsg.Format("���ڵ���%15s����%15s���%#6d�м�¼",strProID,strTableName,access_index);
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,strMsg,true);
			m_pRecord_foxpro->MoveNext();
		}
	}
	catch(_com_error &e)
	{
//		AfxMessageBox(e.Description());
		ReportExceptionError(e);
		delete [] strFieldArr;
		throw;
	}
	delete [] strFieldArr;

	m_pRecord_foxpro->Close();
	m_pRecord_access->Close();

	return true;

}


/////////////////////////////////////////////////////////////////
//
// ����Volume��throw(_com_error);
//
// szProjectName[in]	���̵�ID��
//
void InputOtherData::UpdateVolumeTable(LPCTSTR szProjectName)
{
	HRESULT hr;
	long lVolueId;
	CString strProjectName,SQL;
	_variant_t varTemp;
	_RecordsetPtr IRecordset_Vol;
	
	if(szProjectName==NULL)
	{
		_com_error e(E_INVALIDARG);
		ReportExceptionError(e);
		throw;
	}

	strProjectName=szProjectName;
	strProjectName.TrimLeft();
	strProjectName.TrimRight();//�޼��հ��ַ���

	if(strProjectName.IsEmpty())
	{
		_com_error e(E_INVALIDARG);
		ReportExceptionError(_T("������Ϊ��"));
		throw;
	}

	hr=IRecordset_Vol.CreateInstance(__uuidof(Recordset));

	if(FAILED(hr))
	{
		_com_error e(hr);
		ReportExceptionError(e);
		throw;
	}

	//ɾ����ʱ��
	try
	{
		theApp.m_pConAllPrj->Execute(_bstr_t("DROP TABLE TemporarilyTable1"),NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		if(e.Error()!=DB_E_NOTABLE)
		{
			ReportExceptionError(e);
			throw;
		}
	}

	try
	{
		//������ʱ��,��Volume��EnginIDΪ�յ����ݷ�����ʱ����
		theApp.m_pConAllPrj->Execute(_bstr_t("SELECT * INTO TemporarilyTable1 FROM Volume WHERE EnginID is null"),
									  NULL,adCmdText);
		//mark_hubb_xx�������������������ڡ�ϵͳ��ΪTemporarilyTable1 ���Ѿ����ڡ�

		lVolueId=GetMaxValue("SELECT * FROM volume ORDER BY VolumeID ASC",theApp.m_pConAllPrj,"VolumeID");

		//����ʱ���е����ݸ��º�Ż�VolueID
		SQL.Format(_T("UPDATE TemporarilyTable1 SET EnginID='%s',VolumeID=%d"),strProjectName,lVolueId);
		
		theApp.m_pConAllPrj->Execute(_bstr_t(SQL),NULL,adCmdText);

		theApp.m_pConAllPrj->Execute(_bstr_t("INSERT INTO Volume SELECT * FROM TemporarilyTable1"),
									  NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		throw;
	}

	//ɾ����ʱ��
	try
	{
		theApp.m_pConAllPrj->Execute(_bstr_t("DROP TABLE TemporarilyTable1"),NULL,adCmdText);
	}
	catch(_com_error &e)
	{
		if(e.Error()!=DB_E_NOTABLE)
		{
			ReportExceptionError(e);
			throw;
		}
	}

}
//////////////////////////////////
//�����ݿ�IPEDSort.mdb��ImportTblINFO�ֶ�TableName��ȡ��Ҫ����ı���.
//
bool InputOtherData::GetImportTblNames(CString *&pTableName, int &nTblCount, CString pNotTbl[], int nNotCount)
{
	_RecordsetPtr pRsTblINFO;
	CString sql, strTmpTbl;
	bool	bTblExis=false;
	pRsTblINFO.CreateInstance(__uuidof(Recordset));
	try
	{
		//��Ҫ����ı��������ݿ��ж�ȡ.
		sql = "SELECT * FROM [ImportTblINFO] WHERE Foxpro=TRUE ORDER BY TableName DESC";
		pRsTblINFO->Open(_bstr_t(sql), theApp.m_pConIPEDsort.GetInterfacePtr(),
				adOpenStatic, adLockOptimistic, adCmdText);
		nTblCount = pRsTblINFO->GetRecordCount();
		if( nTblCount <= 0 )
		{
			return FALSE;		//û��Ҫ����ı�
		}
		pTableName = new CString[nTblCount];

		//ѭ��ȡ����������������������⴦�������뵽pTableName;
		for(nTblCount=0; !pRsTblINFO->adoEOF; pRsTblINFO->MoveNext() )
		{
			strTmpTbl = vtos(pRsTblINFO->GetCollect(_variant_t("TableName")));
			strTmpTbl.MakeUpper();
			bTblExis = false;		
			for(int i=0; i<nNotCount; i++)
			{
				if( !strTmpTbl.CompareNoCase(pNotTbl[i]) )
				{
					bTblExis = true;
					break;
				}
			}
			if( !bTblExis )		
			{
				//����Ҫ�����⴦��
				pTableName[nTblCount] = strTmpTbl;
				nTblCount++;			//��������1.
			}
		}
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return FALSE;
	}
	return TRUE;
}

bool InputOtherData::PutFieldValue(_RecordsetPtr pSource, _RecordsetPtr pTarget, CString strFieldName)
{//��Դ���¼�ֶ�strFieldName��ֵ���Ƶ�Ŀ�ı��Ӧ��¼�ֶ�
	if ( strFieldName.IsEmpty() )
	{
		return false;
	}
	_variant_t vv;
	CString strTemp;
	vv=pSource->GetCollect(_variant_t( strFieldName ));
	
	if(vv.vt==VT_BSTR)
	{
		strTemp=(LPCTSTR)(_bstr_t)vv;
		strTemp.TrimLeft();
		strTemp.TrimRight();
		
		if(strTemp.IsEmpty())
		{
			vv.Clear();
			pTarget->PutCollect(_variant_t( strFieldName ),vv);
			
		}
		else
		{
			pTarget->PutCollect(_variant_t( strFieldName ),_variant_t(strTemp));
		}
	}
	else
	{
		pTarget->PutCollect(_variant_t( strFieldName ),vv);
	}
	return false;
}

bool InputOtherData::Have_Table(_ConnectionPtr m_pCon_x, CString str_x)
{
//	AfxMessageBox( _T("�жϱ�"+str_x+"�Ƿ����") );
	_RecordsetPtr m_pRec_x;
	//CVtot turn;
	m_pRec_x=m_pCon_x->OpenSchema(adSchemaTables);
	CString TempStr;
	for(;!m_pRec_x->adoEOF;m_pRec_x->MoveNext())
	{
		TempStr=(LPCSTR)_bstr_t(  m_pRec_x->GetCollect("TABLE_NAME")   );
		if(TempStr==str_x)
			return true;
	}
	return false;

}

bool InputOtherData::MeetRequirement(CString AllPath)
{
	return true;
}

long InputOtherData::GetMaxValue(CString SQL, _ConnectionPtr pConn, CString strFieldName)
{//�ҳ������ֶ�strFieldName�����ֵ
	_RecordsetPtr pRs;
	pRs.CreateInstance(__uuidof(Recordset));
	_variant_t varTemp;
	long lVolueId=0;
	try
	{
		pRs->Open(_variant_t(SQL),
								 pConn.GetInterfacePtr(),
								 adOpenDynamic,adLockOptimistic,adCmdText);
		if(pRs->adoEOF && pRs->BOF)
		{
			lVolueId=1;
		}
		else
		{
			pRs->MoveLast();
			varTemp=pRs->GetCollect(_variant_t(strFieldName));
			
			lVolueId=varTemp;
			lVolueId++;
		}
		pRs->Close();
		pRs=NULL;
		return lVolueId;
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return -1;
	}	
}
