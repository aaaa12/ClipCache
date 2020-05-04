// SetDlg.cpp: 实现文件
//

#include "pch.h"
#include "SetDlg.h"
#include "resource.h"
#include "afxdialogex.h"


// SetDlg 对话框

IMPLEMENT_DYNAMIC(SetDlg, CDialogEx)

SetDlg::SetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SET, pParent)
{

}

SetDlg::~SetDlg()
{
}

void SetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SetDlg 消息处理程序


BOOL SetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString str;
	//全局的这个函数写入注册表，非全局的写入ini文件
	GetPrivateProfileString("Set", "TempPath",
		CString("NULL"), str.GetBuffer(MAX_PATH), MAX_PATH, "setting.ini");
	SetDlgItemText(IDC_EDIT_TEMP, str);

	GetPrivateProfileString("Set", "CollectPath",
		CString("NULL"), str.GetBuffer(MAX_PATH), MAX_PATH, "setting.ini");
	SetDlgItemText(IDC_EDIT_COLLECT, str);

	GetPrivateProfileString("Set", "FilePath",
		CString("NULL"), str.GetBuffer(MAX_PATH), MAX_PATH, "setting.ini");
	SetDlgItemText(IDC_EDIT_FILE, str);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void SetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_EDIT_TEMP, str);
	::WritePrivateProfileString("Set", "TempPath", str, "setting.ini");
	GetDlgItemText(IDC_EDIT_FILE, str);
	::WritePrivateProfileString("Set", "FilePath", str, "setting.ini");
	GetDlgItemText(IDC_EDIT_COLLECT, str);
	::WritePrivateProfileString("Set", "CollectPath", str, "setting.ini");
	CDialogEx::OnOK();
}
