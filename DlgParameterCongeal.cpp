// DlgParameterCongeal.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "DlgParameterCongeal.h"
#include "vtot.h"
#include "foxbase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgParameterCongeal dialog


CDlgParameterCongeal::CDlgParameterCongeal(CWnd* pParent /*=NULL*/)
	: CXDialog(CDlgParameterCongeal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgParameterCongeal)
	m_dFlux = 0.0;
	m_dMediumCongealTemp = 0.0;
	m_dMediumHeat = 0.0;
	m_dMediumMeltHeat = 0.0;
	m_dPipeDensity = 0.0;
	m_dPipeHeat = 0.0;
	m_dStopTime = 0.0;
	m_dMediumDensity = 0.0;

	m_pConCongeal = NULL;
	//}}AFX_DATA_INIT
	
}


void CDlgParameterCongeal::DoDataExchange(CDataExchange* pDX)
{
	CXDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgParameterCongeal)
	DDX_Control(pDX, IDC_COMBO_HEAT_LOSS_DATA, m_ctlHeatLossData);
	DDX_Text(pDX, IDC_EDIT_FLUX, m_dFlux);
	DDX_Text(pDX, IDC_EDIT_MEDIUM_CONGEAL_TEMP, m_dMediumCongealTemp);
	DDX_Text(pDX, IDC_EDIT_MEDIUM_HEAT, m_dMediumHeat);
	DDX_Text(pDX, IDC_EDIT_MEDIUM_MELT_HEAT, m_dMediumMeltHeat);
	DDX_Text(pDX, IDC_EDIT_PIPE_DENSITY, m_dPipeDensity);
	DDX_Text(pDX, IDC_EDIT_PIPE_HEAT, m_dPipeHeat);
	DDX_Text(pDX, IDC_EDIT_STOP_TIME, m_dStopTime);
	DDX_Text(pDX, IDC_EDIT_MEDIUM_DENSITY, m_dMediumDensity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgParameterCongeal, CXDialog)
	//{{AFX_MSG_MAP(CDlgParameterCongeal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParameterCongeal message handlers

BOOL CDlgParameterCongeal::OnInitDialog() 
{
	CXDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	GetDataToPreventCongealControl();
	InitToolTip();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------                
// DATE         : [2006/02/13]
// Author       : ZSY
// Parameter(s) : nID[in] ��¼�������
// Return       : �ɹ�����1�����򷵻�0
// Remark       : �����ݿ���ȡ�����ݵ���������������
//------------------------------------------------------------------
short CDlgParameterCongeal::GetDataToPreventCongealControl( long nID )
{
	CString strSQL;	// SQL���
	CString strTmp;	// ��ʱ�ַ�����
	double dTmp;	// ��ʱ�����ͱ���

	try
	{
		if ( m_pRsCongealData == NULL || m_pRsCongealData->State != adStateOpen )
		{
			return 0;
		}

		m_ID = nID;	// ���浱ǰ��¼�����
		// if (m_pRsCongealData->GetRecordCount() > 0 )
		if ( !m_pRsCongealData->adoEOF || !m_pRsCongealData->BOF )
		{
			m_pRsCongealData->MoveFirst();
			strSQL.Format( "ID=%d", nID );
			m_pRsCongealData->Find(_bstr_t(strSQL), NULL, adSearchForward);
		}
		if ( m_pRsCongealData->adoEOF || m_pRsCongealData->BOF )
		{
			//û�е�ǰ���ʱ������һ���¼�¼
			m_pRsCongealData->AddNew();
			//���
			m_pRsCongealData->PutCollect(_T("ID"), _variant_t(nID));
			//���̴���
			m_pRsCongealData->PutCollect(_T("EnginID"), _variant_t( m_strProjectID ));			
			//
			m_pRsCongealData->Update(vtMissing);

			m_ctlHeatLossData.SetCurSel(0);
		}
		else
		{
			//�ܵ�ͨ��֧���ܴ�����(����)��ʧ�ĸ���ϵ��
			dTmp			= vtof( m_pRsCongealData->GetCollect(_T("c_Heat_Loss_Data")) );	
			strTmp.Format("%.2f",dTmp);
			m_ctlHeatLossData.SetWindowText(strTmp);
		}
		
		//�����ܶ�
		m_dMediumDensity = vtof(m_pRsCongealData->GetCollect(_variant_t("c_Medium_Density")));

		//�ܲ��ܶ�
		m_dPipeDensity	= vtof( m_pRsCongealData->GetCollect(_T("c_Pipe_Density")) );

		//���ʱ���
		m_dMediumHeat	= vtof( m_pRsCongealData->GetCollect(_T("c_Medium_Heat")) );

		//�ܲı���
		m_dPipeHeat		= vtof( m_pRsCongealData->GetCollect(_T("c_Pipe_Heat")) );

		//����
		m_dFlux			= vtof( m_pRsCongealData->GetCollect(_T("c_Flux")) );

		//�����ڽ���
		m_dMediumMeltHeat	= vtof( m_pRsCongealData->GetCollect(_T("c_Medium_Melt_Heat")) );

		//�����ڹ��ڶ����¶�
		m_dMediumCongealTemp = vtof( m_pRsCongealData->GetCollect(_T("c_Medium_Congeal_Temp")) );

		//������ܵ�����Һ��ͣ��ʱ��(Сʱ)
		m_dStopTime		= vtof( m_pRsCongealData->GetCollect(_T("c_Stop_Time")) );

	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return 0;
	}
	UpdateData( FALSE );
	
	return 1;
}

//------------------------------------------------------------------                
// DATE         : [2006/02/13]
// Author       : ZSY
// Parameter(s) : pCon[in] ԭʼ������ݿ�����
// Return       : void
// Remark       : ���õ�ǰ����ԭʼ������ݿ�����
//------------------------------------------------------------------
void CDlgParameterCongeal::SetCurrentProjectConnect(_ConnectionPtr pCon)
{
	ASSERT( pCon != NULL );
	m_pConCongeal = pCon;
}

//------------------------------------------------------------------                
// DATE         : [2006/02/13]
// Author       : ZSY
// Parameter(s) : ProjectID[in] ����ID
// Return       : void
// Remark       : ������ѡ���̵�ID��
//------------------------------------------------------------------
void CDlgParameterCongeal::SetProjectID(LPCTSTR ProjectID)
{
	ASSERT( ProjectID );
	m_strProjectID = ProjectID;
}

//------------------------------------------------------------------                
// DATE         : [2006/02/13]
// Author       : ZSY
// Parameter(s) : pCurRs[in] 
// Return       : void
// Remark       : �������ݼ�¼��
//------------------------------------------------------------------
void CDlgParameterCongeal::SetCurrentRes(_RecordsetPtr pCurRs)
{
	m_pRsCongealData = pCurRs;
}


//------------------------------------------------------------------                
// DATE         : [2006/02/13]
// Author       : ZSY
// Parameter(s) : void	
// Return       : void
// Remark       : �򿪷��������ʱ��ԭʼ���ݱ�
//------------------------------------------------------------------
void CDlgParameterCongeal::OpenCongealDBTbl()
{
	CString strSQL; // SQL
	try
	{
		if ( m_pRsCongealData == NULL )
		{
			m_pRsCongealData.CreateInstance( __uuidof( Recordset ) );
		}else if ( m_pRsCongealData->State == adStateOpen )
		{
			m_pRsCongealData->Close();
		}

		ASSERT( !m_strProjectID.IsEmpty() );

		strSQL = "SELECT * FROM [PRE_CALC_CONGEAL] WHERE EnginID='"+m_strProjectID+"' OBDER BY ID";
		m_pRsCongealData->Open( _variant_t( strSQL ), m_pConCongeal.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText );

	}
	catch (_com_error& e)
	{
		AfxMessageBox( e.Description() );
		return ;
	}
}

//------------------------------------------------------------------
// DATE         : [2006/02/16]
// Author       : ZSY
// Parameter(s) : void	
// Return       : �������ݳɹ�����1�����򷵻�0
// Remark       : ���������·���Ҫ���������뵽���ݿ��С�
//------------------------------------------------------------------
short CDlgParameterCongeal::PutDataToPreventCongealDB(long nID)
{
	try
	{
		UpdateData( TRUE );
		
		if ( m_pRsCongealData == NULL && m_pRsCongealData->State == adStateOpen )
		{
			return 0;
		}
		CString strTmp;
		CString strSQL;
		double	dTmp;

		if (nID != -1 && m_ID != nID)
		{
			// д�뵽ָ���ļ�¼��
			if ( !m_pRsCongealData->adoEOF || !m_pRsCongealData->BOF )
			{
				m_pRsCongealData->MoveFirst();
				strSQL.Format( "ID=%d", nID );
				m_pRsCongealData->Find(_bstr_t(strSQL), NULL, adSearchForward);
			}
			if ( m_pRsCongealData->adoEOF || m_pRsCongealData->BOF )
			{
				//û�е�ǰ���ʱ������һ���¼�¼
				m_pRsCongealData->AddNew();
				//���
				m_pRsCongealData->PutCollect(_T("ID"), _variant_t(nID));
				//���̴���
				m_pRsCongealData->PutCollect(_T("EnginID"), _variant_t( m_strProjectID ));			
				//
				m_pRsCongealData->Update(vtMissing);
			}
		}
		else if (m_pRsCongealData->adoEOF || m_pRsCongealData->BOF)
		{
			return 0;
		}
		
		//�����ܶ� 
		m_pRsCongealData->PutCollect(_T("c_Medium_Density"), _variant_t(m_dMediumDensity));
		
		//�ܲ��ܶ�
		m_pRsCongealData->PutCollect( _T("c_Pipe_Density"), _variant_t(m_dPipeDensity) );
		
		//���ʱ���
		m_pRsCongealData->PutCollect( _T("c_Medium_Heat"), _variant_t(m_dMediumHeat) );
		
		//�ܲı���
		m_pRsCongealData->PutCollect( _T("c_Pipe_Heat"), _variant_t(m_dPipeHeat) );
		
		//����
		m_pRsCongealData->PutCollect( _T("c_Flux"), _variant_t(m_dFlux) );
		
		//�����ڽ���
		m_pRsCongealData->PutCollect( _T("c_Medium_Melt_Heat"), _variant_t(m_dMediumMeltHeat) );
		
		//�����ڹ��ڶ����¶�
		m_pRsCongealData->PutCollect( _T("c_Medium_Congeal_Temp"), _variant_t(m_dMediumCongealTemp) );
		
		//������ܵ�����Һ��ͣ��ʱ��(Сʱ)
		m_pRsCongealData->PutCollect( _T("c_Stop_Time"), _variant_t(m_dStopTime) );
		
		//�ܵ�ͨ��֧���ܴ�����(����)��ʧ�ĸ���ϵ��
		m_ctlHeatLossData.GetWindowText(strTmp);
		dTmp = strtod(strTmp, NULL);
		m_pRsCongealData->PutCollect( _T("c_Heat_Loss_Data"), _variant_t(dTmp) );
		
		//���
	//	m_pRsCongealData->PutCollect(_T("ID"), _variant_t((long)m_ID));
		
		//���̴���
		m_pRsCongealData->PutCollect(_T("EnginID"), _variant_t(m_strProjectID));
		
		m_pRsCongealData->Update();

	}
	catch (_com_error& e) 
	{
		AfxMessageBox(e.Description());
		return 0;
	}

	return 1;
	
}

//------------------------------------------------------------------
// DATE         : [2006/02/24]
// Author       : ZSY
// Parameter(s) : void
// Return       : void
// Remark       : ��ʼ��������ʾ��Ϣ
//------------------------------------------------------------------
void CDlgParameterCongeal::InitToolTip()
{
	Txtup TipArr[20];
	int pos = 0;

	// ��������������ӵĲ���
	TipArr[pos].dID = IDC_EDIT_MEDIUM_DENSITY;	//�����ܶ�
	TipArr[pos++].txt = _T("kg/m3");
	
	TipArr[pos].dID = IDC_EDIT_PIPE_DENSITY;	//�ܲ��ܶ�
	TipArr[pos++].txt = _T("kg/m3");
	
	TipArr[pos].dID = IDC_EDIT_MEDIUM_HEAT;		//���ʱ���
	TipArr[pos++].txt = _T(" J/(kg.��)");

	TipArr[pos].dID = IDC_EDIT_PIPE_HEAT;		//�ܵ����ϵı���
	TipArr[pos++].txt = _T(" J/(kg.��)");
	
	TipArr[pos].dID = IDC_EDIT_MEDIUM_MELT_HEAT;//�����ܽ���
	TipArr[pos++].txt = _T(" J/kg");

	TipArr[pos].dID = IDC_EDIT_MEDIUM_CONGEAL_TEMP;//�����ڹ��ڶ����¶�
	TipArr[pos++].txt = _T("��");

	TipArr[pos].dID = IDC_EDIT_STOP_TIME;		//������ܵ�����Һ��ͣ��ʱ��
	TipArr[pos++].txt = _T("h");

	m_pctlTip.Create(this);
//	m_pctlTip.SetTipBkColor(RGB(255,255,255));
	for (int i = 0; i < pos; i++)
	{	
		m_pctlTip.AddTool(GetDlgItem(TipArr[i].dID), (TipArr[i].txt));
	}
}

BOOL CDlgParameterCongeal::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pctlTip.RelayEvent(pMsg);
	return CXDialog::PreTranslateMessage(pMsg);
}
