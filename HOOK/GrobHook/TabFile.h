#pragma once


// TabFile 对话框

class TabFile : public CDialogEx
{
	DECLARE_DYNAMIC(TabFile)

public:
	TabFile(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TabFile();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABDLG_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
