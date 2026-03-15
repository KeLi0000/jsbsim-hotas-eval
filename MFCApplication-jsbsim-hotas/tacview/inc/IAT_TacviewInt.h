// IAT_TacviewInt.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#ifdef _MSC_VER
#ifdef IATTACVIEWFILE_EXPORTS
#define IAT_TACVIEWINT_API _declspec(dllexport)
#else
#define IAT_TACVIEWINT_API _declspec(dllimport)
#endif 
#else
#define IAT_TACVIEWINT_API
#endif

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


	typedef struct TacviewAgent_DataPackage {
		uint8_t m_ID;
		char m_Name[32];
		char m_Color[32];
		double m_Longitude;
		double m_Latitude;
		double m_Altitude;
		double m_RollDeg;
		double m_PitchDeg;
		double m_YawDeg;
	}sTACV_AGENT_DP;

	typedef struct TacviewEvent_DataPackage {
		uint8_t m_ID;
		enum EventType {
			Destroyed = 0,
		}m_Event;
	}sTACV_EVENT_DP;

	class IAT_TACVIEWINT_API CTacviewInt {
	public:
		CTacviewInt();
		~CTacviewInt() = default;

		void Init(const char* file_path);

		void Run(double time, sTACV_AGENT_DP frame[8], uint8_t agent_cnt, sTACV_EVENT_DP events[16], uint8_t event_cnt);

		void Close();

	private:
		char m_FilePath[128];
		bool m_IsRunning;
		FILE* m_pFile;
		char m_WriteLine[1024];
	};

#ifdef __cplusplus
}
#endif // __cplusplus
