#include "pch.h"
#include "Tool.h"


CTool* CTool::instance = nullptr;

CString CTool::GetPathFromTree(HTREEITEM hCurrent, CTreeCtrl *tree)
{
	CString sTmp;
	CString sRet;
	if (hCurrent)
	{
		sRet = tree->GetItemText(hCurrent);
		hCurrent = tree->GetParentItem(hCurrent); //返回父项目句柄
	}

	while (hCurrent)
	{
		sTmp = tree->GetItemText(hCurrent);    //检索列表中项目文字
		//if (strTemp.Right(1) != "\\")
		//	strTemp += "\\";
		sRet = sTmp + "\\" + sRet;
		hCurrent = tree->GetParentItem(hCurrent); //返回父项目句柄
	}
	return sRet;
}

void CTool::AddSubDirToTree(HTREEITEM hParent,CTreeCtrl *tree, CImageList *imageList)
{
	CString strPath = GetPathFromTree(hParent, tree);     //获取全路径
	//CString strPath = m_tree->GetItemText(hParent);
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);    //查找包含字符串的文件
	while (bContinue)
	{
		bContinue = file.FindNextFile();        //查找下一个文件
		if (/*file.IsDirectory() &&*/ !file.IsDots())
		{
			
			SHFILEINFO info;
			//AfxMessageBox(file.GetFilePath());
			//AfxMessageBox(file.GetFileName());
			//CString temp = file.GetFilePath() + "\\" + file.GetFileName();
			//AfxMessageBox(temp);
			SHGetFileInfo(file.GetFilePath(), 0, &info, sizeof(&info), SHGFI_DISPLAYNAME | SHGFI_ICON);
			int i = imageList->Add(info.hIcon);
			tree->InsertItem(file.GetFileName(),i, i, hParent);
		}
	}
}

void CTool::BuildTree(CString sDir, CTreeCtrl *tree, CImageList *imageList)
{
	if (!PathIsDirectory(sDir))
	{
		CString msg;
		msg.Format("路径错误：%s", sDir.GetBuffer());
		AfxMessageBox(msg);
		return;
	}

	tree->DeleteAllItems();
	//只留第一个图片
	int i=imageList->GetImageCount();
	for (; i > 1; i--)
	{
		imageList->Remove(i-1);
	}

	HTREEITEM m_hRoot = tree->InsertItem(sDir,0,0);         //插入根节点
	AddSubDirToTree(m_hRoot, tree, imageList);
	tree->Expand(m_hRoot, TVE_EXPAND);               //展开或折叠子项列表 TVE_EXPAND展开列表  

}
