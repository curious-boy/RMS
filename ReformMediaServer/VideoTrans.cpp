#include "StdHeader.h"
#include "VideoTrans.h"
#include "log.h"
#include "ReformMediaServerDef.h"
#include <iostream>

DWORD WINAPI CVideoTrans::TransThread( LPVOID lpParameter)
{
	CVideoTrans* pOwner = reinterpret_cast<CVideoTrans*>(lpParameter);
	if(pOwner == NULL)
	{
		//g_pLog->WriteLog("ͨ��Ϊ��\n");
		return 1;
	}

	HANDLE hMulEvents[2];
	hMulEvents[0]=pOwner->m_hTransKill;
	hMulEvents[1]=pOwner->m_DataEvent;

	//g_pLog->WriteLog("chu shi chang du %d\n",pOwner->m_Buflength);

	try
	{
		DWORD dwEvent;
		while(1)
		{
			dwEvent = WaitForMultipleObjects(2,hMulEvents,FALSE, VT_SEND_TIMEOUT);
			switch(dwEvent)
			{
			case WAIT_OBJECT_0://�˴��˳���д�˳�����
				{
					g_pLog->WriteLog("VT�����˳��߳�\n");
					return 1;
				}
				break;
			case WAIT_OBJECT_0 + 1:
				{
					if(!pOwner->OnTransVideo())
					{
						g_pLog->WriteLog("WARN:VT����ʧ�ܣ��˳��̣߳�socket:%d\n", pOwner->m_Socket);
						pOwner->m_bTrans = FALSE;
						return 1;
					}
					break;
				}
			case WAIT_TIMEOUT:
				{
					g_pLog->WriteLog("WARN:VT�������ݳ�ʱ, ChannelName:%s, SOCKET:%d\n", pOwner->ChannelName, pOwner->m_Socket);
				}
				break;
			default://�쳣��д�˳�����
				{
					g_pLog->WriteLog("ERROR:VT�����쳣�¼�(event:%u error:%u)\n", dwEvent, GetLastError());
					return 1;
				}
			}
		}
	}
	catch(...)
	{
		g_pLog->WriteLog("ERROR:VT�����쳣�¼���exit");
	}
	return 1;
}

CVideoTrans::CVideoTrans(SOCKET sock)
{
	m_bufsize = MEMBUF_LENGTH;
	m_Membuf = new char[m_bufsize];
	m_Socket	= INVALID_SOCKET;
	m_Buflength = 0;
	m_BufMutex	= CreateMutex( NULL , FALSE , NULL ) ;
	m_bTrans	= FALSE;
	m_hTransThread = NULL;

	m_Socket = sock;
	m_Buflength = 0;

	DWORD dw;
	m_DataEvent		 =  CreateEvent( NULL , TRUE , FALSE , NULL ) ;
	m_hTransKill	 =	CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hTransThread =	CreateThread(NULL,0, LPTHREAD_START_ROUTINE(TransThread),this,0,&dw);

	if(!(m_DataEvent&&m_hTransKill&&m_hTransThread))
	{
		g_pLog->WriteLog("ERROR:VT�����߳�ʧ��\n");
	}
	else
		m_bTrans = TRUE;
}

CVideoTrans::~CVideoTrans(void)
{
	m_bTrans = FALSE;

	terminate_thread(m_hTransThread, m_hTransKill);
	//if(m_hTransThread)
	//{
	//	if(m_hTransKill != NULL)
	//	{
	//		SetEvent(m_hTransKill);
	//		DWORD dwStatus;
	//		for(int i=0;i<5;i++)
	//		{
	//			dwStatus = ::GetExitCodeThread(m_hTransThread, &dwStatus);
	//			if( dwStatus==STILL_ACTIVE && i==4)
	//			{
	//				//TerminateThread(m_hTransThread,0);//���ô˺����ᵼ�³���
	//				g_pLog->WriteLog("ERROR:VT�ر��߳�ʧ��\n");
	//				break;
	//			}
	//			else
	//			{
	//				if(dwStatus==STILL_ACTIVE)
	//				{
	//					SetEvent(m_hTransKill);
	//					ResumeThread(m_hTransThread);
	//					Sleep(30);
	//				}
	//				else
	//					break;
	//			}
	//		}
	//	}

	//	CloseHandle(m_hTransThread);
	//	m_hTransThread=NULL;
	//}

	if(m_hTransKill != NULL)
	{
		CloseHandle(m_hTransKill);
		m_hTransKill=NULL;
	}
		
	delete [] m_Membuf;
	//if (m_Socket != INVALID_SOCKET)
	//{
	//	g_pLog->WriteLog("�ر�SOCKET(%d)\n", m_Socket);
	//	closesocket(m_Socket);
	//	m_Socket = INVALID_SOCKET;
	//}
}

BOOL CVideoTrans::AddBuffer(char *pbuf, int length)
{
	if(m_bTrans == FALSE)
		return FALSE;

	WaitForSingleObject( m_BufMutex ,  INFINITE ) ;
	//if(length > MEMBUF_LENGTH)
	//{
	//	g_pLog->WriteLog("WARN:VT������̫С, %s, recv-len:%d, buf-len:%d\n", ChannelName, length ,MEMBUF_LENGTH);
	//	
	//	char* temp = NULL;
	//	if(m_Buflength >0 )
	//	{
	//		temp = new char[m_Buflength];
	//		memcpy(temp, m_Membuf, m_Buflength);
	//	}
	//	delete [] m_Membuf;
	//	m_bufsize = 2*length + m_Buflength;
	//	m_Membuf = new char[m_bufsize];
	//	if(m_Buflength >0)
	//		memcpy(m_Membuf, temp, m_Buflength);
	//}
	if((length + m_Buflength) > MEMBUF_LENGTH)
	{
		/*		ReleaseMutex( m_BufMutex ) ;
		End();
		return FALSE;*/
		g_pLog->WriteLog("VT���ݴ��ڻ�����, socket:%d, recv-len:%d, buf-len:%d\n", m_Socket, length ,m_Buflength);
		std::cout<<"VT���ݴ��ڻ�����, socket"<<m_Socket<<"len:"<<length<<"buf"<<m_Buflength<<std::endl;
		m_Buflength = 0;
	}

	memcpy( m_Membuf + m_Buflength, pbuf , length ) ;
	m_Buflength = m_Buflength + length;

	ReleaseMutex( m_BufMutex ) ;
	SetEvent(m_DataEvent);
	return TRUE;
}

BOOL CVideoTrans::OnTransVideo()
{
	if(m_Buflength < 512)
	{
		ResetEvent( m_DataEvent ) ;   //���������ź�
		return TRUE;
	} 	

	//����Ƿ���Է�������
	fd_set fdset;
	struct timeval tv;
	tv.tv_sec=0;
	tv.tv_usec=5;
	FD_ZERO(&fdset);
	FD_SET(m_Socket,&fdset);
	int iRet = select(m_Socket+1, NULL, &fdset, NULL, &tv);
	if (iRet == SOCKET_ERROR)
	{
		//socket����
		int hr;
		hr = WSAGetLastError();
		g_pLog->WriteLog("VT������Ƶ����ʧ��%d��δ֪����(%d)\n", m_Socket, hr);
		return FALSE;
	}
	else if(iRet == 0)
	{
		//���ɷ�������
		Sleep(TIME_LOOP);
		return TRUE;
	}
	else
	{
		//���Է�������
		WaitForSingleObject( m_BufMutex ,  INFINITE ) ;
		//���͵ĳ��ȣ�ÿ����෢��15*1024
		int sendlength = BLOCKSIZE_LIVE;
		if(sendlength > m_Buflength)
			sendlength = m_Buflength;

		// ����
		int re = send(m_Socket, m_Membuf, sendlength, 0);
		if(re == SOCKET_ERROR )
		{
			int hr;
			hr = WSAGetLastError();

			switch(hr)
			{
				////////chenq,�˴���BUG���ͻ��˷������ݳɹ���������������������������Ҳ�Ӳ���������
			case WSAEWOULDBLOCK:
				{
					g_pLog->WriteLog("VT������Ƶ��������(%d)��\n", m_Socket);
					ReleaseMutex( m_BufMutex ) ;
					Sleep(300);
					return TRUE;
				}
				break;
			case WSAENOTSOCK :
				{
					g_pLog->WriteLog("VT������Ƶ����ʧ�ܣ�WSAENOTSOCK(%d)��\n", m_Socket);
				}
				break;
			case 0:
				{
					ReleaseMutex( m_BufMutex ) ;
					Sleep(30);
					return TRUE;
				}
				break;
			default:
				g_pLog->WriteLog("VT������Ƶ����ʧ��%d��δ֪����(%d)\n", m_Socket, hr);
				break;
			}

			ReleaseMutex( m_BufMutex ) ;
			return FALSE;
		}
		else if(re == 0)
		{
			g_pLog->WriteLog("VT��������ret = 0\n");
			Sleep(TIME_LOOP);
			return TRUE;
		}

		m_Buflength = m_Buflength - re;
		//	g_pLog->WriteLog("�������ݣ�%d,ʵ�ʷ���%d\n",sendlength, re);

		//���Ѿ����͵���Ϣ����
		if(m_Buflength > 0)
		{

			memmove( m_Membuf, m_Membuf + re , m_Buflength ) ;//waiting for optimize .azl.
		}

		ReleaseMutex( m_BufMutex ) ;
		if(m_Buflength == 0)
		{
			ResetEvent( m_DataEvent ) ;   //���������ź�
		} 

		Sleep(TIME_LOOP);
	}
	return TRUE;
}
