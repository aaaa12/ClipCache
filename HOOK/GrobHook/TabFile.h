#pragma once
#include<vector>
using namespace std;

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

public:
	virtual BOOL OnInitDialog();
public:
	CListCtrl m_list;        //列表控件 IDC_LIST
	CTreeCtrl m_tree;        //树形控件 IDC_TREE
	CImageList m_ImageList;  //图像列表(相同大小图像集合) 
	HTREEITEM m_hRoot;       //句柄 CTreeCtrl的根结点"我的电脑"
protected:

public:
	void GetLogicalDrives(HTREEITEM hParent);  //获取驱动
	void GetDriveDir(HTREEITEM hParent);       //获取子项
	afx_msg void OnItemexpandedTree(NMHDR *pNMHDR, LRESULT *pResult);
	void AddSubDir(HTREEITEM hParent);         //添加子目录
	CString GetFullPath(HTREEITEM hCurrent);   //获取树项目全根路径
	afx_msg void OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ExpendPath(HTREEITEM hItem, const vector<CString> &vPath, int p);//展开指定路径
};
