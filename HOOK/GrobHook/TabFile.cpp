// TabFile.cpp: 实现文件
//

#include "pch.h"
#include "TabFile.h"
#include "GrobHook.h"
#include "afxdialogex.h"


// TabFile 对话框

IMPLEMENT_DYNAMIC(TabFile, CDialogEx)

TabFile::TabFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TABDLG_FILE, pParent)
{

}

TabFile::~TabFile()
{
}

void TabFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_TREE, m_tree);
}


BEGIN_MESSAGE_MAP(TabFile, CDialogEx)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE, &TabFile::OnItemexpandedTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &TabFile::OnSelchangedTree)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_TOCLIP, &TabFile::OnBnClickedBtnToclip)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, &TabFile::OnNMDblclkTree)
END_MESSAGE_MAP()


// TabFile 消息处理程序
void TabFile::ExpendPath(HTREEITEM hItem, const vector<CString> &vPath, int p)
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

BOOL TabFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	tool = CTool::get_instance();

	m_ImageList.Create(16, 16, ILC_COLOR32, 10, 30);     //创建图像序列CImageList对象 
	m_TreeImageList.Create(16, 16, ILC_COLOR32|ILC_MASK, 10, 30);
	HICON hIcon = theApp.LoadIcon(IDI_ICON1);        //图标句柄
	m_ImageList.Add(hIcon);                          //图标添加到图像序列
	m_list.SetImageList(&m_ImageList, LVSIL_SMALL);  //为树形控件设置图像序列LVSIL_SMALL给iconsmall用   
	m_tree.SetImageList(&m_TreeImageList, LVSIL_NORMAL);
	m_tree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS);
	//m_hRoot = m_tree.InsertItem("我的电脑");         //插入根节点
	//GetLogicalDrives(m_hRoot);                       //自定义函数 获取驱动
	//GetDriveDir(m_hRoot);                            //自定义函数 获取驱动子项
	//m_tree.Expand(m_hRoot, TVE_EXPAND);               //展开或折叠子项列表 TVE_EXPAND展开列表  

	//vector<CString> v;
	//v.push_back("D:");
	//v.push_back("EFS");
	//HTREEITEM hChildItem = m_tree.GetNextItem(m_hRoot, TVGN_CHILD);
	//ExpendPath(hChildItem, v, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//函数功能:获取驱动器 参数:路径名                       
void TabFile::GetLogicalDrives(HTREEITEM hParent)
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
void TabFile::GetDriveDir(HTREEITEM hParent)
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
			if (file.IsDirectory() && !file.IsDots())          //找到文件为内容且不为点"."
				m_tree.InsertItem(file.GetFileName(), hChild); //添加盘符路径下树状文件夹
		}
		GetDriveDir(hChild);                                  //递归调用
		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);        //获取树形控件(TVGN_NEXT)下一兄弟项
	}
}


//函数功能:展开事件函数
void TabFile::OnItemexpandedTree(NMHDR *pNMHDR, LRESULT *pResult)
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
void TabFile::AddSubDir(HTREEITEM hParent)
{
	CString strPath = GetFullPath(hParent);     //获取全路径
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);    //查找包含字符串的文件
	while (bContinue)
	{
		bContinue = file.FindNextFile();        //查找下一个文件
		if (file.IsDirectory() && !file.IsDots())
			m_tree.InsertItem(file.GetFileName(), hParent);
	}
}



//函数功能:获取树项目全根路径
CString TabFile::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp;
	CString strReturn = "";
	while (hCurrent != m_hRoot)
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
void TabFile::OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	m_list.DeleteAllItems();
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hSelectItem = m_tree.GetSelectedItem();
	CString sFullName = tool->GetPathFromTree(hSelectItem, &m_tree);
	SetDlgItemText(IDC_EDT_PATH, sFullName);
	sFullName += "\\*.*";

	CFileFind file;
	BOOL bContinue = file.FindFile(sFullName);
	while (bContinue)
	{
		bContinue = file.FindNextFile();
		if (/*!file.IsDirectory() && */!file.IsDots())
		{
			SHFILEINFO info;
			CString temp = sFullName;
			int index = temp.Find("*.*");
			temp.Delete(index, 3);
			SHGetFileInfo(temp + file.GetFileName(), 0, &info, sizeof(&info), SHGFI_DISPLAYNAME | SHGFI_ICON);
			int i = m_ImageList.Add(info.hIcon);
			m_list.InsertItem(i, info.szDisplayName, i);
		}
	}

	//SetPathCtl();

	*pResult = 0;
}


void TabFile::OnSize(UINT nType, int cx, int cy)
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
	if (m_list.m_hWnd)
		m_list.MoveWindow(&rcRight);

}


void TabFile::OnBnClickedBtnToclip()
{
	// TODO: 在此添加控件通知处理程序代码
	DirIntoClipBoard(GetPathCtl());

}

void TabFile::SetPathCtl()
{
	//MessageBox("11");
	CString sDir;
	HTREEITEM hSelectItem = m_tree.GetSelectedItem();

	sDir = m_tree.GetItemText(hSelectItem);

	HTREEITEM hParentItem = m_tree.GetParentItem(hSelectItem);
	while (hParentItem)
	{
		if ("我的电脑" == m_tree.GetItemText(hParentItem))
			break;
		sDir = m_tree.GetItemText(hParentItem) + "\\" + sDir;
		hParentItem = m_tree.GetParentItem(hParentItem);
	}

	SetDlgItemText(IDC_EDT_PATH, sDir);
}

CString TabFile::GetPathCtl()
{
	CString ret;
	GetDlgItemText(IDC_EDT_PATH, ret);
	return ret;
}

void TabFile::DirIntoClipBoard(CString sDir)
{
	//要复制剪切的文档或者文件夹
	char *lpBuffer = sDir.GetBuffer();
	UINT uBufLen = strlen(lpBuffer);

	//true拷贝，false剪切
	bool bCopy = true;

	UINT uDropEffect = 0;
	DROPFILES dropFiles = { 0 };
	UINT uGblLen = 0;
	UINT uDropFilesLen = 0;
	HGLOBAL hGblFiles;
	HGLOBAL hGblEffect;
	char * szData = NULL;
	char * szFileList = NULL;
	DWORD * dwDropEffect = NULL;

	///////////////////////////自定义剪切板，用来设置标识（复制还是剪切）/////////////////////////

	uDropEffect = RegisterClipboardFormat("Preferred DropEffect");    //参数随便填

	hGblEffect = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(DWORD));
	dwDropEffect = (DWORD*)GlobalLock(hGblEffect);

	//设置自定义剪切板的内容为复制或者剪切标识
	if (bCopy)
	{
		*dwDropEffect = DROPEFFECT_COPY;
	}
	else
	{
		*dwDropEffect = DROPEFFECT_MOVE;
	}

	GlobalUnlock(hGblEffect);

	///////////////////////////文件剪切板,用来存放文件列表/////////////////////////    

	uDropFilesLen = sizeof(DROPFILES);
	//DROPFILES结构的大小
	dropFiles.pFiles = uDropFilesLen;
	dropFiles.pt.x = 0;
	dropFiles.pt.y = 0;
	dropFiles.fNC = FALSE;
	//true: UNICODE, false: ascii
	dropFiles.fWide = TRUE;

	//uBufLen * 2表示的是宽字符大小， 加8表示文件末尾需要2个空指针结尾，每个指针占4个字节大小
	uGblLen = uDropFilesLen + uBufLen * 2 + 8;
	hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, uGblLen);

	szData = (char *)GlobalLock(hGblFiles);

	//把DROPFILES结构大小的内容放到szData剪切板空间的最开始
	memcpy(szData, (LPVOID)(&dropFiles), uDropFilesLen);



	//szFileList指向需要放入文件的那个空间，前面存放了DROPFILES结构大小的空间
	szFileList = szData + uDropFilesLen;

	//把文件列表转为宽字符，并存放到szFileList指向的那片空间
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE,
		lpBuffer, uBufLen, (WCHAR *)szFileList, uBufLen);
	CString str = szFileList;
	GlobalUnlock(hGblFiles);

	if (::OpenClipboard(NULL))
	{
		EmptyClipboard();

		//可以设置剪切板内容为拖动文件
		SetClipboardData(CF_HDROP, hGblFiles);

		//可以设置剪切板内容为复制或者剪切标识
		SetClipboardData(uDropEffect, hGblEffect);

		//关闭剪切板
		CloseClipboard();
	}
}

void TabFile::OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSelectItem = m_tree.GetSelectedItem();


	CString sFullName = tool->GetPathFromTree(hSelectItem, &m_tree);
	MessageBox(sFullName);
	if (sFullName.Find(".") != -1)
	{
		MessageBox("非文件夹");
		return;
	}
	
	DirIntoClipBoard(sFullName);




	this->GetParent()->GetParent()->SendMessage(WM_CLOSE);



	*pResult = 0;
}

void TabFile::InitFileTree()
{

	CString sDir;
    GetPrivateProfileString("Set", "FilePath",
		CString("NULL"), sDir.GetBuffer(MAX_PATH), MAX_PATH, "setting.ini");
	
	sDir.ReleaseBuffer();
	tool->BuildTree(sDir, &m_tree,&m_TreeImageList);


}