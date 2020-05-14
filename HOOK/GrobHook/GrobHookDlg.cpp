
// GrobHookDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GrobHook.h"
#include "GrobHookDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <io.h>
#include <iostream>
#include<vector>
#include<fstream>
#include<iterator>

#include "SetDlg.h"
#include "KMHookDll.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGrobHookDlg 对话框
CGrobHookDlg::CGrobHookDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GROBHOOK_DIALOG, pParent)
	, m_iMod(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGrobHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_iMod);
}

BEGIN_MESSAGE_MAP(CGrobHookDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SEND_TEXT2WIN, &CGrobHookDlg::OnSendText2Win)
	ON_MESSAGE(WM_SEND_LOG, &CGrobHookDlg::OnSendLog)
	ON_MESSAGE(WM_USER_SAVECLIP, &CGrobHookDlg::OnSaveClip)
	ON_MESSAGE(WM_USER_SHOWTAB, &CGrobHookDlg::OnShowTab)
	ON_BN_CLICKED(IDC_BTN_HOOK, &CGrobHookDlg::OnBnClickedBtnHook)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CGrobHookDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CGrobHookDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, &CGrobHookDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO3, &CGrobHookDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO4, &CGrobHookDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RECORD, &CGrobHookDlg::OnBnClickedRecord)
	ON_BN_CLICKED(IDC_BUTTON3, &CGrobHookDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CGrobHookDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BTN_TABTEST, &CGrobHookDlg::OnBnClickedBtnTabtest)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CGrobHookDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, &CGrobHookDlg::OnBnClickedButtonPaste)
	ON_BN_CLICKED(IDC_BTN_SET, &CGrobHookDlg::OnBnClickedBtnSet)
END_MESSAGE_MAP()


// CGrobHookDlg 消息处理程序

BOOL CGrobHookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (NULL == m_pMsgDlg)
	{
		// 创建非模态对话框实例  WS_EX_NOACTIVATE 
		m_pMsgDlg = new CMsgDlg();
		m_pMsgDlg->Create(IDD_MSG, this);
	}

	iRetMod = 2;
	SetHook(m_hWnd, g_hKeyBoard, g_hMouse);
	ShowLog("运行中..");
	CString str;
	::GetPrivateProfileString("RADIO", "OutPutModel",
		CString("NULL"), str.GetBuffer(MAX_PATH), MAX_PATH, "setting.ini");

	m_iMod = atoi(str.GetBuffer());
	iOldMod = m_iMod;
	UpdateData(false);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGrobHookDlg::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	if (IsIconic())//IsIconic()作用是判断窗口是否处于最小化状态
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		//输出日志
		TEXTMETRIC tm;
		dc.GetTextMetrics(&tm);
		int iTxtHight = tm.tmHeight;
		//不超过10个从第一个输出
		if (iLogPos < 10)
		{
			for (int i = 0; i < 10; i++)
			{
				//从iLogSize开始
				dc.TextOut(0, iTxtHight*i, sLog[i]);
			}
		}
		else {
			//超过十个从插入处后一个做环
			for (int i = 0; i < 10; i++)
			{
				//从iLogSize开始
				dc.TextOut(0, iTxtHight*i, sLog[(iLogPos + i) % 10]);
			}
		}

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGrobHookDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGrobHookDlg::OnBnClickedBtnHook()
{
	// TODO: 在此添加控件通知处理程序代码
	SetHook(m_hWnd, g_hKeyBoard, g_hMouse);
}

char CGrobHookDlg::SwitchShift(char c)
{
	int p = 0;
	while (0 != noShift[p])
	{
		if (c == noShift[p])
			return withShift[p];
		p++;
	}
	return 0;
}
char CGrobHookDlg::SwitchNoShift(char c)
{
	int p = 0;
	while (0 != AwithShift[p])
	{
		if (c == AwithShift[p])
			return AnoShift[p];
		p++;
	}
	return 0;

}
vector<string> split(string str, string pattern)
{
	int pos;
	vector<string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

string UpperCase(string &str)
{
	int off = 'A' - 'a';
	for (int i = 0; i < str.size(); i++)
	{
		if ('a' <= str[i] && str[i] <= 'z')
		{
			str[i] += off;
		}
	}
	return str;
}

LRESULT CGrobHookDlg::OnSendText2Win(WPARAM wParam, LPARAM lParam)
{

	if (lParam == 0)
	{
		if (VK_BACK == wParam)//key code of Backspace
		{
			if (value.size() > 0)
				value = value.substr(0, value.size() - 1);
		}
		else
		{
			char c = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
			if ('A' <= c && c <= 'Z')
			{
				if (!GetAsyncKeyState(VK_SHIFT))
					c = SwitchNoShift(c);
			}
			else {
				if (GetAsyncKeyState(VK_SHIFT))
				{
					c = SwitchShift(c);
				}
			}
			value = value + c;
		}
		m_pMsgDlg->SetTxt(value.c_str(), true);
	}
	else
	{
		CString stmp;
		if (HOOK_CMD_SHOW == lParam)
		{
			m_pMsgDlg->SetTxt("", true);

		}
		else if (HOOK_CMD_CLEAR == lParam)
		{
			value = "";
			m_pMsgDlg->SetTxt("", false);
			return 0;
		}
		else if (HOOK_CMD_SUBMIT == lParam)
		{

			if ('-' == value[0])
			{
				//拆
				stmp.Format("执行:%s", value.c_str());
				ShowLog(stmp);
				vector<string> v = split(value, " ");
				if ("-SAVE" == UpperCase(v[0]) && "CLIP" == UpperCase(v[1]))//-SAVE CLIP [filepath]
				{
					stmp.Format("%s.txt", v[2].c_str());
					SaveStrToFile(stmp, GetClipBoradText());
					ShowLog("执行完成");
					m_pMsgDlg->SetTxt("111111111111111111", true);
				}
				else
				{
					ShowLog("解析失败");
					m_pMsgDlg->SetTxt("000000000000000000", true);
				}

			}
			else//
			{
				CString fileName;
				fileName.Format("%s.txt", value.c_str());

				while (GetAsyncKeyState(18)) {};//wait for alt up
				bool ret;
				if (1 == m_iMod)
					ret = TypeTextFile(fileName);
				else if (2 == m_iMod)
					ret = OutPutFile(fileName);
				else if (3 == m_iMod)
					ret = FileToClip(fileName);

				if (ret)
					m_pMsgDlg->SetTxt("111111111111111111", true);
				else
					m_pMsgDlg->SetTxt("000000000000000000", true);


			}
			AnysSleep(1000);
			m_pMsgDlg->SetTxt("", false);
			value = "";
		}
	}

	return 0;
}

LRESULT CGrobHookDlg::OnSendLog(WPARAM wParam, LPARAM lParam)
{

	if (lParam != 0)
	{
		char *c = (char *)lParam;
		CString str;
		str.Format("%s", c);
		ShowLog(str);
	}

	return 0;
}
//no shift type value
bool CGrobHookDlg::CheckShift(char c, int &key)
{
	if ('A' <= c && c <= 'Z')
	{
		key = c;
		return true;
	}

	//number line  0123456789
	string s = ")!@#$%^&*(";
	for (int i = 0; i < 10; i++)
	{
		if (c == s[i])
		{
			key = i + '0';
			return true;
		}
	}
	//~
	if ('~' == c)
	{
		key = 0xc0;
		return true;
	}

	string s1 = ":+<_>?";
	for (int i = 0; i < s1.size(); i++)
	{
		if (c == s1[i])
		{
			key = i + 0xba;
			return true;
		}
	}
	string s2 = "{|}\"";
	for (int i = 0; i < s2.size(); i++) {
		if (c == s2[i])
		{
			key = i + 0xdb;
			return true;
		}
	}

	return false;
}

bool CGrobHookDlg::CheckNoShift(char c, int &key)
{
	if ('a' <= c && c <= 'z')
	{
		key = c + offset;
		return true;
	}

	if ('0' <= c && c <= '9')
	{
		key = c;
		return true;
	}

	if (' ' == c)
	{
		key = c;
		return true;
	}

	if ('`' == c)
	{
		key = 0xc0;
		return true;
	}

	string s1 = ";=,-./";
	for (int i = 0; i < s1.size(); i++)
	{
		if (c == s1[i])
		{
			key = i + 0xba;
			return true;
		}
	}
	string s2 = "[\\]'";
	for (int i = 0; i < s2.size(); i++)
	{
		if (c == s2[i])
		{
			key = i + 0xdb;
			return true;
		}
	}
	return false;

}

void CGrobHookDlg::TypeStr(CString str)//cstring 一个单位4字节
{
	//close caps lock 
	BYTE btKeyState[256];
	GetKeyboardState(btKeyState);
	if (btKeyState[VK_CAPITAL] & 1)
	{
		keybd_event(VK_CAPITAL, 0, 0, 0);
		keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
	}

	int key;
	CString msg;



	for (int i = 0; i < str.GetLength(); i++)
	{
		msg.Format("%x", 0x80 & (unsigned long)str[i]);
		//MessageBox(msg);
		if ((unsigned long)(0x80 & (unsigned long)str[i]) == ((unsigned long)0x80))
		{
			continue;//跳过中文
		}

		Sleep(WaitTime);
		if (CheckShift(str[i], key))
		{
			keybd_event(VK_LSHIFT, 0, 0, 0);
			keybd_event(key, 0, 0, 0);
			keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
		}
		else if (CheckNoShift(str[i], key))
		{
			keybd_event(key, 0, 0, 0);
			keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
		}
		else
		{
			keybd_event(str[i], 0, 0, 0);
			keybd_event(str[i], 0, KEYEVENTF_KEYUP, 0);
		}
	}
}

string CGrobHookDlg::TrimStr(string s)
{
	string ret = s;
	if (!ret.empty())
	{
		ret.erase(0, ret.find_first_not_of(" "));
		ret.erase(ret.find_last_not_of(" ") + 1);
	}
	return ret;
}

void CGrobHookDlg::SpaceAndTabNum(string str, int& sNum, int &tNum)
{
	int p = 0;
	sNum = 0;//space count
	tNum = 0;//tab count
	while (' ' == str[p] || '	' == str[p])
	{
		if (' ' == str[p])
			sNum++;
		if ('	' == str[p])
			tNum++;
		p++;
	}
}
void CGrobHookDlg::TypeKeyBackSpace(int times)
{
	for (int i = 0; i < times; i++)
	{
		Sleep(WaitTime);
		keybd_event(VK_BACK, 0, 0, 0);
		keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
	}
}

void CGrobHookDlg::TypeKeyEnter(string proLine, int &sNum, int &tNum)
{


	/*string strim=TrimStr(proLine);
	if(strim !="")*/
	SpaceAndTabNum(proLine, sNum, tNum);//前几行有tab，回车会自动对齐，所以回车后要先删除自动对齐加的tab

//keybd_event(VK_RETURN, 0, 0, 0);
//keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	for (int i = 0; i < tNum; i++)
	{
		Sleep(WaitTime);
		keybd_event(VK_BACK, 0, 0, 0);
		keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
	}
}

bool CGrobHookDlg::TypeTextFile(CString path)
{
	ifstream myfile(path, ifstream::binary);//不指定二进制读取的话，中文会自动转码

	if (!myfile.good())
	{
		TypeStr("no file");
		ShowLog("路径错误");
		myfile.close();
		return false;
	}

	string oldLine, line;

	int sNum = 0, tNum = 0;
	while (getline(myfile, line)) //line by line call TypeStr
	{
		string strim = TrimStr(line);
		if (strim == "")
			continue;

		Sleep(50);
		TypeKeyEnter(oldLine.c_str(), sNum, tNum);
		TypeStr(line.c_str());
		oldLine = line;
	}
	myfile.close();
	return true;
}

//txt文件要保存为ansi编码
bool CGrobHookDlg::OutPutFile(CString path)
{

	HWND hWnd = NULL;
	string str;
	GetFocusWnd(hWnd);
	if (NULL == hWnd)
	{
		TypeStr("no handle");
		ShowLog("未获取句柄");
		return false;
	}
	if (1 == iRetMod)
	{
		ifstream myfile(path);

		if (!myfile.good())
		{
			TypeStr("no file");
			ShowLog("路径错误");
			myfile.close();
			return false;
		}

		string proline, line;

		int sNum = 0, tNum = 0;
		while (getline(myfile, line)) //line by line call TypeStr
		{

			Sleep(50);

			TypeKeyEnter(proline, sNum, tNum);


			int p = 0;
			while (line[p] != 0)
			{
				::SendMessage(hWnd, WM_CHAR, (WPARAM)(line[p] & 0xFF), 0);//send，要符合各种情况！post
				p++;
			}
			proline = line;
		}
		myfile.close();
	}
	else
	{
		FILE *pf = NULL;
		int filelen = 0;
		int i = 0;
		char *buf;
		pf = fopen(path, "r");
		if (pf == NULL)
		{
			TypeStr("no file");
			ShowLog("路径错误");
			return false;
		}
		else
		{
			fseek(pf, 0, SEEK_END);
			filelen = ftell(pf);
			rewind(pf);
			buf = (char*)malloc(filelen + 1);
			for (i = 0; i < filelen + 1; i++)
				buf[i] = 0;

			fread(buf, filelen, 1, pf);

			fclose(pf);

			int p = 0;
			while (buf[p] != 0)
			{
				::PostMessage(hWnd, WM_CHAR, (WPARAM)(buf[p] & 0xFF), 0);
				p++;
			}

			free(buf); //最后记得要释放
		}
	}
	return true;
}

bool CGrobHookDlg::FileToClip(CString path)
{
	FILE *pf = NULL;
	int filelen = 0;
	int i = 0;
	HGLOBAL clipbuffer;
	char * buffer;
	pf = fopen(path, "r");
	if (pf == NULL)
	{
		//TypeStr("no file");
		ShowLog("路径错误");
		return false;
	};

	fseek(pf, 0, SEEK_END);

	if (OpenClipboard())
	{
		filelen = ftell(pf);
		rewind(pf);
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, filelen + 1);
		fread(clipbuffer, filelen, 1, pf);

		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
	}
	fclose(pf);
	return true;
}

void CGrobHookDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (NULL != m_pMsgDlg)
	{
		// 创建非模态对话框实例   
		m_pMsgDlg->DestroyWindow();
		delete m_pMsgDlg;
		m_pMsgDlg = NULL;
	}
	UnhookWindowsHookEx(g_hKeyBoard);
	UnhookWindowsHookEx(g_hMouse);
}



CString CGrobHookDlg::GetClipBoradText()
{
	// TODO: 在此添加控件通知处理程序代码
	char * buffer = NULL;
	//open the clipboard
	CString fromClipboard;
	if (OpenClipboard())
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		if (0 == hData)
			return "";
		char * buffer = (char*)GlobalLock(hData);
		fromClipboard.Format("%s", buffer);
		GlobalUnlock(hData);
		CloseClipboard();
	}
	return fromClipboard;
}

void CGrobHookDlg::ShowLog(CString str)
{
	CClientDC dc(this);


	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString fileName;
	CString logTime;

	MakeDir("log");

	fileName.Format("log/%4d-%02d-%02d_log.txt", sys.wYear, sys.wMonth, sys.wDay);
	logTime.Format("[%4d/%02d/%02d %02d:%02d:%02d]:", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	sLog[(iLogPos++) % 10] = logTime + str;

	Invalidate(true);

	//保存到文件
	FILE *stream;//创建一个文件的指针
	char ch[] = " ";
	stream = fopen(fileName, "a");
	fprintf(stream, "%s\n", logTime + str);
	fclose(stream);
}

void CGrobHookDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString str;
	str.Format("%d", m_iMod);
	MessageBox(str);
	//CDialogEx::OnOK();
}

void CGrobHookDlg::OnBnClickedRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	//no change
	if (iOldMod == m_iMod)
		return;

	iOldMod = m_iMod;
	CString str;
	str.Format("%d", m_iMod);
	::WritePrivateProfileString("RADIO", "OutPutModel", str, "setting.ini");

	switch (m_iMod) {
	case 0:
		str = "隐藏模式（未完成）";
		break;
	case 1:
		str = "模拟键盘";
		break;
	case 2:
		str = "消息输出";
		break;
	case 3:
		str = "剪切板";
		break;
	default:
		str = "未知";
		break;
	}
	ShowLog("切换模式->" + str);
}


void CGrobHookDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	MakeDir("c\\cc\\cca");
}

void CGrobHookDlg::MakeDir(CString path)
{
	if (-1 == _access(path, 0))
	{
		CString cmd = "mkdir " + path;
		system(cmd);
	}
}

CString CGrobHookDlg::GetFilePath(CString fileName)
{
	int pos = fileName.ReverseFind('\\');
	CString path = fileName.Left(pos);
	return path;

}

void CGrobHookDlg::SaveStrToFile(CString fileName, CString str)
{
	if ("" == str)
		return;

	FILE *pt = NULL;
	CString path = GetFilePath(fileName);
	MakeDir(path);

	pt = fopen(fileName, "w");
	fprintf(pt, "%s", str);
	fclose(pt);
}

CString CGrobHookDlg::ReadStrFromFile(CString path)
{
	CString ret;
	char * buffer;
	FILE * f;
	long length;
	//以二进制形式打开文件	
	f = fopen(path, "rb");
	if (NULL == f)
	{
		return ret;
	}
	//把文件的位置指针移到文件尾	
	fseek(f, 0, SEEK_END);
	//获取文件长度;	
	length = ftell(f);
	//把文件的位置指针移到文件开头	
	fseek(f, 0, SEEK_SET);
	buffer = (char *)malloc((length + 1) * sizeof(char));
	fread(buffer, 1, length, f);
	buffer[length] = '\0';
	fclose(f);
	ret.Format("%s", buffer);

	free(buffer);

	return ret;
}

void CGrobHookDlg::AnysSleep(int iClock)
{
	clock_t s = clock();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (clock() - s > iClock)
			break;
		TranslateMessage(&msg);//转换 如WM_KEYDOWN ->WM_CHAR
		DispatchMessage(&msg);
	}
}

void CGrobHookDlg::OnBnClickedRecord()
{
	// TODO: 在此添加控件通知处理程序代码

		//vector Serize
	vector<int>vecData;
	for (int nIndex = 0; nIndex < 5; ++nIndex)
		vecData.push_back(nIndex);

	//写入文件
	ofstream outFile("record.txt", std::ios::out | std::ios::binary);
	if (outFile.is_open())
	{
		copy(vecData.begin(), vecData.end(), std::ostream_iterator<int>(outFile, " "));
		outFile.close();
	}

	//从文件读取
	vector<int>vecRst;
	ifstream inFile("record.txt", std::ios::binary);
	if (inFile.is_open())
	{
		copy(std::istream_iterator<int>(inFile), std::istream_iterator<int>(), back_inserter(vecRst));
		inFile.close();
	}


}

void CGrobHookDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

}

vector<CString> SplitCString(CString strSrc, CString strGap)
{
	vector<CString> ret;
	int nPos = strSrc.Find(strGap);

	CString strLeft = "";
	while (0 <= nPos) {
		strLeft = strSrc.Left(nPos);
		if (!strLeft.IsEmpty())
			ret.push_back(strLeft);
		strSrc = strSrc.Right(strSrc.GetLength() - nPos - 1);
		nPos = strSrc.Find(strGap);
	}
	if (!strSrc.IsEmpty())
	{
		ret.push_back(strSrc);
	}

	return ret;
}

void CGrobHookDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<CString> v=SplitCString("D:\\test\\collect", "\\");


	//CKMHookDll::GetInstance()->InstallHook(true, true, true);

}


void CGrobHookDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CKMHookDll::GetInstance()->RunScript();
}

void CGrobHookDlg::OnBnClickedBtnTabtest()
{
	// TODO: 在此添加控件通知处理程序代码
	OnShowTab(0, 0);
}

vector<CString> getFiles(CString strPath, UINT &size)
{
	size = 0;
	vector<CString> vRet;
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);    //查找包含字符串的文件
	while (bContinue)//不递归
	{
		bContinue = file.FindNextFile();        //查找下一个文件
		if (/*file.IsDirectory() &&*/ !file.IsDots())
		{
			size = size+file.GetFilePath().GetLength()+1; //GetLength()未计算尾部/0
			vRet.push_back(file.GetFilePath());
		}
	}
	size + 1;
	return vRet;
}
void CGrobHookDlg::OnBnClickedButtonCopy()
{
	// TODO: 在此添加控件通知处理程序代码
	//要复制剪切的文档或者文件夹
	//CString sPath = getFiles("D:\\Temp");
	//MessageBox(sPath);
    //D:\目录test\A
	//D : \目录test\B
	//char lpBuffer[10240];
	//memset(lpBuffer,0, 10240);
	//char *lpBuffer = sPath.GetBuffer();
//csdm: c:\temp1.txt'\0'c:\temp2.txt'\0''\0'
	//char *a = "D:\\目录test\\A";
	//char *b = "D:\\目录test\\B";
	//int lenA = strlen("D:\\目录test\\A") + 1;
	//int lenB = strlen("D:\\目录test\\B") + 1;
	//memcpy(lpBuffer, a, lenA);
	//memcpy(lpBuffer+ lenA, b, lenB);
	//UINT uBufLen = lenA+ lenB+1;//后面要2个'\0'所以再加1
//调整自单个文件复制：https://www.cnblogs.com/xydblog/p/3643453.html
	UINT uBufLen;
	int lenCurrent;
	int lenTotal=0;
	char lpBuffer[10240];
	memset(lpBuffer, 0, 10240);
	vector<CString> vFileList= getFiles("D:\\test", uBufLen);
	if (uBufLen > 10240)
	{
		MessageBox("所选文件夹下文件太多！");
		return;
	}

	for (int i=0;i< vFileList.size();i++)
	{
		lenCurrent = vFileList[i].GetLength()+1;
		memcpy(lpBuffer + lenTotal, vFileList[i].GetBuffer(), lenCurrent);
		lenTotal+= lenCurrent;
	}

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

//调用GlobalAlloc函数分配一块内存，该函数会返回分配的内存句柄。
//调用GlobalLock函数锁定内存块，该函数接受一个内存句柄作为参数，然后返回一个指向被锁定的内存块的指针。 您可以用该指针来读写内存。
//调用GlobalUnlock函数来解锁先前被锁定的内存，该函数使得指向内存块的指针无效。
//调用GlobalFree函数来释放内存块。您必须传给该函数一个内存句柄。

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
	dropFiles.fWide = FALSE;


	uGblLen = uDropFilesLen + uBufLen;
	hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, uGblLen);

	szData = (char *)GlobalLock(hGblFiles);

	//把DROPFILES结构大小的内容放到szData剪切板空间的最开始
	memcpy(szData, (LPVOID)(&dropFiles), uDropFilesLen);

	

	//szFileList指向需要放入文件的那个空间，前面存放了DROPFILES结构大小的空间
	szFileList = szData + uDropFilesLen;
	
	//把文件列表转为宽字符，并存放到szFileList指向的那片空间
	//MultiByteToWideChar(CP_ACP, MB_COMPOSITE,
	//	lpBuffer, uBufLen, (WCHAR *)szFileList, uBufLen);
	memcpy(szFileList, lpBuffer, uBufLen);
	//CString str = szFileList;

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

void CGrobHookDlg::OnBnClickedButtonPaste()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT uDropEffect = RegisterClipboardFormat("Preferred DropEffect");
	UINT cFiles = 0;
	DWORD dwEffect = 0;
	DWORD * dw = NULL;

	if (::OpenClipboard(this->m_hWnd))
	{
		//获取CF_HDROP剪切板的内容
		HDROP hDrop = HDROP(GetClipboardData(CF_HDROP));

		//判断文件夹剪切板是否存在，如果存在,在判断自定义剪切板格式是否存在
		//如果自定义剪切板格式存在，则判断是复制操作还是剪切操作
		if (hDrop)
		{
			//获取自定义剪切板的内容
			dw = (DWORD*)(GetClipboardData(uDropEffect));

			if (dw == NULL)
			{
				dwEffect = DROPEFFECT_COPY;
			}
			else
			{
				dwEffect = *dw;
			}

			//查询获取到的文件或文档
			cFiles = DragQueryFile(hDrop, (UINT)-1, NULL, 0);
			char szFile[MAX_PATH] = { '\0' };
			char lpBuffer[MAX_PATH] = { '\0' };

			//查询所有的文件列表，并把路径显示到文本框中
			for (UINT count = 0; count < cFiles; count++)
			{
				DragQueryFile(hDrop, count, szFile, sizeof(szFile));

				lstrcat(lpBuffer, szFile);
				lstrcat(lpBuffer, "\r\n");

				//把文件显示到文本框
				//SetDlgItemText(IDC_EDIT_DISPLAY, lpBuffer);
			}
		}

		CloseClipboard();
	}

	if (cFiles == 0)
	{
		return;
	}
}

void CGrobHookDlg::OnBnClickedBtnSet()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlg dlg;
	dlg.DoModal();
}

void clearStr(CString &str,CString moveChar)
{
	for (int i = 0; i < moveChar.GetLength(); i++)
	{
		CString ch;
		ch.Format("%c", moveChar[i]);
		str.Replace(ch,"");
	}

}

LRESULT CGrobHookDlg::OnSaveClip(WPARAM wParam, LPARAM lParam)
{
	//Sleep(300);
	CString sTmpDir;
	GetPrivateProfileString("Set", "TempPath",
		CString("NULL"), sTmpDir.GetBuffer(MAX_PATH), MAX_PATH, "setting.ini");
	SetDlgItemText(IDC_EDIT_TEMP, sTmpDir);
	sTmpDir.ReleaseBuffer();

	CString txt=GetClipBoradText();

	CString brief = txt.Left(20);//展示20个字符
	clearStr(brief, "<>/\\|:\"\*?	\n\r");
	time_t rawtime;
	struct tm *ptminfo;
	time(&rawtime);
	ptminfo = localtime(&rawtime);
	long y = ptminfo->tm_year + 1900;
	long m = ptminfo->tm_mon + 1;
	long d = ptminfo->tm_mday;
	long h = ptminfo->tm_hour;
	long mi = ptminfo->tm_min;
	long s = ptminfo->tm_sec;

	long kk = ((( m * 30 + d) * 12 + h) * 60 + mi) * 60 + s;
	kk = 99999999 - kk;
	CString fileName;
	fileName.Format("%d[%s].txt", kk, brief);

	//MessageBox(fileName);
	SaveStrToFile(sTmpDir + "\\" + fileName, txt);
	return 0;
}

LRESULT CGrobHookDlg::OnShowTab(WPARAM wParam, LPARAM lParam)
{
	if (NULL == tab)
	{
		CWnd* pDesktopWnd = CWnd::GetDesktopWindow();
		// 创建非模态对话框实例   
		tab = new TabMain();
		tab->Create(IDD_TABDLG_MAIN, pDesktopWnd);
	}
	tab->ShowWindow(SW_SHOWNORMAL);
	::SetWindowPos(tab->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return 0;
}