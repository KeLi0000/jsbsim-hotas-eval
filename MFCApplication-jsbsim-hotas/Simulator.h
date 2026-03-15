#pragma once
#include <afxwin.h>
#include "ShareVar.h"
#include "SimThread.h"
#include "IAT_TacviewInt.h".
#include "IAT_TCPLib_v2.h"
#include "CMyHotas.h"
#include "CMyJSBSim.h"
#include <queue>
#include <string>

class CSimulator : public CWnd
{
	DECLARE_DYNAMIC(CSimulator)

public:
	CSimulator(CWnd* pParent);
	virtual ~CSimulator() = default;

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
    // 安全结束线程
    bool Check_Close_Thread(HANDLE& hThread);
    CWnd* m_pParent;
    sSyncParams* m_pSyncParams;
	sHotasState* m_pHotasState;
	sJsbsimState* m_pJsbsimState;

	// 1、HOTAS创建处理
	HANDLE m_hCreateHotas;
	CMyHotas m_Hotas;
	sHOTASCParams m_CreateHotasTParams{};
	void Init_CreateHotas();
	void Check_CreateHotas();
	// 2、HOTAS轮询处理
	HANDLE m_hRunHotas;
	sHOTASSParams m_RunHotasTParams{};
	void Init_RunHotas();
	void Check_RunHotas();
    // 3、创建环境线程管理
	HANDLE m_hCreateJsbsim;
	sJSBSIMCParams m_CreateJsbsimTParams{};
	CMyJSBSim m_Jsbsim;
    void Init_CreateJsbsim();
    void Check_CreateJsbsim();
	// 2、仿真运行线程管理
	clock_t m_LastStepClock;
	bool m_NeedStopJsbsim;
	void Init_RunJsbsim();
	void Check_RunJsbsim();
	// 3、迭代环境线程管理
	HANDLE m_hStepJsbsim;
	sJSBSIMSParams m_StepJsbsimTParams{};
	void Init_StepJsbsim();
	void Check_StepJsbsim();
	// 4、Tacview服务端
	bool m_NeedMeta;
	CIatTcpServer m_TacviewServer;
	double m_LastSendTime;
	std::queue<std::string> m_TacviewSendQueue;
	void Init_TacviewServer();
	void Send_TacviewServer();
	void Send_TacviewRow(char* row_buf, uint16_t row_len);
	// 5、TacView文件写
	CTacviewInt m_TacviewFile;
	void Write_TacviewFrame();

protected:
	afx_msg LRESULT OnUmSystemSyncDataPointer(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnUmSystemInit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSystemStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSystemPause(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSystemCont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSystemStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUmSystemReset(WPARAM wParam, LPARAM lParam);
};

