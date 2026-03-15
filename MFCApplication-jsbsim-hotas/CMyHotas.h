#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_joystick.h>

// 定义两个设备的结构体
typedef struct {
	SDL_Joystick* m_Device{ nullptr }; // 设备指针，单位：指针；取值：nullptr或有效SDL_Joystick实例
	SDL_JoystickID m_DeviceId{ 0 };// 设备ID，单位：无；取值范围：SDL分配的整型ID，0表示未初始化
	char m_Name[128]{ 0 };	// 设备名称，单位：字符串；取值范围：最多127字符的UTF-8名称
	int m_AxisCount{ 0 };		// 轴数量，单位：个；取值范围：>=0
	int m_ButtonCount{ 0 };	// 按键数量，单位：个；取值范围：>=0
	int m_HatCount{ 0 };		// 帽子开关数量，单位：个；取值范围：>=0
	double* m_AxesNorm{ nullptr };		// 归一化后的轴值数组，单位：无量纲；取值范围：每轴[-1.0, 1.0]
	Uint8* m_Buttons{ nullptr };		// 按键状态数组，单位：无；取值范围：0(未按)或1(按下)
	Uint8* m_Hats{ nullptr };		// 帽子开关状态数组，单位：方向编码；取值范围：SDL_HAT_*常量（0-8）
} sHOTAS_State;

class CMyHotas
{
public:
	CMyHotas();
	~CMyHotas();
	bool Init();

	void Reset();

	bool Step();

	// 获取HOTAS摇杆指定轴的归一化值，范围[-1,1]
	double GetHotasAxis(int axis_index);
	// 获取HOTAS摇杆指定按键状态，返回true表示按下
	bool GetHotasButton(int button_index);
	// 获取HOTAS摇杆指定帽子开关状态，返回SDL_HAT_*编码
	uint8_t GetHotasHat(int hat_index);

	// 获取油门设备指定轴的归一化值，范围[-1,1]
	double GetThrottleAxis(int axis_index);
	// 获取油门设备指定按键状态，返回true表示按下
	bool GetThrottleButton(int button_index);
	// 获取油门设备指定帽子开关状态，返回SDL_HAT_*编码
	uint8_t GetThrottleHat(int hat_index);

	// 获取脚舵设备指定轴的归一化值，范围[-1,1]
	double GetRudderAxis(int axis_index);

private:

	bool InitHotasDevice();
	void CloseHotasDevice();
	bool UpdateHotasState();
	sHOTAS_State m_HotasDevice; // HOTAS设备

	bool InitThrottleDevice();
	void CloseThrottleDevice();
	bool UpdateThrottleState();
	sHOTAS_State m_ThrottleDevice; // 油门设备

	bool InitRudderDevice();
	void CloseRudderDevice();
	bool UpdateRudderState();
	sHOTAS_State m_RudderDevice; // 脚舵设备

	// 通用的设备状态更新函数
	bool UpdateDeviceState(sHOTAS_State* device);

	bool InitDevice(const char* target_name, sHOTAS_State* device);
	void free_device(sHOTAS_State* device);
	
};

