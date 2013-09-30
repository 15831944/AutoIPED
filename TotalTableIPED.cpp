// TotalTableIPED.cpp: implementation of the CTotalTableIPED class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TotalTableIPED.h"
#include "edibgbl.h"
#include "Mainfrm.h"
#include "vtot.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define	 pi			3.1415927

CTotalTableIPED::CTotalTableIPED()
{

}

CTotalTableIPED::~CTotalTableIPED()
{

}

//************************************
//* ģ �� ��: c_paicol()	         *
//* ��    ��: ����������ϻ��ܱ�     *
//* �ϼ�ģ��: MAIN_MENU              *
//* �¼�ģ��:  modC_RING()           *
//* �� �� ��: 1��paint_c,            *
//*           2��e_paicol,           *
//* �޸�����:                        *
//************************************
BOOL CTotalTableIPED::c_paicol()
{
	_RecordsetPtr paicol_set;
	paicol_set.CreateInstance(__uuidof(Recordset));
	_variant_t key;
	CString str;
	int pos=0;

	if(paicol_set->State==adStateOpen)
	{
		paicol_set->Close();
	}
	if(!modC_RING())//9/2
	{
		return false;
	}
	while(pos<10)
	{
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
	}

	CString strSQL; //*1

	strSQL="delete from e_paicol where enginid='"+EDIBgbl::SelPrjID+"'";
	_bstr_t sql;
	sql=strSQL;
	//��ձ�e_paicol�е����м�¼,�˱���������������ϱ�Ľ��
	try
	{
	m_Pconnection->Execute(sql,NULL,adCmdText);
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	} //*1

	//�Ա�paint_c��Ԥ����,������ǶԸñ���л��ܴ���
	try
	{
		
		strSQL="update paint_c set ������=' ',�������=0,��ϼ�����=0 where((pai_c_face='N') or (pai_c_face='n') and enginid='"+EDIBgbl::SelPrjID+"')"; //*2
		m_Pconnection->Execute((_bstr_t)strSQL,NULL,adCmdText);//*2
		
		//�Ա�paint_c������Ӧ���޸�
		if(InitPaint_c()==false)	//*3 //*3
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
			return false;
		}
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);

		return	false;
	}
	while(pos<20)
	{
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
	}

	//�Ա�paint_c���л���
	try
	{
		if(paicol_set->State==adStateOpen)
		{
			paicol_set->Close();
		}
		_RecordsetPtr TargetSet;//Ŀ���¼��
		TargetSet.CreateInstance(__uuidof(Recordset));	//*4
		TargetSet->Open(_bstr_t(_T("select * from e_paicol")),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);

		m_strPaint_c="col_name";
		m_valPaint_c="col_amount";

		//�Ե������ͳ��1
		strSQL="select * from paint_c where enginid='"+EDIBgbl::SelPrjID+"' order by ������ desc";
		sql=strSQL;
		paicol_set->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		TotalTable(paicol_set,TargetSet,"������","�׺ϼ�����"); //*4
		while(pos<30)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}

		//���������ͳ��2 //*5
		strSQL="select * from paint_c where enginid='"+EDIBgbl::SelPrjID+"' order by ������ desc";
		if(paicol_set->State==adStateOpen)
		{
			paicol_set->Close();
		}
		sql=strSQL;
		paicol_set->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		TotalTable(paicol_set,TargetSet,"������","��ϼ�����"); //*5
		while(pos<40)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}

		//��pai_r_colr����ͳ��5  //*9
		strSQL="select * from paint_c where enginid='"+EDIBgbl::SelPrjID+"' order by pai_r_colr";
		sql=strSQL;
		if(paicol_set->State==adStateOpen)
		{
			paicol_set->Close();
		}
		paicol_set->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		TotalTable(paicol_set,TargetSet,"pai_r_colr","pai_r_t");
		while(pos<50)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}   //*9

		//��pai_a_colr����ͳ��6
		strSQL="select * from paint_c where enginid='"+EDIBgbl::SelPrjID+"' order by pai_a_colr";
		sql=strSQL;
		if(paicol_set->State==adStateOpen)  //*10
		{
			paicol_set->Close();
		}
		paicol_set->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		TotalTable(paicol_set,TargetSet,"pai_a_colr","pai_a_t");
		while(pos<60)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}   

		//8/26
		//���ֶ�:col_unit���и�ֵ
		strSQL="update e_paicol set col_unit='kg' where enginid='"+EDIBgbl::SelPrjID+"'";
		sql=strSQL;
		m_Pconnection->Execute(sql,NULL,adCmdText);
		//8/26
		//��pai_a1����ͳ��3
		strSQL="select * from paint_c where enginid='"+EDIBgbl::SelPrjID+"' order by pai_a1 desc";
		sql=strSQL;
		if(paicol_set->State==adStateOpen)//*7
		{
			paicol_set->Close();
		}
		paicol_set->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		TotalTable(paicol_set,TargetSet,"pai_a1","pai_a_c1");
		while(pos<70)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}//*7


		//��pai_a2����ͳ��4
		strSQL="select * from paint_c where enginid='"+EDIBgbl::SelPrjID+"' order by pai_a2 desc";
		sql=strSQL;
		if(paicol_set->State==adStateOpen)
		{
			paicol_set->Close();
		}
		paicol_set->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		TotalTable(paicol_set,TargetSet,"pai_a2","pai_a_c2");

		while(pos<80)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}
		// ��������ܳ��������ٽ���һ�λ��ܣ���Ϊ��ͬ�����Ϳ��ܻ�ʹ����ͬ�����ᣩ zsy [2005/12/23]
		TotalSelfTable();
		
		//8/26���ֶ�col_unit�и�ֵ
		strSQL="update e_paicol set col_unit='m2' where enginid='"+EDIBgbl::SelPrjID+"' and col_unit is NULL";
		sql=strSQL;
		m_Pconnection->Execute(sql,NULL,adCmdText);

		//ɾ������ֵΪ��ļ�¼
		strSQL="delete from e_paicol where enginid='"+EDIBgbl::SelPrjID+"' and col_amount=0";
		sql=strSQL;
		m_Pconnection->Execute(sql,NULL,adCmdText);
		while(pos<100)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}

		//8/26
		//8/28
//		AfxMessageBox("����������ϻ��ܱ�ɹ�!");//10/13

		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);		//8/28
		
	}catch(_com_error e)
	{
		AfxMessageBox("����������ϻ��ܱ�ʧ��!");
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
		return false;
	}

	//9/18
	//�Ա�pass ���д���
	_RecordsetPtr pass_set;
	int flagPass=0;
	pass_set.CreateInstance(__uuidof(Recordset));
	pass_set->Open(_bstr_t(_T("select * from pass where enginid='"+EDIBgbl::SelPrjID+"'")),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
	if(pass_set->GetRecordCount()>0)
	{
		pass_set->MoveFirst();
		for(;!pass_set->adoEOF;pass_set->MoveNext())
		{
			if(flagPass==0)
			{
				pass_set->PutCollect(_T("pass_mark2"),_T("T"));
			}
			else
			{
				pass_set->PutCollect(_T("pass_mark2"),_T("F"));
			}
			key=pass_set->GetCollect(_T("pass_mod2"));
			if((!(key.vt==VT_NULL||key.vt==VT_EMPTY))&&(flagPass==0))
			{
				str=key.bstrVal;
				flagPass=(str.Compare("C_PAICOL")==0)?1:0;
			}
		}
	}	

	//9/18

	return TRUE;
}

//ɾ���ַ�����ǰ��ո��
CString CTotalTableIPED::Trim(LPCTSTR pcs)
{
	CString s = pcs;
	s.TrimLeft();
	s.TrimRight();
	return s;
	
}

//�Ա�:paint_t������Ӧ���޸�
BOOL CTotalTableIPED::InitPaint_c()
{
	_RecordsetPtr Paint_cSet;
	_RecordsetPtr pRsRing_T;
	pRsRing_T.CreateInstance(__uuidof(Recordset));
	Paint_cSet.CreateInstance(__uuidof(Recordset));
	_variant_t key;
	CString strRingPaintName;
	CString str;
	CString strTemp;
	try
	{
		//��ɫ����ͷ������Ͽ⣬ȡ���������ơ�
		pRsRing_T->Open(_variant_t("SELECT * FROM [a_ring_paint]"), m_pConnectionCODE.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		if (pRsRing_T->adoEOF && pRsRing_T->BOF)
		{
			strRingPaintName = "������";		//ʼ��û�м�¼ʱĬ��Ϊ"������".
		}
		else
		{
			pRsRing_T->MoveFirst();
			strRingPaintName = vtos(pRsRing_T->GetCollect(_variant_t("paint_name")));
			if (strRingPaintName.IsEmpty())
			{
				strRingPaintName = "������";		//ȡ������һ�����ַ���ʱ����Ĭ��ֵ
			}
		}
		Paint_cSet->Open(_bstr_t(_T("select * from paint_c where enginid='"+EDIBgbl::SelPrjID+"'")),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		if(Paint_cSet->GetRecordCount()==0)
		{
			AfxMessageBox(" ��ʾ:û��������ϱ��ԭʼ��¼!");
			return false;
		}
		for(;!Paint_cSet->adoEOF;Paint_cSet->MoveNext())
		{//4
			key=Paint_cSet->GetCollect(_T("pai_c_face"));
			//�޸�������
			if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
			{//3
				str=key.bstrVal;
				if(!((str=="n")||(str=="N")))
				{//2
					key=Paint_cSet->GetCollect(_T("������"));
					if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
					{//1
					strTemp=key.bstrVal;
					 strTemp=Trim(strTemp);
					str=""+strTemp+"("+str+")";
					Paint_cSet->PutCollect(_T("������"),_variant_t(str));
					}//1
				}//2
			}//3


			key=Paint_cSet->GetCollect(_T("pai_r_colr"));
			//�޸��ֶ�pai_r_colr��ֵ
			if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
			{//2
				str=key.bstrVal;
				if(!(str=="n"||str=="N"))
				{//1
					str=strRingPaintName+"("+str+")";
					Paint_cSet->PutCollect(_T("pai_r_colr"),_variant_t(str));
				}//1
				else if(str=="n"||str=="N")
				{
					str=" ";
					Paint_cSet->PutCollect(_T("pai_r_colr"),_variant_t(str));
				}
			}//2

			//�޸��ֶ�pai_a_colr�е�ֵ
			key=Paint_cSet->GetCollect(_T("pai_a_colr"));
			if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
			{//2
				str=key.bstrVal;
				if(!(str=="n"||str=="N"))
				{//1
				str=strRingPaintName+"("+str+")";
				Paint_cSet->PutCollect(_T("pai_a_colr"),_variant_t(str));
				}//1
				else if(str=="n"||str=="N")
				{
					str=" ";
					Paint_cSet->PutCollect(_T("Pai_a_colr"),_variant_t(str));
				}
			}//2
			Paint_cSet->Update();
		}//4

	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;

}


//�Ը������ֶ�ֵ���л���,����ͨ�õĻ��ܺ���
//Դ��¼��SourceSet���밴�ֶ�source_str��������
BOOL CTotalTableIPED::TotalTable(_RecordsetPtr SourceSet,_RecordsetPtr TargetSet,CString source_str,CString source_val)
{
	SourceSet->MoveFirst();
	double totalval=0.0;
	double temptotal=0.0;
	CString strTemp;
	CString last_str;
	last_str="";//����ʼֵ
	int flag=0;
	_variant_t key;
	try
	{
		for(;!SourceSet->adoEOF;SourceSet->MoveNext())
		{//4

			key=SourceSet->GetCollect(_variant_t(source_str));//��Դ��¼����ȡֵ
			if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
			{//3
				strTemp=key.bstrVal;
				strTemp=Trim(strTemp);

				key=SourceSet->GetCollect(_variant_t(source_val));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{//1
					temptotal=key.dblVal;
				}//1
				else 
				{//1
					temptotal=0.0;
				}//1

				//��������SourceSet��¼���е��ֶ�(source_str)ֵ�Ƿ����
				if(!(strTemp.Compare(last_str)==0))//�����
				{//2
					
					//�������ǵ�һ��,�ڻ��ܱ�������һ����¼
					if(flag!=0&&last_str!="")
					{//1
						TargetSet->AddNew();
						TargetSet->PutCollect(_T(_variant_t(m_strPaint_c)),_variant_t(last_str));
						TargetSet->PutCollect(_T(_variant_t(m_valPaint_c)),_variant_t(totalval));
						TargetSet->PutCollect(_T("enginid"),_variant_t(EDIBgbl::SelPrjID));//8/25
						TargetSet->Update();
					}//1
					last_str=strTemp;
					totalval=0.0;
				}//2
				flag=1;
				totalval+=temptotal;//���

			}//3û��ȡ��ֵ
			else 
			{
				if(flag!=0&&last_str!="")
				{
					TargetSet->AddNew();
					TargetSet->PutCollect(_T(_variant_t(m_strPaint_c)),_variant_t(last_str));
					TargetSet->PutCollect(_T(_variant_t(m_valPaint_c)),_variant_t(totalval));
					TargetSet->PutCollect(_T("enginid"),_variant_t(EDIBgbl::SelPrjID));//8/25
					TargetSet->Update();
					totalval=0.0;
				}
				last_str="";
			}
		}//4


		//���˼�¼β��Ҫ����һ����¼
		if(last_str!="")
		{
			TargetSet->AddNew();
			TargetSet->PutCollect(_T(_variant_t(m_strPaint_c)),_variant_t(last_str));
			TargetSet->PutCollect(_T(_variant_t(m_valPaint_c)),_variant_t(totalval));
			TargetSet->PutCollect(_T("enginid"),_variant_t(EDIBgbl::SelPrjID));
			TargetSet->Update();
		
		}

	}catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}


//************************************
//* ģ �� ��: c_presiz()			 *
//* ��    ��: ���ɱ��²��Ϲ����ܱ� *
//* �ϼ�ģ��: MAIN_MENU              *
//* �¼�ģ��:                        *
//* �� �� ��: 1��e_presiz,           *
//*              pre_calc,col        *
//* �޸�����:                        *
//************************************

BOOL CTotalTableIPED::c_presiz()
{
	int pos=1;
	_RecordsetPtr presiz_set;
	presiz_set.CreateInstance(__uuidof(Recordset));
	_variant_t key;
	CString str;
	try
	{

		//��ձ�e_presiz�е����м�¼,�ñ�Ϊ���²��ϻ��ܱ�(���)�Ľ����
		CString strSQL;
		_bstr_t sql;
		strSQL="delete from e_presiz where enginid='"+EDIBgbl::SelPrjID+"'";//8/27
		sql=strSQL;
		m_Pconnection->Execute(sql,NULL,adCmdText);
		while(pos<30)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}
		//�Ա�e_presiz����Ԥ����
		if(InitPresiz()==false)
		{
			AfxMessageBox("���ɱ��²��ϻ��ܱ�û�гɹ�!");
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
			return false;
		}
		while(pos<60)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}
		//�Ա��²��Ͻ��л���8/28
		_RecordsetPtr sourceSet;
		sourceSet.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr TargetSet;
		TargetSet.CreateInstance(__uuidof(Recordset));
		sourceSet->Open(_bstr_t(_T("select * from temp_presiz order by size_key")),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		TargetSet->Open(_bstr_t(_T("select * from e_presiz")),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		while(pos<80)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}

		if(totalPresiz(sourceSet,TargetSet)==false)
		{
			AfxMessageBox("���ɱ��²��ϻ��ܱ�û�гɹ�!");
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
			return false;
		}
		else
		{
			while(pos<100)
			{
				((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
			}
//			AfxMessageBox("���ɱ��²��ϻ��ܱ�ɹ�!");//10/13
		}
		//8/28
		//�Ա�pass ���д���
		_RecordsetPtr pass_set;
		int flagPass=0;
		pass_set.CreateInstance(__uuidof(Recordset));
		pass_set->Open(_bstr_t(_T("select * from pass where enginid='"+EDIBgbl::SelPrjID+"'")),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		if(pass_set->GetRecordCount()>0)
		{
			pass_set->MoveFirst();
			for(;!pass_set->adoEOF;pass_set->MoveNext())
			{
				if(flagPass==0)
				{
					pass_set->PutCollect(_T("pass_mark1"),_T("T"));
				}
				else
				{
					pass_set->PutCollect(_T("pass_mark1"),_T("F"));
				}
				key=pass_set->GetCollect(_T("pass_mod1"));
				if((!(key.vt==VT_NULL||key.vt==VT_EMPTY))&&(flagPass==0))
				{
					str=key.bstrVal;
					flagPass=(str.Compare("C_PRESIZ")==0)?1:0;
				}
			}
		}
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
		return false;
	}
	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);

	return true;
}


//��ʼ����:presiz 
BOOL CTotalTableIPED::InitPresiz()
{
	_RecordsetPtr presiz_set;
	CString strSQL;
	_bstr_t sql;
	CString str;
	_variant_t key;
	double val;

	presiz_set.CreateInstance(__uuidof(Recordset));
	try
	{
		//�жϱ�temp_presiz�Ƿ����
		if(tableExists(m_Pconnection,"temp_presiz")==false)
		{
			//������temp_presiz
			strSQL="create table temp_presiz (id long,size_num double,��� double,�Ᵽ�²��� char,�Ᵽ�º� double,c_pre_wei double,c_pipe_wei double,";
			strSQL=strSQL+"���� double,size_amoun double,size_unit char,�ⵥ��� double,��ȫ��� double,size_key char,size_mark char,�ڱ��²��� char,�ڱ��º� double,EnginID char)";
			sql=strSQL;

			m_Pconnection->Execute(sql,NULL,adCmdText);
		}
		else
		{
			//��ձ�temp_presiz
			strSQL="delete from temp_presiz";
			sql=strSQL;
			m_Pconnection->Execute(sql,NULL,adCmdText);

		}
		

		//����:e_preexp�е����ݵ��뵽��:temp_presiz
		strSQL="insert into temp_presiz(id,���,�Ᵽ�²���,�Ᵽ�º�,c_pre_wei,c_pipe_wei,";
		strSQL=strSQL+"����,�ⵥ���,��ȫ���,�ڱ��²���,�ڱ��º�,EnginID )";
		strSQL=strSQL+"  select id,���,�Ᵽ�²���,�Ᵽ�º�,c_pre_wei,c_pipe_wei , ";
		strSQL=strSQL+"����,�ⵥ���,��ȫ���,�ڱ��²���,�ڱ��º�,EnginID ";
		strSQL=strSQL+"from e_preexp where �ڱ��²��� is not NULL and EnginID='"+EDIBgbl::SelPrjID+"'";
		strSQL=strSQL+" and �ڱ��²���<>''";
		sql=strSQL;
		m_Pconnection->Execute(sql,NULL,adCmdText);

		strSQL="select * from temp_presiz";
		sql=strSQL;
		presiz_set->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);

		if(presiz_set->GetRecordCount()>0)
		{//3
			presiz_set->MoveFirst();
			int count=0;
			for(;!presiz_set->adoEOF;presiz_set->MoveNext())
			{//2
				key=presiz_set->GetCollect(_T("�ڱ��²���"));
				str=key.bstrVal;
				str=Trim(str);
				presiz_set->PutCollect(_T("�Ᵽ�²���"),_variant_t(str));
				key=presiz_set->GetCollect(_T("�ڱ��º�"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{//1
					val=key.dblVal;
				}//1
				else
				{//1
					val=0.0;
				}//1
				presiz_set->PutCollect(_T("�Ᵽ�º�"),_variant_t(val));
				val=0.0;
				presiz_set->PutCollect(_T("�ڱ��º�"),_variant_t(val));
				str="";
				presiz_set->PutCollect(_T("�ڱ��²���"),_variant_t(str));
				str="�ڱ��²�";
				presiz_set->PutCollect(_T("size_mark"),_variant_t(str));
				presiz_set->Update();
				count++;//ͳ�Ƽ�¼����

			}//2
		}//3


		//�ѱ�:e_preexp�е����м�¼����ӵ���e_presiz��
		strSQL="insert into e_presiz(id,���,�Ᵽ�²���,�Ᵽ�º�,c_pre_wei,c_pipe_wei,";
		strSQL=strSQL+"����,�ⵥ���,��ȫ���,�ڱ��²���,�ڱ��º�,EnginID )";
		strSQL=strSQL+"  select id,���,�Ᵽ�²���,�Ᵽ�º�,c_pre_wei,c_pipe_wei , ";
		strSQL=strSQL+"����,�ⵥ���,��ȫ���,�ڱ��²���,�ڱ��º�,EnginID ";
		strSQL=strSQL+"from e_preexp where  EnginID='"+EDIBgbl::SelPrjID+"' ";
		sql=strSQL;
		m_Pconnection->Execute(sql,NULL,adCmdText);

		if(presiz_set->State==adStateOpen)
		{
			presiz_set->Close();
		}
		
		//�Ա�e_presiz�еĵ�ǰ��¼�еĹ������޸�
		strSQL="select * from e_presiz	where enginid='"+EDIBgbl::SelPrjID+"'";
		sql=strSQL;

		presiz_set->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		if(presiz_set->GetRecordCount()>0)
		{//2
			double val1;

			presiz_set->MoveFirst();
			for(;!presiz_set->adoEOF;presiz_set->MoveNext())
			{//1
				//ȡ���
				key=presiz_set->GetCollect(_T("���"));
				val=(key.vt==VT_EMPTY||key.vt==VT_NULL)?0.0:key.dblVal;
				key=presiz_set->GetCollect(_T("�ڱ��º�"));
				val1=(key.vt==VT_EMPTY||key.vt==VT_NULL)?0.0:key.dblVal;
				val=val+2*val1;
				presiz_set->PutCollect(_T("���"),_variant_t(val));
				presiz_set->Update();
				
			}//1
		}//2

		//����e_presiz �ͱ�:temp_presiz������һ��
		strSQL="insert into temp_presiz (id ,size_num ,��� ,�Ᵽ�²��� ,�Ᵽ�º� ,c_pre_wei ,c_pipe_wei ,";
		strSQL=strSQL+"���� ,size_amoun ,size_unit ,�ⵥ��� ,��ȫ��� ,size_key ,size_mark ,�ڱ��²��� ,�ڱ��º� ,EnginID )";
		strSQL=strSQL+"select id ,size_num ,��� ,�Ᵽ�²��� ,�Ᵽ�º� ,c_pre_wei ,c_pipe_wei ,";
		strSQL=strSQL+"���� ,size_amoun ,size_unit ,�ⵥ��� ,��ȫ��� ,size_key ,size_mark ,�ڱ��²��� ,�ڱ��º� ,EnginID ";
		strSQL=strSQL+"from e_presiz where enginid='"+EDIBgbl::SelPrjID+"'";

		sql=strSQL;
		m_Pconnection->Execute(sql,NULL,adCmdText);
		//8/27
		strSQL="delete from e_presiz where enginid='"+EDIBgbl::SelPrjID+"'";
		sql=strSQL;
		m_Pconnection->Execute(sql,NULL,adCmdText);
		//8/27

		if(presiz_set->State==adStateOpen)
		{
			presiz_set->Close();
		}

		strSQL="select * from temp_presiz where enginid='"+EDIBgbl::SelPrjID+"'";
		sql=strSQL;
		presiz_set->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		if(presiz_set->GetRecordCount()>0)
		{//4
			presiz_set->MoveFirst();
			CString strTemp;
			double valTemp;

			for(;!presiz_set->adoEOF;presiz_set->MoveNext())
			{//3
				//�޸�size_key
				key=presiz_set->GetCollect(_T("�Ᵽ�²���"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{
					str=key.bstrVal;
				}
				else
				{
					str="";
				}
				str=Trim(str);
				key=presiz_set->GetCollect(_T("���"));
				val=(!(key.vt==VT_EMPTY||key.vt==VT_NULL))?key.dblVal:0.0;
				valTemp=val;
				strTemp.Format("%6.1f",val);
				str=str+strTemp;
				key=presiz_set->GetCollect(_T("�Ᵽ�º�"));
				val=(!(key.vt==VT_EMPTY||key.vt==VT_NULL))?key.dblVal:0.0;
				strTemp.Format("%4.0f",val);
				str=str+strTemp;
				presiz_set->PutCollect(_T("size_key"),_variant_t(str));

				//�޸�size_amount
				key=presiz_set->GetCollect(_T("����"));
				val=(!(key.vt==VT_EMPTY||key.vt==VT_NULL))?key.dblVal:0.0;
				presiz_set->PutCollect(_T("size_amoun"),_variant_t(val));

				//�޸�size_unit
				str="m3";
				presiz_set->PutCollect(_T("size_unit"),_variant_t(str));

				//�޸�size_mark
				if(valTemp>=2000)
				{//2
					key=presiz_set->GetCollect("size_mark");
					if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
					{//1
						str=key.bstrVal;
						str=Trim(str);
					}//1
					else
					{//1
						str="";
					}//1
					str=str+"�豸";
					presiz_set->PutCollect(_T("size_mark"),_variant_t(str));
				}//2	
				presiz_set->Update();

			}//3
		}//4
		if(presiz_set->State==adStateOpen)
		{
			presiz_set->Close();
		}

}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	
	return true;

}


//�жϸñ��Ƿ����
BOOL CTotalTableIPED::tableExists(_ConnectionPtr pCon, CString tbn)
{
	if(pCon==NULL || tbn=="")
		return false;
	_RecordsetPtr rs;
	if(tbn.Left(1)!="[")
		tbn="["+tbn;
	if(tbn.Right(1)!="]")
		tbn+="]";
	try{
		rs=pCon->Execute(_bstr_t(tbn),NULL,adCmdTable);
		rs->Close();
		return true;
	}
	catch(_com_error e)
	{
		return false;
	}

}


//�Ա�e_presiz ���л���
//��¼�� sourceSet ���밴ָ�����ֶν�������
BOOL CTotalTableIPED::totalPresiz(_RecordsetPtr sourceSet, _RecordsetPtr TargetSet)
{
	_variant_t key;
	double totalSize_amoun=0.0;//�����ֶ�size_amoun�ĺ�
	double tempSize_amoun=0.0;
	
	double totalVol=0.0;//������ȫ����ĺ�
	double tempVol=0.0;

	CString strTempKey;
	CString last_strKey;
	last_strKey="";//����ʼֵ
	int flag=0;

	double TempSize_num,lastSize_num;
	double TempSpec,lastSpec;
	CString TempOut_heat,lastOut_heat;
	double TempOut_warm,lastOut_warm;
	CString TempSize_unit,lastSize_unit;
	CString TempSize_mark,lastSize_mark;

	
	if(sourceSet->GetRecordCount()>0)
	{

		sourceSet->MoveFirst();
	}
	else 
	{
		AfxMessageBox("��ʾ:���²���û��ԭʼ��¼!");
		return false;
	}

	try
	{
		for(;!sourceSet->adoEOF;sourceSet->MoveNext())
		{//4

			key=sourceSet->GetCollect(_T("size_key"));//��Դ��¼����ȡ��ֵ
			if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
			{//3
				strTempKey=key.bstrVal;
				strTempKey=Trim(strTempKey);//9/2

				//ȡ�ֶΣ�size_amoun ��ֵ
				key=sourceSet->GetCollect(_T("size_amoun"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{//1
					tempSize_amoun=key.dblVal;
				}//1
				else 
				{//1
					tempSize_amoun=0.0;
				}//1

				//ȡ�ֶ�: ��ȫ��� ��ֵ
				key=sourceSet->GetCollect(_T("��ȫ���"));
				tempVol=(!(key.vt==VT_EMPTY||key.vt==VT_NULL))?key.dblVal:0.0;
					
				//ȡ�ֶ�:size_num �е�ֵ
				key=sourceSet->GetCollect(_T("size_num"));
				TempSize_num=(!(key.vt==VT_NULL||key.vt==VT_EMPTY))?key.dblVal:0.0;

				//ȡ�ֶ�:���
				key=sourceSet->GetCollect(_T("���"));
				TempSpec=(!(key.vt==VT_NULL||key.vt==VT_EMPTY))?key.dblVal:0.0;

				//�Ᵽ�²���
				key=sourceSet->GetCollect(_T("�Ᵽ�²���"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{
					TempOut_heat=key.bstrVal;
					TempOut_heat=Trim(TempOut_heat);
				}
				else
				{
					TempOut_heat=" ";
				}

				//�Ᵽ�º�
				key=sourceSet->GetCollect(_T("�Ᵽ�º�"));
				TempOut_warm=(!(key.vt==VT_EMPTY||key.vt==VT_NULL))?key.dblVal:0.0;

				//�ֶΣ�size_unit
				key=sourceSet->GetCollect(_T("size_unit"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{
					TempSize_unit=key.bstrVal;
					TempSize_unit=Trim(TempSize_unit);
				}
				else
				{
					TempSize_unit=" ";
				}

				//�ֶΣ�size_mark
				key=sourceSet->GetCollect(_T("size_mark"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{
					TempSize_mark=key.bstrVal;
					TempSize_mark=Trim(TempSize_mark);
				}
				else
				{
					TempSize_mark=" ";
				}

				//��������SourceSet��¼���е��ֶ�(source_str)ֵ�Ƿ����
				if(!(strTempKey.Compare(last_strKey)==0))//�����
				{//2
					
					//�������ǵ�һ��,�ڻ��ܱ�������һ����¼
					if(flag!=0&&last_strKey!="")
					{//1
						CString strGet;
						TargetSet->AddNew();
						TargetSet->PutCollect(_T("enginid"),_variant_t(EDIBgbl::SelPrjID));//9/2
						TargetSet->PutCollect(_T("size_key"),_variant_t(last_strKey));
						TargetSet->PutCollect(_T("size_amoun"),_variant_t(totalSize_amoun));
						TargetSet->PutCollect(_T("��ȫ���"),_variant_t(totalVol));
						
						TargetSet->PutCollect(_T("size_num"),_variant_t(lastSize_num));

						TargetSet->PutCollect(_T("���"),_variant_t(lastSpec));

						TargetSet->PutCollect(_T("�Ᵽ�²���"),_variant_t(lastOut_heat));

						TargetSet->PutCollect(_T("�Ᵽ�º�"),_variant_t(lastOut_warm));

						TargetSet->PutCollect(_T("size_unit"),_variant_t(lastSize_unit));
						TargetSet->PutCollect(_T("size_mark"),_variant_t(lastSize_mark));

						TargetSet->Update();
					}//1
					last_strKey=strTempKey;
					totalSize_amoun=0.0;
					totalVol=0.0;//9/2
					lastSize_num=TempSize_num;
					lastSpec=TempSpec;
					lastOut_heat=TempOut_heat;
					lastOut_warm=TempOut_warm;
					lastSize_unit=TempSize_unit;
					lastSize_mark=TempSize_mark;
				}//2
				flag=1;
				totalSize_amoun+=tempSize_amoun;//���
				totalVol+=tempVol;//9/2

			}//3û��ȡ��ֵ
			else 
			{
				if(flag!=0&&last_strKey!="")
				{
					CString strGet;
					TargetSet->AddNew();
					TargetSet->PutCollect(_T("enginid"),_variant_t(EDIBgbl::SelPrjID));
					TargetSet->PutCollect(_T("size_key"),_variant_t(last_strKey));
					TargetSet->PutCollect(_T("size_amoun"),_variant_t(totalSize_amoun));
					TargetSet->PutCollect(_T("��ȫ���"),_variant_t(totalVol));
					
					TargetSet->PutCollect(_T("size_num"),_variant_t(lastSize_num));

					TargetSet->PutCollect(_T("���"),_variant_t(lastSpec));

					TargetSet->PutCollect(_T("�Ᵽ�²���"),_variant_t(lastOut_heat));

					TargetSet->PutCollect(_T("�Ᵽ�º�"),_variant_t(lastOut_warm));

					TargetSet->PutCollect(_T("size_unit"),_variant_t(lastSize_unit));

					TargetSet->PutCollect(_T("size_mark"),_variant_t(lastSize_mark));

					TargetSet->Update();
					totalSize_amoun=0.0;
					totalVol=0.0;
				}
				last_strKey="";
			}
		}//4


		//���˼�¼β��Ҫ����һ����¼
		if(last_strKey!=""&&sourceSet->GetRecordCount()>0)
		{
			CString strGet;
			TargetSet->AddNew();
			TargetSet->PutCollect(_T("enginid"),_variant_t(EDIBgbl::SelPrjID));
			TargetSet->PutCollect(_T("size_key"),_variant_t(last_strKey));
			TargetSet->PutCollect(_T("size_amoun"),_variant_t(totalSize_amoun));
			TargetSet->PutCollect(_T("��ȫ���"),_variant_t(totalVol));
			
			TargetSet->PutCollect(_T("size_num"),_variant_t(lastSize_num));

			TargetSet->PutCollect(_T("���"),_variant_t(lastSpec));

			TargetSet->PutCollect(_T("�Ᵽ�²���"),_variant_t(lastOut_heat));

			TargetSet->PutCollect(_T("�Ᵽ�º�"),_variant_t(lastOut_warm));

			TargetSet->PutCollect(_T("size_unit"),_variant_t(lastSize_unit));

			TargetSet->PutCollect(_T("size_mark"),_variant_t(lastSize_mark));

			TargetSet->Update();

		}

	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}


//���ɱ��²��ϻ��ܱ�(��׼+����)
void CTotalTableIPED::c_preast()
{
	CString strpi;
	strpi.Format("%9.7f",pi);
	_RecordsetPtr a_matastSet;
	a_matastSet.CreateInstance(__uuidof(Recordset));
	_RecordsetPtr a_wireSet;
	a_wireSet.CreateInstance(__uuidof(Recordset));
	_RecordsetPtr e_preexpSet;
	e_preexpSet.CreateInstance(__uuidof(Recordset));
	_RecordsetPtr e_preexpSet1;
	e_preexpSet1.CreateInstance(__uuidof(Recordset));
	_RecordsetPtr a_vertSet;
	a_vertSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	_bstr_t sql;

	CString main_mat;
	_variant_t key;
	CString ast;
	CString sum_field;
	CString con;
	CString str_wire_base;
	CString sum_field1;
	CString vert_form;
	CString ver_amou;
	double wire_tot;
	int a_vertFieldsCount;
	CString strast;//9/3
	double wire_amoun;
	CString ast2;
	double preexp_tot;
	double vert_amouD;

	try
	{

		strSQL="select * from a_wire where enginid='"+EDIBgbl::SelPrjID+"'"; //9/22
		sql=strSQL;
		a_wireSet->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		
		strSQL="update a_wire set wire_amoun=0 where enginid='"+EDIBgbl::SelPrjID+"'";
		sql=strSQL;
		m_Pconnection->Execute(sql,NULL,adCmdText);//9/22

		strSQL="select * from a_matast where EnginID='"+EDIBgbl::SelPrjID+"' ";
		sql=strSQL;
		a_matastSet->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);//*01
		
		strSQL="select * from e_preexp where enginid='"+EDIBgbl::SelPrjID+"'";	//*2
		sql=strSQL;
		e_preexpSet->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		
		strSQL="select * from a_vert where enginid='"+EDIBgbl::SelPrjID+"'";//9/22
		sql=strSQL;
		a_vertSet->Open(sql,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		
		//��ȡ��a_vert�е��ֶθ���
		FieldsPtr a_vert;
		a_vertSet->get_Fields(&a_vert);
		a_vertFieldsCount=a_vert->Count;
		a_vertFieldsCount=a_vertFieldsCount/5;	

		int i;									
		CString stri;
		CString str;
		for(i=1;i<=a_vertFieldsCount;i++)
		{
			stri.Format("%d",i);
			str="vert_amou"+stri;
			strSQL="update a_vert set "+str+"=0 where enginid='"+EDIBgbl::SelPrjID+"'";//9/22
			sql=strSQL;
			m_Pconnection->Execute(sql,NULL,adCmdText);
		}

		//��ȡ��:a_matast�е��ֶθ���
		FieldsPtr a_matast;
		a_matastSet->get_Fields(&a_matast);
		int a_matastFieldsCount;
		a_matastFieldsCount=a_matast->Count;
		a_matastFieldsCount--; //��ȥ�����ֶΡ�zsy


		if(a_matastSet->GetRecordCount()<=0)//??�Ǽ������л��Ƿ���
		{
			return;
		}								//*2

		for(i=1;i<a_matastFieldsCount;i++)  //*01
		{ //10
			a_matastSet->MoveFirst();//*01
			for(;!a_matastSet->adoEOF;a_matastSet->MoveNext()) //*02
			{  //9
				key=a_matastSet->GetCollect(_T("mat_name"));///????
				if(!(key.vt==VT_NULL||key.vt==VT_EMPTY))
				{
					main_mat=key.bstrVal;
				}
				else
				{
					main_mat="";
				}
				

				key=a_matastSet->GetCollect(_variant_t((short)i));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{
					ast=key.bstrVal;
					ast=Trim(ast);
				}
				else if(key.vt==VT_EMPTY||key.vt==VT_NULL)
				{
					ast="";
				}
				if(ast=="")
				{
					continue;
				}

				//��λ��¼
				strast="wire_name='"+ast+"'";
				if(a_wireSet->GetRecordCount()>0)
				{
					a_wireSet->MoveFirst();//9/3
					a_wireSet->Find(_bstr_t(strast),0,adSearchForward);
				}									//*02


				for(;(a_wireSet->GetRecordCount()>0)&&(!a_wireSet->adoEOF);)  //*03
				{ //7
					key=a_wireSet->GetCollect(_T("wire_sum"));
					if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
					{
						sum_field=key.bstrVal;
					}
					else 
					{
						sum_field="";
					}
					key=a_wireSet->GetCollect(_T("wire_con"));
					if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
					{
						con=key.bstrVal;
						con=Trim(con);
						con.Replace(".and."," and ");
						con.Replace(".AND."," AND ");
						con.Replace("TRIM(main_mat)","'"+main_mat+"'");//9/3
						con.Replace("#","<>");
					}
					else
					{
						con="";
					}		//*03

					key=a_wireSet->GetCollect(_T("wire_base"));//*04 
					if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
					{ 
						str_wire_base=key.bstrVal;
						str_wire_base=Trim(str_wire_base);
					}
					else
					{
						str_wire_base="";
					}

					if(str_wire_base!="")  //*04
					{//6
						//9/8
						key=a_wireSet->GetCollect(_T("wire_size"));//*05
						if(!(key.vt==VT_NULL||key.vt==VT_EMPTY))
						{
							ast2=key.bstrVal;
							ast2=Trim(ast2);
						}
						else
						{
							ast2="";
						}
						strSQL="vert_name='"+ast2+"'";
						//9/8
						//9/2
						if(a_vertSet->GetRecordCount()>=0)
						{//5
							a_vertSet->MoveFirst();
							
							//9/2
							a_vertSet->Find(_bstr_t(strSQL),0,adSearchForward);
							short k;

							for(k=1;k<=a_vertFieldsCount&&!a_vertSet->adoEOF;k++)//9/2
							{//4
								strSQL.Format("%d",k);
								vert_form="vert_form"+strSQL;
								key=a_vertSet->GetCollect(_variant_t(vert_form));
								if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
								{//3
									sum_field1=key.bstrVal;
									sum_field1=Trim(sum_field1);
									if(sum_field1=="") break;
									//�Ա�:e_preexp�ĵ�ǰ�ֶν������
									CString strSQL1;
									if(e_preexpSet1->State==adStateOpen)
									{
										e_preexpSet1->Close();
									}
									sum_field1.Replace("pi",""+strpi+"");	//9/17
									strSQL1="select sum("+sum_field1+") as tot from e_preexp where "+con+" and enginid='"+EDIBgbl::SelPrjID+"'";
									e_preexpSet1->Open((_bstr_t)strSQL1,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
									if(e_preexpSet1->GetRecordCount()>0)
									{//2
										e_preexpSet1->MoveFirst();
										key=e_preexpSet1->GetCollect(_T("tot"));
										preexp_tot=(key.vt==VT_EMPTY||key.vt==VT_NULL)?0.0:key.dblVal;
										//�Ա�a_vert�е�ver_amou�ֶν����޸�
										ver_amou="vert_amou"+strSQL;
										key=a_vertSet->GetCollect(_variant_t(ver_amou));
										vert_amouD=(key.vt==VT_EMPTY||key.vt==VT_NULL)?0.0:key.dblVal;
										vert_amouD+=preexp_tot;
										a_vertSet->PutCollect(_variant_t(ver_amou),_variant_t(vert_amouD));
										a_vertSet->Update();

									}//2
									
								}//3
								else
								{
									break;
								}
							}//4
						}//9/2 
					}//������ϼ��� //6
					else  //*04
					{//2
						if(e_preexpSet1->State==adStateOpen)
						{
							e_preexpSet1->Close();
						}
						if(!(sum_field==""||con==""))
						{
							sum_field.Replace("pi",""+strpi+"");//9/17
							strSQL="select sum("+sum_field+") as aa from e_preexp where "+con+" and enginid='"+EDIBgbl::SelPrjID+"'";
						
							e_preexpSet1->Open((_bstr_t)strSQL,(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
							if(e_preexpSet1->GetRecordCount()>0)  
							{//1
								e_preexpSet1->MoveFirst();
								key=e_preexpSet1->GetCollect(_T("aa"));
								wire_tot=(key.vt==VT_EMPTY||key.vt==VT_NULL)?0.0:key.dblVal;
								key=a_wireSet->GetCollect(_T("wire_amoun"));
								wire_amoun=(key.vt==VT_EMPTY||key.vt==VT_NULL)?0.0:key.dblVal;
								wire_amoun=wire_amoun+wire_tot;
								a_wireSet->PutCollect(_T("wire_amoun"),_variant_t(wire_amoun));
								a_wireSet->Update(); //*5
							}//1
						}
					}//2 *04
				
					a_wireSet->MoveNext(); //*03
					if(!a_wireSet->adoEOF)
					{
						a_wireSet->Find(_bstr_t(strast),0,adSearchForward); //*03
					}
				}//7

		
			}  //9  *02

		}  //10		*01
		
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return;
	}
	
	try
	{	
		//�޸ı�a_wire.wire_amoun(ȡ��)  //*3
		strSQL="update a_wire set wire_amoun=int(wire_amoun) ";//9/22
		strSQL=strSQL+" where (wire_unit='��' or wire_unit='ֻ' or wire_unit='��') and enginid='"+EDIBgbl::SelPrjID+"'";
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);  //9/22

		//ɾ����:e_ast�е����м�¼
		strSQL="delete from e_ast where enginid='"+EDIBgbl::SelPrjID+"'";//9/22		
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);//9/22
		
		//����:a_wire�еļ�¼���Ƶ���e_ast
		strSQL="insert into e_ast (col_num,col_name,gen_amount,col_size,col_unit,EnginID)";//�������ӵļ�¼����Ϊ��ǰ����. by zsy
		strSQL=strSQL+" select wire_num,wire_name,wire_amoun,wire_size,wire_unit,EnginID ";
		strSQL=strSQL+" from a_wire where (trim(wire_base)='' and wire_amoun<>0 ";//9/22
		strSQL=strSQL+" or wire_base is NULL and wire_amoun<>0) and enginid='"+EDIBgbl::SelPrjID+"'";//9/22
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);	//9/22

		//����a_vert�е���Ӧ�ֶθ��Ƶ���e_ast	//*5

		if(!(a_vertCopyE_ast(a_vertFieldsCount)))
		{
			return;
		}			//*5

		//���ñ�e_ast�е��ֶΣ�col_mark��ֵ
		strSQL="update e_ast set col_mark=trim(col_name)+trim(col_size) where enginid='"+EDIBgbl::SelPrjID+"'";  //9/22
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);//9/22
		strSQL="update e_ast set col_mark=col_name where col_size is NULL and enginid='"+EDIBgbl::SelPrjID+"'";//9/22
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);//9/22

		//����
		_RecordsetPtr e_astSet;
		e_astSet.CreateInstance(__uuidof(Recordset));
		_RecordsetPtr epreastSet;
		epreastSet.CreateInstance(__uuidof(Recordset));
		strSQL="select * from e_ast  where enginid='"+EDIBgbl::SelPrjID+"' order by col_mark ";//9/22
		e_astSet->Open(_bstr_t(strSQL),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);//9/22
		strSQL="delete from epreast where enginid='"+EDIBgbl::SelPrjID+"'";//9/22
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);//9/22
		strSQL="select * from epreast where enginid='"+EDIBgbl::SelPrjID+"'";//9/22
		epreastSet->Open(_bstr_t(strSQL),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);//9/22
		if(!TotalPreast(e_astSet,epreastSet))
		{
			return;
		}   //*10

		strSQL="update epreast set col_mark=' ' where enginid='"+EDIBgbl::SelPrjID+"'"; //9/22
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);//9/22

		strSQL="update epreast set gen_amount=int(gen_amount+0.999) where (col_unit='��' ";//9/22
		strSQL=strSQL+" or col_unit='ֻ' or col_unit='��') and enginid='"+EDIBgbl::SelPrjID+"'";
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);//9/22

		strSQL="update epreast set col_amount=gen_amount where enginid='"+EDIBgbl::SelPrjID+"'";//9/22
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);//9/22
	
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return;
	}

}


//�Լ�¼���е�str�ֶ���� �����غ�
//ע�����ȷ����¼����Ϊ��
double CTotalTableIPED::sum(CString str, _RecordsetPtr set)
{
	double tot=0.0;
	double val;
	_variant_t key;
	set->MoveFirst();
	try
	{
		for(;!set->adoEOF;set->MoveNext())
		{
			key=set->GetCollect((_variant_t)str);
			val=(key.vt==VT_NULL||key.vt==VT_EMPTY)?0.0:key.dblVal;
			tot=tot+val;
			
		}
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return -1;//��ʾ���ʧ��
	}
	return tot;
}

//����a_vert�еļ�¼���Ƶ���:e_ast��
bool CTotalTableIPED::a_vertCopyE_ast(int count)
{
	CString vert_num;
	CString vert_mat;
	CString vert_amou;
	CString vert_size;
	CString vert_unit;
	CString strSQL;
	CString stri;
	int i;
	try{
			for(i=1;i<=count;i++)
			{
				stri.Format("%d",i);
				vert_size="vert_size"+stri;
				vert_mat="vert_mat"+stri;
				vert_amou="vert_amou"+stri;
				vert_unit="vert_unit"+stri;
				strSQL="insert into e_ast (col_num,col_name,Gen_amount,col_size,col_unit,EnginID) "; //�������ӵļ�¼����Ϊ��ǰ����. by zsy
				strSQL=strSQL+" select vert_num,"+vert_mat+","+vert_amou+","+vert_size+","+vert_unit+",EnginID ";
				strSQL=strSQL+" from a_vert where "+vert_amou+" <> 0 and enginid='"+EDIBgbl::SelPrjID+"'";//9/22
				m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);//9/22

			}
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

//�Ա��²��Ͻ��л��ܺ���
bool CTotalTableIPED::TotalPreast(_RecordsetPtr sourceSet, _RecordsetPtr TargetSet)
{
	_variant_t key;
	short iCol_num=0;
	
	double totalGet_amount=0.0;//����get_amount�ĺ�
	double tempGet_amount=0.0;

	double totalCol_amount;//����col_amount�ĺ�
	double tempCol_amount;

	CString strCol_name;
	CString lastCol_name;
	CString strCol_size;
	CString lastCol_size;
	CString strCol_unit;
	CString lastCol_unit;

	CString strTempKey;
	CString last_strKey;
	last_strKey="";//����ʼֵ
	int flag=0;
	
	if(sourceSet->GetRecordCount()>0)
	{
		sourceSet->MoveFirst();
	}

	try
	{
		for(;!sourceSet->adoEOF;sourceSet->MoveNext())
		{//4

			key=sourceSet->GetCollect(_T("col_mark"));//��Դ��¼����ȡ��ֵ
			if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
			{//3
				strTempKey=key.bstrVal;


				//ȡ�ֶ�: get_amount ��ֵ
				key=sourceSet->GetCollect(_T("gen_amount"));
				tempGet_amount=(!(key.vt==VT_EMPTY||key.vt==VT_NULL))?key.dblVal:0.0;


				//ȡ�ֶ�:col_amount��ֵ
				key=sourceSet->GetCollect(_T("col_amount"));
				tempCol_amount=(key.vt==VT_EMPTY||key.vt==VT_NULL)?0.0:key.dblVal;
				
				key=sourceSet->GetCollect(_T("col_name"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{
					strCol_name=key.bstrVal;
				}
				else
				{
					strCol_name="";
				}

				
				key=sourceSet->GetCollect(_T("col_size"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{
					strCol_size=key.bstrVal;
				}
				else
				{
					strCol_size="";
				}


				key=sourceSet->GetCollect(_T("col_unit"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{
					strCol_unit=key.bstrVal;
				}
				else
				{
					strCol_unit="";
				}

				//��������SourceSet��¼���е��ֶ�(source_str)ֵ�Ƿ����
				if(!(strTempKey.Compare(last_strKey)==0))//�����
				{//2
					
					//�������ǵ�һ��,�ڻ��ܱ�������һ����¼
					if(flag!=0&&last_strKey!="")
					{//1
						
						iCol_num++;
						TargetSet->AddNew();
						TargetSet->PutCollect(_T("col_mark"),_variant_t(last_strKey));
						TargetSet->PutCollect(_T("col_num"),_variant_t(iCol_num));
						TargetSet->PutCollect(_T("gen_amount"),_variant_t(totalGet_amount));
						TargetSet->PutCollect(_T("col_amount"),_variant_t(totalCol_amount));
						TargetSet->PutCollect(_T("col_name"),_variant_t(lastCol_name));
						TargetSet->PutCollect(_T("col_size"),_variant_t(lastCol_size));
						TargetSet->PutCollect(_T("col_unit"),_variant_t(lastCol_unit));
						TargetSet->PutCollect(_T("EnginID"),_variant_t(EDIBgbl::SelPrjID));  //����Ϊ��ǰ����.  by zsy

						TargetSet->Update();
					}//1
					last_strKey=strTempKey;
					lastCol_name=strCol_name;
					lastCol_size=strCol_size;
					lastCol_unit=strCol_unit;
					totalGet_amount=0.0;
					totalCol_amount=0.0;
				}//2
				flag=1;
				totalGet_amount+=tempGet_amount;
				totalCol_amount+=tempCol_amount;

			}//3û��ȡ��ֵ
			else 
			{
				if(flag!=0&&last_strKey!="")
				{
					iCol_num++;
					TargetSet->AddNew();
					TargetSet->PutCollect(_T("col_mark"),_variant_t(last_strKey));
					TargetSet->PutCollect(_T("col_num"),_variant_t(iCol_num));
					TargetSet->PutCollect(_T("gen_amount"),_variant_t(totalGet_amount));
					TargetSet->PutCollect(_T("col_amount"),_variant_t(totalCol_amount));
					TargetSet->PutCollect(_T("col_name"),_variant_t(lastCol_name));
					TargetSet->PutCollect(_T("col_size"),_variant_t(lastCol_size));
					TargetSet->PutCollect(_T("col_unit"),_variant_t(lastCol_unit));
					TargetSet->PutCollect(_T("EnginID"),_variant_t(EDIBgbl::SelPrjID));  //����Ϊ��ǰ����. by zsy

					TargetSet->Update();

					totalGet_amount=0.0;
					totalCol_amount=0.0;
					
				}
				last_strKey="";
			}
		}//4


		//���˼�¼β��Ҫ����һ����¼
		if(last_strKey!=""&&sourceSet->GetRecordCount()>0)
		{
			TargetSet->AddNew();
			iCol_num++;
			TargetSet->PutCollect(_T("col_mark"),_variant_t(last_strKey));
			TargetSet->PutCollect(_T("col_num"),_variant_t(iCol_num));
			TargetSet->PutCollect(_T("gen_amount"),_variant_t(totalGet_amount));
			TargetSet->PutCollect(_T("col_amount"),_variant_t(totalCol_amount));
			TargetSet->PutCollect(_T("col_name"),_variant_t(lastCol_name));
			TargetSet->PutCollect(_T("col_size"),_variant_t(lastCol_size));
			TargetSet->PutCollect(_T("col_unit"),_variant_t(lastCol_unit));
			TargetSet->PutCollect(_T("EnginID"),_variant_t(EDIBgbl::SelPrjID));//����Ϊ��ǰ����. by zsy

			TargetSet->Update();

		}

	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

//���ܣ�����ɫ����ͷ������ܱ�
bool CTotalTableIPED::modC_RING()
{
	try
	{
		int pos=1;  //9/4
		_RecordsetPtr pRsPaint_c;
		pRsPaint_c.CreateInstance(__uuidof(Recordset));

		CString strSQL;
		_variant_t var;
		//��ձ�PAINT_C
		strSQL = "DELETE FROM [paint_c] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
		//	*****���������ʵ���paint�ͱ��¶���������ɫ���paint_c2(��Ϊֱ�Ӵ�e_preexp������ȡ)  ****/
		//	*****�ϲ���������ɫ��ƿ�paint_c��						****/

		//����E_PAIEXP�е����ݸ��Ƶ���PAINT_C��//*3
		strSQL = "INSERT INTO [paint_c] (PAI_VOL,���,����,PAI_TYPE,PAI_SIZE,PAI_AMOU";
		strSQL += ",�������,��ע,������,�������,��ÿ������,�׺ϼ�����,������,�������";
		strSQL += ",��ÿ������,��ϼ�����,PAI_A1,PAI_A_T1,PAI_A2,PAI_A_T2,PAI_AREA,PAI_A_C1";
		strSQL += ",PAI_A_C2,PAI_CODE,PAI_C_FACE,PAI_R_COLR,PAI_R_COST,PAI_R_AREA";
		strSQL += ",PAI_R_T,PAI_A_COLR,PAI_A_COST,PAI_A_AREA,PAI_A_T,EnginID)";

		strSQL += "	 SELECT PAI_VOL,���,����,PAI_TYPE,PAI_SIZE,PAI_AMOU";
		strSQL +=",�������,��ע,������,�������,��ÿ������,�׺ϼ�����,������,�������";
		strSQL += ",��ÿ������,��ϼ�����,PAI_A1,PAI_A_T1,PAI_A2,PAI_A_T2,PAI_AREA,PAI_A_C1";
		strSQL += ",PAI_A_C2,PAI_CODE,PAI_C_FACE,PAI_R_COLR,PAI_R_COST,PAI_R_AREA";
		strSQL += ",PAI_R_T,PAI_A_COLR,PAI_A_COST,PAI_A_AREA,PAI_A_T,EnginID";

		strSQL += " FROM [e_paiexp] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText); //*3
		//��e_preexp���е����ݸ��Ƶ���paint_c��.     //*2
		strSQL = "INSERT INTO [paint_c] (PAI_VOL, ����, PAI_SIZE ";
		strSQL += ", PAI_AMOU, �������, ��ע, pai_code, EnginID ) ";

		strSQL += "SELECT C_VOL AS PAI_VOL, ����, ��� AS PAI_SIZE ";
		strSQL += ", ���� AS PAI_AMOU, ������ AS �������, ��ע, c_color AS pai_code, EnginID";
		strSQL += " FROM [e_preexp] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";

		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);//*2
		/// ************************************* /
		CString strFace="", strRing="", strArrow="";
		double  dw, dCode, dLength, dFace_l,
				dRing_l, dS_ring,dArrow_l, dS_arrow,
				dS_word, dev_area;
		
		double	dRing_Dosage;		//ɫ����ͷ�Ķ���
		
		//�򿪱� pRsColr_l	�����ᵥλ��������
		//		pRsRing_Paint ɫ����ͷ������Ͽ�
		//		pRsColor	ɫ����ͷ������ɫ׼���
		//		pRsArrow	ɫ����ͷ�ߴ�׼���		 ��������Ϊ���Ա,һ����ȫ����.����ٶ�	 [2005/06/10]
		if ( !OpenCodeDB() )
		{
			return false;
		}
		m_strExecuteSQLTblName = "ExecuteSQL";		//ִ��SQL������ʱ����
		if (!CreateTempTable(m_Pconnection, m_strExecuteSQLTblName))
		{
			return FALSE;
		}
		if (pRsExecSQL == NULL)
		{
			pRsExecSQL.CreateInstance(__uuidof(Recordset));
		}
		if (pRsExecSQL->State != adStateOpen)
		{
			pRsExecSQL->Open(_variant_t("select * from ["+m_strExecuteSQLTblName+"]"), m_Pconnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic,adCmdText);
		}
		//ɫ����ͷ������ϵĶ���
		if (pRsRing_Paint->adoEOF && pRsRing_Paint->BOF)
		{
			dRing_Dosage = 1;		//���û�м�¼ʱĬ��Ϊ1��.
		}else
		{
			pRsRing_Paint->MoveFirst();
			dRing_Dosage = vtof(pRsRing_Paint->GetCollect(_variant_t("paint_dosage")));
			if (dRing_Dosage < 0)
			{
				dRing_Dosage = 1;
			}
		}
		////����ԭʼ���ݱ�
		if( pRsPaint_c->State == adStateOpen )
		{
			pRsPaint_c->Close();
		}
		strSQL = "SELECT * FROM [paint_c] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
		pRsPaint_c->Open(_bstr_t(strSQL), (IDispatch*)m_Pconnection, adOpenStatic, adLockOptimistic, adCmdText);
		//9/28
		if ((!pRsPaint_c->adoEOF) && (!pRsPaint_c->BOF))
		{
			pRsPaint_c->MoveFirst();
			while (!pRsPaint_c->adoEOF)
			{
				//��ʼ����
				strFace = strRing =  strArrow = " "; //*4
				dFace_l = dRing_l = dS_ring = dArrow_l = dS_arrow = dS_word = 0.0;
				
				var = pRsPaint_c->GetCollect("pai_size");      //�⾶
				dw  = (var.vt==VT_NULL || var.vt==VT_EMPTY)?0.0:var.dblVal;
				
				var = pRsPaint_c->GetCollect("pai_code");       //������ɫ����
				dCode = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;
				
				var = pRsPaint_c->GetCollect("pai_amou");         //�ܳ�
				dLength = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;
				
				var = pRsPaint_c->GetCollect("�������");         //�豸���
				dev_area = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;
				
				
				//����ģ�� 
				if( !moduleCOLOR_S(dw/*�⾶*/, dCode/*������ɫ����*/, dLength/*�ܳ�*/, strFace/*����ɫ*/,
					dFace_l/*����ÿ������*/, strRing/*ɫ��*/, dRing_l/*ÿ������*/,
					dS_ring/*ɫ�������*/, strArrow/*��ͷ/����*/, dArrow_l/*ÿ������*/,
					dS_arrow/*��ͷ�����*/, dS_word/*���������*/, dev_area/*�豸���*/) )																							
				{
					return false;
				}
				pRsPaint_c->Fields->GetItem("pai_c_face")->PutValue(_variant_t(strFace));
				pRsPaint_c->Fields->GetItem("��ÿ������")->PutValue(_variant_t(dFace_l));
				pRsPaint_c->Fields->GetItem("pai_r_colr")->PutValue(_variant_t(strRing));
				pRsPaint_c->Fields->GetItem("pai_r_cost")->PutValue(_variant_t(dRing_l));
				
				pRsPaint_c->Fields->GetItem("pai_r_area")->PutValue(_variant_t(dS_ring));
				pRsPaint_c->Fields->GetItem("pai_a_colr")->PutValue(_variant_t(strArrow));
				pRsPaint_c->Fields->GetItem("pai_a_cost")->PutValue(_variant_t(dArrow_l));
				pRsPaint_c->Fields->GetItem("pai_a_area")->PutValue(_variant_t((dS_arrow+dS_word)));

				//ɫ�������� 
				pRsPaint_c->PutCollect("PAI_R_T",_variant_t(dRing_Dosage*dRing_l*dS_ring));
				//��ͷ���ֵ�����
				pRsPaint_c->PutCollect("pai_a_t",_variant_t(dRing_Dosage*dArrow_l*(dS_arrow+dS_word)));
				
				pRsPaint_c->Update();
				
				pRsPaint_c->MoveNext();
				
				if(pos<100)
				{
					((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
				}
				if(pos>=100) pos=1;
			}
		}

		while(pos<100)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);			
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);

		return false;
	}
	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
	return true;

}
//��    ��: ��ɫ��,��ͷ,���ֵ�   ��ɫ�������              
bool CTotalTableIPED::moduleCOLOR_S(double& dw/*�⾶*/, double& dCode/*������ɫ����*/,
		double& dLength/*�ܳ�*/, CString& strFace/*����ɫ*/, double& dFace_l/*����ÿ������*/, 
		CString& strRing/*ɫ��*/, double& dRing_l/*ÿ������*/, double& dS_ring/*ɫ�������*/, 
		CString& strArrow/*��ͷ/����*/, double& dArrow_l/*ÿ������*/, double& dS_arrow/*��ͷ�����*/, 
		double& dS_word/*���������*/,   double& dev_area/*�豸���*/)
{
	try
	{
		CString strSQL, str, strMedia, strExpression;
		_variant_t var;
		double  dTmpSum, cTmpSum;
		//����״̬
		strFace="N";
		strRing="N";
		strArrow="N";

		if( pRsColor->RecordCount <= 0 )
		{
			return false;
		}
		else
		{
			pRsColor->MoveFirst();
		}
		str.Format("%f",dCode);
		strSQL = "colr_code="+str+" ";
		pRsColor->Find(_bstr_t(strSQL), NULL, adSearchForward);//*1
		if( !pRsColor->adoEOF )	//*2
		{
			var      = pRsColor->GetCollect("colr_media");
			strMedia = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
			var      = pRsColor->GetCollect("colr_face");
			strFace  = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;

			var      = pRsColor->GetCollect("colr_ring");
			strRing  = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
			var      = pRsColor->GetCollect("colr_arrow");
			strArrow = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
		}  //*2

		if( pRsExecSQL->State != adStateOpen )
		{
			strSQL="SELECT * FROM ["+m_strExecuteSQLTblName+"] ";// WHERE "+strExpression+" ";
			pRsExecSQL->Open(_bstr_t(strSQL), (IDispatch*)m_Pconnection, adOpenStatic, adLockOptimistic, adCmdText);
		}
		long rCount = pRsExecSQL->GetRecordCount();
		if ( rCount> 1)
		{
/*			strSQL = "DELETE FROM ["+m_strExecuteSQLTblName+"]";
			m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);

			//����һ����¼.
			str.Format("%f",dw);
			strSQL = "INSERT INTO ["+m_strExecuteSQLTblName+"] (DW) VALUES ("+str+") ";
*/
			for (pRsExecSQL->MoveFirst(), pRsExecSQL->MoveNext(); !pRsExecSQL->adoEOF; pRsExecSQL->MoveNext())
			{
				pRsExecSQL->Delete(adAffectCurrent);
			}
			pRsExecSQL->MoveFirst();
		}else if (rCount == 1)
		{
			pRsExecSQL->MoveFirst();
		}else
		{
			pRsExecSQL->AddNew();
		}
		//��DW���浽��ʱ���У�Ȼ��ִ�дӱ���ȡ���ı��ʽ��
		pRsExecSQL->PutCollect(_variant_t("DW"),_variant_t(dw));
		
		//9/28
		if(pRsArrow->GetRecordCount()>0)
		{
			pRsArrow->MoveFirst();
			//9/28
			while( !pRsArrow->adoEOF )//*3
			{
				var = pRsArrow->GetCollect("arrow_dw");
				strExpression = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
				//���ַ���.AND.ת��Ϊ AND ��
				strExpression.Replace(".AND.", " AND ");
				strExpression.Replace(".OR.",  " OR ");

				pRsExecSQL->PutFilter((long)adFilterNone);
				pRsExecSQL->PutFilter(_variant_t(strExpression));
				if( pRsExecSQL->RecordCount > 0 )
				{
					break;
				}
				pRsArrow->MoveNext();
			}
		}
		if( pRsArrow->adoEOF || pRsArrow->BOF )
		{
			return false;
		}//*3
		////
		CString strSQLArrow_c,  //�ֶ�arrow_c��ֵ��һ�����ʽ��
				strSQLArrow_d;  //�ֶ�arrow_d��ֵ��һ�����ʽ��
		double  dTmpArrow_b,
				dTmpArrow_g;
		var = pRsArrow->GetCollect("arrow_c");         //{{ȡ���ֶ�ֵ //*4
		strSQLArrow_c = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
		strSQLArrow_c.Replace(".AND.", " AND ");
		strSQLArrow_c.Replace(".OR.",  " OR ");
		//ȡ��ARROW_D��ֵ,��SQL���Խ��м��㡣
		var = pRsArrow->GetCollect("arrow_d");
		strSQLArrow_d = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
		strSQLArrow_d.Replace(".AND.", " AND ");
		strSQLArrow_d.Replace(".OR.",  " OR ");      
		//arrow_b�ֶε�ֵ
		var = pRsArrow->GetCollect("arrow_b");
		dTmpArrow_b = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;
		//arrow_g�ֶε�ֵ
		var = pRsArrow->GetCollect("arrow_g");
		dTmpArrow_g = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;	//}}

		//ִ��ȡ���ı��ʽ��
		strSQL = "UPDATE ["+m_strExecuteSQLTblName+"] SET D_ARROW="+strSQLArrow_d+", C_ARROW="+strSQLArrow_c+" ";
		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);

		//�ӱ�ExecuteSQL��ȡ�������Ľ����
		var = pRsExecSQL->GetCollect("D_ARROW");
		dTmpSum = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;
		var = pRsExecSQL->GetCollect("C_ARROW");
		cTmpSum = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;

		//***�޻�������,�������л�,��ͷ����
		//***����ɫ��,��ͷ,���ֵ������
		dS_ring = dS_arrow = dS_word = 0.0;	//*4
		if( strRing.CompareNoCase("N") ) //*5
		{
			dS_ring = dLength/5*pi*dw*dTmpArrow_b/1000000;	
		}
		else
		{	
			dS_word = dLength/5*dTmpSum*dTmpArrow_g/1000000; //*5
		}
		dS_arrow = dLength/5*dTmpSum*(cTmpSum+dTmpSum)/1000000;

		//***ƽ̨����֧�����޼�ͷ���� 
		if( !strArrow.CompareNoCase("N") )
		{
			dS_arrow = 0.0;
			dS_word = 0.0;
		}

		//
		if( pRsColr_l->RecordCount <= 0)
		{
			return false;
		}
		else
		{
			pRsColr_l->MoveFirst();
		}
		//��λCOLR_NAME='"+strFace+"'֮��ȡ��COLR_LOST�ֶε�ֵ�� 
		strSQL = "COLR_NAME='"+strFace+"' ";
		pRsColr_l->Find(_bstr_t(strSQL), NULL, adSearchForward);
		if( !pRsColr_l->adoEOF )
		{
			var = pRsColr_l->GetCollect("colr_lost");
			dFace_l = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;
		}
		//���¶�λ��ȡ��COLR_LOST�ֶε�ֵ�� 
		pRsColr_l->MoveFirst();
		strSQL = "COLR_NAME='"+strRing+"' ";
		pRsColr_l->Find(_bstr_t(strSQL), NULL, adSearchForward);
		if( !pRsColr_l->adoEOF )
		{
			var = pRsColr_l->GetCollect("colr_lost");
			dRing_l = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;
		}
		//���¶�λ��ȡ��COLR_LOST�ֶε�ֵ�� 
		pRsColr_l->MoveFirst();
		strSQL = "COLR_NAME='"+strArrow+"' ";
		pRsColr_l->Find(_bstr_t(strSQL), NULL, adSearchForward);
		if( !pRsColr_l->adoEOF )
		{
			var = pRsColr_l->GetCollect("colr_lost");
			dArrow_l = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;
		}
		//
		if( dw >= 2000 || dw == 0 )
		{
			strRing = "N";
			dRing_l = dS_ring = dS_arrow = 0;
			dS_word = 0.08 * (int)((dev_area + 49) / 50);
		}
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}


//************************************
//* ģ �� ��: c_precol()             *
//* ��    ��: ���ɱ��²��ϻ��ܱ�     *
//*           �ͱ��¸������ϻ��ܱ�	 *
//* �ϼ�ģ��: MAIN_MENU              *
//* �¼�ģ��: c_preast()             *
//* �޸�����:                        *
//************************************
BOOL CTotalTableIPED::c_precol()
{
	//ɾ����e_precol�еļ�¼
	_RecordsetPtr e_precolSet;
	_RecordsetPtr e_preexpSet;
	e_preexpSet.CreateInstance(__uuidof(Recordset));
	e_precolSet.CreateInstance(__uuidof(Recordset));
	_RecordsetPtr epreastSet;
	epreastSet.CreateInstance(__uuidof(Recordset));
	CString strSQL;
	m_strPaint_c="col_name";
	m_valPaint_c="col_amount";
	_variant_t key;
	double val;
	CString str;
	int pos=0;
	try
	{
		strSQL="delete from e_precol where enginid='"+EDIBgbl::SelPrjID+"'";	//*1
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText);    //*1
		
		strSQL="select * from e_precol where enginid='"+EDIBgbl::SelPrjID+"'";   //*2
		e_precolSet->Open(_bstr_t(strSQL),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);

		//��e_preexp�е� "��ȫ���"����
		strSQL="select trim(�Ᵽ�²���) as �Ᵽ�²���,��ȫ��� from e_preexp  where enginid='"+EDIBgbl::SelPrjID+"' order by trim(�Ᵽ�²���)";
		e_preexpSet->Open(_bstr_t(strSQL),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		if(e_preexpSet->GetRecordCount()>0)
		{
			TotalTable(e_preexpSet,e_precolSet,"�Ᵽ�²���","��ȫ���");		//����
		}

		for(;pos<10;)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}		//*2

		//��:e_preexp�е� "�ڱ��²���"  ����
		if(e_preexpSet->State==adStateOpen)
		{
			e_preexpSet->Close();
		}
		//*3
		strSQL="select trim(�ڱ��²���) as �ڱ��²���,��ȫ���	from e_preexp   where enginid='"+EDIBgbl::SelPrjID+"' order by trim(�ڱ��²���)";
		e_preexpSet->Open(_bstr_t(strSQL),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		if(e_preexpSet->GetRecordCount()>0)
		{
			TotalTable(e_preexpSet,e_precolSet,"�ڱ��²���","��ȫ���");
		}
		for(;pos<20;)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}//*3

		//��:e_preexp�е�"�������� " ����
		if(e_preexpSet->State==adStateOpen)//*4
		{
			e_preexpSet->Close();
		}
		strSQL="select trim(��������) as ��������,������ from e_preexp where enginid='"+EDIBgbl::SelPrjID+"'";
		strSQL=strSQL+" and ������<=5 order by trim(��������)";
		e_preexpSet->Open(_bstr_t(strSQL),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		if(e_preexpSet->GetRecordCount()>0)
		{
			TotalTable(e_preexpSet,e_precolSet,"��������","������");
		}
		if(e_preexpSet->State==adStateOpen)
		{
			e_preexpSet->Close();//*4
		}
		for(;pos<30;)	//*6
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}
		
		//��e_preexp�е� "��������"����
		strSQL="select trim(��������) as ��������,����ȫ��� from e_preexp where enginid='"+EDIBgbl::SelPrjID+"'";
		strSQL=strSQL+" and ������>5 order by trim(��������)";
		e_preexpSet->Open(_bstr_t(strSQL),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		if(e_preexpSet->GetRecordCount()>0)
		{
			TotalTable(e_preexpSet,e_precolSet,"��������","����ȫ���");
		}
		for(;pos<40;)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}		//*6
			

		//ɾ����e_precol���ֶ�(col_name)ֵΪ0 �ļ�¼ //*7
		strSQL="delete from e_precol where (col_name is NULL or col_name=' ' )and enginid='"+EDIBgbl::SelPrjID+"'";
		m_Pconnection->Execute(_bstr_t(strSQL),NULL,adCmdText); //*7
		
		//���ݱ�a_yl��ֵ�Ա�e_precol������Ӧ���޸�
		//�޸��ֶΣ�gen_anount col_unit
		if(e_precolSet->GetRecordCount()>0)//*8
		{
			e_precolSet->MoveFirst();
			a_yl(e_precolSet);
		}
		for(;pos<50;)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}//*8
		

		//�޸��ֶ�:col_size
		if(e_precolSet->GetRecordCount()>0)//*9
		{
			e_precolSet->MoveFirst();
			CString col_name;
			int i,j;
			for(;!e_precolSet->adoEOF;e_precolSet->MoveNext())
			{
				key=e_precolSet->GetCollect(_T("col_name"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{
					col_name=key.bstrVal;
					col_name=Trim(col_name);
					i=col_name.Find("(",0);
					j=col_name.Find(")",0);
					if(!(i<0||j<0))
					{
						col_name=col_name.Mid(i+1,j-i-1);
						e_precolSet->PutCollect(_T("col_size"),_variant_t(col_name));
						e_precolSet->Update();
					}
				}
			}
		}
		for(;pos<60;)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}//*9

		c_preast();

		for(;pos<80;)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}
		
		//����:epreast�е����м�¼��������e_precol��//*10
		if(e_precolSet->State==adStateOpen)
		{
			e_precolSet->Close();
		}
		strSQL="select * from e_precol where enginid='"+EDIBgbl::SelPrjID+"'";
		e_precolSet->Open(_bstr_t(strSQL),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		epreastSet->Open(_bstr_t(_T("select * from epreast where enginid='"+EDIBgbl::SelPrjID+"'")),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);//9/22
		if(epreastSet->GetRecordCount()>0)
		{
			epreastSet->MoveFirst();
			for(;!epreastSet->adoEOF;epreastSet->MoveNext())
			{
				e_precolSet->AddNew();
				key=epreastSet->GetCollect(_T("col_name"));
				if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
				{
					str=key.bstrVal;
				}
				else 
				{
					str="";
				}
				e_precolSet->PutCollect(_T("col_name"),_variant_t(str));
				 
				key=epreastSet->GetCollect(_T("gen_amount"));
				val=(key.vt==VT_NULL||key.vt==VT_EMPTY)?0.0:key.dblVal;
				e_precolSet->PutCollect(_T("gen_amount"),_variant_t(val));

				key=epreastSet->GetCollect(_T("col_size"));
				if(key.vt==VT_EMPTY||key.vt==VT_NULL)
				{
					str="";
				}
				else
				{
					str=key.bstrVal;
				}
				e_precolSet->PutCollect(_T("col_size"),_variant_t(str));

				key=epreastSet->GetCollect(_T("col_unit"));
				if(key.vt==VT_EMPTY||key.vt==VT_NULL)
				{
					str="";
				}
				else 
				{
					str=key.bstrVal;
				}
				e_precolSet->PutCollect(_T("col_unit"),_variant_t(str));

				key=epreastSet->GetCollect(_T("col_mark"));
				if(key.vt==VT_EMPTY||key.vt==VT_NULL)
				{
					str="";
				}
				else 
				{
					str=key.bstrVal;
				}
				e_precolSet->PutCollect(_T("col_mark"),_variant_t(str));

				key=epreastSet->GetCollect(_T("col_amount"));
				val=(key.vt==VT_NULL||key.vt==VT_EMPTY)?0.0:key.dblVal;
				e_precolSet->PutCollect(_T("col_amount"),_variant_t(val));
				e_precolSet->PutCollect(_T("enginid"),_variant_t(EDIBgbl::SelPrjID));

				e_precolSet->Update();
			}
		
		}
		for(;pos<90;)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}
		if(e_precolSet->GetRecordCount()>0)//9/13
		{
			a_config(e_precolSet);//*10
		}

		//���ֶΣ�col_num���±��
		long n=1;
		if(e_precolSet->GetRecordCount()>0)
		{
			e_precolSet->MoveFirst();
		}
		for(;e_precolSet->GetRecordCount()>0&&(!e_precolSet->adoEOF);n++,e_precolSet->MoveNext())
		{
			e_precolSet->PutCollect(_T("col_num"),_variant_t((double)n));
			e_precolSet->Update();
		}
		for(;pos<100;)
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}

	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
		return false;
	}

	//9/18
	//�Ա�pass ���д���
	_RecordsetPtr pass_set;
	int flagPass=0;
	pass_set.CreateInstance(__uuidof(Recordset));
	pass_set->Open(_bstr_t(_T("select * from pass where enginid='"+EDIBgbl::SelPrjID+"'")),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
	if(pass_set->GetRecordCount()>0)
	{
		pass_set->MoveFirst();
		for(;!pass_set->adoEOF;pass_set->MoveNext())
		{
			if(flagPass==0)
			{
				pass_set->PutCollect(_T("pass_mark1"),_T("T"));
			}
			else
			{
				pass_set->PutCollect(_T("pass_mark1"),_T("F"));
			}
			key=pass_set->GetCollect(_T("pass_mod1"));
			if((!(key.vt==VT_NULL||key.vt==VT_EMPTY))&&(flagPass==0))
			{
				str=key.bstrVal;
				flagPass=(str.Compare("C_PRECOL")==0)?1:0;
			}
		}
	}
	//9/18

	if(e_precolSet->GetRecordCount()>0)
	{
//		AfxMessageBox("���ɱ��²��ϻ��ܱ�ɹ�!");//10/13
	}
	else
	{
		AfxMessageBox("���²���ԭʼ��¼Ϊ��!");
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
		return false;
	}
	((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(0);
	return true;

}

//����a_config �е���Ϣ���뵽���ʽ:e_precol��
void CTotalTableIPED::a_config(_RecordsetPtr sourceSet)
{
	// test add valve  [2005/07/01]
/*	_RecordsetPtr	pRsPipe_Valve;		//�ܵ��뷧�ŵ�ӳ���
	CString			strSQL;				//SQL���
	long	nResCount = 0;				//�ܵ������ӷ��ŵĸ���.
	pRsPipe_Valve.CreateInstance(__uuidof(Recordset));
	strSQL = "SELECT * FROM [Pipe_Valve] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ORDER By valveID ";
	pRsPipe_Valve->Open(_bstr_t(strSQL), m_Pconnection.GetInterfacePtr(),
						adOpenStatic, adLockOptimistic, adCmdText);				
		
	nResCount = pRsPipe_Valve->GetRecordCount();
	
	if (nResCount <= 0)
	{	
		nResCount = 0;
	}
	//*/
	_variant_t key;
	CString val_mat;
	CString str;
	double val_amount;
	_RecordsetPtr a_configSet;
	a_configSet.CreateInstance(__uuidof(Recordset));
	a_configSet->Open(_bstr_t("select * from a_config where enginid='"+EDIBgbl::SelPrjID+"'"),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
	if(a_configSet->GetRecordCount()<=0)
	{
		return;
	}
	key=a_configSet->GetCollect(_T("�����²���"));
	if(key.vt==VT_EMPTY||key.vt==VT_NULL)
	{
		val_mat="";
	}
	else 
	{
		val_mat=key.bstrVal;
	}
	sourceSet->AddNew();
	sourceSet->PutCollect(_T("col_name"),_variant_t(val_mat));
	
	key=a_configSet->GetCollect(_T("��������"));
	val_amount=(key.vt==VT_NULL||key.vt==VT_EMPTY)?0.0:key.dblVal;
	val_amount=val_amount*0.015;
//	val_amount+=nResCount;		// test add valve [2005/07/01]
	sourceSet->PutCollect(_T("gen_amount"),_variant_t(val_amount));
	sourceSet->PutCollect(_T("col_amount"),_variant_t(val_amount));
	str="m3";
	sourceSet->PutCollect(_T("col_unit"),_variant_t(str));
	str="���ű�����";
	sourceSet->PutCollect(_T("col_mark"),_variant_t(str));
	sourceSet->PutCollect(_T("enginid"),_variant_t(EDIBgbl::SelPrjID));
	sourceSet->Update();

}


void CTotalTableIPED::a_yl(_RecordsetPtr sourceSet)
{
	_RecordsetPtr a_ylSet;
	_variant_t key;
	double col_yl;
	double col_amount;
	CString col_unit;

	CString col_name;
	CString strSQL;
	a_ylSet.CreateInstance(__uuidof(Recordset));
	a_ylSet->Open(_bstr_t("select * from a_yl where enginid='"+EDIBgbl::SelPrjID+"'"),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);//9/21
	if(sourceSet->GetRecordCount()>0&&a_ylSet->GetRecordCount()>0)
	{
		sourceSet->MoveFirst();
		for(;!sourceSet->adoEOF;sourceSet->MoveNext())
		{
			key=sourceSet->GetCollect(_T("col_name"));
			if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
			{
				col_name=key.bstrVal;
				strSQL="col_name='"+col_name+"'";
				a_ylSet->MoveFirst();
				a_ylSet->Find(_bstr_t(strSQL),0,adSearchForward);
				if(!a_ylSet->adoEOF)
				{
					key=a_ylSet->GetCollect(_T("col_yl"));
					col_yl=(key.vt==VT_EMPTY||key.vt==VT_NULL)?0.0:key.dblVal;
					key=sourceSet->GetCollect(_T("col_amount"));
					col_amount=(key.vt==VT_EMPTY||key.vt==VT_NULL)?0.0:key.dblVal;
					col_amount=col_amount*(1+col_yl);
					sourceSet->PutCollect(_T("gen_amount"),_variant_t(col_amount));
					key=a_ylSet->GetCollect(_T("col_unit"));
					if(!(key.vt==VT_EMPTY||key.vt==VT_NULL))
					{
						col_unit=key.bstrVal;
						sourceSet->PutCollect(_T("col_unit"),_variant_t(col_unit));
					}
					sourceSet->Update();					
				}
				else
				{
					//��ʩ����������û�иò���,��ʩ������Ϊ�������.		//2005.4.2	ZSY
					sourceSet->PutCollect(_T("gen_amount"), sourceSet->GetCollect(_T("col_amount")));
					//û��Ĭ�ϵĵ�λ.

				}
			
			}
		}
	}

}


//�޸�pass ��
void CTotalTableIPED::passModiTatal(CString str1, CString str2, CString str3)
{
	_variant_t key;
	CString str;
	_RecordsetPtr pass_set;
	int flagPass=0;
	pass_set.CreateInstance(__uuidof(Recordset));
	pass_set->Open(_bstr_t(_T("select * from pass")),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
	if(pass_set->GetRecordCount()>0)
	{
		pass_set->MoveFirst();
		for(;!pass_set->adoEOF;pass_set->MoveNext())
		{
			if(flagPass==0)
			{
				pass_set->PutCollect(_variant_t(str3),_T("T"));
			}
			else
			{
				pass_set->PutCollect(_variant_t(str3),_T("F"));
			}
			key=pass_set->GetCollect(_variant_t(str1));
			if((!(key.vt==VT_NULL||key.vt==VT_EMPTY))&&(flagPass==0))
			{
				str=key.bstrVal;
				flagPass=(str.Compare(str2)==0)?1:0;
			}
		}
	}	
	if(pass_set->State==adStateOpen)
	{
		pass_set->Close();
	}

}


//------------------------------------------------------------------                
// DATE         :[2005/06/10]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :��׼���
//				pRsColr_l;	�����ᵥλ��������
//				pRsColor;	ɫ����ͷ������ɫ׼���
//				pRsArrow;	ɫ����ͷ�ߴ�׼���
//------------------------------------------------------------------
short CTotalTableIPED::OpenCodeDB()
{
	try
	{
		CString	strSQL;				//SQL���

		//�����ᵥλ��������
		if ( pRsColr_l == NULL )
		{
			pRsColr_l.CreateInstance(__uuidof(Recordset));
		}
		if ( pRsColr_l->GetState() == adStateOpen )
		{
			pRsColr_l->Close();
		}
		strSQL = "SELECT * FROM [a_colr_l] ";
		pRsColr_l->Open(_bstr_t(strSQL), (IDispatch*)m_pConnectionCODE,
						adOpenStatic, adLockOptimistic, adCmdText);

		//ɫ����ͷ������Ͽ⡣
		if (pRsRing_Paint == NULL)
		{
			pRsRing_Paint.CreateInstance(__uuidof(Recordset));
		}
		if (pRsRing_Paint->State == adStateOpen)
		{
			pRsRing_Paint->Close();
		}
		strSQL = "SELECT * FROM [a_ring_paint]";
		pRsRing_Paint->Open(_variant_t(strSQL), m_pConnectionCODE.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		
		//ɫ����ͷ������ɫ׼���
		if ( pRsColor == NULL )
		{
			pRsColor.CreateInstance(__uuidof(Recordset));
		}
		if ( pRsColor->GetState() == adStateOpen )
		{
			pRsColor->Close();
		}
		strSQL = "SELECT * FROM [a_color] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		pRsColor->Open(_bstr_t(strSQL), (IDispatch*)m_Pconnection,
						adOpenStatic, adLockOptimistic, adCmdText);

		//	ɫ����ͷ�ߴ�׼���
		if ( pRsArrow == NULL )
		{
			pRsArrow.CreateInstance(__uuidof(Recordset));
		}
		if ( pRsArrow->GetState() == adStateOpen )
		{
			pRsArrow->Close();
		}
		strSQL = "SELECT * FROM [a_arrow] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		pRsArrow->Open(_bstr_t(strSQL), (IDispatch*)m_Pconnection,
						adOpenStatic, adLockOptimistic, adCmdText);
		
	}
	catch (_com_error& e) 
	{
		AfxMessageBox(e.Description());
		return 0;
	}
	return 1;
}

//------------------------------------------------------------------
// DATE         :[2005/11/14]
// Parameter(s) :
// Return       :
// Remark       :����һ����ʱ�����ڼ���ɫ����ͷ������
//------------------------------------------------------------------
BOOL CTotalTableIPED::CreateTempTable(_ConnectionPtr pCon, CString strTblName)
{
	try
	{
		CString strSQL;
		if (pCon == NULL || strTblName.IsEmpty())
		{
			return FALSE;
		}
		if( !tableExists(m_Pconnection, strTblName) )//9/2
		{
			strSQL = "CREATE TABLE ["+strTblName+"] (DW double, D_ARROW double, C_ARROW double)";
			pCon->Execute(_bstr_t(strSQL), NULL, adCmdText);
		}
	}catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/12/23]
// Parameter(s) :
// Return       :
// Remark       :��һ����¼���У�����ָ�����ֶν��л���
//------------------------------------------------------------------
BOOL CTotalTableIPED::TotalSelfTable()
{
	try
	{
		_RecordsetPtr pRsSou;
		_RecordsetPtr pRsDes;
		pRsSou.CreateInstance( __uuidof(Recordset) );
		pRsDes.CreateInstance( __uuidof(Recordset) );
		CString strSQL;		// SQL���
		CString strTempTblName = "p";		//��ʱ����-
		if ( tableExists( m_Pconnection, strTempTblName ) )
		{
			m_Pconnection->Execute( _bstr_t("DROP TABLE ["+strTempTblName+"] "), NULL, -1 );
		}
		//�Է���������¼����
		strSQL = " SELECT [col_name],[col_size],[col_unit],[col_mark],sum(col_num) AS [col_num],\
			sum(col_amount) AS [col_amount] INTO "+strTempTblName+" FROM [E_PAICOL] \
			WHERE EnginID='"+EDIBgbl::SelPrjID+"' GROUP BY [col_name],[col_size],[col_unit],[col_mark] \
			ORDER BY [col_name],[col_size],[col_unit],[col_mark]";
		m_Pconnection->Execute( _bstr_t(strSQL), NULL, -1 );
		
		strSQL = " delete from e_paicol where enginid='"+EDIBgbl::SelPrjID+"'";
		m_Pconnection->Execute( _bstr_t(strSQL), NULL, -1 );
		if ( pRsDes->State  == adStateOpen )
		{
			pRsDes->Close();
		}
		pRsDes->Open(_variant_t("select * from [e_paicol] "), m_Pconnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText );

		strSQL = " SELECT * FROM ["+strTempTblName+"] ";
		pRsSou->Open(_bstr_t(strSQL),(IDispatch*)m_Pconnection,adOpenStatic,adLockOptimistic,adCmdText);
		if ( pRsSou->adoEOF &&pRsSou->BOF )
		{
			return TRUE;
		}
		// ��ͳһ���ܵ�����д�뵽�������
		for ( ; !pRsSou->adoEOF; pRsSou->MoveNext() )
		{
			pRsDes->AddNew();

			pRsDes->PutCollect( _variant_t("col_name"), pRsSou->GetCollect( _variant_t("col_name")));
			pRsDes->PutCollect( _variant_t("col_size"), pRsSou->GetCollect( _variant_t("col_size")));
			pRsDes->PutCollect( _variant_t("col_unit"), pRsSou->GetCollect( _variant_t("col_unit")));
			pRsDes->PutCollect( _variant_t("col_mark"), pRsSou->GetCollect( _variant_t("col_mark")));
			pRsDes->PutCollect( _variant_t("col_num"),  pRsSou->GetCollect( _variant_t("col_num")));
			pRsDes->PutCollect( _variant_t("col_amount"), pRsSou->GetCollect( _variant_t("col_amount")));

			pRsDes->PutCollect( _variant_t("EnginID"), _variant_t(EDIBgbl::SelPrjID));
			pRsDes->Update();
		}
		
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	return TRUE;
}