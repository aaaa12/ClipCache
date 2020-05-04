// TabMain.cpp: 实现文件
//

#include "pch.h"
#include "TabMain.h"
#include "GrobHook.h"
#include "afxdialogex.h"

// TabMain 对话框

IMPLEMENT_DYNAMIC(TabMain, CDialogEx)

TabMain::TabMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TABDLG_MAIN, pParent)
{

}

TabMain::~TabMain()
{
}

void TabMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tab);
}


BEGIN_MESSAGE_MAP(TabMain, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &TabMain::OnTcnSelchangeTab)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// TabMain 消息处理程序


BOOL TabMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//关联对话框,并且将IDC_TABTEST控件设为父窗口 
	//m_tabHist.Create(IDD_TABDLG_HISTROY, GetDlgItem(IDC_TAB));
	m_tabFile.Create(IDD_TABDLG_FILE, GetDlgItem(IDC_TAB));
	m_tabText.Create(IDD_TABDLG_TXT, GetDlgItem(IDC_TAB));

	m_tab.InsertItem(0, "历史Txt");
	m_tab.InsertItem(1, "文件");
	m_tab.InsertItem(2, "文本");
	m_tab.SetCurSel(0);

	//分别设置隐藏和显示 
	//m_tabHist.ShowWindow(true);
	//m_tabFile.ShowWindow(false);
	//m_tabText.ShowWindow(false);
	bResize = true;
	SetCtlRect();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void TabMain::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int CurSel = m_tab.GetCurSel();
	switch (CurSel)
	{
	case 0:
		//m_tabHist.ShowWindow(true);
		m_tabFile.ShowWindow(false);
		m_tabText.ShowWindow(true);
		m_tabText.SetExpendModel(EXPEND_HISTROY);
		break;
	case 1:
		//m_tabHist.ShowWindow(false);
		m_tabFile.ShowWindow(true);
		m_tabText.ShowWindow(false);
		break;
	case 2:
		//m_tabHist.ShowWindow(false);
		m_tabFile.ShowWindow(false);
		m_tabText.ShowWindow(true);
		m_tabText.SetExpendModel(EXPEND_COLLECT);
		break;
	default:
		;
	}


	*pResult = 0;
}

void TabMain::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	SetCtlRect();
}

void TabMain::SetCtlRect()
{
	if (!bResize)
		return;

	CRect rc;
	this->GetClientRect(&rc);
	m_tab.MoveWindow(&rc);

	//窗口的rect
	m_tab.GetClientRect(&rc);
	//调整子对话框在父窗口中的位置 
	rc.top += 20;
	rc.bottom -= 1;
	rc.left += 1;
	rc.right -= 2;

	////设置子对话框尺寸并移动到指定位置 
	//m_tabHist.MoveWindow(&rc);
	m_tabFile.MoveWindow(&rc);
	m_tabText.MoveWindow(&rc);

}

void TabMain::SetTxtExpendModel(int flag)
{
	m_tabText.SetExpendModel(flag);
}