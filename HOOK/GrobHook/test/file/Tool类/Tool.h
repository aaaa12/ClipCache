#pragma once
#include "afxdialogex.h"
class CTool
{
private:
	static CTool* instance;

public:
	static CTool* get_instance() {//单例模式
		if (instance == nullptr) {
			instance = new CTool;
		}
		return instance;
	}
	/////////////////////////////////////
	/*
	@param
	   string sDir: 路径
       CTreeCtrl tree:树
	*/
	void BuildTree(CString sDir, CTreeCtrl *tree, CImageList *imageList);
	CString GetPathFromTree(HTREEITEM hCurrent, CTreeCtrl *tree);
	void AddSubDirToTree(HTREEITEM hParent, CTreeCtrl *tree, CImageList *imageList);


};

