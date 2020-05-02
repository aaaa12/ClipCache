// TabText.cpp: 实现文件
//

#include "pch.h"
#include "TabText.h"
#include "GrobHook.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(TabText, CDialogEx)
END_MESSAGE_MAP()


// TabText 消息处理程序
