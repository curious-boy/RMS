#pragma once

//#include "ReformMediaServerDef.h"
#include "ServerSocket.h"
#include "queuelist.h"
//#include "device.h"
#include <string>
#include <WinSock2.h>
#include <hash_map>
#include "async_socket_client.h"

using namespace std;

#define RF_STREAM_SERVER_PORT 9000
#define MAX_NUM_CLIENT_MS 100
//---------------------------------------
// ��ʱͨ�����ݶ���
typedef struct _QueueList
{
	int sock;			// Socket(�����ڷ����������)
	unsigned int nLen;	// ���ݰ�����
	char* buffer;	// ���ݰ�

	_QueueList()
	{
		sock	= 0;
		nLen	= 0;
		//memset(buffer, 0, sizeof(buffer));
		buffer = NULL;
	}
} QueueList, *LPQueueList;

////---------------------------------------
//// client��Ϣ
//typedef struct _RFMediaClient
//{
//	int clientId;		// ���ɵı��
//	int sock;			// Socket(�����ڷ����������)
//
//	_RFMediaClient()
//	{
//
//	}
//} RFMediaClient, *LPRFMediaClient;

// ��ʱ���÷�����
class MediaServer
{
public:
	MediaServer(void);
	~MediaServer(void);

	// ��ʼ���������
	int Start();

private:
	

	void RequestVRResource(char *recvBuff, int sock);
	void RequestVRResourceWithIcon(char *recvBuff, int sock);

	static SOCKET _bind_socket;
	static int threadWrite(LPVOID p_pJobParam);

	static hash_map<SOCKET, AsyncSocketClient> _client_map;
	static int threadNetWork(LPVOID p_pJobParam);

	//static int ThreadListe(LPVOID p_pJobParam);
	//static int ThreadRecv(LPVOID p_pJobParam);
	//static int ThreadSend(LPVOID p_pJobParam);
	static int ThreadProcessData(LPVOID p_pJobParam);

	int AnalyseMsg(char *buff, int len, int sock);

	BOOL OnRegist(char *recvBuff, int sock);
	void OnHandshake(char *recvBuff, int sock);
	void OnUpdateDevice(char *recvBuff, int sock);
	BOOL CreateUpdateDeviceSql(char* buff, list<string>& sqlList);
	void OnUpdateConfig(char *recvBuff, int sock);
	void OnRequestDevices(char *recvBuff, int sock);
	void OnRequestVideoRecords(char *recvBuff, int sock);
	int GetServerIPAddress();
private:

	BOOL	m_bExit;			// �˳���ʶ
	HANDLE	m_hExitEvent;		// �˳��ź�
	//HANDLE  m_hPrepareEvent;	// ���������߳��ź�
	
	string m_strServAddr;

	CServerSocket m_Socket;
	CRITICAL_SECTION m_CriticalSection;

	CQBuffList<QueueList> m_qSend;
	CQBuffList<QueueList> m_qRecv;


	int init_socket_server();
};

extern MediaServer g_servObj;
