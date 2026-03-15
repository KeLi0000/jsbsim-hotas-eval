#include "pch.h"
#include "SimThread.h"

DWORD WINAPI CallBack_CreateHotas(LPVOID lpParameter)
{
	sHOTASCParams* tmp_threadParams = (sHOTASCParams*)lpParameter;
	tmp_threadParams->m_InitRtn = tmp_threadParams->m_pHotasObj->Init();
	return 0;
}

DWORD WINAPI CallBack_RunHotas(LPVOID lpParameter)
{
	sHOTASSParams* tmp_threadParams = (sHOTASSParams*)lpParameter;
	while (true)
	{
		if (tmp_threadParams->m_StopFlag) {
			break;
		}
		tmp_threadParams->m_StepRtn = tmp_threadParams->m_pHotasObj->Step();
		if (tmp_threadParams->m_StepRtn) {
			tmp_threadParams->m_HotasElevator = -tmp_threadParams->m_pHotasObj->GetHotasAxis(1) * 0.5; // Pitch
			tmp_threadParams->m_HotasAileron = tmp_threadParams->m_pHotasObj->GetHotasAxis(0) * 0.5; // Roll
			tmp_threadParams->m_HotasFire = tmp_threadParams->m_pHotasObj->GetHotasButton(5); // Trigger

			tmp_threadParams->m_ThrottlePropulsion = (-tmp_threadParams->m_pHotasObj->GetThrottleAxis(2) - (-1)) / 2; // Throttle
			tmp_threadParams->m_ThrottleRudder = -tmp_threadParams->m_pHotasObj->GetRudderAxis(2); // Rudder
			if (tmp_threadParams->m_pHotasObj->GetThrottleButton(22)) {
				tmp_threadParams->m_ThrottleFlaps = 0; // Flaps 收起
			}
			else if (tmp_threadParams->m_pHotasObj->GetThrottleButton(23)) {
				tmp_threadParams->m_ThrottleFlaps = 2; // Flaps 全出
			}
			else {
				tmp_threadParams->m_ThrottleFlaps = 1; // Flaps 半展
			}
			if (tmp_threadParams->m_pHotasObj->GetThrottleButton(29)) {
				tmp_threadParams->m_ThrottleEngineStart = false; // Engine 关
			}
			else {
				tmp_threadParams->m_ThrottleEngineStart = true; // Engine 开
			}
			if (!tmp_threadParams->m_pHotasObj->GetThrottleButton(17)) {
				tmp_threadParams->m_ThrottleFuelCutoff = true; // Fuel Cutoff 开
			}
			else {
				tmp_threadParams->m_ThrottleFuelCutoff = false; // Fuel Cutoff 关
			}
		}
		else {
			break;
		}
	}
	return 0;
}

DWORD WINAPI CallBack_CreateJsbsim(LPVOID lpParameter)
{
	sJSBSIMCParams* tmp_threadParams = (sJSBSIMCParams*)lpParameter;
	tmp_threadParams->m_pJsbsimObj->Set_Dt(tmp_threadParams->m_Dt);
	tmp_threadParams->m_InitRtn = tmp_threadParams->m_pJsbsimObj->Init();
	return 0;
}

DWORD WINAPI CallBack_StepJsbsim(LPVOID lpParameter)
{
	sJSBSIMSParams* tmp_threadParams = (sJSBSIMSParams*)lpParameter;

	// 单步运行JSBSim，并在成功时采集关键状态
	tmp_threadParams->m_pJsbsimObj->Set_AileronNormCmd(tmp_threadParams->m_AileronNormCmd);
	tmp_threadParams->m_pJsbsimObj->Set_ElevatorNormCmd(tmp_threadParams->m_ElevatorNormCmd);
	tmp_threadParams->m_pJsbsimObj->Set_RudderNormCmd(tmp_threadParams->m_RudderNormCmd);
	tmp_threadParams->m_pJsbsimObj->Set_ThrottleNormCmd(tmp_threadParams->m_ThrottleNormCmd);
	tmp_threadParams->m_pJsbsimObj->Set_FlapsCmd(tmp_threadParams->m_FlapsCmd);
	tmp_threadParams->m_pJsbsimObj->Set_EngineStartCmd(tmp_threadParams->m_EngineStartCmd);
	tmp_threadParams->m_pJsbsimObj->Set_FuelCutoffCmd(tmp_threadParams->m_FuelCutoffCmd);
	tmp_threadParams->m_StepRtn = tmp_threadParams->m_pJsbsimObj->Step();
	if (tmp_threadParams->m_StepRtn) {
		tmp_threadParams->m_Lng = tmp_threadParams->m_pJsbsimObj->Get_Longitude();
		tmp_threadParams->m_Lat = tmp_threadParams->m_pJsbsimObj->Get_Latitude();
		tmp_threadParams->m_Alt = tmp_threadParams->m_pJsbsimObj->Get_Altitude();

		tmp_threadParams->m_Yaw = tmp_threadParams->m_pJsbsimObj->Get_Yaw();
		tmp_threadParams->m_Pitch = tmp_threadParams->m_pJsbsimObj->Get_Pitch();
		tmp_threadParams->m_Roll = tmp_threadParams->m_pJsbsimObj->Get_Roll();

		tmp_threadParams->m_Vt = tmp_threadParams->m_pJsbsimObj->Get_Vt();
		tmp_threadParams->m_Vc = tmp_threadParams->m_pJsbsimObj->Get_Vc();
		tmp_threadParams->m_Vg = tmp_threadParams->m_pJsbsimObj->Get_Vg();

		tmp_threadParams->m_N2 = tmp_threadParams->m_pJsbsimObj->Get_N2();
		tmp_threadParams->m_P = tmp_threadParams->m_pJsbsimObj->Get_Propulsion();
		tmp_threadParams->m_Twr = tmp_threadParams->m_P / (17400 + 230);
	}
	return 0;
}

