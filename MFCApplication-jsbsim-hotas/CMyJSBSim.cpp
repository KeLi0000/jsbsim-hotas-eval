#include "pch.h"
#include "CMyJSBSim.h"

#include "initialization/FGTrim.h"
#include "initialization/FGInitialCondition.h"
#include "models/propulsion/FGEngine.h"
#include "input_output/FGPropertyManager.h"

#include <cstdlib>
#include <string>
#include "IAT_Math.h"

using namespace JSBSim;

// 工具函数：读取环境变量JSBSIM_PATH，若为空则使用当前目录
static SGPath GetRootPath()
{
	const char* env_path = std::getenv("JSBSIM_PATH");
	return env_path ? SGPath(env_path) : SGPath(".");
}

CMyJSBSim::CMyJSBSim()
	: m_FDMExec()
{
}

void CMyJSBSim::Set_Aircraft(const char* aircraft)
{
	if (aircraft) {
		m_AircraftName = aircraft;
	}
}

void CMyJSBSim::Set_Dt(double dt)
{
	if (dt > 0.0) {
		m_Dt = dt;
	}
}

void CMyJSBSim::Set_Reset(const char* name)
{
	if (name) {
		m_ResetFile = name;
	}
}

void CMyJSBSim::Set_ElevatorNormCmd(double cmd)
{
	m_FDMExec.GetPropertyManager()->GetNode("fcs/elevator-cmd-norm")->setDoubleValue(cmd);
	//m_FDMExec.GetPropertyManager()->GetNode("fcs/pitch-trim-cmd-norm")->setDoubleValue(cmd);
}

void CMyJSBSim::Set_AileronNormCmd(double cmd)
{
	m_FDMExec.GetPropertyManager()->GetNode("fcs/aileron-cmd-norm")->setDoubleValue(cmd);
	//m_FDMExec.GetPropertyManager()->GetNode("fcs/roll-trim-cmd-norm")->setDoubleValue(cmd);
}

void CMyJSBSim::Set_RudderNormCmd(double cmd)
{
	m_FDMExec.GetPropertyManager()->GetNode("fcs/rudder-cmd-norm")->setDoubleValue(cmd);
	//m_FDMExec.GetPropertyManager()->GetNode("fcs/yaw-trim-cmd-norm")->setDoubleValue(cmd);
}

void CMyJSBSim::Set_ThrottleNormCmd(double cmd)
{
	m_FDMExec.GetPropertyManager()->GetNode("fcs/throttle-cmd-norm[0]")->setDoubleValue(cmd);
}

void CMyJSBSim::Set_FlapsCmd(double cmd)
{
	m_FDMExec.GetPropertyManager()->GetNode("fcs/flap-cmd-norm")->setDoubleValue(cmd);
}

void CMyJSBSim::Set_EngineStartCmd(bool cmd)
{
	m_FDMExec.GetPropertyManager()->GetNode("propulsion/starter_cmd")->setBoolValue(cmd);
}

void CMyJSBSim::Set_FuelCutoffCmd(bool cmd)
{
	m_FDMExec.GetPropertyManager()->GetNode("propulsion/cutoff_cmd")->setBoolValue(cmd);
}

bool CMyJSBSim::Init()
{
	if (EnsureInitialized()) {
		m_FDMExec.RunIC();
		return true;
	}
	// 重置执行器实例并配置路径
	m_FDMExec.SetRootDir(GetRootPath());
	m_FDMExec.SetAircraftPath(SGPath("aircraft"));
	m_FDMExec.SetEnginePath(SGPath("engine"));
	m_FDMExec.SetSystemsPath(SGPath("systems"));
	m_FDMExec.SetOutputPath(SGPath("."));

	// 设置步长
	m_FDMExec.Setdt(m_Dt);
	m_FDMExec.SetDebugLevel(0);

	// 加载模型
	if (!m_FDMExec.LoadModel(SGPath("aircraft"), SGPath("engine"), SGPath("systems"), m_AircraftName)) {
		return false;
	}

	// 加载初始条件
	auto ic = m_FDMExec.GetIC();
	if (!ic->Load(SGPath(m_ResetFile))) {
		return false;
	}

	// 运行初始条件
	m_FDMExec.RunIC();
	bool result = m_FDMExec.Run();  // MAKE AN INITIAL RUN

	// 起飞准备
	Set_EngineStartCmd(false);
	Set_FuelCutoffCmd(true);
	m_FDMExec.GetPropertyManager()->GetNode("gear/gear-cmd-norm")->setDoubleValue(0.0); // 收起起落架
	m_FDMExec.GetPropertyManager()->GetNode("fcs/speedbrake-cmd-norm")->setDoubleValue(0.0);// 收起扰流板

	m_Initialized = true;
	return true;
}

bool CMyJSBSim::Step()
{
	if (!EnsureInitialized()) {
		return false;
	}
	// 单步推进，无Sleep
	return m_FDMExec.Run();
}

void CMyJSBSim::Reset()
{
	if (!EnsureInitialized()) {
		return;
	}
	else {
		m_FDMExec.RunIC();
	}
}

double CMyJSBSim::Get_SimTime() const
{
	return EnsureInitialized() ? m_FDMExec.GetSimTime() : 0.0;
}

double CMyJSBSim::Get_Longitude() const
{
return EnsureInitialized() ? m_FDMExec.GetPropagate()->GetLongitudeDeg() : 0.0;
}

double CMyJSBSim::Get_Latitude() const
{
return EnsureInitialized() ? m_FDMExec.GetPropagate()->GetGeodLatitudeDeg() : 0.0;
}

double CMyJSBSim::Get_Altitude() const
{
return EnsureInitialized() ? m_FDMExec.GetPropagate()->GetAltitudeASLmeters() : 0.0;
}

double CMyJSBSim::Get_Vt() const
{
	if (!EnsureInitialized()) return 0.0;
	// vt为真空速（ft/s），转换为m/s
	const double vt_fps = m_FDMExec.GetPropertyManager()->GetNode("velocities/vt-fps")->getDoubleValue();
	return FGJSBBase::FeetToMeters(vt_fps);
}

double CMyJSBSim::Get_Vc() const
{
	if (!EnsureInitialized()) return 0.0;
	const double vc_fps = m_FDMExec.GetPropertyManager()->GetNode("velocities/vc-fps")->getDoubleValue();
	return FGJSBBase::FeetToMeters(vc_fps);
}

double CMyJSBSim::Get_Vg() const
{
	if (!EnsureInitialized()) return 0.0;
	const double vg_fps = m_FDMExec.GetPropertyManager()->GetNode("velocities/vg-fps")->getDoubleValue();
	return FGJSBBase::FeetToMeters(vg_fps);
}

double CMyJSBSim::Get_Yaw() const
{
	return EnsureInitialized() ? m_FDMExec.GetPropagate()->GetEulerDeg(3) : 0.0;
}

double CMyJSBSim::Get_Pitch() const
{
	return EnsureInitialized() ? m_FDMExec.GetPropagate()->GetEulerDeg(2) : 0.0;
}

double CMyJSBSim::Get_Roll() const
{
	return EnsureInitialized() ? m_FDMExec.GetPropagate()->GetEulerDeg(1) : 0.0;
}

double CMyJSBSim::Get_AoA() const
{
	if (!EnsureInitialized()) return 0.0;
	// 优先从属性树读取aero/alpha-rad，再转换为度；若失败则回退Propagate
	auto pm = m_FDMExec.GetPropertyManager();
	auto node = pm ? pm->GetNode("aero/alpha-rad", true) : nullptr;
	if (node) {
		return ZkR2D(node->getDoubleValue());
	}
	return 0.0;
}

double CMyJSBSim::Get_Beta() const
{
	if (!EnsureInitialized()) return 0.0;
	// 优先从属性树读取aero/beta-rad，再转换为度；若失败则回退Propagate
	auto pm = m_FDMExec.GetPropertyManager();
	auto node = pm ? pm->GetNode("aero/beta-rad", true) : nullptr;
	if (node) {
		return ZkR2D(node->getDoubleValue());
	}
	return 0.0;
}

bool CMyJSBSim::Get_NedVelocity(double* vel)
{
	if (!EnsureInitialized() || !vel) {
		return false;
	}
	// 返回NED坐标系速度分量，转换为m/s
	vel[0] = FGJSBBase::FeetToMeters(m_FDMExec.GetPropagate()->GetVel(1));
	vel[1] = FGJSBBase::FeetToMeters(m_FDMExec.GetPropagate()->GetVel(2));
	vel[2] = FGJSBBase::FeetToMeters(m_FDMExec.GetPropagate()->GetVel(3));
	return true;
}

double CMyJSBSim::Get_N2() const
{
	if (!EnsureInitialized()) return 0.0;
	// 优先从属性树读取propulsion/engine[0]/n2；若失败则回退Propagate
	auto pm = m_FDMExec.GetPropertyManager();
	auto node = pm ? pm->GetNode("propulsion/engine[0]/n2", true) : nullptr;
	if (node) {
		return node->getDoubleValue();
	}
	return 0.0;
}

double CMyJSBSim::Get_Propulsion() const
{
	if (!EnsureInitialized()) return 0.0;
	// 优先从属性树读取propulsion/engine[0]/thrust-lbs；若失败则回退Propagate
	auto pm = m_FDMExec.GetPropertyManager();
	auto node = pm ? pm->GetNode("propulsion/engine[0]/thrust-lbs", true) : nullptr;
	if (node) {
		return node->getDoubleValue();
	}
	return 0.0;
}

bool CMyJSBSim::EnsureInitialized() const
{
	return m_Initialized;
}
