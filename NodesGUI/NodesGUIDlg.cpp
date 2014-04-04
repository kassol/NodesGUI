
// NodesGUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NodesGUI.h"
#include "NodesGUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNodesGUIDlg �Ի���

CNodesGUIDlg::CNodesGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNodesGUIDlg::IDD, pParent)
	, pNode(NULL)
	, m_pTaskDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pNode = new node(service, 9199);
	boost::thread thrd(boost::bind(&CNodesGUIDlg::run_service, this));
}

CNodesGUIDlg::~CNodesGUIDlg()
{
	service.stop();
	delete pNode;
}

void CNodesGUIDlg::run_service()
{
	service.run();
}

void CNodesGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEAFLIST, m_ctrlAvailList);
}

BEGIN_MESSAGE_MAP(CNodesGUIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CNodesGUIDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SCAN, &CNodesGUIDlg::OnBnClickedScan)
	ON_BN_CLICKED(IDC_DISTRIBUTE, &CNodesGUIDlg::OnBnClickedDistribute)
	ON_BN_CLICKED(IDC_FEEDBACK, &CNodesGUIDlg::OnBnClickedFeedback)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LEAFLIST, &CNodesGUIDlg::OnLvnItemchangedLeaflist)
END_MESSAGE_MAP()


// CNodesGUIDlg ��Ϣ�������

BOOL CNodesGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_ctrlAvailList.SetExtendedStyle(LVS_EX_FULLROWSELECT
		|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_ctrlAvailList.InsertColumn(0, _T("IP Address"));
	m_ctrlAvailList.InsertColumn(1, _T("State"));
	m_ctrlAvailList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ctrlAvailList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNodesGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNodesGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNodesGUIDlg::OnBnClickedCancel()
{
	PostQuitMessage(0);
}


void CNodesGUIDlg::OnOK()
{

}


void CNodesGUIDlg::OnCancel()
{
	PostQuitMessage(0);
}


void CNodesGUIDlg::OnBnClickedScan()
{
	if (pNode->IsConnected() && pNode->InCharge())
	{
		pNode->Scan();
		boost::thread thrd(boost::bind(&CNodesGUIDlg::update_availlist, this));
		pNode->Ping();
	}
}


void CNodesGUIDlg::update_availlist()
{
	while(true)
	{
		UpdateAvailList();
		Sleep(1000);
	}
}

void CNodesGUIDlg::distribute()
{
	std::vector<node_struct> &avail_list = pNode->GetAvailList();
	
	if (avail_list.empty())
	{
		return;
	}
	pNode->Distribute();
}

void CNodesGUIDlg::feedback()
{
	if (!pNode->IsMaster())
	{
		pNode->Feedback();
	}
}

void CNodesGUIDlg::update_tasklist()
{
	m_pTaskDlg->UpdateTaskList(pNode);
}


void CNodesGUIDlg::UpdateAvailList()
{
	std::vector<node_struct> &avail_list = pNode->GetAvailList();
	auto ite_node = avail_list.begin();
	m_ctrlAvailList.DeleteAllItems();
	int nCount = 0;
	while(ite_node != avail_list.end())
	{
		nCount = m_ctrlAvailList.GetItemCount();
		m_ctrlAvailList.InsertItem(nCount, _T(""));
		m_ctrlAvailList.SetItemText(nCount, 0, CString(ite_node->ip_.c_str()));
		if (ite_node->is_busy)
		{
			m_ctrlAvailList.SetItemText(nCount, 1, _T("busy"));
		}
		else
		{
			m_ctrlAvailList.SetItemText(nCount, 1, _T("free"));
		}
		if (ite_node->is_checked)
		{
			m_ctrlAvailList.SetCheck(nCount);
		}
		else
		{
			m_ctrlAvailList.SetCheck(nCount, 0);
		}
		m_ctrlAvailList.SetColumnWidth(0, LVSCW_AUTOSIZE);
		m_ctrlAvailList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
		++ite_node;
	}
}


void CNodesGUIDlg::OnBnClickedDistribute()
{
	if (!pNode->IsDistributing())
	{
		USES_CONVERSION;
		wchar_t szPath[MAX_PATH];
		CString strFindPath;
		ZeroMemory(szPath, sizeof(szPath));
		BROWSEINFO bi;
		bi.hwndOwner = m_hWnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = szPath;
		bi.lpszTitle = _T("��ѡ������Ŀ¼");
		bi.ulFlags = 0;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = 0;
		LPITEMIDLIST lp = SHBrowseForFolder(&bi);
		if (lp && SHGetPathFromIDList(lp, szPath))
		{
			strFindPath.Format(_T("%s\\*.*"), szPath);
			CFileFind find;
			BOOL bf = find.FindFile(strFindPath);
			CString strTaskPath;
			while(bf)
			{
				bf = find.FindNextFile();
				strTaskPath = find.GetFilePath();
				if (strTaskPath.Right(strTaskPath.GetLength()-
					strTaskPath.ReverseFind(_T('.'))-1).CompareNoCase(_T("txt")) == 0)
				{
					pNode->AddTask(T2A(strTaskPath));
				}
			}
		}
		else
		{
			AfxMessageBox(_T("��Ч��Ŀ¼��������ѡ��"));
			return;
		}
		boost::thread thrd(boost::bind(&CNodesGUIDlg::distribute, this));
		if (m_pTaskDlg == NULL)
		{
			m_pTaskDlg = new CTaskListDlg;
			m_pTaskDlg->Create(IDD_TASKLIST);
		}
		m_pTaskDlg->ShowWindow(SW_SHOW);
		boost::thread thrd2(boost::bind(&CNodesGUIDlg::update_tasklist, this));
	}
}


void CNodesGUIDlg::OnBnClickedFeedback()
{
	if (!pNode->IsFeedback() && pNode->IsBusy())
	{
		USES_CONVERSION;
		wchar_t szPath[MAX_PATH];
		CString strFindPath;
		ZeroMemory(szPath, sizeof(szPath));
		BROWSEINFO bi;
		bi.hwndOwner = m_hWnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = szPath;
		bi.lpszTitle = _T("��ѡ������Ŀ¼");
		bi.ulFlags = 0;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = 0;
		LPITEMIDLIST lp = SHBrowseForFolder(&bi);
		if (lp && SHGetPathFromIDList(lp, szPath))
		{
			strFindPath.Format(_T("%s\\*.*"), szPath);
			CFileFind find;
			BOOL bf = find.FindFile(strFindPath);
			CString strTaskPath;
			while(bf)
			{
				bf = find.FindNextFile();
				if (!find.IsDirectory())
				{
					strTaskPath = find.GetFilePath();
					pNode->AddFeedBack(T2A(strTaskPath));
				}
			}
		}
		else
		{
			AfxMessageBox(_T("��Ч��Ŀ¼��������ѡ��"));
			return;
		}
		boost::thread thrd(boost::bind(&CNodesGUIDlg::feedback, this));
	}
	else
	{
		AfxMessageBox(_T("�ϴη���δ�������޷�������"));
	}
}


void CNodesGUIDlg::OnLvnItemchangedLeaflist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->uOldState != 0)
	{
		USES_CONVERSION;
		CString strIp = m_ctrlAvailList.GetItemText(pNMLV->iItem, 0);
		std::vector<node_struct>& leaf_list = pNode->GetAvailList();
		auto ite = std::find(leaf_list.begin(), leaf_list.end(),
			node_struct(NULL, std::string(T2A(strIp))));
		if (ite != leaf_list.end())
		{
			ite->is_checked = m_ctrlAvailList.GetCheck(pNMLV->iItem) ? 1 : 0;
		}
	}
	*pResult = 0;
}
