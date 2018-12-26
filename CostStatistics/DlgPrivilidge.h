#pragma once
#include <vector>
#include "resource.h"


using namespace std;

struct RoleInfo
{
	int value;
	CString name;
};

struct UserRole
{
	bool bManager;
	int userID;
	CString userName;
	vector<int> arrRoles;
};

typedef enum _enumPrivilidgeType 
{
	EN_PRIVILIDGE_NULL			= 0,	//��ʼֵ
	EN_PRIVILIDGE_APPROVAL		= 1,	//����Ȩ������
	EN_PRIVILIDGE_TABLEDISPLAY	= 2,	//tabҳ����
	EN_PRIVILIDGE_CONTRACT_MODAL = 3	//��ͬ����ģ��鿴
}EN_PRIVILIDGE_TYPE;


bool JudgePrivilidge(int userID, CString modalKey,int nPrivilidge);
bool IsAdmin(int nUserID);
class CDlgPrivilidge : public CDialog
{
	DECLARE_DYNAMIC(CDlgPrivilidge)

public:
	CDlgPrivilidge(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPrivilidge();
	
// �Ի�������
	enum { IDD = IDD_DLGPRIVILIDGE };
	

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void OnListClick(NMHDR* p,LRESULT* pL);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CString m_strModal;
	CTreeListCtrl m_treeList;
	bool m_bHaveModifyed;
	std::vector<UserRole> m_arrUser;
	CMap<int,int&,int,int&> m_map2Prievilidge;
	void InitList(CTreeListCtrl& listCtrl,int id);
	void LoadBindGroup();
	void OnListDbClick(NMHDR* p,LRESULT* pL);
	DECLARE_MESSAGE_MAP()
public:
	void SetModalKey(CString strModal);
	void SetType(EN_PRIVILIDGE_TYPE enType = EN_PRIVILIDGE_APPROVAL);

	void GetPrivlilidgeUserInfo(CString strMutex,std::vector<UserRole>& vecInfo);
	bool IsUserAdmin(int nUserID);

	void GetManagerUserID(CString strMutex,std::vector<int>& vecMgrID);
	void GetManagerUserInfo(CString strMutex,std::vector<UserRole>& vecMgrInfo);

	std::vector<int> GetApprovalLevelFromUser(CString strMutex,int nUserID);
private:
	int ExcuteBatchSQL(CStringArray& arrSQL);

public:
	CArray<RoleInfo> m_arrRoles;

private:
	EN_PRIVILIDGE_TYPE m_enPrililidgeType;
	
};
