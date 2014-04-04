#pragma once
#include "afxcmn.h"
#include "node.h"


// CTaskListDlg dialog

class CTaskListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskListDlg)

public:
	CTaskListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaskListDlg();

// Dialog Data
	enum { IDD = IDD_TASKLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctrlTaskList;
	bool is_updating_list;

public:
	void UpdateTaskList(node* pNode);
	virtual BOOL OnInitDialog();
};
