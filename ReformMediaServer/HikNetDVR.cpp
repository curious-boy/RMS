#include "HikNetDVR.h"
#include "StdHeader.h"
#include "HCNetSDK.h"
#include "Log.h"

CHikNetDVR::CHikNetDVR(const char* const sDVRIP, const WORD wDVRPort, const char* const sUserName, const char* const sPassword)
	:CDVR(sDVRIP, wDVRPort, sUserName, sPassword)
{
	m_InvalidID = -1;
	m_lLoginID = m_InvalidID;
}

CHikNetDVR::~CHikNetDVR(void)
{
	clearChannels();
	disconnect();
}

BOOL CHikNetDVR::connect()
{
	BOOL ret = TRUE;
	if(m_lLoginID < 0)
	{
		char ip[16];
		strcpy_s(ip,m_DVRIP.c_str());
		// ��¼��DVR�豸
		NET_DVR_DEVICEINFO_V30 struDeviceInfo;
		memset(&struDeviceInfo,0,sizeof(NET_DVR_DEVICEINFO_V30));
		m_lLoginID = NET_DVR_Login_V30( ip, m_DVRPort, m_UserName, m_Password, &struDeviceInfo );
		if (m_lLoginID < 0)
		{
			// ��¼ʧ��
			g_pLog->WriteLog("���ú���SDK����ʧ��,%s, %s:%d\n",NET_DVR_GetErrorMsg(),ip,m_DVRPort);
			ret = FALSE;
		}	
		else
		{
			// ����ͨ������
			m_CnlNum = struDeviceInfo.byChanNum;
			m_StartCnl=struDeviceInfo.byStartChan;
			g_pLog->WriteLog("DVR��������ɹ�,loginID:%ld,%s:%d\n", m_lLoginID, ip,m_DVRPort);
		}
	}
	return ret;
}

BOOL CHikNetDVR::disconnect()
{
	BOOL ret = TRUE;
	if(m_lLoginID >=0)
	{
		clearChannels();
		ret = NET_DVR_Logout(m_lLoginID);
		m_lLoginID = -1;
		g_pLog->WriteLog("���ô�SDK�ǳ�,%s:%d\n",m_DVRIP.c_str(), m_DVRPort);
		if(ret == FALSE)
		{
			g_pLog->WriteLog("���ú���SDK�ǳ�ʧ��,%s\n", NET_DVR_GetErrorMsg());
		}
	}
	return ret;
}

