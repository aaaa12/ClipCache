#pragma once


// TabHistroy 对话框

class TabHistroy : public CDialogEx
{
	DECLARE_DYNAMIC(TabHistroy)

public:
	TabHistroy(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TabHistroy();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABDLG_HISTROY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
