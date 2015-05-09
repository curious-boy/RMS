//FILESERVERDEF.CPP

#include "stdHeader.h"
#include "ReformMediaServerDef.h"
#include "ManagerConnect.h"
#include "SysInfo.h"
#include "ManagerRtsp.h"
#include "ManagerDVR.h"
#include "dvr.h"
#include "channel.h"
#include "dhdvrchannel.h"
#include "HikNetDVRChannel.h"
#include <tchar.h>
#include "Log.h"

#define MAX_STR 100

HWND				g_hWnd = NULL;
WNDPROC				g_lpOriginalProc = NULL;	// ��Ϣ��Ӧ����ԭʼ��ַ
HWND				g_hWnd2 = NULL;
WNDPROC				g_lpOriginalProc2 = NULL;   // ��Ϣ��Ӧ����ԭʼ��ַ

CManagerConnect		*g_pManagerConnect = NULL;
CManagerRtsp		*g_pManagerRtsp = NULL;
CLog				*g_pLog = NULL;

CSysInfo			*g_pSysInfo = NULL;
CManagerDVR			*g_pManagerDVR = NULL;

#ifdef _DEBUG
int scount=0;
int ocount=0;
int lcount=0;
#endif

void Ticks2Time(_int64 dwtime, CTime &time)
{
	_int64 dw = dwtime/10000000;

    int  days[13] = { 0, 31 , 29 , 031 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ;
    int  i ;
    int second,minute,hour,year,month,date;
    second = dw % 60 ;
    dw          = dw / 60 ;        /*dat is minute*/
    
    minute = dw % 60 ;
    dw          = dw / 60 ;        /*dat is hour  */
    
    hour   = dw % 24 ;
    dw          = dw / 24 ;        /*dat is day   */
        
    i            = 2001    ;

	dw = dw - (365*3+366)*500 + 15;

    while( dw >= 365 )
    {
        if( ( i % 4 ) == 0 )
            dw -= 366 ;
        else
            dw -= 365 ;
        i++ ;		
    }
    
    year  = i ;
    if( i % 4 ) 
        days[2] = 28 ;
    
    for( i = 1 ; i < 13 ; i++ )
    {
        if( dw < days[i] )
            break;
        dw -= days[i] ;    
    }
    month  =  i  ;
    date    = (int)dw + 1 ;
	time = CTime::CTime(year,month,date,hour,minute,second);
}

_int64 Time2Ticks(CTime time)
{
	_int64 dwtime;

    int  days[13] = { 0, 31 , 29 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ;
    int  day  , i ;
    _int64  seconds ;
    
    seconds =    0 ;
    day     =    0 ;
    
    if( ( time.GetYear()-2000 )%4 != 0 )
        days[2] = 28 ;
    
    for( i = 0 ; i < ( time.GetYear()-2000 ) ; i++ )    
    {
        if( i%4 == 0 )
            day += 366 ;
        else
            day += 365 ;
    }    
    for( i = 1 ; i < time.GetMonth() ; i++ ) 
    {
        day += days[i] ;
    }       
    day += ( time.GetDay() - 1 ); 
	seconds =(365*3+366)*499 - 15;
	seconds = seconds*24*3600;
	seconds =seconds + 365*3*24*3600;
	seconds =seconds +( day * 24 + time.GetHour() )*3600 + time.GetMinute() * 60 + time.GetSecond();
    
//    seconds = ( day * 24 + time.GetHour() )*3600 + time.GetMinute() * 60 + time.GetSecond() + ((365*3+366)*499)*24*3600 + 365*3*24*3600;
	dwtime = seconds*10000000;
//    return seconds ;  
	
	return dwtime;
}

LRESULT CALLBACK BuildWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

//����rtsp��Ϣ
LRESULT WINAPI WndSubClassProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //������Ϣ��Ӧ
{
	/* CWinThread *p = AfxGetThread();
	TRACE("2,Current Thread ID = 0x%X\n", p->m_nThreadID); */
	if(uMsg == WM_DESTROY)
	{
		return CallWindowProc(g_lpOriginalProc2,hWnd,uMsg,wParam,lParam);
	}
	if(uMsg == WM_LISTEN_COMMANDSOCKET)
	{
		if(WSAGETSELECTEVENT(lParam) == FD_ACCEPT)
		{
			g_pManagerRtsp->OnAccept();
		}
		else if(WSAGETSELECTEVENT(lParam) == FD_CLOSE)
		{
//			g_pLog->WriteLog("g_pManagerConnect�����رգ�CloseCli:\n");
//			g_pManagerConnect->OnAccept();
		}

		return CallWindowProc(g_lpOriginalProc2,hWnd,uMsg,wParam,lParam);
	}

//	int length = g_pManagerConnect->m_SocketArray.GetSize();
	if((uMsg > WM_LISTEN_COMMANDSOCKET)&&(uMsg <= WM_LISTEN_COMMANDSOCKET + MAX_CLIENT_SIZE * 4))
	{
		if(WSAGETSELECTEVENT(lParam) == FD_CLOSE)
		{
//			printdebug("�ͻ��˹رգ�CloseCli:FD_CLOSE\n");
			g_pLog->WriteLog("RTSP�ͻ��������ر�(%d)\n", (int)(uMsg - WM_LISTEN_COMMANDSOCKET - 1));
			g_pManagerRtsp->CloseCli(uMsg - WM_LISTEN_COMMANDSOCKET - 1);
		}

		if(WSAGETSELECTEVENT(lParam) == FD_READ)
		{
			g_pManagerRtsp->ReceiveMsg(uMsg - WM_LISTEN_COMMANDSOCKET - 1);
		}

		return CallWindowProc(g_lpOriginalProc2, hWnd, uMsg, wParam, lParam);
	}

	return CallWindowProc(g_lpOriginalProc2, hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI WndSubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //������Ϣ��Ӧ
{
	//CWinThread *p = AfxGetThread();
	/*ATL::AtlCreateThread();
	TRACE("2,Current Thread ID = 0x%X\n", p->m_nThreadID);*/
	if(uMsg == WM_DESTROY)
	{
		return CallWindowProc(g_lpOriginalProc,hWnd,uMsg,wParam,lParam);
	}
	// ��Ϣ,�첽SOCK������Ϣ,��
	if(uMsg == WM_LISTEN_SOCKET)
	{
		if(WSAGETSELECTEVENT(lParam) == FD_ACCEPT)
		{
			g_pManagerConnect->OnAccept();
		}
		else if(WSAGETSELECTEVENT(lParam) == FD_CLOSE)
		{
//			g_pLog->WriteLog("g_pManagerConnect�����رգ�CloseCli:\n");
//			g_pManagerConnect->OnAccept();
		}
		return CallWindowProc(g_lpOriginalProc,hWnd,uMsg,wParam,lParam);
	}

//	int length = g_pManagerConnect->m_SocketArray.GetSize();
	if((uMsg > WM_LISTEN_SOCKET)&&(uMsg <= WM_LISTEN_SOCKET + MAX_CLIENT_SIZE*4))
	{
		if(WSAGETSELECTEVENT(lParam) == FD_CLOSE)
		{
//			printdebug("�ͻ��˹رգ�CloseCli:FD_CLOSE\n");
			g_pLog->WriteLog("MC�ͻ��������ر�(%d)\n", (int)(uMsg - WM_LISTEN_SOCKET - 1));
			g_pManagerConnect->CloseCli(uMsg - WM_LISTEN_SOCKET - 1);
		}
		if(WSAGETSELECTEVENT(lParam) == FD_READ)
		{
			g_pManagerConnect->ReceiveMsg(uMsg - WM_LISTEN_SOCKET - 1);
		}
		return CallWindowProc(g_lpOriginalProc,hWnd,uMsg,wParam,lParam);
	}

	// ��Ϣ,DVR�쳣�ϴ���Ϣ
	if(uMsg == WM_DVRALARMCOMMAND)
	{
		char tempbuf[256];
		DWORD dwType = wParam; //�쳣����
		LONG lIndex = lParam; //�����쳣����Ӧ���͵ľ��
		ZeroMemory(tempbuf, 256);

		switch(dwType)
		{
		case 0x8001: //�����Խ�ʱ�����쳣
			sprintf(tempbuf,"�����Խ�ʱ�����쳣!!!");
			//TRACE("%s",tempbuf);
			//TODO: �ر������Խ�
			break;
		case 0x8002: //�����ϴ�ʱ�����쳣
			sprintf(tempbuf,"�����ϴ�ʱ�����쳣!!!");
			//TRACE("%s",tempbuf);
			//TODO: �رձ����ϴ�
			break;
		case 0x8003: //����Ԥ��ʱ�쳣
			sprintf(tempbuf,"����Ԥ��ʱ�����쳣!!!");
			//TRACE("%s",tempbuf);
			//TODO: �ر�����Ԥ��
			break;
		case 0x8004: //͸��ͨ������ʱ�쳣
			sprintf(tempbuf,"͸��ͨ������ʱ�����쳣!!!");
			//TRACE("%s",tempbuf);
			//TODO: �ر�͸��ͨ��
			break;
		case 0x8005: //Ԥ��ʱ����
			break;
		default:
			break;
		}
	}

	// ��Ϣ,ֹͣת��
	if(uMsg == WM_STOPTRANS_COMMANDSOCKET)
	{
		int dvrindex = wParam;
		int channelindex = lParam;

		//if(g_pManagerDVR->m_pDVRArray[dvrindex]!= NULL)
		//{
		//	if(g_pManagerDVR->m_pDVRArray[dvrindex]->m_pChannelArray[channelindex] != NULL)
		//	{
		//		if(g_pManagerDVR->m_pDVRArray[dvrindex]->m_type == HIK_DVRDH_CHANNEL/*��DVR*/ )
		//		{
		//			CDHDVRChannel *pChannel = (CDHDVRChannel *)g_pManagerDVR->m_pDVRArray[dvrindex]->m_pChannelArray[channelindex];
		//			pChannel->StopTrans();
		//		}
		//		else if(g_pManagerDVR->m_pDVRArray[dvrindex]->m_type == HIK_DVRHK_CHANNEL/*����DVR*/ )
		//		{
		//			CHikNetDVRChannel *pChannel = (CHikNetDVRChannel *)g_pManagerDVR->m_pDVRArray[dvrindex]->m_pChannelArray[channelindex];
		//			pChannel->StopTrans();
		//		}
		//	}
		//}
	}
	return CallWindowProc(g_lpOriginalProc, hWnd, uMsg, wParam, lParam);
}

//�����������ڣ����������첽��RTSP��SOCKET
BOOL BuildWindow()
{
	HINSTANCE hInstance = NULL;
    hInstance = GetModuleHandle(NULL);	
	char lpszClassName[]="ReformMediaServerWIN";//��������

	WNDCLASS wndClass;
	wndClass.style =0; // ��������Ϊȱʡ����
	wndClass.lpfnWndProc=BuildWndProc; //���崰�ڴ�������
	//wndClass.lpfnWndProc=WndSubClassProc; //���崰�ڴ�������
	wndClass.cbClsExtra=0; //����������չ
	wndClass.cbWndExtra=0; //����ʵ������չ
	wndClass.hInstance=hInstance; //��ǰʵ�����
	wndClass.hIcon=0;//���ڵ���С��ͼ��Ϊȱʡͼ��
	wndClass.hCursor=0; // ���ڲ��ü�ͷ���
	wndClass.hbrBackground=0; //���ڱ���Ϊ��ɫ
	wndClass.lpszMenuName=NULL; //�����޲˵�
	wndClass.lpszClassName=lpszClassName; //��������Ϊ�����ڡ�
         
    if(!RegisterClass( &wndClass ))
	{
		//DWORD re = GetLastError();
		g_pLog->WriteErr("�޷�ע�ᴰ���࣬�����ǲ���ϵͳ��֧�ָó���");
		return FALSE;
	}

	//LPCTSTR lpszClass = AfxRegisterWndClass(NULL);
    g_hWnd=::CreateWindow(lpszClassName,             // windows class name
					"ReformMediaServerWin1",        // window caption
                    WS_OVERLAPPEDWINDOW,            // window style
                    CW_USEDEFAULT, CW_USEDEFAULT,   // position and dimensions
                    CW_USEDEFAULT, CW_USEDEFAULT,
                    NULL,						    // owner window handle--NULL is Desktop
					NULL,					    	// for popup and overlapped windows: window menu handle
					hInstance,   	// handle to application instance
					NULL				            // pointer to window-creation data
					);
	if(g_hWnd == NULL)
	{
		//DWORD re = GetLastError();
		//g_pLog->WriteLog("�޷����ɴ���1���������:%d\n",re);
		g_pLog->WriteErr("�޷����ɴ���1!\n");
		return FALSE;
	}

	// change the window procdure address
	g_lpOriginalProc = (WNDPROC) SetWindowLong(g_hWnd, GWL_WNDPROC, (LONG)WndSubClassProc);
	//g_lpOriginalProc=(WNDPROC)WndSubClassProc;
	
    g_hWnd2 = ::CreateWindow(lpszClassName,         // windows class name
					"ReformMediaServerWin2",        // window caption
                    WS_OVERLAPPEDWINDOW,            // window style
                    CW_USEDEFAULT, CW_USEDEFAULT,   // position and dimensions
                    CW_USEDEFAULT, CW_USEDEFAULT,
                    NULL,						    // owner window handle--NULL is Desktop
					NULL,					    	// for popup and overlapped windows: window menu handle
					hInstance,	    				// handle to application instance
					NULL				            // pointer to window-creation data
					);
	if(g_hWnd2 == NULL)
	{
		//DWORD re = GetLastError();
		//g_pLog->WriteLog("�޷����ɴ���2.�������:%d\n",re);
		g_pLog->WriteErr("�޷����ɴ���2!\n");
		return FALSE;
	}

	// rtsp socket
	g_lpOriginalProc2 = (WNDPROC) SetWindowLong(g_hWnd2, GWL_WNDPROC, (LONG)WndSubClassProc2);
	return TRUE;
}

BOOL check_IP(const char* str)
{
	if(strlen(str) > 16)
		return FALSE;
	char tip[16]={0};
	memcpy(tip, str, 16);
	char* token=NULL;
	char* dots[4]={0};
	int ilen = 0;
	token = strtok(tip, ".");
	while(token !=NULL)
	{
		dots[ilen] = token;
		ilen++;
		token = strtok(NULL, ".");
	}
	if(ilen != 4)
		return FALSE;
	else
	{
		for(int i=0;i!=4;i++)
		{
			int e = atoi(dots[i]);
			if(e>255 || e <0 )
				return FALSE;
		}
	}
	return TRUE;
}

void terminate_thread(HANDLE& thread, HANDLE hevent)
{
	if(thread)
	{
		DWORD dwStatus;
		for(int i=0;;i++)
		{
			if(hevent != NULL)
				SetEvent(hevent);
			ResumeThread(thread);	
			Sleep(30);
			BOOL ret = ::GetExitCodeThread(thread, &dwStatus);
			if(ret== FALSE)
			{
				g_pLog->WriteLog("ERROR:VS�ر��߳�ʧ��(err:%d tid:%d)\n", GetLastError(), thread);
				break;
			}
			else
			{
				if( dwStatus==STILL_ACTIVE)
				{
					if(i==100)
						g_pLog->WriteLog("WARN:VSδ�ܹر��߳�(%d)\n", thread);
				}
				else
				{
					g_pLog->WriteLog("VS�ر��߳�%d\n", thread);
					break;
				}		
			}
		}
		CloseHandle(thread);
		thread=NULL;
	}
}

CString GetAppPath()
{
	//��ȡӦ�ó����Ŀ¼
    char modulePath[MAX_PATH];
    GetModuleFileName(NULL, modulePath, MAX_PATH);
    CString strModulePath(modulePath);
    strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
    return strModulePath;
}