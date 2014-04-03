
// NodesGUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NodesGUI.h"
#include "NodesGUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNodesGUIDlg 对话框

CNodesGUIDlg::CNodesGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNodesGUIDlg::IDD, pParent)
	, pNode(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pNode = new node(service, 9199);
	boost::thread thrd(boost::bind(&CNodesGUIDlg::run_service, this));
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
END_MESSAGE_MAP()


// CNodesGUIDlg 消息处理程序

BOOL CNodesGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_ctrlAvailList.SetExtendedStyle(LVS_EX_FULLROWSELECT
		|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_ctrlAvailList.InsertColumn(0, _T("IP Address"));
	m_ctrlAvailList.InsertColumn(1, _T("State"));
	m_ctrlAvailList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ctrlAvailList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNodesGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	if (pNode->IsConnected())
	{
		pNode->Scan();
		boost::thread thrd(boost::bind(&CNodesGUIDlg::update_availlist, this));
	}
}


void CNodesGUIDlg::update_availlist()
{
	while(!pNode->IsScanFinished())
	{
		UpdateAvailList();
		Sleep(1000);
	}
	UpdateAvailList();
}

void CNodesGUIDlg::distribute()
{
	std::vector<node_struct> &avail_list = pNode->GetAvailList();
	
	if (avail_list.empty())
	{
		return;
	}
	auto ite_aval = avail_list.begin();
	while(ite_aval != avail_list.end())
	{
		boost::thread thrd(boost::bind(&node::Distribute, pNode,
			ite_aval->long_session_, ite_aval->ip_));
		++ite_aval;
	}
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
		++ite_node;
	}
}


void CNodesGUIDlg::OnBnClickedDistribute()
{
	USES_CONVERSION;
	wchar_t szPath[MAX_PATH];
	CString strFindPath;
	ZeroMemory(szPath, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("请选择任务目录");
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
		AfxMessageBox(_T("无效的目录，请重新选择"));
		return;
	}

	boost::thread thrd(boost::bind(&CNodesGUIDlg::distribute, this));
}


void CNodesGUIDlg::OnBnClickedFeedback()
{
	
}
