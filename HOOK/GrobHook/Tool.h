#pragma once
#include "afxdialogex.h"
class CTool
{
private:
	static CTool* instance;

public:
	static CTool* get_instance() {//����ģʽ
		if (instance == nullptr) {
			instance = new CTool;
		}
		return instance;
	}
	/////////////////////////////////////
	/*
	@param
	   string sDir: ·��
       CTreeCtrl tree:��
	*/
	void BuildTree(CString sDir, CTreeCtrl *tree, CImageList *imageList);
	CString GetPathFromTree(HTREEITEM hCurrent, CTreeCtrl *tree);
	void AddSubDirToTree(HTREEITEM hParent, CTreeCtrl *tree, CImageList *imageList);


};

