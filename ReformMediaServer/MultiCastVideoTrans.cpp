#include "StdHeader.h"
#include "MultiCastVideoTrans.h"
#include "log.h"
#include "ReformMediaServerDef.h"

DWORD WINAPI CMultiCastVideoTrans::TransThread( LPVOID lpParameter)
{
	CMultiCastVideoTrans* pOwner = reinterpret_cast<CMultiCastVideoTrans*>(lpParameter);
	if(pOwner == NULL)
	{
		//g_pLog->WriteLog("ͨ��Ϊ��\n");
		return 1;
	}

	HANDLE hMulEvents[2];
	hMulEvents[0] = pOwner->m_hTransKill;
	hMulEvents[1] = pOwner->m_DataEvent;

	//g_pLog->WriteLog("chu shi chang du %d\n",pOwner->m_Buflength);

	try
	{
		while(1)
		{
			switch(WaitForMultipleObjects(2, hMulEvents, FALSE, INFINITE))
			{
			case WAIT_OBJECT_0:		// �˴��˳���д�˳�����
				{
					return 1;
				}
				break;
			case WAIT_OBJECT_0 + 1:
				{
					if(pOwner!= NULL)
					{
						if(!pOwner->OnTransVideo())
						{
							if(pOwner!= NULL)
							{
								pOwner->End();
							}
							////g_pLog->WriteLog("����ʧ��\n");
							//g_pLog->WriteLog("����ʧ��,����ϣ�\n");
							return 1;
						}
					}else{
						return 1;
					}
					break;
				}
			default:// �쳣��д�˳�����
				{
					// g_pLog->WriteLog("NetPlayThread�����쳣�¼�");
					Sleep(50);
					break;
				}
			}
//			OnPlay();
		}
	}
	/*catch(CException *pEx)
	{
		pEx->Delete();
		pEx = NULL;
		if(pOwner != NULL)
		{
			pOwner->End();
		}
	}*/
	catch(...)
	{
		if(pOwner != NULL)
		{
			pOwner->End();
		}
	}
	return 1;
}

CMultiCastVideoTrans::CMultiCastVideoTrans(void)
{
	m_Membuf = new char[MEMBUF_LENGTH];
	m_Socket = INVALID_SOCKET;

	m_Buflength = 0;
	m_BufMutex = CreateMutex( NULL , FALSE , NULL ) ;
	m_bTrans = FALSE;
}

CMultiCastVideoTrans::~CMultiCastVideoTrans(void)
{
	g_pLog->WriteLog("ע��CMultiCastVideoTrans\n");

	End();

	if(m_hTransThread)
	{
		if(m_hTransKill != NULL)
		{
			SetEvent(m_hTransKill);
			DWORD dwStatus;
			for(int i=0;i<5;i++)
			{
				if(!::GetExitCodeThread(m_hTransThread, &dwStatus)||i==4)
				{
					TerminateThread(m_hTransThread,0);
					break;
				}
				else
				{
					if(dwStatus==STILL_ACTIVE)
					{
						SetEvent(m_hTransKill);
						Sleep(30);
					}
					else
						break;
				}
			}
		}
		CloseHandle(m_hTransThread);
		m_hTransThread = NULL;
	}

	if(m_hTransKill != NULL)
	{
		CloseHandle(m_hTransKill);
		m_hTransKill=NULL;
	}
	delete [] m_Membuf;
}

BOOL CMultiCastVideoTrans::End()
{
	g_pLog->WriteLog("ֹͣ����\n");
	m_bTrans = FALSE;

	if(m_hTransKill != NULL)
	{
		SetEvent(m_hTransKill);
	}

	Sleep(100);

	if (m_Socket != INVALID_SOCKET)
	{
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}

	return TRUE;
}

BOOL CMultiCastVideoTrans::Init(SOCKET sock,sockaddr_in remote)
{
	m_Socket = sock;
	m_Remote = remote;
	m_Buflength = 0;

	DWORD dw;
    m_DataEvent		= CreateEvent( NULL , TRUE , FALSE , NULL ) ;
	m_hTransKill	= CreateEvent( NULL, FALSE, FALSE, NULL );
	m_hTransThread	= CreateThread( NULL, 0, LPTHREAD_START_ROUTINE(TransThread), this, 0, &dw);

	if(!(m_DataEvent && m_hTransKill && m_hTransThread))
	{
		g_pLog->WriteLog("�����߳�ʧ��\n");

		return FALSE;
	}
	m_bTrans = TRUE;
	return TRUE;
}

BOOL CMultiCastVideoTrans::AddBuffer(char *pbuf, int length)
{
	if(m_bTrans == FALSE)
		return FALSE;

    WaitForSingleObject( m_BufMutex ,  INFINITE );
	if((length + m_Buflength) > MEMBUF_LENGTH)
	{
		//g_pLog->WriteLog("���ݴ��ڻ�����, %d,%d\n",length ,m_Buflength);
		m_Buflength = 0;
	}
	
	memcpy( m_Membuf + m_Buflength, pbuf , length ) ;
	m_Buflength = m_Buflength + length;

	ReleaseMutex( m_BufMutex ) ;
	SetEvent(m_DataEvent);

	return TRUE;
}

BOOL CMultiCastVideoTrans::OnTransVideo()
{
	WaitForSingleObject( m_BufMutex ,  INFINITE ) ;
	// ���͵ĳ��ȣ�ÿ����෢��15 * 1024
	int sendlength = BLOCKSIZE_UDP;
	if(sendlength > m_Buflength)
		sendlength = m_Buflength;

	// ����
	// int re = send(m_Socket, m_Membuf, sendlength, 0);
	int re = sendto( m_Socket, m_Membuf, sendlength, 0, (struct sockaddr*)&m_Remote, sizeof(m_Remote) );
	if( re <= 0 )
	{
		int hr;
		hr = WSAGetLastError();
		switch(hr)
		{
////////chenq,�˴���BUG���ͻ��˷������ݳɹ���������������������������Ҳ�Ӳ���������
		  case WSAEWOULDBLOCK:
			{
//				  g_pLog->WriteLog("ReceiveMsg��������WSAEWOULDBLOCK��\n");
				ReleaseMutex( m_BufMutex ) ;
				Sleep(30);
				return TRUE;
			}
		  break;
		  case WSAENOTSOCK :
			{
				g_pLog->WriteLog("WSAENOTSOCK��\n");
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
			  g_pLog->WriteLog("dont know\n");
		  break;
		}

		ReleaseMutex( m_BufMutex ) ;
		g_pLog->WriteLog("�쳣�ر�socket:%d, error:%d\n", m_Socket, hr );
		return FALSE;
	}

	m_Buflength = m_Buflength - re;
//	g_pLog->WriteLog("�������ݣ�%d,ʵ�ʷ���%d\n",sendlength, re);

	// ���Ѿ����͵���Ϣ����
	if(m_Buflength > 0)
		memmove( m_Membuf, m_Membuf + re , m_Buflength ) ;

	ReleaseMutex( m_BufMutex ) ;

	if(m_Buflength == 0)
	{
        ResetEvent( m_DataEvent ) ;   // ���������ź�
	} 
	Sleep(30);

	return TRUE;
}
