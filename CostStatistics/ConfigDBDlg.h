#pragma once


// CConfigDbServerDlg 对话框


#define  CONFIG_FILE	"config.ini"
#define  SERVER_IP		"10.53.252.207"

#define  SERVER_PORT	3360
#define  SERVER_NUMBER	0
#define  LOCAL_DB		0

class CServerCofig
{
public:
	CServerCofig() : m_strIP(SERVER_IP),m_nPort(SERVER_PORT),m_nNumber(SERVER_NUMBER),m_nLocalDb(LOCAL_DB)
	{
		char szCurPath[MAX_PATH] = {0};  
		GetModuleFileName(NULL,(LPSTR)szCurPath,sizeof(szCurPath));

		CString strPath = szCurPath;
		m_strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
		m_strPath.Append(CONFIG_FILE);
	}

	~CServerCofig()
	{
	}

	CString GetIP()
	{
		char szTemp[32] = {0};
		::GetPrivateProfileString(TEXT("SERVER"), TEXT("ip"), SERVER_IP, szTemp, MAX_PATH, m_strPath);
		m_strIP = szTemp;

		return m_strIP;
	}

	int GetPort()
	{
		m_nPort = ::GetPrivateProfileInt(TEXT("SERVER"), TEXT("port"),SERVER_PORT, m_strPath);
		return m_nPort;
	}

	int GetCode()
	{
		m_nNumber = ::GetPrivateProfileInt(TEXT("SERVER"), TEXT("servercode"),SERVER_NUMBER, m_strPath);
		return m_nNumber;
	}

	int GetLocal()
	{
		m_nNumber = ::GetPrivateProfileInt(TEXT("SERVER"), TEXT("localdb"),LOCAL_DB, m_strPath);
		return m_nNumber;
	}

	bool WriteConfig(CString strIP,CString strPort,CString strServerCode)
	{
		::WritePrivateProfileString(_T("SERVER"),_T("ip"),strIP,m_strPath);
		::WritePrivateProfileString(_T("SERVER"),_T("port"),strPort,m_strPath);
		::WritePrivateProfileString(_T("SERVER"),_T("servercode"),strServerCode,m_strPath);
		::WritePrivateProfileString(_T("SERVER"),_T("localdb"),0,m_strPath);

		return GetLastError() == 0 ? false : true;
	}

private:
	CString m_strPath;
	CString m_strIP;
	int m_nPort;
	int m_nNumber;
	int m_nLocalDb;

	CString m_strIPTmp;
	int m_nPortTmp;
	int m_nNumberTmp;
};



class CConfigDBDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDBDlg)

public:
	CConfigDBDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfigDBDlg();

// 对话框数据
	enum { IDD = IDD_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CEdit m_EditIP;
	CEdit m_EditPort;
	CEdit m_EditNumber;
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
