// Channel.h: interface for the CChannel class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "ReformMediaServerDef.h"
#include <string>
#include "CLock.h"
#include "CBufferCmd.h"

using namespace std;

//#define RF_VIDEO_RECORD_FOLDER "reformvideorecord"

////class CVideoRecord;
//class CDVR  ;
//class CVideoTrans  ;
//class CMultiCastVideoTrans;

class CChannel  
{
//public:
//	CChannel();
//	virtual ~CChannel();
//
//	virtual BOOL	Init(int ChannelID, int index, int DVRChannelID, CDVR *pDVR);	// ��ʼ��
//	virtual BOOL	InitMultiCast();												// ��ʼ���ಥ
//	virtual BOOL	SetImageParam(IMAGEPARAM_INFO info);
//	BOOL	SendBuffer(char *pbuffer, DWORD length, BOOL bhead);			// ���ͻ��˷�������
//	virtual BOOL	GetPic(char* sFileName);
//	virtual BOOL	StartTrans()=0;
//	BOOL	AddTrans(SOCKET sock);											// ��������Դ
//	BOOL	CloseTrans(int index);											// �ر�����Դ
//	virtual BOOL	PostStopTrans();
//	virtual BOOL	StopTrans() = 0;
//
//	virtual BOOL	PTZControl(DWORD dwPTZCommand, DWORD param1, DWORD param2, DWORD param3, BOOL dwStop);		// ��̨����
//	//virtual BOOL	StartRecord(BOOL bRecord, char * pPath);						// ����¼��
//
//	CDVR			*m_pDVR;				// ��ͨ��������DVR�豸��ָ��
//	string			m_strDVRName;			// ����dvr����
//	int				m_DVRChannelID;			// ͨ����DVR�е�˳���
//
//	CHANNEL_INFO	m_ChannelInfo;			// ͨ����������Ϣ
//	IMAGEPARAM_INFO m_ImageInfo;			// ��Ƶͼ��Ĳ�����Ϣ
//
//	int				m_index;				// ��������
//	string			m_strCameraId;			// ����ͷ���
//	int				m_Frames;
//	int				m_subFrames;
//	int				m_Playhandle;
//
//	int				m_SocketNum;
//
//	BOOL			m_bRecord;				// �Ƿ�¼��
//	string			m_strRecordPath;		// ¼��·��
//	string			m_strRecordName;		// ¼�����ƣ�¼������ = dvr���� + dvrͨ������ + ¼��ʼʱ�䣩
//
//	time_t			m_tLastSaveTime;		// ¼�񱣴�ʱ��
//	string			m_strLastSaveTime;		// ¼�񱣴�ʱ��
//	//FILE			*fp;					// �ļ�ָ��
//	int				m_recordId;				// ¼���ļ������ݿ��еı��
//
//	int				m_FileLength;			// �ļ��ֽڳ���
//	BOOL			m_bTrans;
//	BOOL			m_bMotion;
//	BOOL			m_bCurMotiom;
//
//	CVideoTrans	*m_VideoTransArray[MAX_CLIENT_SIZE];			// �յ�������֮��ת����ʽ������(�鲥��������б��ڵ��û�����)
//	CMultiCastVideoTrans *m_MultiCastVedio;						// 
//
//	string m_type_ID;//�豸����Ӣ����д
//	string m_type;//�豸��������
//	FILE			*_fp;					// �ļ�ָ��
//	void save_record(char *pbuffer, DWORD length);
//	void add_record_to_db();
//	bool get_record_file_path(char* path, char* file_path, char* file_name);
//
//	long icon_port;
//	bool is_icon_init_;
//	bool is_ready_icon_;
//	char icon_path[MAX_PATH];
//	void init_icon(char* name);
//	virtual void save_icon(char *pbuffer, DWORD length) = 0;
//
//	int m_work_status;
//
//	time_t m_disconnect_time;
//
//	virtual BOOL stop_by_Socket(SOCKET s);
//
//	//�ͷ�Ӳ�̿ռ�
//	//�ɹ�ɾ��¼���ļ�������1
//	//ֻɾ�������ݿ��¼��δ��ɾ���ļ�������0������
//	//��ѯ��һ�����ݿ��¼ʧ�ܣ�����-1
//	int freeDiskspace();
//
//	int PTZControlIndex;
//	BOOL m_bControl;
//
//	int m_YTStatus;
//	map<int,int> m_mapYTCmd;
//	int m_lastCmd;
//
//	static volatile DWORD m_RecordStatus;
//
//	virtual BOOL Init(LONG lLoginID, int nChannelID){return FALSE;}

public:
	CChannel(const LONG lLoginID, const int nChannelID, const int tStreamType);
	virtual ~CChannel();

	const LONG _LoginID;
	const int _ChannelID;
	const int _StreamType;

	//enum{
	//	CH_Run=0,
	//	CH_Pend,
	//	CH_Stop
	//}_ConnectStatus;
	LONG _Playhandle;
	virtual BOOL connect()=0;//{return FALSE;};
	virtual BOOL disconnect() =0;//{return TRUE;};

	BOOL addCmd(CBufferCmd* cmd);
	BOOL delCmd(CBufferCmd* cmd);
	void VedioFunc(const BYTE* pBuffer, const DWORD dwBufSize);

	int m_ptzcount;
	static const int m_ptznum = 16;
	static const int m_ptzdiff = 30;
	time_t m_ptzbegintime;
	BOOL m_bControl;
	int m_lastCmd;
	BOOL	PTZControl(DWORD dwPTZCommand, DWORD param1, DWORD param2, DWORD param3, BOOL dwStop);		// ��̨����
	
	CTry _Atry;

	BOOL idle();
	
	/*
	*��idle()���ʹ�ã���idle()����TRUEʱ��
	*�Ƚ��ϴ�����ִ��ʱ���뵱ǰʱ�䣺
	*���ʱ���1Сʱ������FALSE;
	*���򣬷���TRUE
	*/
	BOOL cmdTimeOut();
private:
	CLock _lock;
	time_t m_lastcmdtime;

	map<DWORD, CBufferCmd*> _CmdList;
	virtual BOOL	_PTZControl(DWORD dwPTZCommand, DWORD param1, DWORD param2, DWORD param3, BOOL dwStop){return FALSE;}		// ��̨����
};
