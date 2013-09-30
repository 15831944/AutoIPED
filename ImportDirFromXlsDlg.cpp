// ImportDirFromXlsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "autoiped.h"
#include "ImportDirFromXlsDlg.h"

#include "ProjectOperate.h"	// ���̲�����,���ڵ��빤��


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportDirFromXlsDlg dialog


CImportDirFromXlsDlg::CImportDirFromXlsDlg(CWnd* pParent /*=NULL*/)
	: CImportFromXLSDlg(pParent)
{
	//{{AFX_DATA_INIT(CImportDirFromXlsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	this->SetRegSubKey(_T("ImportDirFromXlsDlg"));

}


void CImportDirFromXlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CImportFromXLSDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportDirFromXlsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportDirFromXlsDlg, CImportFromXLSDlg)
	//{{AFX_MSG_MAP(CImportDirFromXlsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportDirFromXlsDlg message handlers

BOOL CImportDirFromXlsDlg::InitPropertyWnd()
{
	CImportFromXLSDlg::InitPropertyWnd();

	
	return TRUE;
	//  [2005/12/28]
	//
	// ��ʼ�����Կؼ�
	//
	CPropertyWnd::ElementDef ElementDef;

	struct
	{
		LPCTSTR ElementName;	// ��Ԫ������
		UINT	Style;			// ��ʾ��ʽ
		LPCTSTR HintInfo;		// �뵥Ԫ��ص���ʾ��Ϣ
	}ElementSet[]=
	{
		{_T("���ԭʼ����"),	CPropertyWnd::TitleElement,NULL},

		{_T("����"),			CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ������Ŷ�Ӧ��Excel����Ϊ����")},

		{_T("�������"),			CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ���������ƶ�Ӧ��Excel����Ϊ����")},

		{_T("��ҵID"),			CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ������ҵID��Ӧ��Excel����Ϊ����\r\nע�����Ӧ��������ҵ�Ĵ��Ŷ���������")
		 _T("����\"����\"�Ĵ�����0������ɲ���дĬ��Ϊ0")},

		{_T("��ƽ׶�ID"),	CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ������ƽ׶�ID��Ӧ��Excel����Ϊ����\r\nע�����Ӧ��������ҵ�Ĵ��Ŷ���������")
		 _T("����\"ʩ��ͼ\"�Ĵ�����4������ɲ���дĬ��Ϊ4")},

		{_T("רҵID"),	CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ����רҵID��Ӧ��Excel����Ϊ����\r\nע�����Ӧ��������ҵ�Ĵ��Ŷ���������")
		 _T("����\"�Ȼ�\"�Ĵ�����3������ɲ���дĬ��Ϊ3")},
	};


	for(int i=0;i<sizeof(ElementSet)/sizeof(ElementSet[0]);i++)
	{
		ElementDef.szElementName=ElementSet[i].ElementName;
		ElementDef.Style=ElementSet[i].Style;

		ElementDef.pVoid=NULL;

		if(ElementSet[i].HintInfo!=NULL)
		{
			
			ElementDef.pVoid=new CString(ElementSet[i].HintInfo);
		}

		m_PropertyWnd.InsertElement(&ElementDef);

	}

	m_PropertyWnd.RefreshData();

	//
	// �ӡ�����ԭʼ���ݡ��Ժ�ĵ�Ԫ�������EXCEL��������Ϣ
	//
	TCHAR ColumeName[128];
	int iPos;

	for(i=1;i<sizeof(ElementSet)/sizeof(ElementSet[0]);i++)
	{
		iPos=this->m_PropertyWnd.FindElement(0,ElementSet[i].ElementName);

		this->m_PropertyWnd.CreateElementControl(iPos);

		this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);

		for(int k=0;k<100;k++)
		{
			int j,m,m2;

			for(j=k,m=1;j/26!=0;m++,j/=26);

			ColumeName[m]=_T('\0');
			
			m--;
			j=k;
			m2=m;
			while(m>=0)
			{
				ColumeName[m]=j%26+_T('A');

				j=j/26;

				if(m2!=m)
				{
					ColumeName[m]--;
				}

				m--;
			}

			((CComboBox*)ElementDef.pControlWnd)->AddString(ColumeName);
		}

	}

	return TRUE;
}

/////////////////////////////////////////////////////////
//
// ��֤����������Ƿ���Ч
//
// �����Ч����TRUE�����򷵻�FALSE��ʱ���������������
//
BOOL CImportDirFromXlsDlg::ValidateData()
{
	BOOL bRet;
	CPropertyWnd::ElementDef ElementDef;
	CString strTemp;
	int iPos;

	bRet=CImportFromXLSDlg::ValidateData();

	if(!bRet)
		return FALSE;

	LPCTSTR szFields[]=
	{
		_T("����"),
		_T("�������")
	};

	for(int i=0;i<sizeof(szFields)/sizeof(szFields[0]);i++)
	{
		iPos=this->m_PropertyWnd.FindElement(0,szFields[i]);
		this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);

		if(ElementDef.RightElementText.IsEmpty())
		{
			strTemp.Format(_T("%s����Ϊ��"),szFields[i]);
			ReportExceptionErrorLV1(strTemp);
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////
//
// ��ʼ��������
//
// ����֮ǰValidateData���뷵��TRUE
//
void CImportDirFromXlsDlg::BeginImport()
{
	int iPos;
	CPropertyWnd::ElementDef ElementDef;
	CProjectOperate::ImportFromXLSStruct ImportTable;
	CProjectOperate::ImportFromXLSElement ImportTableItem[50];
	int TableItemCount;
	CProjectOperate Import;

	CWaitCursor WaitCursor;

	//
	// �����������ֶεĶ��ձ�
	//
/*	struct
	{
		LPCTSTR NameCh;		// pre_calc���ֶ�������������
		LPCTSTR NameField;	// pre_calc���ֶ���
	}FieldsName[]=
	{
		{_T("����"),			_T("jcdm")},
		{_T("�������"),		_T("jcmc")},
		{_T("��ҵID"),			_T("SJHYID")},
		{_T("��ƽ׶�ID"),		_T("SJJDID")},
		{_T("רҵID"),			_T("ZYID")},
	};
*/
	this->SetHintInformation(_T("��ʼ��������"));

	Import.SetProjectDbConnect(this->GetProjectDbConnect());

	//
	// ��дImportFromXLSStruct�ṹ��Ϣ
	//
	ImportTable.ProjectDBTableName=_T("volume");

	ImportTable.ProjectID=this->GetProjectID();

	ImportTable.XLSFilePath=this->m_XLSFilePath;

	iPos=this->m_PropertyWnd.FindElement(0,_T("�������ݿ�ʼ�к�"));
	this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
	ImportTable.BeginRow=_ttoi(ElementDef.RightElementText);

	iPos=this->m_PropertyWnd.FindElement(0,_T("Excel��������"));
	this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
	ImportTable.XLSTableName=ElementDef.RightElementText;

	iPos=this->m_PropertyWnd.FindElement(0,_T("�������������"));
	this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
	ImportTable.RowCount=_ttoi(ElementDef.RightElementText);

	//
	// �����Կؼ��е���������ImportTableItem�ṹ
	//

	iPos++;
	TableItemCount = 0;		// �ӿؼ��ĸ���
	while(TRUE)
	{
		if(iPos>=this->m_PropertyWnd.GetElementCount())
			break;

		this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
		iPos++;

		// ���ҵ�Ԫ��Ϊ��ʱ������
		if(!ElementDef.RightElementText.IsEmpty())
		{
			ImportTableItem[TableItemCount].SourceFieldName = ElementDef.RightElementText;

			//
			// ͨ��FieldsName���ҵ���Ӧ��pre_calc���ֶ���
			//
/*			for(int i=0;i<sizeof(FieldsName)/sizeof(FieldsName[0]);i++)
			{
				if(ElementDef.szElementName==_T(FieldsName[i].NameCh))
					break;
			}

			//���û�ҵ�����
			if(i==sizeof(FieldsName)/sizeof(FieldsName[0]))
				continue;

			ImportTableItem[TableItemCount].DestinationName=FieldsName[i].NameField;
*/
			ImportTableItem[TableItemCount].DestinationName = ElementDef.strFieldName;
			TableItemCount++;
		}
	}

	ImportTable.pElement=ImportTableItem;
	ImportTable.ElementNum=TableItemCount;

	//
	// ��ʼ��������
	//
	try
	{
		Import.ImportA_DirFromXLS(&ImportTable);
	}
	catch(_com_error &e)
	{
		this->m_HintInformation=_T("���ݵ���ʧ��");
		this->UpdateData(FALSE);

		ReportExceptionErrorLV1(e);

		return;
	}
	catch(COleDispatchException *e)
	{
		this->m_HintInformation=_T("���ݵ���ʧ��");
		this->UpdateData(FALSE);

		ReportExceptionErrorLV1(e);
		e->Delete();
		return;
	}

	this->SetHintInformation(_T("���ݵ���ɹ�"));
}

BOOL CImportDirFromXlsDlg::OnInitDialog() 
{
	CImportFromXLSDlg::OnInitDialog();
	
	this->SetWindowText(_T("������Ŀ¼"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
