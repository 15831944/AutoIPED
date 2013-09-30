// EDIBgbl.h: interface for the EDIBgbl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDIBGBL_H__264E7A48_66B4_440E_B867_3490AE90662E__INCLUDED_)
#define AFX_EDIBGBL_H__264E7A48_66B4_440E_B867_3490AE90662E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum IPEDCodeCallingKey{
   //������ƹ涨��׼�ؼ���
   nKey_CODE_DL5072_1997 = 1,
   nKey_CODE_SH3010_2000, //2,
   nKey_CODE_GB8175_2008 , //3
   nKey_CODE_CJJ34_2002 ,//4
   nKey_CODE_DL5072_2007 ,//5
   nKey_CODE_GB4272_2008,  //6
   nKey_CODE_GB50264_1997,  //7
   nKey_CODE_GB15586_1995 ,//8
   nKey_CODE_GB4272_1992 ,//9
   nKey_CODE_GB8175_1987, //10
   nKey_CODE_SH3126_2001  //11
};

class EDIBgbl  
{
public:
	typedef struct CAPTION2FIELD		//EXCEL���еı����Ӧ��ACCESS���е��ֶ���
	{
		CString strCaption;			// ���ļ����⣬��EXCEL�е��ֶ�����
		CString	strExcelNO;			// ��ӦExcel���к�
		CString strField;			// ACCESS�е��ֶ�����
		CString strRemark;			// �ֶεı�ע��Ϣ
		int		nFieldType;			// �ֶε�����,0Ϊ�ַ���,1Ϊ������
	};

public:	// Method
	
	// �½�һ����ʱ�����ڼ������ϵ����ֵ��
	static int NewCalcAlfaTable();

	//�����ݿ�pCon��,��ָ���ı���в���.��Ĭ�ϵļ�¼���뵽ָ���Ĺ�����
	static bool InsertDefTOEng(_ConnectionPtr& pCon,CString strTblName,CString strPrjID,CString strDefKey="");

	// �������趨�����ݿ�·�����浽�ļ��С�
	static bool GetCurDBName();
	
	// ���ļ��ж�ȡ���ݿ��·��
	static bool SetCurDBName();

	// �滻���ݿ���һЩ�ֶε�ֵ
	static BOOL ReplaceFieldValue(CString strEnginID=_T(""));
	
	//��ù������ݿ�Ŀ¼
	static CString GetShareDbPath();

public:		// Property
	
	// ��ǰѡ��ľ��ID
	static long SelVlmID;
	
	// ��ǰѡ�����ҵID
	static long SelHyID;	
	
	// ��ƽ׶�ID
	static long SelDsgnID;
	
	// ���̴���
	static CString SelPrjID;

	//��������
	static CString SelPrjName;
	
	//������
    static CString SelVlmCODE;

	// Kgf��N��ת��ϵ��( = 9.80665 )
	const  static  double kgf2N;

	// �淶��׼(��ҵ)���� �磺������ҵ
	static CString sCur_CodeName;

	// �淶��׼���� �磺DL/T5054-1996
	static CString sCur_CodeNO;	

	// �淶��׼�������ֱ�־����������ʡ�ռ䣬ÿ���������ݼ�¼��Ӧ����������ֶΣ�
	//����ÿ���ܵ������԰���ͬ�ı��±�׼����,�磺1
	static long iCur_CodeKey;	

	// רҵID
	static long SelSpecID;
	
// PATH
	//�������ݿ����ڵ�·��
	static CString sMaterialPath;

	//��ǰ�������ݿ�����ơ�
	static CString sCur_MaterialCodeName;

	//��׼���ݿ����ڵ�·��
	static CString sCritPath;

	//��ǰ��׼���ݿ������
	static CString sCur_CritDbName;
	
	//�������ݿ��·��
//	static CString strCur_MediumDBPath;

	static _RecordsetPtr pRsCalc;

public:
	EDIBgbl();
	virtual ~EDIBgbl();

protected:
};

#endif // !defined(AFX_EDIBGBL_H__264E7A48_66B4_440E_B867_3490AE90662E__INCLUDED_)
