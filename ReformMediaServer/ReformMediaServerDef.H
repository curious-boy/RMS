//FILESERVERDEF.H

#pragma once

#include "StdHeader.h"

#include <map>
#include <string>
#include <vector>
#include <list>
#include <algorithm> 
//#include <functional>
using namespace std;

class CLog;
class CManagerConnect;
class CSysInfo;
class CManagerRtsp;
class CManagerDVR;

extern HWND			g_hWnd;
extern WNDPROC		g_lpOriginalProc;           // ��Ϣ��Ӧ����ԭʼ��ַ
extern HWND			g_hWnd2;
extern WNDPROC		g_lpOriginalProc2;           // ��Ϣ��Ӧ����ԭʼ��ַ

extern CLog				*g_pLog;
extern CManagerConnect	*g_pManagerConnect;
extern CManagerRtsp		*g_pManagerRtsp;
extern CSysInfo			*g_pSysInfo;
extern CManagerDVR		*g_pManagerDVR;

#define WM_DVRALARMCOMMAND			2410	//��Ϣ,DVR�쳣�ϴ���Ϣ
#define WM_LISTEN_SOCKET			4000	//��Ϣ,�첽SOCK������Ϣ,��
#define WM_LISTEN_COMMANDSOCKET		3000	//��Ϣ,�첽SOCK������Ϣ,RTSP
#define WM_STOPTRANS_COMMANDSOCKET	3500	//��Ϣ,ֹͣת��

#define   MAX_CLIENT_SIZE			512

#define   MAX_SERVER_SIZE			64

#define   MAX_CHANNEL_SIZE			64

#define   HEAD_SIZE					40


#define   MEMBUF_LENGTH					16*1024*1024
///ͨ������
#define  HIK_DVRHK_CHANNEL			1//����DVR
#define  HIK_DVRDH_CHANNEL			2//��DVR
#define  HIK_JPG_CHANNEL			3//��DVR
#define  HIK_MJPG_CHANNEL			4//��DVR


///��Ƶ��ʽ
#define   VIDEO_QCIF					1
#define   VIDEO_CIF						2
#define   VIDEO_4CIF					4

/************************************************************************/
/*��������ͻ��˼�����ӿڣ�                              */
/************************************************************************/
#define FM_COMMAND_MESSAGE 0X5000
//Զ���ļ�����
#define FM_FILEPLAY_REG		FM_COMMAND_MESSAGE + 0
#define FM_FILEPLAY_RE		FM_COMMAND_MESSAGE + 1
#define FM_FILEDOWN_REG		FM_COMMAND_MESSAGE + 2

#define FM_NETPLAY_PAUSE		FM_COMMAND_MESSAGE + 3
#define FM_NETPLAY_CONTINUE		FM_COMMAND_MESSAGE + 4

#define FM_CHANNELPLAY_REG		FM_COMMAND_MESSAGE + 5
#define FM_CHANNELPLAY_RE		FM_COMMAND_MESSAGE + 6

// ��̨����
#define FM_CHANNEL_PTZCONTROL_REG		FM_COMMAND_MESSAGE + 7
#define FM_CHANNEL_PTZCONTROL_RE		FM_COMMAND_MESSAGE + 8

//�������Ϳͻ���֮����Ƶ�ļ���������
#define FILE_PLAY			1
#define FILE_DOWN			2
#define LIVE_PLAY			3

#define BUF_SIZE			1024

#define 	OPTIONS_CMD	1
#define		PLAY_CMD 	2
#define		SETUP_CMD	3		
#define		STOP_CMD	4
#define		PAUSE_CMD	5


typedef struct 
{
	char FileName[MAX_PATH];
	int startpos;
	int endpos;//-1��ʾ���ļ���β��
}FILE_INFO;//��ѯ������¼���ļ���Ϣ

//typedef struct
//{
//	int channelType;
//	bool multicast;
//	char multicastAddress[20];
//	char multicastPort[10];
//}

typedef struct 
{
	int startblock;//������ʼ�ļ����
	int endblock;//���������ļ����
	int BlockFileNum;//����¼�����

}IDXFILEHEAD_INFO;//.IDX�ļ���ͷ��Ϣ

typedef struct 
{
	_int64 starttime;//������ʼshijian
	_int64 endtime;//��������shijian 
	int startblock;//������ʼ�ļ����
	int endblock;//���������ļ����
	int stampnum;//����ʱ�������
}IDXFILEBLOCK_INFO;//.IDX�ļ�����Ϣ

typedef struct 
{
	int blockindex;//���������ļ����
	int pos;//������ʱ������ļ����λ��
	_int64 time;//����ʱ���ʱ�����ʱ���
}IDXSTAMP_INFO;//.IDX�ļ���ʱ�����Ϣ

typedef struct 
{
	char filename[150];//���������ļ����
	int pos;//������ʱ������ļ����λ��
	_int64 time;//����ʱ���ʱ�����ʱ���
}FILESTAMP_INFO;//.IDX�ļ���ʱ�����Ϣ

typedef struct 
{
	char channelid[MAX_PATH];
	_int64 starttime;//������ʼshijian
	_int64 endtime;//��������shijian 
	_int64 session;
	int		type;	//����
	int		encodetype;//�����ʽ;
}FIND_INFO;//.IDX�ļ���ʱ�����Ϣ

typedef struct 
{
	FIND_INFO findinfo;
//	_int64 time;//������Ϣ�����ʱ�䣬SETUP
	SOCKET sock;
//	int index;//��������λ��

}RTSPCLIENT_INFO;//.IDX�ļ���ʱ�����Ϣ


typedef struct 
{
	char	UserName[32];
	char	Password[32];
}USER_NAME_INFO;                                                                                                                                                                                                                                                                                                              
typedef struct 
{
	char	ServerIP[16];
	char	DVRName[20];		// DVR����
	int		port;
	int		ChannelID;			// ͨ��ID
	char	ChannelName[32];	// ͨ������
	int		ChannelType;		// ͨ������mgqja625450
	int		Format;
	int		Quality;			//��1-10)
	char	username[32];
	char	psw[32];
	bool	isMulticast;
	char	MultiCastIP[16];
	int		MultiPort;
}CHANNEL_INFO;

typedef struct 
{
	string	strCameraID;	
	string	eCameraType;
	string	strName;	
	string	strModel;
	string	strProvider;
	string	strCodec;
	string	strLogin;
	string	strPassword;
	string	strIP;
	string	strPort;
	string	strCnlID;
	string	boolMultiCast;
	string	strMultiCastAddress;
	string	strMultiCastPort;
	string	eRecordType;
}CAMERA_INFO;

typedef struct 
{
	int Brightness;//����ֵ��0-255,DVR��1-10��
	int Contrast; //�Աȶȣ�0-127��DVR��1-10��
	int Saturation;//���Ͷ�(0-127��DVR��1-10��
	int Hue;//ɫ��0-255��DVR��1-10��
}IMAGEPARAM_INFO;

// ʱ���
typedef struct _TimePack
{
	unsigned short    nYear;
	unsigned char     nMonth;
	unsigned char     nDay;
	unsigned char     nSer;
	unsigned char     nHour;
	unsigned char     nMinute;
	unsigned char     nSecond;
} TimePack, * LPTimePack;


// ------------------------------
// IPC�豸
typedef struct _RFCamera
{
	int			nId;					// ���ݿ��е��Զ����
	string		strCameraId;			// ϵͳ���ɵ�����ͷ�豸���
	string		strCameraType;			// ����ͷ����
	string		strCameraName;			// ����ͷ����
	string		strMode;				// ����ͷ����(DVR��NVR��IPC������)
	string		strProvider;			// �豸����(�󻪡���������˼������)
	string		strCodec;				// �����ʽ
	string		strLogin;				// ���ӵ��豸ʱ��¼���û���
	string		strPassword;			// ��¼����
	string		strIp;					// �豸��IP��ַ
	string		strPort;				// �豸���Ӷ˿ں�
	string		strCnlId;				// �豸����ͨ����

	string		strMultiCast;			// �Ƿ�㲥����
	string		strMultiCastAddress;	// �㲥��ַ
	string		strMultiCastPort;		// �㲥�˿�
	string		strRecordType;			// 
	
	_RFCamera()
	{
		nId						= -1;
		strCameraId				= "";
		strCameraType			= "";
		strCameraName			= "";
		strMode					= "DVR";
		strProvider				= "��";
		strCodec				= "H264";
		strLogin				= "admin";
		strPassword				= "admin";
		strIp					= "";
		strPort					= "37776";
		strCnlId				= "1";

		strMultiCast			= "true";
		strMultiCastAddress		= "";
		strMultiCastPort		= "5152";
		strRecordType			= "";
	};
} RFCamera, *LPRFCamera;

// IPC�豸�б�
typedef map<int, LPRFCamera> MapRFCamera,*LPMapRFCamera; 
typedef MapRFCamera::value_type ValuetRFCamera;
typedef MapRFCamera::iterator IterRFCamera;

// ¼��洢����
enum RF_RECORD_TYPE
{
	RECORD_FORBIT = 0,				// ���洢
	RECORD_BYMONTH,					// ���µ�ָ�����ڴ洢
	RECORD_BYWEEK,					// ָ��һ�ܵ�ĳ��洢
	RECORD_EVERYDAY,				// ���趨�ľ�������ʱ��洢
	RECORD_AWAYS					// ���Ǵ洢
};

// ʱ������
typedef struct _RFTime
{
	int			Hour;
	int			Minute;
	int			Second;
} RFTime;

// ��ʼʱ�䵽����ʱ��
typedef struct _RFRecordTime
{
	RFTime		tStart;
	RFTime		tStop;
}RFRecordTime;

// ------------------------------
// ¼��洢�Ĳ���
typedef struct _RFRecordSetting
{
	int				nRecordId;			// ��Ӧ�����ļ���¼����
	string			strCameraId;		// �����·IPC���д洢
	string			strExName;			// ��¼�����ӵ����Ʊ�ʶ
	string			strRecordPath;		// �洢��Ŀ��·������ʱ�����ڵ�ǰ·����,������������չ��
	RF_RECORD_TYPE	nRecordType;		// ¼������
	int				nRecordMask;		// ��ʶ�洢������( 01111111111111111111111111111111 )
	bool			bRecordAnyTime;		// ¼���ʱ���
	list<RFRecordTime>	listRecordTime;	// ¼���¼��ʱ��
	string			strChangeFileReasion;// ¼��ʱ���趨ʱ��¼�񡢻��߰���С¼��
	int				nChangeFileValue;	// ��������߶����ֽ�
	string			strDiskFullDo;		// ���̿ռ䲻��ʱ��ֹͣ¼��ɾ������ǰ�ļ�¼���л���ϵͳ�д��̿ռ�������

	_RFRecordSetting()
	{
		nRecordId				= -1;
		strCameraId				= "";
		strExName				= "";
		strRecordPath			= "";
		nRecordType				= RECORD_FORBIT;
		nRecordMask				= 0;
		bRecordAnyTime			= false;
		strChangeFileReasion	= "bytime";// bysize
		nChangeFileValue		= 500;
		strDiskFullDo			= "";

	}
} RFRecordSetting, *LPRFRecordSetting;

// �趨��¼������б�
typedef map<string, LPRFRecordSetting> MapRecordSettings;
typedef MapRecordSettings::iterator IterMapRecordSettings;
typedef MapRecordSettings::value_type ValueMapRecordSettings;


// ¼��״̬
typedef struct _RFRecordState
{
	int			nRecordId;			// ¼����
	int			nRecordIndex;		// ����ж��¼��ʱ�䣬�˴���¼�ڼ���ʱ���
	bool		bIsRecording;		// �Ƿ�������״̬
	string		strFile;			// ��ǰ¼������ƣ���Ϊ�����ļ����ܳ���4G�����Ե�¼��ʱ��ܳ�ʱ����Ҫ�з��ļ�
	int			nRecordSize;		// ��ǰ¼���ļ��Ĵ�С(��λ���ֽ�)
	int			nRecordTime;		// ��ǰ¼���ʱ��(��λ����)

	_RFRecordState()
	{
		nRecordId		= -1;
		bIsRecording	= false;
		nRecordIndex	= -1;
		strFile			= "";
		nRecordSize		= 0;
		nRecordTime		= 0;
	}			
} RFRecordState, * LpRFRecordState;

// ¼���е�¼��״̬��Ϣ
typedef map<int, LpRFRecordState> MapRecordState;
typedef MapRecordState::iterator IterMapRecordState;
typedef MapRecordState::value_type ValueMapRecordState;

// ��ѯ¼��ʱ��¼���ڱ��ص���Ϣ
typedef struct _QueryRecordResult
{
	int			nRecordId;				// ¼���¼���ʱ��id��ʶ
	int			nDeviceid;				// id��ʶ
	string		strRecordName;			// ¼���ļ�������
	string		strFolder;				// ¼���ļ����ڵ��ļ���·��
	int			nFileSize;				// ¼���ļ���С
	bool		bIsRecording;			// ¼���ļ��Ƿ���¼���У����¼���ڽ��У���ʱ��ȡ��¼���ļ������ݻ�ʧ�ܣ�
	string		tStart;					// ¼��ʼʱ��
	string		tEnd;					// ¼�����ʱ��
	string		strDVRName;				// ¼���ļ�����DVR�����Ʊ�ʶ
	string		strDVRAddr;				// ¼���ļ�����DVR��IP��ַ
	string		strDVRPort;				// ¼���ļ�����DVR�Ķ˿ں�
	string		strDVRChannel;			// ¼���ļ�����DVR��ͨ����
	string		strProvider;
}QueryRecordResult, *LpQueryRecordResult;

// ��ѯʱ���ص�¼���б�
typedef map<int, LpQueryRecordResult> MapQueryRecordResult;
typedef MapQueryRecordResult::iterator IterMapQueryRecordResult;
typedef MapQueryRecordResult::value_type ValueMapQueryRecordResult;


//---------------------------------------
// ��̨���Ʋ��Ե��趨
typedef struct _PTZControlPolicy
{
	int			nUserId;			// �ͻ��˵ı�ʶ
	TimePack	timeLastCtrl;		// ������ʱ��
	vector<int>	vetWaitingUsers;	// �ȴ��б�(�Ƿ�ʵ���Ŷ�֪ͨ����?)

} PTZControlPolicy, *LPPTZControlPolicy;

//---------------------------------------
// �豸������Ϣ
typedef struct _ConnectionInfo
{
	string	strRtspUrl;		// rtsp�����ַ���
	string	strIPAddr;		// IP
	int		nPort;			// �˿�
	int		nCnlId;			// �豸ͨ��

	int		nPlayerId;		// �ͻ���ʶ���
	string	strProvider;	// ����


	_ConnectionInfo()
	{
		strRtspUrl = "";
		strIPAddr = "";
		nPort = 37776;
		nCnlId = 1;
		nPlayerId = -1;
		strProvider = "DH";
	};
} ConnectionInfo, *LPConnectionInfo;


BOOL BuildWindow();
LRESULT WINAPI WndSubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //������Ϣ��Ӧ
LRESULT WINAPI WndSubClassProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //������Ϣ��Ӧ

void Ticks2Time(_int64 dwtime, CTime &time);
_int64 Time2Ticks(CTime time);

#define BLOCKSIZE_LIVE 8*1024
#define BLOCKSIZE_FILE 8*1024
#define BLOCKSIZE_UDP 15*1024

#define TIME_LOOP 30 


/*
	PicFormat_BMP = 0,
	PicFormat_JPEG,
	PicFormat_JPEG_70,
	PicFormat_JPEG_50,
	PicFormat_JPEG_30,
	PicFormat_JPEG_10,
	*/
#define ICON_FORMAT 1
#define ICON_EXTENSION "jpeg"
#define ICON_WIDTH 256
#define ICON_HEIGHT 256

#define RECORD_INTERVAL 30*60

#define ICON_HK_BUFFER_LEN 1024*1024

BOOL check_IP(const char* str);

#define VT_SEND_TIMEOUT 10000

void terminate_thread(HANDLE& thread, HANDLE hevent = NULL);

CString GetAppPath();

// �����Ƽ���̨��������
enum PTZCommand
{
    PTZ_UP = 0,						// ����
    PTZ_DOWN,						// ����
    PTZ_LEFT,						// ����
    PTZ_RIGHT,						// ����
    PTZ_ZOOM_ADD,					// �䱶+
    PTZ_ZOOM_DEC,					// �䱶-
    PTZ_FOCUS_ADD,					// ����+
    PTZ_FOCUS_DEC,					// ����-
    PTZ_APERTURE_ADD,				// ��Ȧ+
    PTZ_APERTURE_DEC,				// ��Ȧ-
    PTZ_POINT_MOVE,					// ת��Ԥ�õ�
    PTZ_POINT_SET,					// ����Ԥ�õ�
    PTZ_LAMP_OPEN,					// �򿪵ƹ�
    PTZ_LAMP_CLOSE,					// �رյƹ�
    PTZ_WIPER_OPEN,					// ����ˢ
    PTZ_WIPER_CLOSE,				// �ر���ˢ
    PTZ_THROUGHFOG_OPEN,			// ��͸��
    PTZ_THROUGHFOG_CLOSE,			// �ر�͸��
    PTZ_DEV_OPEN,					// �򿪸����豸
    PTZ_DEV_CLOSE,					// �رո����豸
    PTZ_LEFTUP,						// ����
    PTZ_RIGHTUP,					// ����
    PTZ_LEFTDOWN,					// ����
    PTZ_RIGHTDOWN,					// ����
	PTZ_MAX_COMMAND					// ���ֵ
};
