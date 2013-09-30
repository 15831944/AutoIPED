// ExplainTable.cpp: implementation of the CExplainTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "ExplainTable.h"
#include "EDIBgbl.h"
#include "vtot.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExplainTable::CExplainTable()
{

}

CExplainTable::~CExplainTable()
{

}
//���ܣ�     ���ɱ���˵����
//���ñ�   1. [pare_calc]
//			 2. [a_color]
//			 3. [pass]
//���ɱ�   [e_preexp]
bool CExplainTable::CreateBWTable()
{ 
	try
	{  
		::CoInitialize(NULL);
		_RecordsetPtr  pRs, pRsPre_calc;
    	pRs.CreateInstance(__uuidof(Recordset));
	//	pRs->CursorLocation = adUseClient;
    	pRsPre_calc.CreateInstance(__uuidof(Recordset));
	//	pRsPre_calc->CursorLocation = adUseClient;

		CString strSQL,        //SQL��䡣
				str; 
		_variant_t var;
		pos = 1;
		((CMainFrame* )::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
/*  8/31  �ж�PASS��*/
		strSQL = "SELECT * FROM [pass] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";

		pRs->Open(_variant_t(strSQL),(IDispatch*)m_Pconnection,\
			adOpenStatic, adLockOptimistic, adCmdText);

		if( pRs->adoEOF && pRs->BOF )
		{
			AfxMessageBox("׼�����û����Ϊ'"+EDIBgbl::SelPrjName+"'�Ĺ��̣� �������ɱ���˵����");
			return false;
		}
		str = "PASS_MOD1='C_PREEXP'";
		pRs->MoveFirst();
		pRs->Find(_bstr_t(str), 0, adSearchForward);      //��λ˵����ı�ǡ�
		if( pRs->adoEOF || pRs->BOF )
		{
			AfxMessageBox("���ݿ���󣬲������ɱ���˵����");
			return false;
		}
		var = pRs->GetCollect("PASS_LAST1");
		CString	strLastMod = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""): (CString)var.bstrVal;

		str="PASS_MOD1='"+strLastMod+"'";
		pRs->MoveFirst();
		pRs->Find(_bstr_t(str), 0, adSearchForward);          //��λ����˵�����ǰһ�������
	
		if( pRs->adoEOF || pRs->BOF )
		{
			AfxMessageBox("���ݿ���󣬲������ɱ���˵����");
			return false;
		}
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		CString strPassMark;
		var = pRs->GetCollect("PASS_MARK1");
		strPassMark = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""): (CString)var.bstrVal;
		//////////////////////////////////////////////////////////////////////////
		strPassMark = _T("T");		// ����˵����������ģ�����
		//////////////////////////////////////////////////////////////////////////
		
		strPassMark.MakeUpper();
		if( strPassMark != _T("T") )
		{
			strSQL = "SELECT DISTINCT LEFT(C_VOL,5) AS C_VOL FROM [pre_calc] WHERE EnginID='"+EDIBgbl::SelPrjID+"' AND (c_amount=0 OR c_amount IS NULL) ";	
			if( pRsPre_calc->State == adStateOpen )
			{
				pRsPre_calc->Close();
			}
			pRsPre_calc->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection, adOpenStatic, adLockOptimistic, adCmdText);
			CString strMss;
			for( int i=1; !pRsPre_calc->adoEOF; pRsPre_calc->MoveNext(), i++ )
			{
				var = pRsPre_calc->GetCollect("C_VOL");
				if( var.vt == VT_NULL || var.vt == VT_EMPTY )
				{
					i--;
					continue;
				}
				if( i == 8 )
				{
					strMss += (CString)var.bstrVal+"\n";
					i = 0;
				}
				else
				{
					strMss += (CString)var.bstrVal+"\t";
				}
			}
			if( strMss.IsEmpty() )
			{
				AfxMessageBox("���Ƚ��м��㣡");
				return false;
			}
			strMss += "\n\n���ϱ�������δ�༭��, �������ɱ���˵����!";
			AfxMessageBox(strMss);
			return false;
		}
		strSQL = "SELECT * FROM [pre_calc] WHERE EnginID = '"+EDIBgbl::SelPrjID+"' ORDER BY ID ";
		if( pRs->State == adStateOpen )
		{
			pRs->Close();
		}
		pRs->Open( _variant_t(strSQL), (IDispatch*)m_Pconnection, adOpenStatic, adLockOptimistic, adCmdText);
		if( pRs->RecordCount <= 0 )
		{
			AfxMessageBox("��[pre_calc]����û��' "+EDIBgbl::SelPrjID+" '���̵ļ�¼���������ɱ���˵����");
			return false;
		}
		//���ֶ�C_NUM��ID��Ϊ��ͬ����. (�����ֶεĺ�����ͬ)
		int lastC_NUM, lastID;
		pRs->MoveLast();
		lastID = vtoi( pRs->GetCollect(_variant_t("ID")) );
		lastC_NUM = vtoi( pRs->GetCollect(_variant_t("c_num")) );

		if( lastID != lastC_NUM )
		{
			pRs->MoveFirst();
			while( !pRs->adoEOF )
			{
				pRs->PutCollect(_variant_t("C_NUM"), pRs->GetCollect(_variant_t("ID")) );
				pRs->MoveNext();
			}
		}
		//

		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		/////����[pre_calc]��ǰ���̵ļ�¼���Ƶ���[e_preexp]��
		strSQL = "DELETE  FROM [e_preexp] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);

		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);

		strSQL = "INSERT INTO [e_preexp] (ID,���,c_vol,����,���,c_pi_thi,�¶�t";
		strSQL += ",c_wind,�Ᵽ�²���,��������,�Ᵽ�º�,cal_thi,������,�ص�";
		strSQL += ",c_price,c_hour,c_rate,�¶�t0,c_pre_wei,c_pro_wei,c_pipe_wei,c_wat_wei";
		strSQL += ",c_with_wat,c_no_wat,����,c_area,��ע,c_srsb";
		strSQL += ",cal_srsb,c_lost,������¶�,�ⵥ���,��ȫ���,���������";
		strSQL += ",����ȫ���,c_pi_mat,c_distan,c_pg,������";
		strSQL += ",c_pass,c_steam,c_p_s,�ڱ��²���,�ڱ��º�";
		strSQL += ",c_in_wei,�ڵ����,��ȫ���,c_ts,c_color,EnginID)";

		strSQL += "	SELECT ID,c_num AS ���,c_vol,c_name1 AS ����,c_size AS ���,c_pi_thi,c_temp AS �¶�t";
		strSQL += ",c_wind,c_name2 AS �Ᵽ�²���,c_name3 AS ��������,c_pre_thi AS �Ᵽ�º�,cal_thi,c_pro_thi AS ������,c_place AS �ص�";
		strSQL += ",c_price,c_hour,c_rate,c_con_temp AS �¶�t0,c_pre_wei,c_pro_wei,c_pipe_wei,c_wat_wei";
		strSQL += ",c_with_wat,c_no_wat,c_amount AS ����,c_area,c_mark AS ��ע,c_srsb";
		strSQL += ",cal_srsb,c_lost,c_tb3 AS ������¶�,c_v_pre AS �ⵥ���,c_tv_pre AS ��ȫ���,c_v_pro AS ���������";
		strSQL += ",c_tv_pro AS ����ȫ���,c_pi_mat,c_distan,c_pg,c_area*c_amount AS ������";
		strSQL += ",c_pass,c_steam,c_p_s,c_name_in AS �ڱ��²���,c_in_thi AS �ڱ��º�";
		strSQL += ",c_in_wei,c_v_in AS �ڵ����,c_tv_in AS ��ȫ���,c_ts,c_color,EnginID";
		
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);

		strSQL += "	FROM [pre_calc] WHERE EnginID='"+EDIBgbl::SelPrjID+"' AND C_PASS='Y' "; //2005.3.7
		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);

		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		strSQL = "SELECT DISTINCT colr_code, colr_arrow,colr_ring FROM [a_color] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		if(pRs->State == adStateOpen)
		{
			pRs->Close();
		}
		pRs->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection,
			    adOpenStatic, adLockOptimistic, adCmdText);

		pRs->MoveFirst();
		int colrCode;
		CString colrRing, colrArrow,strColrCode;
		//��ɫ������ͷ�����ֶθ�ֵ������ֵ�ӱ�a_color��ȡcolr_ring,colr_arrow�ֶΣ�
		while( !pRs->adoEOF )
		{
			var = pRs->GetCollect("colr_code");
			colrCode = vtoi(var);
			strColrCode.Format("%d",colrCode);			
			var = pRs->GetCollect("colr_ring");
			colrRing = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(" "): (CString)var.bstrVal;

			var = pRs->GetCollect("colr_arrow");
			colrArrow = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(" "): (CString)var.bstrVal;

			if( !colrRing.Compare("N") )
			{
				colrRing = "��";
			}
			if( !colrArrow.Compare("N") )
			{
				colrArrow = "��";
			}
			strSQL = "UPDATE [e_preexp] SET ɫ��='"+colrRing+"' , ��ͷ='"+colrArrow+"'  WHERE EnginID='"+EDIBgbl::SelPrjID+"' AND c_color="+strColrCode+" ";
			m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
			pRs->MoveNext();
			if( pos<100 )
			{
				((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
			}
		}
/* 8/31 ȡ��дpass��8/31	*/
		strSQL = "SELECT * FROM [pass] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		if ( pRs->State == adStateOpen )
		{
			pRs->Close();
		}
		pRs->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection,
				 adOpenStatic, adLockOptimistic, adCmdText);
		pRs->MoveFirst();
		////�����ɱ���˵����֮ǰ�Ĳ���״̬�á�T��
		bool bPass = true;
		while( !pRs->adoEOF )
		{
			var = pRs->GetCollect("PASS_MOD1");		
			str = (var.vt == VT_NULL||var.vt==VT_EMPTY)?_T("") : (CString)var.bstrVal;
			if( bPass )
			{
				pRs->Fields->GetItem("PASS_MARK1")->PutValue(_variant_t("T"));
			}else
			{
				pRs->Fields->GetItem("PASS_MARK1")->PutValue(_variant_t("F"));//֮��Ĳ����á�F��
			}
			if( !str.Compare("C_PREEXP") )
			{			
				bPass = false;
			}
		    pRs->MoveNext();
			if( pos< 100 )
			{
				((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
			//	Sleep(20);
			}
		}
		while( pos< 100 )
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		//	Sleep(20);
		}

//		AfxMessageBox("���ɱ���˵���ɹ���");
		return true;	
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

//��������˵����
bool CExplainTable::CreatePaintTable()
{
	try
	{
		::CoInitialize(NULL); 
		_RecordsetPtr pRsPass,     //pass��ļ�¼����
					  pRs;      //������
		pRs.CreateInstance(__uuidof(Recordset));
	//	pRs->CursorLocation = adUseClient;
		pRsPass.CreateInstance(__uuidof(Recordset));
	//	pRsPass->CursorLocation = adUseClient;

		CString strSQL, //SQL��䡣
				str;     
		_variant_t  var;
		pos = 1;
/* 8/31 PASS��   8/31 */
		strSQL = "SELECT * FROM [pass] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		if( pRsPass->State == adStateOpen )
		{
			pRsPass->Close();
		}
		pRsPass->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection,
				  adOpenStatic, adLockOptimistic, adCmdText);
		if( pRsPass->adoEOF && pRsPass->BOF )
		{
			AfxMessageBox("׼�����û����Ϊ'"+EDIBgbl::SelPrjName+"'�Ĺ���, ������������˵����!");
			return false;
		}
/*		// �ж��������ʵ��г���������=0�ļ�¼��������������˵����");

		strSQL = "PASS_MOD2='C_PAIEXP'";
		//���α궨λ��һ����¼��Ȼ��Ž��в��ҡ�
		pRsPass->MoveFirst();       
		pRsPass->Find(_bstr_t(strSQL), 0, adSearchForward);

		var = pRsPass->GetCollect("PASS_LAST2");
		str = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""): (CString)var.bstrVal;
		strSQL = "PASS_MOD2='"+str+"'";
		pRsPass->MoveFirst();
		pRsPass->Find(_bstr_t(strSQL), 0, adSearchForward);
		
		var = pRsPass->GetCollect("PASS_MARK2");
		str = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""): (CString)var.bstrVal;
		str.MakeUpper();
		if( str != "T" )   //
		{
			AfxMessageBox("�������ʵ��г���������=0�ļ�¼��\n������������˵����");
			return false;
		}
*/

/*	
		//���ܱ���˵����Ŀ��ơ�
		strSQL = "PASS_MOD1='E_PREEXP'";
		pRsPass->MoveFirst();
		pRsPass->Find(_bstr_t(strSQL), 0, adSearchForward);

		var = pRsPass->GetCollect("PASS_MARK1");
		str = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""): (CString)var.bstrVal;
		str.MakeUpper();
	 	if( str != "T" )
		{
			AfxMessageBox("����˵����δ�༭��������������˵����!");
			return false;
		}

*/	
		
		//////////////////////////////////////////////////////////
/*		if( pRs->State == adStateOpen )				by zsy    ����ԭʼ����Ϊ�յ�ʱ�򣬿��ܻ��б���������Ҫ�����.���Բ��ܷ���      [2005/09/08]
		{
			pRs->Close();
		}
		strSQL = "SELECT * FROM [paint] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
		pRs->Open(_bstr_t(strSQL), (IDispatch*)m_Pconnection, adOpenStatic, adLockOptimistic, adCmdText);
		if( pRs->RecordCount <= 0 )
		{
			AfxMessageBox("����ԭʼ����Ϊ��,�����������ԭʼ��¼Ȼ�������ɣ�");
//			return false;
		}
*/
		//���T120���еĵ�ǰ�����еļ�¼��
		EmptyT120Table();

		//���ú���moduleT120()����t120��]
		if ( gbAutoPaint120 )
		{
			if( !moduleT120() )
			{
				return false;
			}
		}
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		////////////////
		strSQL = "DELETE FROM [e_paiexp] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);

		//////////��paint���е���Ӧ���̵����ݼ��뵽e_paiexp���С�{{  1
		strSQL = "INSERT INTO [e_paiexp] (ID, PAI_VOL, ���";
		strSQL += ",����, PAI_TYPE, PAI_SIZE, PAI_AMOU, �������, ��ע";
		strSQL += ",������, �������, ��ÿ������, �׺ϼ�����, ������";
		strSQL += ",�������, ��ÿ������, ��ϼ�����, PAI_A1, PAI_A_T1, PAI_A2";
		strSQL += ",PAI_A_T2, PAI_AREA, PAI_A_C1, PAI_A_C2, PAI_CODE, PAI_C_FACE, EnginID)";
		/////////paint���е��ֶ�	//ID AS ���
		strSQL += " SELECT ID, PAI_VOL, ID AS ���";
		strSQL += ",PAI_NAME AS ����, PAI_TYPE, PAI_SIZE, PAI_AMOU, PAI_P_AREA AS �������, PAI_MARK AS ��ע";
		strSQL += ",PAI_NAME1 AS ������, PAI_TIMES1 AS �������, PAI_COST1 AS ��ÿ������, PAI_TCOST1 AS �׺ϼ�����, PAI_NAME2 AS ������";
		strSQL += ",PAI_TIMES2 AS �������, PAI_COST2 AS ��ÿ������, PAI_TCOST2 AS ��ϼ�����, PAI_A1, PAI_A_T1, PAI_A2";
		strSQL += ",PAI_A_T2, PAI_AREA, PAI_A_C1, PAI_A_C2, PAI_CODE, PAI_C_FACE, EnginID";

		strSQL += " FROM [paint] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";

		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText); /////  1 }}

		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);

		//e_paiexp�����½� ����,����,��� ���б�š�  2005.3.12
		short NO = 1;
//		strSQL = "SELECT ���  FROM [e_paiexp] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ORDER BY PAI_VOL,PAI_SIZE,���� ";
		strSQL = "SELECT  ��� FROM [e_paiexp] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ORDER BY ���";
		if(pRs->GetState() == adStateOpen )
		{
			pRs->Close();
		}
		pRs->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection,
				adOpenStatic, adLockOptimistic, adCmdText);
		if( !pRs->adoEOF && !pRs->BOF )
		{
		/*	for(pRs->MoveFirst(); !pRs->adoEOF ; pRs->MoveNext(), NO++)
			{
				pRs->PutCollect(_variant_t("���"), _variant_t(NO));
				pRs->Update();
			}
		*/
			pRs->MoveLast();
			NO = vtoi(pRs->GetCollect(_variant_t("���")));
			NO++;
		}
		//��t120���������򣬽��������Ž��б��
		strSQL = "SELECT ���  FROM [T120] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ORDER BY ���� ";
		if(pRs->GetState() == adStateOpen )
		{
			pRs->Close();
		}
		pRs->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection,
				adOpenStatic, adLockOptimistic, adCmdText);
		if( !pRs->adoEOF && !pRs->BOF )
		{
			for( pRs->MoveFirst(); !pRs->adoEOF; NO++, pRs->MoveNext() )
			{
				pRs->PutCollect(_variant_t("���"), _variant_t(NO));
				pRs->Update();
			}
		}
		if(pRs->GetState() == adStateOpen )
		{
			pRs->Close();
		}

		//

		//////////��t120���е���Ӧ���̵����ݼ��뵽e_paiexp���С�{{  2
		CString strSQL1;
		strSQL1 = "INSERT INTO [e_paiexp] ( PAI_VOL, ���";
		strSQL1 += ",����, PAI_TYPE, PAI_SIZE, PAI_AMOU, �������, ��ע";
		strSQL1 += ",������, �������, ��ÿ������, �׺ϼ�����, ������";
		strSQL1 += ",�������, ��ÿ������, ��ϼ�����, PAI_A1, PAI_A_T1, PAI_A2";
		strSQL1 += ",PAI_A_T2, PAI_AREA, PAI_A_C1, PAI_A_C2, EnginID)";
		//////////t120���е��ֶΡ�
		strSQL1 += " SELECT PAI_VOL, ���";
		strSQL1 += ",����, PAI_TYPE, PAI_SIZE, PAI_AMOU, �������, ��ע";
		strSQL1 += ",������, �������, ��ÿ������, �׺ϼ�����, ������";
		strSQL1 += ",�������, ��ÿ������, ��ϼ�����, PAI_A1, PAI_A_T1, PAI_A2";
		strSQL1 += ",PAI_A_T2, PAI_AREA, PAI_A_C1, PAI_A_C2, EnginID";

		strSQL1 += " FROM [T120] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";

		m_Pconnection->Execute(_bstr_t(strSQL1), NULL, adCmdText); ////  2}}

		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		strSQL = "SELECT DISTINCT colr_code, colr_face FROM [a_color] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		if(pRs->GetState() == adStateOpen )
		{
			pRs->Close();
		}
		pRs->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection,
				adOpenStatic, adLockOptimistic, adCmdText);
		if( pRs->adoEOF && pRs->BOF )
		{
			AfxMessageBox("������������˵��������ⱻ�ƻ���������ѡ�񹤳̣�");
			return false;
		}
		int code;
		CString  strCode, colrFace;
		while( !pRs->adoEOF )
		{
			var = pRs->GetCollect("COLR_CODE");
			code = (var.vt==VT_NULL || var.vt==VT_EMPTY)?0: (int)var.dblVal;
			strCode.Format("%d",code);

			var = pRs->GetCollect("COLR_FACE");
			colrFace = (var.vt==VT_NULL || var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
			if( !colrFace.CompareNoCase("N") )
			{
				colrFace = _T(" ");
			}
			strSQL = "UPDATE [e_paiexp] SET pai_c_face='"+colrFace+"' WHERE EnginID='"+EDIBgbl::SelPrjID+"' AND pai_code="+strCode+"";
			m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
			
			pRs->MoveNext();
			if( pos < 100 )
			{
				((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
			}
			else
				pos = 1;
		}
/*	8/31  PASS��8/31 */
		pRsPass->MoveFirst();
		bool bPass = true;
		CString strTab;
		while( !pRsPass->adoEOF )       //����ģ��ǰ�����в���״̬��Ϊͨ����֮�����ΪF.
		{
			var = pRsPass->GetCollect("PASS_MOD2");
			strTab = (var.vt==VT_NULL || var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
			if( bPass )
			{
				pRsPass->Fields->GetItem("PASS_MARK2")->PutValue(_variant_t("T"));
			}else
			{
				pRsPass->Fields->GetItem("PASS_MARK2")->PutValue(_variant_t("F"));
			}
			if( !strTab.Compare("C_PAIEXP") )
			{
				bPass=false;
			}
			pRsPass->MoveNext();
		}
		if( pRsPass->State == adStateOpen )
			pRsPass->Close();
		pRsPass.Release();

		if( pRs->State == adStateOpen )
			pRs->Close();
		while( pos < 100 )
		{
			((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		}
		//		AfxMessageBox("��������˵����ɹ���");
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

BOOL CExplainTable::EmptyT120Table() const
{
	CString strSQL;
	strSQL.Format( _T("DELETE FROM [T120] WHERE EnginID='%s' "), EDIBgbl::SelPrjID );
	m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
	return TRUE;
}

//����T120��
BOOL CExplainTable::moduleT120()
{
	try
	{
		_RecordsetPtr  pRs,					//��ʱ���¼��        
			pRsT120ls,			//Table120ls��ļ�¼����
			pT120;				//T120��ļ�¼����
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;
		pRsT120ls.CreateInstance(__uuidof(Recordset));
		pRsT120ls->CursorLocation = adUseClient;
		pT120.CreateInstance(__uuidof(Recordset));
		pT120->CursorLocation = adUseClient;

		CString strSQL, strOldName, strName;
		double   areaSum=0, fArea=0;
		int      flg=1;
		_variant_t  var;
		//����һ����ʱ��Table120ls��
		if( !IsTableExists(m_Pconnection,"Table120ls") )
		{
			strSQL = "CREATE TABLE [Table120ls] (���� char,������  double, ��� double, ���� double, C_PI_THI double)";
			m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
		}
		//�����ʱ��
		strSQL = "DELETE FROM [Table120ls]";
		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
		//
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		//����[e_preexp]�е�ֵ�浽��ʱ���С�
		strSQL = "INSERT INTO [Table120ls] SELECT ����,������ ,��� ,���� ,C_PI_THI FROM [e_preexp] WHERE EnginID='"+EDIBgbl::SelPrjID+"' AND �¶�t<=120 ";
		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);

		//�޸��ֶ�'������'��
		strSQL = "UPDATE [Table120ls] SET ������=���*3.1415926*����/1000 WHERE C_PI_THI<>0";
		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);

		if( pRsT120ls->State == adStateOpen )
		{
			pRsT120ls->Close();
		}
		strSQL = "SELECT trim(����) AS ����, ������ FROM [Table120ls] ORDER BY trim(����)";
		pRsT120ls->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection, adOpenStatic, adLockOptimistic, adCmdText);
		if( pRsT120ls->RecordCount <= 0 )
			return EmptyT120Table();

		//���T120���еĵ�ǰ�����еļ�¼�����������µļ�¼��
		EmptyT120Table();
		strSQL = "SELECT * FROM [T120] WHERE EnginID='"+EDIBgbl::SelPrjID+"'";
		pT120->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection, adOpenStatic, adLockOptimistic, adCmdText);
	
		((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);
		pRsT120ls->MoveFirst();
		while( !pRsT120ls->adoEOF )
		{
			var = pRsT120ls->GetCollect("����");
			strName = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
			strName.TrimRight();
			var = pRsT120ls->GetCollect("������");
			fArea = (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal;
			if( flg==1 )
			{
				strOldName = strName;
				areaSum = fArea;
				flg=0;
				pRsT120ls->MoveNext();
				continue;
			}
			if( !strName.Compare(strOldName) ) //������ͬ������������ֵ��͡�
			{
				areaSum += fArea;
			}
			else                             //�������ɵļ�¼���ӵ�T120���С�
			{
				pT120->AddNew();
				pT120->PutCollect(_T("����"), _variant_t(strOldName));
				pT120->PutCollect(_T("�������"), _variant_t(areaSum));
				pT120->PutCollect(_T("PAI_TYPE"), _variant_t("��120��ܵ�"));
				pT120->PutCollect(_T("EnginID"), _variant_t(EDIBgbl::SelPrjID));
				pT120->Update();
				strOldName = strName;
				areaSum = fArea;
			}
			pRsT120ls->MoveNext();
			if( pos < 100 )
			{
				((CMainFrame*)::AfxGetMainWnd())->m_wndStatusBar.OnProgress(pos++);			
			}
			else
			{
				pos = 1;
			}
		}
		if( !flg )      //�����һ�����ƴ���
		{
			pT120->AddNew();
			pT120->PutCollect(_T("����"), _variant_t(strOldName));
			pT120->PutCollect(_T("�������"), _variant_t(areaSum));
			pT120->PutCollect(_T("PAI_TYPE"), _variant_t("��120��ܵ�"));
			pT120->PutCollect(_T("EnginID"), _variant_t(EDIBgbl::SelPrjID));
			pT120->Update();
		}
		if( pRs->State == adStateOpen )
		{
			pRs->Close();
		}
		strSQL = "SELECT * FROM [a_pai] WHERE EnginID='"+EDIBgbl::SelPrjID+"' AND PAI_TYPE='��120��ܵ�'";
		pRs->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection, adOpenStatic, adLockOptimistic, adCmdText);
		if( pRs->GetRecordCount() <=0 )
		{
			AfxMessageBox("�������û������Ϊ����120��ܵ����ļ�¼��");
			return false;
		}
		CString str_N1, str_N2, str_A1, str_A2;
		CString strd_T1, strd_C1, strd_T2, strd_C2, strd_A_T1, strd_A_T2;
		//�ӱ�a_paiȡ����Ӧ��ֵ��
		var    = pRs->GetCollect("PAI_N1");       //������        
		str_N1 = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(" "):(CString)var.bstrVal;

		var    = pRs->GetCollect("PAI_N2");			//������
		str_N2 = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(" "):(CString)var.bstrVal;

		var    = pRs->GetCollect("PAI_A1");			//PAI_A1
		str_A1 = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(" "):(CString)var.bstrVal;

		var    = pRs->GetCollect("PAI_A2");			//PAI_A2
		str_A2 = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(" "):(CString)var.bstrVal;

		var  = pRs->GetCollect("PAI_T1");           //��������������תΪ�ַ����� //�������
		strd_T1.Format("%f", (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal);

		var  = pRs->GetCollect("PAI_C1");			//��ÿ������
		strd_C1.Format("%f", (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal);

		var  = pRs->GetCollect("PAI_T2");			//�������
		strd_T2.Format("%f", (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal);

		var  = pRs->GetCollect("PAI_C2");			//
		strd_C2.Format("%f", (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal);

		var  = pRs->GetCollect("PAI_A_T1");			//
		strd_A_T1.Format("%f", (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal);

		var  = pRs->GetCollect("PAI_A_T2");			//
		strd_A_T2.Format("%f", (var.vt==VT_NULL||var.vt==VT_EMPTY)?0.0:var.dblVal);

		strSQL = "UPDATE [T120] SET ������='"+str_N1+"', �������="+strd_T1+" ";
		strSQL += ", ��ÿ������="+strd_C1+", ������='"+str_N2+"', �������="+strd_T2+" ";
		strSQL += ", ��ÿ������="+strd_C2+", PAI_A1='"+str_A1+"', PAI_A_T1="+strd_A_T1+" ";
		strSQL += ", PAI_A2='"+str_A2+"', PAI_A_T2="+strd_A_T2+" ";
		strSQL += " WHERE EnginID='"+EDIBgbl::SelPrjID+"' AND PAI_TYPE='��120��ܵ�'";

		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
		//
		strSQL = "UPDATE [T120] SET �׺ϼ�����=�������*��ÿ������*������� ";
		strSQL += ", ��ϼ�����=�������*��ÿ������*������� ";
		strSQL += ", PAI_A_C1=PAI_A_T1*�������, PAI_A_C2=PAI_A_T2*������� ";
		strSQL += "  WHERE EnginID='"+EDIBgbl::SelPrjID+"'";

		m_Pconnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

//�жϸñ��Ƿ����
BOOL CExplainTable::IsTableExists(_ConnectionPtr pCon, CString tb)
{
	if(pCon==NULL || tb=="")
		return false;
	_RecordsetPtr rs;
	if(tb.Left(1)!="[")
		tb="["+tb;
	if(tb.Right(1)!="]")
		tb+="]";
	try{
		rs=pCon->Execute(_bstr_t(tb), NULL, adCmdTable);
		rs->Close();
		return true;
	}
	catch(_com_error e)
	{
		return false;
	}

}
bool CExplainTable::EditBWTable()
{
	if( !IsPass("pass_mod1", "E_PREEXP", "pass_last1", "pass_mark1") )
	{
		AfxMessageBox("����˵����δ���ɣ����ܱ༭��");
		return false;
	}
	WritePass("pass_mod1","E_PREEXP","pass_mark1");
	return true;
}
//���ܡ��ж�PASS���еģ�strModeģ���Ƿ�ͨ����ͨ������true
//strField:Ϊ��ͬ��ģ���Ӧ���ֶΣ�pass_mod1��pass_mod2...)
//strMode:��ǰģ�顣
//pass_Last: ǰһ��ģ���ֶΡ�
//pass_mark��Ϊ��ͬ��ģ���Ӧ��״̬��

bool CExplainTable::IsPass(CString strField, CString strMode, CString pass_Last, CString pass_mark, bool flg, CString strCur)
{
	try{

		_RecordsetPtr pRsPass;
		pRsPass.CreateInstance(__uuidof(Recordset));
		_variant_t var;
		CString strSQL, strLastMode, strTmp;
		strSQL = "SELECT * FROM [PASS] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
		pRsPass->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection, adOpenStatic, adLockOptimistic,adCmdText);
		
		if( pRsPass->adoEOF && pRsPass->BOF )
		{
			return false;	
		}
		strTmp = ""+strField+"='"+strMode+"'";
		pRsPass->Find(bstr_t(strTmp), 0, adSearchForward);
		if( pRsPass->adoEOF )
		{
			return false;
		}
		var = pRsPass->GetCollect(_variant_t(pass_Last));
		strLastMode = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
		//�ж�strModeģ���ǰһ�β�����
		pRsPass->MoveFirst();
		strTmp = ""+strField+"='"+strLastMode+"'";
		pRsPass->Find(bstr_t(strTmp), 0, adSearchForward);
		if( pRsPass->adoEOF )
		{
			return false;
		}
		var = pRsPass->GetCollect(_variant_t(pass_mark));
		strLastMode = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;	
		strLastMode.TrimRight();
		strLastMode.MakeUpper();

		if( strLastMode != "T" )
		{
			return false;
		}
		//�ж��ܷ���������ʱ����������������һ��������
		if( flg && !strCur.IsEmpty() )
		{
			pRsPass->MoveFirst();
			strTmp = "pass_mod1='"+strCur+"'";
			pRsPass->Find(_bstr_t(strTmp), 0, adSearchForward);
			if( pRsPass->adoEOF )
			{
				return false;
			}
			var = pRsPass->GetCollect("pass_mark1");
			strLastMode = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
			strLastMode.TrimRight();
			strLastMode.MakeUpper();
			if( strLastMode != "T" )
			{
				return false;
			}
		}
		pRsPass->Close();
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}
//���ܡ�дPASS��
//pass_mode:Ϊ��ͬ��ģ���Ӧ���ֶΣ�pass_mod1��pass_mod2...)
//strMode:��ǰģ�顣
//pass_mark��Ϊ��ͬ��ģ���Ӧ��״̬��
bool CExplainTable::WritePass(CString pass_mode, CString strMode, CString pass_mark)
{
	try
	{
		_RecordsetPtr pRsPass;
		pRsPass.CreateInstance(__uuidof(Recordset));
		CString strSQL, strTmp;
		_variant_t var;
		bool flg = true;
		strMode.MakeUpper();
		strSQL = "SELECT * FROM [PASS] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";

		pRsPass->Open(_variant_t(strSQL), (IDispatch*)m_Pconnection, adOpenStatic, adLockOptimistic, adCmdText);
		if( pRsPass->adoEOF && pRsPass->BOF )
		{
			return false;
		}
		pRsPass->MoveFirst();
		while( !pRsPass->adoEOF )
		{
			var = pRsPass->GetCollect(_bstr_t(pass_mode));
			strTmp = (var.vt==VT_NULL||var.vt==VT_EMPTY)?_T(""):(CString)var.bstrVal;
			if( flg )
			{
				pRsPass->PutCollect(bstr_t(pass_mark), "T");
				pRsPass->Update();
				strTmp.MakeUpper();
				if( strTmp == strMode )
				{
					flg = false;
				}
			}
			else
			{
				pRsPass->PutCollect(bstr_t(pass_mark), "F");
				pRsPass->Update();
			}
			pRsPass->MoveNext();
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

bool CExplainTable::EditPaint()
{
	if( !IsPass("pass_mod2", "E_PAIEXP", "pass_last2", "pass_mark2") )
	{
		AfxMessageBox("����˵����δ����,���ܱ༭!");
		return false;
	}
	WritePass("pass_mod2", "E_PAIEXP", "pass_mark2");
	return true;
}
