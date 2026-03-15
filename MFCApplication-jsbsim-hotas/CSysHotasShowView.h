#pragma once

#include "ShareVar.h"
#include "HSChartCtrl\ChartCtrl.h"
#include "HSChartCtrl\ChartPointsSerie.h"
#include "HSChartCtrl\ChartLineSerie.h"
#include "HSChartCtrl\ChartAxisLabel.h"
#include "HSChartCtrl\ChartGrid.h"

// CSysHotasShowView 窗体视图

class CSysHotasShowView : public CFormView
{
	DECLARE_DYNCREATE(CSysHotasShowView)

protected:
	CSysHotasShowView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSysHotasShowView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOTAS_SHOW_DIALOG };
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
	afx_msg LRESULT OnUmSystemSyncDataPointer(WPARAM wParam, LPARAM lParam);
private:
	CChartCtrl m_StickChartCtrl;	// HOTAS 操纵杆量
	CChartCtrl m_ThrottleChartCtrl;	// HOTAS 油门量
	CChartCtrl m_RudderChartCtrl;	// HOTAS 踏板量
	CChartPointsSerie* m_StickPoint;	// 操纵杆点
	CChartLineSerie* m_ThrottleLine;	// 油门线
	CChartLineSerie* m_RudderLine;		// 踏板线
	void InitStickChart();
	void InitThrottleChart();
	void InitRudderChart();
	void UpdateStickChart();
	void UpdateThrottleChart();
	void UpdateRudderChart();	

	sSyncParams* m_pSyncParams{ nullptr };// 系统共享参数指针
public:
	virtual void OnInitialUpdate();
protected:
	afx_msg LRESULT OnUmHotasStepUpdate(WPARAM wParam, LPARAM lParam);
};
