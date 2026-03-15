#include "pch.h"
#include "CMyHotas.h"

const char* HOTAS_DEVICE_NAME = "Joystick - HOTAS Warthog";
const char* THROTTLE_DEVICE_NAME = "Throttle - HOTAS Warthog";
const char* RUDDER_DEVICE_NAME = "Thrustmaster T-Pendular-Rudder";

// 将SDL返回的有符号16位轴值映射到[-1, 1]
static inline double NormalizeAxisValue(const Sint16 value)
{
	return value >= 0 ? static_cast<double>(value) / 32767.0 : static_cast<double>(value) / 32768.0;
}

double CMyHotas::GetHotasAxis(int axis_index)
{
	// 返回HOTAS摇杆指定轴的归一化值，若索引或缓冲无效返回0
	if (m_HotasDevice.m_AxesNorm && axis_index >= 0 && axis_index < m_HotasDevice.m_AxisCount) {
		return m_HotasDevice.m_AxesNorm[axis_index];
	}
	return 0.0;
}

bool CMyHotas::GetHotasButton(int button_index)
{
	// 返回HOTAS摇杆指定按键状态，true表示按下
	if (m_HotasDevice.m_Buttons && button_index >= 0 && button_index < m_HotasDevice.m_ButtonCount) {
		return m_HotasDevice.m_Buttons[button_index] != 0;
	}
	return false;
}

uint8_t CMyHotas::GetHotasHat(int hat_index)
{
	// 返回HOTAS摇杆指定帽子开关的SDL_HAT编码，若无效返回0
	if (m_HotasDevice.m_Hats && hat_index >= 0 && hat_index < m_HotasDevice.m_HatCount) {
		return m_HotasDevice.m_Hats[hat_index];
	}
	return 0;
}

double CMyHotas::GetThrottleAxis(int axis_index)
{
	// 返回油门设备指定轴的归一化值，若索引或缓冲无效返回0
	if (m_ThrottleDevice.m_AxesNorm && axis_index >= 0 && axis_index < m_ThrottleDevice.m_AxisCount) {
		return m_ThrottleDevice.m_AxesNorm[axis_index];
	}
	return 0.0;
}

bool CMyHotas::GetThrottleButton(int button_index)
{
	// 返回油门设备指定按键状态，true表示按下
	button_index--;
	if (m_ThrottleDevice.m_Buttons && button_index >= 0 && button_index < m_ThrottleDevice.m_ButtonCount) {
		return m_ThrottleDevice.m_Buttons[button_index] != 0;
	}
	return false;
}

uint8_t CMyHotas::GetThrottleHat(int hat_index)
{
	// 返回油门设备指定帽子开关的SDL_HAT编码，若无效返回0
	if (m_ThrottleDevice.m_Hats && hat_index >= 0 && hat_index < m_ThrottleDevice.m_HatCount) {
		return m_ThrottleDevice.m_Hats[hat_index];
	}
	return 0;
}

double CMyHotas::GetRudderAxis(int axis_index)
{
	// 返回油门设备指定轴的值，若索引或缓冲无效返回0
	if (m_RudderDevice.m_AxesNorm && axis_index >= 0 && axis_index < m_RudderDevice.m_AxisCount) {
		return m_RudderDevice.m_AxesNorm[axis_index];
	}
	return 0.0;
}

// 构造函数：当前不分配资源，资源在Init/Reset中集中管理
CMyHotas::CMyHotas()
{
}

// 析构函数：确保调用方可通过Reset/Close释放资源
CMyHotas::~CMyHotas()
{
}

// 初始化SDL子系统，依次打开HOTAS与油门设备，任一失败则回滚
bool CMyHotas::Init()
{
	// 初始化SDL并分别尝试打开摇杆与油门设备
	// 初始化 SDL，只启用游戏手柄功能
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {		
		return false;
	}
	const bool hotas_ok = InitHotasDevice();
	const bool throttle_ok = InitThrottleDevice();
	const bool rudder_ok = InitRudderDevice();

	if (!hotas_ok || !throttle_ok || !rudder_ok) {
		// 清理已打开的设备，避免部分成功导致资源泄露
		CloseHotasDevice();
		CloseThrottleDevice();
		CloseRudderDevice();
		return false;
	}
	return true;
}

// 重置：关闭已打开的两个设备，释放相关资源
void CMyHotas::Reset()
{
	CloseHotasDevice();
	CloseThrottleDevice();
	CloseRudderDevice();
}

// 每帧更新：同步SDL内部状态后分别更新两个设备的输入数据
bool CMyHotas::Step()
{
	// 刷新SDL的输入状态，再读取两个设备的状态
	SDL_UpdateJoysticks();

	bool updated = false;
	if (m_HotasDevice.m_Device && m_ThrottleDevice.m_Device && m_RudderDevice.m_Device) 
	{
		updated = UpdateHotasState() && UpdateThrottleState() && UpdateRudderState();
	}

	return updated;
}

// 打开HOTAS摇杆设备，若已打开则返回false避免重复
bool CMyHotas::InitHotasDevice()
{
	if (!m_HotasDevice.m_Device) {
		return InitDevice(HOTAS_DEVICE_NAME, &m_HotasDevice);
	}
	return false;
}

// 关闭HOTAS设备并释放内存
void CMyHotas::CloseHotasDevice()
{
	free_device(&m_HotasDevice);
}

// 更新HOTAS设备的轴、按键、帽子状态
bool CMyHotas::UpdateHotasState()
{
	return UpdateDeviceState(&m_HotasDevice);
}

// 打开油门设备，若已打开则返回false避免重复
bool CMyHotas::InitThrottleDevice()
{
	if (!m_ThrottleDevice.m_Device) {
		return InitDevice(THROTTLE_DEVICE_NAME, &m_ThrottleDevice);
	}
    return false;
}

// 关闭油门设备并释放内存
void CMyHotas::CloseThrottleDevice()
{
	free_device(&m_ThrottleDevice);
}

// 更新油门设备的轴、按键、帽子状态
bool CMyHotas::UpdateThrottleState()
{
	return UpdateDeviceState(&m_ThrottleDevice);
}

bool CMyHotas::InitRudderDevice()
{
	if (!m_RudderDevice.m_Device) {
		return InitDevice(RUDDER_DEVICE_NAME, &m_RudderDevice);
	}
	return false;
}

void CMyHotas::CloseRudderDevice()
{
	free_device(&m_RudderDevice);
}

bool CMyHotas::UpdateRudderState()
{
	return UpdateDeviceState(&m_RudderDevice);
}

// 通用设备更新：读取轴、按键、帽子并写入缓存，轴值做归一化
bool CMyHotas::UpdateDeviceState(sHOTAS_State* device)
{
	if (!device || !device->m_Device) {
		return false;
	}

	for (int i = 0; i < device->m_AxisCount && device->m_AxesNorm; ++i) {
		// SDL轴值范围为[0, 65535]，归一化到[-1, 1]
		const Sint16 raw_axis = SDL_GetJoystickAxis(device->m_Device, i);
		device->m_AxesNorm[i] = NormalizeAxisValue(raw_axis);
	}
	for (int i = 0; i < device->m_ButtonCount && device->m_Buttons; ++i) {
		// SDL按键返回0/1，直接存入缓冲
		device->m_Buttons[i] = SDL_GetJoystickButton(device->m_Device, i);
	}
	for (int i = 0; i < device->m_HatCount && device->m_Hats; ++i) {
		// SDL帽子状态为SDL_HAT_*编码，直接存入缓冲
		device->m_Hats[i] = SDL_GetJoystickHat(device->m_Device, i);
	}

	return true;
}


// 初始化设备：按名称匹配并打开目标摇杆，分配输入缓冲
bool CMyHotas::InitDevice(const char* target_name, sHOTAS_State* device) {
	// 遍历当前所有摇杆，找到名称匹配的目标设备
    int joystick_count = 0;
    SDL_JoystickID* joysticks = SDL_GetJoysticks(&joystick_count);

    if (!joysticks) {        
        return false;
    }

    for (int i = 0; i < joystick_count; i++) {
        // 先打开设备获取名称
        SDL_Joystick* opened = SDL_OpenJoystick(joysticks[i]);
        if (!opened) {
            continue;
        }

        const char* name = SDL_GetJoystickName(opened);
        const bool is_target = (name && strstr(name, target_name));

        if (is_target) {
            device->m_Device = opened;
            device->m_DeviceId = joysticks[i];
            strncpy_s(device->m_Name, name, sizeof(device->m_Name) - 1);
            device->m_Name[sizeof(device->m_Name) - 1] = '\0';

			device->m_AxisCount = SDL_GetNumJoystickAxes(device->m_Device);
			device->m_ButtonCount = SDL_GetNumJoystickButtons(device->m_Device);
			device->m_HatCount = SDL_GetNumJoystickHats(device->m_Device);

			// 分配内存（轴值使用double存储归一化结果）
			device->m_AxesNorm = (double*)calloc(device->m_AxisCount, sizeof(double));
			device->m_Buttons = (Uint8*)calloc(device->m_ButtonCount, sizeof(Uint8));
			device->m_Hats = (Uint8*)calloc(device->m_HatCount, sizeof(Uint8));

            SDL_free(joysticks);
            return true;
        }
        else {
            if (name) {
                bool is_our_device = strstr(name, HOTAS_DEVICE_NAME) || strstr(name, THROTTLE_DEVICE_NAME) || strstr(name, RUDDER_DEVICE_NAME);
                if (!is_our_device) {
					// 不是我们要找的设备，关闭它
                    SDL_CloseJoystick(opened);
				}            
            }            
            device->m_Device = NULL;
        }
    }
	SDL_free(joysticks);
	return false;
}

void CMyHotas::free_device(sHOTAS_State* device) {
	if (device->m_Device) {
		SDL_CloseJoystick(device->m_Device);
		device->m_Device = NULL;
	}
	if (device->m_AxesNorm) {
		free(device->m_AxesNorm);
		device->m_AxesNorm = NULL;
	}
	if (device->m_Buttons) {
		free(device->m_Buttons);
		device->m_Buttons = NULL;
	}
	if (device->m_Hats) {
		free(device->m_Hats);
		device->m_Hats = NULL;
	}
	device->m_DeviceId = 0;
	device->m_AxisCount = 0;
	device->m_ButtonCount = 0;
	device->m_HatCount = 0;
	if (sizeof(device->m_Name) > 0) {
		device->m_Name[0] = '\0';
	}
}