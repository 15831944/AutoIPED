// TotalTableIPED.h: interface for the CTotalTableIPED class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOTALTABLEIPED_H__D56263C0_85BA_4E9F_B52C_11D964B61BA6__INCLUDED_)
#define AFX_TOTALTABLEIPED_H__D56263C0_85BA_4E9F_B52C_11D964B61BA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//���ܱ���

class CTotalTableIPED 
{
public:
	BOOL TotalSelfTable();	// �Ի��ܳ��������������ͳһ����.(����)
	BOOL CreateTempTable(_ConnectionPtr pCon, CString strTblName);
	void passModiTatal(CString str1,CString str2,CString str3);
	//9/2
	bool moduleCOLOR_S(double& dw,double& dCode, double& dLength, CString& strFace, double& dFace_1, CString& strRing, double& dRing_1, double& dSRing, CString& strArrow, double& dArrow_1, double& dS_arrow, double& dS_word, double& dev_area);
	bool modC_RING();
	//9/2
	_ConnectionPtr m_pConnectionCODE;
	BOOL c_presiz();
	CString m_valPaint_c;
	CString Trim(LPCTSTR pcs);
	_ConnectionPtr m_Pconnection;
	BOOL c_paicol();
	BOOL c_precol();
	void c_preast();
	CTotalTableIPED();
	virtual ~CTotalTableIPED();
	CString m_strExecuteSQLTblName;

protected:
	short OpenCodeDB();
	void a_yl(_RecordsetPtr sourceSet);
	void a_config(_RecordsetPtr sourceSet);
	bool TotalPreast(_RecordsetPtr sourceSet,_RecordsetPtr TargetSet);
	bool a_vertCopyE_ast(int count);
	double sum(CString str,_RecordsetPtr set);
//	void c_preast();
	BOOL totalPresiz(_RecordsetPtr sourceSet,_RecordsetPtr TargetSet);
	BOOL tableExists(_ConnectionPtr pCon,CString tbn);
	BOOL InitPresiz();
	CString m_strPaint_c;
	BOOL TotalTable(_RecordsetPtr SourceSet,_RecordsetPtr TargetSet,CString source_str,CString source_val);
	BOOL InitPaint_c();

	_RecordsetPtr pRsColr_l;	//�����ᵥλ��������
	_RecordsetPtr pRsColor;		//ɫ����ͷ������ɫ׼���
	_RecordsetPtr pRsArrow;		//ɫ����ͷ�ߴ�׼���
	_RecordsetPtr pRsRing_Paint;//ɫ����ͷ������Ͽ�
	_RecordsetPtr pRsExecSQL;	//���ڼ���SQL������ʱ��
	
};

#endif // !defined(AFX_TOTALTABLEIPED_H__D56263C0_85BA_4E9F_B52C_11D964B61BA6__INCLUDED_)
