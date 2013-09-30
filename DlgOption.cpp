// DlgOption.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "DlgOption.h"
#include "EDIBgbl.h"

#include "MaterialName.h"		//�������Ƶ��滻
#include "heatprecal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOption dialog
extern CAutoIPEDApp theApp;


CDlgOption::CDlgOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOption)
	m_IsColseCompress = bIsCloseCompress;
	m_IsReplOld = bIsReplaceOld;
	m_IsMoveUpdate = bIsMoveUpdate;
	m_IsAutoSelectPre = bIsAutoSelectPre;
	m_bIsHeatLoss =		bIsHeatLoss;
	m_IsAutoAddValve = bIsAutoAddValve;
	m_dMaxRatio = 0.0;
	m_dSurfaceMaxTemp = 0.0;
	m_bIsRunSelectEng= gbIsRunSelectEng;
	m_bIsSeleTbl = gbIsSelTblPrice;
	m_bIsReplaceUnit = gbIsReplaceUnit;
	m_bAutoPaint120 = gbAutoPaint120;
	m_bWithoutProtectionCost = gbWithoutProtectionCost;
	m_bInnerByEconomic = gbInnerByEconomic;

	//}}AFX_DATA_INIT
}


void CDlgOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOption)
	DDX_Control(pDX, IDC_PROGRESS1, m_ReplaceProgress);
	DDX_Check(pDX, IDC_CHECK1, m_IsColseCompress);
	DDX_Check(pDX, IDC_CHECK2, m_IsReplOld);
	DDX_Check(pDX, IDC_CHECK3, m_IsMoveUpdate);
	DDX_Check(pDX, IDC_CHECK_AUTOSELECT_PRE, m_IsAutoSelectPre);
	DDX_Check(pDX, IDC_JUDGEMENT_HEAT_LOSS, m_bIsHeatLoss);
	DDX_Check(pDX, IDC_AUTO_ADD_VALVE, m_IsAutoAddValve);
	DDX_Text(pDX, IDC_EDIT_TEMP_RATIO, m_dMaxRatio);
	DDX_Text(pDX, IDC_EDIT_FACE_MAX_TEMP, m_dSurfaceMaxTemp);
	DDX_Check(pDX, IDC_CHECK_SELECT_ENG, m_bIsRunSelectEng);
	DDX_Check(pDX, IDC_CHECK_PRICE_RATIO, m_bIsSeleTbl);
	DDX_Check(pDX, IDC_CHECK_IMPORT_REPLACE_UNIT, m_bIsReplaceUnit);
	DDX_Check(pDX, IDC_CHECK_AUTOPAINT_120, m_bAutoPaint120);
	DDX_Check(pDX, IDC_CHECK_WITHOUT_PROTECTION_COST, m_bWithoutProtectionCost);
	DDX_Check(pDX, IDC_CHECK_INNER_BY_ECONOMIC, m_bInnerByEconomic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOption, CDialog)
	//{{AFX_MSG_MAP(CDlgOption)
	ON_BN_CLICKED(IDC_REPLACE_OLDNAME_NEWNAME, OnReplaceOldnameNewname)
	ON_BN_CLICKED(IDC_REPLACE_CURRENT_OLDNAME_NEWNAME, OnReplaceCurrentOldnameNewname)
	ON_BN_CLICKED(IDC_RADIO_HAND_SET, OnRadioHandSet)
	ON_BN_CLICKED(IDC_RADIO_RENEW_SET, OnRadioRenewSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOption message handlers

void CDlgOption::OnOK() 
{
	UpdateData(TRUE);
	long nCheckedNO;

 	//����ѡ���½ṹ��
	nCheckedNO = GetCheckedRadioButton(IDC_RADIO_HAND_SET,IDC_RADIO_RENEW_SET);
	if( nCheckedNO != 0 )
	{
		giInsulStruct = nCheckedNO - IDC_RADIO_HAND_SET;
	}
	//����ˮ���������ʵı�׼
	nCheckedNO = GetCheckedRadioButton( IDC_RADIO_97, IDC_RADIO_67 );
	if ( nCheckedNO != 0 )
	{
		giCalSteanProp = nCheckedNO - IDC_RADIO_97;
	}

	CHeatPreCal::SetConstantDefine("ConstantDefine","FaceMaxTemp",m_dSurfaceMaxTemp);
	CHeatPreCal::SetConstantDefine("ConstantDefine","Ratio_MaxHotTemp",m_dMaxRatio);

	bIsCloseCompress= m_IsColseCompress;    //�ر�ʱ�Ƿ�ѹ�����ݿ� 
	bIsReplaceOld	= m_IsReplOld;        	//��ʱ�Ƿ��Զ��滻�ɲ�������
	bIsMoveUpdate	= m_IsMoveUpdate;       //�༭ԭʼ�����ƶ���¼ʱ�Ƿ����
	bIsAutoSelectPre= m_IsAutoSelectPre;    //�༭ԭʼ�����ƶ���¼ʱ�Զ�ѡ���²���
	bIsHeatLoss		= m_bIsHeatLoss;		//����ʱ�ж����ɢ���ܶ�
	bIsAutoAddValve	= m_IsAutoAddValve; 	//�༭ԭʼ�����ƶ���¼ʱ�ڹܵ����Զ����ӷ���
	gbIsRunSelectEng= m_bIsRunSelectEng;	//����ʱ�Ƿ񵯳�ѡ�񹤳̾��Ի���
	gbIsSelTblPrice	= m_bIsSeleTbl;			//�༭ԭʼ���ݣ��Զ�ʱȡ���е��ȼ۱�������
	gbIsReplaceUnit = m_bIsReplaceUnit;		// ���뱣����������ʱ�滻��λ
	gbAutoPaint120  = m_bAutoPaint120;		// ͳ������ʱ���Զ����ϱ������ݽ����¶�С��120�ȵļ�¼��
	gbWithoutProtectionCost  = m_bWithoutProtectionCost;		// ���㾭�ú��ʱ��������������Ϸ��ã�Ĭ��Ϊ0-����
	gbInnerByEconomic  = m_bInnerByEconomic;		// ˫����ı��¼��㾭�ú��ʱ�ڲ㲻�������¶ȷ����㣬Ĭ��Ϊ0-�������¶ȷ�����
	//��ѡ����е�״ֵ̬д�뵽ע���
	theApp.WriteRegedit();

	CDialog::OnOK();
}

/////////////////////////////////////////////////
//
// ��Ӧ"�滻�������ݿ�ľɲ��ϵ�����"��ť
//
void CDlgOption::OnReplaceOldnameNewname() 
{
	CWaitCursor WaitCursor;
	CMaterialName MaterialName;
	CMaterialName::tagReplaceStruct ReplaceInfo[6];
	int ProgressPos=1;
	int i;
	int pos=1;
	//��ʾ������
	m_ReplaceProgress.ShowWindow(SW_NORMAL);

	m_ReplaceProgress.SetRange(1,10);
	m_ReplaceProgress.SetPos(ProgressPos);

	ReplaceInfo[pos].strTableName=_T("E_PREEXP");
	ReplaceInfo[pos++].pstrFieldsName=_T("C_PI_MAT");

//	ReplaceInfo[pos].strTableName=_T("EDIT");				// �ñ�Ϊһ����ʱ��,����Ҫ����. [7/8/2005]
//	ReplaceInfo[pos++].pstrFieldsName=_T("C_PI_MAT");

	ReplaceInfo[pos].strTableName=_T("PRE_CALC");
	ReplaceInfo[pos++].pstrFieldsName=_T("C_PI_MAT");

//	ReplaceInfo[pos].strTableName=_T("PRE_TAB");
//	ReplaceInfo[pos++].pstrFieldsName=_T("C_PI_MAT");

	ProgressPos++;
	m_ReplaceProgress.SetPos(ProgressPos);
 
	try
	{
		MaterialName.SetNameRefRecordset(theApp.m_pConMaterial,_T("Material_OLD_NEW"));
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
	}

	try
	{
		for(i=1;i<pos;i++)
		{
			MaterialName.ReplaceNameOldToNew(theApp.m_pConnection,ReplaceInfo[i]);

			ProgressPos++;
			m_ReplaceProgress.SetPos(ProgressPos);
		}
	}
	catch(_com_error &e)
	{
		CString str;

		str.Format(_T("%s���%d�ֶγ���"),ReplaceInfo[i].strTableName,ReplaceInfo[i].pstrFieldsName);

		Exception::SetAdditiveInfo(str);

		ReportExceptionErrorLV1(e);
	}

//	ReplaceInfo[0].strTableName=_T("A_C09");
//	ReplaceInfo[0].pstrFieldsName=_T("PIPE_MAT");

	ReplaceInfo[0].strTableName=_T("A_PIPE");
	ReplaceInfo[0].pstrFieldsName=_T("CL1");

	try
	{
		for(int i=0;i<1;i++)
		{
			MaterialName.ReplaceNameOldToNew(theApp.m_pConnectionCODE,ReplaceInfo[i]);

			ProgressPos++;
			m_ReplaceProgress.SetPos(ProgressPos);
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
	}
			ProgressPos++;
			m_ReplaceProgress.SetPos(ProgressPos);

	//���ؽ�����
	m_ReplaceProgress.SetPos(10);
	Sleep(10);
	m_ReplaceProgress.ShowWindow(SW_HIDE);
}

/////////////////////////////////////////////////
//
// ��Ӧ"�滻��ǰ���̵ľɲ��ϵ�����"��ť
//
void CDlgOption::OnReplaceCurrentOldnameNewname() 
{
	CWaitCursor WaitCursor;
	CMaterialName MaterialName;
	_RecordsetPtr IRecordset;
	CString SQL;
	CMaterialName::tagReplaceStruct ReplaceInfo[6];
	HRESULT hr;
	int ProgressPos=0;
	int pos=1;
	hr=IRecordset.CreateInstance(__uuidof(Recordset));

	if(FAILED(hr))
	{
		ReportExceptionErrorLV1(_com_error(hr));
		return;
	}

	//��ʾ������
	m_ReplaceProgress.ShowWindow(SW_NORMAL);

	m_ReplaceProgress.SetRange(1,4);
	m_ReplaceProgress.SetPos(ProgressPos);

	ReplaceInfo[pos].strTableName=_T("E_PREEXP");
	ReplaceInfo[pos++].pstrFieldsName=_T("C_PI_MAT");

//	ReplaceInfo[pos].strTableName=_T("EDIT");
//	ReplaceInfo[pos++].pstrFieldsName=_T("C_PI_MAT");

	ReplaceInfo[pos].strTableName=_T("PRE_CALC");
	ReplaceInfo[pos++].pstrFieldsName=_T("C_PI_MAT");

//	ReplaceInfo[pos].strTableName=_T("PRE_TAB");
//	ReplaceInfo[pos++].pstrFieldsName=_T("C_PI_MAT");

	try
	{
		MaterialName.SetNameRefRecordset(theApp.m_pConMaterial,_T("Material_OLD_NEW"));
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
	}


	try
	{
		for(int i=1;i<pos;i++)
		{
//			MaterialName.ReplaceNameOldToNew(theApp.m_pConnection,ReplaceInfo[i]);
			SQL.Format(_T("SELECT * FROM %s WHERE EnginID='%s'"),ReplaceInfo[i].strTableName,EDIBgbl::SelPrjID);

			IRecordset->Open(_variant_t(SQL),_variant_t((IDispatch*)theApp.m_pConnection),
							adOpenDynamic,adLockOptimistic,adCmdText);

			MaterialName.ReplaceNameOldToNew(IRecordset,ReplaceInfo[i]);

			IRecordset->Close();

			ProgressPos++;
			m_ReplaceProgress.SetPos(ProgressPos);
		}
	}
	catch(_com_error &e)
	{
		ReportExceptionErrorLV1(e);
	}
	//���ؽ�����
	m_ReplaceProgress.ShowWindow(SW_HIDE);
}
//////////////////////////////
//ѡ���ֹ�ȷ���ļ�¼
void CDlgOption::OnRadioHandSet() 
{
	CheckRadioButton(IDC_RADIO_HAND_SET,IDC_RADIO_RENEW_SET,IDC_RADIO_HAND_SET);
}
///////////////////////////
//ѡ�����㱣�½ṹ
void CDlgOption::OnRadioRenewSet() 
{
	CheckRadioButton(IDC_RADIO_HAND_SET,IDC_RADIO_RENEW_SET,IDC_RADIO_RENEW_SET);
}

BOOL CDlgOption::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	strTmp;		//��ʱ����

 	//���㱣�½ṹ��
	CheckRadioButton(IDC_RADIO_HAND_SET,IDC_RADIO_RENEW_SET,IDC_RADIO_HAND_SET+giInsulStruct );

	//����ˮ���������ʵı�׼
	CheckRadioButton( IDC_RADIO_97, IDC_RADIO_67, IDC_RADIO_97 + giCalSteanProp );

	//���ϱ��½ṹ�����¶����Ᵽ�²��ϵ����ʹ���¶ȵı�ֵ
	if( !CHeatPreCal::GetConstantDefine("ConstantDefine","Ratio_MaxHotTemp",strTmp) )
	{
		m_dMaxRatio = 0.9;
	}
	else
	{
		m_dMaxRatio = strtod(strTmp, NULL);
	}
	//�������������¶�
	if (!CHeatPreCal::GetConstantDefine("ConstantDefine", "FaceMaxTemp", strTmp))
	{
		m_dSurfaceMaxTemp = 50;
	}
	else
	{
		m_dSurfaceMaxTemp = strtod(strTmp,NULL);
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
