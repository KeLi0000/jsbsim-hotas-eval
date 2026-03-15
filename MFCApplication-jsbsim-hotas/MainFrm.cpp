
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication-jsbsim-hotas.h"

#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(UM_SYSTEM_INIT, &CMainFrame::OnUmSystemInit)
	ON_MESSAGE(UM_SYSTEM_START, &CMainFrame::OnUmSystemStart)
	ON_MESSAGE(UM_SYSTEM_PAUSE, &CMainFrame::OnUmSystemPause)
	ON_MESSAGE(UM_SYSTEM_CONT, &CMainFrame::OnUmSystemCont)
	ON_MESSAGE(UM_SYSTEM_STOP, &CMainFrame::OnUmSystemStop)
	ON_MESSAGE(UM_SYSTEM_RESET, &CMainFrame::OnUmSystemReset)
	ON_MESSAGE(UM_HOTAS_CREATE_SUCCESSFUL, &CMainFrame::OnUmHotasCreateSuccessful)
	ON_MESSAGE(UM_HOTAS_CREATE_FAILED, &CMainFrame::OnUmHotasCreateFailed)
	ON_MESSAGE(UM_JSBSIM_CREATE_SUCCESSFUL, &CMainFrame::OnUmJsbsimCreateSuccessful)
	ON_MESSAGE(UM_JSBSIM_CREATE_FAILED, &CMainFrame::OnUmJsbsimCreateFailed)
	ON_MESSAGE(UM_HOTAS_STEP_FAILED, &CMainFrame::OnUmHotasStepFailed)
	ON_MESSAGE(UM_HOTAS_STEP_UPDATE, &CMainFrame::OnUmHotasStepUpdate)
	ON_MESSAGE(UM_JSBSIM_STEP_UPDATE, &CMainFrame::OnUmJsbsimStepUpdate)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
	m_pSimulator = new CSimulator(this);
}

CMainFrame::~CMainFrame()
{
	delete m_pSimulator;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

void CMainFrame::SyncParamsPtr()
{
	m_pJsbsimView->PostMessage(UM_SYSTEM_SYNC_DATA_POINTER, (WPARAM)(&m_SyncParams));
	m_pHotasView->PostMessage(UM_SYSTEM_SYNC_DATA_POINTER, (WPARAM)(&m_SyncParams));
	m_pSimulator->PostMessage(UM_SYSTEM_SYNC_DATA_POINTER, (WPARAM)(&m_SyncParams));
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// 设置样式，固定分割条
	if (!m_wndSplitter1.CreateStatic(this, 2, 1))
	{
		MessageBox(_T("一层分屏创建失败！"));
		return FALSE;
	}
	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter1, 1, 2, WS_CHILD | WS_VISIBLE, m_wndSplitter1.IdFromRowCol(1, 0)))
	{
		MessageBox(_T("二层分屏创建失败！"));
		return FALSE;
	}
	// 系统控制窗口
#if DISPLAY_SHOW_RESOLUTION == 0
	if (!m_wndSplitter1.CreateView(0, 0, RUNTIME_CLASS(CSysCtrlView), CSize(300, 75), pContext))
#elif DISPLAY_SHOW_RESOLUTION == 1
	if (!m_wndSplitter1.CreateView(0, 0, RUNTIME_CLASS(CSysCtrlView), CSize(300, 110), pContext))
#endif
	{
		MessageBox(_T("系统控制窗口创建失败！"));
		return FALSE;
	}
	m_pCtrlView = (CSysCtrlView*)m_wndSplitter1.GetPane(0, 0);
	// 输入窗口
#if DISPLAY_SHOW_RESOLUTION == 0
	if (!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CSysJsbsimShowView), CSize(705, 400), pContext))
#elif DISPLAY_SHOW_RESOLUTION == 1
	if (!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CSysJsbsimShowView), CSize(1060, 400), pContext))
#endif
	{
		MessageBox(_T("系统输入窗口创建失败！"));
		return FALSE;
	}
	m_pJsbsimView = (CSysJsbsimShowView*)m_wndSplitter2.GetPane(0, 0);
	// 显示窗口
	if (!m_wndSplitter2.CreateView(0, 1, RUNTIME_CLASS(CSysHotasShowView), CSize(300, 300), pContext))
	{
		MessageBox(_T("系统显示窗口创建失败！"));
		return FALSE;
	}
	m_pHotasView = (CSysHotasShowView*)m_wndSplitter2.GetPane(0, 1);
	// 同步参数指针
	SyncParamsPtr();
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序


afx_msg LRESULT CMainFrame::OnUmSystemInit(WPARAM wParam, LPARAM lParam)
{
	m_pSimulator->SendMessage(UM_SYSTEM_INIT);
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmSystemStart(WPARAM wParam, LPARAM lParam)
{
	m_pSimulator->SendMessage(UM_SYSTEM_START);
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmSystemPause(WPARAM wParam, LPARAM lParam)
{
	m_pSimulator->SendMessage(UM_SYSTEM_PAUSE);
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmSystemCont(WPARAM wParam, LPARAM lParam)
{
	m_pSimulator->SendMessage(UM_SYSTEM_CONT);
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmSystemStop(WPARAM wParam, LPARAM lParam)
{
	m_pSimulator->SendMessage(UM_SYSTEM_STOP);
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmSystemReset(WPARAM wParam, LPARAM lParam)
{
	m_pSimulator->SendMessage(UM_SYSTEM_RESET);
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmHotasCreateSuccessful(WPARAM wParam, LPARAM lParam)
{
	if (m_SyncParams.m_EnvValid && m_SyncParams.m_HotasValid) {
		m_pCtrlView->SendMessage(UM_SIM_CREATE_SUCCESSFUL);
	}
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmHotasCreateFailed(WPARAM wParam, LPARAM lParam)
{
	m_pCtrlView->SendMessage(UM_SIM_CREATE_FAILED);
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmJsbsimCreateSuccessful(WPARAM wParam, LPARAM lParam)
{
	if (m_SyncParams.m_EnvValid && m_SyncParams.m_HotasValid) {
		m_pCtrlView->SendMessage(UM_SIM_CREATE_SUCCESSFUL);
	}
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmJsbsimCreateFailed(WPARAM wParam, LPARAM lParam)
{
	m_pCtrlView->SendMessage(UM_SIM_CREATE_FAILED);
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmHotasStepFailed(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmHotasStepUpdate(WPARAM wParam, LPARAM lParam)
{
	m_pHotasView->SendMessage(UM_HOTAS_STEP_UPDATE);
	return 0;
}

afx_msg LRESULT CMainFrame::OnUmJsbsimStepUpdate(WPARAM wParam, LPARAM lParam)
{
	m_pJsbsimView->SendMessage(UM_JSBSIM_STEP_UPDATE);
	return 0;
}
