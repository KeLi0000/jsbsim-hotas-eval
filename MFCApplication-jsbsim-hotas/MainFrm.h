
// MainFrm.h: CMainFrame 类的接口
//

#pragma once

#include "ShareVar.h"
#include "CSysCtrlView.h"
#include "CSysJsbsimShowView.h"
#include "CSysHotasShowView.h"
#include "Simulator.h"

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
protected:
	CSplitterWnd m_wndSplitter1;
	CSplitterWnd m_wndSplitter2;

	CSysCtrlView* m_pCtrlView{ nullptr };	// 控制区域
	CSysJsbsimShowView* m_pJsbsimView{ nullptr };	// 信息区域
	CSysHotasShowView* m_pHotasView{ nullptr };	// 显示区域

	CSimulator* m_pSimulator{ nullptr };// 仿真管理对象指针

	sSyncParams m_SyncParams;// 系统参数指针
	void SyncParamsPtr();	// 同步系统参数指针
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnUmSystemInit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSystemStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSystemPause(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSystemCont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSystemStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSystemReset(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmHotasCreateSuccessful(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmHotasCreateFailed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmJsbsimCreateSuccessful(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmJsbsimCreateFailed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmHotasStepFailed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmHotasStepUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmJsbsimStepUpdate(WPARAM wParam, LPARAM lParam);
};


