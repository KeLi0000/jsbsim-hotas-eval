#pragma once

#include "ShareVar.h"

// CSysCtrlView 窗体视图

class CSysCtrlView : public CFormView
{
	DECLARE_DYNCREATE(CSysCtrlView)

protected:
	CSysCtrlView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSysCtrlView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTRL_DIALOG };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInitButton();
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedPauseButton();
	afx_msg void OnBnClickedContinueButton();
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnBnClickedResetButton();
protected:
	afx_msg LRESULT OnUmSimCreateSuccessful(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSimCreateFailed(WPARAM wParam, LPARAM lParam);
};


