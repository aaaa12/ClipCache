#pragma once
#include "TabText.h"
#include "TabFile.h"

// TabMain 对话框

class TabMain : public CDialogEx
{
	DECLARE_DYNAMIC(TabMain)

public:
	TabMain(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TabMain();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABDLG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	TabFile m_tabFile;
	TabText m_tabText;
	void SetCtlRect();
	bool bResize=false;
public:
	virtual BOOL OnInitDialog();
private:
	CTabCtrl m_tab;
	void SwitchTab(int iCode);
public:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetTxtExpendModel(int flag);
};
