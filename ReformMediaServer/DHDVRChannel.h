#pragma once
#include "channel.h"

#define MCASTADDR "233.0.0.1"		// ����ʹ�õĶಥ���ַ��
#define MCASTPORT 5150				// ���ض˿ںš�

// ��dvr�豸
class CDHDVRChannel : public CChannel
{
//public:
//	CDHDVRChannel();
//public:
//	~CDHDVRChannel(void);
//
//	// ��ʼ��ͨ������������Ƶ����
//	BOOL Init(LONG lLoginID, int nChannelID);
//	BOOL	InitMultiCast();
//
//	BOOL	StartTrans();
//	BOOL	PostStopTrans();
//	BOOL	StopTrans();
//	BOOL	SetImageParam(IMAGEPARAM_INFO info);
//	BOOL	GetPic(char* sFileName);
	static const map<int,int> m_mapYTCmd;
	BOOL	_PTZControl(DWORD dwPTZCommand, DWORD param1, DWORD param2, DWORD param3, BOOL dwStop);// ��̨����
//	//BOOL	StartRecord(BOOL bRecord, char * pPath);			// ����¼��
//
//	void save_icon(char *pbuffer, DWORD length);
//
//private:
	//void	SaveRecord(char *pbuffer, DWORD length);
public:
	CDHDVRChannel(LONG lLoginID, const int nChannelID, const int tStreamType);
	~CDHDVRChannel();
	BOOL connect();
	BOOL disconnect();
};
