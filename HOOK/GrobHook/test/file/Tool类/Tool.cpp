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
		hCurrent = tree->GetParentItem(hCurrent); //���ظ���Ŀ���
	}

	while (hCurrent)
	{
		sTmp = tree->GetItemText(hCurrent);    //�����б�����Ŀ����
		//if (strTemp.Right(1) != "\\")
		//	strTemp += "\\";
		sRet = sTmp + "\\" + sRet;
		hCurrent = tree->GetParentItem(hCurrent); //���ظ���Ŀ���
	}
	return sRet;
}

void CTool::AddSubDirToTree(HTREEITEM hParent,CTreeCtrl *tree, CImageList *imageList)
{
	CString strPath = GetPathFromTree(hParent, tree);     //��ȡȫ·��
	//CString strPath = m_tree->GetItemText(hParent);
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);    //���Ұ����ַ������ļ�
	while (bContinue)
	{
		bContinue = file.FindNextFile();        //������һ���ļ�
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
		msg.Format("·������%s", sDir.GetBuffer());
		AfxMessageBox(msg);
		return;
	}

	tree->DeleteAllItems();
	//ֻ����һ��ͼƬ
	int i=imageList->GetImageCount();
	for (; i > 1; i--)
	{
		imageList->Remove(i-1);
	}

	HTREEITEM m_hRoot = tree->InsertItem(sDir,0,0);         //������ڵ�
	AddSubDirToTree(m_hRoot, tree, imageList);
	tree->Expand(m_hRoot, TVE_EXPAND);               //չ�����۵������б� TVE_EXPANDչ���б�  

}
