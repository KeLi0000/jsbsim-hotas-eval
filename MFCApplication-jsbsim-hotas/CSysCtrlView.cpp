// CSysCtrlView.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication-jsbsim-hotas.h"
#include "CSysCtrlView.h"


// CSysCtrlView

IMPLEMENT_DYNCREATE(CSysCtrlView, CFormView)

CSysCtrlView::CSysCtrlView()
	: CFormView(IDD_CTRL_DIALOG)
{

}

CSysCtrlView::~CSysCtrlView()
{
}

void CSysCtrlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSysCtrlView, CFormView)
	ON_BN_CLICKED(IDC_INIT_BUTTON, &CSysCtrlView::OnBnClickedInitButton)
	ON_BN_CLICKED(IDC_START_BUTTON, &CSysCtrlView::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_PAUSE_BUTTON, &CSysCtrlView::OnBnClickedPauseButton)
	ON_BN_CLICKED(IDC_CONTINUE_BUTTON, &CSysCtrlView::OnBnClickedContinueButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CSysCtrlView::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_RESET_BUTTON, &CSysCtrlView::OnBnClickedResetButton)
	ON_MESSAGE(UM_SIM_CREATE_SUCCESSFUL, &CSysCtrlView::OnUmSimCreateSuccessful)
	ON_MESSAGE(UM_SIM_CREATE_FAILED, &CSysCtrlView::OnUmSimCreateFailed)
END_MESSAGE_MAP()


// CSysCtrlView 诊断

#ifdef _DEBUG
void CSysCtrlView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSysCtrlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSysCtrlView 消息处理程序

void CSysCtrlView::OnBnClickedInitButton()
{
	// 修改按钮状态
	GetDlgItem(IDC_INIT_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESET_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONTINUE_BUTTON)->EnableWindow(FALSE);
	// 向框架发送初始化消息
	GetParentFrame()->PostMessage(UM_SYSTEM_INIT);
}

void CSysCtrlView::OnBnClickedStartButton()
{
	// 修改按钮状态
	GetDlgItem(IDC_INIT_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow();
	GetDlgItem(IDC_RESET_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow();
	GetDlgItem(IDC_CONTINUE_BUTTON)->EnableWindow(FALSE);
	// 向框架发送开始消息
	GetParentFrame()->PostMessage(UM_SYSTEM_START);
}

void CSysCtrlView::OnBnClickedPauseButton()
{
	// 修改按钮状态
	GetDlgItem(IDC_INIT_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESET_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONTINUE_BUTTON)->EnableWindow();
	// 向框架发送暂停消息
	GetParentFrame()->PostMessage(UM_SYSTEM_PAUSE);
}

void CSysCtrlView::OnBnClickedContinueButton()
{
	// 修改按钮状态
	GetDlgItem(IDC_INIT_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow();
	GetDlgItem(IDC_RESET_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow();
	GetDlgItem(IDC_CONTINUE_BUTTON)->EnableWindow(FALSE);
	// 向框架发送继续消息
	GetParentFrame()->PostMessage(UM_SYSTEM_CONT);
}

void CSysCtrlView::OnBnClickedStopButton()
{
	// 修改按钮状态
	GetDlgItem(IDC_INIT_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESET_BUTTON)->EnableWindow();
	GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONTINUE_BUTTON)->EnableWindow(FALSE);
	// 向框架发送结束消息
	GetParentFrame()->PostMessage(UM_SYSTEM_STOP);
}

void CSysCtrlView::OnBnClickedResetButton()
{
	// 修改按钮状态
	GetDlgItem(IDC_INIT_BUTTON)->EnableWindow();
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESET_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONTINUE_BUTTON)->EnableWindow(FALSE);
	// 向框架发送重置消息
	GetParentFrame()->PostMessage(UM_SYSTEM_RESET);
}

afx_msg LRESULT CSysCtrlView::OnUmSimCreateSuccessful(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_INIT_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow();
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESET_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONTINUE_BUTTON)->EnableWindow(FALSE);
	return 0;
}

afx_msg LRESULT CSysCtrlView::OnUmSimCreateFailed(WPARAM wParam, LPARAM lParam)
{
	// 修改按钮状态
	GetDlgItem(IDC_INIT_BUTTON)->EnableWindow();
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESET_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONTINUE_BUTTON)->EnableWindow(FALSE);
	return 0;
}
