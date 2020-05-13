// TabText.cpp: 实现文件
//

#include "pch.h"
#include "TabText.h"
#include "GrobHook.h"
#include "afxdialogex.h"
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib") //如果没有这行，会出现link错误
// TabText 对话框

IMPLEMENT_DYNAMIC(TabText, CDialogEx)

TabText::TabText(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TABDLG_TXT, pParent)
{

}

TabText::~TabText()
{
}

void TabText::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_EDT_TEXT, m_text);
}


BEGIN_MESSAGE_MAP(TabText, CDialogEx)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE, &TabText::OnItemexpandedTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &TabText::OnSelchangedTree)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_OK, &TabText::OnBnClickedBtnOk)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, &TabText::OnNMDblclkTree)
END_MESSAGE_MAP()


// TabText 消息处理程序


//函数功能:获取驱动器 参数:路径名                       
void TabText::GetLogicalDrives(HTREEITEM hParent)
{
	//获取系统分区驱动器字符串信息
	size_t szAllDriveStrings = GetLogicalDriveStrings(0, NULL);           //驱动器总长度
	char *pDriveStrings = new char[szAllDriveStrings + sizeof(_T(""))];  //建立数组
	GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
	size_t szDriveString = strlen(pDriveStrings);                        //驱动大小
	while (szDriveString > 0)
	{
		CString sItem = pDriveStrings;//去掉'\'
		sItem = sItem.Left(2);
		m_tree.InsertItem(sItem, hParent);       //在父节点hParent添加盘符
		pDriveStrings += szDriveString + 1;             //pDriveStrings即C:\ D:\ E:\盘
		szDriveString = strlen(pDriveStrings);
	}

	//free(pDriveStrings);//位置已经偏移，所以free比较麻烦，算了

}

//函数功能:获取驱动盘符下所有子项文件夹
void TabText::GetDriveDir(HTREEITEM hParent)
{
	HTREEITEM hChild = m_tree.GetChildItem(hParent);   //获取指定位置中的子项
	while (hChild)
	{
		CString strText = m_tree.GetItemText(hChild);  //检索列表中项目文字
		if (strText.Right(1) != "\\")                   //从右边1开始获取从右向左nCount个字符
			strText += _T("\\");
		strText += "*.*";
		//将当前目录下文件枚举并InsertItem树状显示
		CFileFind file;                                       //定义本地文件查找
		BOOL bContinue = file.FindFile(strText);              //查找包含字符串的文件
		while (bContinue)
		{
			bContinue = file.FindNextFile();                  //查找下一个文件
			if (/*file.IsDirectory() && */!file.IsDots())          //找到文件为内容且不为点"."
				m_tree.InsertItem(file.GetFileName(), hChild); //添加盘符路径下树状文件夹
		}
		GetDriveDir(hChild);                                  //递归调用
		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);        //获取树形控件(TVGN_NEXT)下一兄弟项
	}
}


//函数功能:展开事件函数
void TabText::OnItemexpandedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	TVITEM item = pNMTreeView->itemNew;                  //发送\接受关于树形视图项目信息
	//if (item.hItem == m_hRoot)
		//return;
	HTREEITEM hChild = m_tree.GetChildItem(item.hItem);  //获取指定位置中的子项
	while (hChild)
	{
		tool->AddSubDirToTree(hChild, &m_tree, &m_TreeImageList);                               //添加子目录
		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);   //获取树形控件TVGN_NEXT下兄弟项
	}
	*pResult = 0;
}


//函数功能:添加子目录
void TabText::AddSubDir(HTREEITEM hParent)
{
	CString strPath = GetFullPath(hParent);     //获取全路径
	//CString strPath = m_tree.GetItemText(hParent);
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);    //查找包含字符串的文件

	//先删除
	HTREEITEM hChild = m_tree.GetNextItem(hParent, TVGN_CHILD);
	while (hChild)
	{
		HTREEITEM hNextChild = m_tree.GetNextItem(hChild, TVGN_NEXT);
		m_tree.DeleteItem(hChild);
		hChild = hNextChild;

	}

	while (bContinue)
	{
		bContinue = file.FindNextFile();        //查找下一个文件
		if (/*file.IsDirectory() &&*/ !file.IsDots())
			m_tree.InsertItem(file.GetFileName(), hParent);

		//MessageBox(file.GetFileName());


	}
}



//函数功能:获取树项目全根路径
CString TabText::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp;
	CString strReturn = "";
	while (hCurrent)
	{
		strTemp = m_tree.GetItemText(hCurrent);    //检索列表中项目文字
		if (strTemp.Right(1) != "\\")
			strTemp += "\\";
		strReturn = strTemp + strReturn;
		hCurrent = m_tree.GetParentItem(hCurrent); //返回父项目句柄
	}
	return strReturn;
}


//函数功能:选中事件显示图标
void TabText::OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码

	ShowText(GetTreePath());

	*pResult = 0;
}

void TabText::OnBnClickedBtnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetParent()->GetParent()->ShowWindow(SW_HIDE);
	//CString source = ReadStrFromFile(GetTreePath());
	////put your text in source 
	//if (OpenClipboard())
	//{
	//	HGLOBAL clipbuffer;
	//	char * buffer;
	//	EmptyClipboard();
	//	char* src = source.GetBuffer();
	//	int a = strlen(src) + 1;
	//	clipbuffer = GlobalAlloc(GMEM_DDESHARE, a);
	//	buffer = (char*)GlobalLock(clipbuffer);
	//	strcpy(buffer, src);
	//	GlobalUnlock(clipbuffer);
	//	SetClipboardData(CF_TEXT, clipbuffer);
	//	CloseClipboard();
	//}

}
BOOL TabText::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	tool = CTool::get_instance();

	m_TreeImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 10, 30);
	HICON hIcon = theApp.LoadIcon(IDI_ICON1);        //图标句柄
	m_TreeImageList.Add(hIcon);                          //图标添加到图像序列
	m_tree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS);
	m_tree.SetImageList(&m_TreeImageList, LVSIL_NORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void TabText::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rc, rcLeft, rcRight;
	this->GetClientRect(&rc);

	int midPos = 250;

	//调整子对话框在父窗口中的位置 
	rcLeft.top = rc.top;
	rcLeft.bottom = rc.bottom - 30;
	rcLeft.left = rc.left;
	rcLeft.right = rc.left + midPos;

	rcRight.top = rc.top;
	rcRight.bottom = rc.bottom - 30;
	rcRight.left = rc.left + midPos;
	rcRight.right = rc.right;

	////设置子对话框尺寸并移动到指定位置 
	if (m_tree.m_hWnd)
		m_tree.MoveWindow(&rcLeft);
	if (m_text.m_hWnd)
		m_text.MoveWindow(&rcRight);

}

void TabText::ExpendPath(HTREEITEM hItem, const vector<CString> &vPath, int p)
{
	if (vPath.size() <= p)
		return;

	while (hItem)
	{
		if (m_tree.GetItemText(hItem) == vPath[p])
		{
			m_tree.Expand(hItem, TVE_EXPAND);
			HTREEITEM hChildItem = m_tree.GetNextItem(hItem, TVGN_CHILD);

			ExpendPath(hChildItem, vPath, p + 1);
		}

		hItem = m_tree.GetNextItem(hItem, TVGN_NEXT);

	}
}

CString TabText::ReadStrFromFile(CString path)
{
	CString ret;
	char * buffer;
	FILE * f;
	long length;
	//以二进制形式打开文件	
	f = fopen(path, "rb");
	if (NULL == f)
	{
		return ret;
	}
	//把文件的位置指针移到文件尾	
	fseek(f, 0, SEEK_END);
	//获取文件长度;	
	length = ftell(f);
	//把文件的位置指针移到文件开头	
	fseek(f, 0, SEEK_SET);
	buffer = (char *)malloc((length + 1) * sizeof(char));
	fread(buffer, 1, length, f);
	buffer[length] = '\0';
	fclose(f);
	ret.Format("%s", buffer);

	free(buffer);

	return ret;
}
void TabText::ShowText(CString sFullName)
{

	if (sFullName.Find(".txt") == -1)
	{
		m_text.SetWindowText("仅展示txt文件");
	}
	else
	{
		CString str = ReadStrFromFile(sFullName);
		m_text.SetWindowText(str);
	}

}

vector<CString> TabText::SplitCString(CString strSrc, CString strGap)
{
	//Name.GetBuffer()会导致CString::left()不起作用
	//Names.ReleaseBuffer();          // 添加这个函数
	vector<CString> ret;
	int nPos = strSrc.Find(strGap);

	CString strLeft="";
	while (0 <= nPos) {
		strLeft = strSrc.Left(nPos);
		if (!strLeft.IsEmpty())
			ret.push_back(strLeft);
		strSrc = strSrc.Right(strSrc.GetLength() - nPos - 1);
		nPos = strSrc.Find(strGap);
	}
	if (!strSrc.IsEmpty())
	{
		ret.push_back(strSrc);
	}
	return ret;
}


//#define EXPEND_HISTROY 1
//#define EXPEND_COLLECT 2
void TabText::SetExpendModel(int flag)
{

	CString sDir;
	if (EXPEND_HISTROY == flag)
	{
		GetPrivateProfileString("Set", "TempPath",
			CString("NULL"), sDir.GetBuffer(MAX_PATH), MAX_PATH, "setting.ini");
		//SetDlgItemText(IDC_EDIT_TEMP, sDir);
	}
	else if (EXPEND_COLLECT == flag)
	{
		GetPrivateProfileString("Set", "CollectPath",
			CString("NULL"), sDir.GetBuffer(MAX_PATH), MAX_PATH, "setting.ini");
		//SetDlgItemText(IDC_EDIT_COLLECT, sDir);
	}
	sDir.ReleaseBuffer();
	tool->BuildTree(sDir, &m_tree, &m_TreeImageList);

	/*vector<CString> v = SplitCString(sDir,"\\");
	HTREEITEM hChildItem = m_tree.GetNextItem(m_hRoot, TVGN_CHILD);
	ExpendPath(hChildItem, v, 0);*/

}

CString TabText::GetTreePath()
{
	HTREEITEM hSelectItem = m_tree.GetSelectedItem();
	CString sFullName = tool->GetPathFromTree(hSelectItem, &m_tree);

	return sFullName;
}

void TabText::CollapseTree(HTREEITEM hItem)
{
	HTREEITEM hChild, hNext, hParent;

	if ((!hItem) || (!m_tree))
		return;
	//存在则收起节点
	m_tree.Expand(hItem, TVE_COLLAPSE);

	hChild = m_tree.GetNextItem(hItem, TVGN_CHILD);
	if (hChild)
	{//如果有子节点，收起子节点
		CollapseTree(hChild);
	}
	else
	{//没有子节点，寻找其兄弟节点
		hNext = m_tree.GetNextItem(hItem, TVGN_NEXT);
		if (hNext)
		{//存在兄弟节点
			CollapseTree(hNext);
		}
		else
		{//不存在兄弟节点，继续找父节点
			hParent = m_tree.GetParentItem(hItem);
			hNext = m_tree.GetNextItem(hParent, TVGN_NEXT);
			if (hNext)
				CollapseTree(hNext);
		}
	}
}

void TabText::OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSelectItem = m_tree.GetSelectedItem();

	
	CString sFullName=tool->GetPathFromTree(hSelectItem,&m_tree);

	if (sFullName.Find(".txt") == -1)
	{
		MessageBox("非txt文件");
		return;
	}

	CString txt = ReadStrFromFile(sFullName);

	//写入剪切板
	if (OpenClipboard())
	{
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		char* src = txt.GetBuffer();
		int a = strlen(src) + 1;
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, a);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, src);
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
	}
	this->GetParent()->GetParent()->ShowWindow(SW_HIDE);
	*pResult = 0;
}
