// CSysJsbsimShowView.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication-jsbsim-hotas.h"
#include "CSysJsbsimShowView.h"
#include "IAT_Math.h"


// CSysJsbsimShowView

IMPLEMENT_DYNCREATE(CSysJsbsimShowView, CFormView)

CSysJsbsimShowView::CSysJsbsimShowView()
	: CFormView(IDD_JSBSIM_SHOW_DIALOG)
	, m_EngineN2(0)
	, m_EngineProp(0)
	, m_EngineTwr(0)
	, m_Yaw(0)
	, m_Pitch(0)
	, m_Roll(0)
	, m_Vt(0)
	, m_Vc(0)
	, m_Vg(0)
	, m_Alt(0)
{

}

CSysJsbsimShowView::~CSysJsbsimShowView()
{
}

void CSysJsbsimShowView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_TrajChartCtrl);
	DDX_Control(pDX, IDC_CUSTOM2, m_AltChartCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_EngineN2);
	DDX_Text(pDX, IDC_EDIT2, m_EngineProp);
	DDX_Text(pDX, IDC_EDIT3, m_EngineTwr);
	DDX_Text(pDX, IDC_EDIT4, m_Yaw);
	DDX_Text(pDX, IDC_EDIT5, m_Pitch);
	DDX_Text(pDX, IDC_EDIT6, m_Roll);
	DDX_Text(pDX, IDC_EDIT7, m_Vt);
	DDX_Text(pDX, IDC_EDIT8, m_Vc);
	DDX_Text(pDX, IDC_EDIT9, m_Vg);
	DDX_Text(pDX, IDC_EDIT10, m_Alt);
}

BEGIN_MESSAGE_MAP(CSysJsbsimShowView, CFormView)
	ON_MESSAGE(UM_SYSTEM_SYNC_DATA_POINTER, &CSysJsbsimShowView::OnUmSystemSyncDataPointer)
	ON_BN_CLICKED(IDC_UPDATE_AXIS_BUTTON, &CSysJsbsimShowView::OnBnClickedUpdateAxisButton)
	ON_MESSAGE(UM_JSBSIM_STEP_UPDATE, &CSysJsbsimShowView::OnUmJsbsimStepUpdate)
END_MESSAGE_MAP()


// CSysJsbsimShowView 诊断

#ifdef _DEBUG
void CSysJsbsimShowView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSysJsbsimShowView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSysJsbsimShowView 消息处理程序

afx_msg LRESULT CSysJsbsimShowView::OnUmSystemSyncDataPointer(WPARAM wParam, LPARAM lParam)
{
	m_pSyncParams = (sSyncParams*)wParam;
	m_pJsbsimState = &m_pSyncParams->m_EnvState;
	return 0;
}

void CSysJsbsimShowView::InitTrajChart()
{
	m_TrajChartCtrl.EnableRefresh(false);
	m_EastMinMax[0] = 122.0;
	m_EastMinMax[1] = 122.1;
	m_NorthMinMax[0] = 47;
	m_NorthMinMax[1] = 47.1;
	/// 横轴
	CChartStandardAxis* pBottomAxis = m_TrajChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	CChartAxisLabel* BottomAxisLabel = pBottomAxis->GetLabel();
	BottomAxisLabel->SetText(_T("经度"));
	pBottomAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pBottomAxis->SetMinMax(m_EastMinMax[0], m_EastMinMax[1]);
	pBottomAxis->GetGrid()->SetVisible(TRUE);
	pBottomAxis->SetTickIncrement(TRUE, 0.1);

	/// 纵轴
	CChartStandardAxis* pLeftAxis = m_TrajChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	CChartAxisLabel* LeftAxisLabel = pLeftAxis->GetLabel();
	LeftAxisLabel->SetText(_T("纬度"));
	pLeftAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pLeftAxis->SetMinMax(m_NorthMinMax[0], m_NorthMinMax[1]);
	pLeftAxis->GetGrid()->SetVisible(TRUE);
	pLeftAxis->SetTickIncrement(TRUE, 0.1);

	/// 图例	
	CChartLegend* pLegend = m_TrajChartCtrl.GetLegend();
	pLegend->SetVisible(true);
	pLegend->DockLegend(CChartLegend::dsDockBottom);
	pLegend->SetHorizontalMode(true);

	// 创建载机的轨迹
	m_AcTrajLine = m_TrajChartCtrl.CreateLineSerie();
	m_AcTrajLine->SetSeriesOrdering(poNoOrdering);
	m_AcTrajLine->SetWidth(2);
	m_AcTrajLine->SetColor(0x0000ff);
	m_AcTrajLine->SetName(_T("飞机轨迹"));
	// 创建当前位置点
	m_AcNowPoint = m_TrajChartCtrl.CreatePointsSerie();
	m_AcNowPoint->SetSeriesOrdering(poNoOrdering);
	m_AcNowPoint->SetPointSize(8, 8);
	m_AcNowPoint->SetPointType(CChartPointsSerie::ptEllipse);
	m_AcNowPoint->SetColor(0x0000ff);
	m_AcNowPoint->SetBorderColor(0x0000ff);

	m_TrajChartCtrl.EnableRefresh(true);
	m_ChartInitUpdate = false;
}

void CSysJsbsimShowView::UpdateTrajChart()
{
	// 关闭Chart更新
	m_TrajChartCtrl.EnableRefresh(false);
	// 更新飞机
	auto nowState = m_EnvStateTraj[m_EnvStateTraj.size() - 1];
	double lla[3] = { nowState.m_Lng, nowState.m_Lat, nowState.m_Alt };
	m_AcNowPoint->ClearSerie();// 清空当前位置点
	m_AcNowPoint->AddPoint(lla[0], lla[1]);// 设置当前位置
	m_AcTrajLine->AddPoint(lla[0], lla[1]);// 增加轨迹点
	Update_CoordMinMax(lla);	
	// 打开Chart更新
	m_TrajChartCtrl.EnableRefresh(true);
}

void CSysJsbsimShowView::Update_CoordMinMax(double lla[3])
{
	if (lla[0] > m_EastMinMax[1]) {
		m_EastMinMax[1] = lla[0];
	}	if (lla[0] < m_EastMinMax[0]) {
		m_EastMinMax[0] = lla[0];
	}
	if (lla[1] > m_NorthMinMax[1]) {
		m_NorthMinMax[1] = lla[1];
	}	if (lla[1] < m_NorthMinMax[0]) {
		m_NorthMinMax[0] = lla[1];
	}
}

void CSysJsbsimShowView::InitAltChart()
{
	m_AltChartCtrl.EnableRefresh(false);
	/// 横轴
	CChartStandardAxis* pBottomAxis = m_AltChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	CChartAxisLabel* BottomAxisLabel = pBottomAxis->GetLabel();
	BottomAxisLabel->SetText(_T("时间/t"));
	pBottomAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pBottomAxis->SetMinMax(0, 5);
	pBottomAxis->GetGrid()->SetVisible(TRUE);
	pBottomAxis->SetTickIncrement(TRUE, 1);

	/// 纵轴
	CChartStandardAxis* pLeftAxis = m_AltChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	CChartAxisLabel* LeftAxisLabel = pLeftAxis->GetLabel();
	LeftAxisLabel->SetText(_T("海拔高度/m"));
	pLeftAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pLeftAxis->SetMinMax(-50, 2000);
	pLeftAxis->GetGrid()->SetVisible(TRUE);

	/// 图例	
	CChartLegend* pLegend = m_AltChartCtrl.GetLegend();
	pLegend->SetVisible(true);
	pLegend->DockLegend(CChartLegend::dsDockBottom);
	pLegend->SetHorizontalMode(true);

	// 创建海拔高度曲线
	m_AltLine = m_AltChartCtrl.CreateLineSerie();
	m_AltLine->SetSeriesOrdering(poNoOrdering);
	m_AltLine->SetWidth(2);
	m_AltLine->SetColor(0x0000ff);

	m_AltChartCtrl.EnableRefresh(true);
}

void CSysJsbsimShowView::UpdateAltChart()
{
	// 关闭Chart更新
	m_AltChartCtrl.EnableRefresh(false);
	// 更新飞机
	auto nowState = m_EnvStateTraj[m_EnvStateTraj.size() - 1];
	m_AltLine->AddPoint(nowState.m_Time, nowState.m_Alt);// 增加轨迹点
	double xMinMax[2] = { 0, 5 };
	m_AltLine->GetSerieXMinMax(xMinMax[0], xMinMax[1]);
	if (m_AltLine->GetPointsCount() > 0) {
		xMinMax[1] = xMinMax[1] > (xMinMax[0] + 5) ? xMinMax[1] + 1 : (xMinMax[0] + 5);
	}
	else {
		xMinMax[0] = 0;
		xMinMax[1] = 5;
	}
	CChartStandardAxis* pBottomAxis = (CChartStandardAxis*)m_AltChartCtrl.GetBottomAxis();
	pBottomAxis->SetMinMax(xMinMax[0], xMinMax[1]);
	// 打开Chart更新
	m_AltChartCtrl.EnableRefresh(true);
}

void CSysJsbsimShowView::OnBnClickedUpdateAxisButton()
{
	// 调整轨迹绘图坐标轴
	double tmp_northLength = m_NorthMinMax[1] - m_NorthMinMax[0];
	tmp_northLength = tmp_northLength > 1e-6 ? tmp_northLength : 1;
	double tmp_eastLength = m_EastMinMax[1] - m_EastMinMax[0];
	tmp_eastLength = tmp_eastLength > 1e-6 ? tmp_eastLength : 1;
	double tmp_axisLength = tmp_northLength > tmp_eastLength ? tmp_northLength : tmp_eastLength;
	double tmp_northMid = (m_NorthMinMax[1] + m_NorthMinMax[0]) / 2;
	m_NorthMinMax[1] = tmp_northMid + tmp_axisLength / 2;
	m_NorthMinMax[0] = tmp_northMid - tmp_axisLength / 2;
	CChartStandardAxis* pLeftAxis = (CChartStandardAxis*)m_TrajChartCtrl.GetLeftAxis();
	pLeftAxis->SetMinMax(m_NorthMinMax[0] - 0.1, m_NorthMinMax[1] + 0.1);
	double tmp_eastMid = (m_EastMinMax[1] + m_EastMinMax[0]) / 2;
	m_EastMinMax[1] = tmp_eastMid + tmp_axisLength / 2;
	m_EastMinMax[0] = tmp_eastMid - tmp_axisLength / 2;
	CChartStandardAxis* pBottomAxis = (CChartStandardAxis*)m_TrajChartCtrl.GetBottomAxis();
	pBottomAxis->SetMinMax(m_EastMinMax[0] - 0.1, m_EastMinMax[1] + 0.1);
}

void CSysJsbsimShowView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	InitTrajChart();
	InitAltChart();
}

afx_msg LRESULT CSysJsbsimShowView::OnUmJsbsimStepUpdate(WPARAM wParam, LPARAM lParam)
{
	m_EnvStateTraj.push_back(m_pSyncParams->m_EnvState);
	UpdateTrajChart();
	UpdateAltChart();
	UpdateEdits();
	return 0;
}

void CSysJsbsimShowView::UpdateEdits()
{
	m_EngineN2 = LkRound(m_pJsbsimState->m_N2, 3);
	m_EngineProp = LkRound(m_pJsbsimState->m_P, 1);
	m_EngineTwr = LkRound(m_pJsbsimState->m_Twr, 3);
	m_Yaw = LkRound(m_pJsbsimState->m_Yaw, 2);
	m_Pitch = LkRound(m_pJsbsimState->m_Pitch, 2);
	m_Roll = LkRound(m_pJsbsimState->m_Roll, 2);
	m_Vt = LkRound(m_pJsbsimState->m_Vt, 2);
	m_Vc = LkRound(m_pJsbsimState->m_Vc, 2);
	m_Vg = LkRound(m_pJsbsimState->m_Vg, 2);
	m_Alt = LkRound(m_pJsbsimState->m_Alt, 2);
	UpdateData(FALSE);
}
