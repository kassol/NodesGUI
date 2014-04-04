
// NodesGUIDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "node.h"
#include "TaskListDlg.h"


// CNodesGUIDlg �Ի���
class CNodesGUIDlg : public CDialogEx
{
// ����
public:
	CNodesGUIDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CNodesGUIDlg();

// �Ի�������
	enum { IDD = IDD_NODESGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	node* pNode;
	boost::asio::io_service service;
	CTaskListDlg* m_pTaskDlg;

public:
	void UpdateAvailList();

private:
	void run_service();
	void update_availlist();
	void distribute();
	void feedback();
	void update_tasklist();

public:
	CListCtrl m_ctrlAvailList;
	afx_msg void OnBnClickedCancel();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedScan();
	afx_msg void OnBnClickedDistribute();
	afx_msg void OnBnClickedFeedback();
	afx_msg void OnLvnItemchangedLeaflist(NMHDR *pNMHDR, LRESULT *pResult);
};
