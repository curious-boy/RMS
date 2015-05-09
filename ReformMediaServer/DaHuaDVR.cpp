#include "DaHuaDVR.h"
#include "StdHeader.h"
#include "dhnetsdk.h"
#include "Log.h"
#include "Config.h"

const char* const CDaHuaDVR::m_loginerr[10]={"���벻��ȷ","�û���������","��¼��ʱ","�ʺ��ѵ�¼","�ʺ��ѱ����� ","�ʺű���Ϊ������","��Դ���㣬ϵͳæ","������ʧ��","������ʧ��","��������û�������"};

CDaHuaDVR::CDaHuaDVR(const char * sDVRIP,const WORD wDVRPort, const char * sUserName, const char* sPassword)
	:CDVR(sDVRIP, wDVRPort, sUserName, sPassword)
{
	m_InvalidID = 0;
	m_lLoginID = m_InvalidID;
}

CDaHuaDVR::~CDaHuaDVR(void)
{
	clearChannels();
	disconnect();
}

BOOL CDaHuaDVR::connect()
{
	BOOL ret = TRUE;
	if(m_lLoginID==0)
	{
		char ip[16];
		strcpy_s(ip,m_DVRIP.c_str());
		int err = 0;
		NET_DEVICEINFO info;// ���ڷ����豸��Ϣ
		memset(&info, 0 ,sizeof(NET_DEVICEINFO));
		m_lLoginID = CLIENT_Login(ip, m_DVRPort, m_UserName, m_Password, &info, &err);	// ��¼�������豸��Ϣ
		if(m_lLoginID == 0)
		{
			ret = FALSE;
			//log error
			if(err<=10 && err>=1)
				g_pLog->WriteLog("���ô�SDK����ʧ��:%s, %s:%d\n",m_loginerr[err-1],ip,m_DVRPort);
			else
				g_pLog->WriteLog("���ô�SDK����ʧ��:�������Ͳ�ʶ��code=%d, %s:%d\n",err,ip,m_DVRPort);
		}
		else
		{
			//CLIENT_GetDevConfig
			m_CnlNum = info.byChanNum;
			m_StartCnl = 0;	

			g_pLog->WriteLog("DVR�󻪵���ɹ�,loginID:%ld,%s:%d\n", m_lLoginID, ip,m_DVRPort);
		}
	}
	return ret;
}

BOOL CDaHuaDVR::disconnect()
{
	BOOL ret = TRUE;
	if(m_lLoginID > 0)
	{
		clearChannels();
		ret = CLIENT_Logout(m_lLoginID);
		m_lLoginID=0;
		g_pLog->WriteLog("���ô�SDK�ǳ�,%s:%d\n",m_DVRIP.c_str(), m_DVRPort);
		if(ret == FALSE)
		{
			g_pLog->WriteLog("���ô�SDK�ǳ�ʧ��,code=%d\n",CLIENT_GetLastError());
		}
	}
	return ret;
}
