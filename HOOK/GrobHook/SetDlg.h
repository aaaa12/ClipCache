#pragma once


// SetDlg 对话框

class SetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetDlg)

public:
	SetDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
