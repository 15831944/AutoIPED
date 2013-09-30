// EditOriginalData.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "EditOriginalData.h"
#include "vtot.h"
#include "MainFrm.h"
#include "AutoIPEDDoc.h"

#include "uewasp.h"
#include "EDIBgbl.h"

extern CAutoIPEDApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define atmospheric_pressure 0.101325
/////////////////////////////////////////////////////////////////////////////
// CEditOriginalData dialog

CEditOriginalData::CEditOriginalData(CWnd* pParent /*=NULL*/)
	: CDialog(CEditOriginalData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditOriginalData)
	m_VolNo = _T("");
	m_ID = 0;
	m_ReMark = _T("");
	m_IsVolListEnable = FALSE;
	m_IsHeatPreservationTypeEnable = FALSE;
	m_WideSpeed = 0.0f;
	m_PriceRatio = 0.0f;
	m_RunPerHour = 0.0f;
	m_MediumTemperature = 0.0f;
	m_IsUpdateByRoll = TRUE;
	m_Amount = 0;
	m_bIsAutoSelectMatOnRoll = FALSE;
	m_dHeatLoss = "";
	m_dHeatDensity = "";
	m_dThick1 = 0.0;
	m_dThick2 = 0.0;
	m_dTa = 0.0;
	m_dTs = 0.0;
	m_dPressure = 0.0;
	m_bIsCalInThick = FALSE;
	m_bIsCalPreThick = FALSE;
	m_dWindPlaimWidth = 0.0;
	//}}AFX_DATA_INIT

//	m_bIsAutoSelectMatOnRoll=FALSE;
	m_ProjectID=_T("");
	m_dMaxD0 = 2000;

	m_pDlgCongeal		= NULL;
	m_pDlgSubterranean	= NULL;
	m_pDlgCurChild		= NULL;
	m_pMatProp			= NULL;
} 


CEditOriginalData::~CEditOriginalData()
{
	// �ͷ����ж�̬���������Ա����
	if ( m_pDlgCongeal != NULL )
	{
		delete m_pDlgCongeal;
	}
	if ( m_pDlgSubterranean != NULL )
	{
		delete m_pDlgSubterranean;
	}
}


void CEditOriginalData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditOriginalData)
	DDX_Control(pDX, IDC_TITLE_TAB, m_ctlTitleTab);
	DDX_Control(pDX, IDC_PIPE_MEDIUM, m_ctlPipeMedium);
	DDX_Control(pDX, IDC_EDIT_METHOD, m_ctlCalcMethod);
	DDX_Control(pDX, IDC_HEAT_TRANSFER_COEF, m_ctlHeatTran);
	DDX_Control(pDX, IDC_ENVIRON_TEMP, m_ctlEnvTemp);
	DDX_Control(pDX, IDC_DEL_ALL_INSU, m_DeleteAll);
	DDX_Control(pDX, IDC_COLOR, m_ColorRing);
	DDX_Control(pDX, IDC_SAVECURRENT, m_SaveCurrent);
	DDX_Control(pDX, IDC_DELCURRENT, m_DelCurrent);
	DDX_Control(pDX, IDC_ADDNEW, m_AddNew);
	DDX_Control(pDX, IDC_LAST, m_MoveLast);
	DDX_Control(pDX, IDC_SUBSEQUENT, m_MoveSubsequent);
	DDX_Control(pDX, IDC_PREVIOUS, m_MovePrevious);
	DDX_Control(pDX, IDC_FOREFRONT, m_MoveForefront);
	DDX_Control(pDX, IDC_SAFEGUARD_MATNAME, m_SafeguardMatName);
	DDX_Control(pDX, IDC_OUTSIZE_MATNAME, m_OutSizeMatName);
	DDX_Control(pDX, IDC_INSIDE_MATNAME, m_InsideMatName);
	DDX_Control(pDX, IDC_HEAT_PRESERVATION_TYPE, m_HeatPreservationTypeList);
	DDX_Control(pDX, IDC_VOLLIST, m_VolList);
	DDX_Control(pDX, IDC_PIPE_MAT, m_PipeMat);
	DDX_Control(pDX, IDC_BUILDIN_PLACE, m_BuidInPlace);
	DDX_Control(pDX, IDC_PIPE_THICK, m_PipeThick);
	DDX_Control(pDX, IDC_PIPE_SIZE, m_PipeSize);
	DDX_Control(pDX, IDC_PIPE_DEVICE_NAME, m_PipeDeviceName);
	DDX_Text(pDX, IDC_VOLNO, m_VolNo);
	DDX_Text(pDX, IDC_ID, m_ID);
	DDX_Text(pDX, IDC_REMARK, m_ReMark);
	DDX_Check(pDX, IDC_VOLLIST_ISENABLE, m_IsVolListEnable);
	DDX_Check(pDX, IDC_HEAT_PRESERVATION_TYPE_ENABLE, m_IsHeatPreservationTypeEnable);
	DDX_Text(pDX, IDC_WIDE_SPEED, m_WideSpeed);
	DDX_Text(pDX, IDC_PRICE, m_PriceRatio);
	DDX_Text(pDX, IDC_RUN_PERHOUR, m_RunPerHour);
	DDX_Text(pDX, IDC_MEDIUM_TEMPERATURE, m_MediumTemperature);
	DDX_Check(pDX, IDC_UPDATABYROLL, m_IsUpdateByRoll);
	DDX_Text(pDX, IDC_AMOUNT, m_Amount);
	DDX_Check(pDX, IDC_CHECK_AUTOSELECT_PRE_EDIT, m_bIsAutoSelectMatOnRoll);
	DDX_Text(pDX, IDC_EDIT_HEAT_LOSS, m_dHeatLoss);
	DDX_Text(pDX, IDC_EDIT_HEAT_DENSITY, m_dHeatDensity);
	DDX_Text(pDX, IDC_EDIT_THICK1, m_dThick1);
	DDX_Text(pDX, IDC_EDIT_THICK2, m_dThick2);
	DDX_Text(pDX, IDC_EDIT_TA, m_dTa);
	DDX_Text(pDX, IDC_EDIT_TS, m_dTs);
	DDX_Text(pDX, IDC_PIPE_PRESSURE, m_dPressure);
	DDX_Check(pDX, IDC_CHECK_THICK1, m_bIsCalInThick);
	DDX_Check(pDX, IDC_CHECK_THICK2, m_bIsCalPreThick);  
	DDX_Text(pDX, IDC_EDIT_PLAIM_THICK, m_dWindPlaimWidth);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CEditOriginalData, CDialog)

BEGIN_MESSAGE_MAP(CEditOriginalData, CDialog)
	//{{AFX_MSG_MAP(CEditOriginalData)
	ON_BN_CLICKED(IDC_FOREFRONT, OnForefront)
	ON_BN_CLICKED(IDC_PREVIOUS, OnPrevious)
	ON_BN_CLICKED(IDC_SUBSEQUENT, OnSubsequent)
	ON_BN_CLICKED(IDC_LAST, OnLast)
	ON_BN_CLICKED(IDC_ADDNEW, OnAddNew)
	ON_BN_CLICKED(IDC_DELCURRENT, OnDelCurrent)
	ON_BN_CLICKED(IDC_SAVECURRENT, OnSaveCurrent)
	ON_CBN_DROPDOWN(IDC_COLOR, OnDropdownColor)
	ON_CBN_DROPDOWN(IDC_PIPE_DEVICE_NAME, OnDropdownPipeDeviceName)
	ON_CBN_DROPDOWN(IDC_PIPE_SIZE, OnDropdownPipeSize)
	ON_CBN_DROPDOWN(IDC_PIPE_THICK, OnDropdownPipeThick)
	ON_CBN_DROPDOWN(IDC_INSIDE_MATNAME, OnDropdownInsideMatname)
	ON_CBN_DROPDOWN(IDC_OUTSIZE_MATNAME, OnDropdownOutsizeMatname)
	ON_CBN_DROPDOWN(IDC_SAFEGUARD_MATNAME, OnDropdownSafeguardMatname)
	ON_NOTIFY(NM_CLICK, IDC_HEAT_PRESERVATION_TYPE, OnItemchangedHeatPreservationType)
	ON_NOTIFY(NM_CLICK, IDC_VOLLIST, OnItemchangedVollist)
	ON_BN_CLICKED(IDC_VOLLIST_ISENABLE, OnUpdateAllCheckBox)
	ON_EN_CHANGE(IDC_VOLNO, OnChangeVolno)
	ON_BN_CLICKED(IDC_AUTO_SELECTMAT, OnAutoSelectmat)
	ON_EN_CHANGE(IDC_ID, OnChangeId)
	ON_BN_CLICKED(IDC_SORT_BY_VOL, OnSortByVol)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_DEL_ALL_INSU, OnDelAllInsu)
	ON_BN_CLICKED(IDC_CHECK_METHOD, OnCheckMethod)
	ON_BN_CLICKED(IDC_CHECK_TA, OnCheckTa)
	ON_BN_CLICKED(IDC_CHECK_THICK1, OnCheckThick1)
	ON_BN_CLICKED(IDC_CHECK_THICK2, OnCheckThick2)
	ON_BN_CLICKED(IDC_CHECK_TS, OnCheckTs)
	ON_CBN_SELCHANGE(IDC_ENVIRON_TEMP, OnSelchangeEnvironTemp)
	ON_CBN_SELCHANGE(IDC_HEAT_TRANSFER_COEF, OnSelchangeHeatTransferCoef)
	ON_BN_CLICKED(IDC_CHECK_ENV_TEMP, OnCheckEnvTemp)
	ON_BN_CLICKED(IDC_CHECK_ALPHA, OnCheckAlpha)
	ON_CBN_SELCHANGE(IDC_EDIT_METHOD, OnSelchangeEditMethod)
	ON_CBN_SELCHANGE(IDC_BUILDIN_PLACE, OnSelchangeBuildinPlace)
	ON_CBN_SELCHANGE(IDC_PIPE_MEDIUM, OnSelchangePipeMedium)
	ON_BN_CLICKED(IDC_BUTTON_ADD_VALVE, OnButtonAddValve)
	ON_BN_CLICKED(IDC_BUTTON_CALC_RATIO, OnButtonCalcRatio)
	ON_BN_CLICKED(IDC_HEAT_PRESERVATION_TYPE_ENABLE, OnUpdateAllCheckBox)
	ON_CBN_SELCHANGE(IDC_PIPE_MAT, OnSelchangePipeMat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditOriginalData message handlers

//////////////////////////////////////////////////////////////////////////
//
// ������ѡ���̵����ݿ������
//
// IConnection[in]	�Ѿ����ӵ����ݿ������ָ��
//
void CEditOriginalData::SetCurrentProjectConnect(_ConnectionPtr &IConnection)
{
	m_ICurrentProjectConnection=IConnection;
}

//////////////////////////////////////////////////////////////////////////
//
// �������ݿ����ӵ�����ָ�������
//
_ConnectionPtr& CEditOriginalData::GetCurrentProjectConnect()
{
	return m_ICurrentProjectConnection;
}

//////////////////////////////////////////////////////////////////////////
//
// ���ù��������ݿ������
//
// IConnection[in]	�Ѿ����ӵ����ݿ������ָ��
//
void CEditOriginalData::SetPublicConnect(_ConnectionPtr &IConnection)
{
	m_IPublicConnection=IConnection;

}

//////////////////////////////////////////////////////////////////////////
//
// ���ع������ݿ����ӵ�����ָ�������
//
_ConnectionPtr& CEditOriginalData::GetPublicConnect()
{
	return m_IPublicConnection;
}

/////////////////////////////////////////////////////////////
//
// ������ѡ���̵�ID��
//
// ProjectID[in]	��ѡ���̵�ID��
//
void CEditOriginalData::SetProjectID(LPCTSTR ProjectID)
{
	if(ProjectID==NULL)
		return;

	m_ProjectID=ProjectID;
}

//////////////////////////////////////////////////////////
//
// ������ѡ���̵�ID��
//
CString& CEditOriginalData::GetProjectID()
{
	return m_ProjectID;
}

////////////////////////////////////////////////////////
//
// ��PRE_CALC��
// �򿪷��������ʱ��Ҫ�Ĳ�����		ZSY		[2005/06/01] 
// ����عܵ��ı��²�����
BOOL CEditOriginalData::InitCurrentProjectRecordset()
{
	CString SQL;

	if(m_ProjectID.IsEmpty())
	{
		AfxMessageBox(_T("δѡ�񹤳�"));
		return FALSE;
	}

	if(m_ICurrentProjectConnection==NULL)
	{
		AfxMessageBox(_T("δ�������ݿ�"));
		return FALSE;
	}

	try
	{
		if( m_ICurrentProjectRecordset==NULL )
		{
			m_ICurrentProjectRecordset.CreateInstance(__uuidof(Recordset));
		}
		else if( m_ICurrentProjectRecordset->GetState() & adStateOpen )
		{
			m_ICurrentProjectRecordset->Close();
		}
		//��PRE_CALC��
		SQL.Format(_T("SELECT * FROM pre_calc WHERE EnginID='%s' ORDER BY id"),GetProjectID());
		m_ICurrentProjectRecordset->Open(_variant_t(SQL),GetCurrentProjectConnect().GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);

		//�򿪷��������ʱ��Ҫ�Ĳ����� 
		if ( m_pRsCongealData == NULL )
		{
			m_pRsCongealData.CreateInstance(__uuidof(Recordset));
		}
		else if ( m_pRsCongealData->GetState() == adStateOpen )
		{
			m_pRsCongealData->Close();
		}
		SQL.Format("SELECT * FROM [PRE_CALC_CONGEAL] WHERE EnginID = '"+GetProjectID()+"' ORDER BY ID ");
		m_pRsCongealData->Open(_variant_t(SQL), GetCurrentProjectConnect().GetInterfacePtr(),
						  adOpenStatic, adLockOptimistic, adCmdText);

		// ��عܵ��Ĳ�����
		if ( NULL == m_pRsSubterranean )
		{
			m_pRsSubterranean.CreateInstance( __uuidof( Recordset ) );
		}
		else if ( m_pRsSubterranean->GetState() == adStateOpen )
		{
			m_pRsSubterranean->Close();
		}
		SQL.Format("SELECT * FROM [PRE_CALC_SUBTERRANEAN] WHERE EnginID='"+GetProjectID()+"' ORDER BY ID");
		m_pRsSubterranean->Open( _variant_t(SQL), GetCurrentProjectConnect().GetInterfacePtr(),
							adOpenStatic, adLockOptimistic, adCmdText);

	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

	return TRUE;
}

BOOL CEditOriginalData::OnInitDialog() 
{
	BOOL bRet;
	m_pRsEnvironment.CreateInstance(__uuidof(Recordset));		//�����¶�ȡֵ������¼��
	m_pRsHeat.CreateInstance(__uuidof(Recordset));				//����ϵ��ȡֵ������¼��

	CDialog::OnInitDialog();
	//�ƶ�ʱ�Ƿ���¼�¼
	m_IsUpdateByRoll = bIsMoveUpdate;
	//TRUE--ƽ�ڱ�־,FALSE--�ܵ�
	m_bIsPlane = FALSE;
	//��ʾ��Ϣ(��ʾ��λ)			 [2005/06/23]
	InitToolTip();

	//��ԭʼ���ݡ�
	bRet=InitCurrentProjectRecordset();
	if(bRet==FALSE)
	{
		CDialog::OnCancel();
		return FALSE;
	}

	bRet=InitVolListControl();
	if(bRet==FALSE)
	{
		CDialog::OnCancel();
		return TRUE;
	}
	//��ʼ�����¶ȵ�ȡֵ.
	UpdateEnvironmentComBox();
	//
	//����ϵ��.
	UpdateHeatTransferCoef();
	//ѡ����㷽��
	InitCalcMethod();
	//
	//���ڽ���
	InitPipeMedium();
	//
	//��װ�ص�
	UpdateBuildPlaceComBox();


	bRet=InitHeatPreservationTypeList();
	if(bRet==FALSE)
	{
		CDialog::OnCancel();
		return TRUE;
	}
	
	bRet=InitBitmapButton();
	if(bRet==FALSE)
	{
		CDialog::OnCancel();
		return TRUE;
	}

	bRet=PutDataToEveryControl();
	if(bRet==FALSE)
	{
		CDialog::OnCancel();
		return TRUE;
	}
	//��ʼ����
	InitConstant();
	//���¸��ؼ���״̬��
	UpdateControlsState();

	//��ʼ�����Ͽؼ�
	GetPropertyofMaterial mGetPropertyofMaterial;
	int iCodeID = mGetPropertyofMaterial.GetCodeID( EDIBgbl::sCur_MaterialCodeName,theApp.m_pConMaterial );
	if ( iCodeID == -1 )
		iCodeID = 0;
	CArray<CString,CString> mAllMatName;
	mGetPropertyofMaterial.GetAllMatLanguageName( iCodeID,GetPropertyofMaterial::eSolid,theApp.m_pConMaterialName,theApp.m_pConMaterial,mAllMatName );
	int iCount = mAllMatName.GetSize();
	for ( int i=0; i<iCount; i++ )
	{
		m_PipeMat.AddString( mAllMatName.GetAt(i) );
	}

	UpdateData();

	//�Զ�ѡ��
	if(GetIsAutoSelectMatOnRoll())
	{	//û�м�¼ʱ��������.
		if( m_ICurrentProjectRecordset->GetRecordCount() >  0 )
		{
			OnAutoSelectmat();
		}
	}

	m_ctlTitleTab.ShowWindow(SW_SHOW);

	//���á��˳���ΪĬ�ϵİ�ť
	this->SetDefID(IDC_EXIT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////
//
// ��ʼ�����LIST�ؼ� 
//
// �����ʼ���ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::InitVolListControl()
{
	CString SQL;
	CString TempStr1,TempStr2;
	_RecordsetPtr IRecordset;
	_variant_t varTemp;
	HRESULT hr;
	int nItem;
	RECT rect;
	CString strPriceRatio;	// �ȼ۱�������

	m_VolList.GetWindowRect(&rect);
	m_VolList.InsertColumn(1,_T("����"),LVCFMT_LEFT,(int)((rect.right-rect.left)/10.0*3.0));
	m_VolList.InsertColumn(2,_T("�������"),LVCFMT_LEFT,(int)((rect.right-rect.left)/10.0*7.3));
	m_VolList.InsertColumn(3,_T("�ȼ۱�������"),LVCFMT_LEFT,(int)0);

	m_VolList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	if(GetPublicConnect()==NULL)
	{
		AfxMessageBox(_T("δ�����������ݿ������"));
		return FALSE;
	}

	hr=IRecordset.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return FALSE;
	}

	SQL.Format(_T("SELECT * FROM a_vol WHERE [vol] IS NOT NULL AND [vol_name] IS NOT NULL AND  [VOL_BAK] <> 'UESoft' ORDER BY VOL,vol_name"));

	try
	{
		IRecordset->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
						adOpenDynamic,adLockOptimistic,adCmdText);

		nItem=0;
		while(!IRecordset->adoEOF)
		{

			TempStr1 =	vtos( IRecordset->GetCollect(_variant_t("vol")));		//����
			TempStr2 =	vtos( IRecordset->GetCollect(_variant_t("vol_name")));	//�����
			strPriceRatio = vtos( IRecordset->GetCollect( _variant_t("vol_price")));	// �ȼ۱�������
			if( TempStr1.IsEmpty() || TempStr2.IsEmpty() )
			{
				IRecordset->MoveNext();
				continue;
			}

			m_VolList.InsertItem(nItem,TempStr1);
			m_VolList.SetItemText(nItem,1,TempStr2);
			m_VolList.SetItemText(nItem,2,strPriceRatio);

			nItem++;
			IRecordset->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////
//
// ��ʼ����������LIST�ؼ� 
//
// �����ʼ���ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::InitHeatPreservationTypeList()
{
	CString SQL;
	CString TempStr1,TempStr2;
	_RecordsetPtr IRecordset;
	_variant_t varTemp;
	int nItem;
	RECT rect;

	m_HeatPreservationTypeList.GetWindowRect(&rect);
	m_HeatPreservationTypeList.InsertColumn(1,_T("���ʹ���"),LVCFMT_LEFT,(int)((rect.right-rect.left)/10.0*4.0));
	m_HeatPreservationTypeList.InsertColumn(2,_T("������������"),LVCFMT_LEFT,(int)((rect.right-rect.left)/10.0*7.3));

	m_HeatPreservationTypeList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	if(GetPublicConnect()==NULL)
	{
#ifdef _DEBUG
		AfxMessageBox(_T("Ϊ�����������ݿ������"));
#endif
		return FALSE;
	}

	IRecordset.CreateInstance(__uuidof(Recordset));

//	SQL.Format(_T("SELECT * FROM a_pre"));
	SQL.Format(_T("SELECT * FROM a_pre WHERE EnginID='%s'"), GetProjectID());

	try
	{
//		IRecordset->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
//						adOpenDynamic,adLockOptimistic,adCmdText);
		IRecordset->Open(_variant_t(SQL),GetCurrentProjectConnect().GetInterfacePtr(), adOpenDynamic,adLockOptimistic,adCmdText);

		nItem=0;
		while(!IRecordset->adoEOF)
		{
			varTemp=IRecordset->GetCollect(_variant_t("AUTO_CODE"));
			if(varTemp.vt!=VT_NULL)
			{
				TempStr1 = vtos(varTemp);
			}
			else
			{
				IRecordset->MoveNext();
				continue;
			}

			varTemp=IRecordset->GetCollect(_variant_t("AUTO_MARK"));
			if(varTemp.vt!=VT_NULL)
			{
				TempStr2 = vtos(varTemp);
			}
			else
			{
				IRecordset->MoveNext();
				continue;
			}

			m_HeatPreservationTypeList.InsertItem(nItem,TempStr1);
			m_HeatPreservationTypeList.SetItemText(nItem,1,TempStr2);

			nItem++;
			IRecordset->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////
//
// ��ʼ��λͼ��Ť
//
// �����ʼ���ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::InitBitmapButton()
{
	CBitmap Bitmap;

	struct
	{
		CMyButton *pButton;		
		UINT uBitmapID;			//λͼ��ID
		LPCTSTR szTooltipText;	//��ʾ��Ϣ
	}ButtonInfo[]=
	{
		{&m_MoveForefront,	IDB_BITMAP1,	_T("��һ����¼")},
		{&m_MovePrevious,	IDB_BITMAP2,	_T("ǰһ����¼")},
		{&m_MoveSubsequent,	IDB_BITMAP3,	_T("��һ����¼")},
		{&m_MoveLast,		IDB_BITMAP4,	_T("���һ����¼")},
		{&m_AddNew,			IDB_BITMAP5,	_T("�½�")},
		{&m_DelCurrent,		IDB_BITMAP6,	_T("ɾ����ǰ��¼")},
		{&m_SaveCurrent,	IDB_BITMAP7,	_T("�����޸�")},
		{&m_DeleteAll,      IDB_BITMAP10,   _T("ɾ�����еļ�¼")}
	};

	for(int i=0;i<sizeof(ButtonInfo)/sizeof(ButtonInfo[0]);i++)
	{
		Bitmap.LoadBitmap(ButtonInfo[i].uBitmapID);

		ButtonInfo[i].pButton->SetBitmap((HBITMAP)Bitmap.Detach());
		ButtonInfo[i].pButton->SetTooltipText(ButtonInfo[i].szTooltipText,TRUE);
	}
	
	return TRUE;
}

/////////////////////////////////////////////////////////
//
// ���Դ򿪵ļ�¼���ĵ�ǰ�α����ڵ�λ��ȡ���ݵ������ؼ�
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::PutDataToEveryControl()
{
	CString TempStr;
//	int		nTmp;
	short   flg;
	short	nIndex;			//
		
	if(m_ICurrentProjectRecordset->adoEOF)
	{
		return TRUE;
	}
	try
	{
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("id"),m_ID);
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_vol"),m_VolNo);		
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_color"),TempStr);
		m_ColorRing.SetWindowText(TempStr);
		
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_name1"),TempStr);
		m_PipeDeviceName.SetWindowText(TempStr);
		
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_size"),TempStr);
		m_PipeSize.SetWindowText(TempStr);
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_pi_thi"),TempStr);
		m_PipeThick.SetWindowText(TempStr);
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_temp"),m_MediumTemperature);
		
		//��װ�ص�
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_place"),TempStr);

		if (TempStr.IsEmpty())
		{
			m_BuidInPlace.SetCurSel(0);
		}
		else
		{
			TempStr = TempStr.Left(4);
			m_BuidInPlace.SelectString(-1,TempStr);
		}

		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_pi_mat"),TempStr);
		m_PipeMat.SetWindowText(TempStr);

		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_mark"),m_ReMark);
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_name_in"),TempStr);
		m_InsideMatName.SetWindowText(TempStr);

		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_name2"),TempStr);
		m_OutSizeMatName.SetWindowText(TempStr);

		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_name3"),TempStr);
		m_SafeguardMatName.SetWindowText(TempStr);

		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_wind"),m_WideSpeed);
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_price"),m_PriceRatio);
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_hour"),m_RunPerHour);
		
		//�͹ܵ��ĺ��,ָ���ĺ�Ȳ��ɴ����롣
		//	GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_pre_thi"),m_HeatPreservationThick);
		
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_amount"),m_Amount);		
		GetTbValue(m_ICurrentProjectRecordset, _variant_t("c_Env_Temp_Index"), TempStr);//�����¶ȶ��� 
		nIndex = (short)_tcstol(TempStr, NULL, 10);
		if(	nIndex >=0 && nIndex < m_ctlEnvTemp.GetCount() )
		{	
			m_ctlEnvTemp.GetLBText(nIndex, TempStr);
			m_ctlEnvTemp.SelectString( 0, TempStr );
			//��������л�û����¶�ȡֵ������Ӧ�Ļ����¶�ֵ
			//	GetConditionTemp(m_dTa,nTmp,nIndex);		
		}
		else
		{
			m_ctlEnvTemp.SetWindowText("");
		}
		//����ϵ��ȡֵ.
		TempStr = vtos(m_ICurrentProjectRecordset->GetCollect(_variant_t("c_Alfa_Index")));
		nIndex = (short)_tcstol(TempStr, NULL, 10);
		if( nIndex >=0 && nIndex < this->m_ctlHeatTran.GetCount() )
		{
			m_ctlHeatTran.GetLBText(nIndex, TempStr);
			m_ctlHeatTran.SelectString( 0, TempStr ); // ���ѡ����ַ��������б��У��򱣳ֵ�ǰ��ѡ��
		}
		else
		{
			m_ctlHeatTran.SetWindowText("");
		}
		//���㷽��
		TempStr = vtos(m_ICurrentProjectRecordset->GetCollect(_variant_t("c_CalcMethod_Index")));
		nIndex = (short)_tcstol(TempStr, NULL, 10);
		if( nIndex >=0 && nIndex < this->m_ctlCalcMethod.GetCount() )
		{
			//
			m_ctlCalcMethod.GetLBText(nIndex, TempStr);
			
			m_ctlCalcMethod.SelectString(-1,TempStr);
			
			//Ϊ������¶ȼ��㷽��ʱ
			if (nSurfaceTemperatureMethod == nIndex)
			{
				flg = TRUE;
			}
			else
			{
				flg = FALSE;
			}
			((CButton*)GetDlgItem(IDC_CHECK_TS))->SetCheck(flg);
			((CButton*)GetDlgItem(IDC_EDIT_TS))->EnableWindow(flg);
		}
		else
		{
			m_ctlCalcMethod.SetWindowText("");
		}

		//ɢ����ֻ����λС��
		m_dHeatLoss.Format("%.2f",vtof(m_ICurrentProjectRecordset->GetCollect(_variant_t("c_lost"))) );
		//�����ܶ�
		m_dHeatDensity.Format("%.2f", vtof(m_ICurrentProjectRecordset->GetCollect(_variant_t("c_HeatFlowrate"))) );
		//�ڱ��º�
		TempStr.Format("%.2f",vtof(m_ICurrentProjectRecordset->GetCollect(_variant_t("c_in_thi"))));
		m_dThick1 =  strtod(TempStr,NULL);
		//�Ᵽ�º�
		TempStr.Format("%.2f",vtof( m_ICurrentProjectRecordset->GetCollect(_variant_t("c_pre_thi")) ));
		m_dThick2 = strtod(TempStr,NULL);
		//�����¶�
		TempStr.Format("%.2f",vtof( m_ICurrentProjectRecordset->GetCollect(_variant_t("c_con_temp"))) );
		m_dTa	  = strtod(TempStr,NULL);
		//������¶�,ֻ����λС��
		TempStr.Format("%.2f",vtof( m_ICurrentProjectRecordset->GetCollect(_variant_t("c_tb3"))));
		m_dTs	 = strtod(TempStr,NULL);
		//����ѹ��
		m_dPressure = vtof( m_ICurrentProjectRecordset->GetCollect(_variant_t("c_Pressure")) );
		//���ڽ���
		TempStr	= vtos( m_ICurrentProjectRecordset->GetCollect(_variant_t("c_Medium")) );
		if (TempStr.IsEmpty())
		{
			TempStr = _T("������");
		}
		m_ctlPipeMedium.SelectString(0, TempStr);
		
		//�Ƿ��ֶ������ڱ��²�ĺ��
		m_bIsCalInThick =  vtob( m_ICurrentProjectRecordset->GetCollect(_variant_t("C_CalInThi")));
		//�Ƿ��ֶ������Ᵽ�²�ĺ��
		m_bIsCalPreThick = vtob( m_ICurrentProjectRecordset->GetCollect(_variant_t("c_CalPreThi")));
		//�ط��ٷ����ƽ�ڿ��
		m_dWindPlaimWidth = vtof(m_ICurrentProjectRecordset->GetCollect(_variant_t("c_B")));

		ShowWindowRect();
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}

	UpdateData(FALSE);
	
	return TRUE;
}

/////////////////////////////////////////////////////////
//
// �Ӹ����ؼ�ȡ���ݵ����ݿ�
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
//����֮ǰ��UpdateData(TRUE)�Ի���.
BOOL CEditOriginalData::PutDataToDatabaseFromControl(long nID)
{
	CString TempStr;	//
	short	nIndex;		//��ʱ������ֵ
	short	nMethodIndex;// ���¼��㷽��������
//	UpdateData(TRUE);

	try
	{
		if(m_ICurrentProjectRecordset->adoEOF || m_ICurrentProjectRecordset->BOF)
			return FALSE;

		//����
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_vol"),_variant_t(m_VolNo));

		//ɫ��
		m_ColorRing.GetWindowText(TempStr);
		TempStr=TempStr.Left(4);
		TempStr.TrimLeft();
		TempStr.Format("%d",_tcstol(TempStr,NULL,10));
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_color"),_variant_t(TempStr));
		
		//�����¶ȶ���
		m_ctlEnvTemp.GetWindowText(TempStr);
		nIndex = m_ctlEnvTemp.FindString(-1, TempStr);
		if( nIndex != -1 )
		{
			m_ICurrentProjectRecordset->PutCollect(_variant_t("c_Env_Temp_Index"), _variant_t((long)nIndex));
		}
		//�����ݱ��е���Ӧ����д��.
	//	m_ICurrentProjectRecordset->PutCollect(_variant_t("c_con_temp"), _variant_t());
		
		//����ϵ����ȡֵ����
		m_ctlHeatTran.GetWindowText(TempStr);
		nIndex = m_ctlHeatTran.FindString(-1,TempStr);
		if(nIndex != -1)
		{
			m_ICurrentProjectRecordset->PutCollect(_variant_t("c_Alfa_Index"),_variant_t((long)nIndex));
		}
		//���㷽��������
		m_ctlCalcMethod.GetWindowText(TempStr);
		nMethodIndex = m_ctlCalcMethod.FindString(-1,TempStr);
		if( nMethodIndex == -1 )
		{
			//��û��ѡ����㷽��ʱ,Ĭ��Ϊ���ú�ȷ�.
			nMethodIndex = 0 ;
		}
		//������㷽��������
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_CalcMethod_Index"),_variant_t((long)nMethodIndex));

		//������¶�
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_tb3"),_variant_t(m_dTs));
		//�����¶�
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_con_temp"),_variant_t(m_dTa));

		//�ڱ��º�
		if (((CButton*)GetDlgItem(IDC_CHECK_THICK1))->GetCheck())
		{
			m_ICurrentProjectRecordset->PutCollect(_variant_t("c_in_thi"),_variant_t(m_dThick1));
		}
		//�Ᵽ�º�
		if (((CButton*)GetDlgItem(IDC_CHECK_THICK2))->GetCheck())
		{
			m_ICurrentProjectRecordset->PutCollect(_variant_t("c_pre_thi"),_variant_t(m_dThick2));
		}
		//����ѹ��
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_Pressure"),_variant_t(m_dPressure));

		//���ڽ���
		m_ctlPipeMedium.GetWindowText(TempStr);
		if ( -1 != m_ctlPipeMedium.FindString(-1, TempStr) )
		{
			m_ICurrentProjectRecordset->PutCollect(_variant_t("c_Medium"),_variant_t(TempStr));
		}
		//�豸��ܵ�����
		m_PipeDeviceName.GetWindowText(TempStr);
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_name1"),_variant_t(TempStr));
		//�⾶
		m_PipeSize.GetWindowText(TempStr);
		TempStr.TrimLeft();
		if(!TempStr.IsEmpty())
			m_ICurrentProjectRecordset->PutCollect(_variant_t("c_size"),_variant_t(TempStr));
		//���
		m_PipeThick.GetWindowText(TempStr);
		TempStr.TrimLeft();
		if(!TempStr.IsEmpty())
			m_ICurrentProjectRecordset->PutCollect(_variant_t("c_pi_thi"),_variant_t(TempStr));
		//�����¶�
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_temp"),_variant_t(m_MediumTemperature));
		//��װ�ص�
		m_BuidInPlace.GetWindowText(TempStr);
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_place"),_variant_t(TempStr));
		//�ܵ�����
		m_PipeMat.GetWindowText(TempStr);
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_pi_mat"),_variant_t(TempStr));
		//��ע
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_mark"),_variant_t(m_ReMark));

		//�ڱ��²���
		m_InsideMatName.GetWindowText(TempStr);
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_name_in"),_variant_t(TempStr));

		//�Ƿ��ֶ������ڱ��²�ĺ��.
		if ( TempStr.IsEmpty() )
		{
			//������Ϊ�գ�û��ָ���ĺ��.
			m_bIsCalInThick = 0;
		}
		m_ICurrentProjectRecordset->PutCollect(_variant_t("C_CalInThi"),_variant_t(Btob(m_bIsCalInThick)));
		
		//�Ᵽ�²���
		m_OutSizeMatName.GetWindowText(TempStr);
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_name2"),_variant_t(TempStr));

		//�Ƿ��ֶ������Ᵽ�²�ĺ��
		if ( TempStr.IsEmpty() )
		{
			m_bIsCalPreThick = 0;
		}//
		m_ICurrentProjectRecordset->PutCollect(_variant_t("C_CalPreThi"),_variant_t(Btob(m_bIsCalPreThick)));

		//��������
		m_SafeguardMatName.GetWindowText(TempStr);
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_name3"),_variant_t(TempStr)); 
		//���� 
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_wind"),_variant_t(m_WideSpeed)); 
		//�ȼ۱������� 
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_price"),_variant_t(m_PriceRatio)); 
		//������Сʱ��
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_hour"),_variant_t(m_RunPerHour)); 
		//���� 
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_amount"),_variant_t(m_Amount));
		//�ط��ٷ����ƽ�ڿ��
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_B"),_variant_t(m_dWindPlaimWidth));

		m_ICurrentProjectRecordset->Update();
	}
	catch(_com_error &e)
	{
		m_ICurrentProjectRecordset->CancelUpdate();
		ReportExceptionError(e);
		return FALSE;
	}
	switch( nMethodIndex )	// ���ݷ�����������Щ������Ҫ��������һЩ��
	{	
	case nPreventCongealMethod:	
		PutDataToPreventCongealDB(nID);// �������Ի����е�����д�뵽���ݿ��� [2005/06/01] 
		break;
	case nSubterraneanMethod:	// ����عܵ��ı�������д�뵽���ݿ���	 [2006/02/20]
		PutDataToSubterraneanDB(nID);
		break;
	}

	//�����µľ����
	UpdateVolTable();

	return TRUE;
}

//////////////////////////////////////////////
//
// ��Ӧ����һ����¼����Ť
//
void CEditOriginalData::OnForefront() 
{
	UpdateData();
//�ȹ�����ѡ��
/*	//�Զ�ѡ��
	if(GetIsAutoSelectMatOnRoll())
	{
		OnAutoSelectmat();
	}

*/

	try
	{
		if(m_IsUpdateByRoll)
		{
			PutDataToDatabaseFromControl();
			if( !Refresh() )
				return;
		}

		if(m_ICurrentProjectRecordset->BOF)
			return;

		m_ICurrentProjectRecordset->MoveFirst();

		PutDataToEveryControl();
		UpdateControlsState();
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
	//2005.2.24
	//�Զ�ѡ��
	if(GetIsAutoSelectMatOnRoll())
	{
		OnAutoSelectmat();
	}

	UpdateWindow();

}

/////////////////////////////////////////
//
// ��Ӧ��ǰһ����¼����Ť
//
void CEditOriginalData::OnPrevious() 
{
	UpdateData();
//2005.2.24
/*
	//�Զ�ѡ��
	if(GetIsAutoSelectMatOnRoll())
	{
		OnAutoSelectmat();
	}

*/

	try
	{
		if(m_IsUpdateByRoll)
		{
			PutDataToDatabaseFromControl();
			if( !Refresh() )
				return;
		}
		if(m_ICurrentProjectRecordset->BOF)
			return;

		m_ICurrentProjectRecordset->MovePrevious();

		if(m_ICurrentProjectRecordset->BOF)
			return;

		PutDataToEveryControl();
		UpdateControlsState();
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}
//2005.2.24
	//�Զ�ѡ��
	if(GetIsAutoSelectMatOnRoll())
	{
		OnAutoSelectmat();
	}

	UpdateWindow();
}

//////////////////////////////////////////////
//
// ��Ӧ����һ����¼����Ť
//
void CEditOriginalData::OnSubsequent() 
{
	UpdateData();
//2005.2.24
/*	//�Զ�ѡ��
	if(GetIsAutoSelectMatOnRoll())
	{
		OnAutoSelectmat();
	}

*/

	try
	{
		if(m_IsUpdateByRoll)
		{
			PutDataToDatabaseFromControl();
			if( !Refresh() )
				return;
		}
		if(m_ICurrentProjectRecordset->adoEOF)
			return;
		//ѡ���е��ƶ���¼ʱ,�Զ����ӷ���
		if (bIsAutoAddValve)
		{
			CString	strSize;	//�⾶
			CString strType;	//�ܵ�������
			
			strType = m_VolNo.Right(1);
			
			if (!strType.CompareNoCase("O") || !strType.CompareNoCase("M") || !strType.CompareNoCase("S"))
			{
				m_PipeSize.GetWindowText(strSize);
				if (m_Amount > 10 && strtod(strSize,NULL) < m_dMaxD0)
				{
					if (AfxMessageBox("�Ƿ��Զ����ӷ���!",MB_YESNO) == IDYES)
					{
						OnButtonAddValve();
						
						return;
					}
				}
			}
		}
		//
		m_ICurrentProjectRecordset->MoveNext(); 
		if(m_ICurrentProjectRecordset->adoEOF)
			return;

		PutDataToEveryControl();
		UpdateControlsState();		
	} 
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}	
//2005.2.24 
	//�Զ�ѡ��
	if(GetIsAutoSelectMatOnRoll())
	{
		OnAutoSelectmat();
	}
 
	UpdateWindow();

}

////////////////////////////////////////////////
// 
// ��Ӧ�����һ����¼����Ť
//
void CEditOriginalData::OnLast() 
{
	UpdateData();
//2005.2.24
/*
	//�Զ�ѡ��
	if(GetIsAutoSelectMatOnRoll())
	{
		OnAutoSelectmat();
	}
*/

	try
	{
		if(m_IsUpdateByRoll)
		{
			PutDataToDatabaseFromControl();
			if( !Refresh() )
				return;
		}
		if(m_ICurrentProjectRecordset->adoEOF)
			return;

		m_ICurrentProjectRecordset->MoveLast();

		if(m_ICurrentProjectRecordset->adoEOF)
			return;
		
/*		//// // TEST ADD VALVE  [2005/07/07]
		_RecordsetPtr pRs;
		pRs.CreateInstance(__uuidof(Recordset));
		CString strSQL;
		strSQL.Format("SELECT * FROM [PIPE_VALVE] WHERE EnginID='%s' AND ValveID=%d",
					m_ProjectID, vtoi(m_ICurrentProjectRecordset->GetCollect("ID") ));
		pRs->Open(_bstr_t(strSQL), m_ICurrentProjectConnection.GetInterfacePtr(),
					adOpenStatic,adLockOptimistic,adCmdText);
		if (pRs->GetRecordCount() <= 0)
		{
			if(bIsAutoAddValve)
			{
				CString strSize;
				m_PipeSize.GetWindowText(strSize);
				GetDlgItem(IDC_PIPE_SIZE)->GetWindowText(strSize.GetBuffer(50),3);
				
				if (strtod(strSize,NULL) < m_dMaxD0 && (!m_VolNo.Right(1).CompareNoCase("O") || !m_VolNo.Right(1).CompareNoCase("S") || !m_VolNo.Right(1).CompareNoCase("M") )) 
				{
					if( IDYES == AfxMessageBox("�Ƿ��Զ����ӷ���!",MB_YESNO) )
					{
						OnButtonAddValve();
						return;
					}
				}
			}
		}

//*/
		PutDataToEveryControl();
		UpdateControlsState();
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
	}	
//2005/.2.24
	//�Զ�ѡ��
	if(GetIsAutoSelectMatOnRoll())
	{
		OnAutoSelectmat();
	}

	UpdateWindow();

}

//////////////////////////////////////////
//
// ��Ӧ�����Ӽ�¼����Ť
//
void CEditOriginalData::OnAddNew() 
{
	UpdateData(TRUE);
	long	Num=0;
	int		nTmp;

	if(GetProjectID().IsEmpty())
		return;

	try
	{
		if(!m_ICurrentProjectRecordset->adoEOF || !m_ICurrentProjectRecordset->BOF)
		{
			//�Զ�ѡ��
			if(GetIsAutoSelectMatOnRoll())
			{
				OnAutoSelectmat();
			}

			//
			// �ж��Ƿ�Ӧ�ý����ݴ������ݿ�
			//
			if(m_IsUpdateByRoll)
			{
				PutDataToDatabaseFromControl();
				if( !Refresh() )
					return;
			}

			m_ICurrentProjectRecordset->MoveFirst();

			while(!m_ICurrentProjectRecordset->adoEOF)
			{
				Num++;
				m_ICurrentProjectRecordset->MoveNext();
			}
		}

		Num++;
		m_ICurrentProjectRecordset->AddNew();
		m_ICurrentProjectRecordset->PutCollect(_variant_t("id"),_variant_t(Num));
		m_ICurrentProjectRecordset->PutCollect(_variant_t("EnginID"),_variant_t(GetProjectID()));
		m_ICurrentProjectRecordset->Update();
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return;
	}

	//�½�һ����¼ʱ��ֻ����¼���Զ���1�������ļ̳���һ����¼��ֵ��
	//ֻ�̳о���.  2005.3.6
	CString strVolumeID;
	strVolumeID = m_VolNo;

	PutDataToEveryControl();        //����䲻�̳С�ȫ������ֵ��

	m_VolNo = strVolumeID; //ֻ�̳о���
	//��û����¶ȵ�ȡֵ������Ӧ��ֵ.
	GetConditionTemp(m_dTa,nTmp);

	UpdateData(FALSE);

	UpdateControlsState();
}
////////////////////////////////////////////////
//
// ��Ӧ��ɾ����ǰ��¼����Ť
//
void CEditOriginalData::OnDelCurrent() 
{
	long	CurPos;
	CString	strSQL;

	if(AfxMessageBox(_T("�Ƿ�ɾ����ǰ��¼"),MB_YESNO) == IDNO)
	{
		return;
	}

	try
	{
		if(!m_ICurrentProjectRecordset->adoEOF || !m_ICurrentProjectRecordset->BOF)
		{
			CurPos=RecNo(m_ICurrentProjectRecordset);

			m_ICurrentProjectRecordset->Delete(adAffectCurrent);
			m_ICurrentProjectRecordset->Update();

			// ɾ�������ļ�¼
			if (!m_pRsCongealData->adoEOF || !m_pRsCongealData->BOF)
			{
				m_pRsCongealData->MoveFirst();
				strSQL.Format("ID=%d", CurPos);
				m_pRsCongealData->Find( _bstr_t(strSQL), NULL, adSearchForward );
				if (!m_pRsCongealData->GetadoEOF())
				{
					m_pRsCongealData->Delete(adAffectCurrent);
					m_pRsCongealData->Update();
				}
				NumberSubtractOne(m_pRsCongealData, CurPos);
			}
			// ɾ����ر��µļ�¼
			if (!m_pRsSubterranean->adoEOF || !m_pRsSubterranean->BOF)
			{
				m_pRsSubterranean->MoveFirst();
				strSQL.Format("ID=%d", CurPos);
				m_pRsSubterranean->Find( _bstr_t(strSQL), NULL, adSearchForward);
				if (!m_pRsSubterranean->GetadoEOF())
				{
					m_pRsSubterranean->Delete(adAffectCurrent);
					m_pRsSubterranean->Update();
				}
				NumberSubtractOne( m_pRsSubterranean, CurPos );				
			}

			m_ICurrentProjectRecordset->MoveFirst();
			if(m_ICurrentProjectRecordset->adoEOF && m_ICurrentProjectRecordset->BOF)
			{
				UpdateControlsState();
				return;
			}
			//��¼�����±��(��1��ʼ),����λ����Ӧ��CurPosλ��
			RenewNumberFind(m_ICurrentProjectRecordset,1,CurPos);
			//  [2005/06/27]
			//ɾ��ĳ���ܵ�ʱ,�ڹܵ�-����ӳ�����ҲҪɾ����Ӧ�ļ�¼.
			strSQL.Format("DELETE * FROM [PIPE_VALVE] WHERE (PipeID=%d OR ValveID=%d) AND EnginID='%s' ",
						CurPos,CurPos,m_ProjectID);
			m_ICurrentProjectConnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
			//�ı����
			strSQL.Format("UPDATE [PIPE_VALVE] SET PipeID=PipeID-1 WHERE PipeID>%d AND EnginID='%s' ",
							CurPos, m_ProjectID);
			m_ICurrentProjectConnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
			//
			strSQL.Format("UPDATE [PIPE_VALVE] SET ValveID=ValveID-1 WHERE ValveID>%d AND EnginID='%s' ",
							CurPos, m_ProjectID);
			m_ICurrentProjectConnection->Execute(_bstr_t(strSQL), NULL, adCmdText);

			PutDataToEveryControl();
			UpdateControlsState();
		}
	}
	catch(_com_error &e)
	{ 
		ReportExceptionError(e);
		return;
	}

}

//////////////////////////////////////////////////
//
// ��Ӧ�����浱ǰ����Ť
//
void CEditOriginalData::OnSaveCurrent() 
{
	BOOL bRet;
	try
	{
		UpdateData();
		bRet=PutDataToDatabaseFromControl();
		if(bRet)
		{
			if( !Refresh() )
				return;
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
	}
}

/////////////////////////////////////////////////
//
// ���¡�ɫ������Ͽ�
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::UpdateColorComBox()
{
	HRESULT hr;
	CString SQL,TempStr1,TempStr2,TempStr,TextStr;
	int iRet;
	_RecordsetPtr IRecordset_Color;

	m_ColorRing.GetWindowText(TextStr);

	do
	{
		iRet=m_ColorRing.DeleteString(0);
	}while(iRet!=CB_ERR);

	if(GetPublicConnect()==NULL)
	{
		AfxMessageBox(_T("δ���ӹ������ݿ�"));
		return FALSE;
	}

	hr=IRecordset_Color.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return FALSE;
	}

//	SQL.Format(_T("SELECT * FROM a_color"));
	SQL.Format(_T("SELECT * FROM a_color WHERE EnginID='%s'"),GetProjectID());

	try
	{
//		IRecordset_Color->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
//							   adOpenDynamic,adLockOptimistic,adCmdText);
		IRecordset_Color->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
							   adOpenDynamic,adLockOptimistic,adCmdText);

		while(!IRecordset_Color->adoEOF)
		{
			GetTbValue(IRecordset_Color,_variant_t("colr_code"),TempStr1);
			GetTbValue(IRecordset_Color,_variant_t("colr_media"),TempStr2);

			if(!TempStr1.IsEmpty() && !TempStr2.IsEmpty())
			{
				TempStr.Format("%-4s %s",TempStr1,TempStr2);
				m_ColorRing.AddString(TempStr);
			}

			IRecordset_Color->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

	m_ColorRing.SetWindowText(TextStr);
	return TRUE;
}

///////////////////////////////////////////////////////
//
// ���¡��ܵ��豸���ơ���Ͽ�
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::UpdatePipeDeviceNameComBox()
{
	HRESULT hr;
	CString SQL,TempStr,TextStr;
	_RecordsetPtr IRecordset_vol;

	m_PipeDeviceName.GetWindowText(TextStr);
	UpdateData();

/*
	int iRet;
	do
	{
		iRet=m_PipeDeviceName.DeleteString(0);
	}while(iRet!=CB_ERR);
*/
	m_PipeDeviceName.ResetContent();
	
	if(GetPublicConnect()==NULL)
	{
		AfxMessageBox(_T("δ���ӹ������ݿ�"));
		return FALSE;
	}
	hr=IRecordset_vol.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return FALSE;
	}

	SQL.Format(_T("SELECT DISTINCT [VOL_NAME] FROM [a_vol] WHERE LEFT('%s',5)=LEFT(vol,5) OR VOL_BAK='UESoft'"),m_VolNo);

	try
	{
		IRecordset_vol->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
							 adOpenDynamic,adLockOptimistic,adCmdText);

		if(IRecordset_vol->adoEOF && IRecordset_vol->BOF)
		{
//			AfxMessageBox(_T("�����޴˾���!"));
			return TRUE;
		}

		while(!IRecordset_vol->adoEOF)
		{
			GetTbValue(IRecordset_vol,_variant_t("vol_name"),TempStr); 

			if(!TempStr.IsEmpty())
				m_PipeDeviceName.AddString(TempStr);

			IRecordset_vol->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}
	
	m_PipeDeviceName.SetWindowText(TextStr);
	return TRUE;
}

///////////////////////////////////////////////////
//
// ���¡��ܵ��⾶�����Ͽ�
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::UpdatePipeSizeComBox()
{
	HRESULT hr;
	CString SQL,TempStr,TextStr;
	int iRet;
	_RecordsetPtr IRecordset_Pipe;

	m_PipeSize.GetWindowText(TextStr);

	do
	{
		iRet=m_PipeSize.DeleteString(0);
	}while(iRet!=CB_ERR);

	if(GetPublicConnect()==NULL)
	{
		AfxMessageBox(_T("δ���ӹ������ݿ�"));
		return FALSE;
	}

	hr=IRecordset_Pipe.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return FALSE;
	}

	SQL.Format(_T("SELECT DISTINCT(PIPE_DW) FROM a_pipe ORDER BY PIPE_DW"));

	try
	{
		IRecordset_Pipe->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
							  adOpenDynamic,adLockOptimistic,adCmdText);

		while(!IRecordset_Pipe->adoEOF)
		{
			GetTbValue(IRecordset_Pipe,_variant_t("PIPE_DW"),TempStr);

			if(!TempStr.IsEmpty())
				m_PipeSize.AddString(TempStr);

			IRecordset_Pipe->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

	m_PipeSize.SetWindowText(TextStr);
	return TRUE;
}

////////////////////////////////////////////////////
//
// ���¡��ܵ��ں���Ͽ�
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::UpdatePipeThickComBox()
{
	HRESULT hr;
	CString SQL,TempStr,TextStr;
	int iRet;
	_RecordsetPtr IRecordset_Pipe;

	m_PipeThick.GetWindowText(TextStr);
	UpdateData();

	do
	{
		iRet=m_PipeThick.DeleteString(0);
	}while(iRet!=CB_ERR);

	if(GetPublicConnect()==NULL)
	{
		AfxMessageBox(_T("δ���ӹ������ݿ�"));
		return FALSE;
	}

	hr=IRecordset_Pipe.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return FALSE;
	}

	m_PipeSize.GetWindowText(TempStr);
	
	TempStr.TrimLeft();
	if(TempStr.IsEmpty())
		return TRUE;

	SQL.Format(_T("SELECT DISTINCT(PIPE_S) FROM a_pipe WHERE abs(PIPE_DW-%s)<0.001 ORDER BY PIPE_S"),TempStr);

	try
	{
		IRecordset_Pipe->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
							  adOpenDynamic,adLockOptimistic,adCmdText);

		while(!IRecordset_Pipe->adoEOF)
		{
			GetTbValue(IRecordset_Pipe,_variant_t("PIPE_S"),TempStr);

			if(!TempStr.IsEmpty())
				m_PipeThick.AddString(TempStr);

			IRecordset_Pipe->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

	m_PipeThick.SetWindowText(TextStr);
	return TRUE;

}

////////////////////////////////////////////////////
//
// ���¡���װ�ص㡱��Ͽ�
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::UpdateBuildPlaceComBox()
{
	HRESULT hr;
	CString SQL;		//SQL���
	CString TempStr;	//��ʱ�ַ���
	CString TextStr;	//ǰһ��ѡ��İ�װ�ص�
	short	nIndex;		//��װ�ص��Ӧ��������
	short	pos;		//
	_RecordsetPtr IRecordset_Loc;

	m_BuidInPlace.GetWindowText(TextStr);

	//��������
	m_BuidInPlace.ResetContent();		
	if(GetPublicConnect()==NULL)
	{
		AfxMessageBox(_T("δ���ӹ������ݿ�"));
		return FALSE;
	}

	hr=IRecordset_Loc.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return FALSE;
	}

	SQL.Format(_T("SELECT * FROM [��װ�ص��] "));

	try
	{
		IRecordset_Loc->Open(_variant_t(SQL),GetPublicConnect().GetInterfacePtr(),
							 adOpenDynamic,adLockOptimistic,adCmdText);

		while(!IRecordset_Loc->adoEOF)
		{
			TempStr = vtos(IRecordset_Loc->GetCollect(_variant_t("LOC_NAME")));
			if(!TempStr.IsEmpty())
			{
				pos = m_BuidInPlace.AddString(TempStr);
				
				//�ص��Ӧ��������
				nIndex = vtoi(IRecordset_Loc->GetCollect(_variant_t("SiteIndex")));
				m_BuidInPlace.SetItemData(pos, nIndex);				
			}
			IRecordset_Loc->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}
	
	if( -1 == m_BuidInPlace.SelectString(-1, TextStr) )
	{
		m_BuidInPlace.SetCurSel(0);
	}
	return TRUE;
}


////////////////////////////////////////////////////////
//
// ���¡��ڱ��²�������ơ���Ͽ�
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::UpdateInsideMatNameCombox()
{
	HRESULT hr;
	CString SQL,TempStr,TextStr;
	int iRet;
	_RecordsetPtr IRecordset_Mat;

	m_InsideMatName.GetWindowText(TextStr);

	do
	{
		iRet=m_InsideMatName.DeleteString(0);
	}while(iRet!=CB_ERR);

	if(GetPublicConnect()==NULL)
	{
		AfxMessageBox(_T("δ���ӹ������ݿ�"));
		return FALSE;
	}

	hr=IRecordset_Mat.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return FALSE;		
	}

//	SQL.Format(_T("SELECT * FROM a_mat WHERE mat_code='PRE'"));
	SQL.Format(_T("SELECT * FROM a_mat WHERE mat_code='PRE' AND EnginID='%s' ORDER BY MAT_NAME"),GetProjectID());

	try
	{
//		IRecordset_Mat->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
//							 adOpenDynamic,adLockOptimistic,adCmdText);
		IRecordset_Mat->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
							 adOpenDynamic,adLockOptimistic,adCmdText);

		while(!IRecordset_Mat->adoEOF)
		{
			GetTbValue(IRecordset_Mat,_variant_t("MAT_NAME"),TempStr);

			if(!TempStr.IsEmpty())
				m_InsideMatName.AddString(TempStr);

			IRecordset_Mat->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

	m_InsideMatName.SetWindowText(TextStr);
	return TRUE;
}

//////////////////////////////////////////////////////
//
// ���¡��Ᵽ�²�������ơ���Ͽ�
//
// ��������ɹ�����TRUE�����򷵻�FALSE
//
BOOL CEditOriginalData::UpdateOutSizeMatNameComBox()
{
	HRESULT hr;
	CString SQL,TempStr,TextStr;
	int iRet;
	_RecordsetPtr IRecordset_Pipe;

	m_OutSizeMatName.GetWindowText(TextStr);

	do
	{
		iRet=m_OutSizeMatName.DeleteString(0);
	}while(iRet!=CB_ERR);

	if(GetPublicConnect()==NULL)
	{
		AfxMessageBox(_T("δ���ӹ������ݿ�"));
		return FALSE;
	}

	hr=IRecordset_Pipe.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return FALSE;
	}

//	SQL.Format(_T("SELECT * FROM a_mat WHERE mat_code='PRE'"));
	SQL.Format(_T("SELECT * FROM a_mat WHERE mat_code='PRE' AND EnginID='%s' ORDER BY MAT_NAME"),GetProjectID());

	try
	{
//		IRecordset_Pipe->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
//							  adOpenDynamic,adLockOptimistic,adCmdText);
		IRecordset_Pipe->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
							  adOpenDynamic,adLockOptimistic,adCmdText);

		while(!IRecordset_Pipe->adoEOF)
		{
			GetTbValue(IRecordset_Pipe,_variant_t("mat_name"),TempStr);

			if(!TempStr.IsEmpty())
				m_OutSizeMatName.AddString(TempStr);

			IRecordset_Pipe->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

	m_OutSizeMatName.SetWindowText(TextStr);
	return TRUE;
}

//////////////////////////////////////////////////////////
//
// ����"�������������"��Ͽ�
//
// ��������ɹ�����TRUE,���򷵻�FALSE
//
BOOL CEditOriginalData::UpdateSafeguardMatNameComBox()
{
	HRESULT hr;
	CString SQL,TempStr,TextStr;
	int iRet;
	_RecordsetPtr IRecordset_Mat;

	m_SafeguardMatName.GetWindowText(TextStr);

	do
	{
		iRet=m_SafeguardMatName.DeleteString(0);
	}while(iRet!=CB_ERR);

	if(GetPublicConnect()==NULL)
	{
		AfxMessageBox(_T("δ���ӹ������ݿ�"));
		return FALSE;
	}

	hr=IRecordset_Mat.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return FALSE;
	}

//	SQL.Format(_T("SELECT * FROM a_mat WHERE mat_code='PRO'"));
	SQL.Format(_T("SELECT * FROM a_mat WHERE mat_code='PRO' AND EnginID='%s' ORDER BY MAT_NAME"),GetProjectID());

	try
	{
//		IRecordset_Mat->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
//							 adOpenDynamic,adLockOptimistic,adCmdText);
		IRecordset_Mat->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
							 adOpenDynamic,adLockOptimistic,adCmdText);

		while(!IRecordset_Mat->adoEOF)
		{
			GetTbValue(IRecordset_Mat,_variant_t("mat_name"),TempStr);

			if(!TempStr.IsEmpty())
				m_SafeguardMatName.AddString(TempStr);

			IRecordset_Mat->MoveNext();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

	m_SafeguardMatName.SetWindowText(TextStr);
	return TRUE;
}

////////////////////////////////////////////////////////////
//
// ����a_vol���Ҳ����Ի����еľ���ʱ,����a_vol��
//
// �����ɹ�����TRUE,���򷵻�FALSE
//
BOOL CEditOriginalData::UpdateVolTable()
{
	CString SQL,csVol,csPipeDeviceName,csTemp;
	HRESULT hr;
	_RecordsetPtr IRecordset_Vol;

	hr=IRecordset_Vol.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
		return FALSE;

	csVol=m_VolNo;
	m_PipeDeviceName.GetWindowText(csPipeDeviceName);

	csVol.TrimLeft();
	csVol.TrimRight();
	csPipeDeviceName.TrimLeft();
	csPipeDeviceName.TrimRight();

	csVol=csVol.Left(5);

	if(csVol.IsEmpty() || csPipeDeviceName.IsEmpty())
		return TRUE;

	SQL.Format(_T("SELECT * FROM a_vol WHERE LEFT(vol,5)='%s' AND vol_name='%s'"),csVol,csPipeDeviceName);


	try
	{
		IRecordset_Vol->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
							adOpenDynamic,adLockOptimistic,adCmdText);

		if(IRecordset_Vol->adoEOF && IRecordset_Vol->BOF)
		{
			//�˴������µľ�����ӵ���A_VOL�С������EXCELʱ�ִ�Volume��ȡ��
			IRecordset_Vol->AddNew();
			
			IRecordset_Vol->PutCollect(_variant_t("vol"),_variant_t(csVol));
			IRecordset_Vol->PutCollect(_variant_t("vol_name"),_variant_t(csPipeDeviceName));
			IRecordset_Vol->PutCollect(_variant_t("vol_price"),_variant_t(m_PriceRatio));

			this->m_ColorRing.GetWindowText(csTemp);
			csTemp.Format("%d",_tcstol(csTemp, NULL, 10 ));
			IRecordset_Vol->PutCollect(_variant_t("vol_colr"),_variant_t(csTemp));

			IRecordset_Vol->Update();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}

	return TRUE;
}

void CEditOriginalData::OnDropdownColor() 
{
	UpdateColorComBox();	
}

void CEditOriginalData::OnDropdownPipeDeviceName() 
{
	UpdatePipeDeviceNameComBox();	
}

void CEditOriginalData::OnDropdownPipeSize() 
{
	UpdatePipeSizeComBox();	
}

void CEditOriginalData::OnDropdownPipeThick() 
{
	UpdatePipeThickComBox();	
}

void CEditOriginalData::OnDropdownInsideMatname() 
{
	UpdateInsideMatNameCombox();	
}

void CEditOriginalData::OnDropdownOutsizeMatname() 
{
	UpdateOutSizeMatNameComBox();	
}

void CEditOriginalData::OnDropdownSafeguardMatname() 
{
	UpdateSafeguardMatNameComBox();	
}

void CEditOriginalData::OnItemchangedHeatPreservationType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString TempStr;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->iItem==-1)
	{
		*pResult=0;
		return;
	}

	UpdateData();

	m_VolNo.TrimLeft();
	if(m_VolNo.IsEmpty())
	{
		*pResult=0;
		return;
	}

	while(m_VolNo.GetLength()/sizeof(TCHAR)<5)
	{
		m_VolNo+=_T(" ");
	}

	m_VolNo=m_VolNo.Left(5);
	TempStr=m_HeatPreservationTypeList.GetItemText(pNMListView->iItem,0);
	m_VolNo+=TempStr;

	UpdateData(FALSE);
	UpdateControlsState();
	
	*pResult = 0;
}

void CEditOriginalData::OnItemchangedVollist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(pNMListView->iItem==-1)
	{
		*pResult=0;
		return;
	}
	CString	strDeviceName;		//�ܵ�/�豸����
	CString	strPriceRatio;		// �ȼ۱�������

	m_VolNo=m_VolList.GetItemText(pNMListView->iItem,0);
	
	strDeviceName = m_VolList.GetItemText(pNMListView->iItem,1);
	if ( !strDeviceName.IsEmpty() )
	{
		m_PipeDeviceName.SetWindowText(strDeviceName);
	}
	if ( 1 == gbIsSelTblPrice )
	{
		strPriceRatio = m_VolList.GetItemText(pNMListView->iItem, 2);
		m_PriceRatio = strtod( strPriceRatio, NULL );
	}
	UpdateData(FALSE);
	UpdateControlsState();

	*pResult = 0;
}

void CEditOriginalData::OnUpdateAllCheckBox() 
{
	UpdateControlsState();
}

void CEditOriginalData::OnChangeVolno() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateControlsState();	
}
//��Ӧ��¼�ŵ�ֱ�������
void CEditOriginalData::OnChangeId() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	int Num;

	UpdateData();

	try
	{
		if(m_ICurrentProjectRecordset->adoEOF && m_ICurrentProjectRecordset->BOF)
		{
			m_ID=0;
			UpdateData(FALSE);
			return;
		}

		if(m_ID<=0)
			Num=1;
		else
			Num=m_ID;

		m_ICurrentProjectRecordset->MoveFirst();
		
		while(!m_ICurrentProjectRecordset->adoEOF)
		{
			Num--;
			if(Num>0)
				m_ICurrentProjectRecordset->MoveNext();
			else
				break;
		}

		if(m_ICurrentProjectRecordset->adoEOF)
			m_ICurrentProjectRecordset->MovePrevious();
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return;
	}

	PutDataToEveryControl();
	UpdateControlsState();
	//2005.2.24
	//�Զ�ѡ����ϡ�
	if( this->GetIsAutoSelectMatOnRoll() )
	{
		this->OnAutoSelectmat();
	}
}

//////////////////////////////////////////////
//
// ���¸����ؼ���״̬
//
void CEditOriginalData::UpdateControlsState()
{
	CString TempStr;
	CString strSize;
	CString	strSQL;

	UINT uControlIDs[]=
	{
		IDC_ID,
		IDC_FOREFRONT,
		IDC_PREVIOUS,
		IDC_SUBSEQUENT,
		IDC_LAST,
		IDC_DELCURRENT,
		IDC_DEL_ALL_INSU,
		IDC_SAVECURRENT,
		IDC_UPDATABYROLL,
		IDC_VOLNO,
		IDC_COLOR,
		IDC_PIPE_DEVICE_NAME,
		IDC_PIPE_SIZE,
		IDC_PIPE_THICK,
		IDC_MEDIUM_TEMPERATURE,
		IDC_SAFEGUARD_MATNAME,
		IDC_BUILDIN_PLACE,
		IDC_PIPE_MAT,
		IDC_REMARK,
		IDC_VOLLIST_ISENABLE,
		IDC_HEAT_PRESERVATION_TYPE_ENABLE,
		IDC_VOLLIST,
		IDC_HEAT_PRESERVATION_TYPE,
		IDC_WIDE_SPEED,
		IDC_PRICE,
		IDC_RUN_PERHOUR,
		IDC_AUTO_SELECTMAT,
		IDC_CHECK_AUTOSELECT_PRE_EDIT,
		IDC_SORT_BY_VOL,
		IDC_AMOUNT,						// ����
		IDC_ENVIRON_TEMP,
		IDC_CHECK_TS,
		IDC_EDIT_TS,
		IDC_CHECK_TA,
		IDC_EDIT_TA,
		IDC_CHECK_METHOD,
		IDC_EDIT_METHOD,
		IDC_HEAT_TRANSFER_COEF,
		IDC_CHECK_ENV_TEMP,
		IDC_CHECK_ALPHA,
		IDC_PIPE_PRESSURE,
		IDC_PIPE_MEDIUM,
		IDC_BUTTON_ADD_VALVE,
		IDC_EDIT_PLAIM_THICK,
		IDC_BUTTON_CALC_RATIO
	};

	if(m_ICurrentProjectRecordset->adoEOF && m_ICurrentProjectRecordset->BOF)
	{
		//û��ԭʼ����
		int i=0;
		for(i=0;i<sizeof(uControlIDs)/sizeof(UINT);i++)
		{
			GetDlgItem(uControlIDs[i])->EnableWindow(FALSE);
		}
		m_ctlCalcMethod.SetCurSel(-1);
		ShowWindowRect();
		UINT controlID[] = // ��Ҫ���⴦���һЩ�ؼ�(��ؼ���ʱ��
		{
			IDC_CHECK_THICK1,
			IDC_EDIT_THICK1,
			IDC_CHECK_THICK2,
			IDC_EDIT_THICK2,
			IDC_INSIDE_MATNAME,
			IDC_OUTSIZE_MATNAME,
		};
		for ( i = 0 ; i < sizeof(controlID) / sizeof(UINT) ; i ++)
		{
			GetDlgItem(controlID[i])->EnableWindow(FALSE);
		}

		return ;
	}
	else
	{
		for(int i=0;i<sizeof(uControlIDs)/sizeof(UINT);i++)
		{
			GetDlgItem(uControlIDs[i])->EnableWindow(TRUE);
		}
	}

	UpdateData();

	if(m_ICurrentProjectRecordset->BOF)
	{
		m_MoveForefront.EnableWindow(FALSE);
		m_MovePrevious.EnableWindow(FALSE);
	}
	else
	{
		m_ICurrentProjectRecordset->MovePrevious();

		if(m_ICurrentProjectRecordset->BOF)
		{
			m_MoveForefront.EnableWindow(FALSE);
			m_MovePrevious.EnableWindow(FALSE);
			m_ICurrentProjectRecordset->MoveFirst();

		}
		else
		{
			m_MoveForefront.EnableWindow(TRUE);
			m_MovePrevious.EnableWindow(TRUE);
			m_ICurrentProjectRecordset->MoveNext();
		}		
	}

	if(m_ICurrentProjectRecordset->adoEOF)
	{
		m_MoveLast.EnableWindow(FALSE);
		m_MoveSubsequent.EnableWindow(FALSE);
	}
	else
	{
		m_ICurrentProjectRecordset->MoveNext();

		if(m_ICurrentProjectRecordset->adoEOF)
		{
			m_MoveLast.EnableWindow(FALSE);
			m_MoveSubsequent.EnableWindow(FALSE);
			m_ICurrentProjectRecordset->MoveLast();
		}
		else
		{
			m_MoveLast.EnableWindow(TRUE);
			m_MoveSubsequent.EnableWindow(TRUE);
			m_ICurrentProjectRecordset->MovePrevious();
		}
	}

	//��ѡ���᣺
	if(m_IsVolListEnable)
	{
		if( GetDlgItem(IDC_VOLLIST_ISENABLE)->IsWindowEnabled() )
			m_VolList.EnableWindow(TRUE);
		else
			m_VolList.EnableWindow(FALSE);
	}
	else
	{
		m_VolList.EnableWindow(FALSE);
	}
	//��ѡ�����������ͣ�
	if(m_IsHeatPreservationTypeEnable)
	{
		if( GetDlgItem(IDC_HEAT_PRESERVATION_TYPE_ENABLE)->IsWindowEnabled() )
			m_HeatPreservationTypeList.EnableWindow(TRUE);
		else
			m_HeatPreservationTypeList.EnableWindow(FALSE);
	}
	else
	{
		m_HeatPreservationTypeList.EnableWindow(FALSE);
	}
	//ѡ����㷽��.
	GetDlgItem(IDC_EDIT_METHOD)->EnableWindow(((CButton*)GetDlgItem(IDC_CHECK_METHOD))->GetCheck()?TRUE:FALSE);
	
	//������¶�.
	GetDlgItem(IDC_EDIT_TS)->EnableWindow(((CButton*)GetDlgItem(IDC_CHECK_TS))->GetCheck()?TRUE:FALSE);
	//�����¶�.
	GetDlgItem(IDC_EDIT_TA)->EnableWindow(((CButton*)GetDlgItem(IDC_CHECK_TA))->GetCheck()?TRUE:FALSE);

	//ֻ�е��ܵ�����Ϊ�͹ܵ�����ˮ�ܵ����������ܵ�ʱ�������������ӷ���
	TempStr = m_VolNo;
	TempStr.TrimRight();
	TempStr = TempStr.Right(1);

	m_PipeSize.GetWindowText(strSize);
	
	if (strtod(strSize,NULL) < m_dMaxD0 ) // �ܵ���¼
	{
		if (!TempStr.CompareNoCase("O") || !TempStr.CompareNoCase("M") || !TempStr.CompareNoCase("S"))
		{
			GetDlgItem(IDC_BUTTON_ADD_VALVE)->EnableWindow(TRUE);
		}else
		{
			GetDlgItem(IDC_BUTTON_ADD_VALVE)->EnableWindow(FALSE);
		}

		GetDlgItem(IDC_EDIT_PLAIM_THICK)->EnableWindow(FALSE);
	}else	// ƽ��
	{
		GetDlgItem(IDC_EDIT_PLAIM_THICK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ADD_VALVE)->EnableWindow(FALSE);
	}
	try
	{
		_RecordsetPtr pRs;
		pRs.CreateInstance(__uuidof(Recordset));
		strSQL.Format("SELECT * FROM [Pipe_Valve] WHERE ValveID=%d AND EnginID='%s' ",
				m_ID, m_ProjectID);	
		pRs->Open(_variant_t(strSQL), m_ICurrentProjectConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if (pRs->GetRecordCount() > 0)
		{
			m_PipeSize.EnableWindow(FALSE);
		}
		//ѡ����ѡ���ˡ�ѡ����е��ȼ۱������ۡ�ʱ���ã�������Ϊ,ˮ������ ʱ����ʽ�����ȼ۱�������
		if (  1 == gbIsSelTblPrice )
		{
			GetDlgItem( IDC_BUTTON_CALC_RATIO )->EnableWindow( TRUE );
		}
		else
		{
			CString strMediumName;
			CString strTmp;
			GetDlgItem( IDC_BUTTON_CALC_RATIO )->EnableWindow( FALSE );
			int pos = m_ctlPipeMedium.GetCurSel();
			if ( -1 != pos )
			{
				m_ctlPipeMedium.GetLBText( pos, strMediumName);
			}else
			{
				m_ctlPipeMedium.GetWindowText( strMediumName );
			}
			if ( !strMediumName.IsEmpty() )
			{
				CHeatPreCal::GetMediumDensity( strMediumName, &strTmp );
				if ( !strTmp.CompareNoCase( "ˮ" ) || !strTmp.CompareNoCase("����") )
				{
					GetDlgItem( IDC_BUTTON_CALC_RATIO )->EnableWindow( TRUE );
				}
			}		
		}
	}
	catch (_com_error)
	{
	}

	m_ctlCalcMethod.GetWindowText(TempStr);
	if (m_ctlCalcMethod.FindString(-1, TempStr) != nSubterraneanMethod)
	{
		UpdateControlSubterranean( -1 );
//		//�ڱ��º��
//		GetDlgItem(IDC_EDIT_THICK1)->EnableWindow(((CButton*)GetDlgItem(IDC_CHECK_THICK1))->GetCheck()?TRUE:FALSE);
//		//�Ᵽ�º��
//		GetDlgItem(IDC_EDIT_THICK2)->EnableWindow(((CButton*)GetDlgItem(IDC_CHECK_THICK2))->GetCheck()?TRUE:FALSE);
	}
	
}

//���ܣ�
//��ԭʼ�����е�һЩ�ֶε�ֵ��Ϊ�ο��������������ֶ�ֵ
BOOL CEditOriginalData::Refresh()
{
	double	e_rate = 0;
	double	con_temp1 = 0;
	double	con_temp2 = 0;
	double	e_hours = 0;
	double	e_wind = 0;

	double	Pro_thi;				//��������
//	double	dConTemp;				//�����¶ȡ�

	_RecordsetPtr IRecord_Config;
	_variant_t TempVar;
	CString SQL,TempStr;
	HRESULT hr;

	hr=IRecord_Config.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return FALSE;
	}

	SQL.Format(_T("SELECT * FROM a_config WHERE EnginID='%s'"),GetProjectID());

	if(m_ICurrentProjectRecordset->adoEOF || m_ICurrentProjectRecordset->BOF)
	{
		return TRUE;
	}

	try
	{
		IRecord_Config->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
							 adOpenStatic,adLockOptimistic,adCmdText);

		if ( IRecord_Config->GetRecordCount() > 0 )
		{
			GetTbValue(IRecord_Config,_variant_t("�������"),e_rate);
			GetTbValue(IRecord_Config,_variant_t("�����¶�"),con_temp1);
			GetTbValue(IRecord_Config,_variant_t("�����¶�"),con_temp2);
			GetTbValue(IRecord_Config,_variant_t("������Сʱ"),e_hours);
			GetTbValue(IRecord_Config,_variant_t("�������"),e_wind);
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}
 
	try
	{
		//////////Ӧ�����½�������////////////////////////
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_rate"),_variant_t(e_rate));
		m_ICurrentProjectRecordset->Update();
		////////////////////////////////////////////
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_vol"),TempStr);
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return FALSE;
	}


	/////////////////////////�п��ܲ���Ҫ///////////////////
	TempStr.TrimLeft();
	if(TempStr.IsEmpty())
	{
		GetDlgItem(IDC_VOLNO)->SetFocus();
	}
	////////////////////////////////////////////////////
/*		ȡ��ѡ���͹ܵ�ʱ������������ //  [2005/06/02] 
	if(TempStr.Right(1)==_T("O"))
	{
		GetDlgItem(IDC_HEAT_PRESERVATION_THICK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_HEAT_PRESERVATION_THICK)->EnableWindow(FALSE);
		GetDlgItem(IDC_HEAT_PRESERVATION_THICK)->SetWindowText(_T("0"));
		try
		{
			TempVar.Clear();
			TempVar.vt=VT_NULL;
			m_ICurrentProjectRecordset->PutCollect(_variant_t("c_pre_thi"),TempVar);
			m_ICurrentProjectRecordset->Update();
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());
			return FALSE;
		}
	}
*/
	try
	{
		GetTbValue(m_ICurrentProjectRecordset,_variant_t("c_name3"),TempStr);
		if(!TempStr.IsEmpty())
		{
			if(TempStr.Find(_T("("))==-1)
			{
				Pro_thi=0.5;//Ĭ�ϱ�������0.5mm
			}
			else
			{
				TCHAR *StopString;

				TempStr=TempStr.Mid(TempStr.Find(_T("("))+1);
				Pro_thi=static_cast<double>(_tcstod(TempStr,&StopString));
			}

			try
			{
				m_ICurrentProjectRecordset->PutCollect(_variant_t("c_pro_thi"),_variant_t(Pro_thi));
				m_ICurrentProjectRecordset->Update();
			}
			catch(_com_error &e)
			{
				ReportExceptionError(e);
				return FALSE;
			}
		}
/*		//��װ�ص㡣
		TempStr = vtos(m_ICurrentProjectRecordset->GetCollect(_variant_t("c_place")));
		if(!TempStr.IsEmpty())
		{
			CString place,steam;
			place=TempStr.Left(4);
			steam=TempStr.Right(4);
			try
			{
				m_ICurrentProjectRecordset->PutCollect(_variant_t("c_place"),_variant_t(place));
				m_ICurrentProjectRecordset->PutCollect(_variant_t("c_steam"),_variant_t(steam));
				if(place==_T("����"))
				{
					m_ICurrentProjectRecordset->PutCollect(_variant_t("c_con_temp"),_variant_t(con_temp1));
				}
				else
				{
					m_ICurrentProjectRecordset->PutCollect(_variant_t("c_con_temp"),_variant_t(con_temp2));
				}

			}
			catch(_com_error &e)
			{
				ReportExceptionError(e);
				return FALSE;
			}
		}
*/		//�����¶�ȡֵ���û�ѡ�������ȷ��.
		//���û��,�����������.����ȡֵ.
	//	if( GetConditionTemp(dConTemp, nMethodIndex) )
	//	{
	//		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_con_temp"), _variant_t(dConTemp));
	//	}
		//�����¶�ֵ
	//	m_dTa = dConTemp;

		//���½��ʵ�����
		
		//������ݵ���ȷ��
		if ( !CheckDataValidity() )
		{
			return false;
		}
		

		m_ICurrentProjectRecordset->Update();
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return TRUE;
}

//////////////////////////////////////////////
//
// ��Ӧ���Զ�ѡ���²��ϡ�
//
void CEditOriginalData::OnAutoSelectmat() 
{
	double e_hours,e_wind;

	_RecordsetPtr IRecord_Config;
	_variant_t TempVar;
	CString SQL;
	CString txtC_vol,txtC_size,txtC_temp,txtC_p_s;
	CString name_pre,name_pro,name_prein;
	HRESULT hr;

	UpdateData();

	hr=IRecord_Config.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return;
	}
	
	SQL.Format(_T("SELECT * FROM a_config WHERE EnginID='%s' "),GetProjectID());

	try
	{
		IRecord_Config->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
							 adOpenDynamic,adLockOptimistic,adCmdText);

		if (IRecord_Config->adoEOF && IRecord_Config->BOF)
		{
			AfxMessageBox("������Ƴ�����Ϊ��!");
		}
		else
		{
			GetTbValue(IRecord_Config,_variant_t("������Сʱ"),e_hours);
			GetTbValue(IRecord_Config,_variant_t("�������"),e_wind);
			
			m_ICurrentProjectRecordset->PutCollect(_variant_t("c_hour"),_variant_t(e_hours));
			// �ı����ʱ,���жϰ�װ�ص�.		 [2005/06/23]
/*
			m_BuidInPlace.GetWindowText(txtC_p_s);
			if (-1 != txtC_p_s.Find("����"))
			{
				//���ڵķ���Ϊ0;
				e_wind = 0;
			}
*/			
			m_ICurrentProjectRecordset->PutCollect(_variant_t("c_wind"),_variant_t(e_wind));
			
			//���Ի����еĿؼ����ݽ��и���
			m_RunPerHour=e_hours;
			m_WideSpeed=e_wind;
			
			this->UpdateData(FALSE);
			
			m_ICurrentProjectRecordset->Update();
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return;
	}

	GetDlgItem(IDC_VOLNO)->GetWindowText(txtC_vol);
	GetDlgItem(IDC_PIPE_SIZE)->GetWindowText(txtC_size);
	GetDlgItem(IDC_MEDIUM_TEMPERATURE)->GetWindowText(txtC_temp);
	GetDlgItem(IDC_BUILDIN_PLACE)->GetWindowText(txtC_p_s);

	txtC_vol.TrimLeft();
	txtC_size.TrimLeft();
	txtC_temp.TrimLeft();
	txtC_p_s.TrimLeft();

	if(txtC_vol.IsEmpty() || txtC_size.IsEmpty() || txtC_temp.IsEmpty() || txtC_p_s.IsEmpty())
	{
		return;
	}	

	try
	{
		AUTO(txtC_vol,txtC_size,txtC_temp,name_pre,name_pro,txtC_p_s,name_prein);
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return;
	}
/*	//test  �����ȼ۱�������
	double P,T,H,S;
	int R;
	P = 1;
	T = 350;
	int stdid=97;
    SETSTD_WASP(stdid);  //���ü����׼ IF97/IFC67  ��������ѡ�Ի����м�һ������
	
	PT2H(P,T,&H,&R);
	PT2S(P,T,&S,&R);
//*/
	GetDlgItem(IDC_INSIDE_MATNAME)->SetWindowText(name_prein);
	GetDlgItem(IDC_OUTSIZE_MATNAME)->SetWindowText(name_pre);
	GetDlgItem(IDC_SAFEGUARD_MATNAME)->SetWindowText(name_pro);

}

void CEditOriginalData::compound(CString &mat_in, CString &mat)
{
	CString SQL;
	_RecordsetPtr IRecordset_Compnd;

	IRecordset_Compnd.CreateInstance(__uuidof(Recordset));

//	SQL.Format(_T("SELECT * FROM a_compnd"));
	SQL.Format(_T("SELECT * FROM a_compnd WHERE EnginID='%s'"),GetProjectID());

	try
	{
//		IRecordset_Compnd->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
//								adOpenDynamic,adLockOptimistic,adCmdText);
		IRecordset_Compnd->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
								adOpenDynamic,adLockOptimistic,adCmdText);

		if (LocateFor(IRecordset_Compnd,_variant_t("complex"),CFoxBase::EQUAL,_variant_t(mat)))
		{
			if (!IRecordset_Compnd->adoEOF)
			{
				mat_in	= vtos( IRecordset_Compnd->GetCollect(_variant_t("comp_in")) );
				mat		= vtos( IRecordset_Compnd->GetCollect(_variant_t("comp_out")) );
			}
		}		
	}
	catch(_com_error)
	{
		throw;
	}
}

void CEditOriginalData::cond(_RecordsetPtr &IRecordset, CString &mat,CString &v,
							 CString &p,CString t,CString d,CString h)
{
//	CString SQL;
	TCHAR *StopString;
	CString a,k,TempStr,con;
	BOOL Bool,bRet;
	CParseExpression Parse;
	int c1,rec,max_k;

	CParseExpression::tagExpressionVar Var[5];

	try
	{
		if (IRecordset->adoEOF && IRecordset->BOF)
		{
			return;
		}
		max_k=IRecordset->GetFields()->GetCount()-2;

		IRecordset->MoveFirst();
		while(!IRecordset->adoEOF)
		{
			this->GetTbValue(IRecordset,_variant_t("auto_obj"),TempStr);

			TempStr.TrimLeft();
			if(!TempStr.IsEmpty())
				break;

			IRecordset->MoveNext();
		}

		if(IRecordset->adoEOF)
			return;

		c1=RecNo(IRecordset);
	}
	catch(_com_error)
	{
		throw;
	}

	Bool=FALSE;

	Var[0].VarName=_T("v");
	Var[0].VarValue=v;
	Var[1].VarName=_T("p");
	Var[1].VarValue=p;
	Var[2].VarName=_T("t");
	Var[2].VarValue=_tcstod(t,&StopString);
	Var[3].VarName=_T("d");
	Var[3].VarValue=_tcstod(d,&StopString);
	Var[4].VarName=_T("h");
	Var[4].VarValue=h;

	Parse.SetConnectionForParse(GetPublicConnect());

	try
	{
		while(!IRecordset->adoEOF)
		{
			GetTbValue(IRecordset,_variant_t("auto_obj"),a);

			bRet=Parse.ParseUseConn_Bool(a,Var,5,NULL);
			if(bRet)
			{
				rec=RecNo(IRecordset);
				Bool=TRUE;
				break;
			}
			IRecordset->MoveNext();
		}

		IRecordset->MoveFirst();
	}
	catch(_com_error)
	{
		throw;
	}
	
	k=_T("1");

//	DO WHILE RECNO()<C1 .AND. VAL(k)<=max_k
	try
	{
		while(RecNo(IRecordset)<c1 && atof(k)<=max_k)
		{
	//		con=auto_con&k
			TempStr=_T("auto_con");
			TempStr+=k;

			GetTbValue(IRecordset,_variant_t(TempStr),con);
			con.TrimLeft();
	//		IF ""=TRIM(con)
			if(con.IsEmpty())
			{
	//			SKIP
				IRecordset->MoveNext();
				continue;
	//			LOOP
			}
	//		ENDIF

			bRet=Parse.ParseUseConn_Bool(con,Var,5,NULL);
	//		IF &con
	//			SKIP
	//		ELSE
	//			k=STR(VAL(k)+1,1)
	//			GO TOP
	//			LOOP
	//		ENDIF

			if(bRet)
			{
				IRecordset->MoveNext();
			}
			else
			{
				TempStr.Format(_T("%d"),_tcstol(k,&StopString,10)+1);
				k=TempStr;
				IRecordset->MoveFirst();
				continue;
			}
		}//	ENDDO

		IRecordset->MoveFirst();
		if(Bool)
		{
			while(TRUE)
			{
				rec--;
				if(rec>0)
					IRecordset->MoveNext();
				else
					break;
			}

			TempStr=_T("auto_con");
			TempStr+=k;
			GetTbValue(IRecordset,_variant_t(TempStr),mat);
		}
	}
	catch(_com_error)
	{
		throw;
	}
}

void CEditOriginalData::AUTO(CString &v, CString &s, CString &t, CString &pre, CString &pro, CString &p, CString &pre_in)
{
	CString d;
	CString mat,mat_in,h;
	CString SQL;
	CString csTemp;
	int max_k;

	_RecordsetPtr IRecordset_pre;

	d=s;
	p=p.Left(4);
	mat=_T("�Ҳ���!");

	IRecordset_pre.CreateInstance(__uuidof(Recordset));
//	SQL.Format(_T("SELECT * FROM a_pre"));
	SQL.Format(_T("SELECT * FROM a_pre WHERE EnginID='%s'"),GetProjectID());

	try
	{
//		IRecordset_pre->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
//							 adOpenDynamic,adLockOptimistic,adCmdText);
		IRecordset_pre->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
							 adOpenDynamic,adLockOptimistic,adCmdText);

		max_k=IRecordset_pre->GetFields()->GetCount()-2;

		cond(IRecordset_pre,mat,v,p,t,d);

		mat_in=pre_in=_T("");

		if(mat.Find(_T("������ʽ"))!=-1)
		{
			compound(mat_in,mat);
			pre_in=mat_in;
		}
	}
	catch(_com_error)
	{
		throw;
	}

	pre=mat;

	_RecordsetPtr IRecordset_mat;
	IRecordset_mat.CreateInstance(__uuidof(Recordset));

//	SQL.Format(_T("SELECT * FROM a_mat"));
	SQL.Format(_T("SELECT * FROM a_mat WHERE EnginID='%s'"),GetProjectID());

	try
	{
//		IRecordset_mat->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
//							adOpenDynamic,adLockOptimistic,adCmdText);
		IRecordset_mat->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
							adOpenDynamic,adLockOptimistic,adCmdText);

//		LocateFor(IRecordset_mat,_variant_t("mat_name"),CFoxBase::EQUAL,_variant_t(pre));

		while(!IRecordset_mat->adoEOF)
		{
			GetTbValue(IRecordset_mat,_variant_t("mat_name"),csTemp);

			if(csTemp==pre)
				break;

			IRecordset_mat->MoveNext();
		}

		if(IRecordset_mat->adoEOF)
		{
			h=_T("");
		}
		else
		{
			GetTbValue(IRecordset_mat,_variant_t("mat_h"),h);
		}

		mat=_T("�Ҳ���!");

	//	SELE 5
	//	USE a_pro
		_RecordsetPtr IRecordset_pro;
		IRecordset_pro.CreateInstance(__uuidof(Recordset));

//		SQL.Format(_T("SELECT * FROM a_pro"));
//		IRecordset_pro->Open(_variant_t(SQL),_variant_t((IDispatch*)GetPublicConnect()),
//							adOpenDynamic,adLockOptimistic,adCmdText);

		SQL.Format(_T("SELECT * FROM a_pro WHERE EnginID='%s'"),GetProjectID());
		IRecordset_pro->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
							adOpenDynamic,adLockOptimistic,adCmdText);


	//	max_k=FCOUNT()-2
		max_k=IRecordset_pro->GetFields()->GetCount()-2;
	//	DO cond WITH mat
		cond(IRecordset_pro,mat,v,p,t,d,h);
	//	pro=mat
		pro=mat;
	}
	catch(_com_error)
	{
		throw;
	}
}

///////////////////////////////////////////
//
// ��Ӧ�����������š���Ť
//
void CEditOriginalData::OnSortByVol() 
{
	CString csSQL;
	long iNumber;

	try
	{
		UpdateData();
		
		if(m_ProjectID.IsEmpty())
		{
			AfxMessageBox(_T("δѡ�񹤳�"));
			return;
		}

		if(m_ICurrentProjectConnection==NULL)
		{
			AfxMessageBox(_T("δ�������ݿ�"));
			return;
		}

		if(!PutDataToDatabaseFromControl())
			return;

		if(!Refresh())
			return;
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return;
	}

	try
	{
		if(!(m_ICurrentProjectRecordset->GetState()==adStateClosed))
		{
			m_ICurrentProjectRecordset->Close();
		}

		csSQL.Format(_T("SELECT * FROM pre_calc WHERE EnginID='%s' ORDER BY c_vol ASC,c_size DESC"),GetProjectID());

		m_ICurrentProjectRecordset->Open(_variant_t(csSQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
										 adOpenStatic,adLockOptimistic,adCmdText);

		iNumber=1;
		while(!m_ICurrentProjectRecordset->adoEOF)
		{
			m_ICurrentProjectRecordset->PutCollect(_variant_t("id"),_variant_t(iNumber));
			m_ICurrentProjectRecordset->Update();
			iNumber++;
			
			m_ICurrentProjectRecordset->MoveNext();
		}

		if(m_ICurrentProjectRecordset->adoEOF && m_ICurrentProjectRecordset->BOF)
			return;

		m_ICurrentProjectRecordset->MoveFirst();
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return;
	}

	PutDataToEveryControl();
	UpdateControlsState();

}

////////////////////////////////////////////////////
//
// ��Ӧ���˳�����Ť
//
void CEditOriginalData::OnExit() 
{
	UpdateData();
	try
	{
		if(m_IsUpdateByRoll)
		{
			if ( PutDataToDatabaseFromControl() )
				Refresh();
		}
	}
	catch(_com_error)
	{	
	}
	bIsMoveUpdate = m_IsUpdateByRoll;			//�ƶ�����.
	bIsAutoSelectPre = m_bIsAutoSelectMatOnRoll;//�ƶ�ѡ��.

	OnOK();
	//2005.2.24
	EndDialog(0);
	if( IsWindow(this->GetSafeHwnd()) )
		DestroyWindow();
}

/////////////////////////////////////////////////////////////////
//
// �����ڹ���ʱ�Ƿ��Զ�ѡ��
//
// bSelect[in]	��ΪTRUEʱ������ǰ��������ƶ���¼ʱ�Զ�ѡ��
//				��ΪFALSE��֮
//
void CEditOriginalData::SetIsAutoSelectMatOnRoll(BOOL bSelect)
{
	m_bIsAutoSelectMatOnRoll=bSelect;
}

///////////////////////////////////////////////////////////////
//
// �����Ƿ��ڹ���ʱ�Զ�ѡ��
// 
// ������ΪTRUEʱ�Զ�ѡ�ģ������Զ�ѡ��
//
BOOL CEditOriginalData::GetIsAutoSelectMatOnRoll()
{
	return m_bIsAutoSelectMatOnRoll;
}


void CEditOriginalData::OnCancel()
{
	EndDialog( 0 );
	if( IsWindow( this->GetSafeHwnd()) )
	{
		DestroyWindow();
	}
	CDialog::OnClose();
}
//ɾ�����еı���ԭʼ���ݡ�
void CEditOriginalData::OnDelAllInsu() 
{
	if(	AfxMessageBox(_T("�Ƿ�ɾ�����е�ԭʼ����!"),MB_YESNO+MB_ICONQUESTION+MB_DEFBUTTON2) == IDNO )
	{
		return ;
	}
	try
	{
		CString strSQL;
		//���ԭʼ����ʱ��
		strSQL = "DELETE * FROM [PRE_CALC] WHERE EnginID='"+GetProjectID()+"' ";
		m_ICurrentProjectConnection->Execute(_bstr_t(strSQL), NULL, -1);

		//���ԭʼ����ʱ,�ڹܵ�-����ӳ�����ҲҪɾ����Ӧ�ļ�¼		 [2005/06/27]
		strSQL.Format("DELETE * FROM [PIPE_VALVE] WHERE EnginID='%s'",m_ProjectID);
		m_ICurrentProjectConnection->Execute(_bstr_t(strSQL), NULL, adCmdText);

		// ������¼
		strSQL.Format("DELETE * FROM [PRE_CALC_CONGEAL] WHERE EnginID='%s'", m_ProjectID);
		m_ICurrentProjectConnection->Execute( _bstr_t(strSQL), NULL, adCmdText);

		// �������
		strSQL.Format("DELETE * FROM [PRE_CALC_SUBTERRANEAN] WHERE EnginID='%s'", m_ProjectID);
		m_ICurrentProjectConnection->Execute( _bstr_t(strSQL), NULL, adCmdText);

		if( this->InitCurrentProjectRecordset() != TRUE )
		{
			OnCancel();
			return;
		}
		this->UpdateControlsState();
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return;
	}

}
/////////////////////////////////
//
//�����еı���ԭʼ�����Զ�ѡ����ϡ�
bool CEditOriginalData::AutoSelAllMat()
{
	//��ԭʼ���ݱ�
	if( !this->InitCurrentProjectRecordset() )
		return false;

	int Count = m_ICurrentProjectRecordset->GetRecordCount();	//��¼����.
	if( Count <= 0 )
	{
		return false;
	}

	double e_hours=0,e_wind=0;
	_RecordsetPtr IRecord_Config;
//	_variant_t TempVar;
	CString SQL, strPass;
	CString txtC_vol,txtC_size,txtC_temp,txtC_p_s;
	CString name_pre,name_pro,name_prein;
	HRESULT hr;
	hr=IRecord_Config.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		ReportExceptionError(_com_error((HRESULT)hr));
		return false;
	}	
	SQL.Format(_T("SELECT * FROM a_config WHERE EnginID='%s'"),GetProjectID());
	try
	{
		IRecord_Config->Open(_variant_t(SQL),_variant_t((IDispatch*)GetCurrentProjectConnect()),
							 adOpenDynamic,adLockOptimistic,adCmdText);
		if( !IRecord_Config->adoEOF && !IRecord_Config->BOF )
		{
			e_hours = vtof(IRecord_Config->GetCollect(_variant_t("������Сʱ")));
			e_wind =  vtof(IRecord_Config->GetCollect(_variant_t("�������")));
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionError(e);
		return false;
	}
	int pos = 1;
	//�����м�¼����ѡ���½ṹ
	for(m_ICurrentProjectRecordset->MoveFirst(); !m_ICurrentProjectRecordset->adoEOF; m_ICurrentProjectRecordset->MoveNext(),pos++ )
	{

		if(1)    //��ʾ������.  ����������������ʱ����ʾ.
		{
			((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndStatusBar.GetProgressCtrl().SetRange(0, Count);
			((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndStatusBar.OnProgress(pos);
			
			if( gbIsStopCalc )	//�ж��Ƿ�ֹͣ����
			{
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndStatusBar.OnProgress(0);
				return false;
			}
		}

		//������Сʱ
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_hour"),_variant_t(e_hours));
		//�������
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_wind"),_variant_t(e_wind));
		

		txtC_vol  = vtos(m_ICurrentProjectRecordset->GetCollect(_variant_t("C_vol")));
		txtC_size = vtos(m_ICurrentProjectRecordset->GetCollect(_variant_t("C_size")));
		txtC_temp = vtos(m_ICurrentProjectRecordset->GetCollect(_variant_t("C_temp")));
		txtC_p_s  = vtos(m_ICurrentProjectRecordset->GetCollect(_variant_t("c_place")));

		strPass   = vtos(m_ICurrentProjectRecordset->GetCollect(_variant_t("c_pass")));
		if( strPass.CompareNoCase("Y") || txtC_vol.IsEmpty() || txtC_size.IsEmpty() || txtC_temp.IsEmpty() || txtC_p_s.IsEmpty() )
		{
			continue;
		}	
		try
		{	//�Զ�ѡ��
			AUTO(txtC_vol,txtC_size,txtC_temp,name_pre,name_pro,txtC_p_s,name_prein);
		}
		catch(_com_error &e)
		{
			ReportExceptionError(e);
			return false;
		}
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_name_in"),_variant_t(name_prein)); //�ڱ��²���
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_name2"),_variant_t(name_pre));		//�Ᵽ�²���
		m_ICurrentProjectRecordset->PutCollect(_variant_t("c_name3"),_variant_t(name_pro));		//��������

		m_ICurrentProjectRecordset->Update();
	}
	((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndStatusBar.OnProgress(0);
	return true;
}
///////////////////////////////////////////
//���»����¶ȵ�ȡֵ��.
//
bool CEditOriginalData::UpdateEnvironmentComBox(CString strFilter)
{
	CString strSQL;			//SQL���
	CString strTemp;
	CString strEnv;			//�����¶ȵ�ȡֵ
	int     nTmp;
	bool	bPlace=false;			//�Ƿ�Ͱ�װ�ص��й�.
	try
	{
		//���Ȼ�õ�ǰ��ʾ�Ļ����¶�ȡֵ
		m_ctlEnvTemp.GetWindowText(strEnv);

		m_ctlEnvTemp.ResetContent();	//�����Ͽ�
		if(m_pRsEnvironment->State == adStateOpen )
		{
			m_pRsEnvironment->Close();
		}
		//
		if( strFilter.IsEmpty() )
		{
			strSQL = "SELECT * FROM [�����¶�ȡֵ��] ORDER BY Index ";	//��CODE(�淶)����ȡ.
		}
		else
		{	
			//�й�������,�����ϵ�������.
			strSQL = "SELECT * FROM [�����¶�ȡֵ��] WHERE "+strFilter+" ORDER BY Index ";	//��CODE(�淶)����ȡ.
		}
		m_pRsEnvironment->Open(_variant_t(strSQL), this->GetPublicConnect().GetInterfacePtr(),
			adOpenStatic, adLockOptimistic, adCmdText);
		if( m_pRsEnvironment->GetRecordCount() <= 0 )
		{
			return true;
		}
		int nIndex;
		for(m_pRsEnvironment->MoveFirst(); !m_pRsEnvironment->adoEOF; m_pRsEnvironment->MoveNext())
		{
			strTemp = vtos( m_pRsEnvironment->GetCollect(_variant_t("Ta_Mode_Desc_Local")) );
			nIndex = m_ctlEnvTemp.AddString(strTemp);

			nTmp = vtoi( m_pRsEnvironment->GetCollect(_variant_t("Index")));			//������
			nIndex = 	m_ctlEnvTemp.SetItemData(nIndex, nTmp );
		}
		//���»ָ�ԭ����ѡ��.
		m_ctlEnvTemp.SelectString(-1,strEnv);
		
		//���´���ѡ��ͬ�ķ���ϵ��,���»����¶ȵ�ȡֵ.
		//���ǰ�װλ��ʱ,��������Ҫ��ͬ.
/*		if ( -1 == ( pos = m_ctlHeatTran.GetCurSel() ) )
		{
			return false;
		}
		

		//ԭ��ѡ���ȡֵ.
		strTemp = strEnv.Mid(0,4);
		if ( -1 != m_BuidInPlace.FindString(-1, strTemp) )
		{
			//�Ͱ�װ�ص����
			bPlace = true;
		}

		//ѡ��ķ���ϵ������
		m_ctlHeatTran.GetLBText(pos, strTemp);
		//ȡǰ�����ַ�,�Ƿ�Ϊ��װ�ص�
		strTemp = strTemp.Mid(0,4);				
		//�����п�ѡ�õĻ���ȡֵ�в��Ұ�װ�ص�.
		pos = m_ctlEnvTemp.FindString(-1,strTemp);
		if ( -1 != pos && bPlace )  //
		{
			//ԭ���Ѿ�ѡ���ȡֵ�в���
			nTmp = strEnv.Find(strTemp, 0);
			if ( -1 != nTmp )
			{
				//�ҵ�,ѡ��ԭ��������
				m_ctlEnvTemp.SelectString(-1,strEnv);
			}
			else
			{
				m_ctlEnvTemp.SetCurSel(pos);
			}
		}
		else
		{
			//��ԭ����ѡ���ڵ�ǰ��ѡ����б��в���.
			pos = m_ctlEnvTemp.SelectString(-1,strEnv);
			if ( -1 == pos)
			{
				m_ctlEnvTemp.SetCurSel(0);
			}
		}
*/
	}
	catch(_com_error& e)
	{
		ReportExceptionError(e);
		return false;
	}
	return true;
}

//
//���·���ϵ�������б�.
//
bool CEditOriginalData::UpdateHeatTransferCoef(CString strFilter)
{
	CString strSQL;			//SQL���
	CString strALPHA;		//����ϵ����ȡֵ
	CString	strTmp;
	CString strValName;
	short	nTmp;
	short	nIndex;			//�뻷���¶ȶ�Ӧ������

	bool	bPlace=false;

	try
	{
		//��õ�ǰ�ķ���ϵ����ȡֵ
		m_ctlHeatTran.GetWindowText(strALPHA);
		
		this->m_ctlHeatTran.ResetContent();		//�����б��е�����/
//		m_ctlHeatTran.SetDroppedWidth(200);
		if(m_pRsHeat->State == adStateOpen)
		{
			m_pRsHeat->Close();
		}
		if( strFilter.IsEmpty() )
		{
			strSQL = "SELECT * FROM [����ϵ��ȡֵ��] ORDER BY Index";
		}
		else
		{
			//�й�������,�뻷���¶������. 
			strSQL = "SELECT * FROM [����ϵ��ȡֵ��] WHERE "+strFilter+" ORDER BY Index";
		}
		
//		strSQL.Format("SELECT * FROM [����ϵ��ȡֵ��] %s ORDER BY Index",strFilter.IsEmpty()?"":"WHERE "+strFilter+" ");

		m_pRsHeat->Open(_variant_t(strSQL), theApp.m_pConnectionCODE.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if( m_pRsHeat->GetRecordCount() <= 0 )
		{
			return true;
		}
		//�г����ϲ��������ļ�¼.
		for(; !m_pRsHeat->adoEOF; m_pRsHeat->MoveNext())
		{
			strTmp = vtos(m_pRsHeat->GetCollect(_T("Ta_Mode_Desc_Local")));
			strValName = vtos(m_pRsHeat->GetCollect(_T("Variable_Name_Desc")));

			if( !strTmp.IsEmpty() && !strValName.CompareNoCase("ALPHA") )
			{
				nIndex = m_ctlHeatTran.AddString(strTmp);
				//����ϵ����Ӧ��������
				nTmp = vtoi(m_pRsHeat->GetCollect(_T("Index")));
				m_ctlHeatTran.SetItemData(nIndex, nTmp);
			}
		}

		//���»ָ�ԭ����ѡ��
		m_ctlHeatTran.SelectString(-1,strALPHA);

		//�����뻷���¶�ȡֵ�ļ�¼
/*		//���ǰ�װλ��ʱ,��������Ҫ��ͬ.
		if ( -1 == ( pos = m_ctlEnvTemp.GetCurSel() ) )
		{
			return false;
		}

		//ԭ��ѡ���ȡֵ.
		strTmp = strALPHA.Mid(0,4);
		if ( -1 != m_BuidInPlace.FindString(-1, strTmp) )
		{
			//�Ͱ�װ�ص����
			bPlace = true;
		}
		//ѡ��ķ���ϵ������
		m_ctlEnvTemp.GetLBText(pos, strTmp);
		//ȡǰ�����ַ�,�Ƿ�Ϊ��װ�ص�
		strTmp = strTmp.Mid(0,4);				
		//�����п�ѡ�õĻ���ȡֵ�в��Ұ�װ�ص�.
		pos = m_ctlHeatTran.FindString(-1,strTmp);
		if ( -1 != pos && bPlace )  //
		{
			//ԭ���Ѿ�ѡ���ȡֵ�в���
			nTmp = strALPHA.Find(strTmp, 0);
			if ( -1 != nTmp )
			{
				//�ҵ�,ѡ��ԭ��������
				m_ctlHeatTran.SelectString(-1,strALPHA);
			}
			else
			{
				m_ctlHeatTran.SetCurSel(pos);
			}
		}
		else
		{
			//��ԭ����ѡ���ڵ�ǰ��ѡ����б��в���.
			pos = m_ctlHeatTran.SelectString(-1,strALPHA);
			if ( -1 == pos)
			{
				m_ctlHeatTran.SetCurSel(0);
			}
		}
*/

	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;

}

/////////////////////////////////////////////////////
//����:
//���ݻ����¶�ȡֵ������,��[Ta_Variable]�����ҳ���Ӧ��ֵ.
int CEditOriginalData::GetConditionTemp(double &dConTemp, int & nMethodIndex, int nIndex)
{
	CString strTmp;			//��ʱ����
	CString	strSQL;			//SQL����.

	_RecordsetPtr	pRs;		//���ݱ�ļ�¼��.
	_variant_t		var;
	pRs.CreateInstance(__uuidof(Recordset));

	try
	{
		//�����ɲ�����������.���´����ݿ���ȡ��������
		if (nIndex == -1)
		{
			//��ö�Ӧ�İ�װ�ص�,����������ȡֵ������
			nIndex = vtoi(m_ICurrentProjectRecordset->GetCollect(_variant_t("c_Env_Temp_Index") ));
		}
		if(m_pRsEnvironment==NULL || m_pRsEnvironment->State == adStateClosed || m_pRsEnvironment->GetRecordCount() <= 0  )
		{	
			return 0;
		}
		m_pRsEnvironment->MoveFirst();
		strSQL.Format("Index=%d",nIndex);
		m_pRsEnvironment->Find(_bstr_t(strSQL), NULL, adSearchForward);
		if( m_pRsEnvironment->adoEOF )
		{
			//û���ҵ�����ֵ����Ӧ�ļ�¼.
			return 0;
		}
		//���㷽��������ֵ.
		var	= m_pRsEnvironment->GetCollect(_T("InsulationCalcMethodIndex"));
		if( var.vt != VT_EMPTY && var.vt != VT_NULL )
		{
			nMethodIndex = vtoi(var);
		}
		//��Ӧ��������¶�ֵ.
		double dTmp;
		dTmp = vtof(m_pRsEnvironment->GetCollect(_T("SurfaceTemperature")));
		if ( DZero  < dTmp )
		{
			//ֻ�е�������¶ȴ���0ʱ�ŷ���.
			strTmp.Format("%.2f",dTmp);
			m_dTs = strtod(strTmp,NULL);
		}
		else
		{
			//û���ض���������¶ȡ�
		}
		//�����¶ȵ����� 
		nIndex = vtoi(m_pRsEnvironment->GetCollect(_T("TaModeIndex")));		//���û����������ֵ�ڣ�ȡ����ֵ����0
		strSQL.Format("SELECT * FROM [Ta_Variable] WHERE Index=%d",nIndex);
		pRs->Open(_variant_t(strSQL), GetPublicConnect().GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if( !pRs->adoEOF && !pRs->BOF )
		{
			pRs->MoveFirst();
			//��ø�����������Ӧ�Ļ����¶�ֵ.
 			dConTemp = vtof( pRs->GetCollect(_variant_t("Ta_Variable_VALUE")) );
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return 0;
	}
	return 1;
}

//���ܣ�
//���Ƽ��㷽����ѡ��.
void CEditOriginalData::OnCheckMethod() 
{
	this->UpdateControlsState();	
}
//���ƻ����¶ȵ��ֶ�����
void CEditOriginalData::OnCheckTa() 
{
	this->UpdateControlsState();		
}
//�����ڲ㱣�º�
void CEditOriginalData::OnCheckThick1() 
{
	CString	strMatName;
	short	bEnable;
	m_OutSizeMatName.GetWindowText(strMatName);
	if ( !strMatName.IsEmpty() )
	{
		//�����Ᵽ�²������ȵ�״̬���ڲ���ͬ.(�ֹ�������Զ�����)
		bEnable = ((CButton*)GetDlgItem(IDC_CHECK_THICK1))->GetCheck(); 
		((CButton*)GetDlgItem(IDC_CHECK_THICK2))->SetCheck( bEnable );
	}
	else
	{
		//�Ᵽ�²㲻���ֶ�����.
		((CButton*)GetDlgItem(IDC_CHECK_THICK2))->SetCheck( FALSE );
	}
	this->UpdateControlsState();

}
//������㱣�º�
void CEditOriginalData::OnCheckThick2() 
{
	CString	strMatName;
	short	bEnable;
	m_InsideMatName.GetWindowText(strMatName);
	if ( !strMatName.IsEmpty() )
	{
		//�����ڱ��²������ȵ�״̬�������ͬ.(�ֹ�������Զ�����)
		bEnable = ((CButton*)GetDlgItem(IDC_CHECK_THICK2))->GetCheck();
		((CButton*)GetDlgItem(IDC_CHECK_THICK1))->SetCheck( bEnable );
	}
	else
	{
		//�ڱ��²㲻���ֶ�����.
		((CButton*)GetDlgItem(IDC_CHECK_THICK1))->SetCheck( FALSE );
	}
	this->UpdateControlsState();	

}
//����������¶�
void CEditOriginalData::OnCheckTs() 
{
	//�ڹ�����ѡ����,һЩ��������ֵ���Զ��ı�֮��, ��Ҫ���иı�֮��Ĵ���.
	AutoChangeValue();
	//���¿ؼ�����ʾ״̬���Ƿ���á�
	this->UpdateControlsState();	
}

//
//��ʼ���㷽����ѡ��
short CEditOriginalData::InitCalcMethod()
{
	CString strSQL;
	_RecordsetPtr pRsMethod;
	pRsMethod.CreateInstance(__uuidof(Recordset));

	try
	{
		this->m_ctlCalcMethod.ResetContent();		//�����б��е�����/
		strSQL = "SELECT * FROM [���¼��㷽����] ORDER BY Index";
		pRsMethod->Open(_variant_t(strSQL), theApp.m_pConnectionCODE.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
		if( pRsMethod->GetRecordCount() <= 0 )
		{
			return true;
		}
		CString strTmp, strValName;
		int nIndex,nTmp;

		for(; !pRsMethod->adoEOF; pRsMethod->MoveNext())
		{
			strTmp = vtos(pRsMethod->GetCollect(_T("Ta_Mode_Desc_Local")));
			if( !strTmp.IsEmpty() )
			{
				nIndex = m_ctlCalcMethod.AddString(strTmp);
				//��Ӧ��������
				nTmp = vtoi(pRsMethod->GetCollect(_T("Index")));
				m_ctlCalcMethod.SetItemData(nIndex, nTmp);
			}
		}
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}

	return 1;
}

//
//ѡ�񻷾��¶ȵ�ȡֵ.
void CEditOriginalData::OnSelchangeEnvironTemp() 
{
	CString strTmp,			//��ʱ����
			strSQL;			//SQL����.

	int				nIndex;			//��ǰ��¼,�����¶�ȡֵ������
	int				nPos;
	int				nMethodIndex;	//���㷽����ȡֵ����
	_RecordsetPtr	pRs;			//���ݱ�ļ�¼��.
	_variant_t		var;
	short			SiteIndex;		//��װ�ص������
	pRs.CreateInstance(__uuidof(Recordset));

	try
	{
		UpdateData(TRUE);
		nPos = m_ctlEnvTemp.GetCurSel();
		if(nPos == -1)
		{
			return ;
		}
		
		//��ö�Ӧ�����¶ȵ�ȡֵ������
		nIndex = m_ctlEnvTemp.GetItemData( nPos );

		//�����¶�ȡֵ���û�ѡ�������ȷ��.
		//�����¶ȵ�����(nIndex) , ȡ���¶�ֵ
		if( GetConditionTemp(m_dTa, nMethodIndex,nIndex) )
		{
		}			
		
		if( m_pRsEnvironment->GetRecordCount() <= 0  )
		{	
			return ;
		}
		m_pRsEnvironment->MoveFirst();
		strSQL.Format("Index=%d",nIndex);

		m_pRsEnvironment->Find(_bstr_t(strSQL), NULL, adSearchForward);
		if( m_pRsEnvironment->adoEOF )
		{
			//û���һ����¶�ȡֵ����Ӧ�ļ�¼.
			return ;
		}
		//���㷽��������ֵ.
		var	= m_pRsEnvironment->GetCollect(_T("InsulationCalcMethodIndex"));
		if( var.vt != VT_EMPTY && var.vt != VT_NULL )
		{
			nMethodIndex = vtoi(var);
			m_ctlCalcMethod.SetCurSel(nMethodIndex);
			((CButton*)GetDlgItem(IDC_CHECK_METHOD))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_EDIT_METHOD))->EnableWindow(FALSE);
			//�����¶������ϵ���Ĺ���
			strSQL.Format("InsulationCalcMethodIndex=%d OR InsulationCalcMethodIndex IS NULL",nMethodIndex);
		}
		else
		{
			//m_ctlCalcMethod.SetCurSel(-1);

			((CButton*)GetDlgItem(IDC_CHECK_METHOD))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_EDIT_METHOD))->EnableWindow(TRUE);
			//�����¶������ϵ���Ĺ���
			strSQL.Format("InsulationCalcMethodIndex IS NULL");
		}
		//�����¶���Ϊ���ɸ�д��״̬
		((CButton*)GetDlgItem(IDC_CHECK_TA))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT_TA))->EnableWindow(FALSE);

		//�ı����ϵ��
		if( !((CButton*)GetDlgItem(IDC_CHECK_ALPHA))->GetCheck() )
		{
			//�Լ��㷽��Ϊ����,ȷ����Ӧ�ķ���ϵ��.
			this->UpdateHeatTransferCoef(strSQL);
		}

		//У�鰲װ�ص�{{
		nPos = m_ctlEnvTemp.GetCurSel();
		//��ö�Ӧ�����¶ȵ�ȡֵ������
		nIndex = m_ctlEnvTemp.GetItemData( nPos );
		if ( -1 != GetSiteIndex(m_pRsEnvironment,nIndex,SiteIndex) )
		{
			//
			UpdateSite(SiteIndex);
		}
		//У�鰲װ�ص�}}

		ShowWindowRect();
		
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());
		return ;
	}
	UpdateData(FALSE);
}

//
//ѡ�����ϵ��ȷ�����㷽��
void CEditOriginalData::OnSelchangeHeatTransferCoef() 
{
	CString strTmp;			//��ʱ����
	CString	strSQL;			//SQL����.

	int		nIndex;			//��ǰ��¼,����ϵ��ȡֵ������
	int		nPos;
	int		nMethodIndex;	//���㷽����ȡֵ����
	short	SiteIndex;		//��װ�ص�����		

	_variant_t		var;
	
	try
	{
		UpdateData();
		
		nPos = m_ctlHeatTran.GetCurSel();
		if(nPos == -1)
		{
			return ;
		}

		//��ö�Ӧ�İ�װ�ص�,����������ȡֵ������
		nIndex = m_ctlHeatTran.GetItemData( nPos );
			
		if( m_pRsHeat->GetRecordCount() <= 0  )
		{	
			return ;
		}

		m_pRsHeat->MoveFirst();
		strSQL.Format("Index=%d",nIndex);
		//
		m_pRsHeat->Find(_bstr_t(strSQL), NULL, adSearchForward);
		if( m_pRsHeat->adoEOF )
		{
			//û���ҵ�����ϵ��,����Ӧ�ļ�¼.
			return ;
		}
		//���㷽��������ֵ.
		var	= m_pRsHeat->GetCollect(_T("InsulationCalcMethodIndex"));
		if( var.vt != VT_EMPTY && var.vt != VT_NULL )
		{
			nMethodIndex = vtoi(var);
			m_ctlCalcMethod.SetCurSel(nMethodIndex);	
			((CButton*)GetDlgItem(IDC_CHECK_METHOD))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_EDIT_METHOD))->EnableWindow(FALSE);
			//���÷���ϵ���뻷���¶ȵĹ���.
			strSQL.Format("InsulationCalcMethodIndex=%d OR InsulationCalcMethodIndex IS NULL",nMethodIndex);
		}
		else
		{
			//m_ctlCalcMethod.SetCurSel(-1);

			((CButton*)GetDlgItem(IDC_CHECK_METHOD))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_EDIT_METHOD))->EnableWindow(TRUE);

			//���÷���ϵ���뻷���¶ȵĹ���.
			strSQL.Format("InsulationCalcMethodIndex IS NULL");			
		}

		//���û����¶�ȡֵ�Ĺ���
		//
		if( !((CButton*)GetDlgItem(IDC_CHECK_ENV_TEMP))->GetCheck() )
		{
			this->UpdateEnvironmentComBox( strSQL );
		}

		//У�鰲װ�ص�{{		
		nPos = m_ctlHeatTran.GetCurSel();			
		//��ö�Ӧ�����¶ȵ�ȡֵ������
		nIndex = m_ctlHeatTran.GetItemData( nPos );
		if ( -1 != GetSiteIndex(m_pRsHeat,nIndex,SiteIndex) )
		{
			//
			UpdateSite(SiteIndex);
		}
		//У�鰲װ�ص�	}}

		ShowWindowRect();

	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());		
		return ;
	}
	UpdateData(FALSE);
}

//�г����еĻ����¶�ȡֵ
void CEditOriginalData::OnCheckEnvTemp() 
{
	if( ((CButton*)GetDlgItem(IDC_CHECK_ENV_TEMP))->GetCheck() )
	{
		this->UpdateEnvironmentComBox();
	}
}

//�г����еķ���ϵ����ȡֵ
void CEditOriginalData::OnCheckAlpha() 
{
	if( ((CButton*)GetDlgItem(IDC_CHECK_ALPHA))->GetCheck() )
	{
		this->UpdateHeatTransferCoef();
	}
}

//ѡ��ͬ�ļ��㷽��
void CEditOriginalData::OnSelchangeEditMethod() 
{
	UpdateData(TRUE);
	
	int nPos = m_ctlCalcMethod.GetCurSel();		//ѡ���λ��
	CString STR;								//��������
	long	nMethodIndex;						//����������
	STR.Format("%d",m_ctlCalcMethod.GetItemData(nPos));
	nMethodIndex = m_ctlCalcMethod.GetItemData(nPos);
	if ( nSurfaceTemperatureMethod == nMethodIndex )
	{
		//������¶���Ϊ���Ը�д.
		if( !((CButton*)GetDlgItem(IDC_CHECK_TS))->GetCheck() )
		{
			//
			((CButton*)GetDlgItem(IDC_CHECK_TS))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_EDIT_TS))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_EDIT_TS))->SetFocus();
		}
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_TS))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT_TS))->EnableWindow(FALSE);
	}
	//����������·���Ҫ��������һЩ����.
	ShowWindowRect();
	//
	UpdateData(FALSE);
	
}

//��ʼ���ڽ��� 
short CEditOriginalData::InitPipeMedium()
{
	_RecordsetPtr pRsMedium;	//���ڽ���
	CString		  strSQL;		//SQL���
	CString		  strMedium;	//��������
	short		  nIndex;		//���ʶ�Ӧ�����
	short		  pos;			//�ؼ���λ��

	try
	{
		
		pRsMedium.CreateInstance(__uuidof(Recordset));

		//
		strSQL.Format("SELECT * FROM [MediumAlias] ORDER BY MediumID");
		pRsMedium->Open(_variant_t(strSQL), theApp.m_pConMedium.GetInterfacePtr(),
						adOpenStatic, adLockOptimistic, adCmdText);
		

		//��ս����б�
		m_ctlPipeMedium.ResetContent();
		//
		for (; !pRsMedium->adoEOF; pRsMedium->MoveNext())
		{
			strMedium = vtos(pRsMedium->GetCollect(_variant_t("MediumAlias")));
			if ( !strMedium.IsEmpty() )
			{
				//���ʱ�����Ӧ���ʵ�����
				nIndex = vtoi(pRsMedium->GetCollect(_variant_t("MediumID")));
				//
				pos = m_ctlPipeMedium.AddString(strMedium);
				//���ù��ڽ��ʱ���������
				m_ctlPipeMedium.SetItemData(pos, nIndex);
			}
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return 0;
	}
	
	return 1;
}

//����:
//�������¶ȡ�����ϵ���Ͱ�װ�ص�����һ��,���ܿ���
short CEditOriginalData::UpdateSite(short SiteIndex)
{
	CString		strTmp;		//��ʱ����
	CString		strSQL;		//SQL���
	short		pos;		//
	short		nIndex;		//��ǰѡ�������ֵ
	short		nCurrSiteIndex;		//��ǰ�İ�װ�ص������
	bool		bEmpty=false;		//
	short		bReturnFlg;			//���ú�ʱ�ķ���״̬
	//��鰲װ�ص�ѡ��.
	nIndex = m_BuidInPlace.GetCurSel();
	if ( -1 == SiteIndex )
	{	//û�а�װ�ص�Ŀ���
		if ( -1 == nIndex )
		{
			m_BuidInPlace.SetCurSel(0);
		}
		SiteIndex = m_BuidInPlace.GetCurSel();
	}
	else
	{		
		if ( -1 != nIndex && SiteIndex != nIndex )
		{
			//����Ϊ����������������.
			if( -1 == m_BuidInPlace.SetCurSel(SiteIndex) )
				m_BuidInPlace.SetCurSel(nIndex);			//��ԭ
		}
	}

	//�����¶ȵ�ѡ��
	if ( m_ctlEnvTemp.GetCount() > 0 )
	{
		pos = m_ctlEnvTemp.GetCurSel();
		if ( -1 == pos )
		{
			//û�е�ǰ��,Ĭ��Ϊ��һ��
			pos = 0;
			bEmpty = true;
		}
		for (; pos < m_ctlEnvTemp.GetCount(); pos++ )
		{
			nIndex = (short)m_ctlEnvTemp.GetItemData(pos);
			//��õ�ǰ�����¶�ȡֵ��Ӧ�İ�װ�ص������
			bReturnFlg = GetSiteIndex(m_pRsEnvironment,nIndex,nCurrSiteIndex);
			if ( !bEmpty && 0 == bReturnFlg )
			{	
				//��ǰ��ѡ��û�а�װ�ص�Ŀ���ʱ
				//
				m_ctlEnvTemp.SetCurSel(pos);
				break;
			}
			else
			{
				if ( 1 == bReturnFlg && SiteIndex == nCurrSiteIndex )
				{
					//���õ�ǰ�жϵ�ѡ��
					m_ctlEnvTemp.SetCurSel(pos);
					break;
				}
			}
			//�е�ǰֵʱ,�ٴӵ�һ����¼�Ƚ�
			if( !bEmpty )
			{
				pos = -1;
				bEmpty = true;
			}
		}
		if (pos == m_ctlEnvTemp.GetCount())
		{
			//��û�����ư�װ�ص�ʱ,��Ϊ��һ����¼
			m_ctlEnvTemp.SetCurSel(0);
		}
		
	}


	bEmpty = false;
	//����ϵ����ѡ��
	if ( m_ctlHeatTran.GetCount() > 0 )
	{
		pos = m_ctlHeatTran.GetCurSel();
		if ( -1 == pos )
		{
			//û�е�ǰ��,Ĭ��Ϊ��һ��.
			pos = 0;
			bEmpty = true;
		}
		for (; pos < m_ctlHeatTran.GetCount(); pos++ )
		{
			nIndex = (short)m_ctlHeatTran.GetItemData(pos);
			//��õ�ǰ�����¶�ȡֵ��Ӧ�İ�װ�ص������.
			bReturnFlg = GetSiteIndex(m_pRsHeat,nIndex,nCurrSiteIndex);
			if ( (!bEmpty && 0 == bReturnFlg) || (1 == bReturnFlg && SiteIndex == nCurrSiteIndex) )
			{
				//����Ϊ��ǰ�жϵ�ѡ��
				m_ctlHeatTran.SetCurSel(pos);		
				break;
			}

			//�е�ǰֵʱ,�ٴӵ�һ����¼�Ƚ�
			if( !bEmpty )
			{
				pos = -1;
				bEmpty = true;
			}
		}
		if ( pos == m_ctlHeatTran.GetCount() )
		{
			//��û�����ư�װ�ص�ʱ,��Ϊ��һ����¼		
			m_ctlHeatTran.SetCurSel(0);
		}
	}
	//�ڹ�����ѡ����,һЩ��������ֵ���Զ��ı�֮��, ��Ҫ���иı�֮��Ĵ���.
	AutoChangeValue();
    
	return 1;
}

//����:
//��ð�װ�ص������.
//����ֵ,   -1			�����û���ҵ���Ӧ������
//			0			û���ر�ָ����װ�ص�
//			1			�ҵ�,�ɹ�����
short CEditOriginalData::GetSiteIndex(_RecordsetPtr &pRs, short nIndex, short& SiteIndex)
{
	try
	{
		SiteIndex = -1;
		if (pRs == NULL || pRs->GetRecordCount()<=0)
		{
			return -1;
		}
		
		CString			strSQL;
		_variant_t		var;
		pRs->Filter = (long)adFilterNone;
		strSQL.Format("Index=%d",nIndex);
		//��λ�����������ļ�¼.
		pRs->Filter = _variant_t(strSQL);
		if ( !pRs->adoEOF )
		{
			//��ð�װ�ص������.
			var = pRs->GetCollect(_variant_t("SiteIndex"));
			SiteIndex = vtoi( var );
			pRs->Filter = (long)adFilterNone;
			if(var.vt == VT_NULL || var.vt == VT_EMPTY)
			{
				SiteIndex = -1;
				return 0;
			}
			else
			{
				return 1;
			}
		}
		pRs->Filter = (long)adFilterNone;
	}
	catch (_com_error)
	{
		return -1;
	}

	return -1;
}

//
//ѡ��װ�ص�
void CEditOriginalData::OnSelchangeBuildinPlace() 
{
	UpdateData();
	short pos;
	pos = m_BuidInPlace.GetCurSel();
	if (-1 != pos)
	{
		UpdateSite(pos);
	}
	UpdateData(FALSE);
}


//--------------------------------------------------------------------------
//DATE:			[2005/5/20]
//
//����:			ѡ����ڽ���				
//--------------------------------------------------------------------------
void CEditOriginalData::OnSelchangePipeMedium() 
{
	UpdateMediumMaterialDensity();
}

//--------------------------------------------------------------------------
// DATE         :[2005/5/23]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :����������ݵ���ȷ��,�Ϸ���.
//--------------------------------------------------------------------------
bool CEditOriginalData::CheckDataValidity()
{
	int		nMethodIndex=-1;		//���㷽��������
	CString strTmp;
	double  t = m_MediumTemperature;	// �����¶�
	try
	{
		//������㷽��Ϊ������¶ȷ�ʱ��������¶�һ��Ҫ��ֵ
		nMethodIndex = vtoi( m_ICurrentProjectRecordset->GetCollect(_variant_t("c_CalcMethod_Index")) );
		if( nSurfaceTemperatureMethod == nMethodIndex )
		{
			if( DZero >= fabs(m_dTs) )
			{
				AfxMessageBox("��ʾ��\n\n���㷽��Ϊ������¶ȷ�ʱ��������¶Ȳ���Ϊ0��");
				((CButton*)GetDlgItem(IDC_CHECK_TS))->SetCheck(TRUE);
				((CButton*)GetDlgItem(IDC_EDIT_TS))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_EDIT_TS))->SetFocus();
				return false;
			}
			else if ( !(m_dTa > t && m_dTs <= m_dTa && m_dTs >= t) ) // ����
					if ( !(m_dTa < t && m_dTs >= m_dTa && m_dTs <= t) )	// ����
						if ( !(m_dTa == t && m_dTs == t) )		// �����¶ȶ����
			{				
				AfxMessageBox("�����¶ȣ�����¶Ȼ򻷾��¶������д�");
				return false;
			}
		}
		
		//���ѡ���ֶ����뱣�º�,���º�Ҫ����0
		if ( 1 == m_bIsCalInThick || 1 == m_bIsCalPreThick )
		{ 
			m_InsideMatName.GetWindowText(strTmp);		//�ڲ㱣��
			if ( !strTmp.IsEmpty() )
			{
				if( DZero >= fabs(m_dThick1))
				{
					AfxMessageBox("��ʾ��\n\n�ֹ����뱣�²���ʱ���ڱ��²��ȵ�ֵ������ڣ� ");
					return false;
				}
			}
			m_OutSizeMatName.GetWindowText(strTmp);		//��㱣��
			if ( !strTmp.IsEmpty() )				
			{
				if( DZero >= fabs(m_dThick2))
				{
					AfxMessageBox("��ʾ��\n\n�ֹ����뱣�²���ʱ���Ᵽ�²��ȵ�ֵ������ڣ� ");
					return false;
				}
			}
		}
		CString	strInMat;		//�ڱ��²����
		CString	strOutMat;		//�Ᵽ�²����
		CString strProMat;		//���������
		m_InsideMatName.GetWindowText(strInMat);
		m_OutSizeMatName.GetWindowText(strOutMat);
		if(!strInMat.IsEmpty() && strOutMat.IsEmpty())
		{
			AfxMessageBox("��ʾ��\n\n��ѡ���Ᵽ�²����!");
			return false;
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return true;
}


//--------------------------------------------------------------------------
// DATE         :[2005/5/24]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�ڹ�����ѡ����,һЩ��������ֵ���Զ��ı�֮��, ��Ҫ���иı�֮��Ĵ���.
//--------------------------------------------------------------------------
short CEditOriginalData::AutoChangeValue()
{
	CString	strTemp;			//��ʱ�ַ���
	int		nMethodIndex;		//���㷽��������ֵ
	int		nIndex;
	short	pos;				//λ��
	bool	bEnable = FALSE;			//�Ƿ񼤻

	try
	{	//��Ӧ�Ļ����¶ȵ�ѡ��.
		m_ctlEnvTemp.GetWindowText(strTemp);
		if ( !strTemp.IsEmpty() )
		{
			pos = m_ctlEnvTemp.FindString(-1, strTemp);
			if ( -1 != pos )
			{
				//���õ�ǰ�����¶ȵ�ȡֵ���Ͷ�Ӧ��������¶�ֵ
				nIndex = m_ctlEnvTemp.GetItemData(pos);
				GetConditionTemp(m_dTa,nMethodIndex,nIndex);
			}
		}
		//���㷽����ѡ��,��������¶ȹ�����
		m_ctlCalcMethod.GetWindowText(strTemp);
		if ( !strTemp.IsEmpty() )
		{
			pos = m_ctlCalcMethod.FindString(-1, strTemp);
			if ( -1 != pos )
			{
				nIndex = m_ctlCalcMethod.GetItemData(pos);
				if ( nIndex == nSurfaceTemperatureMethod )
				{
					//������¶ȼ��㷽��ʱ.��Ҫ����������¶ȵ�ֵ��
					bEnable = TRUE;
				}
				else
				{
					bEnable = FALSE;
				}
			}
		}
		((CButton*)GetDlgItem(IDC_CHECK_TS))->SetCheck(bEnable);
		((CButton*)GetDlgItem(IDC_EDIT_TS))->EnableWindow(bEnable);
		//
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return 0;
	}
	return	1;
}

//------------------------------------------------------------------                
// DATE         :[2005/05/31]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :���õ�ǰ�Ի���Ĵ�С
//------------------------------------------------------------------
short CEditOriginalData::ShowWindowRect()
{
	//����������·���Ҫ��������һЩ����.
	CRect	rCongeal;		//��ʾ�����������ĸ������Ĵ��ڴ�С
	CRect	rWindow;		//��ǰ�Ի��򴰿ڵĴ�С
	int		nWidth = 10;	//�ұ߶�ӵĿհ״��Ŀ��
	bool	bMinMove = false;	//�Ƿ�ı����ڵĴ��ڵĴ�С
	long	nMethodIndex = -1;	//���㷽��������
	short	pos;

	pos = m_ctlCalcMethod.GetCurSel();
	if ( -1 != pos )
	{
		nMethodIndex = m_ctlCalcMethod.GetItemData(pos);
	}
	try
	{
		if ( nMethodIndex != nSubterraneanMethod )
		{
			UpdateControlSubterranean( -1 );
		}
		// ��Ҫ�����µĲ����ķ���
		switch( nMethodIndex )
		{
		case nPreventCongealMethod:	// �������¼���
			if ( NULL == m_pDlgCongeal )
			{
				m_pDlgCongeal = new CDlgParameterCongeal();
			}
			AddDynamicDlg( m_pDlgCongeal, IDD_PARAMETER_CONGEAL );
			m_pDlgCongeal->SetProjectID( GetProjectID() );
			m_pDlgCongeal->SetCurrentRes( m_pRsCongealData );
			m_pDlgCongeal->GetDataToPreventCongealControl( m_ID ); // ��÷������µĲ���
			break;
			
		case nSubterraneanMethod: // ��عܵ��ļ��㷽��
			if ( NULL == m_pDlgSubterranean )
			{
				m_pDlgSubterranean = new CDlgParameterSubterranean();
			}
			AddDynamicDlg( m_pDlgSubterranean, IDD_PARAMETER_SUBTERRANEAN );
			m_pDlgSubterranean->SetProjectID( GetProjectID() );
			m_pDlgSubterranean->SetCurrentRes( m_pRsSubterranean );
			m_pDlgSubterranean->GetDataToSubterraneanControl( m_ID ); // �����عܵ����¼���Ĳ���
			break;
			
		default:
			bMinMove = true;	// ����Ҫ��ʾ���ӵĴ���
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox( e.Description() );
		// 
		return 0 ;
	}
	// �ƶ�����
	this->GetWindowRect(&rWindow);
	m_ctlTitleTab.GetClientRect(rCongeal);
	
	if ( bMinMove )	// ֻ��ʾ��׼�����봰�� 
	{
		if ( m_ctlTitleTab.IsWindowEnabled() )	// ���ǵ�ǰ��ʾ�����ӵĴ���
		{
			rWindow.right -= rCongeal.right + nWidth;
			//��Ҫ�ı����ڵĴ��ڵĴ�С
			MoveWindow( &rWindow );
			m_ctlTitleTab.EnableWindow( FALSE );
		}
	}
	else if ( !m_ctlTitleTab.IsWindowEnabled() ) // ��ʾ���ӵĴ���
	{
		rWindow.right += rCongeal.right + nWidth;
		MoveWindow( &rWindow );
		m_ctlTitleTab.EnableWindow( TRUE );
	}

/*	if ( nPreventCongealMethod == nMethodIndex || nSubterraneanMethod == nMethodIndex )	//�������·�
	{
		rWindow.right += rCongeal.right+nWidth;
		if ( !m_ctlTitleTab.IsWindowEnabled())
		{
			bMove = true;
			//��ʾ������������·��ĺͲ���
			m_ctlTitleTab.EnableWindow(TRUE);
		}
	}
	else
	{
		rWindow.right -= rCongeal.right+nWidth;
		if ( m_ctlTitleTab.IsWindowEnabled())
		{
			bMove = true;
			//����
			m_ctlTitleTab.EnableWindow(FALSE);
		}
	}
	if ( bMove )
	{
		//��Ҫ�ı����ڵĴ��ڵĴ�С
		this->MoveWindow(&rWindow);
	}

	// �����ݿ��е�����д�뵽������Ĳ�����
	GetDataToPreventCongealControl();
*/
	return	1;
}


//------------------------------------------------------------------                
// DATE         :[2005/05/31]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�����ݿ���ȡ�����ݵ���������������.
//------------------------------------------------------------------
short CEditOriginalData::GetDataToPreventCongealControl()
{
//	double		dTmp;
	CString		strTmp;		//��ʱ�ַ�
	CString		strSQL;		//SQL���
	
	try
	{
		//������Ƿ������·�����ʱ������ȡ����Ӧ�����ݡ�
		if ( !m_ctlTitleTab.IsWindowEnabled() )
		{
			return 0;
		}
		//
		if ( m_pRsCongealData == NULL || m_pRsCongealData->State != adStateOpen )
		{
			return 0;
		}

/*
		if (m_pRsCongealData->GetRecordCount() > 0 )
		{
			m_pRsCongealData->MoveFirst();
			strSQL.Format("ID=%d",m_ID);
			m_pRsCongealData->Find(_bstr_t(strSQL), NULL, adSearchForward);
		}
		if ( m_pRsCongealData->adoEOF || m_pRsCongealData->BOF )
		{
			//û�е�ǰ���ʱ������һ���¼�¼.
			m_pRsCongealData->AddNew();

			//���
			m_pRsCongealData->PutCollect(_T("ID"), _variant_t((long)m_ID));

			//���̴���
			m_pRsCongealData->PutCollect(_T("EnginID"), _variant_t(GetProjectID()));
			
			m_pRsCongealData->Update(vtMissing);
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

		//�ܵ�ͨ��֧���ܴ�����(����)��ʧ�ĸ���ϵ��
		dTmp			= vtof( m_pRsCongealData->GetCollect(_T("c_Heat_Loss_Data")) );	
		strTmp.Format("%.2f",dTmp);
		m_ctlHeatLossData.SetWindowText(strTmp);
		
		//	= vtof( m_pRsCongealData->GetCollect(_T("EnginID")) );
*/
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return 0;
	}
	
	return	1;
}


//------------------------------------------------------------------                
// DATE         :[2005/06/01]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :���������·���Ҫ���������뵽���ݿ��С�
//------------------------------------------------------------------
short CEditOriginalData::PutDataToPreventCongealDB(long nID)
{
	try
	{
		//������Ƿ������·�����ʱ������ȡ����Ӧ�����ݡ�
		if ( !m_ctlTitleTab.IsWindowEnabled() )
		{
			return 0 ;
		}
		//test zsy 2006
		if ( m_pDlgCongeal != NULL )
		{
			m_pDlgCongeal->PutDataToPreventCongealDB(nID);
			return 1;
		}
		//test zsy 2006

		if ( m_pRsCongealData == NULL || m_pRsCongealData->State != adStateOpen )
		{
			return 0;
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(e.Description());
		return 0;
	}
/*
		CString strTmp;
		double	dTmp;

		if (m_pRsCongealData->adoEOF || m_pRsCongealData->BOF)
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
	//	m_pRsCongealData->PutCollect(_T("EnginID"), _variant_t(GetProjectID()));
		 
		m_pRsCongealData->Update(vtMissing);

	}
	catch (_com_error& e) 
	{
		AfxMessageBox(e.Description());
		return 0;
	}
*/
	return 1;
}


//------------------------------------------------------------------                
// DATE         :[2005/06/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :��һ��ֱ�ܵ������ӷ���.
//------------------------------------------------------------------
BOOL CEditOriginalData::AddValveData()
{
	short ID = 1;			//���
	CString strSQL;
 	try
	{
		if (m_ICurrentProjectRecordset->adoEOF && m_ICurrentProjectRecordset->BOF)
		{
			ID = 0;		//ԭʼ��¼Ϊ��
		}
		else
		{
			ID = vtoi(m_ICurrentProjectRecordset->GetCollect(_variant_t("ID")));
		}

		//�ڵ�ǰ��λ������һ���µļ�¼
		this->InsertNew(m_ICurrentProjectRecordset, 1);
		m_ICurrentProjectRecordset->PutCollect(_variant_t("ID"),_variant_t((long)++ID));
		m_ICurrentProjectRecordset->PutCollect(_variant_t("EnginID"),_variant_t(m_ProjectID));

		if (NULL != m_pRsCongealData && (!m_pRsCongealData->GetadoEOF() || !m_pRsCongealData->GetBOF()))
		{
			//�ı����
			strSQL.Format("UPDATE [PRE_CALC_CONGEAL] SET ID=ID+1 WHERE ID>=%d AND EnginID='%s' ", ID, m_ProjectID);
			m_ICurrentProjectConnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
		}
		if (NULL != m_pRsSubterranean && (!m_pRsSubterranean->GetadoEOF() || !m_pRsSubterranean->GetBOF()))
		{
			strSQL.Format("UPDATE [PRE_CALC_SUBTERRANEAN] SET ID=ID+1 WHERE ID>=%d AND EnginID='%s' ", ID, m_ProjectID);
			m_ICurrentProjectConnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
		}
		
		//����ǰ�����ݱ��浽�����ӵļ�¼��
		this->PutDataToDatabaseFromControl(ID);
		this->PutDataToEveryControl();
		
		//���±�Ų���λ��ָ���ļ�¼
		RenewNumberFind(m_ICurrentProjectRecordset, ID, ID);

		//���¿ؼ���״̬
		UpdateControlsState();
	}
	catch (_com_error)
	{
		return FALSE;
	}

	return TRUE;
}


//------------------------------------------------------------------                
// DATE         :[2005/06/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :��Ӧ����һ�����Ű�ť����Ϣ.
//------------------------------------------------------------------
void CEditOriginalData::OnButtonAddValve() 
{
	UpdateData();
	
	CString	strTmp;
	long	pipeID;			//�ܵ������
	long	valveID;		//���ŵ����
	pipeID = m_ID;

	//�����⾶�ͱں����ж��Ƿ�Ϊ�ܵ���ƽ��.
	m_PipeSize.GetWindowText(strTmp);
	if (strtod(strTmp, NULL) >= m_dMaxD0)
	{
		m_bIsPlane = TRUE;
	}
	else
	{
		m_bIsPlane = FALSE;
	}
	//�ж��Ƿ��ȱ�������.
	if(m_IsUpdateByRoll)
	{
		PutDataToDatabaseFromControl();
		if( !Refresh() )
			return;
	}

/*
	//test  [2005/06/28]
	UpdateData();
	if (!m_bIsPlane && m_Amount > m_dValveSpace)
	{
		//Ϊ�ܵ�,ͬʱ�ܵ����ȴ���10mʱ.�Զ�����һ������
		AddValveData();
	}
*/
	//���ܵ��뷧�ŵĹ�ϵӳ�䵽һ������
	try
	{
		//�ܵ�ʱ
		if (!m_bIsPlane)
		{
			//Ϊ�ܵ�,����һ������
			strTmp.Format("%f",m_dMaxD0);
			m_PipeSize.SetWindowText(strTmp);		//����Ϊƽ�ڡ�
			m_PipeThick.SetWindowText("0");
			//����
			if ( !m_VolNo.IsEmpty() )
			{
				m_VolNo.TrimRight();
				m_VolNo.TrimLeft();
				while (m_VolNo.GetLength() <= 5)
				{
					m_VolNo+=" ";					
				}
				m_VolNo.SetAt(5,'V');
	 			m_VolNo = m_VolNo.Mid(0,6);
			}
			//�豸��
			m_PipeDeviceName.GetWindowText(strTmp);
			if (strTmp.IsEmpty() || -1 == strTmp.Find("����"))
			{
				strTmp += "����";
				m_PipeDeviceName.SetWindowText(strTmp); 				 
			}
			UpdateData(FALSE);
			
			AddValveData();
		}
		else
		{
			//ƽ���ϲ������ӷ���
			AfxMessageBox("ƽ���ϲ������ӷ���");
			return;
		}

		_RecordsetPtr pRs;			//�ܵ��뷧�ŵĹ�ϵ��
		CString	strSQL;				//SQL���
										   												
		pRs.CreateInstance(__uuidof(Recordset));
		//

 		strSQL.Format("SELECT * FROM [pipe_valve] WHERE PipeID=%d AND EnginID='%s' ORDER BY ValveID",
						pipeID, m_ProjectID);
		

		pRs->Open(_variant_t(strSQL), theApp.m_pConnection.GetInterfacePtr(),
				adOpenStatic, adLockOptimistic, adCmdText);
		if (pRs->adoEOF && pRs->BOF && pRs->GetRecordCount() <= 0 )
		{
			//�ڹܵ������һ�����ż�¼��
			valveID = vtoi(  m_ICurrentProjectRecordset->GetCollect(_variant_t("ID")) );
		}
		else
		{
			//�ڡ��ܵ������š�ӳ�����ȡ��ǰ�ܵ���Ӧ�ķ��ŵ������š�
			pRs->MoveLast();
			valveID = vtoi(pRs->GetCollect(_variant_t("valveID")));
			valveID++;
		}

		//���ӹܵ�-���ŵ�ӳ��
		pRs->AddNew();
		pRs->PutCollect(_variant_t("PipeID"),_variant_t(pipeID));
		pRs->PutCollect(_variant_t("ValveID"),_variant_t(valveID));
		pRs->PutCollect(_variant_t("EnginID"),_variant_t(m_ProjectID));
		
		pRs->Update();
   
		//�ı����
		strSQL.Format("UPDATE [PIPE_VALVE] SET PipeID=PipeID+1 WHERE PipeID>%d AND EnginID='%s' ",
						pipeID, m_ProjectID);
		m_ICurrentProjectConnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
		//
		strSQL.Format("UPDATE [PIPE_VALVE] SET VALVEID=VALVEID+1 WHERE VALVEID>%d AND EnginID='%s' ",
						valveID, m_ProjectID);
		m_ICurrentProjectConnection->Execute(_bstr_t(strSQL), NULL, adCmdText);
		
		//���¿ؼ���״̬
		UpdateControlsState();

		UpdateData(FALSE);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return;
	}
}


//------------------------------------------------------------------                
// DATE         :[2005/06/22]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :��ָ���ļ�¼�����±��,����λ����Ӧ��λ��.
//------------------------------------------------------------------
BOOL CEditOriginalData::RenewNumberFind(_RecordsetPtr &pRs, long ID, long FindNO)
{
	try
	{
		if (pRs == NULL || pRs->adoEOF && pRs->BOF)
		{
			return FALSE;
		}
		//���±��
		for (; !pRs->adoEOF; pRs->MoveNext(), ID++)
		{
			pRs->PutCollect(_variant_t("ID"),ID);
		}
		//��λ��ָ���ļ�¼
		for(pRs->MoveFirst(); FindNO > 1 && !pRs->adoEOF; pRs->MoveNext(),FindNO--)
		{
			;
		}
		if (pRs->adoEOF)
		{
			pRs->MovePrevious();
		}
		return TRUE;
	}
	catch (_com_error)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CEditOriginalData::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pctlTip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------                
// DATE         :[2005/06/23]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :������Ҫ��ʾ��Ϣ�ı༭����֮��Ӧ����ʾ����
//------------------------------------------------------------------
BOOL CEditOriginalData::InitToolTip()
{
	int	pos = 0;		//�����±�
	Txtup	TipArr[20];
	
	TipArr[pos].dID = IDC_PIPE_SIZE;	//�ܵ��⾶/���
	TipArr[pos++].txt = _T("mm");		//��λ

	TipArr[pos].dID = IDC_PIPE_THICK;	//�ܵ��ں�
	TipArr[pos++].txt = _T("mm");		//��λ

	TipArr[pos].dID = IDC_PIPE_PRESSURE;	//����ѹ��
	TipArr[pos++].txt = _T("MPa");			//��λ
	
	TipArr[pos].dID = IDC_MEDIUM_TEMPERATURE;	//�����¶ȡ�
	TipArr[pos++].txt = _T("��");				//��λ

	TipArr[pos].dID = IDC_EDIT_TS;				//�����¶ȡ�
	TipArr[pos++].txt = _T("��");				//��λ
	
	TipArr[pos].dID = IDC_EDIT_TA;				//�����¶ȡ�
	TipArr[pos++].txt = _T("��");				//��λ

	TipArr[pos].dID = IDC_AMOUNT;				//�ܵ����Ȼ�ƽ�����
	TipArr[pos++].txt = _T("m �� �O");			//��λ

	TipArr[pos].dID = IDC_EDIT_THICK2;			//�Ᵽ�º�.
	TipArr[pos++].txt = _T("mm");				//��λ

	TipArr[pos].dID = IDC_EDIT_THICK1;			//�ڱ��º�.
	TipArr[pos++].txt = _T("mm");				//��λ

	TipArr[pos].dID = IDC_WIDE_SPEED;			//���٣�m/s��
	TipArr[pos++].txt = _T("m/s");				//��λ

	TipArr[pos].dID = IDC_EDIT_HEAT_DENSITY;	//�����ܶ�
	TipArr[pos++].txt = _T("W/�O");				//��λ
	
	TipArr[pos].dID = IDC_EDIT_HEAT_LOSS;		//ɢ����
	TipArr[pos++].txt = _T("W");				//��λ

	TipArr[pos].dID = IDC_EDIT_PLAIM_THICK;		//�ط��ٷ����ƽ�ڿ��
	TipArr[pos++].txt = _T("m");

	//////////////////////////////////////////////////////////////////////////
/*
	// ��������������ӵĲ���
	TipArr[pos].dID = IDC_EDIT_MEDIUM_DENSITY1;	//�����ܶ�
	TipArr[pos++].txt = _T("Kg/m3");
	
	TipArr[pos].dID = IDC_EDIT_PIPE_DENSITY;	//�ܲ��ܶ�
	TipArr[pos++].txt = _T("Kg/m3");
	
	TipArr[pos].dID = IDC_EDIT_MEDIUM_HEAT;		//���ʱ���
	TipArr[pos++].txt = _T("J/(Kg/��)");

	TipArr[pos].dID = IDC_EDIT_PIPE_HEAT;		//�ܵ����ϵı���
	TipArr[pos++].txt = _T("J/(Kg/��)");
	
	TipArr[pos].dID = IDC_EDIT_MEDIUM_MELT_HEAT;//�����ܽ���
	TipArr[pos++].txt = _T("J/Kg");

	TipArr[pos].dID = IDC_EDIT_MEDIUM_CONGEAL_TEMP;//�����ڹ��ڶ����¶�
	TipArr[pos++].txt = _T("��");

	TipArr[pos].dID = IDC_EDIT_STOP_TIME;		//������ܵ�����Һ��ͣ��ʱ��
	TipArr[pos++].txt = _T("h");

*/
	m_pctlTip.Create(this);
//	m_pctlTip.SetTipBkColor(RGB(255,255,255));
	for (int i=0; i<pos; i++)
	{
		m_pctlTip.AddTool(GetDlgItem(TipArr[i].dID), (TipArr[i].txt));
	}

	return TRUE;
}

//------------------------------------------------------------------                
// DATE         :[2005/06/24]
// Author       :ZSY
// Parameter(s) :
// Return       :
// Remark       :�򿪳������ݿ�,��ʼһЩ����.
//------------------------------------------------------------------
BOOL CEditOriginalData::InitConstant()
{
	_RecordsetPtr pRs;			// ������ļ�¼��
	pRs.CreateInstance(__uuidof(Recordset));
	CString strSQL;				// SQL���
	try
	{
		strSQL = "SELECT * FROM [A_CONFIG] WHERE EnginID='"+m_ProjectID+"' ";
		pRs->Open(_variant_t(strSQL), GetCurrentProjectConnect().GetInterfacePtr(),
				adOpenStatic, adLockOptimistic, adCmdText);
		if (pRs->adoEOF && pRs->BOF)
		{
			// ��������û�м�¼��
			m_dMaxD0 = 2000;		//�ֽ��⾶��һ��Ĭ��ֵ  
			m_dValveSpace = 10;		//���ŵļ�� 
			return	FALSE; 
		}
		m_dMaxD0 = vtof(pRs->GetCollect(_variant_t("ƽ����Բ�ܵķֽ��⾶")) );		
		if( m_dMaxD0 <= 0 )
		{
			// ���ֽ��⾶������ʱ,��һ��Ĭ��ֵ 
			m_dMaxD0 = 2000;
		}
		m_dValveSpace = vtof(pRs->GetCollect(_variant_t("���ż��"))); 
		if (m_dValveSpace <= 0) 
		{
			m_dValveSpace = 10;
		}
		
		//��ù�¯���ڹ���������ѹ��
		m_dBoilerPressure = vtof(pRs->GetCollect(_variant_t("��¯���ڹ�������ѹ��")));
		//��¯���ڹ��������¶�
		m_dBoilerTemperature = vtof(pRs->GetCollect(_variant_t("��¯���ڹ��������¶�")));
		//��ȴˮ�¶�
		m_dCoolantTemperature = vtof(pRs->GetCollect(_variant_t("��ȴˮ�¶�")));
	}
	catch (_com_error)
	{	
		return FALSE;
	}
	return TRUE;
}

//------------------------------------------------------------------
// DATE         :[2005/11/14]
// Parameter(s) :
// Return       :
// Remark       :�����ȼ۱�������
//------------------------------------------------------------------
void CEditOriginalData::OnButtonCalcRatio() 
{
	UpdateData();
	CString strSQL;	// SQL���
	double	dPriRatio=0;	// ����õ��ȼ۱�
	if ( gbIsSelTblPrice )	// ����á�������ֵϵ����
	{
		try
		{
			_RecordsetPtr pRsVol;	// ���й��̵ľ��
			pRsVol.CreateInstance( __uuidof( Recordset ) );
			CString strPipeName;	// ��ǰ��¼�Ĺܵ����豸����
			m_PipeDeviceName.GetWindowText( strPipeName );
			if ( !strPipeName.IsEmpty() )
			{				
				strSQL = " SELECT * FROM [A_VOL] WHERE [VOL_NAME]='"+strPipeName+"' AND [VOL_BAK]='UESoft' ";
				if ( pRsVol->State == adStateOpen )
				{
					pRsVol->Close();
				}
				pRsVol->Open( _variant_t( strSQL ), GetPublicConnect().GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText );
				if ( !pRsVol->adoEOF ||  !pRsVol->BOF)
				{
					pRsVol->MoveFirst();
					dPriRatio = vtof( pRsVol->GetCollect( _variant_t( "vol_price" ) ) );
				}
				else
				{
					strSQL = " SELECT * FROM [A_VOL] WHERE [VOL_NAME]='"+strPipeName+"' AND LEFT(VOL,5)=LEFT('"+m_VolNo+"',5) ";
					if ( pRsVol->State == adStateOpen )
					{
						pRsVol->Close();
					}
					pRsVol->Open( _variant_t( strSQL ), GetPublicConnect().GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText );
					if ( !pRsVol->adoEOF || !pRsVol->BOF )
					{
						pRsVol->MoveFirst();
						dPriRatio = vtof( pRsVol->GetCollect( _variant_t( "vol_price" ) ) );
					}
				}
			}
		}
		catch (_com_error& e)
		{
			AfxMessageBox( e.Description() );
			return ;
		}
		if ( dPriRatio > 0 )	// �ڱ����ҵ���Ӧ�ı�ֵ
		{
			m_PriceRatio = dPriRatio;
			UpdateData( FALSE );
			return ;
		}
	}
	//////////////////////////////////////////////////////////////////////////	
	//					���ù�ʽ���������ֵϵ��

	InitConstant();		// ��ʼ����
	int RANGE;
	double	dPipePress = m_dPressure+atmospheric_pressure;	//����ˮ����ʱȡ���ڱ�ѹ��+����ѹ��
	double  dMediumTemp = m_MediumTemperature;	//�����¶�
	double  Tw = m_dCoolantTemperature; //��ȴˮ�¶�
	double	Sw=0;					//��ȴˮ����
	double  Hw=0;					//��ȴˮ����
	double	Sst=0;					//��¯���ڹ�����������
	double  Hst=0;					//��¯���ڹ�����������
	double  S=0;					//���ʱ���
	double  H=0;					//���ʱ���

	CString strMediaName;
	CString strTempMedia;
	m_ctlPipeMedium.GetWindowText(strMediaName);
	CHeatPreCal::GetMediumDensity(strMediaName, &strTempMedia);
	if ( !strTempMedia.IsEmpty() )
	{
		//ֻ���ڽ���Ϊ��ˮ�� �� ��������ʱ�ö�̬�����������.
		if ( strTempMedia.Find("ˮ") || strTempMedia.Find("����") )
		{
			if ( 0 == giCalSteanProp )			//      IF97 �� IFC67
				SETSTD_WASP( 97 );
			else
				SETSTD_WASP( 67 );
			
			//��ȴˮ
			PT2S(dPipePress,Tw, &Sw, &RANGE);
			PT2H(dPipePress,Tw, &Hw, &RANGE);
			
			//��¯���ڹ�������
			PT2S(m_dBoilerPressure, m_dBoilerTemperature, &Sst, &RANGE);
			PT2H(m_dBoilerPressure, m_dBoilerTemperature, &Hst, &RANGE);
			
			//����
			PT2S(dPipePress,dMediumTemp, &S, &RANGE);
			PT2H(dPipePress,dMediumTemp, &H, &RANGE);
			
			double dAe = (H-Hw-(Tw+273)*(S - Sw)) / (Hst-Hw-(Tw+273)*(Sst-Sw));
			
			dAe = (dAe < 0) ? 0 : dAe;
			CString str;
			
			str.Format( "%.2f", dAe );
			m_PriceRatio = strtod(str, NULL);
		}
	}
	
	UpdateData(FALSE);
}

//------------------------------------------------------------------
// DATE         :[2005/11/17]
// Parameter(s) :
// Return       :
// Remark       :�����ݿ���и��ݹܵ���������ý���Ӷֵϵ��
//------------------------------------------------------------------
BOOL CEditOriginalData::GetAeValue(CString strTypeName, double &dAe)
{
	if (strTypeName.IsEmpty())
	{
		return FALSE;
	}
	CString strSQL;
	
	return TRUE;
}

//------------------------------------------------------------------                
// DATE         : [2006/02/11]
// Author       : ZSY
// Parameter(s) : 
// Return       : 
// Remark       : ����һ���ӶԻ���
//------------------------------------------------------------------
void CEditOriginalData::AddDynamicDlg(CDialog *pDlg, UINT nID)
{
	//2006.01.07  ��̬����һ������Ի��� 

	if ( IsWindow(m_pDlgCurChild->GetSafeHwnd()) )
	{
		m_pDlgCurChild->ShowWindow( SW_HIDE );
	}
	if ( pDlg == NULL || !IsWindow( pDlg->GetSafeHwnd() ) )
	{ 
		pDlg->Create( nID, this );	
		DWORD style=::GetWindowLong(pDlg->GetSafeHwnd(),GWL_STYLE);
		if ( style & WS_CAPTION ) 
		{ 
			style &= ~WS_CAPTION; 
		} 
		style &= ~WS_SYSMENU; 
		SetWindowLong( pDlg->GetSafeHwnd(), GWL_STYLE, style );
	}

	CRect rect;
	m_ctlTitleTab.GetWindowRect( &rect );
	this->ScreenToClient( &rect );
	rect.DeflateRect(3,11,3,3);
	pDlg->SetWindowPos( NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOACTIVATE );
	// ���ñ���
	CString strTitle;
	pDlg->GetWindowText(strTitle);
	m_ctlTitleTab.SetWindowText(strTitle);
 
	m_pDlgCurChild = pDlg;
}


//------------------------------------------------------------------
// DATE         : [2006/02/20]
// Author       : ZSY
// Parameter(s) : VOID
// Return       : VOID
// Remark       : ����ر��·�����Ҫ���������뵽���ݿ��С�
//------------------------------------------------------------------
void CEditOriginalData::PutDataToSubterraneanDB(long nID)
{
	if ( !m_ctlTitleTab.IsWindowEnabled() )	//���û��ʾ�����ӵĴ���ʱ������ȡ����Ӧ�����ݡ�
	{
		return ;
	}

	if ( m_pDlgSubterranean != NULL )
	{
		//
		m_pDlgSubterranean->PutDataToSubterraneanDB(nID);
	}
}

void CEditOriginalData::UpdateControlSubterranean(int nPipeCount)
{
	BOOL bIsEnable = TRUE;
	int  i;
	int  j;
	CString strText;
	long pID[][2] = 
	{
		{IDC_CHECK_THICK1,  IDC_EDIT_THICK1},
		{IDC_CHECK_THICK2,  IDC_EDIT_THICK2},
		{IDC_STATIC_THICK1,	IDC_INSIDE_MATNAME},
		{IDC_STATIC_THICK2,	IDC_OUTSIZE_MATNAME},		
		{-1,-1}
	};
	CString strReplace[][2] = 
	{
		{_T("��"), _T("�ڶ���")},
		{_T("��"), _T("��һ��")},
		{_T(""),   _T("")},
	};
	switch(nPipeCount)
	{
	case 1:
	case 2:
		m_bIsCalInThick  = FALSE;
		m_bIsCalPreThick = FALSE;
		for (i = 0; pID[i][0] != -1; i++)
		{
			GetDlgItemText(pID[i][0], strText);
			for (j = 0; !strReplace[j][0].IsEmpty(); j++)
			{
				strText.Replace(strReplace[j][0], strReplace[j][1]);
			}
			SetDlgItemText(pID[i][0], strText);
			if ( i + nPipeCount < 4 )
			{
				bIsEnable = FALSE;
			}
			else
			{
				bIsEnable = TRUE;
			}
			GetDlgItem(pID[i][0])->EnableWindow( bIsEnable );
			GetDlgItem(pID[i][1])->EnableWindow( bIsEnable );
		}
		break;
	default:
		// �ָ��������״̬
		for (i = 0; pID[i][0] != -1; i++)
		{
			GetDlgItemText(pID[i][0], strText);
			for (j = 0; !strReplace[j][0].IsEmpty(); j++)
			{
				strText.Replace(strReplace[j][1], strReplace[j][0]);
			}
			SetDlgItemText(pID[i][0], strText);
			GetDlgItem(pID[i][0])->EnableWindow( TRUE );
			GetDlgItem(pID[i][1])->EnableWindow( TRUE );
		}
		//�ڱ��º��
		GetDlgItem(IDC_EDIT_THICK1)->EnableWindow(((CButton*)GetDlgItem(IDC_CHECK_THICK1))->GetCheck()?TRUE:FALSE);
		//�Ᵽ�º��
		GetDlgItem(IDC_EDIT_THICK2)->EnableWindow(((CButton*)GetDlgItem(IDC_CHECK_THICK2))->GetCheck()?TRUE:FALSE);
	}

	UpdateData(FALSE);	
}

BOOL CEditOriginalData::NumberSubtractOne(_RecordsetPtr &pRs, long lStartPos)
{
	if (pRs == NULL || pRs->GetState() != adStateOpen || (pRs->adoEOF && pRs->BOF) )
	{
		return FALSE;
	}
	long nID;
	for (pRs->MoveFirst(); !pRs->adoEOF; pRs->MoveNext() )
	{
		nID = vtoi( pRs->GetCollect("ID") );
		if (nID > lStartPos)
		{
			nID--;
			pRs->PutCollect(_variant_t("ID"), _variant_t(nID));
			pRs->Update();
		}
	}
	return TRUE;
}

void CEditOriginalData::OnSelchangePipeMat() 
{
	UpdateMediumMaterialDensity();
}

// ���ݽ������ƺ͹ܵ����ϳƻ����Ծ͵��ܶȣ��Զ�д�뵽�����������ӵĶԻ����С�
void CEditOriginalData::UpdateMediumMaterialDensity()
{
	UpdateData(TRUE);

	UINT nMethodIndex = -1;
	CString strMedium;
	CString strMat;
	double  dMediumDensity;
	double  dMatDensity;

	UINT pos;
	pos = m_ctlCalcMethod.GetCurSel();
	if (pos >= 0)
	{
		nMethodIndex = m_ctlCalcMethod.GetItemData(pos);
	}
	if (nMethodIndex == nPreventCongealMethod)
	{
		if (NULL == m_pMatProp)
		{
			m_pMatProp = new GetPropertyofMaterial;
		}
	    m_ctlPipeMedium.GetWindowText(strMedium);
		if (!strMedium.IsEmpty())
		{
			dMediumDensity = CHeatPreCal::GetMediumDensity(strMedium);
			if (dMediumDensity > 0)
			{
				m_pDlgCongeal->m_dMediumDensity = dMediumDensity;
			}
		}

		m_PipeMat.GetWindowText(strMat);
		if (!strMat.IsEmpty())
		{
			dMatDensity = m_pMatProp->GetDensity( strMat,2,theApp.m_pConMaterial );
			if (dMatDensity > 0)
			{
				m_pDlgCongeal->m_dPipeDensity = dMatDensity;
			}
		}
		m_pDlgCongeal->UpdateData(FALSE);	
	}
	
	UpdateData(FALSE);
}
