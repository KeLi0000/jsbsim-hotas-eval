#include "pch.h"
#include "Simulator.h"

#define TIMER_CREATE_HOTAS 1
#define TIMER_RUN_HOTAS 2
int Timer_Create_Hotas_Elapse = 1;
int Timer_Run_Hotas_Elapse = 1;

#define TIMER_CREATE_JSBSIM 3
#define TIMER_RUN_JSBSIM 4
#define TIMER_STEP_JSBSIM 5
int Timer_Create_Jsbsim_Elapse = 1;
int Timer_Run_Jsbsim_Elapse = 1;
int Timer_Step_Jsbsim_Elapse = 10;

IMPLEMENT_DYNAMIC(CSimulator, CWnd)

BEGIN_MESSAGE_MAP(CSimulator, CWnd)
	ON_WM_TIMER()
	ON_MESSAGE(UM_SYSTEM_SYNC_DATA_POINTER, &CSimulator::OnUmSystemSyncDataPointer)
	ON_MESSAGE(UM_SYSTEM_INIT, &CSimulator::OnUmSystemInit)
	ON_MESSAGE(UM_SYSTEM_START, &CSimulator::OnUmSystemStart)
	ON_MESSAGE(UM_SYSTEM_PAUSE, &CSimulator::OnUmSystemPause)
	ON_MESSAGE(UM_SYSTEM_CONT, &CSimulator::OnUmSystemCont)
	ON_MESSAGE(UM_SYSTEM_STOP, &CSimulator::OnUmSystemStop)
	ON_MESSAGE(UM_SYSTEM_RESET, &CSimulator::OnUmSystemReset)
END_MESSAGE_MAP()


CSimulator::CSimulator(CWnd* pParent)
	: m_pParent(pParent)
	, m_pSyncParams(NULL)
	, m_LastSendTime(0)
	, m_LastStepClock(0)
	, m_NeedMeta(true)
	, m_hCreateHotas(NULL)
	, m_hRunHotas(NULL)
	, m_hCreateJsbsim(NULL)
	, m_hStepJsbsim(NULL)
	, m_NeedStopJsbsim(false)
{
	CreateEx(NULL, _T("STATIC"), _T("Hi_TA"), WS_DISABLED, 5, 5, 30, 30, NULL, NULL);
	ShowWindow(SW_HIDE);
}

void CSimulator::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TIMER_CREATE_HOTAS:
		Check_CreateHotas();
		break;
	case TIMER_CREATE_JSBSIM:
		Check_CreateJsbsim();
		break;
	case TIMER_RUN_HOTAS:
		Check_RunHotas();
		break;
	case TIMER_RUN_JSBSIM:
		Check_RunJsbsim();
		break;
	case TIMER_STEP_JSBSIM:
		Check_StepJsbsim();
		break;
	default:
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

bool CSimulator::Check_Close_Thread(HANDLE& hThread)
{
	DWORD tmp_exit = 0;
	GetExitCodeThread(hThread, &tmp_exit);
	if (tmp_exit != STILL_ACTIVE)
	{
		CloseHandle(hThread);
		hThread = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

void CSimulator::Init_CreateHotas()
{
	if (m_hCreateHotas == NULL)
	{
		m_CreateHotasTParams.m_pHotasObj = &m_Hotas;
		m_CreateHotasTParams.m_InitRtn = false;
		// 开启创建HOTAS线程
		m_hCreateHotas = CreateThread(NULL, 0, CallBack_CreateHotas, &m_CreateHotasTParams, 0, NULL);
		// 打开创建环境定时器
		SetTimer(TIMER_CREATE_HOTAS, Timer_Create_Hotas_Elapse, NULL);
	}
}

void CSimulator::Check_CreateHotas()
{
	if (Check_Close_Thread(m_hCreateHotas))
	{
		KillTimer(TIMER_CREATE_HOTAS);
		if (m_CreateHotasTParams.m_InitRtn)
		{
			m_pSyncParams->m_HotasValid = true;
			// 向框架发送HOTAS创建成功消息
			m_pParent->PostMessage(UM_HOTAS_CREATE_SUCCESSFUL);
		}
		else
		{
			m_pSyncParams->m_HotasValid = false;
			// 向框架发送HOTAS创建失败消息
			m_pParent->PostMessage(UM_HOTAS_CREATE_FAILED);
		}
	}
}

void CSimulator::Init_RunHotas()
{
	if (m_hRunHotas == NULL)
	{
		m_RunHotasTParams.m_pHotasObj = &m_Hotas;
		m_RunHotasTParams.m_StepRtn = false;
		m_RunHotasTParams.m_StopFlag = false;
		// 开启Run HOTAS线程
		m_hRunHotas = CreateThread(NULL, 0, CallBack_RunHotas, &m_RunHotasTParams, 0, NULL);
		// 打开创建环境定时器
		SetTimer(TIMER_RUN_HOTAS, Timer_Run_Hotas_Elapse, NULL);
	}
}

void CSimulator::Check_RunHotas()
{
	if (!m_RunHotasTParams.m_StepRtn) {
		m_pSyncParams->m_HotasValid = false;
		m_pSyncParams->m_HotasState.m_HotasPitch = 0.0;
		m_pSyncParams->m_HotasState.m_HotasRoll = 0.0;
		m_pSyncParams->m_HotasState.m_HotasYaw = 0.0;
		m_pSyncParams->m_HotasState.m_ThrottlePropulsion[0] = 0.0;
		m_pSyncParams->m_HotasState.m_ThrottlePropulsion[1] = 0.0;
		m_pParent->PostMessage(UM_HOTAS_STEP_FAILED);
	}
	else {
		m_pSyncParams->m_HotasValid = true;
		// 读取HOTAS输入数据
		m_pSyncParams->m_HotasState.m_HotasPitch = m_RunHotasTParams.m_HotasElevator;
		m_pSyncParams->m_HotasState.m_HotasRoll = m_RunHotasTParams.m_HotasAileron;
		m_pSyncParams->m_HotasState.m_HotasYaw = m_RunHotasTParams.m_ThrottleRudder;
		m_pSyncParams->m_HotasState.m_ThrottlePropulsion[0] = m_RunHotasTParams.m_ThrottlePropulsion;
		m_pSyncParams->m_HotasState.m_ThrottlePropulsion[1] = m_RunHotasTParams.m_ThrottlePropulsion;
		m_pSyncParams->m_HotasState.m_ThrottleFlaps = m_RunHotasTParams.m_ThrottleFlaps;
		m_pSyncParams->m_HotasState.m_ThrottleEngineRun = m_RunHotasTParams.m_ThrottleEngineStart;
		m_pSyncParams->m_HotasState.m_ThrottleFuleCutoff = m_RunHotasTParams.m_ThrottleFuelCutoff;
		// 发送HOTAS数据更新消息
		m_pParent->PostMessage(UM_HOTAS_STEP_UPDATE);
	}
	if (Check_Close_Thread(m_hRunHotas))
	{
		KillTimer(TIMER_RUN_HOTAS);
	}
}

afx_msg
void CSimulator::Init_CreateJsbsim()
{
	if (m_hCreateJsbsim == NULL)
	{
		m_CreateJsbsimTParams.m_pJsbsimObj = &m_Jsbsim;
		m_CreateJsbsimTParams.m_Dt = SIM_STEP_LENGTH;
		m_CreateJsbsimTParams.m_InitRtn = false;
		// 开启创建环境线程
		m_hCreateJsbsim = CreateThread(NULL, 0, CallBack_CreateJsbsim, &m_CreateJsbsimTParams, 0, NULL);
		// 打开创建环境定时器
		SetTimer(TIMER_CREATE_JSBSIM, Timer_Create_Jsbsim_Elapse, NULL);
		// 创建Tacview文件
		m_TacviewFile.Init("results\\SimResult.acmi");
	}
}

void CSimulator::Check_CreateJsbsim()
{
	if (Check_Close_Thread(m_hCreateJsbsim))
	{
		KillTimer(TIMER_CREATE_JSBSIM);
		if (m_CreateJsbsimTParams.m_InitRtn) {
			m_pSyncParams->m_EnvValid = true;
			// 向框架发送环境创建成功消息
			m_pParent->PostMessage(UM_JSBSIM_CREATE_SUCCESSFUL);
		}
		else {
			m_pSyncParams->m_EnvValid = false;
			// 向框架发送环境创建失败消息
			m_pParent->PostMessage(UM_JSBSIM_CREATE_FAILED);
		}
	}
}

void CSimulator::Init_RunJsbsim()
{
	m_LastSendTime = 0;
	m_LastStepClock = clock();
	// 打开运行环境定时器
	SetTimer(TIMER_RUN_JSBSIM, Timer_Run_Jsbsim_Elapse, NULL);
	// 第一次迭代环境
	Init_StepJsbsim();
}

void CSimulator::Check_RunJsbsim()
{
	if (m_NeedStopJsbsim) { 
		KillTimer(TIMER_RUN_JSBSIM);
		m_pParent->PostMessage(UM_JSBSIM_STEP_UPDATE);
		// 关闭ACMI文件记录
		m_TacviewFile.Close();
		// 关闭Tacview遥传
		m_TacviewServer.CloseRemote();
	}
	else {
		clock_t  nowStepClock = clock();
		double elapsedTime = (1.0 * nowStepClock - 1.0 * m_LastStepClock) / CLOCKS_PER_SEC;
		if (elapsedTime >= SIM_STEP_LENGTH)
		{
			// 若未结束，则更新环境
			Init_StepJsbsim();	// 迭代环境
			m_LastStepClock = clock();	// 记录当前clock
		}
	}
}

void CSimulator::Init_StepJsbsim()
{
	if (m_hStepJsbsim == NULL)
	{
		// 赋环境控制量
		m_StepJsbsimTParams.m_AileronNormCmd = m_pHotasState->m_HotasRoll;
		m_StepJsbsimTParams.m_ElevatorNormCmd = m_pHotasState->m_HotasPitch;
		m_StepJsbsimTParams.m_RudderNormCmd = m_pHotasState->m_HotasYaw;
		m_StepJsbsimTParams.m_ThrottleNormCmd = m_pHotasState->m_ThrottlePropulsion[0];
		if (m_pHotasState->m_ThrottleFlaps == 0) {
			m_StepJsbsimTParams.m_FlapsCmd = 0.0;
		}
		else if (m_pHotasState->m_ThrottleFlaps == 1) {
			m_StepJsbsimTParams.m_FlapsCmd = 0.25;
		}
		else if (m_pHotasState->m_ThrottleFlaps == 2) {
			m_StepJsbsimTParams.m_FlapsCmd = 0.5;
		}
		else {
			m_StepJsbsimTParams.m_FlapsCmd = 0.0;
		}
		m_StepJsbsimTParams.m_EngineStartCmd = m_pHotasState->m_ThrottleEngineRun;
		m_StepJsbsimTParams.m_FuelCutoffCmd = m_pHotasState->m_ThrottleFuleCutoff;
		// 赋其他值
		m_StepJsbsimTParams.m_pJsbsimObj = &m_Jsbsim;
		m_StepJsbsimTParams.m_StepRtn = false;
		// 开启Step环境线程
		m_hStepJsbsim = CreateThread(NULL, 0, CallBack_StepJsbsim, &m_StepJsbsimTParams, 0, NULL);
		// 打开创建环境定时器
		SetTimer(TIMER_STEP_JSBSIM, Timer_Step_Jsbsim_Elapse, NULL);
	}
}

void CSimulator::Check_StepJsbsim()
{
	if (Check_Close_Thread(m_hStepJsbsim))
	{
		// 更新仿真时间
		m_pSyncParams->m_SimTime = m_Jsbsim.Get_SimTime();
		KillTimer(TIMER_STEP_JSBSIM);
		// 读迭代状态
		m_pSyncParams->m_EnvValid = m_StepJsbsimTParams.m_StepRtn;
		m_pJsbsimState->m_Time = m_pSyncParams->m_SimTime;
		m_pJsbsimState->m_Lng = m_StepJsbsimTParams.m_Lng;
		m_pJsbsimState->m_Lat = m_StepJsbsimTParams.m_Lat;
		m_pJsbsimState->m_Alt = m_StepJsbsimTParams.m_Alt;
		m_pJsbsimState->m_Yaw = m_StepJsbsimTParams.m_Yaw;
		m_pJsbsimState->m_Pitch = m_StepJsbsimTParams.m_Pitch;
		m_pJsbsimState->m_Roll = m_StepJsbsimTParams.m_Roll;
		m_pJsbsimState->m_Vt = m_StepJsbsimTParams.m_Vt;
		m_pJsbsimState->m_Vc = m_StepJsbsimTParams.m_Vc;
		m_pJsbsimState->m_Vg = m_StepJsbsimTParams.m_Vg;
		m_pJsbsimState->m_N2 = m_StepJsbsimTParams.m_N2;
		m_pJsbsimState->m_P = m_StepJsbsimTParams.m_P;
		m_pJsbsimState->m_Twr = m_StepJsbsimTParams.m_Twr;
		// 发送数据更新消息
		double tmp_elapsedTime = m_pJsbsimState->m_Time - m_LastSendTime;
		if (tmp_elapsedTime > SIM_JSBSIM_UPDATE_LENGTH) {
			m_LastSendTime = m_pJsbsimState->m_Time;
			m_pParent->PostMessage(UM_JSBSIM_STEP_UPDATE);
			Send_TacviewServer();
			Write_TacviewFrame();
		}
	}
}

void CSimulator::Write_TacviewFrame()
{
	// 向acmi文件写
	sTACV_AGENT_DP frame[8];
	uint8_t tmp_frameCnt = 0;
	sTACV_EVENT_DP events[16];
	uint8_t tmp_eventsCnt = 0;
	// 发送飞机信息
	sTACV_AGENT_DP tmp_agent;
	{
		// 我方飞机信息
		tmp_agent.m_ID = 1;
		tmp_agent.m_Longitude = m_pJsbsimState->m_Lng;
		tmp_agent.m_Latitude = m_pJsbsimState->m_Lat;
		tmp_agent.m_Altitude = m_pJsbsimState->m_Alt;
		tmp_agent.m_RollDeg = m_StepJsbsimTParams.m_Roll;
		tmp_agent.m_PitchDeg = m_StepJsbsimTParams.m_Pitch;
		tmp_agent.m_YawDeg = m_StepJsbsimTParams.m_Yaw;
		strcpy(tmp_agent.m_Name, "F-16");
		strcpy(tmp_agent.m_Color, "Blue");
		frame[tmp_frameCnt++] = tmp_agent;
	}	
	m_TacviewFile.Run(m_pSyncParams->m_SimTime, frame, tmp_frameCnt, events, tmp_eventsCnt);
}

void CSimulator::Init_TacviewServer()
{
	// 配置TCP服务器端监听参数
	SOCKADDR_IN tmp_server;
	memset(&tmp_server, 0, sizeof(SOCKADDR_IN));
	tmp_server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	tmp_server.sin_family = AF_INET;
	tmp_server.sin_port = htons(42674);
	m_TacviewServer.SetServerInfo(tmp_server);
	// 开启监听
	m_TacviewServer.StartListen();
	m_NeedMeta = true;	// 设置需要给客户端发送Meta信息
	// 清空发送数据序列
	for (size_t i = 0; i < m_TacviewSendQueue.size(); i++)
	{
		m_TacviewSendQueue.pop();
	}
}

void CSimulator::Send_TacviewServer()
{
	char tmp_sendRowBuf[1024] = { 0 };
	// 发送Meta信息
	if (m_NeedMeta) {
		sprintf(tmp_sendRowBuf, "FileType=text/acmi/tacview\nFileVersion=2.2\n0,ReferenceTime=2024-02-27T09:00:00Z\n");
		Send_TacviewRow(tmp_sendRowBuf, strlen(tmp_sendRowBuf));
		m_NeedMeta = false;
	}
	// 发送帧头
	memset(tmp_sendRowBuf, 0, 1024);
	sprintf(tmp_sendRowBuf, "#%.3f\n", m_pSyncParams->m_SimTime);
	Send_TacviewRow(tmp_sendRowBuf, strlen(tmp_sendRowBuf));
	// 发送飞机信息
	// 我方飞机
	memset(tmp_sendRowBuf, 0, 1024);
	// 设置平台基础参数字段
	sprintf(
		tmp_sendRowBuf,
		"%d,T=%.9f|%.9f|%.9f|%.3f|%.3f|%.3f,Name=F-16,Color=Blue",
		1, m_pJsbsimState->m_Lng, m_pJsbsimState->m_Lat, m_pJsbsimState->m_Alt,
		m_StepJsbsimTParams.m_Roll, m_StepJsbsimTParams.m_Pitch, m_StepJsbsimTParams.m_Yaw
	);
	strcat(tmp_sendRowBuf, "\n");
	Send_TacviewRow(tmp_sendRowBuf, strlen(tmp_sendRowBuf));
}

void CSimulator::Send_TacviewRow(char* row_buf, uint16_t row_len)
{
	m_TacviewSendQueue.push(std::string(row_buf));
	if (m_TacviewServer.IsRemote(0) && m_TacviewServer.IsRecv(0)) {
		for (size_t i = 0; i < m_TacviewSendQueue.size(); i++)
		{
			std::string tmp_rowBuf = m_TacviewSendQueue.front();
			m_TacviewServer.SetSendBuffer(0, (char*)(tmp_rowBuf.c_str()), tmp_rowBuf.length());
			m_TacviewServer.SetSend(0);
			while (m_TacviewServer.GetSend(0));
			m_TacviewSendQueue.pop();
		}
	}
}

afx_msg LRESULT CSimulator::OnUmSystemSyncDataPointer(WPARAM wParam, LPARAM lParam)
{
	m_pSyncParams = (sSyncParams*)wParam;
	m_pHotasState = &m_pSyncParams->m_HotasState;
	m_pJsbsimState = &m_pSyncParams->m_EnvState;
	return 0;
}

LRESULT CSimulator::OnUmSystemInit(WPARAM wParam, LPARAM lParam)
{
	Init_CreateHotas();	// 创建HOTAS
	Init_CreateJsbsim();// 创建JSBSim环境
	Init_TacviewServer();// 初始化Tacview TCP服务端
	return 0;
}

afx_msg LRESULT CSimulator::OnUmSystemStart(WPARAM wParam, LPARAM lParam)
{
	Init_RunHotas();	// 初始化运行HOTAS
	Init_RunJsbsim();	// 初始化运行环境
	return 0;
}

afx_msg LRESULT CSimulator::OnUmSystemPause(WPARAM wParam, LPARAM lParam)
{
	// 关闭运行环境定时器
	KillTimer(TIMER_RUN_JSBSIM);	
	return 0;
}


afx_msg LRESULT CSimulator::OnUmSystemCont(WPARAM wParam, LPARAM lParam)
{
	// 关闭运行环境定时器
	SetTimer(TIMER_RUN_JSBSIM, Timer_Run_Jsbsim_Elapse, NULL);
	return 0;
}


afx_msg LRESULT CSimulator::OnUmSystemStop(WPARAM wParam, LPARAM lParam)
{
	// 关闭仿真环境
	m_NeedStopJsbsim = true;
	m_RunHotasTParams.m_StopFlag = true;		
	return 0;
}


afx_msg LRESULT CSimulator::OnUmSystemReset(WPARAM wParam, LPARAM lParam)
{
	// 关闭仿真环境
	// 重置迭代相关标志
	m_LastStepClock = 0;
	// 重置Tacview交互相关信息
	m_NeedMeta = true;
	m_LastSendTime = 0;
	m_NeedStopJsbsim = false;
	while (m_TacviewSendQueue.empty() == false) m_TacviewSendQueue.pop();
	// 关闭Tacview文件
	m_TacviewFile.Close();
	// 关闭Tacview Server连接
	m_TacviewServer.CloseRemote();
	// 重置其他
	return 0;
}
