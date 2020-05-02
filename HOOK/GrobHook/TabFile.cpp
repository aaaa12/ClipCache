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
}


BEGIN_MESSAGE_MAP(TabFile, CDialogEx)
END_MESSAGE_MAP()


// TabFile 消息处理程序
