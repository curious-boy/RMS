#pragma once
#include "StdHeader.h"

// ���ڻ�ȡ��д��FileServer.ini�����ļ�
class CSysInfo
{
public:
	CSysInfo(void);
public:
	~CSysInfo(void);

	int m_FilePort;
	int m_RtspPort;
	int m_ListenDvrPort;
	int m_bLog;
	int m_PushPort;

	char m_XMLFilePath[MAX_PATH];

	BOOL WriteLoacl();//��ϵͳ������Ϣд���ļ��б���
	BOOL ReadLoacl();//���ļ���ȡϵͳ������Ϣ

	BOOL GetDeviceObjects();
};
