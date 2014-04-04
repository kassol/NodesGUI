// TaskListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NodesGUI.h"
#include "TaskListDlg.h"
#include "afxdialogex.h"


// CTaskListDlg dialog

IMPLEMENT_DYNAMIC(CTaskListDlg, CDialogEx)

CTaskListDlg::CTaskListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskListDlg::IDD, pParent)
	, is_updating_list(false)
{

}

CTaskListDlg::~CTaskListDlg()
{
}

void CTaskListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlTaskList);
}


BEGIN_MESSAGE_MAP(CTaskListDlg, CDialogEx)
END_MESSAGE_MAP()


// CTaskListDlg message handlers


void CTaskListDlg::UpdateTaskList(node* pNode)
{
	if (is_updating_list)
	{
		return;
	}
	is_updating_list = true;
	std::vector<task_struct>& task_list = pNode->GetTaskList();
	int nCount = 0;
	while(is_updating_list && !task_list.empty())
	{
		m_ctrlTaskList.DeleteAllItems();
		auto ite = task_list.begin();
		while(ite != task_list.end())
		{
			nCount = m_ctrlTaskList.GetItemCount();
			m_ctrlTaskList.InsertItem(nCount, _T(""));
			m_ctrlTaskList.SetItemText(nCount, 0, CString(ite->task_.c_str()));
			if (ite->state_ == 0)
			{
				m_ctrlTaskList.SetItemText(nCount, 1, _T("Free"));
			}
			else if (ite->state_ == 1)
			{
				m_ctrlTaskList.SetItemText(nCount, 1, _T("Sent"));
			}
			else if (ite->state_ == 2)
			{
				m_ctrlTaskList.SetItemText(nCount, 1, _T("Finished"));
			}
			m_ctrlTaskList.SetColumnWidth(0, LVSCW_AUTOSIZE);
			m_ctrlTaskList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
			++ite;
		}
		Sleep(1000);
	}
	is_updating_list = false;
}


BOOL CTaskListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ctrlTaskList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ctrlTaskList.InsertColumn(0, _T("Task"));
	m_ctrlTaskList.InsertColumn(1, _T("State"));
	m_ctrlTaskList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ctrlTaskList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
