#pragma once

#include "ReformMediaServerDef.h"
#include <map>

using namespace std;

#define MAX_CONNECTION_NUMBER	32		// ���������

typedef struct _SvrSocket
{
	SOCKET	sock;		// Socket
	IN_ADDR ip;			// IP��ַ
	int		port;		// Socket�˿ں�
	BOOL	bRegister;	// ע��

	_SvrSocket()
	{
		sock		= NULL;
		port		= 0;
		bRegister	= false;
		// memset(ip, 0, sizeof(ip));
	}
} SvrSocket, *LPSvrSocket;

typedef map<SOCKET, LPSvrSocket> MapSvrSocket,*LPMapSvrSocket;
typedef MapSvrSocket::value_type vtSvrSocket;
typedef MapSvrSocket::iterator itSvrSocket;

class CServerSocket
{
public:
	CServerSocket(void);
public:
	~CServerSocket(void);

public:
	SOCKET m_hSocket;					// ����socket ���
	sockaddr_in m_addr;					// ÿ�οͻ�������ʱ���ͻ��˵ĵ�ַ
	UINT m_uPort;						// �˿ں�
	BOOL m_bInit;						// ��ʼ��winsock���ź�
	MapSvrSocket m_mapSvrSocket;		// �ͻ��˵�winsock��map
	SOCKET m_hClientSocketLast;			// �ͻ������һ�����ӵ�socket ���

	CRITICAL_SECTION m_CriticalSection;

public:
	int InitAndListen(UINT nPort);
	virtual int CheckSocketReadable(SOCKET sock);
	virtual int CheckSocketWriteable(SOCKET sock);
	virtual int SendMsg(char *buff, USHORT len, SOCKET sock);
	virtual int ReceiveMsg(char* buff, SOCKET sock);
	virtual void CloseClientSocket(SOCKET sock);
	void GetSvrSockMap(MapSvrSocket & mapSevSocket);

private:
	int InitSocket();
	void GetError(DWORD error);
};