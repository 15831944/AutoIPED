// ImportFromPreCalcXLSDlg.cpp: implementation of the CImportFromPreCalcXLSDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "autoiped.h"
#include "ImportFromPreCalcXLSDlg.h"

#include "ProjectOperate.h"	// ���̲�����,���ڵ��빤��

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImportFromPreCalcXLSDlg::CImportFromPreCalcXLSDlg(CWnd* pParent)
:CImportFromXLSDlg(pParent)
{
	this->SetRegSubKey(_T("ImportFromPreCalcXLSDlg"));
}

CImportFromPreCalcXLSDlg::~CImportFromPreCalcXLSDlg()
{

}

BOOL CImportFromPreCalcXLSDlg::InitPropertyWnd()
{
	CImportFromXLSDlg::InitPropertyWnd();
	
	this->SetWindowText(_T("��Excel���뱣��ԭʼ����") );
	
	// zsy [2005/12/28]
	
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
		{_T("����ԭʼ����"),	CPropertyWnd::TitleElement,NULL},								//4

		{_T("���"),			CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("���ѡ���Զ�����������ݽ�����ԭʼ���ݱ��ĩβ����\r\n������Ҫѡ����Excel��Ӧ���е�������Ϊ���")
		 _T("����ʱ���ԭʼ���ݱ���������ͬ����ŵļ�¼���ᱻ����")},	//5

		{_T("����"),			CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ������Ŷ�Ӧ��Excel����Ϊ����")},

		{_T("ɫ��"),			CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ����ɫ����Ӧ��Excel����Ϊ����\r\nע�����Ӧ������ɫ���Ĵ��Ŷ���������")
		 _T("����\"����ˮ\"�Ĵ�����8")},

		{_T("�ܵ�/�豸����"),	CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ����ܵ�/�豸���ƶ�Ӧ��Excel����Ϊ����")},

		{_T("�ܵ��⾶/���"),	CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ����ܵ��⾶/����Ӧ��Excel����Ϊ����")},

		{_T("�ܵ��ں�"),		CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ����ܵ��ں��Ӧ��Excel����Ϊ����")},

		{_T("�����¶�"),		CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ��������¶ȶ�Ӧ��Excel����Ϊ����")},

		{_T("��װ�ص�"),		CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ���밲װ�ص��Ӧ��Excel����Ϊ����")},

		{_T("�ܵ�����"),		CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ����ܵ����ʶ�Ӧ��Excel����Ϊ����")},

		{_T("��ע"),			CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ���뱸ע��Ӧ��Excel����Ϊ����")},

		{_T("�ڱ��²��������"),CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ�����ڱ��²�������ƶ�Ӧ��Excel����Ϊ����")},

		{_T("�Ᵽ�²��������"),CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ�����Ᵽ�²�������ƶ�Ӧ��Excel����Ϊ����")},

		{_T("�������������"),	CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ���뱣����������ƶ�Ӧ��Excel����Ϊ����")},

		{_T("����"),			CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ������ٶ�Ӧ��Excel����Ϊ����")},

		{_T("�ȼ۱�������"),	CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ�����ȼ۱������۶�Ӧ��Excel����Ϊ����")},

		{_T("������Сʱ��"),	CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ����������Сʱ����Ӧ��Excel����Ϊ����")},

		{_T("�͹ܵ����º�"),	CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ�����͹ܵ����º��Ӧ��Excel����Ϊ����\r\n���Ҫʹ������Ч��������ĩβ���ַ�Ӧ����\'O\'")},

		{_T("����"),			CPropertyWnd::ChildElement|CPropertyWnd::ComBoxElement,
		 _T("��ѡ����������Ӧ��Excel����Ϊ����")},
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

		if(ElementDef.szElementName==_T("���"))
		{
			((CComboBox*)ElementDef.pControlWnd)->AddString(_T("�Զ�"));
		}

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

/////////////////////////////////////////////
//
// ��֤�������ݵ���Ч��
//
BOOL CImportFromPreCalcXLSDlg::ValidateData()
{
	BOOL bRet;
	CPropertyWnd::ElementDef ElementDef;
	CString strTemp;
	int iPos;

	bRet=CImportFromXLSDlg::ValidateData();

	if(!bRet)
		return FALSE;

	iPos=this->m_PropertyWnd.FindElement(0,_T("���"));
	this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);

	if(ElementDef.RightElementText.IsEmpty())
	{
		strTemp.Format(_T("��Ų���Ϊ��"));
		ReportExceptionErrorLV1(strTemp);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////
//
// ִ�е������ݵĲ���
//
void CImportFromPreCalcXLSDlg::BeginImport()
{
	int iPos;
	CPropertyWnd::ElementDef ElementDef;
	CProjectOperate::ImportFromXLSStruct ImportTable;
	CProjectOperate::ImportFromXLSElement ImportTableItem[60];
	int TableItemCount;
	CProjectOperate Import;

	CWaitCursor WaitCursor;
	BOOL	bIsAutoNO = FALSE;		// �Լ�¼�Զ����

	this->m_HintInformation=_T("��ʼ��������");
	this->UpdateData(FALSE);
	this->UpdateWindow();

	Import.SetProjectDbConnect(this->GetProjectDbConnect());

	//
	// ��дImportFromXLSStruct�ṹ��Ϣ
	//
	ImportTable.ProjectDBTableName=_T("pre_calc");

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
	TableItemCount = 0;
	while(TRUE)
	{
		if(iPos>=this->m_PropertyWnd.GetElementCount())
			break;

		this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
		iPos++;

		if ( ElementDef.szElementName == _T("���") && ElementDef.RightElementText == _T("�Զ�") )
		{
			bIsAutoNO = TRUE;
			continue;
		}
		
		// ���ҵ�Ԫ��Ϊ��ʱ������
		if( !ElementDef.RightElementText.IsEmpty() )
		{
			ImportTableItem[TableItemCount].SourceFieldName = ElementDef.RightElementText;
			ImportTableItem[TableItemCount].DestinationName = ElementDef.strFieldName;
			TableItemCount++;
		}
	}
	ImportTable.pElement=ImportTableItem;
	ImportTable.ElementNum=TableItemCount;
	try
	{
		//
		// ����ڡ���š������ˡ��Զ�����ID���Զ���д
		// �����˲�ͬ�Ĺ��̴���
		//Import.ImportPre_CalcFromXLS( &ImportTable, bIsAutoNO );
		Import.ImportTblEnginIDXLS( &ImportTable, bIsAutoNO );

/*
		iPos=this->m_PropertyWnd.FindElement(0,_T("���"));
		this->m_PropertyWnd.GetElementAt(&ElementDef,iPos);
		if(ElementDef.RightElementText==_T("�Զ�"))
		{
			Import.ImportPre_CalcFromXLS(&ImportTable,TRUE);
		}
		else
		{
			if(!ElementDef.RightElementText.IsEmpty())
			{
				ImportTableItem[TableItemCount].SourceFieldName=ElementDef.RightElementText;
				ImportTableItem[TableItemCount].DestinationName=_T("id");
				TableItemCount++;

				ImportTable.ElementNum=TableItemCount;
				Import.ImportPre_CalcFromXLS(&ImportTable,FALSE);
			}
		}
*/
	}
	catch(_com_error &e)
	{
		this->m_HintInformation=_T("���ݵ���ʧ��");
		this->UpdateData(FALSE);
		this->GetProjectDbConnect()->RollbackTrans();
		ReportExceptionErrorLV1(e);
		
		return;
	}
	catch(COleDispatchException *e)
	{
		this->m_HintInformation=_T("���ݵ���ʧ��");
		this->UpdateData(FALSE);
		this->GetProjectDbConnect()->RollbackTrans();
		ReportExceptionErrorLV1(e);
		e->Delete();
		return;
	}


	this->m_HintInformation=_T("���ݵ���ɹ�");
	this->UpdateData(FALSE);
}
