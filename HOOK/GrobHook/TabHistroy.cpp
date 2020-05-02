// TabHistroy.cpp: 实现文件
//

#include "pch.h"
#include "TabHistroy.h"
#include "GrobHook.h"
#include "afxdialogex.h"


// TabHistroy 对话框

IMPLEMENT_DYNAMIC(TabHistroy, CDialogEx)

TabHistroy::TabHistroy(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TABDLG_HISTROY, pParent)
{

}

TabHistroy::~TabHistroy()
{
}

void TabHistroy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TabHistroy, CDialogEx)
END_MESSAGE_MAP()


// TabHistroy 消息处理程序
