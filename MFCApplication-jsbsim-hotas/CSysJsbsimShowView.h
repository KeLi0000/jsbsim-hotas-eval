#pragma once

#include "ShareVar.h"
#include "HSChartCtrl\ChartCtrl.h"
#include "HSChartCtrl\ChartPointsSerie.h"
#include "HSChartCtrl\ChartLineSerie.h"
#include "HSChartCtrl\ChartAxisLabel.h"
#include "HSChartCtrl\ChartGrid.h"
#include <vector>

// CSysJsbsimShowView 窗体视图

class CSysJsbsimShowView : public CFormView
{
	DECLARE_DYNCREATE(CSysJsbsimShowView)

protected:
	CSysJsbsimShowView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSysJsbsimShowView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JSBSIM_SHOW_DIALOG };
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

	sJsbsimState* m_pJsbsimState{ nullptr };// JSBSim状态指针
	sSyncParams* m_pSyncParams{ nullptr };// 系统共享参数指针

private:
	CChartCtrl m_TrajChartCtrl;// 水平面轨迹图
	bool m_ChartInitUpdate;	// 首次更新Chart标志
	double m_EastMinMax[2];	// 东向最小最大
	double m_NorthMinMax[2];// 北向最小最大
	void InitTrajChart();
	void UpdateTrajChart();
	void Update_CoordMinMax(double lla[3]);
	CChartPointsSerie* m_AcNowPoint;	// 飞机当前位置点
	CChartLineSerie* m_AcTrajLine;

	CChartCtrl m_AltChartCtrl; // 高度随时间变化图
	void InitAltChart();
	void UpdateAltChart();
	CChartLineSerie* m_AltLine;// 高度曲线


public:
	afx_msg void OnBnClickedUpdateAxisButton();
	virtual void OnInitialUpdate();
protected:
	afx_msg LRESULT OnUmJsbsimStepUpdate(WPARAM wParam, LPARAM lParam);
private:
	std::vector<sJsbsimState> m_EnvStateTraj;// 环境状态轨迹缓存
	void UpdateEdits(); // 更新编辑框显示
	double m_EngineN2;
	double m_EngineProp;
	double m_EngineTwr;
	double m_Yaw;
	double m_Pitch;
	double m_Roll;
	double m_Vt;
	double m_Vc;
	double m_Vg;
	double m_Alt;
};


