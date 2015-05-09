// ManagerConnect.h: interface for the CManagerConnet class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

// �ͻ�����Ƶ�ļ����͹�����

#include "ReformMediaServerDef.h"

class CVideoSend;

// 
class CManagerConnect  
{
public:
	CManagerConnect();
	virtual ~CManagerConnect();

	BOOL OnAccept();
	BOOL InitListenSocket();

	void ReceiveMsg(int iPlayNum);
	void CloseCli(int index);
	//void stop_Client(int index);
//	void	StopFileDown();
	int FindClient(_int64 session);

	SOCKET m_Listen;
    sockaddr_in local;

	int		m_clientnum;

	CVideoSend *m_SocketArray[MAX_CLIENT_SIZE];        // socket�б�   

//	HANDLE				m_VideoSendMutex ;

////RTSP
 //  int m_RtspStatus;	 //���ڱ�ʾRTSP�Զ�����״̬

private:
	BOOL AddClient(CVideoSend *pCon, int &index);	

};
