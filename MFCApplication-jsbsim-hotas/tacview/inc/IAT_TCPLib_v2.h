#pragma once

#include <Windows.h>
#include <cstdint>

#ifdef IAT_TCPLIB_EXPORT
#define IAT_TCPLIB_API _declspec(dllexport)
#else
#define IAT_TCPLIB_API _declspec(dllimport)
#endif

// TCP Server 连接线程参数
typedef struct TCP_Remote_Parameters
{
	// Remote 激活标志
	bool m_IsRemote;
	// 接收缓冲区
	char s_RecvBuffer[1024];
	// 接收缓冲区长度
	int s_RecvBufferLen;
	// 接收缓冲区刷新
	bool m_NeedRecv;
	// 发送缓冲区
	char s_SendBuffer[1024];
	// 发送缓冲区长度
	int s_SendBufferLen;
	// 发送缓冲区刷新
	bool m_NeedSend;
	// 关闭Remote收发线程
	bool m_Close;
	// 连接套接字
	SOCKET s_RemoteSocket;
}TCP_RMT_PARAMS;

// 监听线程参数
typedef struct TCP_Server_Parameters
{
	// 服务端信息，需设置
	SOCKADDR_IN s_ServerInfo;
	// 客户端信息
	SOCKADDR_IN s_ClientInfo[8];
	// 远程连接Socket参数
	TCP_RMT_PARAMS m_RemoteRecvSend[8];
	// 关闭标志
	bool m_IsStop;
}TCP_SERVER_PARAMS;

typedef enum TCP_STATUS
{
    IDLE = 0,
    LSTN,
    SEND,
	RECV
}E_TCP_STATUS;

class IAT_TCPLIB_API CIatTcpServer
{
public:
	CIatTcpServer();
	~CIatTcpServer();
private:
	HANDLE m_hThread;
	E_TCP_STATUS m_CurrStatus;
	SOCKADDR_IN m_ServerInfo;

	TCP_SERVER_PARAMS m_TcpListenParams;

public:
	E_TCP_STATUS GetStatus() { return m_CurrStatus; }
	bool SetServerInfo(SOCKADDR_IN usr_ServerInfo);
	SOCKADDR_IN GetClientInfo(uint8_t client_num) { return m_TcpListenParams.s_ClientInfo[client_num]; }
	bool IsRemote(uint8_t client_num) { return m_TcpListenParams.m_RemoteRecvSend[client_num].m_IsRemote; }
	bool IsRecv(uint8_t client_num) { return m_TcpListenParams.m_RemoteRecvSend[client_num].m_NeedRecv; }
	uint16_t GetRecvCnt(uint8_t client_num) { return m_TcpListenParams.m_RemoteRecvSend[client_num].s_RecvBufferLen; }
	void SetSend(uint8_t client_num) { m_TcpListenParams.m_RemoteRecvSend[client_num].m_NeedSend = true; }
	bool GetSend(uint8_t client_num) { return m_TcpListenParams.m_RemoteRecvSend[client_num].m_NeedSend; }
	void GetRecvBuffer(uint8_t client_num, char* recv_buf) { 
		memcpy(recv_buf, m_TcpListenParams.m_RemoteRecvSend[client_num].s_RecvBuffer, 512); 
		m_TcpListenParams.m_RemoteRecvSend[client_num].m_NeedRecv = false;
	}
	void SetSendBuffer(uint8_t client_num, char* send_buf, uint16_t send_cnt) {
		memcpy(m_TcpListenParams.m_RemoteRecvSend[client_num].s_SendBuffer, send_buf, send_cnt);
		m_TcpListenParams.m_RemoteRecvSend[client_num].s_SendBufferLen = send_cnt;
	}

	bool StartListen();
	bool StopListen();
	bool CloseRemote();
private:
	bool CloseSocket();
};
