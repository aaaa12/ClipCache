#pragma once


// TabText 对话框

class TabText : public CDialogEx
{
	DECLARE_DYNAMIC(TabText)

public:
	TabText(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TabText();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABDLG_TXT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
