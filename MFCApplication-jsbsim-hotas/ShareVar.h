#pragma once

#define SIM_STEP_LENGTH 0.01
#define SIM_JSBSIM_UPDATE_LENGTH 0.1
#define SIM_TACVIEW_UPDATE_LENGTH 0.5

typedef struct JsbsimState_DataPackage {
	double m_Time{ 0 };		// 仿真时间，s
	double m_Lng{ 0 };		// 经度，deg
	double m_Lat{ 0 };		// 纬度，deg
	double m_Alt{ 0 };		// 海拔高度，m
	double m_Yaw{ 0 };		// 方位角，deg
	double m_Pitch{ 0 };	// 俯仰角，deg
	double m_Roll{ 0 };		// 横滚角，deg
	double m_Vt{ 0 };		// 真空速，m/s
	double m_Vc{ 0 };		// 表速，m/s
	double m_Vg{ 0 };		// 地速，m/s

	double m_N2{ 0 };		// 发动机N2转速，%
	double m_P{ 0 };		// 发动机推力，N
	double m_Twr{ 0 };		// 推重比
}sJsbsimState;

typedef struct HotasState_DataPackage {
	double m_HotasPitch{ 0 };	// HOTAS俯仰轴输入
	double m_HotasRoll{ 0 };	// HOTAS滚转轴输入
	double m_HotasYaw{ 0 };		// HOTAS偏航轴输入
	double m_ThrottlePropulsion[2]{ 0 };	// HOTAS油门轴输入
	int m_ThrottleFlaps{ 0 };    // HOTAS襟翼档位，0-全收，1-半出，2-全出
	bool m_ThrottleEngineRun{ false };// HOTAS发动机启动开关
	bool m_ThrottleFuleCutoff{ true };// HOTAS燃油切断开关
}sHotasState;

typedef struct SyncParams_DataPackage {
	double m_SimTime{ 0 };		// 仿真时间,s
	
	bool m_EnvValid{ false };// 环境状态有效标志
	sJsbsimState m_EnvState;	// 环境状态数据包

	bool m_HotasValid{ false };// HOTAS输入有效标志
	sHotasState m_HotasState;	// HOTAS状态数据包
}sSyncParams;

#define UM_SYSTEM_SYNC_DATA_POINTER WM_USER + 1
#define UM_SYSTEM_INIT WM_USER + 2
#define UM_SYSTEM_START WM_USER + 3
#define UM_SYSTEM_PAUSE WM_USER + 4
#define UM_SYSTEM_CONT WM_USER + 5
#define UM_SYSTEM_STOP WM_USER + 6
#define UM_SYSTEM_RESET WM_USER + 7

#define UM_HOTAS_CREATE_SUCCESSFUL WM_USER + 8
#define UM_HOTAS_CREATE_FAILED WM_USER + 9

#define UM_HOTAS_STEP_FAILED WM_USER + 10
#define UM_HOTAS_STEP_UPDATE WM_USER + 14

#define UM_JSBSIM_CREATE_SUCCESSFUL WM_USER + 11
#define UM_JSBSIM_CREATE_FAILED WM_USER + 12

#define UM_JSBSIM_STEP_UPDATE WM_USER + 13

#define UM_SIM_CREATE_SUCCESSFUL WM_USER + 15
#define UM_SIM_CREATE_FAILED WM_USER + 16