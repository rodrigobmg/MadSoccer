// DlgConsole.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../VirtualClient2.h"
#include "DlgLog.h"
#include <crtdbg.h> 


// CDlgConsole 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDlgLog, CDialog)

const COLORREF BRUSH_COLOR = RGB(140,140,140);

CDlgLog::CDlgLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLog::IDD, pParent)
{
	m_brush.CreateSolidBrush(BRUSH_COLOR);

}

CDlgLog::~CDlgLog()
{
}

void CDlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLog, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDlgLog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgLog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_BTN_CLEAR, &CDlgLog::OnBnClickedClear)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgConsole 메시지 처리기입니다.
int CDlgLog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ListBox.Create(WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL , CRect(0,0,100,100), this, ID_LISTBOX);
	m_ListBox.AddString(_T("Init"));
	m_BtnClear.Create( _T("Clear"), WS_VISIBLE | WS_CHILD| BS_PUSHBUTTON, CRect(0,0,50,20), this, ID_BTN_CLEAR);
	return 0;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDlgLog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	m_ListBox.MoveWindow(0,20,cx,cy-20);
}


//------------------------------------------------------------------------
// 콘솔 리스트박스에 문자를 추가한다.
//------------------------------------------------------------------------
void CDlgLog::AddString(const std::string &str)
{
	m_ListBox.AddString( common::str2wstr(str).c_str() );
	const int cnt = m_ListBox.GetCount();
	m_ListBox.SetSel(cnt-1,TRUE);
	m_ListBox.SetCurSel(cnt-1);
	if (cnt > 100)
		m_ListBox.DeleteString(0);// 정보가 너무 많다면, 가장 처음것을 삭제한다.
	CreateHorizontalScroll(str);
	TRACE( "%s\n", str.c_str() );
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void CDlgLog::CreateHorizontalScroll(const std::string &str)
{
	CDC *pDC = m_ListBox.GetDC();
	static int minDx = 0;
	CSize dx = pDC->GetTextExtent(CString(str.c_str()));
	if (dx.cx > minDx)
		minDx = dx.cx;
	m_ListBox.ReleaseDC(pDC);
	if (m_ListBox.GetHorizontalExtent() < minDx)
		m_ListBox.SetHorizontalExtent(minDx);
}


void CDlgLog::OnBnClickedOk()
{
//	OnOK();
}
void CDlgLog::OnBnClickedCancel()
{
//	OnCancel();
}

void CDlgLog::OnBnClickedClear()
{
	for (int i=m_ListBox.GetCount();i >=0;--i)
		m_ListBox.DeleteString( i );
}

HBRUSH CDlgLog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == ID_LISTBOX)
	{
		pDC->SetBkColor(BRUSH_COLOR);
		hbr = m_brush;
	}
	return hbr;
}
