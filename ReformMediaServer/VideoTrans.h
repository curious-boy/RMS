#pragma once
//#include "ReformMediaServerDef.h"

/*
���ʹ�ipc���������������ȡ������
*/
class CVideoTrans
{
public:
	CVideoTrans(SOCKET sock);
public:
	~CVideoTrans(void);

	BOOL AddBuffer(char *pbuf, int length);
	BOOL OnTransVideo();

	static DWORD WINAPI TransThread( LPVOID lpParameter);

	SOCKET	m_Socket;
	BOOL	m_bTrans;				//��ǰ�����Ƿ���ţ�

	HANDLE 		m_hTransThread;		//
	HANDLE		m_hTransKill;		//
	HANDLE		m_DataEvent ;		//
	HANDLE		m_BufMutex;

	long m_bufsize;
	char*		m_Membuf;
	int			m_Buflength;

	char	ChannelName[32];

};
