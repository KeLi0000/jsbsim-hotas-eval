#pragma once

#include <string>

#include "FGFDMExec.h"

// 封装JSBSim运行的简单接口
class CMyJSBSim
{
public:
	CMyJSBSim();
	virtual ~CMyJSBSim() = default;

	// 设置要加载的机型名称（如："f16"）
	void Set_Aircraft(const char* aircraft);
	// 设置仿真步长（秒）
	void Set_Dt(double dt);
	// 设置初始化reset文件名（如："reset00.xml"）
	void Set_Reset(const char* name);

	void Set_ElevatorNormCmd(double cmd); // 设置升降舵归一化控制输入，范围[-1,1]
	void Set_AileronNormCmd(double cmd); // 设置副翼归一化控制输入，范围[-1,1]
	void Set_RudderNormCmd(double cmd);   // 设置方向舵归一化控制输入，范围[-1,1]
	void Set_ThrottleNormCmd(double cmd); // 设置油门归一化控制输入，范围[-1,1]
	void Set_FlapsCmd(double cmd);    // 设置襟翼控制输入，范围[0,1]
	void Set_EngineStartCmd(bool cmd); // 设置发动机启动控制输入，true=启动，false=关闭
	void Set_FuelCutoffCmd(bool cmd);  // 设置燃油切断控制输入，true=切断，false=正常供油

	// 初始化JSBSim仿真
	bool Init();
	// 运行仿真步进（单步Run），无Sleep
	bool Step();
	// 重置仿真（重新运行IC）
	void Reset();

	// 获取仿真时间
	double Get_SimTime() const;
	// 获取经度（度）
	double Get_Longitude() const;
	// 获取纬度（度）
	double Get_Latitude() const;
	// 获取海拔高（米）
	double Get_Altitude() const;
	// 获取真空速（m/s）
	double Get_Vt() const;
	// 获取表速（m/s）
	double Get_Vc() const;
	// 获取地速（m/s）
	double Get_Vg() const;
	// 获取方位角（度）
	double Get_Yaw() const;
	// 获取俯仰角（度）
	double Get_Pitch() const;
	// 获取横滚角（度）
	double Get_Roll() const;
	// 获取攻角（度）
	double Get_AoA() const;
	// 获取侧滑角（度）
	double Get_Beta() const;

	// 获取NED速度分量（米/秒），数组长度需>=3
	bool Get_NedVelocity(double* vel);

	double Get_N2() const;
	double Get_Propulsion() const;
private:
	bool EnsureInitialized() const;

	JSBSim::FGFDMExec m_FDMExec; // JSBSim执行器
	std::string m_AircraftName{"f16"};			  // 机型名称
	std::string m_ResetFile{"reset00.xml"};	// 初始化reset文件
	double m_Dt{ 1.0 / 120.0 };				  // 仿真步长（秒）
	bool m_Initialized{ false }; 			  // 初始化标记
};

