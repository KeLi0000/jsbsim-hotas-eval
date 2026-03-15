// CSysHotasShowView.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication-jsbsim-hotas.h"
#include "CSysHotasShowView.h"


// CSysHotasShowView

IMPLEMENT_DYNCREATE(CSysHotasShowView, CFormView)

CSysHotasShowView::CSysHotasShowView()
	: CFormView(IDD_HOTAS_SHOW_DIALOG)
{

}

CSysHotasShowView::~CSysHotasShowView()
{
}

void CSysHotasShowView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM6, m_StickChartCtrl);
	DDX_Control(pDX, IDC_CUSTOM4, m_ThrottleChartCtrl);
	DDX_Control(pDX, IDC_CUSTOM5, m_RudderChartCtrl);
}

BEGIN_MESSAGE_MAP(CSysHotasShowView, CFormView)
	ON_MESSAGE(UM_SYSTEM_SYNC_DATA_POINTER, &CSysHotasShowView::OnUmSystemSyncDataPointer)
	ON_MESSAGE(UM_HOTAS_STEP_UPDATE, &CSysHotasShowView::OnUmHotasStepUpdate)
END_MESSAGE_MAP()


// CSysHotasShowView 诊断

#ifdef _DEBUG
void CSysHotasShowView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSysHotasShowView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSysHotasShowView 消息处理程序

afx_msg LRESULT CSysHotasShowView::OnUmSystemSyncDataPointer(WPARAM wParam, LPARAM lParam)
{
	m_pSyncParams = (sSyncParams*)wParam;
	return 0;
}

void CSysHotasShowView::InitStickChart()
{
	m_StickChartCtrl.EnableRefresh(false);
	/// 横轴
	CChartStandardAxis* pBottomAxis = m_StickChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	CChartAxisLabel* BottomAxisLabel = pBottomAxis->GetLabel();
	BottomAxisLabel->SetText(_T("Aileron"));
	pBottomAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pBottomAxis->SetMinMax(-1.1, 1.1);
	pBottomAxis->GetGrid()->SetVisible(TRUE);
	pBottomAxis->SetTickIncrement(TRUE, 0.2);

	/// 纵轴
	CChartStandardAxis* pLeftAxis = m_StickChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	CChartAxisLabel* LeftAxisLabel = pLeftAxis->GetLabel();
	LeftAxisLabel->SetText(_T("Elevator"));
	pLeftAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pLeftAxis->SetMinMax(-1.1, 1.1);
	pLeftAxis->GetGrid()->SetVisible(TRUE);
	pLeftAxis->SetTickIncrement(TRUE, 0.2);

	/// 图例
	CChartLegend* pLegend = m_StickChartCtrl.GetLegend();
	pLegend->SetVisible(true);
	pLegend->DockLegend(CChartLegend::dsDockBottom);
	pLegend->SetHorizontalMode(true);

	// 创建当前Stick位置点
	m_StickPoint = m_StickChartCtrl.CreatePointsSerie();
	m_StickPoint->SetSeriesOrdering(poNoOrdering);
	m_StickPoint->SetPointSize(15, 15);
	m_StickPoint->SetPointType(CChartPointsSerie::ptEllipse);
	m_StickPoint->SetColor(0x00b700);
	m_StickPoint->SetBorderColor(0x00b700);
	m_StickPoint->AddPoint(0, 0);

	// 创建十字参考线
	CChartLineSerie* pHLine = m_StickChartCtrl.CreateLineSerie();
	pHLine->SetSeriesOrdering(poNoOrdering);
	pHLine->SetWidth(2);
	pHLine->SetColor(0x0000ff);
	pHLine->AddPoint(-1.1, 0);
	pHLine->AddPoint(1.1, 0);
	CChartLineSerie* pVLine = m_StickChartCtrl.CreateLineSerie();
	pVLine->SetSeriesOrdering(poNoOrdering);
	pVLine->SetWidth(2);
	pVLine->SetColor(0x0000ff);
	pVLine->AddPoint(0, -1.1);
	pVLine->AddPoint(0, 1.1);

	m_StickChartCtrl.EnableRefresh(true);
}

void CSysHotasShowView::InitThrottleChart()
{
	m_ThrottleChartCtrl.EnableRefresh(false);
	/// 横轴
	CChartStandardAxis* pBottomAxis = m_ThrottleChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	CChartAxisLabel* BottomAxisLabel = pBottomAxis->GetLabel();
	BottomAxisLabel->SetText(_T("时间/t"));
	pBottomAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pBottomAxis->SetMinMax(0, 5);
	pBottomAxis->GetGrid()->SetVisible(TRUE);
	pBottomAxis->SetTickIncrement(TRUE, 1);

	/// 纵轴
	CChartStandardAxis* pLeftAxis = m_ThrottleChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	CChartAxisLabel* LeftAxisLabel = pLeftAxis->GetLabel();
	LeftAxisLabel->SetText(_T("Throttle"));
	pLeftAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pLeftAxis->SetMinMax(0, 1.1);
	pLeftAxis->GetGrid()->SetVisible(TRUE);

	// 创建海拔高度曲线
	m_ThrottleLine = m_ThrottleChartCtrl.CreateLineSerie();
	m_ThrottleLine->SetSeriesOrdering(poNoOrdering);
	m_ThrottleLine->SetWidth(2);
	m_ThrottleLine->SetColor(0x0000ff);

	m_ThrottleChartCtrl.EnableRefresh(true);
}

void CSysHotasShowView::InitRudderChart()
{
	m_RudderChartCtrl.EnableRefresh(false);
	/// 横轴
	CChartStandardAxis* pBottomAxis = m_RudderChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	CChartAxisLabel* BottomAxisLabel = pBottomAxis->GetLabel();
	BottomAxisLabel->SetText(_T("时间/t"));
	pBottomAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pBottomAxis->SetMinMax(0, 5);
	pBottomAxis->GetGrid()->SetVisible(TRUE);
	pBottomAxis->SetTickIncrement(TRUE, 1);

	/// 纵轴
	CChartStandardAxis* pLeftAxis = m_RudderChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	CChartAxisLabel* LeftAxisLabel = pLeftAxis->GetLabel();
	LeftAxisLabel->SetText(_T("Rudder"));
	pLeftAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pLeftAxis->SetMinMax(-1.1, 1.1);
	pLeftAxis->GetGrid()->SetVisible(TRUE);

	// 创建海拔高度曲线
	m_RudderLine = m_RudderChartCtrl.CreateLineSerie();
	m_RudderLine->SetSeriesOrdering(poNoOrdering);
	m_RudderLine->SetWidth(2);
	m_RudderLine->SetColor(0x0000ff);

	m_RudderChartCtrl.EnableRefresh(true);
}

void CSysHotasShowView::UpdateStickChart()
{
	// 关闭Chart更新
	m_StickChartCtrl.EnableRefresh(false);
	// 更新HOTAS位置
	m_StickPoint->ClearSerie();// 清空当前位置点
	m_StickPoint->AddPoint(m_pSyncParams->m_HotasState.m_HotasRoll, m_pSyncParams->m_HotasState.m_HotasPitch);
	// 打开Chart更新
	m_StickChartCtrl.EnableRefresh(true);
}

void CSysHotasShowView::UpdateThrottleChart()
{
	// 关闭Chart更新
	m_ThrottleChartCtrl.EnableRefresh(false);
	// 更新
	m_ThrottleLine->AddPoint(m_pSyncParams->m_SimTime, m_pSyncParams->m_HotasState.m_ThrottlePropulsion[0]);
	double xMinMax[2] = { 0, 5 };
	m_ThrottleLine->GetSerieXMinMax(xMinMax[0], xMinMax[1]);
	if (m_ThrottleLine->GetPointsCount() > 0) {
		xMinMax[1] = xMinMax[1] > (xMinMax[0] + 5) ? xMinMax[1] + 1 : (xMinMax[0] + 5);			
	}
	else {
		xMinMax[0] = 0;
		xMinMax[1] = 5;
	}
	CChartStandardAxis* pBottomAxis = (CChartStandardAxis*)m_ThrottleChartCtrl.GetBottomAxis();
	pBottomAxis->SetMinMax(xMinMax[0], xMinMax[1]);
	// 打开Chart更新
	m_ThrottleChartCtrl.EnableRefresh(true);
}

void CSysHotasShowView::UpdateRudderChart()
{
	// 关闭Chart更新
	m_RudderChartCtrl.EnableRefresh(false);
	// 更新
	m_RudderLine->AddPoint(m_pSyncParams->m_SimTime, m_pSyncParams->m_HotasState.m_HotasYaw);
	double xMinMax[2] = { 0, 5 };
	m_RudderLine->GetSerieXMinMax(xMinMax[0], xMinMax[1]);
	if (m_RudderLine->GetPointsCount() > 0) {
		xMinMax[1] = xMinMax[1] > (xMinMax[0] + 5) ? xMinMax[1] + 1 : (xMinMax[0] + 5);
	}
	else {
		xMinMax[0] = 0;
		xMinMax[1] = 5;
	}
	CChartStandardAxis* pBottomAxis = (CChartStandardAxis*)m_RudderChartCtrl.GetBottomAxis();
	pBottomAxis->SetMinMax(xMinMax[0], xMinMax[1]);
	// 打开Chart更新
	m_RudderChartCtrl.EnableRefresh(true);
}

void CSysHotasShowView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	InitStickChart();
	InitThrottleChart();
	InitRudderChart();
}

afx_msg LRESULT CSysHotasShowView::OnUmHotasStepUpdate(WPARAM wParam, LPARAM lParam)
{
	UpdateStickChart();
	UpdateThrottleChart();
	UpdateRudderChart();
	return 0;
}
