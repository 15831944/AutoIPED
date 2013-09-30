#if !defined(AFX_EDITORIGINALDATA_H__FC6BD122_A151_4D62_A00D_9FB54575FFAC__INCLUDED_)
#define AFX_EDITORIGINALDATA_H__FC6BD122_A151_4D62_A00D_9FB54575FFAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditOriginalData.h : header file
//

#include "MyButton.h"
#include "Foxbase.h"
#include "DlgParameterCongeal.h"
#include "DlgParameterSubterranean.h"
#include "GetPropertyofMaterial2.h"

/////////////////////////////////////////////////////////////////////////////
// CEditOriginalData dialog

//////////////////////////////////////////////
//
// "����ԭʼ���ݱ༭"�Ի���
//
// δ���²��ϴ���ɲ���
//
//
// �����Ի���ǰ��Ҫ����
// SetCurrentProjectConnect	���ù������ݿ�
// SetPublicConnect			���ù����ο����ݿ�
// SetProjectID				���ù�����
//
////////////////////////////////////////////////

class CEditOriginalData : public CDialog,public CFoxBase
{
// Construction 
public:
	CEditOriginalData(CWnd* pParent = NULL);   // standard constructor

public: 
	_ConnectionPtr m_IPtrShareMaterial;
	void UpdateControlSubterranean(int nPipeCount);
	virtual  ~CEditOriginalData();
	BOOL AddValveData();
	bool AutoSelAllMat();
	// �����ڹ���ʱ�Ƿ��Զ�ѡ��
	void SetIsAutoSelectMatOnRoll(BOOL bSelect);
	// �����Ƿ��ڹ���ʱ�Զ�ѡ��
	BOOL GetIsAutoSelectMatOnRoll();

	//������ѡ���̵����ݿ������
	void SetCurrentProjectConnect(_ConnectionPtr &IConnection);
	//�������ݿ����ӵ�����ָ�������
	_ConnectionPtr& GetCurrentProjectConnect();

	//���ù��������ݿ������
	void SetPublicConnect(_ConnectionPtr& IConnection);
	// ���ع������ݿ����ӵ�����ָ�������
	_ConnectionPtr& GetPublicConnect();

	//������ѡ���̵�ID��
	void SetProjectID(LPCTSTR ProjectID);
	// ������ѡ���̵�ID��
	CString& GetProjectID();

// Dialog Data
	//{{AFX_DATA(CEditOriginalData)
	enum { IDD = IDD_EDIT_ORIGINAL_DATA };
	CButton	m_ctlTitleTab;
	CComboBox	m_ctlPipeMedium;
	CComboBox	m_ctlCalcMethod;
	CComboBox	m_ctlHeatTran;		//����ϵ��ȡֵ������.
	CComboBox	m_ctlEnvTemp;		//�����¶ȵ�ȡֵ����
	CMyButton	m_DeleteAll;
	CComboBox	m_ColorRing;
	CMyButton	m_SaveCurrent;
	CMyButton	m_DelCurrent;
	CMyButton	m_AddNew;
	CMyButton	m_MoveLast;
	CMyButton	m_MoveSubsequent;
	CMyButton	m_MovePrevious;
	CMyButton	m_MoveForefront;
	CComboBox	m_SafeguardMatName;
	CComboBox	m_OutSizeMatName;
	CComboBox	m_InsideMatName;
	CListCtrl	m_HeatPreservationTypeList;
	CListCtrl	m_VolList;
	CComboBox	m_PipeMat;
	CComboBox	m_BuidInPlace;
	CComboBox	m_PipeThick;
	CComboBox	m_PipeSize;
	CComboBox	m_PipeDeviceName;
	CString	m_VolNo;
	int		m_ID;
	CString	m_ReMark;
	BOOL	m_IsVolListEnable;
	BOOL	m_IsHeatPreservationTypeEnable;
	double	m_WideSpeed;
	double	m_PriceRatio;
	double	m_RunPerHour;
	double	m_MediumTemperature;
	BOOL	m_IsUpdateByRoll;
	double	m_Amount;
	BOOL	m_bIsAutoSelectMatOnRoll;
	CString	m_dHeatLoss;
	CString	m_dHeatDensity;
	double	m_dThick1;		//�ڲ㱣�º�
	double	m_dThick2;		//��㱣�º�
	double	m_dTa;			//�����¶�
	double	m_dTs;			//������¶�
	double	m_dPressure;	//�����¶�
	BOOL	m_bIsCalInThick; 
	BOOL	m_bIsCalPreThick;
	double	m_dWindPlaimWidth;
	double	m_dBoilerPressure;		//��ù�¯���ڹ���������ѹ��
	double  m_dBoilerTemperature;	//��¯���ڹ��������¶�
	double	m_dCoolantTemperature;	//��ȴˮ�¶�
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditOriginalData)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DECLARE_DYNCREATE(CEditOriginalData)

	BOOL InitToolTip();
	BOOL RenewNumberFind(_RecordsetPtr& pRs,long ID,long FindNO);
	short PutDataToPreventCongealDB(long nID=-1);
	short GetDataToPreventCongealControl();
	short ShowWindowRect();
	short AutoChangeValue();
	bool CheckDataValidity();
	short GetSiteIndex(_RecordsetPtr& pRs,short nIndexv, short& SiteIndex);
	short UpdateSite(short SiteIndex);
	short InitPipeMedium();
	short InitCalcMethod();
	int GetConditionTemp(double& dConTemp, int & nMethodIndex,int nIndex=-1);
	bool UpdateHeatTransferCoef(CString strFilter="");			
	bool UpdateEnvironmentComBox(CString strFilter="");		
	virtual void OnCancel(void);

	// Generated message map functions
	//{{AFX_MSG(CEditOriginalData)
	virtual BOOL OnInitDialog();
	afx_msg void OnForefront();
	afx_msg void OnPrevious();
	afx_msg void OnSubsequent();
	afx_msg void OnLast();
	afx_msg void OnAddNew();
	afx_msg void OnDelCurrent();
	afx_msg void OnSaveCurrent();
	afx_msg void OnDropdownColor();
	afx_msg void OnDropdownPipeDeviceName();
	afx_msg void OnDropdownPipeSize();
	afx_msg void OnDropdownPipeThick();
	afx_msg void OnDropdownInsideMatname();
	afx_msg void OnDropdownOutsizeMatname();
	afx_msg void OnDropdownSafeguardMatname(); 
	afx_msg void OnItemchangedHeatPreservationType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedVollist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateAllCheckBox();
	afx_msg void OnChangeVolno();
	afx_msg void OnAutoSelectmat();
	afx_msg void OnChangeId();
	afx_msg void OnSortByVol();
	afx_msg void OnExit();
	afx_msg void OnDelAllInsu();
	afx_msg void OnCheckMethod();
	afx_msg void OnCheckTa();
	afx_msg void OnCheckThick1();
	afx_msg void OnCheckThick2();
	afx_msg void OnCheckTs();
	afx_msg void OnSelchangeEnvironTemp();
	afx_msg void OnSelchangeHeatTransferCoef();
	afx_msg void OnCheckEnvTemp();
	afx_msg void OnCheckAlpha();
	afx_msg void OnSelchangeEditMethod();
	afx_msg void OnSelchangeBuildinPlace();
	afx_msg void OnSelchangePipeMedium();
	afx_msg void OnButtonAddValve();
	afx_msg void OnButtonCalcRatio();
	afx_msg void OnSelchangePipeMat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
	BOOL InitCurrentProjectRecordset();

private:
	void AUTO(CString &v,CString &s,CString &t,CString &pre,CString &pro,CString &p,CString &pre_in);
	void cond(_RecordsetPtr &IRecordset, CString &mat,CString &v,CString &p,CString t,CString d,CString h=_T(""));
	void compound(CString &mat_in,CString &mat);
	BOOL Refresh();

	//��ʼ��λͼ��Ť
	BOOL InitBitmapButton();

	//��ʼ����������LIST�ؼ� 
	BOOL InitHeatPreservationTypeList();

	//��ʼ�����LIST�ؼ� 
	BOOL InitVolListControl();

	//���Դ򿪵ļ�¼���ĵ�ǰ�α����ڵ�λ��ȡ���ݵ������ؼ�
	BOOL PutDataToEveryControl();

	//�Ӹ����ؼ�ȡ���ݵ����ݿ�
	BOOL PutDataToDatabaseFromControl(long nID=-1);

	//����������Ͽ������
	BOOL UpdateEveryComboBox();

	//���¡�ɫ������Ͽ�
	BOOL UpdateColorComBox();

	//���¡��ܵ��豸���ơ���Ͽ�
	BOOL UpdatePipeDeviceNameComBox();

	//���¡��ܵ��⾶�����Ͽ�
	BOOL UpdatePipeSizeComBox();

	//���¡��ܵ��ں���Ͽ�
	BOOL UpdatePipeThickComBox();

	//���¡���װ�ص㡱��Ͽ�
	BOOL UpdateBuildPlaceComBox();

	//���¡��ڱ��²�������ơ���Ͽ�
	BOOL UpdateInsideMatNameCombox();

	//����"�������������"��Ͽ�
	BOOL UpdateSafeguardMatNameComBox();

	//���¡��Ᵽ�²�������ơ���Ͽ�
	BOOL UpdateOutSizeMatNameComBox();

	//����a_vol���Ҳ����Ի����еľ���ʱ,����a_vol��
	BOOL UpdateVolTable();

	//���¸����ؼ���״̬
	void UpdateControlsState();


private:
	void UpdateMediumMaterialDensity();
	BOOL NumberSubtractOne(_RecordsetPtr& pRs, long lStartPos);
	void PutDataToSubterraneanDB(long nID=-1);
	void AddDynamicDlg(CDialog *pDlg, UINT nID);
	BOOL GetAeValue(CString strTypeName, double &dAe);
	double m_dValveSpace;
	CDlgParameterCongeal* m_pDlgCongeal; // ����Ĳ�������Ի���
	CDlgParameterSubterranean* m_pDlgSubterranean; // ��عܵ����¼���Ĳ���������Ի���
	CDialog* m_pDlgCurChild;		// ָ��ǰ��չ��һ���ӶԻ���

	GetPropertyofMaterial* m_pMatProp;

	double m_dMaxD0;					//�ܵ�������⾶��
	BOOL InitConstant();
	BOOL m_bIsPlane;					//��ǰ��¼���豸�Ƿ���ƽ��
//	BOOL m_bIsAutoSelectMatOnRoll;
	CToolTipCtrl m_pctlTip;				//��ʾ��Ϣ.
 
	CString m_ProjectID;							//���̵�ID��
	_RecordsetPtr m_ICurrentProjectRecordset;		//ԭʼ���ݱ�.
	_RecordsetPtr m_pRsCongealData;					//����������ԭʼ���ݱ�
	_RecordsetPtr m_pRsSubterranean;				// ��عܵ���ԭʼ���ݱ�
	_RecordsetPtr m_pRsEnvironment;					//�����¶�ȡֵ������
	_RecordsetPtr m_pRsHeat;						//����ϵ��ȡֵ������
 
	_ConnectionPtr m_IPublicConnection; 				//�������ݿ������
	_ConnectionPtr m_ICurrentProjectConnection;		//��ѡ���̵����ݿ������

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORIGINALDATA_H__FC6BD122_A151_4D62_A00D_9FB54575FFAC__INCLUDED_)
