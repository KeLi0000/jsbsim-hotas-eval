#pragma once

#include "pch.h"
#include <Windows.h>
#include <Mmsystem.h>
#include <cstdint>
#include "CMyHotas.h"
#include "CMyJSBSim.h"

/*
 * HOTAS设备初始化线程参数
 */
typedef struct HotasCreate_ThreadParams{
	CMyHotas* m_pHotasObj;	// HOTAS设备对象指针
	bool m_InitRtn;	// 初始化返回值
} sHOTASCParams;

DWORD WINAPI CallBack_CreateHotas(LPVOID lpParameter);

/*
 * HOTAS设备轮询线程参数
 */
typedef struct HotasStep_ThreadParams {
	bool m_StopFlag;	// 线程停止标志
	CMyHotas* m_pHotasObj;// HOTAS设备对象指针
	bool m_StepRtn;		// Step返回值
	double m_HotasElevator;// HOTAS升降舵轴值,Pitch 通道
	double m_HotasAileron;// HOTAS副翼轴值,Roll 通道
	bool m_HotasFire;// HOTAS扳机按键状态

	double m_ThrottlePropulsion;// THROTTLE油门轴值,Throttle 通道
	double m_ThrottleRudder;// THROTTLE方向舵轴值,Rudder 通道
	int m_ThrottleFlaps;// THROTTLE襟翼档位
	bool m_ThrottleEngineStart;// 发动机启动开关状态
	bool m_ThrottleFuelCutoff;// 燃油切断开关状态
} sHOTASSParams;

DWORD WINAPI CallBack_RunHotas(LPVOID lpParameter);

/*
 * JSBSim初始化线程参数
 */
typedef struct JsbsimCreate_ThreadParams {
	CMyJSBSim* m_pJsbsimObj;	// JSBSim对象指针
	double m_Dt;				// 仿真步长	
	bool m_InitRtn;	// 初始化返回值
} sJSBSIMCParams;
DWORD WINAPI CallBack_CreateJsbsim(LPVOID lpParameter);

/*
 * JSBSim Step线程参数
 */
typedef struct JsbsimStep_ThreadParams {
	double m_AileronNormCmd;	// 副翼归一化控制输入，范围[-1,1]
	double m_ElevatorNormCmd;	// 升降舵归一化控制输入，范围[-1,1]
	double m_RudderNormCmd;		// 方向舵归一化控制输入，范围[-1,1]
	double m_ThrottleNormCmd;	// 油门归一化控制输入，范围[0,1]
	double m_FlapsCmd;			// 襟翼控制输入，范围[0,1]
	bool m_EngineStartCmd;	// 发动机启动控制输入，true=启动，false=关闭
	bool m_FuelCutoffCmd;	// 燃油切断控制输入，true=切断，false=正常供油
	CMyJSBSim* m_pJsbsimObj;	// JSBSim对象指针
	double m_Lng;		// 经度
	double m_Lat;		// 纬度
	double m_Alt;		// 海拔高度
	double m_Vt;		// 真空速
	double m_Vc;		// 表速
	double m_Vg;		// 地速
	double m_Yaw;		// 方位角
	double m_Pitch;		// 俯仰角
	double m_Roll;		// 横滚角
	double m_N2;	// 发动机N2转速
	double m_P;		// 发动机推力，lbs
	double m_Twr;	// 推重比
	bool m_StepRtn;	// Step返回值
} sJSBSIMSParams;
DWORD WINAPI CallBack_StepJsbsim(LPVOID lpParameter);
