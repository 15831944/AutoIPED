// DlgSelCityWeather.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "DlgSelCityWeather.h"
#include "EDIBgbl.h"
#include "vtot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CAutoIPEDApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgSelCityWeather dialog


CDlgSelCityWeather::CDlgSelCityWeather(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelCityWeather::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelCityWeather)
	m_strStaticCaption = _T("��ǰ���̵��������:");
	//}}AFX_DATA_INIT
}


void CDlgSelCityWeather::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelCityWeather)
	DDX_Control(pDX, IDC_COMBO_CITY, m_ctlCity);
	DDX_Control(pDX, IDC_COMBO_PROVINCE, m_ctlProvince);
	DDX_Control(pDX, IDC_DATAGRID1, m_gridWeatherData);
	DDX_Text(pDX, IDC_STATIC_WEATHER, m_strStaticCaption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelCityWeather, CDialog)
	//{{AFX_MSG_MAP(CDlgSelCityWeather)
	ON_CBN_SELCHANGE(IDC_COMBO_PROVINCE, OnSelchangeComboProvince)
	ON_CBN_SELCHANGE(IDC_COMBO_CITY, OnSelchangeComboCity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelCityWeather message handlers
//��ʼ�������е�ʡ��
bool CDlgSelCityWeather::InitProvinceCombox()
{
	try
	{
		_RecordsetPtr pRsProvince;
		CString strSQL, strTmp;
		pRsProvince.CreateInstance(__uuidof(Recordset));
		
		strSQL = "SELECT * FROM [ʡ������] ORDER BY ProvinceIndex";
		pRsProvince->Open(_variant_t(strSQL), m_pConWeather.GetInterfacePtr(), 
					adOpenStatic, adLockOptimistic, adCmdText);
		
		m_ctlProvince.ResetContent();
		for(; !pRsProvince->adoEOF; pRsProvince->MoveNext() )
		{
			strTmp = vtos( pRsProvince->GetCollect(_variant_t("Province")));
			if( !strTmp.IsEmpty() )
			{
				m_ctlProvince.AddString(strTmp);
			}
		}
		if(	!m_strCurProvinceName.IsEmpty() )
		{	//��ʼ��λ���ӳ�������ȡ����ʡ����
			int nPos = m_ctlProvince.FindString(-1, m_strCurProvinceName);
			if( nPos != -1 )
			{
				m_ctlProvince.SetCurSel(nPos);
			}
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}

//�����������ݿ�
//��һ����Ա�������ӡ�
bool CDlgSelCityWeather::ConnectWeatherDB()
{
	if(	m_pConWeather == NULL )
	{
		m_pConWeather.CreateInstance(__uuidof(Connection));
	}
	try
	{
		CString strSQL, strDBName;
		strDBName = "�й������������.mdb";
		strSQL = CONNECTSTRING	+ EDIBgbl::sCritPath + strDBName;

		m_pConWeather->Open(_bstr_t(strSQL), "", "", -1 );
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}

	return true;
}

//
//���ݵ�ǰ��ʡ���г���Ӧ����.
bool CDlgSelCityWeather::UpdateCity()
{
	try
	{
		if( m_pRsCity->State == adStateClosed )
		{	
			//û�д����ݿ⡣
			//���´򿪡�
			if( !OpenWeatherDB_Table(m_pRsCity, "�й������������") )
			{
				return false;
			}
		}
		if(m_pRsCity->GetRecordCount() <= 0)
		{
			//û�м�¼
			return false;
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	CString strProv, strSQL, strCity;
	int nIndex = m_ctlProvince.GetCurSel();
	if( nIndex == -1 )
	{
		return false;
	}
	m_ctlProvince.GetLBText( nIndex, strProv);
	if( strProv.IsEmpty() )
	{
		return false;
	}
	try
	{
		m_ctlCity.ResetContent();
		m_pRsCity->Filter = _variant_t("");
		//���������ġ�������ǰʡ�ļ�¼��
		strSQL = "Province='"+strProv+"' ";
		m_pRsCity->Filter = _variant_t(strSQL);

		for( ; !m_pRsCity->adoEOF; m_pRsCity->MoveNext() )
		{
			strCity = vtos(m_pRsCity->GetCollect(_variant_t("City")));
			if( !strCity.IsEmpty() )
			{
				m_ctlCity.AddString(strCity);
			}
		}
		if( !m_strCurCityName.IsEmpty() )
		{	
			nIndex = m_ctlCity.FindString(-1, m_strCurCityName);
			if( -1 != nIndex )
			{
				m_ctlCity.SetCurSel(nIndex);
			}
			m_strCurCityName.Empty();
		}
		else
		{
			//��ʼ��һ����
			m_ctlCity.SetCurSel(0);
		}
		//ȡ������
		m_pRsCity->Filter = _variant_t("");
	}
	catch(_com_error& e)
	{
		AfxMessageBox( e.Description() );
		return false;
	}	
	return true;
}

//
//��������е�ĳ����
bool CDlgSelCityWeather::OpenWeatherDB_Table(_RecordsetPtr &pRs, CString strFilter)
{
	CString strSQL;
	try
	{
		if( pRs == NULL )
		{
			pRs.CreateInstance(__uuidof(Recordset));
		}
		strSQL = "SELECT * FROM ["+strFilter+"] ";

		if(	pRs->State == adStateOpen )
		{
			pRs->Close();
		}
		pRs->Open(_variant_t(strSQL), m_pConWeather.GetInterfacePtr(), 
			adOpenStatic, adLockOptimistic, adCmdText);

	}
	catch(_com_error& e)
	{
		AfxMessageBox( e.Description());
		return false;
	}
	return true;
}

BOOL CDlgSelCityWeather::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�����������ݿ�
	if( !ConnectWeatherDB() )
	{
		return false;
	}
	//���Ȼ�ó������е�ʡ����
	GetCurrentCityName();
	//��ʼʡ��Ͽ�
	InitProvinceCombox();
	//��ʼ�����
	if( !InitDataGridWeather() )
	{
		return false;
	}
	//	�򿪱�
	OpenWeatherDB_Table( m_pRsCity, "�й������������");

	//���³�����Ͽ�.
	if( UpdateCity() )
	{
	//	UpdateDataGridWeather();
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
//ѡ��ͬ��ʡ��������
void CDlgSelCityWeather::OnSelchangeComboProvince() 
{
	//���³���
	if( UpdateCity() )
	{
		UpdateDataGridWeather();
	}
}

//����ѡ��ĳ��и��¶�Ӧ��������
bool CDlgSelCityWeather::UpdateDataGridWeather()
{
	CString strProvince, strCity;
	int nIndex;
	//��ǰ��ʡ
	nIndex = m_ctlProvince.GetCurSel();
	if( nIndex == -1 )
	{
		return false;
	}
	m_ctlProvince.GetLBText(nIndex, strProvince);
	//����
	nIndex = m_ctlCity.GetCurSel();
	if( nIndex == -1 )
	{
		return false;
	}
	m_ctlCity.GetLBText(nIndex, strCity);

	//
	CString strSQL, strTmp;
	_variant_t var;

	try
	{
		strSQL = "Province='"+strProvince+"' and City='"+strCity+"' ";
		m_pRsCity->Filter = _variant_t(strSQL);
		if( !m_pRsCity->adoEOF && m_pRsWeather->GetRecordCount() > 0 )
		{
			//����ǰ���е��������ȡ������
			//�Ա���ȡ������е���������ж�Ӧ���ֶ���
			for(m_pRsWeather->MoveFirst(); !m_pRsWeather->adoEOF; m_pRsWeather->MoveNext() )
			{
				try
				{
					strTmp = vtos( m_pRsWeather->GetCollect(_variant_t("��Ӧ����������е��ֶ���")));
					if( !strTmp.IsEmpty() )
					{
						var = m_pRsCity->GetCollect(_variant_t(strTmp));
						strTmp = vtos(var);
						m_pRsWeather->PutCollect(_variant_t("ֵ"), _variant_t(strTmp));
					}
					else
					{
						//���Ϊ�ա���������Ĵ���
					}

				}
				catch(_com_error)
				{	
				}
			}
			//�ı���ʾ������
			m_strStaticCaption = "ѡ����е��������:";
			UpdateData(FALSE);
		}
		m_pRsCity->Filter = _variant_t("");
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}
//
//ѡ��ͬ�ĳ���
void CDlgSelCityWeather::OnSelchangeComboCity() 
{
	this->UpdateDataGridWeather();	
}

//��ʼ��ǰ���̵ĳ���
bool CDlgSelCityWeather::InitDataGridWeather()
{
	CString strSQL;
	m_strSourTblName = "Ta_Variable";	//�洢�������ݵı�.
	m_strTempTblName = "temp_table";	//��ʱ����.
	try
	{
		if( m_pRsWeather == NULL )
		{
			m_pRsWeather.CreateInstance(__uuidof(Recordset));
			m_pRsWeather->CursorLocation = adUseClient;
		}
		//����Ta_Variable��,��ǰ���̵������¼��һ����ʱ��.
		try
		{
			theApp.m_pConnectionCODE->Execute(_bstr_t("DROP TABLE "+m_strTempTblName+" "), NULL, adCmdText);
		}
		catch(_com_error& e)
		{
			if( e.Error() != DB_E_NOTABLE )
			{
				return false;
			}
		}
		strSQL = "SELECT * INTO "+m_strTempTblName+" FROM ["+m_strSourTblName+"] ";
		theApp.m_pConnectionCODE->Execute(_bstr_t(strSQL), NULL, adCmdText);

		//��������
//		theApp.m_pConnectionCODE->BeginTrans();

		strSQL = "SELECT Index AS ���, Ta_Variable_DESC AS ����, Ta_Variable_VALUE AS ֵ ,Ta_Variable_FieldName AS ��Ӧ����������е��ֶ��� FROM ["+m_strTempTblName+"] ";
		if( m_pRsWeather->State == adStateOpen )
		{
			m_pRsWeather->Close();
		}
		m_pRsWeather->Open(_variant_t(strSQL), theApp.m_pConnectionCODE.GetInterfacePtr(), 
					adOpenStatic, adLockOptimistic, adCmdText);

		m_gridWeatherData.SetRefDataSource( m_pRsWeather->GetDataSource());

		//��������
//		theApp.m_pConnectionCODE->RollbackTrans();
		//�ύ����
//		theApp.m_pConnectionCODE->CommitTrans();
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;	
}
 
//ȷ��,
//���浱ǰ������.
void CDlgSelCityWeather::OnOK() 
{
	CString strSQL;
	try
	{
		try
		{
			
			if( m_gridWeatherData.GetDataSource() != NULL )
			{	//�Զ��ƶ���,�����д��������ύ�����ݿ���,
				//���,�ı���DataGrid�ؼ������ݵ�û�л��о��˳������.
				m_gridWeatherData.SetBookmark(_variant_t((long)1));	
			}
		}
		catch(_com_error)
		{
		}
		//������ʽԭ���ļ�¼ɾ��.
		strSQL = "DELETE * FROM ["+m_strSourTblName+"] ";
		theApp.m_pConnectionCODE->Execute(_bstr_t(strSQL), NULL, adCmdText);
		//����ʱ���е����ݲ��뵽��Ӧ�Ĺ��̱���.
		strSQL = "INSERT INTO ["+m_strSourTblName+"] SELECT * FROM ["+m_strTempTblName+"]";
		theApp.m_pConnectionCODE->Execute(_bstr_t(strSQL), NULL, adCmdText);
		//ɾ����ʱ��
		strSQL = "DROP TABLE ["+m_strTempTblName+"] ";
		theApp.m_pConnectionCODE->Execute(_bstr_t(strSQL), NULL, adCmdText);
		//����������д�뵽�������С�
		CString strProvince, strCity;
		int nCity = m_ctlCity.GetCurSel(), nProvince = m_ctlProvince.GetCurSel();
		if( -1 != nCity && -1 != nProvince )
		{
			m_ctlCity.GetLBText(nCity, strCity);
			m_ctlProvince.GetLBText(nProvince, strProvince);

			strSQL = "UPDATE [Engin] SET ʡ='"+strProvince+"',��='"+strCity+"' WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
			theApp.m_pConAllPrj->Execute(_bstr_t(strSQL), NULL, adCmdText);
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
	}
	CDialog::OnOK();
}

//���ܣ�
//�ӳ������л��ʡ�����ơ�
bool CDlgSelCityWeather::GetCurrentCityName()
{
	CString strSQL;
	_RecordsetPtr pRsDef;
	pRsDef.CreateInstance(__uuidof(Recordset));
	try
	{
		strSQL = "SELECT * FROM [Engin] WHERE EnginID='"+EDIBgbl::SelPrjID+"' ";
		pRsDef->Open(_variant_t(strSQL), theApp.m_pConAllPrj.GetInterfacePtr(),
				adOpenStatic, adLockOptimistic, adCmdText);
		if( pRsDef->GetRecordCount() > 0 )
		{
			m_strCurProvinceName = vtos(pRsDef->GetCollect(_variant_t("ʡ")));
			m_strCurCityName	 = vtos(pRsDef->GetCollect(_variant_t("��")));
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
// DATE         : [2006/03/21]
// Author       : ZSY
// Parameter(s) : dTa[in] �ļ��������������������¶�(�����¶�) 
//				: dHumidity[in] ������ƽ�����ʪ��
//				: pRsPoint[in] �������ļ�¼�Ѿ���������֮���������´�
//				: dReDwePoint[out] ����õ�¶���¶�
// Return       : ���ɹ�����TRUE ���򷵻�FALS
// Remark       : �����ļ��������������������¶Ⱥ�������ƽ�����ʪ�Ȳ����¶���¶�
//------------------------------------------------------------------
BOOL CDlgSelCityWeather::GetDewPointTemp(_RecordsetPtr pRsPoint, double dTa, double dHumidity, double &dReDwePoint)
{
	try
	{
		CString strSQL;	// SQL���
		if (pRsPoint == NULL || pRsPoint->GetState() == adStateClosed)
		{
			// �����ٴ�
			pRsPoint.CreateInstance(__uuidof(Recordset));
			strSQL = "SELECT * FROM [DewPoint]";
			pRsPoint->Open(_variant_t(strSQL), theApp.m_pConWeather.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
		}
		if (pRsPoint->adoEOF && pRsPoint->BOF)
		{// ��¼Ϊ��
			return FALSE;
		}
		strSQL.Format("dTa=%f", dTa);
//		pRsPoint->Find(_bstr_t(strSQL), 0, adSearchForward);
		pRsPoint->PutFilter((long)adFilterNone);
		pRsPoint->PutFilter(_variant_t(strSQL));

		BOOL bFirst = TRUE;	// û�н���ѭ�� 
		double dPreVal = 0;	// ǰһ�ε�ֵ
		double dPreKey = 0;	// ǰһ�εĹؼ���
		double dCurVal;		// ��ǰ��ֵ
		double dCurKey;		// ��ǰ�Ĺؼ���

		for (; !pRsPoint->adoEOF; pRsPoint->MoveNext())
		{
			dCurVal = vtof(pRsPoint->GetCollect(_variant_t("Val")));
			dCurKey = vtof(pRsPoint->GetCollect(_variant_t("Key")));
			
			if (dCurKey >= dHumidity)
			{
				if (bFirst)
					dReDwePoint = dCurVal;	// ��Ҫ���ҵĹؼ���С�ڻ���ڱ�����С�Ĺؼ�ֵ
				else
					dReDwePoint = (dCurVal - dPreVal) / (dCurKey - dPreKey) * (dHumidity - dPreKey) + dPreVal;
				
				return TRUE;
			}
			bFirst = FALSE;
			dPreKey = dCurKey;
			dPreVal = dCurVal;
		}
		if ( bFirst == FALSE && pRsPoint->adoEOF)	// ���ҵĹؼ��ֳ����˼�¼��������
		{
			dReDwePoint = dPreVal;	// ʹ������ֵ
		}

	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
		
	}
	return TRUE;
}
